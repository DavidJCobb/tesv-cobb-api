#include "CellInteriorDataService.h"
#include "skse/Serialization.h"

#include "ReverseEngineered/ExtraData.h"

void CellInteriorDataService::RevertCell(RE::TESObjectCELL* cell, const CellDefaults& defaults) {
   auto cellData = CALL_MEMBER_FN(cell, GetInteriorData)();
   if (cellData)
      memcpy(cellData, &(defaults.data), sizeof(CellData));
   cell->lightingTemplate = (RE::BGSLightingTemplate*) LookupFormByID(defaults.lightingTemplate);
   {  // Handle ExtraData.
      auto extraList = (RE::BaseExtraList*) &cell->extraData;
      if (!defaults.acousticSpace)
         CALL_MEMBER_FN(extraList, SetExtraCellAcousticSpace)(nullptr);
      else {
         auto casted = DYNAMIC_CAST(LookupFormByID(defaults.acousticSpace), TESForm, BGSAcousticSpace);
         if (casted)
            CALL_MEMBER_FN(extraList, SetExtraCellAcousticSpace)(casted);
      }
      if (!defaults.imageSpace)
         CALL_MEMBER_FN(extraList, SetExtraCellImageSpace)(nullptr);
      else {
         auto casted = DYNAMIC_CAST(LookupFormByID(defaults.imageSpace), TESForm, TESImageSpace);
         if (casted)
            CALL_MEMBER_FN(extraList, SetExtraCellImageSpace)(casted);
      }
      if (!defaults.musicType)
         CALL_MEMBER_FN(extraList, SetExtraCellMusicType)(nullptr);
      else {
         auto casted = DYNAMIC_CAST(LookupFormByID(defaults.musicType), TESForm, BGSMusicType);
         if (casted)
            CALL_MEMBER_FN(extraList, SetExtraCellMusicType)(casted);
      }
   }
};

void CellInteriorDataService::StoreDefaults(FormID id, RE::TESObjectCELL* cell) {
   //std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock); // commented out for perf; nothing should be accessing the map during ESP/ESM load anyway
   CellDefaults record;
   //
   auto intData = CALL_MEMBER_FN(cell, GetInteriorData)();
   if (!intData) return; // don't bother storing exterior cells
   if (intData)
      record.data = *intData;
   //
   auto extra = (RE::BaseExtraList*) &(cell->extraData);
   //
   // During TESForm::LoadForm, the cell extra-data types store form IDs rather than 
   // pointers, since the pointed-to forms may not have been loaded yet. According to 
   // shadeMe, they get resolved in TESForm::InitItem (referred to as TESForm::Link 
   // in some xSE versions).
   //
   record.lightingTemplate = reinterpret_cast<UInt32>(cell->lightingTemplate); // it's still a form ID right now.
   /*// Straightforward but slow code.
   RE::IExtraDataSingleFormID* data;
   data = (RE::IExtraDataSingleFormID*) cell->extraData.GetByType(kExtraData_CellAcousticSpace);
   if (data)
      record.acousticSpace = data->id;
   data = (RE::IExtraDataSingleFormID*) cell->extraData.GetByType(kExtraData_CellImageSpace);
   if (data)
      record.imageSpace = data->id;
   data = (RE::IExtraDataSingleFormID*) cell->extraData.GetByType(kExtraData_CellMusicType);
   if (data)
      record.musicType = data->id;
   //*/
   {  // Each GetByType call is a loop. Why not just use one loop for all three?
      UInt8 found = 0;
      for (auto item = (RE::IExtraDataSingleFormID*) extra->m_data; item; item = (RE::IExtraDataSingleFormID*) item->next) {
         switch (item->GetType()) {
            case kExtraData_CellAcousticSpace:
               found |= 1;
               record.acousticSpace = item->id;
               break;
            case kExtraData_CellImageSpace:
               found |= 2;
               record.imageSpace = item->id;
               break;
            case kExtraData_CellMusicType:
               found |= 4;
               record.musicType = item->id;
               break;
            default:
               continue;
         }
         if (found == (1 | 2 | 4))
            break;
      }
   }
   //
   this->_defaults[id] = record;
};
void CellInteriorDataService::Reset(FormID id) {
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   try {
      auto& data = this->_defaults.at(id);
      auto cell  = (RE::TESObjectCELL*) DYNAMIC_CAST(LookupFormByID(id), TESForm, TESObjectCELL);
      if (!cell)
         return;
      CellInteriorDataService::RevertCell(cell, data);
   } catch (std::out_of_range) {}
};
void CellInteriorDataService::ResetFields(FormID id, UInt32 fieldMask, bool alsoModifyCell) {
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   try {
      auto& record = this->_changes.at(id);
      record.changes &= ~fieldMask;
      if (record.is_unchanged())
         this->_changes.erase(id);
      //
      if (alsoModifyCell) {
         auto cell = (RE::TESObjectCELL*) DYNAMIC_CAST(LookupFormByID(id), TESForm, TESObjectCELL);
         if (cell) {
            CellDefaults def;
            if (this->GetDefaults(id, def))
               RevertCellProperties(cell, def, fieldMask);
         }
      }
   } catch (std::out_of_range) {}
};
void CellInteriorDataService::ResetTemplateUsageFlags(FormID id, UInt32 flagsMask, bool alsoModifyCell) {
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   try {
      auto& record = this->_changes.at(id);
      record.changedTemplateUsage &= ~flagsMask;
      if (record.is_unchanged())
         this->_changes.erase(id);
      //
      if (alsoModifyCell) {
         auto cell = (RE::TESObjectCELL*) DYNAMIC_CAST(LookupFormByID(id), TESForm, TESObjectCELL);
         if (cell) {
            auto cellData = CALL_MEMBER_FN(cell, GetInteriorData)();
            if (cellData) {
               CellDefaults def;
               if (this->GetDefaults(id, def)) {
                  UInt32 tFlags = def.data.inheritFromTemplate;
                  tFlags |= (record.data.inheritFromTemplate & record.changedTemplateUsage);
                  tFlags &= ~(~record.data.inheritFromTemplate & record.changedTemplateUsage);
                  cellData->inheritFromTemplate = tFlags;
               }
            }
         }
      }
   } catch (std::out_of_range) {}
}
void CellInteriorDataService::ResetAll() {
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   _MESSAGE("CellInteriorDataService::ResetAll()");
   if (this->_defaults.empty())
      return;
   for (auto it = this->_defaults.begin(); it != this->_defaults.end(); ++it) {
      auto cell = (RE::TESObjectCELL*) DYNAMIC_CAST(LookupFormByID(it->first), TESForm, TESObjectCELL);
      if (!cell)
         continue;
      CellInteriorDataService::RevertCell(cell, it->second);
   }
};
bool CellInteriorDataService::GetDefaults(FormID id, CellDefaults& out) {
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   memset(&out, 0, sizeof(CellDefaults));
   try {
      auto data = this->_defaults.at(id);
      memcpy(&out, &data, sizeof(CellDefaults));
   } catch (std::out_of_range) {
      return false;
   }
   return true;
};
void CellInteriorDataService::Modify(RE::TESObjectCELL* cell, UInt32 traitFlags, UInt32 usageFlags) {
   if (!cell)
      return;
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   //
   FormID id = cell->formID;
   auto cellData = CALL_MEMBER_FN(cell, GetInteriorData)();
   if (!cellData)
      return;
   try {
      auto& record = this->_changes.at(id);
      if (usageFlags)
         record.changedTemplateUsage |= usageFlags;
      if (traitFlags) {
         if (traitFlags <= kChanged_LastLightingFlag)
            memcpy(&record.data, cellData, sizeof(CellData));
         else {
            memcpy(&record.data, cellData, sizeof(CellData)); // in case flags above AND below the threshold are set
            if (traitFlags & kChanged_LightingTemplate) {
               auto form = cell->lightingTemplate;
               record.lightingTemplate = form ? form->formID : 0;
            }
            if (traitFlags & kChanged_AcousticSpace) {
               auto form = CALL_MEMBER_FN(cell, GetAcousticSpace)();
               record.acousticSpace = form ? form->formID : 0;
            }
            if (traitFlags & kChanged_ImageSpace) {
               auto form = cell->GetImageSpace();
               record.imageSpace = form ? form->formID : 0;
            }
            if (traitFlags & kChanged_MusicType) {
               auto form = cell->GetMusicType();
               record.musicType = form ? form->formID : 0;
            }
         }
         record.changes |= traitFlags;
      }
   } catch (std::out_of_range) {
      CellModifications record;
      //
      memcpy(&record.data, cellData, sizeof(CellData));
      if (traitFlags & kChanged_LightingTemplate) {
         auto form = cell->lightingTemplate;
         record.lightingTemplate = form ? form->formID : 0;
      }
      if (traitFlags & kChanged_AcousticSpace) {
         auto form = CALL_MEMBER_FN(cell, GetAcousticSpace)();
         record.acousticSpace = form ? form->formID : 0;
      }
      if (traitFlags & kChanged_ImageSpace) {
         auto form = cell->GetImageSpace();
         record.imageSpace = form ? form->formID : 0;
      }
      if (traitFlags & kChanged_MusicType) {
         auto form = cell->GetMusicType();
         record.musicType = form ? form->formID : 0;
      }
      //
      record.changes = traitFlags;
      record.changedTemplateUsage = usageFlags;
      this->_changes[id] = record;
   }
};
//
void CellInteriorDataService::ApplyToCell(RE::TESObjectCELL* cell, const CellModifications& mods) {
   UInt32 flags = mods.changes;
   {
      auto extraList = (RE::BaseExtraList*) &cell->extraData;
      if (flags & kChanged_LightingTemplate) {
         if (!mods.lightingTemplate)
            cell->lightingTemplate = nullptr;
         else {
            auto casted = (RE::BGSLightingTemplate*) DYNAMIC_CAST(LookupFormByID(mods.lightingTemplate), TESForm, BGSLightingTemplate);
            if (casted)
               cell->lightingTemplate = casted;
         }
      }
      if (flags & kChanged_AcousticSpace) {
         if (!mods.acousticSpace)
            CALL_MEMBER_FN(extraList, SetExtraCellAcousticSpace)(nullptr);
         else {
            auto casted = DYNAMIC_CAST(LookupFormByID(mods.acousticSpace), TESForm, BGSAcousticSpace);
            if (casted)
               CALL_MEMBER_FN(extraList, SetExtraCellAcousticSpace)(casted);
         }
      }
      if (flags & kChanged_ImageSpace) {
         if (!mods.imageSpace)
            CALL_MEMBER_FN(extraList, SetExtraCellImageSpace)(nullptr);
         else {
            auto casted = DYNAMIC_CAST(LookupFormByID(mods.imageSpace), TESForm, TESImageSpace);
            if (casted)
               CALL_MEMBER_FN(extraList, SetExtraCellImageSpace)(casted);
         }
      }
      if (flags & kChanged_MusicType) {
         if (!mods.musicType)
            CALL_MEMBER_FN(extraList, SetExtraCellMusicType)(nullptr);
         else {
            auto casted = DYNAMIC_CAST(LookupFormByID(mods.musicType), TESForm, BGSMusicType);
            if (casted)
               CALL_MEMBER_FN(extraList, SetExtraCellMusicType)(casted);
         }
      }
   }
   CellData* cellData = CALL_MEMBER_FN(cell, GetInteriorData)();
   if (!cellData)
      return;
   {  // Lighting Template usage flags
      UInt32 tFlags = cellData->inheritFromTemplate;
      tFlags |=   (mods.data.inheritFromTemplate & mods.changedTemplateUsage);
      tFlags &= ~(~mods.data.inheritFromTemplate & mods.changedTemplateUsage);
      cellData->inheritFromTemplate = tFlags;
   }
   auto&  loaded = mods.data;
   {  // Lighting and fog settings
      if (flags & kChanged_Ambient)
         cellData->ambientColor.abgr = loaded.ambientColor.abgr;
      if (flags & kChanged_Directional)
         cellData->directionalColor.abgr = loaded.directionalColor.abgr;
      if (flags & kChanged_FogColorNear)
         cellData->fogColorNear.abgr = loaded.fogColorNear.abgr;
      if (flags & kChanged_FogColorFar)
         cellData->fogColorFar.abgr = loaded.fogColorFar.abgr;
      if (flags & kChanged_FogPlaneNear)
         cellData->fogPlaneNear = loaded.fogPlaneNear;
      if (flags & kChanged_FogPlaneFar)
         cellData->fogPlaneFar = loaded.fogPlaneFar;
      if (flags & kChanged_FogPlaneClip)
         cellData->fogClipDistance = loaded.fogClipDistance;
      if (flags & kChanged_FogPower)
         cellData->fogPow = loaded.fogPow;
      if (flags & kChanged_FogMax)
         cellData->fogMax = loaded.fogMax;
      if (flags & kChanged_DirectionalRotationXY)
         cellData->directionalRotXY = loaded.directionalRotXY;
      if (flags & kChanged_DirectionalRotationZ)
         cellData->directionalRotZ = loaded.directionalRotZ;
      if (flags & kChanged_DirectionalFade)
         cellData->directionalFade = loaded.directionalFade;
      if (flags & kChanged_DirectionalAmbientXPos)
         cellData->directionalAmbient.xPos = loaded.directionalAmbient.xPos;
      if (flags & kChanged_DirectionalAmbientXNeg)
         cellData->directionalAmbient.xNeg = loaded.directionalAmbient.xNeg;
      if (flags & kChanged_DirectionalAmbientYPos)
         cellData->directionalAmbient.yPos = loaded.directionalAmbient.yPos;
      if (flags & kChanged_DirectionalAmbientYNeg)
         cellData->directionalAmbient.yNeg = loaded.directionalAmbient.yNeg;
      if (flags & kChanged_DirectionalAmbientZPos)
         cellData->directionalAmbient.zPos = loaded.directionalAmbient.zPos;
      if (flags & kChanged_DirectionalAmbientZNeg)
         cellData->directionalAmbient.zNeg = loaded.directionalAmbient.zNeg;
      if (flags & kChanged_LightFadeBegin)
         cellData->lightFadeBegin = loaded.lightFadeBegin;
      if (flags & kChanged_LightFadeEnd)
         cellData->lightFadeEnd = loaded.lightFadeEnd;
   }
   //
};
void CellInteriorDataService::RevertCellProperties(RE::TESObjectCELL* cell, const CellDefaults& defaults, UInt32 propertyFlags) {
   {  // Extra-data changes
      auto extraList = &cell->extraData;
      if (propertyFlags & kChanged_LightingTemplate) {
         if (!defaults.lightingTemplate)
            cell->lightingTemplate = nullptr;
         else {
            auto casted = (RE::BGSLightingTemplate*) DYNAMIC_CAST(LookupFormByID(defaults.lightingTemplate), TESForm, BGSLightingTemplate);
            if (casted)
               cell->lightingTemplate = casted;
         }
      }
      if (propertyFlags & kChanged_AcousticSpace) {
         if (!defaults.acousticSpace)
            CALL_MEMBER_FN(extraList, SetExtraCellAcousticSpace)(nullptr);
         else {
            auto casted = DYNAMIC_CAST(LookupFormByID(defaults.acousticSpace), TESForm, BGSAcousticSpace);
            if (casted)
               CALL_MEMBER_FN(extraList, SetExtraCellAcousticSpace)(casted);
         }
      }
      if (propertyFlags & kChanged_ImageSpace) {
         if (!defaults.imageSpace)
            CALL_MEMBER_FN(extraList, SetExtraCellImageSpace)(nullptr);
         else {
            auto casted = DYNAMIC_CAST(LookupFormByID(defaults.imageSpace), TESForm, TESImageSpace);
            if (casted)
               CALL_MEMBER_FN(extraList, SetExtraCellImageSpace)(casted);
         }
      }
      if (propertyFlags & kChanged_MusicType) {
         if (!defaults.musicType)
            CALL_MEMBER_FN(extraList, SetExtraCellMusicType)(nullptr);
         else {
            auto casted = DYNAMIC_CAST(LookupFormByID(defaults.musicType), TESForm, BGSMusicType);
            if (casted)
               CALL_MEMBER_FN(extraList, SetExtraCellMusicType)(casted);
         }
      }
   }
   {  // Interior-data changes
      auto  cellData = CALL_MEMBER_FN(cell, GetInteriorData)();
      auto& loaded = defaults.data;
      if (cellData) {
         if (propertyFlags & kChanged_Ambient)
            cellData->ambientColor.abgr = loaded.ambientColor.abgr;
         if (propertyFlags & kChanged_Directional)
            cellData->directionalColor.abgr = loaded.directionalColor.abgr;
         if (propertyFlags & kChanged_FogColorNear)
            cellData->fogColorNear.abgr = loaded.fogColorNear.abgr;
         if (propertyFlags & kChanged_FogColorFar)
            cellData->fogColorFar.abgr = loaded.fogColorFar.abgr;
         if (propertyFlags & kChanged_FogPlaneNear)
            cellData->fogPlaneNear = loaded.fogPlaneNear;
         if (propertyFlags & kChanged_FogPlaneFar)
            cellData->fogPlaneFar = loaded.fogPlaneFar;
         if (propertyFlags & kChanged_FogPlaneClip)
            cellData->fogClipDistance = loaded.fogClipDistance;
         if (propertyFlags & kChanged_FogPower)
            cellData->fogPow = loaded.fogPow;
         if (propertyFlags & kChanged_FogMax)
            cellData->fogMax = loaded.fogMax;
         if (propertyFlags & kChanged_DirectionalRotationXY)
            cellData->directionalRotXY = loaded.directionalRotXY;
         if (propertyFlags & kChanged_DirectionalRotationZ)
            cellData->directionalRotZ = loaded.directionalRotZ;
         if (propertyFlags & kChanged_DirectionalFade)
            cellData->directionalFade = loaded.directionalFade;
         if (propertyFlags & kChanged_DirectionalAmbientXPos)
            cellData->directionalAmbient.xPos = loaded.directionalAmbient.xPos;
         if (propertyFlags & kChanged_DirectionalAmbientXNeg)
            cellData->directionalAmbient.xNeg = loaded.directionalAmbient.xNeg;
         if (propertyFlags & kChanged_DirectionalAmbientYPos)
            cellData->directionalAmbient.yPos = loaded.directionalAmbient.yPos;
         if (propertyFlags & kChanged_DirectionalAmbientYNeg)
            cellData->directionalAmbient.yNeg = loaded.directionalAmbient.yNeg;
         if (propertyFlags & kChanged_DirectionalAmbientZPos)
            cellData->directionalAmbient.zPos = loaded.directionalAmbient.zPos;
         if (propertyFlags & kChanged_DirectionalAmbientZNeg)
            cellData->directionalAmbient.zNeg = loaded.directionalAmbient.zNeg;
         if (propertyFlags & kChanged_LightFadeBegin)
            cellData->lightFadeBegin = loaded.lightFadeBegin;
         if (propertyFlags & kChanged_LightFadeEnd)
            cellData->lightFadeEnd = loaded.lightFadeEnd;
      }
   }
};
//
bool CellInteriorDataService::Save(SKSESerializationInterface* intfc) {
   using namespace Serialization;
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   //
   UInt32 temporary = this->_changes.size();
   SERIALIZATION_ASSERT(WriteData(intfc, &temporary), "Failed to write the record count.");
   for (auto it = this->_changes.begin(); it != this->_changes.end(); it++) {
      SERIALIZATION_ASSERT(WriteData(intfc, &it->first), "Failed to write a cell formID.");
      auto& record = it->second;
      {
         SERIALIZATION_ASSERT(WriteData(intfc, &record.data), "Failed to write a record's interior data.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.changes), "Failed to write a record's changes.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.changedTemplateUsage), "Failed to write a record's template usage flag changes.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.lightingTemplate), "Failed to write a record's lighting-template form ID.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.acousticSpace), "Failed to write a record's acoustic space form ID.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.imageSpace), "Failed to write a record's imagespace form ID.");
         SERIALIZATION_ASSERT(WriteData(intfc, &record.musicType), "Failed to write a record's music-type form ID.");
      }
   }
   return true;
};
bool CellInteriorDataService::Load(SKSESerializationInterface* intfc, UInt32 version) {
   using namespace Serialization;
   std::lock_guard<decltype(this->_lock)> scopedLock(this->_lock);
   //
   UInt32 count = 0;
   SERIALIZATION_ASSERT(ReadData(intfc, &count), "Failed to read the record count.");
   for (UInt32 i = 0; i < count; i++) {
      CellModifications record;
      FormID key;
      SERIALIZATION_ASSERT(ReadData(intfc, &key), "Failed to read a cell formID.");
      SERIALIZATION_ASSERT(ReadData(intfc, &record.data), "Failed to read a record's interior data.");
      SERIALIZATION_ASSERT(ReadData(intfc, &record.changes), "Failed to read a record's changes.");
      SERIALIZATION_ASSERT(ReadData(intfc, &record.changedTemplateUsage), "Failed to read a record's template usage flag changes.");
      if (version >= 3) {
         SERIALIZATION_ASSERT(ReadData(intfc, &record.lightingTemplate), "Failed to read a record's lighting-template form ID.");
      }
      if (version >= 2) {
         SERIALIZATION_ASSERT(ReadData(intfc, &record.acousticSpace), "Failed to read a record's acoustic space form ID.");
         SERIALIZATION_ASSERT(ReadData(intfc, &record.imageSpace), "Failed to read a record's imagespace form ID.");
         SERIALIZATION_ASSERT(ReadData(intfc, &record.musicType), "Failed to read a record's music-type form ID.");
      }
      {  // Fix the form IDs, if the load order changed.
         if (!intfc->ResolveFormId(key, &key)) {
            _MESSAGE("%s: Failed to correct a record's formID (0x%08X). Skipping.", __FUNCTION__, key);
            continue;
         }
         if (record.lightingTemplate)
            if (!intfc->ResolveFormId(record.lightingTemplate, &record.lightingTemplate)) {
               _MESSAGE("%s: Failed to correct a record's lighting-template formID (0x%08X). Skipping.", __FUNCTION__, key);
               continue;
            };
         if (record.acousticSpace)
            if (!intfc->ResolveFormId(record.acousticSpace, &record.acousticSpace)) {
               _MESSAGE("%s: Failed to correct a record's acoustic space formID (0x%08X). Skipping.", __FUNCTION__, key);
               continue;
            };
         if (record.imageSpace)
            if (!intfc->ResolveFormId(record.imageSpace, &record.imageSpace)) {
               _MESSAGE("%s: Failed to correct a record's imagespace formID (0x%08X). Skipping.", __FUNCTION__, key);
               continue;
            };
         if (record.musicType)
            if (!intfc->ResolveFormId(record.musicType, &record.musicType)) {
               _MESSAGE("%s: Failed to correct a record's music-type formID (0x%08X). Skipping.", __FUNCTION__, key);
               continue;
            };
      }
      //
      this->_changes[key] = record;
      //
      {  // Apply the changes.
         auto cell = (RE::TESObjectCELL*) DYNAMIC_CAST(LookupFormByID(key), TESForm, TESObjectCELL);
         if (cell)
            CellInteriorDataService::ApplyToCell(cell, record);
      }
   }
   return true;
};