#pragma once
#include <unordered_map>
#include <utility>

#include "skse/GameExtraData.h"
#include "skse/GameBSExtraData.h"
#include "skse/GameRTTI.h"

#include "ReverseEngineered/ExtraData.h"

//
// Usage:
//  
// registry->RegisterType<RE::ExtraLightData>();
// ...
// BSExtraData* created = registry->Create<RE::ExtraLightData>(); // calls RE::ExtraLightData::Create
//
class ExtraDataRegistry { // TODO: Forward-declare me in the H and move my definition to the CPP?
   private:
      struct Registration {
         void* constructor;
         UInt32 size;
         //
         Registration(void* constructor, UInt32 size) : constructor(constructor), size(size) {};
      };
      std::map<UInt8, Registration> constructors;
   public:
      static ExtraDataRegistry& GetInstance() {
         static ExtraDataRegistry instance;
         return instance;
      }
      //
      template <typename T>
      bool RegisterType() {
         this->constructors.emplace(T::type, &(T::Create), sizeof(T));
         return true;
      };
      //
      template <typename T>
      BSExtraData* Create() {
         try {
            T(*constructor)() = (T(*)()) this->constructors.at(T::type).constructor;
            BSExtraData* out = (BSExtraData*) constructor();
            return out;
         } catch (std::out_of_range) {
            return NULL;
         }
      };
      BSExtraData* Create(UInt8 type) {
         try {
            void*(*constructor)() = (void*(*)()) this->constructors.at(type).constructor;
            BSExtraData* out = (BSExtraData*)constructor();
            return out;
         } catch (std::out_of_range) {
            return NULL;
         }
      };
      std::pair<UInt32, BSExtraData*> Create(UInt8 type) {
         std::pair<UInt32, BSExtraData*> out = std::make_pair(0, NULL);
         try {
            Registration registration = this->constructors.at(type);
            out->first = registration.size;
            void*(*constructor)() = (void*(*)()) registration.constructor;
            out->second = (BSExtraData*)constructor();
         } catch (std::out_of_range) {}
         return out;
      };
      //
      UInt32 GetSize(UInt8 type) {
         try {
            return this->constructors.at(type).size;
         } catch (std::out_of_range) {
            return 0;
         }
      };
};

struct FormModifications {
   private:
      struct ExtraDataEntry {
         private:
            struct Mask {
               UInt32 byteCount = 0;
               UInt8* data = NULL; // dynamically allocated; one bit in this represents a byte in the BSExtraData*
               //
               Mask(UInt32 count) : byteCount(count) {
                  UInt32 bytesToBits = (count + 7) / 8; // divide n by d rounding up: (n + d - 1) / d
                  this->data = new UInt8[bytesToBits] { 0 };
               };
               ~Mask() {
                  delete[] this->data;
               };
               //
               bool ByteIsOverridden(UInt32 index) {
                  if (this->data && index >= byteCount)
                     return false;
                  return this->data[index / 8] & (1 << (index % 8));
               };
               void SetByte(UInt32 index) {
                  if (this->data && index < byteCount)
                     this->data[index / 8] |= (1 << (index % 8));
               };
               void ClearByte(UInt32 index) {
                  if (this->data && index < byteCount)
                     this->data[index / 8] &= ~(1 << (index % 8));
               };
               //
               bool operator!() {
                  UInt32 bytesToBits = (this->byteCount + 7) / 8;
                  for (UInt32 i = 0; i < bytesToBits; i++) {
                     if (this->data[i] != 0)
                        return true;
                  }
                  return false;
               };
            };
            //
            BSExtraData* data = NULL;
            std::unordered_map<UInt8, Mask> masks; // one per mod ID
            Mask* combinedMask = NULL;
            UInt32 cachedDataSize = 0;
         public:
            ExtraDataEntry(UInt8 type) {
               std::pair<UInt32, BSExtraData*> created = ExtraDataRegistry::GetInstance()->Create(type);
               this->data = created->second;
               this->cachedDataSize = created->first;
               this->combinedMask = new Mask(created->first);
            };
            //
            bool ClearField(UInt8 modId, UInt32 offset, UInt32 size) {
               if (!this->data)
                  return false;
               {  // Update mask for this mod ID.
                  try {
                     Mask* mask = &(this->masks.at(modId));
                     for (UInt32 i = 0; i < size; i++)
                        mask->ClearByte(offset + i);
                  } catch (std::out_of_range) {}
               }
               {  // Update combined mask.
                  for (UInt32 i = 0; i < size; i++)
                     this->combinedMask->ClearByte(offset + i);
               }
            };
            bool SetField(UInt8 modId, UInt32 offset, UInt32 size, UInt32 value) {
               if (!this->data)
                  return false;
               memcpy(this->data + offset, &value, size);
               {  // Update mask for this mod ID.
                  Mask* mask = NULL;
                  {  // Get or create mask for this mod ID.
                     try {
                        mask = &(this->masks.at(modId));
                     } catch (std::out_of_range) {
                        //this->masks.emplace(modId, this->cachedDataSize);
                        //mask = &(this->masks.at(modId));
                        auto result = this->masks.emplace(modId, this->cachedDataSize);
                        auto iterator = *result->first;
                        mask = &(iterator->second);
                     }
                  }
                  for (UInt32 i = 0; i < size; i++)
                     mask->SetByte(offset + i);
               }
               {  // Update combined mask.
                  for (UInt32 i = 0; i < size; i++)
                     this->combinedMask->SetByte(offset + i);
               }
            };
            //
            bool ApplyFields(BSExtraData* target) {
               if (!this->data || !this->combinedMask)
                  return false;
               if (!(*(this->combinedMask)))
                  return true;
               for (UInt32 i = 0; i < cachedDataSize; i++) {
                  UInt32 byte = this->combinedMask->data[i / 8];
                  for (UInt8 j = 0; j < 8; j++) { // we're copying byte-by-byte. is there a more efficient way to do this when contiguous bytes are "enabled?"
                     if (byte & (1 << j))
                        memcpy(target + i, this->data + i, 1); // modify byte on target
                  }
               }
               return true;
            };
      };
      //
      std::unordered_map<UInt8, ExtraDataEntry> overrides; // one per data type
      //
      ExtraDataEntry* GetEntry(UInt8 extraDataType) {
         try {
            ExtraDataEntry* entry = &(this->overrides.at(extraDataType));
            if (entry->data)
               return entry;
         } catch (std::out_of_range) {
            return NULL;
         }
      };
      ExtraDataEntry* GetOrCreateEntry(UInt8 extraDataType) {
         ExtraDataEntry* entry = NULL;
         try {
            entry = &(this->overrides.at(extraDataType));
         } catch (std::out_of_range) {
            //this->overrides.emplace(extraDataType);
            //entry = &(this->overrides.at(extraDataType));
            auto result = this->overrides.emplace(extraDataType);
            auto iterator = *result->first;
            entry = &(iterator->second);
         }
         if (!entry || !entry->data)
            return NULL;
         return entry;
      };
      //
   public:
      bool SetProperty(UInt8 modId, UInt8 extraDataType, UInt32 offset, UInt32 size, UInt32 value) {
         ExtraDataEntry* entry = this->GetOrCreateEntry(extraDataType);
         if (!entry)
            return false;
         entry->SetField(modId, offset, size, value);
         return true;
      };
      bool ResetProperty(UInt8 modId, UInt8 extraDataType, UInt32 offset, UInt32 size) {
         ExtraDataEntry* entry = this->GetEntry(extraDataType);
         if (entry)
            entry->ClearField(modId, offset, size);
         return true;
      };
      //
      bool ApplyProperties(BaseExtraList* extraList) {
         for (auto& item : this->overrides) {
            UInt8 type = item->first;
            ExtraDataEntry* entry = &(item->second);
            if (!entry->data)
               continue;
            BSExtraData* target = NULL;
            {  // TODO: Get the target from the extraList, or create it (and add it to that list) if it does not exist.
            }
            if (target) {
               if (!entry->ApplyFields(target))
                  return false;
            }
         };
         return true;
      };
};
class ExtraDataModificationService {
   private:
      std::unordered_map<UInt32, FormModifications> forms;
      //
      FormModifications* GetOrCreateModifications(UInt32 formID) {
         FormModifications* form = NULL;
         try {
            form = &(this->forms.at(formID));
         } catch (std::out_of_range) {
            //this->forms.emplace(formID); // returns an iterator; todo: learn how to use that; it'd be faster than just re-looping as the next line does
            //form = &(this->forms.at(formID));
            auto result = this->forms.emplace(formID);
            auto iterator = *result->first;
            form = &(iterator->second);
         }
         return form;
      };
   public:
      static ExtraDataModificationService& GetInstance() {
         static ExtraDataModificationService instance;
         return instance;
      };
      //
      bool ModifyValue(UInt8 modId, UInt32 formID, UInt8 extraDataType, UInt32 memberOffset, UInt32 memberSize, UInt32 memberValue) {
         //
         // TODO: FINISH ME
         //
      };
};

// Intended usage: ModifyReadOnlyExtraData(modId, formID, ExtraLightData, modifiedFade, 5.0);
#define ModifyReadOnlyExtraData(UInt8 modId, UInt32 formID, extraDataClassName, memberName, value) \
   {\
      ExtraDataModificationService* service = ExtraDataModificationService::GetInstance(); \
      service->ModifyValue(modId, formID, extraDataClassName::type, offsetof(extraDataClassName, memberName), sizeof(extraDataClassName::memberName), reinterpret_cast<UInt32>(value)); \
   }

// --------------------- OLD CODE BELOW ------------------------

class ModifiedExtraData {
   private:
      struct MemberData {
         MemberData(UInt32 size) : size(size) {
            if (this->size > sizeof(MemberData::priorValue))
               throw;
         };
         //
         UInt8 size;
         UInt32 value;
         UInt32 priorValue = 0; // value in vanilla extra-data, i.e. extra-data not modified with this system
         bool priorValueStored = false; // if (false), then we should store an ExtraData's value before overwriting it. if (true), we already did that.
      };
      //
      UInt8 type;
      std::unordered_map<UInt32, MemberData> modifiedMembers;
      //
      MemberData* GetModification(UInt32 offset, UInt8 size = 0) {
         try {
            MemberData* member = &(this->modifiedMembers.at(offset));
            if (size && member->size != size)
               throw new std::out_of_range("");
            return member;
         } catch (std::out_of_range) {
            return NULL;
         }
      };
   public:
      bool ApplyModifications(BSExtraData* target) {
         if (!target)
            return false;
         for (auto iterator = this->modifiedMembers.begin(); iterator != this->modifiedMembers.end(); ++iterator) {
            UInt32 offset = iterator->first;
            MemberData data = iterator->second;
            if (!data.priorValueStored) {
               memcpy(&(iterator->second.priorValue) + (sizeof(data.priorValue) - data.size), target + offset, data.size);
               data.priorValueStored = true;
            }
            memcpy(target + offset, &(data.value), data.size);
         }
         return true;
      };
      bool SetModification(UInt32 offset, UInt8 size, UInt32 value) {
         try {
            MemberData* member = this->GetModification(offset, size);
            if (!member) {
               this->modifiedMembers.emplace(size); // returns an iterator; todo: learn how to use that; it'd be faster than just re-looping as the next line does
               MemberData* member = this->GetModification(offset, size);
            }
            member->value = value;
            return true;
         } catch (std::out_of_range) {
            return false;
         }
      };
      bool RemoveModification(UInt32 offset, BSExtraData* target = NULL) {
         MemberData* data = this->GetModification(offset);
         if (!data)
            return true;
         MemberData copy = *data;
         bool result = (bool)this->modifiedMembers.erase(offset);
         delete data;
         if (result && target != NULL && copy.priorValueStored) {
            memcpy(target + offset, &(copy.priorValue) + (sizeof(copy.priorValue) - copy.size), copy.size);
         }
         return result;
      };
};

class CobbExtraDataService {
   private:
      struct Record {
         Record(UInt32 formID) : formID(formID) {};
         //
         UInt32 formID = 0;
         std::unordered_map<UInt8, ModifiedExtraData> extraData;
         //
         bool ApplyExtraData(UInt8 type) {
         };
         ModifiedExtraData* GetExtraData(UInt8 type) {
            try {
               return &(this->extraData.at(type));
            } catch (std::out_of_range) {
               return NULL;
            }
         };
         ModifiedExtraData* GetOrCreateExtraData(UInt8 type, bool applyNow = false) {
            ModifiedExtraData* out = this->GetExtraData(type);
            if (!out) {
               this->extraData.emplace(type); // returns an iterator; todo: learn how to use that; it'd be faster than just re-looping as the next line does
               out = &(this->extraData.at(type));
            }
            return out;
         };
         //
         BSExtraData* GetOrCreateFormExtraData(UInt8 type) {
            BSExtraData* out = NULL;
            TESForm* form = LookupFormByID(this->formID);
            if (!form)
               return out;
            switch (form->formType) { // there's gotta be a built-in subroutine somewhere, to get the extraData from an arbitrary form regardless of type...
               case FormType::kFormType_Reference:
                  {
                     TESObjectREFR* formConverted = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
                     if (!formConverted)
                        return out;
                     out = formConverted->extraData.GetByType(type);
                     if (!out) {
                        //
                        // TODO: create the data
                        //
                     }
                     return out;
                  }
                  break;
            }
         };
      };
      //
      std::unordered_map<UInt32, Record> extraDataRecords;
      //
      Record* GetOrCreateRecord(UInt32 formID) {
         Record* record = NULL;
         try {
            record = &(this->extraDataRecords.at(formID));
         } catch (std::out_of_range) {
            this->extraDataRecords.emplace(formID); // returns an iterator; todo: learn how to use that; it'd be faster than just re-looping as the next line does
            record = &(this->extraDataRecords.at(formID));
         }
         return record;
      };
   public:
      static CobbExtraDataService* GetSingleton();
      //
      void ModifyValue(UInt32 formID, UInt8 extraDataType, UInt32 memberOffset, UInt32 memberSize, UInt32 memberValue) {
         Record* record = GetOrCreateRecord(formID);
         if (!record)
            return;
         ModifiedExtraData* extraData = record->GetOrCreateExtraData(extraDataType);
         if (!extraData)
            return;
         if (extraData->SetModification(memberOffset, memberSize, memberValue)) {
            BSExtraData* target = record->GetOrCreateFormExtraData(extraDataType);
            if (target)
               extraData->ApplyModifications(target);
         }
      };
};