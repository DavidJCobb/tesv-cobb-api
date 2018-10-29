#include <array>
#include <math.h>
//#include <vector>

#include "CobbRotation.h"

constexpr double PI = 3.14159265358979323846;

namespace Cobb {
   //
   // AXIS-ANGLE
   //
   void AxisAngle::ConvertToDegrees() {
      if (!this->isRadians)
         return;
      this->angle /= (PI / 180.0);
      this->isRadians = false;
   };
   void AxisAngle::ConvertToRadians() {
      if (this->isRadians)
         return;
      this->angle *= PI / 180.0;
      this->isRadians = true;
   };
   AxisAngle::operator Euler() const {
      return (Euler)(Matrix)*this;
   };
   AxisAngle::operator Matrix() const {
      Matrix output;
      //
      float _angle = angle;
      if (!this->isRadians)
         _angle = _angle *= PI / 180.0;
      //
      float fCos = cos(_angle);
      float fSin = sin(_angle);
      float fOneMinusCos = (1 - fCos);
      //
      output.element(0, fCos + pow(x, 2) * fOneMinusCos);
      output.element(1, x * y * fOneMinusCos - z * fSin);
      output.element(2, x * z * fOneMinusCos + y * fSin);
      output.element(3, y * x * fOneMinusCos + z * fSin);
      output.element(4, fCos + pow(y, 2) * fOneMinusCos);
      output.element(5, y * z * fOneMinusCos - x * fSin);
      output.element(6, z * x * fOneMinusCos - y * fSin);
      output.element(7, z * y * fOneMinusCos + x * fSin);
      output.element(8, fCos + pow(z, 2) * fOneMinusCos);
      //
      return output;
   };
   AxisAngle::operator Quaternion() const {
      Quaternion output;
      float fHalfAngle = angle / 2;
      output.w = cos(fHalfAngle);
      output.x = sin(fHalfAngle) * x;
      output.y = sin(fHalfAngle) * y;
      output.z = sin(fHalfAngle) * z;
      return output;
   };
   //
   // EULER
   //
   void Euler::ConvertToDegrees() {
      if (!this->isRadians)
         return;
      this->x /= (PI / 180.0);
      this->y /= (PI / 180.0);
      this->z /= (PI / 180.0);
      this->isRadians = false;
   };
   void Euler::ConvertToRadians() {
      if (this->isRadians)
         return;
      this->x *= PI / 180.0;
      this->y *= PI / 180.0;
      this->z *= PI / 180.0;
      this->isRadians = true;
   };
   Euler::operator AxisAngle() const {
      return (AxisAngle)(Matrix)*this;
   };
   Euler::operator Matrix() const {
      Matrix output;
      //
      float _x = this->x;
      float _y = this->y;
      float _z = this->z;
      if (!this->isRadians) {
         _x *= PI / 180;
         _y *= PI / 180;
         _z *= PI / 180;
      }
      float fSinX = sin(_x);
      float fSinY = sin(_y);
      float fSinZ = sin(_z);
      float fCosX = cos(_x);
      float fCosY = cos(_y);
      float fCosZ = cos(_z);
      //
      // Build the matrix.
      //
      output.data[0][0] = fCosY * fCosZ; // 1,1
      output.data[0][1] = fCosY * fSinZ;
      output.data[0][2] = -fSinY;
      output.data[1][0] = fSinX * fSinY * fCosZ - fCosX * fSinZ; // 2,1
      output.data[1][1] = fSinX * fSinY * fSinZ + fCosX * fCosZ;
      output.data[1][2] = fSinX * fCosY;
      output.data[2][0] = fCosX * fSinY * fCosZ + fSinX * fSinZ; // 3,1
      output.data[2][1] = fCosX * fSinY * fSinZ - fSinX * fCosZ;
      output.data[2][2] = fCosX * fCosY;
      //
      return output;
   };
   Euler::operator Quaternion() const {
      return (Quaternion)(Matrix)*this;
   };
   //
   // MATRIX
   //
   Matrix::operator AxisAngle() const {
      AxisAngle output(0, 0, 0, 0, true);
      //
      // Determine the angle.
      //
      output.angle = acos((Trace() - 1.0) / 2.0);
      //
      // Determine the axis.
      //
      output.x = element(7) - element(5);
      output.y = element(2) - element(6);
      output.z = element(3) - element(1);
      //
      // A 180-degree angle tends to lead to a zero vector as our axis.
      // There seems to be a way to correct that...
      //
      // Source for the math: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm
      // Source for the math: http://sourceforge.net/p/mjbworld/discussion/122133/thread/912b44f7
      //
      //if (output.angle == PI) {
      if (abs(output.angle - PI) < 0.001) {
         output.x = sqrt((element(0) + 1) / 2);
         output.y = sqrt((element(4) + 1) / 2);
         output.z = sqrt((element(8) + 1) / 2);
         //
         // We don't know the signs of the above terms. Per our second source, we can start 
         // to figure that out by finding the largest term, and then...
         //
         float fTemporary = output.x;
         UInt32 iLargestIndex = 0;
         if (fTemporary < output.y) {
            fTemporary = output.y;
            iLargestIndex = 1;
         }
         if (fTemporary < output.z) {
            fTemporary = output.z;
            iLargestIndex = 2;
         }
         //int iSignX = element(iLargestIndex * 3 + 0) < 0 ? -1 : 1;
         //int iSignY = element(iLargestIndex * 3 + 1) < 0 ? -1 : 1;
         //int iSignZ = element(iLargestIndex * 3 + 2) < 0 ? -1 : 1;
         int iSignX = data[iLargestIndex][0] < 0 ? -1 : 1;
         int iSignY = data[iLargestIndex][1] < 0 ? -1 : 1;
         int iSignZ = data[iLargestIndex][2] < 0 ? -1 : 1;
         output.x *= iSignX;
         output.y *= iSignY;
         output.z *= iSignZ;
      }
      //
      // Normalize the axis.
      //
      float fLength = output.Length();
      if (fLength != 0)
         output.Normalize();
      else {
         //
         // Edge-case caused by zero-vector. Dumb fallback to the Z-axis.
         //
         output.x = 0;
         output.y = 0;
         output.z = 1;
      }
      return output;
   };
   Matrix::operator Euler() const {
      //
      // Source for the math: https://web.archive.org/web/20051124013711/http://skal.planet-d.net/demo/matrixfaq.htm#Q37
      //
      // The math there is righthanded, but it's easy to tailor it to lefthanded if 
      // you have a handy-dandy reference like the one at <http://www.vectoralgebra.info/eulermatrix.html>.
      //
      Euler output(0, 0, 0, true);
      //
      // We can easily solve for Y, but we must round it to account for imprecision 
      // that is sometimes introduced when we have converted through other forms, 
      // such as axis-angle. fCYTest exists to help us do that.
      //
      float fY = asin(((float)(SInt32)(-element(2) * 1000000)) / 1000000);
      float fCY = cos(fY);
      float fCYTest = ((float)(SInt32)(fCY * 100)) / 100;
      float fTX, fTY;
      if (fCY && abs(fCY) >= 0.00000011920929 && fCYTest) {
         fTX = element(8) / fCY;
         fTY = element(5) / fCY;
         output.x = atan2(fTY, fTX);
         fTX = element(0) / fCY;
         fTY = element(1) / fCY;
         output.z = atan2(fTY, fTX);
      } else {
         //
         // We can't compute X and Z by using Y, because cos(Y) is zero. We'll have 
         // to compromise: we'll assume that X is zero, and dump everything into Z.
         //
         output.x = 0;
         fTX = element(4); // Setting X to zero simplifies this element to: 0*sinY*sinZ + 1*cosZ
         fTY = element(3); // Setting X to zero simplifies this element to: 0*sinY*cosZ - 1*sinZ
         //
         // Negating the result here appears to be necessary to convert from the source 
         // math's lefthanded system to our righthanded system. I kinda guessed it and 
         // it works perfectly in all my tests, but I didn't derive it through some 
         // genius mathematical proof, so... :\ 
         //
         output.z = -atan2(fTY, fTX); // atan(sinZ/cosZ)
      }
      output.y = fY;
      return output;
   };
   Matrix::operator Quaternion() const {
      //return (Quaternion)(AxisAngle)*this;
      Quaternion q;
      //
      // Shoemake's 1987 algorithm that literally everyone uses. Source has apparently 
      // been lost to time.
      //
      float a = this->Trace();
      if (a > 0) {
         a = sqrt(a + 1.0f);
         q.w = a * 0.5f;
         a = 0.5f / a;
         q.x = (this->data[2][1] - this->data[1][2]) * a;
         q.y = (this->data[0][2] - this->data[2][0]) * a;
         q.z = (this->data[1][0] - this->data[0][1]) * a;
      } else {
         UInt8 i = 0;
         if (this->data[1][1] > this->data[0][0])
            i = 1;
         if (this->data[2][2] > this->data[i][i])
            i = 2;
         UInt8 j = (i + 1) % 3;
         UInt8 k = (j + 1) % 3;
         //
         a = sqrt(this->data[i][i] - this->data[j][j] - this->data[k][k] + 1.0f);
         ((float*) &q.x)[i] = a / 2; // q[n] = value, such that q[0] == x; todo: code a proper [] operator for quaternion
         a = 0.5 / a;
         q.w = (this->data[k][j] - this->data[j][k]) * a;
         ((float*) &q.x)[j] = (this->data[j][i] + this->data[i][j]) * a;
         ((float*) &q.x)[k] = (this->data[k][i] + this->data[i][k]) * a;
      }
      return q;
   };
   //
   // QUATERNION
   //
   Quaternion::operator AxisAngle() const {
      return (AxisAngle)(Matrix)*this;
   };
   Quaternion::operator Euler() const {
      return (Euler)(Matrix)*this;
   };
   Quaternion::operator Matrix() const {
      Matrix output;
      output.data[0][0] = 1 - 2 * pow(y, 2) - 2 * pow(z, 2);
      output.data[0][1] = (2 * x * y) - (2 * z * w);
      output.data[0][2] = (2 * x * z) + (2 * y * w);
      output.data[1][0] = (2 * x * y) + (2 * z * w);
      output.data[1][1] = 1 - 2 * pow(x, 2) - 2 * pow(z, 2);
      output.data[1][2] = (2 * y * z) - (2 * x * w);
      output.data[2][0] = (2 * x * z) - (2 * y * w);
      output.data[2][1] = (2 * y * z) + (2 * x * w);
      output.data[2][2] = 1 - 2 * pow(x, 2) - 2 * pow(y, 2);
      return output;
   };
};