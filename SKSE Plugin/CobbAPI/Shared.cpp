#include "Shared.h"

#include "skse/GameAPI.h"

namespace cobb {
   std::string stringf(const char* format, ...) {
      va_list args;
      va_start(args, format);
      //
      std::string result;
      //
      char buffer[32];
      size_t count = std::vsnprintf(buffer, sizeof(buffer), format, args);
      if (count < sizeof(buffer)) {
         result = buffer;
      } else {
         char* buffer = (char*) malloc(count + 1);
         std::vsnprintf(buffer, count + 1, format, args);
         result = buffer;
         free(buffer);
      }
      //
      va_end(args);
      return result;
   }
};

/*
namespace BhkCollisionObjectIteration {
   void _GetMotionType(bhkCollisionObject* collision, ParametersGetMT* parameters) {
      UInt8 current = collision->unk10->unk08->motionSystem; // VS is compiling this as collision->unk0C->unk08, i.e. it doesn't know that collision inherits an unk04 from a superclass
      if (parameters->result == -1) {
         parameters->result = current;
      } else if (parameters->result != 0 && parameters->result != current) {
         parameters->result = 0;
      }
   };
   void _SetMotionType(bhkCollisionObject* collision, ParametersSetMT* parameters) {
      collision->SetMotionSystem(parameters->motionType, 0, parameters->unknown);
   };

   SInt8 GetMotionTypeOnNodeTree(NiNode* root) {
      //
      // Returns -1 if there are no collision objects, and 0 if there are multiple collision objects with inconsistent motion types.
      // Returns the motion type on all collision objects, otherwise.
      //
      ParametersGetMT parameters;
      parameters.result = -1;
      IterateOverBhkCollisionObjects(root, (Parameters*)&parameters, (IteratorFunction*)&_GetMotionType);
      return parameters.result;
   };
   void SetMotionTypeOnNodeTree(NiNode* root, UInt8 motionType) {
      ParametersSetMT parameters;
      parameters.motionType = motionType;
      parameters.unknown = false; // until we know what this is
      IterateOverBhkCollisionObjects(root, &parameters, (IteratorFunction*)&_SetMotionType);
   };
}
*/