#include "Papyrus/Cell.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameData.h"
#include "skse/GameForms.h"
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESWorldSpace.h"
#include "ReverseEngineered/Objects/Sky.h"
#include "ReverseEngineered/Systems/GameData.h"
#include "ReverseEngineered/Systems/Sound.h"
#include "Services/CellInteriorDataService.h"

#include "Miscellaneous/strings.h"

namespace PapyrusPrefix(Papyrus) {
   namespace Cell {
      enum GetterSource {
         kGetterSourceBit_Defaults       = 1,
         kGetterSourceBit_TemplateInUse  = 2,
         kGetterSourceBit_TemplateAlways = 4,
      };
      template <typename T> T _GetLightingField(RE::TESObjectCELL* cell, RE::TESObjectCELL::InteriorData* data, UInt32 fieldOffset, SInt32 which, UInt32 flag) {
         if (which & kGetterSourceBit_TemplateAlways) {
            if (cell->lightingTemplate)
               return *(T*)(&cell->lightingTemplate->data + fieldOffset);
         }
         if ((which & kGetterSourceBit_TemplateInUse) && (data->inheritFromTemplate & flag)) {
            if (cell->lightingTemplate)
               return *(T*)(&cell->lightingTemplate->data + fieldOffset);
         }
         if (which & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
               return *(T*)(&def.data + fieldOffset);
         }
         return *(T*)(data + fieldOffset);
      };
      #define PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(field, flag) \
         return _GetLightingField<decltype(RE::TESObjectCELL::InteriorData::field)>(cell, data, offsetof(RE::TESObjectCELL::InteriorData, field), getWhich, flag);
      //
      //
      // Papyrus code follows.
      //
      //
      void ForceUniqueLighting(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify the lighting data of a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->inheritFromTemplate = 0;
         if (persist)
            CellInteriorDataService::GetInstance().Modify(cell, 0, 0xFFFFFFFF);
      };
      VMResultArray<SInt32> FromColorCode(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 code) {
         VMResultArray<SInt32> result;
         result.resize(3);
         result[0] = code & 0xFF;
         result[1] = (code >> 0x8) & 0xFF;
         result[2] = (code >> 0x10) & 0xFF;
         return result;
      };
      TESForm* GetAcousticSpace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               TESForm* out = LookupFormByID(def.acousticSpace);
               if (out && out->formType == kFormType_AcousticSpace)
                  return out;
               return nullptr;
            }
         }
         return CALL_MEMBER_FN(cell, GetAcousticSpace)();
      };
      SInt32 GetAmbientColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(ambientColor.abgr, RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient);
         /*//
         if (getWhich & kGetterSourceBit_TemplateAlways) {
            if (cell->lightingTemplate)
               return cell->lightingTemplate->data.ambientColor.abgr;
         } else if (getWhich & kGetterSourceBit_TemplateInUse) {
            if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient))
               return cell->lightingTemplate->data.ambientColor.abgr;
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
               return def.data.ambientColor.abgr;
         }
         return data->ambientColor.abgr;
         //*/
      };
      SInt32 GetDirectionalColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(directionalColor.abgr, RE::TESObjectCELL::kLightingTemplateUsageFlag_Directional);
         /*//
         if (getWhich & kGetterSourceBit_TemplateAlways) {
            if (cell->lightingTemplate)
               return cell->lightingTemplate->data.directionalColor.abgr;
         } else if (getWhich & kGetterSourceBit_TemplateInUse) {
            if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_Directional))
               return cell->lightingTemplate->data.directionalColor.abgr;
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
               return def.data.directionalColor.abgr;
         }
         return data->directionalColor.abgr;
         //*/
      };
      float GetDirectionalFade(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(directionalFade, RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalFade);
         /*//
         if (getWhich & kGetterSourceBit_TemplateAlways) {
            if (cell->lightingTemplate)
               return cell->lightingTemplate->data.directionalFade;
         } else if (getWhich & kGetterSourceBit_TemplateInUse) {
            if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalFade))
               return cell->lightingTemplate->data.directionalFade;
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
               return def.data.directionalFade;
         }
         return data->directionalFade;
         //*/
      };
      VMResultArray<SInt32> GetDirectionalAmbientColors(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         VMResultArray<SInt32> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         if (cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting) {
            registry->LogError("Cannot retrieve lighting data for a cell that uses sky lighting.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve lighting data for an exterior cell.", stackId);
            return result;
         }
         result.resize(6);
         if (cell->lightingTemplate) {
            auto tmpl = cell->lightingTemplate;
            if (getWhich & kGetterSourceBit_TemplateAlways) {
               result[0] = tmpl->directionalAmbient.xPos.abgr;
               result[1] = tmpl->directionalAmbient.xNeg.abgr;
               result[2] = tmpl->directionalAmbient.yPos.abgr;
               result[3] = tmpl->directionalAmbient.yNeg.abgr;
               result[4] = tmpl->directionalAmbient.zPos.abgr;
               result[5] = tmpl->directionalAmbient.zNeg.abgr;
               return result;
            } else if (getWhich & kGetterSourceBit_TemplateInUse) {
               if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient)) {
                  result[0] = tmpl->directionalAmbient.xPos.abgr;
                  result[1] = tmpl->directionalAmbient.xNeg.abgr;
                  result[2] = tmpl->directionalAmbient.yPos.abgr;
                  result[3] = tmpl->directionalAmbient.yNeg.abgr;
                  result[4] = tmpl->directionalAmbient.zPos.abgr;
                  result[5] = tmpl->directionalAmbient.zNeg.abgr;
                  return result;
               }
            }
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               result[0] = def.data.directionalAmbient.xPos.abgr;
               result[1] = def.data.directionalAmbient.xNeg.abgr;
               result[2] = def.data.directionalAmbient.yPos.abgr;
               result[3] = def.data.directionalAmbient.yNeg.abgr;
               result[4] = def.data.directionalAmbient.zPos.abgr;
               result[5] = def.data.directionalAmbient.zNeg.abgr;
               return result;
            }
         }
         result[0] = data->directionalAmbient.xPos.abgr;
         result[1] = data->directionalAmbient.xNeg.abgr;
         result[2] = data->directionalAmbient.yPos.abgr;
         result[3] = data->directionalAmbient.yNeg.abgr;
         result[4] = data->directionalAmbient.zPos.abgr;
         result[5] = data->directionalAmbient.zNeg.abgr;
         return result;
      };
      VMResultArray<SInt32> GetDirectionalRotation(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         VMResultArray<SInt32> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         if (cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting) {
            registry->LogError("Cannot retrieve lighting data for a cell that uses sky lighting.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve lighting data for an exterior cell.", stackId);
            return result;
         }
         result.resize(2);
         if (cell->lightingTemplate) {
            auto tmpl = cell->lightingTemplate;
            if (getWhich & kGetterSourceBit_TemplateAlways) {
               result[0] = tmpl->data.directionalRotXY;
               result[1] = tmpl->data.directionalRotZ;
               return result;
            } else if (getWhich & kGetterSourceBit_TemplateInUse) {
               if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalRotation)) {
                  result[0] = tmpl->data.directionalRotXY;
                  result[1] = tmpl->data.directionalRotZ;
                  return result;
               }
            }
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               result[0] = def.data.directionalRotXY;
               result[1] = def.data.directionalRotZ;
               return result;
            }
         }
         result[0] = data->directionalRotXY;
         result[1] = data->directionalRotZ;
         return result;
      };
      BSFixedString GetEditorID(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = (RE::ExtraEditorID*) cell->extraData.GetByType(kExtraData_EditorID);
         if (!data)
            return nullptr;
         return data->data;
      };
      BGSEncounterZone* GetEncounterZone(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         return CALL_MEMBER_FN((RE::BaseExtraList*) &cell->extraData, GetExtraEncounterZone)();
      };
      VMResultArray<SInt32> GetExteriorCellCoordinates(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         VMResultArray<SInt32> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetExteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve coordinates for an interior cell.", stackId);
            return result;
         }
         result.resize(2);
         result[0] = data->cellX;
         result[1] = data->cellY;
         return result;
      };
      VMResultArray<float> GetExteriorUnitCoordinates(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         VMResultArray<float> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetExteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve coordinates for an interior cell.", stackId);
            return result;
         }
         result.resize(2);
         result[0] = data->unitX;
         result[1] = data->unitY;
         return result;
      };
      float GetFogClipDistance(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         ERROR_AND_RETURN_0_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot retrieve lighting data for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(fogClipDistance, RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance);
         /*//
         return _GetLightingField<float>(cell, data, offsetof(RE::TESObjectCELL::InteriorData, fogClipDistance), getWhich, RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance);
         //*/
         /*//
         if (getWhich & kGetterSourceBit_TemplateAlways) {
            if (cell->lightingTemplate)
               return cell->lightingTemplate->data.fogClipDistance;
         } else if (getWhich & kGetterSourceBit_TemplateInUse) {
            if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance))
               return cell->lightingTemplate->data.fogClipDistance;
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
               return def.data.fogClipDistance;
         }
         return data->fogClipDistance;
         //*/
      };
      VMResultArray<SInt32> GetFogColors(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         VMResultArray<SInt32> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         if (cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting) {
            registry->LogError("Cannot retrieve lighting data for a cell that uses sky lighting.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve lighting data for an exterior cell.", stackId);
            return result;
         }
         result.resize(6);
         if (cell->lightingTemplate) {
            auto tmpl = cell->lightingTemplate;
            if (getWhich & kGetterSourceBit_TemplateAlways) {
               result[0] = tmpl->data.fogColorNear.color.red;
               result[1] = tmpl->data.fogColorNear.color.green;
               result[2] = tmpl->data.fogColorNear.color.blue;
               result[3] = tmpl->data.fogColorFar.color.red;
               result[4] = tmpl->data.fogColorFar.color.green;
               result[5] = tmpl->data.fogColorFar.color.blue;
               return result;
            } else if (getWhich & kGetterSourceBit_TemplateInUse) {
               if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor)) {
                  result[0] = tmpl->data.fogColorNear.color.red;
                  result[1] = tmpl->data.fogColorNear.color.green;
                  result[2] = tmpl->data.fogColorNear.color.blue;
                  result[3] = tmpl->data.fogColorFar.color.red;
                  result[4] = tmpl->data.fogColorFar.color.green;
                  result[5] = tmpl->data.fogColorFar.color.blue;
                  return result;
               }
            }
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               result[0] = def.data.fogColorNear.color.red;
               result[1] = def.data.fogColorNear.color.green;
               result[2] = def.data.fogColorNear.color.blue;
               result[3] = def.data.fogColorFar.color.red;
               result[4] = def.data.fogColorFar.color.green;
               result[5] = def.data.fogColorFar.color.blue;
               return result;
            }
         }
         result[0] = data->fogColorNear.color.red;
         result[1] = data->fogColorNear.color.green;
         result[2] = data->fogColorNear.color.blue;
         result[3] = data->fogColorFar.color.red;
         result[4] = data->fogColorFar.color.green;
         result[5] = data->fogColorFar.color.blue;
         return result;
      };
      VMResultArray<float> GetFogDistances(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         VMResultArray<float> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         if (cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting) {
            registry->LogError("Cannot retrieve lighting data for a cell that uses sky lighting.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve lighting data for an exterior cell.", stackId);
            return result;
         }
         result.resize(2);
         if (getWhich & kGetterSourceBit_TemplateAlways) {
            auto tmpl = cell->lightingTemplate;
            if (tmpl) {
               result[0] = tmpl->data.fogPlaneNear;
               result[1] = tmpl->data.fogPlaneFar;
               return result;
            }
         }
         result[0] = data->fogPlaneNear;
         result[1] = data->fogPlaneFar;
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               result[0] = def.data.fogPlaneNear;
               result[1] = def.data.fogPlaneFar;
            }
         }
         if (getWhich & kGetterSourceBit_TemplateInUse) {
            auto tmpl = cell->lightingTemplate;
            if (cell->lightingTemplate) {
               if (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance)
                  result[0] = tmpl->data.fogPlaneNear;
               if (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance)
                  result[1] = tmpl->data.fogPlaneFar;
            }
         }
         return result;
      };
      float GetFogMax(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         ERROR_AND_RETURN_0_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot retrieve lighting data for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(fogMax, RE::TESObjectCELL::kLightingTemplateUsageFlag_FogMax);
      };
      float GetFogPower(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         ERROR_AND_RETURN_0_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot retrieve lighting data for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         PAPYRUS_BOILERPLATE_RETURN_CELL_LIGHTING(fogPow, RE::TESObjectCELL::kLightingTemplateUsageFlag_FogPower);
      };
      TESForm* GetImageSpace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot get the imagespace of a None cell.", registry, stackId);
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               TESForm* out = LookupFormByID(def.imageSpace);
               if (out && out->formType == kFormType_ImageSpace)
                  return out;
               return nullptr;
            }
         }
         auto extra = (RE::ExtraCellImageSpace*) cell->extraData.GetByType(kExtraData_CellImageSpace);
         if (!extra)
            return nullptr;
         return extra->data;
      };
      VMResultArray<float> GetLightFadeDistances(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         VMResultArray<float> result;
         if (cell == nullptr) {
            registry->LogError("Cannot retrieve data from a None cell.", stackId);
            return result;
         }
         if (cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting) {
            registry->LogError("Cannot retrieve light fade distances for a cell that uses sky lighting.", stackId);
            return result;
         }
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         if (!data) {
            registry->LogError("Cannot retrieve lighting data for an exterior cell.", stackId);
            return result;
         }
         result.resize(2);
         if (cell->lightingTemplate) {
            auto tmpl = cell->lightingTemplate;
            if (getWhich & kGetterSourceBit_TemplateAlways) {
               result[0] = tmpl->data.lightFadeBegin;
               result[1] = tmpl->data.lightFadeEnd;
               return result;
            } else if (getWhich & kGetterSourceBit_TemplateInUse) {
               if (cell->lightingTemplate && (data->inheritFromTemplate & RE::TESObjectCELL::kLightingTemplateUsageFlag_LightFadeDistances)) {
                  result[0] = tmpl->data.lightFadeBegin;
                  result[1] = tmpl->data.lightFadeEnd;
                  return result;
               }
            }
         }
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def)) {
               result[0] = def.data.lightFadeBegin;
               result[1] = def.data.lightFadeEnd;
               return result;
            }
         }
         result[0] = data->lightFadeBegin;
         result[1] = data->lightFadeEnd;
         return result;
      };
      RE::TESForm* GetLightingTemplate(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         return cell->lightingTemplate;
      };
      bool GetLightingTemplateUsageFlag(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 flag) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         return !!(data->inheritFromTemplate & flag);
      };
      SInt32 GetLightingTemplateUsageFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         return data->inheritFromTemplate;
      };
      bool GetLightingTemplateUsageDefaultFlag(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 flag) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         CellInteriorDataService::CellDefaults def;
         if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
            return !!(def.data.inheritFromTemplate & flag);
         return !!(data->inheritFromTemplate & flag);
      };
      SInt32 GetLightingTemplateUsageDefaultFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_0_IF(data == nullptr, "Cannot retrieve lighting data for an exterior cell.", registry, stackId);
         CellInteriorDataService::CellDefaults def;
         if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
            return def.data.inheritFromTemplate;
         return data->inheritFromTemplate;
      };
      BGSMusicType* GetMusicType(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 getWhich) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         if (getWhich & kGetterSourceBit_Defaults) {
            CellInteriorDataService::CellDefaults def;
            if (CellInteriorDataService::GetInstance().GetDefaults(cell->formID, def))
               return DYNAMIC_CAST(LookupFormByID(def.musicType), TESForm, BGSMusicType);
         }
         auto extra = (RE::ExtraCellMusicType*) cell->extraData.GetByType(kExtraData_CellMusicType);
         if (!extra)
            return nullptr;
         return extra->data;
      };
      float GetNorthRotation(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         auto data = (RE::ExtraNorthRotation*) cell->extraData.GetByType(kExtraData_NorthRotation);
         if (!data)
            return 0.0F;
         return data->northRotation;
      };
      RE::TESWorldSpace* GetParentWorldspace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         return cell->parentWorld;
      };
      bool IsPublic(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         return !!(cell->unk2C & RE::TESObjectCELL::kCellFlag_IsPublic);
      };
      void ResetFields(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, UInt32 fieldMask) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot reset data for a None cell.", registry, stackId);
         CellInteriorDataService::GetInstance().ResetFields(cell->formID, fieldMask);
         {  // CellInteriorDataService won't live-update reverted fields. If the game doesn't, then we need to here.
            if (CALL_MEMBER_FN(*RE::g_TES, GetCurrentCell)() == cell) {
               if (fieldMask & CellInteriorDataService::kChanged_AcousticSpace)
                  RE::FadeToAcousticSpace(CALL_MEMBER_FN(cell, GetAcousticSpace)(), !!(cell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior));
               if (fieldMask & CellInteriorDataService::kChanged_ImageSpace) {
                  auto form = cell->GetImageSpace();
                  if (form) {
                     struct Unknown01BA8280 {
                        DEFINE_MEMBER_FN_LONG(Unknown01BA8280, Subroutine00C7BE40, void, 0x00C7BE40, TESImageSpace::Data14*);
                     };
                     auto data = *(Unknown01BA8280**)0x01BA8280;
                     CALL_MEMBER_FN(data, Subroutine00C7BE40)(&form->unk14);
                  }
               }
               if (fieldMask & CellInteriorDataService::kChanged_MusicType)
                  RE::SetCellMusic(cell->GetMusicType());
            }
         }
      };
      void ResetTemplateUsageFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, UInt32 flagsMask) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot reset data for a None cell.", registry, stackId);
         CellInteriorDataService::GetInstance().ResetTemplateUsageFlags(cell->formID, flagsMask);
      };
      void SetAcousticSpace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, RE::TESForm* form, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         auto casted = DYNAMIC_CAST(form, TESForm, BGSAcousticSpace);
         ERROR_AND_RETURN_IF(form && !casted, "The form you specified is not an acoustic space.", registry, stackId);
         CALL_MEMBER_FN(&cell->extraData, SetExtraCellAcousticSpace)(casted);
         //
         if (CALL_MEMBER_FN(*RE::g_TES, GetCurrentCell)() == cell) // live-update if current cell
            RE::FadeToAcousticSpace(casted, !!(cell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior));
         if (persist)
            CellInteriorDataService::GetInstance().Modify(cell, CellInteriorDataService::kChanged_AcousticSpace, 0);
      };
      void SetAmbientColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 color, bool force, bool fix, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->ambientColor.abgr = color;
         if (force) {
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient;
            if (fix && cell->lightingTemplate) {
               UInt8 i = 0;
               do {
                  data->directionalAmbientColors[i].abgr = cell->lightingTemplate->directionalAmbientColors[i].abgr;
               } while (++i < 6);
            }
         }
         if (persist) {
            UInt32 changeFlags = CellInteriorDataService::kChanged_Ambient;
            if (force && fix)
               changeFlags |= CellInteriorDataService::kChanged_DirectionalAmbientXPos |
               CellInteriorDataService::kChanged_DirectionalAmbientXNeg |
               CellInteriorDataService::kChanged_DirectionalAmbientYPos |
               CellInteriorDataService::kChanged_DirectionalAmbientYNeg |
               CellInteriorDataService::kChanged_DirectionalAmbientZPos |
               CellInteriorDataService::kChanged_DirectionalAmbientZNeg;
            CellInteriorDataService::GetInstance().Modify(
               cell,
               changeFlags,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient : 0
            );
         }
      };
      void SetDirectionalAmbientColors(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, VMArray<SInt32> colors, bool force, bool fix, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(colors.Length() < 6, "You must specify six directional ambient colors.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         {
            UInt8 i = 0;
            do {
               SInt32 color;
               colors.Get(&color, i);
               data->directionalAmbientColors[i].abgr = color;
            } while (++i < 6);
         }
         if (force) {
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient;
            if (fix && cell->lightingTemplate)
               data->ambientColor.abgr = cell->lightingTemplate->data.ambientColor.abgr;
         }
         if (persist) {
            UInt32 changeFlags =
               CellInteriorDataService::kChanged_DirectionalAmbientXPos |
               CellInteriorDataService::kChanged_DirectionalAmbientXNeg |
               CellInteriorDataService::kChanged_DirectionalAmbientYPos |
               CellInteriorDataService::kChanged_DirectionalAmbientYNeg |
               CellInteriorDataService::kChanged_DirectionalAmbientZPos |
               CellInteriorDataService::kChanged_DirectionalAmbientZNeg;
            if (force && fix)
               changeFlags |= CellInteriorDataService::kChanged_Ambient;
            CellInteriorDataService::GetInstance().Modify(
               cell,
               changeFlags,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_Ambient : 0
            );
         }
      };
      void SetDirectionalColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 color, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->directionalColor.abgr = color;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_Directional;
         if (persist) {
            CellInteriorDataService::GetInstance().Modify(
               cell,
               CellInteriorDataService::kChanged_Directional,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_Directional : 0
            );
         }
      };
      void SetDirectionalFade(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float fade, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->directionalFade = fade;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalFade;
         if (persist) {
            CellInteriorDataService::GetInstance().Modify(
               cell,
               CellInteriorDataService::kChanged_DirectionalFade,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalFade : 0
            );
         }
      };
      void SetDirectionalRotation(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, VMArray<SInt32> rotations, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(rotations.Length() < 6, "You must specify two rotations (XY and Z) in degrees.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         rotations.Get(&data->directionalRotXY, 0);
         rotations.Get(&data->directionalRotZ, 1);
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalRotation;
         if (persist) {
            CellInteriorDataService::GetInstance().Modify(
               cell,
               CellInteriorDataService::kChanged_DirectionalRotationXY | CellInteriorDataService::kChanged_DirectionalRotationZ,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_DirectionalRotation : 0
            );
         }
      };
      void SetFog(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         RE::TESObjectCELL* cell,
         SInt32 colorNear,
         SInt32 colorFar,
         float planeNear,
         float planeFar,
         float planeClip,
         float fogPow,
         float fogMax,
         bool force,
         bool persist
      ) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogColorNear.abgr = colorNear;
         data->fogPlaneNear = planeNear;
         data->fogColorFar.abgr = colorFar;
         data->fogPlaneFar = planeFar;
         data->fogClipDistance = planeClip;
         data->fogPow = fogPow;
         data->fogMax = fogMax;
         if (force) {
            data->inheritFromTemplate &= ~(
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor        |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance  |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogPower        |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogMax
            );
         }
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell, 
               CellInteriorDataService::kChanged_FogColorNear |
               CellInteriorDataService::kChanged_FogColorFar  |
               CellInteriorDataService::kChanged_FogPlaneNear |
               CellInteriorDataService::kChanged_FogPlaneFar  |
               CellInteriorDataService::kChanged_FogPlaneClip |
               CellInteriorDataService::kChanged_FogPower     |
               CellInteriorDataService::kChanged_FogMax,
               force ?
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor        |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance  |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogPower        |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogMax
               : 0
            );
         }
      };
      void SetFogColors(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 colorNear, SInt32 colorFar, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogColorNear.abgr = colorNear;
         data->fogColorFar.abgr  = colorFar;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor;
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell, 
               CellInteriorDataService::kChanged_FogColorNear |
               CellInteriorDataService::kChanged_FogColorFar,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor : 0
            );
         }
      };
      void SetFogDistances(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float planeNear, float planeFar, float planeClip, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogPlaneNear = planeNear;
         data->fogPlaneFar = planeFar;
         data->fogClipDistance = planeClip;
         if (force) {
            data->inheritFromTemplate &= ~(
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance |
               RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance
            );
         }
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell, 
               CellInteriorDataService::kChanged_FogPlaneNear |
               CellInteriorDataService::kChanged_FogPlaneFar  |
               CellInteriorDataService::kChanged_FogPlaneClip,
               force ?
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance |
                  RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance
               : 0
            );
         }
      };
      void SetFogDistanceClip(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float distance, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogClipDistance = distance;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance;
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell,
               CellInteriorDataService::kChanged_FogPlaneClip,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogClipDistance : 0
            );
         }
      };
      void SetFogDistanceFar(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float distance, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogPlaneNear = distance;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance;
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell, 
               CellInteriorDataService::kChanged_FogPlaneFar,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogFarDistance : 0
            );
         }
      };
      void SetFogDistanceNear(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float distance, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogPlaneNear = distance;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance;
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell,
               CellInteriorDataService::kChanged_FogPlaneNear,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogNearDistance : 0
            );
         }
      };
      void SetFogFarColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 color, bool force, bool fixNear, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogColorFar.abgr = color;
         if (force) {
            if (fixNear && cell->lightingTemplate)
               data->fogColorNear.abgr = cell->lightingTemplate->data.fogColorNear.abgr;
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor;
         }
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell,
               fixNear ?
                  CellInteriorDataService::kChanged_FogColorFar |
                  CellInteriorDataService::kChanged_FogColorNear
               :
                  CellInteriorDataService::kChanged_FogColorFar,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor : 0
            );
         }
      };
      void SetFogMax(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float value, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogMax = value;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogMax;
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell,
               CellInteriorDataService::kChanged_FogMax,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogMax : 0
            );
         }
      };
      void SetFogNearColor(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 color, bool force, bool fixFar, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogColorNear.abgr = color;
         if (force) {
            if (fixFar && cell->lightingTemplate)
               data->fogColorFar.abgr = cell->lightingTemplate->data.fogColorFar.abgr;
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor;
         }
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell,
               fixFar ?
                  CellInteriorDataService::kChanged_FogColorNear |
                  CellInteriorDataService::kChanged_FogColorFar
               :
                  CellInteriorDataService::kChanged_FogColorNear,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogColor : 0
            );
         }
      };
      void SetFogPow(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float value, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->fogPow = value;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_FogPower;
         if (persist) {
            auto& service = CellInteriorDataService::GetInstance();
            service.Modify(
               cell,
               CellInteriorDataService::kChanged_FogPower,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_FogPower : 0
            );
         }
      };
      void SetImageSpace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, RE::TESForm* imageSpace, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         auto casted = DYNAMIC_CAST(imageSpace, TESForm, TESImageSpace);
         ERROR_AND_RETURN_IF(imageSpace && !casted, "The form you specified is not an ImageSpace.", registry, stackId);
         CALL_MEMBER_FN(&cell->extraData, SetExtraCellImageSpace)(casted);
         {  // live-update if current cell
            if (CALL_MEMBER_FN(*RE::g_TES, GetCurrentCell)() == cell) {
               struct Unknown01BA8280 {
                  DEFINE_MEMBER_FN_LONG(Unknown01BA8280, Subroutine00C7BE40, void, 0x00C7BE40, TESImageSpace::Data14*);
               };
               auto data = *(Unknown01BA8280**)0x01BA8280;
               CALL_MEMBER_FN(data, Subroutine00C7BE40)(&casted->unk14);
            }
         }
         if (persist)
            CellInteriorDataService::GetInstance().Modify(cell, CellInteriorDataService::kChanged_ImageSpace, 0);
      };
      void SetLightFadeDistances(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, float planeNear, float planeFar, bool force, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         ERROR_AND_RETURN_IF(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting, "Cannot modify lighting for a cell that uses sky lighting.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         data->lightFadeBegin = planeNear;
         data->lightFadeEnd = planeFar;
         if (force)
            data->inheritFromTemplate &= ~RE::TESObjectCELL::kLightingTemplateUsageFlag_LightFadeDistances;
         if (persist) {
            CellInteriorDataService::GetInstance().Modify(
               cell,
               CellInteriorDataService::kChanged_LightFadeBegin | CellInteriorDataService::kChanged_LightFadeEnd,
               force ? RE::TESObjectCELL::kLightingTemplateUsageFlag_LightFadeDistances : 0
            );
         }
      };
      void SetLightingTemplateUsageFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, SInt32 flags, bool setTo, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         auto data = CALL_MEMBER_FN(cell, GetInteriorData)();
         ERROR_AND_RETURN_IF(data == nullptr, "Cannot modify the lighting data of an exterior cell.", registry, stackId);
         if (setTo) {
            data->inheritFromTemplate |= flags;
         } else {
            data->inheritFromTemplate &= ~flags;
         }
         if (persist)
            CellInteriorDataService::GetInstance().Modify(cell, 0, flags);
      };
      void SetMusicType(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, BGSMusicType* musicType, bool persist) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot modify data on a None cell.", registry, stackId);
         CALL_MEMBER_FN(&cell->extraData, SetExtraCellMusicType)(musicType);
         if (persist)
            CellInteriorDataService::GetInstance().Modify(cell, CellInteriorDataService::kChanged_MusicType, 0);
         if (CALL_MEMBER_FN(*RE::g_TES, GetCurrentCell)() == cell) // live-update if current cell
            RE::SetCellMusic(musicType);
      };
      void StopPersistingChangesTo(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, UInt32 fieldMask) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot reset data for a None cell.", registry, stackId);
         CellInteriorDataService::GetInstance().ResetFields(cell->formID, fieldMask, false);
      };
      void StopPersistingChangedTemplateUsageFlags(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell, UInt32 flagsMask) {
         ERROR_AND_RETURN_IF(cell == nullptr, "Cannot reset data for a None cell.", registry, stackId);
         CellInteriorDataService::GetInstance().ResetTemplateUsageFlags(cell->formID, flagsMask, false);
      };
      SInt32 ToColorCode(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 r, SInt32 g, SInt32 b) {
         // 0xAABBGGRR
         SInt32 result = min(255, max(0, r));
         g = min(255, max(0, g));
         b = min(255, max(0, b));
         result |= (g << 0x8);
         result |= (b << 0x10);
         return result;
      };
      bool UsesSkyLighting(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, RE::TESObjectCELL* cell) {
         ERROR_AND_RETURN_0_IF(cell == nullptr, "Cannot retrieve data from a None cell.", registry, stackId);
         if (!(cell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior))
            return true;
         return !!(cell->unk2C & RE::TESObjectCELL::kCellFlag_UseSkyLighting);
      };
   }
}

bool PapyrusPrefix(Papyrus)::Cell::Register(VMClassRegistry* registry) {
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::TESObjectCELL*, bool>(
         "ForceUniqueLighting",
         PapyrusPrefixString("Cell"),
         Cell::ForceUniqueLighting,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<SInt32>, SInt32>(
         "FromColorCode",
         PapyrusPrefixString("Cell"),
         Cell::FromColorCode,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Cell"), "FromColorCode", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, TESForm*, RE::TESObjectCELL*, SInt32>(
         "GetAcousticSpace",
         PapyrusPrefixString("Cell"),
         Cell::GetAcousticSpace,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, SInt32, RE::TESObjectCELL*, SInt32>(
         "GetAmbientColor",
         PapyrusPrefixString("Cell"),
         Cell::GetAmbientColor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, VMResultArray<SInt32>, RE::TESObjectCELL*, SInt32>(
         "GetDirectionalAmbientColors",
         PapyrusPrefixString("Cell"),
         Cell::GetDirectionalAmbientColors,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, SInt32, RE::TESObjectCELL*, SInt32>(
         "GetDirectionalColor",
         PapyrusPrefixString("Cell"),
         Cell::GetDirectionalColor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, RE::TESObjectCELL*, SInt32>(
         "GetDirectionalFade",
         PapyrusPrefixString("Cell"),
         Cell::GetDirectionalFade,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, VMResultArray<SInt32>, RE::TESObjectCELL*, SInt32>(
         "GetDirectionalRotation",
         PapyrusPrefixString("Cell"),
         Cell::GetDirectionalRotation,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, BSFixedString, RE::TESObjectCELL*>(
         "GetEditorID",
         PapyrusPrefixString("Cell"),
         Cell::GetEditorID,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, BGSEncounterZone*, RE::TESObjectCELL*>(
         "GetEncounterZone",
         PapyrusPrefixString("Cell"),
         Cell::GetEncounterZone,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<SInt32>, RE::TESObjectCELL*>(
         "GetExteriorCellCoordinates",
         PapyrusPrefixString("Cell"),
         Cell::GetExteriorCellCoordinates,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<float>, RE::TESObjectCELL*>(
         "GetExteriorUnitCoordinates",
         PapyrusPrefixString("Cell"),
         Cell::GetExteriorUnitCoordinates,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, RE::TESObjectCELL*, SInt32>(
         "GetFogClipDistance",
         PapyrusPrefixString("Cell"),
         Cell::GetFogClipDistance,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, VMResultArray<SInt32>, RE::TESObjectCELL*, SInt32>(
         "GetFogColors",
         PapyrusPrefixString("Cell"),
         Cell::GetFogColors,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, VMResultArray<float>, RE::TESObjectCELL*, SInt32>(
         "GetFogDistances",
         PapyrusPrefixString("Cell"),
         Cell::GetFogDistances,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, RE::TESObjectCELL*, SInt32>(
         "GetFogMax",
         PapyrusPrefixString("Cell"),
         Cell::GetFogMax,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, float, RE::TESObjectCELL*, SInt32>(
         "GetFogPower",
         PapyrusPrefixString("Cell"),
         Cell::GetFogPower,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, TESForm*, RE::TESObjectCELL*, SInt32>(
         "GetImageSpace",
         PapyrusPrefixString("Cell"),
         Cell::GetImageSpace,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, VMResultArray<float>, RE::TESObjectCELL*, SInt32>(
         "GetLightFadeDistances",
         PapyrusPrefixString("Cell"),
         Cell::GetLightFadeDistances,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, RE::TESForm*, RE::TESObjectCELL*>(
         "GetLightingTemplate",
         PapyrusPrefixString("Cell"),
         Cell::GetLightingTemplate,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, bool, RE::TESObjectCELL*, SInt32>(
         "GetLightingTemplateUsageFlag",
         PapyrusPrefixString("Cell"),
         Cell::GetLightingTemplateUsageFlag,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, RE::TESObjectCELL*>(
         "GetLightingTemplateUsageFlags",
         PapyrusPrefixString("Cell"),
         Cell::GetLightingTemplateUsageFlags,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, bool, RE::TESObjectCELL*, SInt32>(
         "GetLightingTemplateUsageDefaultFlag",
         PapyrusPrefixString("Cell"),
         Cell::GetLightingTemplateUsageDefaultFlag,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, RE::TESObjectCELL*>(
         "GetLightingTemplateUsageDefaultFlags",
         PapyrusPrefixString("Cell"),
         Cell::GetLightingTemplateUsageDefaultFlags,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, BGSMusicType*, RE::TESObjectCELL*, SInt32>(
         "GetMusicType",
         PapyrusPrefixString("Cell"),
         Cell::GetMusicType,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, float, RE::TESObjectCELL*>(
         "GetNorthRotation",
         PapyrusPrefixString("Cell"),
         Cell::GetNorthRotation,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, RE::TESWorldSpace*, RE::TESObjectCELL*>(
         "GetParentWorldspace",
         PapyrusPrefixString("Cell"),
         Cell::GetParentWorldspace,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, RE::TESObjectCELL*>(
         "IsPublic",
         PapyrusPrefixString("Cell"),
         Cell::IsPublic,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::TESObjectCELL*, UInt32>(
         "ResetFields",
         PapyrusPrefixString("Cell"),
         Cell::ResetFields,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::TESObjectCELL*, UInt32>(
         "ResetTemplateUsageFlags",
         PapyrusPrefixString("Cell"),
         Cell::ResetTemplateUsageFlags,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, void, RE::TESObjectCELL*, RE::TESForm*, bool>(
         "SetAcousticSpace",
         PapyrusPrefixString("Cell"),
         Cell::SetAcousticSpace,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction5<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, bool, bool, bool>(
         "SetAmbientColor",
         PapyrusPrefixString("Cell"),
         Cell::SetAmbientColor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction5<StaticFunctionTag, void, RE::TESObjectCELL*, VMArray<SInt32>, bool, bool, bool>(
         "SetDirectionalAmbientColors",
         PapyrusPrefixString("Cell"),
         Cell::SetDirectionalAmbientColors,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, bool, bool>(
         "SetDirectionalColor",
         PapyrusPrefixString("Cell"),
         Cell::SetDirectionalColor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, float, bool, bool>(
         "SetDirectionalFade",
         PapyrusPrefixString("Cell"),
         Cell::SetDirectionalFade,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, VMArray<SInt32>, bool, bool>(
         "SetDirectionalRotation",
         PapyrusPrefixString("Cell"),
         Cell::SetDirectionalRotation,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction10<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, SInt32, float, float, float, float, float, bool, bool>(
         "SetFog",
         PapyrusPrefixString("Cell"),
         Cell::SetFog,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction5<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, SInt32, bool, bool>(
         "SetFogColors",
         PapyrusPrefixString("Cell"),
         Cell::SetFogColors,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction6<StaticFunctionTag, void, RE::TESObjectCELL*, float, float, float, bool, bool>(
         "SetFogDistances",
         PapyrusPrefixString("Cell"),
         Cell::SetFogDistances,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, float, bool, bool>(
         "SetFogDistanceClip",
         PapyrusPrefixString("Cell"),
         Cell::SetFogDistanceClip,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, float, bool, bool>(
         "SetFogDistanceFar",
         PapyrusPrefixString("Cell"),
         Cell::SetFogDistanceFar,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, float, bool, bool>(
         "SetFogDistanceNear",
         PapyrusPrefixString("Cell"),
         Cell::SetFogDistanceNear,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction5<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, bool, bool, bool>(
         "SetFogFarColor",
         PapyrusPrefixString("Cell"),
         Cell::SetFogFarColor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, float, bool, bool>(
         "SetFogMax",
         PapyrusPrefixString("Cell"),
         Cell::SetFogMax,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction5<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, bool, bool, bool>(
         "SetFogNearColor",
         PapyrusPrefixString("Cell"),
         Cell::SetFogNearColor,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, float, bool, bool>(
         "SetFogPow",
         PapyrusPrefixString("Cell"),
         Cell::SetFogPow,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, void, RE::TESObjectCELL*, RE::TESForm*, bool>(
         "SetImageSpace",
         PapyrusPrefixString("Cell"),
         Cell::SetImageSpace,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction5<StaticFunctionTag, void, RE::TESObjectCELL*, float, float, bool, bool>(
         "SetLightFadeDistances",
         PapyrusPrefixString("Cell"),
         Cell::SetLightFadeDistances,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction4<StaticFunctionTag, void, RE::TESObjectCELL*, SInt32, bool, bool>(
         "SetLightingTemplateUsageFlags",
         PapyrusPrefixString("Cell"),
         Cell::SetLightingTemplateUsageFlags,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, void, RE::TESObjectCELL*, BGSMusicType*, bool>(
         "SetMusicType",
         PapyrusPrefixString("Cell"),
         Cell::SetMusicType,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::TESObjectCELL*, UInt32>(
         "StopPersistingChangesTo",
         PapyrusPrefixString("Cell"),
         Cell::StopPersistingChangesTo,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, RE::TESObjectCELL*, UInt32>(
         "StopPersistingChangedTemplateUsageFlags",
         PapyrusPrefixString("Cell"),
         Cell::StopPersistingChangedTemplateUsageFlags,
         registry
      )
   );
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, SInt32, SInt32, SInt32, SInt32>(
         "ToColorCode",
         PapyrusPrefixString("Cell"),
         Cell::ToColorCode,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Cell"), "ToColorCode", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, bool, RE::TESObjectCELL*>(
         "UsesSkyLighting",
         PapyrusPrefixString("Cell"),
         Cell::UsesSkyLighting,
         registry
      )
   );
   //
   return true;
};