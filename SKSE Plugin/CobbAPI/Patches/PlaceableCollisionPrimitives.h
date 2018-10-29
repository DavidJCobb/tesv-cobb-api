#pragma once
#include <vector>
class BGSKeyword;
class TESObjectACTI;

extern const UInt32 g_PlaceableCollisionPrimitiveFormID;

#define COBB_PLACEABLE_NAVCUT_USES_MANAGER 1

namespace CobbPatches {
   namespace PlaceableCollisionPrimitives {
      //
      // This patch is intended to allow you to make placeable, non-static collision 
      // primitives -- particularly NAVCUT. The original plan was to use an injected 
      // base form with form ID 0x00C0BB00, and to patch the game to check for this 
      // in addition to checking for the hardcoded CollisionMarker form. However, 
      // references with injected base forms are not maintained in memory stably and 
      // are not written to the savegame; they're prone to disappearing out from 
      // under the game engine the instant they're unloaded.
      //
      // The new approach is to treat any activator with an injected keyword as a 
      // potential collision volume.
      //
      struct Manager {
         public:
            std::vector<BGSKeyword*>    keywords;   // used to find the activators; any keyword whose EDID starts with "CobbHardcodedPlaceableNavcut_"
            std::vector<TESObjectACTI*> activators; // forms suitable for use as bases for NAVCUT volumes

            inline static Manager& GetInstance() {
               static Manager instance;
               return instance;
            }

            void Load();
            bool Matches(TESObjectACTI* form) const;
      };

      void OnFormsLoaded();
      void Apply();
   }
};