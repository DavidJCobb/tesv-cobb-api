#pragma once
#include <mutex>
#include <unordered_map>
#include <utility>

#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"
#include "skse/GameTypes.h"
#include "skse/NiTypes.h"
#include "skse/Serialization.h"

#include "ReverseEngineered/ExtraData.h"
#include "CobbRotation.h"

#include "Papyrus/_Prefixes.h"
#include "Services/Messaging.h"

class TeleportMarkerService {
   public:
      enum { kSaveVersion = 2 };
      static TeleportMarkerService& GetInstance() {
         static TeleportMarkerService instance;
         return instance;
      };
      //
      struct Coordinates {
         NiPoint3 pos;
         NiPoint3 rot;
      };
      struct Record {
         Coordinates original;
         Coordinates modified;
         //
         Record() {};
         Record(UInt32 dummy) {}; // std::unordered_map::emplace can't create instances unless the constructor takes an argument, which is dumb
         Record(NiPoint3* pos, NiPoint3* rot) {
            this->original.pos = *pos;
            this->original.rot = *rot;
         };
      };
      //
   private:
      std::recursive_mutex lock;
      //
      typedef UInt32 FormID;
      typedef std::unordered_map<FormID, Record> RecordMap;
      struct {
         RecordMap owned;
         RecordMap foreign;
      } records;
      //
   public:
      inline bool IsEmpty() {
         return this->records.owned.empty();
      };
      bool GetMarkerOriginalCoordinates(TESObjectREFR* door, Coordinates* out); // effectively const, except that it uses our lock
      bool GetMarkerModifiedCoordinates(TESObjectREFR* door, Coordinates* out); // effectively const, except that it uses our lock
      bool MoveMarker(TESObjectREFR* door, NiPoint3 pos, NiPoint3 rot);
      bool MoveMarkerRelativeTo(TESObjectREFR* door, const NiPoint3& offsetFromPos, const NiPoint3& offsetFromRot, const NiPoint3& offsetToPos, const NiPoint3& offsetToRot);
      bool MoveMarkerToRelativeEditorLocOffset(TESObjectREFR* door, TESObjectREFR* destination = nullptr);
      void ResetMarker(TESObjectREFR* door);
      void ResetAllMarkers();
      //
      // Handlers:
      //
      bool QuitGame();
      bool NewGame();
      bool Save(SKSESerializationInterface* intfc);
      bool EmptyLoad();
      bool Load(SKSESerializationInterface* intfc, UInt32 version);
      //
      void OnMessage(CobbAPIMessage*);

};
class TeleportMarkerMessage : public CobbAPIMessage { // move to RevealService.h?
   public:
      static constexpr UInt32 typeCode = 'TELE';
      enum Subtype : UInt32 {
         kSubtype_DoorModified = 'DSET',
         kSubtype_DoorReset    = 'DRVT',
      };
   protected:
      Subtype _subtype;
      TeleportMarkerMessage(Subtype t) : CobbAPIMessage(typeCode), _subtype(t) {};
   public:
      inline const decltype(_subtype) subtype() { return this->_subtype; };
      //
      inline static TeleportMarkerMessage* convert(CobbAPIMessage* m) {
         if (m->type() == typeCode)
            return (TeleportMarkerMessage*) m;
         return nullptr;
      };
};
class TeleportMarkerDoorModifiedMessage : public TeleportMarkerMessage {
   public:
      TeleportMarkerDoorModifiedMessage(const TeleportMarkerService::Record& record) : TeleportMarkerMessage(kSubtype_DoorModified), record(record) {};
      //
      UInt32 doorFormID;
      const TeleportMarkerService::Record record;
};
class TeleportMarkerDoorResetMessage : public TeleportMarkerMessage {
   public:
      TeleportMarkerDoorResetMessage() : TeleportMarkerMessage(kSubtype_DoorReset) {};
      //
      UInt32 doorFormID;
};