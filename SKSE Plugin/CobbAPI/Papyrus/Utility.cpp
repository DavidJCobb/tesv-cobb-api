#include "Papyrus/Utility.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameForms.h" // TESForm
#include "skse/GameObjects.h" // ActiveEffect

#include "Miscellaneous/math.h"
#include "Types/bounds.h"

namespace CobbPapyrus {
   namespace Utility {
      template<typename T> bool ScriptObjectWrappedPointerExists(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, T* entity) {
         return entity != nullptr;
      };
      //
      namespace BoundsFuncs {
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
         }
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
         }
      }
      namespace Math {
         VMResultArray<float> GetGoodObjectSpawnCoordinates(
            VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
            VMArray<float> boundsArr, VMArray<float> playerCoordinates, UInt32 iFlags
         ) {
            VMResultArray<float> result;
            if (playerCoordinates.Length() < 6) {
               registry->LogError("You need to specify the coordinates of the player -- position and rotation, as six floats.", stackId);
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
            float distance = 256.0F;
            float length;
            if (iFlags & 1) { // object should match player yaw
               length = bounds.max.y - bounds.min.y;
               result[5] = cobb::radians_to_degrees(playerYaw + cobb::pi);
            } else {
               length = bounds.lengthAlongAxis(playerAxis);
               result[5] = 0.0F;
            }
            if (length > 224.0F) {
               float linear = length * 1.5F + 512.0F;
               float exponential = length * length / 256.0F; // consider instead: 256 + ((x - 224) * ((x - 224) / 256))
               distance = max(256.0F, min(linear, exponential));
            }
            //
            // Above formula caps length at roughly 512 units away from the edge of the object.
            // A concern to note: an object's apparent length can vary wildly if the user is 
            // looking at it diagonally (i.e. facing an AABB corner instead of an edge).
            //
            NiPoint3 position = playerPos + (playerAxis * distance);
            result[0] = position.x;
            result[1] = position.y;
            result[2] = position.z;
            result[3] = 0.0F;
            result[4] = 0.0F;
            return result;
         }
      }
   }
}

bool CobbPapyrus::Utility::Register(VMClassRegistry* registry) {
   {  // "Exists" methods
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, bool, TESForm*>(
            "FormExists",
            PapyrusPrefixString("Utility"),
            Utility::ScriptObjectWrappedPointerExists<TESForm>,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "FormExists", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, bool, ActiveEffect*>(
            "ActiveEffectExists",
            PapyrusPrefixString("Utility"),
            Utility::ScriptObjectWrappedPointerExists<ActiveEffect>,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "ActiveEffectExists", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Bounds utilities
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMResultArray<float>, VMArray<float>>(
         "GetBoundsToHalfwidths",
         PapyrusPrefixString("Utility"),
         Utility::BoundsFuncs::GetBoundsToHalfwidths,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "GetBoundsToHalfwidths", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, VMResultArray<float>, VMArray<float>>(
         "GetBoundsToHalfwidthOffsets",
         PapyrusPrefixString("Utility"),
         Utility::BoundsFuncs::GetBoundsToHalfwidthOffsets,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "GetBoundsToHalfwidthOffsets", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Math utilities
      registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMResultArray<float>, VMArray<float>, VMArray<float>, UInt32>(
         "GetGoodObjectSpawnCoordinates",
         PapyrusPrefixString("Utility"),
         Utility::Math::GetGoodObjectSpawnCoordinates,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("Utility"), "GetGoodObjectSpawnCoordinates", VMClassRegistry::kFunctionFlag_NoWait);
   }
   //
   return true;
};