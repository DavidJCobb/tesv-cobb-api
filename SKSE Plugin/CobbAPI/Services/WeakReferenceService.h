#pragma once
#include <unordered_map>
#include <utility>
#include <vector>

#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"
#include "skse/GameTypes.h"
#include "skse/Serialization.h"

#include "Papyrus/_Prefixes.h"
#include "Utilities/ReferenceExists.h"

/*

   If you have a Papyrus variable or property pointing to a reference, Skyrim will 
   refuse to delete that reference until your variable or property is cleared. This 
   is meant to prevent the reference from being deleted out from under you. The 
   thing is, sometimes you WANT it to be deleted out from under you. In those cases, 
   you use the WeakReference API.

*/

class WeakReferenceService {
   private:
      SimpleLock lock;
      //
      struct WeakReferenceRecord {
         struct Entry {
            UInt32 formID = 0;
            UInt32 handle = 0;
            bool exists = true; // We need to be able to "remove" entries without actually rearranging the array. This bool lets us treat an entry as "removed."
            //
            Entry(UInt32 id) : formID(id) {};
            Entry(UInt32 id, UInt32 handle) : formID(id), handle(handle) {};
         };
         std::vector<Entry> handles;
         UInt32 temporaryFormCount = 0;
         //
         WeakReferenceRecord() {};
         WeakReferenceRecord(UInt32 dummy) {}; // needed because std::unordered_map::emplace sucks
         //
         void DeleteItem(UInt32 handle) {
            UInt32 count = this->handles.size();
            if (handle < 0 || handle >= count)
               return;
            {
               if ((this->handles[handle].formID & 0xFF000000) == 0xFF000000 && this->temporaryFormCount > 0)
                  this->temporaryFormCount--;
            }
            if (handle == count - 1) {
               this->handles.erase(this->handles.end() - 1);
               return;
            }
            this->handles[handle].exists = false;
            this->handles[handle].formID = 0;
            this->handles[handle].handle = 0;
         };
         void DeleteFormID(UInt32 formID) {
            UInt32 count = this->handles.size();
            bool found = false;
            for (UInt32 i = 0; i < count; i++) {
               if (this->handles[i].formID == formID) {
                  found = true;
                  if (i == count - 1) {
                     this->handles.erase(this->handles.end() - 1);
                     break;
                  }
                  this->handles[i].exists = false;
                  this->handles[i].formID = 0;
                  this->handles[i].handle = 0;
                  break;
               }
            }
            if (found && (formID & 0xFF000000) == 0xFF000000 && this->temporaryFormCount > 0)
               this->temporaryFormCount--;
         };
         UInt32 IndexOfForm(UInt32 formID) {
            UInt32 count = this->handles.size();
            for (UInt32 i = 0; i < count; i++)
               if (this->handles[i].formID == formID)
                  return i;
            return -1;
         };
         TESObjectREFR* GetReference(SInt32 handle) {
            if (handle < 0 || handle >= this->handles.size())
               return NULL;
            Entry entry = this->handles[handle];
            if (!entry.exists)
               return NULL;
            TESForm* form = LookupFormByID(entry.formID);
            if (!form) {
               UInt32 handle = entry.handle; // function may set the passed-in variable to null
               TESObjectREFR* reference = NULL;
               if (handle != *g_invalidRefHandle)
                  LookupREFRByHandle(&handle, &reference);
               if (reference)
                  return reference;
               //
               /*#ifdef _DEBUG
               try {
                  if (ReferenceWithIDExists(entry.formID)) {
                     _MESSAGE("Attempted to look up a weak reference to form ID 0x%08X. Got a false negative -- reference exists, but we can't access it.");
                  }
               } catch (const std::runtime_error& e) {};
               #endif*/
               return NULL;
            }
            return DYNAMIC_CAST(form, TESForm, TESObjectREFR);
         };
      };
      typedef std::unordered_map<UInt32, WeakReferenceRecord> WeakReferenceRecordMap;
      //
      WeakReferenceRecordMap records; // key is form ID
      //
      WeakReferenceRecord* GetRecord(UInt32 formID, bool create = true) {
         this->lock.Lock();
         if (!formID) {
            this->lock.Release();
            return NULL;
         }
         WeakReferenceRecord* record = NULL;
         if (!create) {
            try {
               record = &(this->records.at(formID));
            } catch (std::out_of_range) {};
         } else {
            auto result = this->records.emplace(formID, 0); // get or create
            WeakReferenceRecordMap::iterator iterator = result.first;
            record = &(iterator->second);
         }
         //
         this->lock.Release();
         return record;
      };
   public:
      static WeakReferenceService& GetInstance() {
         static WeakReferenceService instance;
         return instance;
      };
      //
      SInt32 AddReference(TESObjectREFR* holder, TESObjectREFR* target) {
         this->lock.Lock();
         WeakReferenceRecord* record = NULL;
         {
            if (holder)
               record = GetRecord(holder->formID);
            #ifdef _DEBUG
            if (!holder)
               _MESSAGE(PapyrusPrefixString(" WeakReferenceService") " was asked to create a new weak reference for a null holder. This will fail.");
            #endif
            if (record == NULL) {
               this->lock.Release();
               #ifdef _DEBUG
               _MESSAGE(PapyrusPrefixString(" WeakReferenceService") " failed to create a new weak reference. We were unable to add a new record.");
               #endif
               return -1;
            }
         }
         UInt32 formID = 0;
         {
            if (target)
               formID = target->formID;
            #ifdef _DEBUG
            if (!target)
               _MESSAGE(PapyrusPrefixString(" WeakReferenceService") " was asked to create a new weak reference to a null target. This will fail.");
            #endif
            SInt32 existing = -1;
            if (formID) {
               existing = record->IndexOfForm(formID);
            }
            if (!formID || existing >= 0) {
               this->lock.Release();
               return existing;
            }
            if ((formID & 0xFF000000) == 0xFF000000)
               record->temporaryFormCount++;
         }
         UInt32 handle = *g_invalidRefHandle;
         {
            if (target)
               handle = target->CreateRefHandle();
         }
         record->handles.push_back(WeakReferenceRecord::Entry(formID, handle));
         UInt32 result = record->handles.size() - 1;
         this->lock.Release();
         return result;
      };
      TESObjectREFR* GetReference(TESObjectREFR* holder, UInt32 handle) {
         this->lock.Lock();
         WeakReferenceRecord* record = NULL;
         {
            if (holder)
               record = GetRecord(holder->formID, false);
            if (!record) {
               this->lock.Release();
               return NULL;
            }
         }
         TESObjectREFR* result = record->GetReference(handle);
         this->lock.Release();
         return result;
      };
      void RemoveReference(TESObjectREFR* holder, TESObjectREFR* target) {
         this->lock.Lock();
         WeakReferenceRecord* record = NULL;
         UInt32 holderID = 0;
         {
            if (holder) {
               holderID = holder->formID;
               record = GetRecord(holder->formID);
            }
            if (!record) {
               this->lock.Release();
               return;
            }
         }
         SInt32 index = -1;
         {
            if (target)
               index = record->IndexOfForm(target->formID);
         }
         if (index >= 0) {
            record->DeleteItem(index);
            if (record->handles.size() == 0)
               this->records.erase(holderID);
         }
         this->lock.Release();
      };
      void RemoveReferenceByIndex(TESObjectREFR* holder, SInt32 index) {
         this->lock.Lock();
         WeakReferenceRecord* record = NULL;
         UInt32 holderID = 0;
         {
            if (holder) {
               holderID = holder->formID;
               record = GetRecord(holder->formID);
            }
            if (!record) {
               this->lock.Release();
               return;
            }
         }
         if (index >= 0) {
            record->DeleteItem(index);
            if (record->handles.size() == 0)
               this->records.erase(holderID);
         }
         this->lock.Release();
      };
      void RemoveAllReferences(TESObjectREFR* holder) {
         if (!holder)
            return;
         this->lock.Lock();
         this->records.erase(holder->formID);
         this->lock.Release();
      }
      //
      enum {
         kSaveVersion = 2
      };
      bool IsEmpty() {
         return this->records.empty();
      };
      void FormDeletedHandler(UInt32 formID) {
         if ((formID & 0xFF000000) != 0xFF000000)
            return;
         this->lock.Lock();
         std::vector<UInt32> recordsToDelete;
         for (auto it = this->records.begin(); it != this->records.end(); it++) {
            if (it->first == formID) {
               recordsToDelete.push_back(formID);
               continue;
            }
            WeakReferenceRecord& record = it->second;
            if (record.temporaryFormCount > 0) {
               record.DeleteFormID(formID);
               if (record.handles.size() == 0)
                  recordsToDelete.push_back(it->first); // we want to delete empty records, but doing it now would invalidate our iterator and break our loop
            }
         }
         {  // Delete any records that are now empty.
            UInt32 count = recordsToDelete.size();
            for (UInt32 i = 0; i < count; i++)
               this->records.erase(recordsToDelete[i]);
         }
         this->lock.Release();
      };
      bool NewGame() {
         this->lock.Lock();
         this->records.clear();
         this->lock.Release();
         return true;
      };
      bool Save(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         this->lock.Lock();
         {
            UInt32 count = this->records.size();
            SERIALIZATION_ASSERT(WriteData(intfc, &count), "Failed to write the record count.");
            for (auto it = this->records.begin(); it != this->records.end(); it++) {
               SERIALIZATION_ASSERT(WriteData(intfc, &it->first), "Failed to write a record's key.");
               WeakReferenceRecord& record = it->second;
               {
                  UInt32 count = record.handles.size();
                  {  // If there are multiple deleted entries at the end of the list, trim them.
                     UInt32 lastValid = 0;
                     for (UInt32 i = count - 1; i >= 0; i--) {
                        if (record.handles[i].exists) {
                           lastValid = i;
                           break;
                        }
                     }
                     if (lastValid < count - 1) {
                        record.handles.erase(record.handles.begin() + lastValid, record.handles.end());
                        count = record.handles.size();
                     }
                  }
                  SERIALIZATION_ASSERT(WriteData(intfc, &count), "Failed to write a record's entry count.");
                  for (UInt32 i = 0; i < count; i++) {
                     SERIALIZATION_ASSERT(WriteData(intfc, &record.handles[i].formID), "Failed to write a record entry's form ID.");
                     SERIALIZATION_ASSERT(WriteData(intfc, &record.handles[i].handle), "Failed to write a record entry's reference handle.");
                     SERIALIZATION_ASSERT(WriteData(intfc, &record.handles[i].exists), "Failed to write a record entry's status.");
                  }
               }
            }
         }
         this->lock.Release();
         return true;
      };
      bool Load(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         this->lock.Lock();
         this->records.clear();
         {
            UInt32 count = 0;
            SERIALIZATION_ASSERT(ReadData(intfc, &count), "Failed to read the record count.");
            for (UInt32 i = 0; i < count; i++) {
               WeakReferenceRecord newRecord;
               UInt32 key;
               SERIALIZATION_ASSERT(ReadData(intfc, &key), "Failed to read a record's key.");
               {  // Fix the form ID, if the load order changed.
                  UInt32 fixedFormID;
                  if (!intfc->ResolveFormId(key, &fixedFormID)) {
                     _MESSAGE("%s: Failed to correct a record's key (0x%08X). Skipping.", __FUNCTION__, key);
                     continue;
                  }
                  key = fixedFormID;
               }
               {
                  UInt32 entryCount = 0;
                  SERIALIZATION_ASSERT(ReadData(intfc, &entryCount), "Failed to read a record's entry count.");
                  newRecord.handles.reserve(entryCount);
                  for (UInt32 j = 0; j < entryCount; j++) {
                     WeakReferenceRecord::Entry newEntry(0);
                     SERIALIZATION_ASSERT(ReadData(intfc, &newEntry.formID), "Failed to read a record entry's form ID.");
                     //
                     if (version > 1) {
                        SERIALIZATION_ASSERT(ReadData(intfc, &newEntry.handle), "Failed to read a record entry's reference handle.");
                     } else {
                        _MESSAGE("%s: Found a weak reference from service version 1. Nothing should've been using this API back then.", __FUNCTION__);
                     };
                     //
                     SERIALIZATION_ASSERT(ReadData(intfc, &newEntry.exists), "Failed to read a record entry's status.");
                     //
                     {  // Fix the form ID, if the load order changed.
                        UInt32 fixedFormID;
                        if (!intfc->ResolveFormId(newEntry.formID, &fixedFormID)) {
                           _MESSAGE("%s: Failed to correct a record entry's form ID (0x%08X). Setting to non-existent/null.", __FUNCTION__, newEntry.formID);
                           fixedFormID = 0;
                        }
                        newEntry.formID = fixedFormID;
                        if (!fixedFormID)
                           newEntry.exists = false;
                     }
                     newRecord.handles.push_back(newEntry);
                     //
                     if ((newEntry.formID & 0xFF000000) == 0xFF000000)
                        newRecord.temporaryFormCount++;
                  }
               }
               #ifdef _DEBUG
               _MESSAGE("%s: Adding a WeakReferenceRecord for form ID 0x%08X.", __FUNCTION__, key);
               #endif
               std::pair<UInt32, WeakReferenceRecord> pair { key, newRecord };
               this->records.insert(pair);
            }
         }
         this->lock.Release();
         return true;
      };
      //
      #ifdef _DEBUG
      void DebugLogAllData() {
         this->lock.Lock();
         _MESSAGE("\n=== " PapyrusPrefixString(" WeakReferenceService") " is logging its contents...\n");
         for (auto it = this->records.begin(); it != this->records.end(); it++) {
            WeakReferenceRecord& record = it->second;
            UInt32 count = record.handles.size();
            _MESSAGE(" - RECORD BELONGING TO FORM ID 0x%08X: %d HANDLES (%d TAGGED AS TEMPORARY)", it->first, count, record.temporaryFormCount);
            for (UInt32 i = 0; i < count; i++) {
               bool exists = record.handles[i].exists;
               UInt32 formID = record.handles[i].formID;
               UInt32 handle = record.handles[i].handle;
               //
               char* status = "active";
               char* retrievable = "is";
               char* accessible = "is";
               {
                  if (!exists)
                     status = "cleared";
                  if (LookupFormByID(formID) == NULL)
                     retrievable = "is not";
                  if (!record.GetReference(i))
                     accessible = "is not";
               }
               //
               try {
                  if (ReferenceWithIDExists(formID))
                     _MESSAGE("    = %02d: Weak reference to form with ID 0x%08X and handle 0x%08X (%s). Form %s, %s retrievable, %s accessible.", i, formID, handle, status, "exists", retrievable, accessible);
                  else
                     _MESSAGE("    = %02d: Weak reference to form with ID 0x%08X and handle 0x%08X (%s). Form %s, %s retrievable, %s accessible.", i, formID, handle, status, "doesn't exist.", retrievable, accessible);
               } catch (const std::runtime_error& e) {
                  _MESSAGE("    = %02d: Weak reference to form with ID 0x%08X and handle 0x%08X (%s). Form unknown, %s retrievable, %s accessible.", i, formID, handle, status, retrievable, accessible);
               }
            };
         };
         _MESSAGE("\n=== " PapyrusPrefixString(" WeakReferenceService") " is done logging.\n");
         this->lock.Release();
      };
      #endif
};