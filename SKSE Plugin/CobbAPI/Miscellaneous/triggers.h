#pragma once

namespace cobb {
   namespace triggers {
      //
      // experimental stuff for writing a trigger system
      //

      enum QualifierBitmask : UInt8 {
         kEqual   = 1,
         kGreater = 2,
         kLess    = 4,
      };
      //
      // I wonder which of these two would compile better...
      //
      template<typename T> bool compare_qualified_number(T basis, UInt8 op, T other) {
         if (!op)
            return basis != other;
         if ((op & kEqual) && basis == other)
            return true;
         if ((op & kGreater) && basis > other)
            return true;
         if ((op & kLess) && basis < other)
            return true;
         return false;
      };
      template<typename T> bool compare_qualified_number_sw(T basis, UInt8 op, T other) {
         switch (op) {
            case 0:
               return basis != other;
            case 1:
               return basis == other;
            case 2:
               return basis > other;
            case 3:
               return basis >= other;
            case 4:
               return basis < other;
            case 5:
               return basis <= other;
            case 6:
               return basis != other;
            case 7:
               return true;
         }
         return false;
      };

      class QualifiedInt { // ranges from -0x0FFFFFFF to 0x0FFFFFFF; suited for compactness, but maybe not as efficient at run-time
         private:
            UInt32 raw = 0;
         public:
            QualifiedInt() {};
            QualifiedInt(SInt32 v, UInt8 qualifier) {
               this->raw = (qualifier & 7) << 0x1E;
               this->raw |= (v & ~0xF0000000);
               if (v < 0)
                  this->raw |= 0x10000000;
            };
            //
            UInt8 qualifier() const noexcept {
               return (this->raw & 0xE0000000) >> 0x1D;
            };
            SInt32 value() const noexcept {
               SInt32 value = this->raw & ~0xF0000000;
               if (this->raw & 0x10000000)
                  value = -value;
               return value;
            };
            //
            bool compare(SInt32 num) const noexcept {
               UInt8  bits  = this->qualifier();
               SInt32 value = this->value();
               if (!bits)
                  return num != value;
               if ((bits & kEqual) && value == num)
                  return true;
               if ((bits & kGreater) && value > num)
                  return true;
               if ((bits & kLess) && value < num)
                  return true;
               return false;
            };
            bool compare(const QualifiedInt& other) const noexcept {
               UInt8  bits  = this->qualifier();
               SInt32 value = this->value();
               SInt32 num   = other.value();
               if (!bits)
                  return num != value;
               if ((bits & kEqual) && value == num)
                  return true;
               if ((bits & kGreater) && value > num)
                  return true;
               if ((bits & kLess) && value < num)
                  return true;
               return false;
            };
      };
   };
};