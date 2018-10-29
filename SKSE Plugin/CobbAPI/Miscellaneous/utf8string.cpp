#include "utf8string.h"

namespace cobb {
   bool _char_is_invalid(char c) {
      if (c == 0xC0 || c == 0xC1)
         return true;
      return c >= 0xF5;
   };

   utf8charpos& utf8charpos::operator++() {
      auto code = **this;
      if (code == invalid_utf8char) {
         if (*this->p == 0xEF && *(this->p + 1) == 0xBF && *(this->p + 2) == 0xBD) // it actually is 0xFFFD
            this->p += 3;
         else
            this->p += 1;
      } else
         this->p += this->intended_bytecount();
      //
      this->_validate();
      return *this;
   };
   utf8char utf8charpos::operator*() const {
      char c = *(this->p);
      if (!(c & 0x80))
         return c;
      if (_char_is_invalid(c))
         return invalid_utf8char;
      utf8char result = 0;
      if ((c & 0xE0) == 0xC0) { // two bytes
         char d = *(this->p + 1);
         if (_char_is_invalid(d))
            return invalid_utf8char;
         result = (c & 0x1F) << 6;
         result |= (d & 0x00FFFFFF);
         if (result < 0x100) // invalid code sequence
            return invalid_utf8char;
      } else if ((c & 0xF0) == 0xE0) { // three bytes
         char d = *(this->p + 1);
         char e = *(this->p + 2);
         if (_char_is_invalid(d) || _char_is_invalid(e))
            return invalid_utf8char;
         result = (c & 0xF) << 12;
         result |= (d & 0x00FFFFFF) << 6;
         result |= (e & 0x00FFFFFF);
         if (result < 0x10000) // invalid code sequence
            return invalid_utf8char;
      } else if ((c & 0xF8) == 0xF0) { // four bytes
         char d = *(this->p + 1);
         char e = *(this->p + 2);
         char f = *(this->p + 3);
         if (_char_is_invalid(d) || _char_is_invalid(e) || _char_is_invalid(f))
            return invalid_utf8char;
         result = (c & 7) << 18;
         result |= (d & 0x00FFFFFF) << 12;
         result |= (e & 0x00FFFFFF) << 6;
         result |= (f & 0x00FFFFFF);
         if (result < 0x1000000) // invalid code sequence
            return invalid_utf8char;
      }
      if ((result > 0x10FFFF) || (result >= 0xD800 && result <= 0xDFFF))
         return invalid_utf8char;
      return result;
   };
   void utf8charpos::get_raw(UInt32& outBytes, UInt8& outSize) const {
      outSize = 1;
      char c = *(this->p);
      if (!(c & 0x80) || _char_is_invalid(c)) {
         outBytes = c;
         return;
      }
      char d = *(this->p + 1);
      if (_char_is_invalid(d)) {
         outBytes = c;
         return;
      }
      if ((c & 0xE0) == 0xC0) { // two bytes
         outSize = 2;
         outBytes = *(UInt16*)(this->p);
         return;
      }
      char e = *(this->p + 2);
      if (_char_is_invalid(e)) {
         outBytes = c;
         return;
      }
      if ((c & 0xF0) == 0xE0) { // three bytes
         outSize = 3;
         outBytes = *(UInt16*)(this->p) << 8 | e;
         return;
      }
      char f = *(this->p + 3);
      if (_char_is_invalid(f)) {
         outBytes = c;
         return;
      }
      outSize = 4;
      outBytes = *(UInt32*)(this->p);
   };
};