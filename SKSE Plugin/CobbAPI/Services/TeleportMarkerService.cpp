#include "TeleportMarkerService.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "shared.h"

RE::ExtraTeleport::TeleportData* _GetTeleportData(TESObjectREFR* door) {
   if (!door)
      return nullptr;
   return CALL_MEMBER_FN(((RE::BaseExtraList*)&(door->extraData)), GetExtraTeleportData)();
};
//
bool TeleportMarkerService::GetMarkerOriginalCoordinates(TESObjectREFR* door, Coordinates* out) {
   if (!door || !out)
      return false;
   {  // Get the marker's original position, whether we've changed it or not.
      std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
      try {
         auto& r = this->records.owned.at(door->formID);
         out->pos = r.original.pos;
         out->rot = r.original.rot;
      } catch (std::out_of_range) {
         try {
            auto& r = this->records.foreign.at(door->formID);
            out->pos = r.original.pos;
            out->rot = r.original.rot;
         } catch (std::out_of_range) {
            auto data = _GetTeleportData(door);
            if (!data)
               return false;
            out->pos = data->markerPosition;
            out->rot = data->markerRotation;
         }
      }
   }
   return true;
};
bool TeleportMarkerService::GetMarkerModifiedCoordinates(TESObjectREFR* door, Coordinates* out) {
   if (!door || !out)
      return false;
   {
      std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
      try {
         auto& r = this->records.owned.at(door->formID);
         out->pos = r.modified.pos;
         out->rot = r.modified.rot;
      } catch (std::out_of_range) {
         try {
            auto& r = this->records.foreign.at(door->formID);
            out->pos = r.modified.pos;
            out->rot = r.modified.rot;
         } catch (std::out_of_range) {
            return false;
         }
      }
   }
   return true;
};
bool TeleportMarkerService::MoveMarker(TESObjectREFR* door, NiPoint3 pos, NiPoint3 rot) {
   if (!door)
      return false;
   FormID formID = door->formID;
   if (formID >> 0x18 == 0xFF) { // Reject temporary forms.
      DEBUG_ONLY_MESSAGE("TeleportMarkerService::MoveMarker rejected an attempt to move the marker of a temporary door, 0x%08X.", formID);
      return false;
   }
   auto data = _GetTeleportData(door);
   if (!data) {
      _MESSAGE("TeleportMarkerService::MoveMarker was unable to get the marker data for a door, 0x%08X.", formID);
      return false;
   }
   {
      std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
      Record record;
      try {
         auto& r = this->records.owned.at(formID);
         r.modified.pos = pos;
         r.modified.rot = rot;
         record = r;
      } catch (std::out_of_range) {
         try {
            auto& f = this->records.foreign.at(formID);
            record.original.pos = f.original.pos;
            record.original.rot = f.original.rot;
         } catch (std::out_of_range) {
            record.original.pos = data->markerPosition;
            record.original.rot = data->markerRotation;
         }
         record.modified.pos = pos;
         record.modified.rot = rot;
         this->records.owned[formID] = record;
      }
      data->markerPosition = pos;
      data->markerRotation = rot;
      {  // Synchronize with other DLLs
         TeleportMarkerDoorModifiedMessage m(record);
         m.doorFormID = formID;
         m.send();
         //
         this->records.foreign.erase(formID);
      }
      //_MESSAGE("[TeleportMarkerService] Moved teleport marker for %08X to position (%f, %f, %f) and radian rotation (%f, %f, %f).", formID, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z);
   }
   return true;
};
bool TeleportMarkerService::MoveMarkerRelativeTo(TESObjectREFR* door, const NiPoint3& offsetFromPos, const NiPoint3& offsetFromRot, const NiPoint3& offsetToPos, const NiPoint3& offsetToRot) {
   if (!door)
      return false;
   if (door->formID >> 0x18 == 0xFF)
      return false;
   auto data = _GetTeleportData(door);
   if (!data)
      return false;
   Cobb::Coordinates desired;
   {
      Cobb::GetRelativeCoordinates(desired, offsetFromPos, offsetFromRot, data->markerPosition, data->markerRotation, true, true);
      NiPoint3 rotation = (NiPoint3)desired.rot;
      Cobb::ApplyRelativeCoordinates(desired, offsetToPos, offsetToRot, desired.pos, rotation, true, desired.rot.isRadians);
   }
   desired.rot.ConvertToRadians();
   return this->MoveMarker(door, desired.pos, (NiPoint3)desired.rot);
};
bool TeleportMarkerService::MoveMarkerToRelativeEditorLocOffset(TESObjectREFR* door, TESObjectREFR* aDest) {
   if (!door)
      return false;
   if (door->formID >> 0x18 == 0xFF) {
      DEBUG_ONLY_MESSAGE("%s rejected an attempt to move the marker of a temporary door, 0x%08X.", __FUNCTION__, door->formID);
      return false;
   }
   auto data = _GetTeleportData(door);
   if (!data) {
      DEBUG_ONLY_MESSAGE("%s was unable to get the teleport data for a door.", __FUNCTION__);
      return false;
   }
   RE::refr_ptr destination = (RE::TESObjectREFR*) aDest;
   if (!destination) { // Get the destination door. (Callers that already have it can optimize by passing it in.)
      destination.set_from_handle(data->refHandle);
      if (!destination) {
         DEBUG_ONLY_MESSAGE("%s was unable to get the destination for a door.", __FUNCTION__);
         return false;
      }
   }
   Cobb::Coordinates desired;
   {  // Get the coordinates to move the marker to.
      RE::NiPoint3 doorEditorPos;
      RE::NiPoint3 doorEditorRot;
      {  // Get the editor position of the destination door.
         void* dummy;
         if (!((RE::TESObjectREFR*)destination)->GetEditorCoordinates(&doorEditorPos, &doorEditorRot, &dummy, nullptr)) {
            if (destination->formID >> 0x18 != 0xFF) {
               doorEditorPos = destination->pos;
               doorEditorRot = destination->rot;
            } else {
               DEBUG_ONLY_MESSAGE("%s was unable to get the editor coordinates for a door's destination.", __FUNCTION__);
               return false;
            }
         }
      }
      NiPoint3 markerOriginalPos;
      NiPoint3 markerOriginalRot;
      {  // Get the marker's original position, whether we've changed it or not.
         std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
         try {
            auto& r = this->records.owned.at(door->formID);
            markerOriginalPos = r.original.pos;
            markerOriginalRot = r.original.rot;
         } catch (std::out_of_range) {
            try {
               auto& r = this->records.foreign.at(door->formID);
               markerOriginalPos = r.original.pos;
               markerOriginalRot = r.original.rot;
            } catch (std::out_of_range) {
               markerOriginalPos = data->markerPosition;
               markerOriginalRot = data->markerRotation;
            }
         }
      }
      Cobb::GetRelativeCoordinates(desired, doorEditorPos, doorEditorRot, markerOriginalPos, markerOriginalRot, true, true);
      NiPoint3 rotation = (NiPoint3)desired.rot;
      Cobb::ApplyRelativeCoordinates(desired, destination->pos, destination->rot, desired.pos, rotation, true, desired.rot.isRadians);
   }
   destination->handleRefObject.DecRef(); // LookupREFRByHandle incremented the refcount
   desired.rot.ConvertToRadians();
   return this->MoveMarker(door, desired.pos, (NiPoint3)desired.rot);
};
void TeleportMarkerService::ResetMarker(TESObjectREFR* door) {
   if (!door)
      return;
   auto data = _GetTeleportData(door);
   if (!data)
      return;
   FormID formID = door->formID;
   {
      std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
      //
      auto it = this->records.owned.find(formID);
      if (it != this->records.owned.end()) {
         data->markerPosition = it->second.original.pos;
         data->markerRotation = it->second.original.rot;
         this->records.owned.erase(it);
      }
      TeleportMarkerDoorResetMessage m;
      m.doorFormID = formID;
      m.send();
      //
      this->records.foreign.erase(formID);
   }
};
void TeleportMarkerService::ResetAllMarkers() {
   std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
   {
      //_MESSAGE("[TeleportMarkerService] Discarding %d owned records...", this->records.owned.size());
      if (!this->records.owned.empty()) {
         for (auto it = this->records.owned.begin(); it != this->records.owned.end(); it++) {
            FormID  formID = it->first;
            Record& record = it->second;
            TESObjectREFR* door = DYNAMIC_CAST(LookupFormByID(formID), TESForm, TESObjectREFR);
            if (door) {
               RE::ExtraTeleport::TeleportData* data = _GetTeleportData(door);
               if (data) {
                  data->markerPosition = record.original.pos;
                  data->markerRotation = record.original.rot;
               }
            } else {
               _MESSAGE("[TeleportMarkerService][ResetAllMarkers] Warning: form ID 0x%08X couldn't be obtained and reset.", formID);
            }
         }
         this->records.owned.clear();
      }
      //_MESSAGE("[TeleportMarkerService] Discarding %d foreign records...", this->records.foreign.size());
      this->records.foreign.clear();
   }
};
//
bool TeleportMarkerService::QuitGame() {
   //_MESSAGE("[TeleportMarkerService] Event: QuitGame.");
   this->ResetAllMarkers();
   return true;
};
bool TeleportMarkerService::NewGame() {
   //_MESSAGE("[TeleportMarkerService] Event: NewGame.");
   this->ResetAllMarkers();
   return true;
};
bool TeleportMarkerService::Save(SKSESerializationInterface* intfc) {
   using namespace Serialization;
   std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
   _MESSAGE("TeleportMarkerService is responding to an event: the player is saving.");
   {
      UInt32 count = this->records.owned.size();
      SERIALIZATION_ASSERT(WriteData(intfc, &count), "Failed to write the record count.");
      for (auto it = this->records.owned.begin(); it != this->records.owned.end(); it++) {
         SERIALIZATION_ASSERT(WriteData(intfc, &it->first), "Failed to write a record's key.");
         Record& record = it->second;
         SERIALIZATION_ASSERT(WriteData(intfc, &record.original.pos), "Failed to write a record's original position.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.original.rot), "Failed to write a record's original rotation.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.modified.pos), "Failed to write a record's modified position.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.modified.rot), "Failed to write a record's modified rotation.");
      }
   }
   return true;
};
bool TeleportMarkerService::EmptyLoad() {
   //_MESSAGE("[TeleportMarkerService] Event: EmptyLoad (loadgame with no teleport data).");
   this->ResetAllMarkers();
   return true;
};
bool TeleportMarkerService::Load(SKSESerializationInterface* intfc, UInt32 version) {
   using namespace Serialization;
   std::lock_guard<decltype(this->lock)> scopedLock(this->lock);
   //_MESSAGE("[TeleportMarkerService] Event: Load.");
   this->ResetAllMarkers();
   {
      UInt32 count = 0;
      SERIALIZATION_ASSERT(ReadData(intfc, &count), "Failed to read the record count.");
      for (UInt32 i = 0; i < count; i++) {
         Record newRecord;
         UInt32 key;
         SERIALIZATION_ASSERT(ReadData(intfc, &key), "Failed to read a record's key.");
         SERIALIZATION_ASSERT(ReadData(intfc, &newRecord.original.pos), "Failed to read a record's original position.");
         SERIALIZATION_ASSERT(ReadData(intfc, &newRecord.original.rot), "Failed to read a record's original rotation.");
         SERIALIZATION_ASSERT(ReadData(intfc, &newRecord.modified.pos), "Failed to read a record's modified position.");
         SERIALIZATION_ASSERT(ReadData(intfc, &newRecord.modified.rot), "Failed to read a record's modified rotation.");
         {  // Fix the form ID, if the load order changed.
            UInt32 fixedFormID;
            if (!intfc->ResolveFormId(key, &fixedFormID)) {
               _MESSAGE("%s: Failed to correct a record's key (0x%08X). Skipping.", __FUNCTION__, key);
               continue;
            }
            key = fixedFormID;
         }
         {  // Apply the marker data.
            TESObjectREFR* door = DYNAMIC_CAST(LookupFormByID(key), TESForm, TESObjectREFR);
            if (door) {
               RE::ExtraTeleport::TeleportData* data = _GetTeleportData(door);
               if (data) {
                  data->markerPosition = newRecord.modified.pos;
                  data->markerRotation = newRecord.modified.rot;
                  DEBUG_ONLY_MESSAGE(PapyrusPrefixString("TeleportMarkerService") " modified the teleport marker for %08X. Position is now (%f, %f, %f).", door->formID, data->markerPosition.x, data->markerPosition.y, data->markerPosition.z);
               } else {
                  _MESSAGE(PapyrusPrefixString("TeleportMarkerService") " encountered a problem while loading markers: form ID 0x%08X was available, but its teleport data couldn't be obtained and modified.", key);
               }
            } else {
               _MESSAGE(PapyrusPrefixString("TeleportMarkerService") " encountered a problem while loading markers: form ID 0x%08X couldn't be obtained and modified.", key);
            }
         }
         DEBUG_ONLY_MESSAGE("%s: Adding a Record for form ID 0x%08X.", __FUNCTION__, key);
         this->records.owned[key] = newRecord;
      }
   }
   return true;
};
void TeleportMarkerService::OnMessage(CobbAPIMessage* m) {
   std::lock_guard<std::recursive_mutex> scopedLock(this->lock); // scoped lock
   //
   TeleportMarkerMessage* message = TeleportMarkerMessage::convert(m);
   if (!message)
      return;
   _MESSAGE("TeleportMarkerService received message of type %08X.", message->subtype());
   switch (message->subtype()) {
      case TeleportMarkerMessage::kSubtype_DoorModified:
         {
            auto   casted = (TeleportMarkerDoorModifiedMessage*) message;
            UInt32 formID = casted->doorFormID;
            this->records.owned.erase(formID);
            this->records.foreign[formID] = casted->record;
         }
         break;
      case TeleportMarkerMessage::kSubtype_DoorReset:
         {
            auto   casted = (TeleportMarkerDoorResetMessage*) message;
            UInt32 formID = casted->doorFormID;
            this->records.foreign.erase(formID);
            auto it = this->records.owned.find(formID);
            if (it != this->records.owned.end()) {
               auto door = (TESObjectREFR*) LookupFormByID(formID);
               if (door) {
                  auto data = _GetTeleportData(door);
                  if (data) {
                     data->markerPosition = it->second.original.pos;
                     data->markerRotation = it->second.original.rot;
                  }
               }
               this->records.owned.erase(it);
            }
         }
         break;
   };
};