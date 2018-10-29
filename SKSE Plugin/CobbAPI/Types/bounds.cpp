#include "bounds.h"

#include "bounds.h"

// Useful note: dot(x, x) == length(x) ^ 2
float _dotProduct(const NiPoint3* a, const NiPoint3* b) {
   return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
};
void _vectorProjection(NiPoint3* outVector, const NiPoint3* axis) {
   float primary = _dotProduct(outVector, axis) / _dotProduct(axis, axis);
   outVector->x = primary * axis->x;
   outVector->y = primary * axis->y;
   outVector->z = primary * axis->z;
}

NiPoint3 Bounds::center() const {
   NiPoint3 result;
   result.x = (this->max.x - this->min.x) / 2 + this->min.x;
   result.y = (this->max.y - this->min.y) / 2 + this->min.y;
   result.z = (this->max.z - this->min.z) / 2 + this->min.z;
   return result;
}
float Bounds::volume() const {
   float x = abs(this->max.x - this->min.x);
   float y = abs(this->max.y - this->min.y);
   float z = abs(this->max.z - this->min.z);
   return x * y * z;
}
float Bounds::lengthAlongAxis(const NiPoint3& axis) const {
   /*NiPoint3 halfwidthX(0, 0, 0);
   NiPoint3 halfwidthY(0, 0, 0);
   NiPoint3 halfwidthZ(0, 0, 0);
   halfwidthX.x = (this->max.x - this->min.x) / 2;
   halfwidthY.y = (this->max.y - this->min.y) / 2;
   halfwidthZ.z = (this->max.z - this->min.z) / 2;
   _vectorProjection(&halfwidthX, &axis);
   _vectorProjection(&halfwidthY, &axis);
   _vectorProjection(&halfwidthZ, &axis);
   halfwidthX += halfwidthY; // note, a += b + c seems buggy; I'm getting null NiPoint3s from it for some reason
   halfwidthX += halfwidthZ;
   return sqrt(_dotProduct(&halfwidthX, &halfwidthX)) * 2;*/
   //
   // The math below is optimized for AABBs. If we ever pre-rotate the bounding boxes (e.g. rotating spawned 
   // items to face the player), we'll have to use non-axis-aligned halfwidth vectors, meaning we'll need to 
   // go back to the math above (separate operations for each halfwidth).
   //
   // If we do decide to honor the OBB's rotation, just use its (rotation) field. Maybe add a bool argument 
   // to allow callers to ignore the rotation setting and use the faster AABB calculation.
   //
   NiPoint3 halfwidths;
   halfwidths.x = (this->max.x - this->min.x) / 2;
   halfwidths.y = (this->max.y - this->min.y) / 2;
   halfwidths.z = (this->max.z - this->min.z) / 2;
   float axisLengthSquared = _dotProduct(&axis, &axis); // if axis isn't normalized, this effectively helps us do that
   halfwidths.x = (halfwidths.x * axis.x) / axisLengthSquared * axis.x;
   halfwidths.y = (halfwidths.y * axis.y) / axisLengthSquared * axis.y;
   halfwidths.z = (halfwidths.z * axis.z) / axisLengthSquared * axis.z;
   return sqrt(_dotProduct(&halfwidths, &halfwidths)) * 2;
}