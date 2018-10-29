#pragma once
#include "skse/NiTypes.h"
#include "CobbRotation.h"

struct Bounds {
   NiPoint3 min;
   NiPoint3 max;
   Cobb::Euler rotation;
   //
   Bounds() {
      this->min.x = 0;
      this->min.y = 0;
      this->min.z = 0;
      this->max.x = 0;
      this->max.y = 0;
      this->max.z = 0;
   };
   Bounds(float x1, float y1, float z1, float x2, float y2, float z2) {
      this->min.x = x1;
      this->min.y = y1;
      this->min.z = z1;
      this->max.x = x2;
      this->max.y = y2;
      this->max.z = z2;
   }
   //
   NiPoint3 center() const;
   float    lengthAlongAxis(const NiPoint3& axis) const; // Doesn't apply the (rotation) field. Normalizes the axis for you (I think).
   float    volume() const;
   bool SeparatingAxisTest(NiPoint3 position, NiPoint3 origin) {
      NiPoint3 localSearchPoint;
      NiPoint3 localPosition;
      {  // Get local search point and axes.
         Cobb::Matrix local = ((Cobb::Matrix)this->rotation).Transpose();
         localSearchPoint = local.ApplyAsFrame(origin);
         localPosition = local.ApplyAsFrame(position);
      }
      {  // Separating axis theorem: bounding box local X
         float pos = localSearchPoint.x;
         float min = localPosition.x + this->min.x;
         float max = localPosition.x + this->max.x;
         if (pos < min || pos > max)
            return false;
      }
      {  // Separating axis theorem: bounding box local Y
         float pos = localSearchPoint.y;
         float min = localPosition.y + this->min.y;
         float max = localPosition.y + this->max.y;
         if (pos < min || pos > max)
            return false;
      }
      {  // Separating axis theorem: bounding box local Z
         float pos = localSearchPoint.z;
         float min = localPosition.z + this->min.z;
         float max = localPosition.z + this->max.z;
         if (pos < min || pos > max)
            return false;
      }
      return true;
   };
   //
   explicit operator bool() const {
      return (min.x != 0 || min.y != 0 || min.z != 0 || max.x != 0 || max.y != 0 || max.z != 0);
   }
   bool operator!() {
      return (min.x == 0 && min.y == 0 && min.z == 0 && max.x == 0 && max.y == 0 && max.z == 0);
   }
   Bounds& operator*= (const float& scale) {
      this->min *= scale;
      this->max *= scale;
      return *this;
   }
   Bounds& operator+= (const float& margin) {
      this->min.x -= margin;
      this->min.y -= margin;
      this->min.z -= margin;
      this->max.x += margin;
      this->max.y += margin;
      this->max.z += margin;
      return *this;
   }
};