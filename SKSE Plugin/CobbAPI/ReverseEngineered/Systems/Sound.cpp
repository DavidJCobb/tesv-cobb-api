#include "Sound.h"
#include "skse/GameForms.h"

namespace RE {
   void FadeToAcousticSpace(BGSAcousticSpace* space, bool forInterior) { // could be a "fade out" method?
      //
      // Reverse-engineered from CellAcousticSpaceListener::ReceiveEvent (virtual method 01). 
      // CellAcousticSpaceListener is an "event sink," and receives a BGSActorCellEvent; 
      // refer to himika's work on that if the SKSE source is not sufficient.
      //
      void(*FadeToAcousticSpaceInternal)(BGSAcousticSpace*) = (void(*)(BGSAcousticSpace*))0x00671500;
      UInt32*            piUnkState = (UInt32*)(0x01B2E2D0);
      BGSAcousticSpace** pkCurrent  = (BGSAcousticSpace**)(0x01B2E2CC);
      //
      if (space == *pkCurrent)
         return;
      if (space) {
         if ((*piUnkState == 2 && forInterior) || (*piUnkState == 1 && !forInterior)) {
            FadeToAcousticSpaceInternal(nullptr);
         }
         *pkCurrent = space;
         *piUnkState = 0;
         return;
      }
      auto last = *pkCurrent;
      *pkCurrent = nullptr;
      FadeToAcousticSpaceInternal(last);
      *piUnkState = forInterior ? 1 : 2;
   };
   void SetCellMusic(BGSMusicType* music) {
      struct DispatcherShim { // SKSE didn't make the damn method public
         DEFINE_MEMBER_FN_LONG(DispatcherShim, SendEvent_Internal, void, 0x006EBC10, BGSPlayerMusicChanger::EventArg*);
      };
      //
      auto changer  = BGSPlayerMusicChanger::GetInstance();
      auto previous = changer->currentMusic;
      if (previous == music)
         return;
      auto dispatcher = (DispatcherShim*)&changer->dispatcher;
      if (previous) {
         BGSPlayerMusicChanger::EventArg ev;
         ev.unk00 = &previous->type;
         ev.unk04 = 1;
         CALL_MEMBER_FN(dispatcher, SendEvent_Internal)(&ev);
      }
      if (music) {
         BGSPlayerMusicChanger::EventArg ev;
         ev.unk00 = &music->type;
         ev.unk04 = 0;
         CALL_MEMBER_FN(dispatcher, SendEvent_Internal)(&ev);
      }
      changer->currentMusic = music;
   };
};