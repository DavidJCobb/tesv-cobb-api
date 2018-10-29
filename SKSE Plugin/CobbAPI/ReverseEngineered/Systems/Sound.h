#pragma once
#include "skse/GameEvents.h"

class BGSAcousticSpace;
class BGSMusicType;
class BSIMusicType;
class PlayerCharacter;
class TESObjectREFR;
namespace RE {
   void FadeToAcousticSpace(BGSAcousticSpace*, bool forInterior = true);
   void SetCellMusic(BGSMusicType*);

   struct BGSPlayerMusicChanger { // subclass of class BSTEventSink<...>
      public:
         inline static BGSPlayerMusicChanger* GetInstance() {
            return (BGSPlayerMusicChanger*) 0x01B2E328;
         };
         typedef UInt32 Unknown;
         struct Event {
            UInt32 refHandle;  // 00 // the player, generally
            UInt32 cellFormID; // 04
            UInt32 unk08;      // 08
            //
            // ...?
         };
         struct EventArg {
            BSIMusicType* unk00;
            UInt32        unk04; // 1 to stop music; 0 to start music
         };
         //
         virtual ~BGSPlayerMusicChanger();
         virtual EventResult ReceiveEvent(UInt32 refHandle, EventDispatcher<Event>); // virtual	EventResult ReceiveEvent(T * evn, EventDispatcher<T> * dispatcher);

         void*  unk04; // vtbl; possibly interface
         EventDispatcher<Event, Unknown> dispatcher; // 08
         PlayerCharacter* currentFocus; // 38
         BGSMusicType*    currentMusic; // 3C

         // TESObjectREFR* unk38
         // BGSMusicType* unk3C
   };
};