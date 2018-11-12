#pragma once

namespace RE {
   class TESObjectREFR;
};
namespace CobbPatches {
   namespace OnLeveledActorRegenerated {
      void _stdcall Handler(RE::TESObjectREFR* actor); // EDIT ME
      void Apply();
   };
};