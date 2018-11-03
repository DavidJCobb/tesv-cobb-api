#include "Papyrus/Editor.h"
#include "Services/DisplayOptionService.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "Miscellaneous/math.h"
#include "CobbRotation.h"
#include "Types/bounds.h"

namespace CobbPapyrus {
   namespace Editor {
      void SetDisplayOptionGlobals(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<TESGlobal*> forms) {
         TESGlobal* optionGizmos  = nullptr;
         TESGlobal* optionShaders = nullptr;
         forms.Get(&optionGizmos, 0);
         forms.Get(&optionShaders, 1);
         DisplayOptionService::GetInstance().SetGizmoGlobal(optionGizmos);
         DisplayOptionService::GetInstance().SetShaderGlobal(optionShaders);
      };
      void SetGizmos(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<TESObjectACTI*> forms) {
         TESObjectACTI* normal;
         TESObjectACTI* large;
         forms.Get(&normal, 0);
         forms.Get(&large, 1);
         DisplayOptionService::GetInstance().SetGizmoForms(normal, large);
      };
      void SetShaders(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<TESEffectShader*> forms) {
         TESEffectShader* selection;
         TESEffectShader* snap;
         forms.Get(&selection, 0);
         forms.Get(&snap, 1);
         DisplayOptionService::GetInstance().SetShaderForms(selection, snap);
      };
      void ApplyDisplayOptions(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*) {
         DisplayOptionService::GetInstance().UpdateForms();
      };
      //
      VMResultArray<float> GetBoundsToHalfwidthOffsets(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> bounds
      ) {
         auto offset = [](SInt32 low, SInt32 high) { return ((high - low) / 2) + low; };
         //
         VMResultArray<float> result;
         NiPoint3 min;
         NiPoint3 max;
         bounds.Get(&min.x, 0);
         bounds.Get(&min.y, 1);
         bounds.Get(&min.z, 2);
         bounds.Get(&max.x, 3);
         bounds.Get(&max.y, 4);
         bounds.Get(&max.z, 5);
         //
         result.resize(3);
         result[0] = offset(max.x, min.x);
         result[1] = offset(max.y, min.y);
         result[2] = offset(max.z, min.z);
         return result;
      };
      VMResultArray<float> GetBoundsToHalfwidths(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> bounds
      ) {
         VMResultArray<float> result;
         NiPoint3 min;
         NiPoint3 max;
         bounds.Get(&min.x, 0);
         bounds.Get(&min.y, 1);
         bounds.Get(&min.z, 2);
         bounds.Get(&max.x, 3);
         bounds.Get(&max.y, 4);
         bounds.Get(&max.z, 5);
         //
         result.resize(3);
         result[0] = abs(max.x - min.x) / 2;
         result[1] = abs(max.y - min.y) / 2;
         result[2] = abs(max.z - min.z) / 2;
         return result;
      };
      //
      VMResultArray<float> GetStorageServiceRecallCoordinates(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> boundsArr, VMArray<float> playerCoordinates, UInt32 iFlags
      ) {
         VMResultArray<float> result;
         if (playerCoordinates.Length() < 6) {
            registry->LogError("You need to specify the coordinates of the drop -- position and rotation, as six floats.", stackId);
            return result;
         }
         result.resize(6);
         //
         NiPoint3 playerPos;
         NiPoint3 playerAxis;
         playerCoordinates.Get(&playerPos.x, 0);
         playerCoordinates.Get(&playerPos.y, 1);
         playerCoordinates.Get(&playerPos.z, 2);
         float playerYaw;
         {
            playerCoordinates.Get(&playerYaw, 5);
            playerYaw = cobb::degrees_to_radians(playerYaw);
            playerAxis.x = sin(playerYaw);
            playerAxis.y = cos(playerYaw);
         }
         //
         Bounds bounds;
         {  // Unpack bounds.
            if (boundsArr.Length() > 2) {
               boundsArr.Get(&bounds.min.x, 0);
               boundsArr.Get(&bounds.min.y, 1);
               boundsArr.Get(&bounds.min.z, 2);
            }
            if (boundsArr.Length() > 5) {
               boundsArr.Get(&bounds.max.x, 3);
               boundsArr.Get(&bounds.max.y, 4);
               boundsArr.Get(&bounds.max.z, 5);
            }
         }
         {  // Compute target distance.
            float distance = 256.0;
            float length = bounds.lengthAlongAxis(playerAxis);
            if (length > 224.0) {
               float linear = length * 1.5 + 512.0;
               float exponential = length * length / 256.0; // consider instead: 256 + ((x - 224) * ((x - 224) / 256))
               distance = max(256, min(linear, exponential));
            }
            //
            // Above formula caps length at roughly 512 units away from the edge of the object.
            // A concern to note: an object's apparent length can vary wildly if the user is 
            // looking at it diagonally (i.e. facing an AABB corner instead of an edge).
            //
            {
               NiPoint3 position = playerPos + (playerAxis * distance);
               result[0] = position.x;
               result[1] = position.y;
               result[2] = position.z;
               //
               if (iFlags & 3) { // Flag: Rotate objects to match player yaw
                  /*NiPoint3 dummy(0, 0, 0);*/
                  NiPoint3 player(0, 0, 0);
                  if (iFlags & 1) {
                     player.z = playerYaw;
                  }
                  /*if (iFlags & 2) {
                     //
                     // TODO: Actually these would affect position, but the rotation math on all 
                     // this is screwy enough as it is.
                     //
                     playerCoordinates.Get(&player.x, 3); // TODO: we'd need to convert to radians, too
                     playerCoordinates.Get(&player.y, 4); // TODO: we'd need to convert to radians, too
                  }*/
                  //
                  /*Cobb::Coordinates out;
                  //Cobb::GetRelativeCoordinates(&out, &dummy, &player, &dummy, &dummy, false, false);
                  Cobb::ApplyRelativeCoordinates(&out, &position, &player, &dummy, &dummy, false, false);
                  out.rot.ConvertToDegrees();
                  result[3] = out.rot.x;
                  result[4] = out.rot.y;
                  result[5] = out.rot.z;*/
                  result[3] = cobb::radians_to_degrees(player.x);
                  result[4] = cobb::radians_to_degrees(player.y);
                  result[5] = cobb::radians_to_degrees(player.z + cobb::pi);
               }
            }
         }
         return result;
      }
   }
}

bool CobbPapyrus::Editor::Register(VMClassRegistry* registry) {
   #define REGISTER_SETTER_NOWAIT(fnc, name, type) \
      registry->RegisterFunction(\
         new NativeFunction1<StaticFunctionTag, void, VMArray<type>>(\
            name,\
            PapyrusPrefixString("Editor"),\
            Editor::fnc,\
            registry\
         )\
      );\
      registry->SetFunctionFlags(PapyrusPrefixString("Editor"), name, VMClassRegistry::kFunctionFlag_NoWait);

   REGISTER_SETTER_NOWAIT(SetDisplayOptionGlobals, "SetDisplayOptionGlobals", TESGlobal*);
   REGISTER_SETTER_NOWAIT(SetGizmos,  "SetGizmos",  TESObjectACTI*);
   REGISTER_SETTER_NOWAIT(SetShaders, "SetShaders", TESEffectShader*);

   registry->RegisterFunction(
      new NativeFunction0 <StaticFunctionTag, void>(
         "ApplyDisplayOptions",
         PapyrusPrefixString("Editor"),
         Editor::ApplyDisplayOptions,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Editor"), "ApplyDisplayOptions", VMClassRegistry::kFunctionFlag_NoWait);

   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>, UInt32>(
         "GetStorageServiceRecallCoordinates",
         PapyrusPrefixString("Editor"),
         Editor::GetStorageServiceRecallCoordinates,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Editor"), "GetStorageServiceRecallCoordinates", VMClassRegistry::kFunctionFlag_NoWait);
   
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<float>, VMArray<float>>(
         "GetBoundsToHalfwidths",
         PapyrusPrefixString("Editor"),
         Editor::GetBoundsToHalfwidths,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Editor"), "GetBoundsToHalfwidths", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, VMResultArray<float>, VMArray<float>>(
         "GetBoundsToHalfwidthOffsets",
         PapyrusPrefixString("Editor"),
         Editor::GetBoundsToHalfwidthOffsets,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("Editor"), "GetBoundsToHalfwidthOffsets", VMClassRegistry::kFunctionFlag_NoWait);

   return true;
};