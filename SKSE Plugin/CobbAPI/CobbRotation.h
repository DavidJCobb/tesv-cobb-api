#pragma once
#include <array>
#include "skse/NiTypes.h"

namespace Cobb {
   class AxisAngle;
   class Euler;
   class Matrix;
   class Quaternion;

   class AxisAngle {
      public:
         float x = 0.0;
         float y = 0.0;
         float z = 0.0;
         float angle = 0.0;
         bool isRadians = true;
         //
         AxisAngle() {};
         AxisAngle(float x, float y, float z, float angle, bool isRadians = true) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->angle = angle;
            this->isRadians = isRadians;
         };
         //
         void ConvertToDegrees();
         void ConvertToRadians();
         float Length() {
            return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
         };
         void Normalize() {
            float length = Length();
            this->x /= length;
            this->y /= length;
            this->z /= length;
         };
         //
         explicit operator Euler() const;
         explicit operator Matrix() const;
         explicit operator Quaternion() const;
   };
   class Euler {
      public:
         float x = 0.0;
         float y = 0.0;
         float z = 0.0;
         bool isRadians = true;
         //
         Euler() {};
         Euler(float x, float y, float z, bool isRadians = true) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->isRadians = isRadians;
         };
         Euler(const NiPoint3& pos, bool isRadians = true) {
            this->x = pos.x;
            this->y = pos.y;
            this->z = pos.z;
            this->isRadians = isRadians;
         }
         //
         void ConvertToDegrees();
         void ConvertToRadians();
         //
         explicit operator AxisAngle() const;
         explicit operator Matrix() const;     // Fun fact: an object's transformation matrix is literally just its rotation cast to a matrix.
         explicit operator Quaternion() const;
         explicit operator NiPoint3() const {
            return NiPoint3(this->x, this->y, this->z);
         };
   };
   class Matrix {
      public:
         float data[3][3];
         //
         Matrix() {};
         Matrix(std::array<float, 9> afData) {
            for (int iIterator = 0; iIterator < 9; iIterator++) {
               int x = iIterator % 3;
               int y = (iIterator - x) / 3;
               this->data[y][x] = afData[iIterator];
            };
         };
         //
         float element(int index) const {
            int x = index % 3;
            int y = (index - x) / 3;
            return this->data[y][x];
         };
         void element(int index, float value) {
            int x = index % 3;
            int y = (index - x) / 3;
            this->data[y][x] = value;
         };
         //
         std::array<float, 3> MultiplyByColumn(std::array<float, 3> avColumn) const {
            return this->MultiplyByColumn(avColumn[0], avColumn[1], avColumn[2]);
         }
         std::array<float, 3> Matrix::MultiplyByColumn(float avColumn[3]) const {
            return this->MultiplyByColumn(avColumn[0], avColumn[1], avColumn[2]);
         }
         std::array<float, 3> Matrix::MultiplyByColumn(float x, float y, float z) const {
            std::array<float, 3> vResult;
            vResult[0] = (this->data[0][0] * x) + (this->data[0][1] * y) + (this->data[0][2] * z);
            vResult[1] = (this->data[1][0] * x) + (this->data[1][1] * y) + (this->data[1][2] * z);
            vResult[2] = (this->data[2][0] * x) + (this->data[2][1] * y) + (this->data[2][2] * z);
            return vResult;
         }
         NiPoint3 Matrix::MultiplyByColumn(NiPoint3 pos) const {
            NiPoint3 vResult;
            vResult.x = (this->data[0][0] * pos.x) + (this->data[0][1] * pos.y) + (this->data[0][2] * pos.z);
            vResult.y = (this->data[1][0] * pos.x) + (this->data[1][1] * pos.y) + (this->data[1][2] * pos.z);
            vResult.z = (this->data[2][0] * pos.x) + (this->data[2][1] * pos.y) + (this->data[2][2] * pos.z);
            return vResult;
         };
         float Trace() const {
            return data[0][0] + data[1][1] + data[2][2];
         };
         Matrix Transpose() const {
            Matrix output;
            output.data[0][0] = data[0][0];
            output.data[0][1] = data[1][0];
            output.data[0][2] = data[2][0];
            output.data[1][0] = data[0][1];
            output.data[1][1] = data[1][1];
            output.data[1][2] = data[2][1];
            output.data[2][0] = data[0][2];
            output.data[2][1] = data[1][2];
            output.data[2][2] = data[2][2];
            return output;
         };
         //
         NiPoint3 ApplyAsFrame(NiPoint3 target) const {
            NiPoint3 result(0, 0, 0);
            result.x = this->data[0][0] * target.x + this->data[0][1] * target.y + this->data[0][2] * target.z;
            result.y = this->data[1][0] * target.x + this->data[1][1] * target.y + this->data[1][2] * target.z;
            result.z = this->data[2][0] * target.x + this->data[2][1] * target.y + this->data[2][2] * target.z;
            return result;
         };
         //
         explicit operator AxisAngle() const;
         explicit operator Euler() const;
         explicit operator Quaternion() const;
   };
   class Quaternion {
      public:
         float w = 0.0;
         float x = 0.0;
         float y = 0.0;
         float z = 0.0;
         //
         Quaternion() {};
         Quaternion(float w, float x, float y, float z) {
            this->w = w;
            this->x = x;
            this->y = y;
            this->z = z;
         };
         Quaternion(Quaternion* copy) {
            this->w = copy->w;
            this->x = copy->x;
            this->y = copy->y;
            this->z = copy->z;
         };
         //
         Quaternion Conjugate() {
            return Quaternion(w, -x, -y, -z);
         };
         Quaternion Inverse() {
            Quaternion result = this->Conjugate();
            result = result / pow(this->Norm(), 2);
            return result;
         };
         Quaternion Normalize() {
            Quaternion result = Quaternion(this);
            result = result / Norm();
            return result;
         };
         float Norm() {
            return sqrt(this->w*this->w + this->x*this->x + this->y*this->y + this->z*this->z);
         };
         //
         Quaternion  Quaternion::operator+(const Quaternion& other) {
            Quaternion result;
            result.w = this->w + other.w;
            result.x = this->x + other.x;
            result.y = this->y + other.y;
            result.z = this->z + other.z;
            return result;
         };
         Quaternion  Quaternion::operator*(const Quaternion& other) {
            Quaternion result;
            result.w = (this->w * other.w) - (this->x * other.x) - (this->y * other.y) - (this->z * other.z);
            result.x = (this->w * other.x) + (this->x * other.w) + (this->y * other.z) - (this->z * other.y);
            result.y = (this->w * other.y) - (this->x * other.z) + (this->y * other.w) + (this->z * other.x);
            result.z = (this->w * other.z) + (this->x * other.y) - (this->y * other.x) + (this->z * other.w);
            return result;
         };
         Quaternion  Quaternion::operator*(const float& other) {
            Quaternion result;
            result.w = this->w * other;
            result.x = this->x * other;
            result.y = this->y * other;
            result.z = this->z * other;
            return result;
         };
         Quaternion  Quaternion::operator/(const float& other) {
            Quaternion result;
            result.w = this->w / other;
            result.x = this->x / other;
            result.y = this->y / other;
            result.z = this->z / other;
            return result;
         };
         //
         explicit operator AxisAngle() const;
         explicit operator Euler() const;
         explicit operator Matrix() const;
   };

   struct Coordinates {
      NiPoint3 pos;
      Euler    rot;
      //
      Coordinates() {};
      Coordinates(const NiPoint3& pos, const Euler& rot) : rot(rot) {
         this->pos.x = pos.x;
         this->pos.y = pos.y;
         this->pos.z = pos.z;
      }
      Coordinates(const NiPoint3& pos, const NiPoint3& rot, bool isRadians = true) : rot(rot, isRadians) {
         this->pos.x = pos.x;
         this->pos.y = pos.y;
         this->pos.z = pos.z;
      };
      Coordinates(const Coordinates& other) : pos(other.pos), rot(other.rot) {};
   };

   static Coordinates& GetRelativeCoordinates(Coordinates& out, const Coordinates& parent, const Coordinates& target) {
      {  // Get output rotation.
         Quaternion qParent = (Quaternion) parent.rot;
         Quaternion qTarget = (Quaternion) target.rot;
         out.rot = (Euler)(qParent.Inverse() * qTarget);
      }
      {  // Get output position.
         Cobb::Matrix mGlobal = ((Cobb::Matrix)parent.rot).Transpose();
         out.pos = mGlobal.MultiplyByColumn(target.pos);
         //
         // We need to convert the parent's world-relative position to parent-relative coordinates 
         // and then subtract it from our outputPosition.
         //
         NiPoint3 parentPositionLocal = mGlobal.MultiplyByColumn(parent.pos);
         out.pos -= parentPositionLocal;
      }
      return out;
   };
   static Coordinates& GetRelativeCoordinates(Coordinates& out, const NiPoint3& parentPos, const NiPoint3& parentRot, const NiPoint3& targetPos, const NiPoint3& targetRot, bool abParentIsRadians = true, bool abTargetIsRadians = true) {
      Coordinates parent(parentPos, parentRot, abParentIsRadians);
      Coordinates target(targetPos, targetRot, abTargetIsRadians);
      return GetRelativeCoordinates(out, parent, target);
   };
   static Coordinates& ApplyRelativeCoordinates(Coordinates& out, const Coordinates& parent, const Coordinates& offset) {
      {  // Construct position.
         if (offset.pos.x != 0 || offset.pos.y != 0 || offset.pos.z != 0) {
            out.pos = ((Matrix)parent.rot).MultiplyByColumn(offset.pos) + parent.pos;
         } else {
            out.pos = parent.pos;
         }
      }
      {  // Construct rotation.
         Quaternion qParent = (Quaternion) parent.rot;
         Quaternion qOffset = (Quaternion) offset.rot;
         out.rot = (Euler)(qParent * qOffset);
      }
      return out;
   };
   static Coordinates& ApplyRelativeCoordinates(Coordinates& out, const NiPoint3& parentPos, const NiPoint3& parentRot, const NiPoint3& offsetPos, const NiPoint3& offsetRot, bool abParentIsRadians = true, bool abOffsetIsRadians = true) {
      Coordinates parent(parentPos, parentRot, abParentIsRadians);
      Coordinates offset(offsetPos, offsetRot, abOffsetIsRadians);
      return ApplyRelativeCoordinates(out, parent, offset);
   };
};