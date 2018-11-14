#pragma once
#include <mutex>
#include <unordered_map>
#include "ReverseEngineered/Forms/TESObjectCELL.h"

class CellInteriorDataService {
   public:
      CellInteriorDataService() {
         this->_defaults.reserve(900); // prep for number of interior cells generally present in vanilla, assuming all DLC
      };
      CellInteriorDataService(const CellInteriorDataService &other) = delete; // no copy-assign
      static CellInteriorDataService& GetInstance() {
         static CellInteriorDataService instance;
         return instance;
      };
   private:
      typedef UInt32 FormID;
      typedef RE::TESObjectCELL::InteriorData CellData;
   public:
      enum ChangeFlags : UInt32 {
         kChanged_Ambient      = 0x00000001,
         kChanged_Directional  = 0x00000002,
         kChanged_FogColorNear = 0x00000004,
         kChanged_FogColorFar  = 0x00000008,
         kChanged_FogPlaneNear = 0x00000010,
         kChanged_FogPlaneFar  = 0x00000020,
         kChanged_FogPlaneClip = 0x00000040,
         kChanged_FogPower     = 0x00000080,
         kChanged_FogMax       = 0x00000100,
         kChanged_DirectionalRotationXY  = 0x00000200,
         kChanged_DirectionalRotationZ   = 0x00000400,
         kChanged_DirectionalFade        = 0x00000800,
         kChanged_DirectionalAmbientXPos = 0x00001000,
         kChanged_DirectionalAmbientXNeg = 0x00002000,
         kChanged_DirectionalAmbientYPos = 0x00004000,
         kChanged_DirectionalAmbientYNeg = 0x00008000,
         kChanged_DirectionalAmbientZPos = 0x00010000,
         kChanged_DirectionalAmbientZNeg = 0x00020000,
         kChanged_LightFadeBegin = 0x00040000,
         kChanged_LightFadeEnd   = 0x00080000,
         //
         kChanged_LastLightingFlag = kChanged_LightFadeEnd,
         //
         kChanged_LightingTemplate = 0x10000000,
         kChanged_AcousticSpace    = 0x20000000,
         kChanged_ImageSpace       = 0x40000000,
         kChanged_MusicType        = 0x80000000,
      };
      struct CellDefaults {
         CellData data;
         FormID   lightingTemplate = 0;
         FormID   acousticSpace = 0;
         FormID   imageSpace = 0;
         FormID   musicType = 0;
      };
      struct CellModifications {
         CellData data;
         UInt32   changes = 0; // ChangeFlags enum as bitmask
         UInt32   changedTemplateUsage = 0; // bitmask indicating which bits of CellData::inheritFromTemplate to apply
         FormID   lightingTemplate = 0;
         FormID   acousticSpace = 0;
         FormID   imageSpace = 0;
         FormID   musicType = 0;
         //
         inline bool is_unchanged() const {
            return this->changes == 0 && this->changedTemplateUsage == 0;
         };
      };
   private:
      std::unordered_map<FormID, CellDefaults> _defaults;
      std::unordered_map<FormID, CellModifications> _changes;
      std::recursive_mutex _lock;
      //
      static void ApplyToCell(RE::TESObjectCELL* cell, const CellModifications& mods);
      static void RevertCell(RE::TESObjectCELL* cell, const CellDefaults& defaults);
      static void RevertCellProperties(RE::TESObjectCELL*, const CellDefaults& defaults, UInt32 propertyFlagsToRevert);
      //
   public:
      void StoreDefaults(FormID cellID, RE::TESObjectCELL* cell);
      void Reset(FormID);
      void ResetFields(FormID cellID, UInt32 fieldMask, bool alsoModifyCell = true);
      void ResetTemplateUsageFlags(FormID cellID, UInt32 flagsMask, bool alsoModifyCell = true);
      void ResetAll();
      bool GetDefaults(FormID cellID, CellDefaults& out); // zeroes (out) if there is no matching entry; returns bool indicating whether the cell is in the dataset
      void Modify(RE::TESObjectCELL*, UInt32 traitFlags, UInt32 usageFlags);
      //
      enum { kSaveVersion = 3 };
      bool Save(SKSESerializationInterface* intfc);
      bool Load(SKSESerializationInterface* intfc, UInt32 version); // also modifies TESObjectCELLs to apply loaded change-data
};