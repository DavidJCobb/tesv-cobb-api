#include "utf8string.h"

namespace cobb {
   void utf8append(std::string& target, UInt32 c) {
      if (c < 0x80) {
         target.push_back(c);
         return;
      }
      if (c < 0x800) {
         target.push_back(0xE0 | ((c >> 0x6) & 0x3F));
         target.push_back(0x80 | (c & 0x1F));
         return;
      }
      if (c < 0x10000) {
         target.push_back(0xE0 | ((c >> 0x12) & 0xF));
         target.push_back(0x80 | ((c >> 0x6) & 0x3F));
         target.push_back(0x80 | (c & 0x1F));
         return;
      }
      target.push_back(0xE0 | ((c >> 0x18) & 0x7));
      target.push_back(0x80 | ((c >> 0x12) & 0x3F));
      target.push_back(0x80 | ((c >> 0x6) & 0x3F));
      target.push_back(0x80 | (c & 0x1F));
   }
   size_t utf8count(std::string& target) {
      utf8iterator<std::string> it(target);
      size_t count = 0;
      while (!it.is_end()) {
         ++count;
         ++it;
      }
      return count;
   }
   //
   _declspec(noinline) UInt32 _utf8iterator_internals::as_unicode() const {
      if (!*p)
         return 0;
      unsigned char a = p[0];
      if (a < 0x80)
         return a;
      if (!_is_bytecount(a))
         return invalid_utf8char;
      //
      // Two bytes:
      //
      UInt32 result;
      char b = p[1];
      if (!_is_continuation(b))
         return invalid_utf8char;
      if ((a & 0xE0) == 0xC0) {
         result = (a & 0x1F) << 6;
         result |= (b & 0x3F);
         return result;
      }
      //
      // Three bytes:
      //
      char c = p[2];
      if (!_is_continuation(c))
         return invalid_utf8char;
      if ((a & 0xF0) == 0xE0) { // three bytes
         result = (a & 0xF) << 12;
         result |= b << 6;
         result |= (c & 0x3F);
         if (result >= 0xD800 && result <= 0xDFFF)
            return invalid_utf8char;
         return result;
      }
      //
      // Four bytes:
      //
      char d = p[3];
      if (!_is_continuation(d))
         return invalid_utf8char;
      result = (a & 7) << 18;
      result |= b << 12;
      result |= c << 6;
      result |= (d & 0x3F);
      if (result > 0x10FFFF)
         return invalid_utf8char;
      return result;
   };
   _declspec(noinline) UInt32 _utf8iterator_internals::as_utf8() const {
      if (!*p)
         return 0;
      char a = p[0];
      char b = p[1];
      char c = p[2];
      char d = p[3];
      if (!_is_bytecount(a))
         return invalid_utf8char;
      {  // 2 bytes
         if (!_is_continuation(b))
            return invalid_utf8char;
         if ((a & 0xE0) == 0xC0)
            return (a << 0x08) | b;
      }
      {  // 3 bytes
         if (!_is_continuation(c))
            return invalid_utf8char;
         if ((a & 0xF0) == 0xE0)
            return (a << 0x10) | (b << 0x08) | c;
      }
      {  // 4 bytes
         if (!_is_continuation(d))
            return invalid_utf8char;
         return (a << 0x18) | (b << 0x10) | (c << 0x08) | d;
      }
   }
};