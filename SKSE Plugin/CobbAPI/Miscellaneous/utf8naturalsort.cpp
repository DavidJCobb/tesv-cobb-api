#include "utf8naturalsort.h"
#include "utf8string.h"
#include <cwctype>

inline static bool isNumber(const char c) {
   return c >= '0' && c <= '9';
}

namespace cobb {
   namespace utf8 {
      int32_t naturalcompare(const std::string& a, const std::string& b) {
         auto i_a = a.begin();
         auto i_b = b.begin();
         for (; i_a != a.end() && i_b != b.end(); ++i_a, ++i_b) {
            unicodechar c_a = utf8::get(a, i_a);
            unicodechar c_b = utf8::get(b, i_b);
            if (!isNumber(c_a) || !isNumber(c_b)) {
               //
               // For non-numeric characters, just do a character code comparison. 
               // This won't internationalize -- it won't even handle accented 
               // characters in English.
               //
               c_a = std::towlower(c_a); // not sure about this; seems to use two-byte chars when unicode can be up to four bytes, no?
               c_b = std::towlower(c_b); // then again, anything past the two-byte mark probably isn't even a "letter" as Westerners conceive of them...
               if (c_a == c_b)
                  continue;
               return c_b - c_a;
            }
            UInt32 n_a = c_a - '0';
            UInt32 n_b = c_b - '0';
            ++i_a;
            ++i_b;
            for (; i_a != a.end(); ++i_a) {
               c_a = utf8::get(a, i_a);
               if (!isNumber(c_a))
                  break;
               n_a = (n_a * 10) + (c_a - '0');
            }
            for (; i_b != b.end(); ++i_b) {
               c_b = utf8::get(b, i_b);
               if (!isNumber(c_b))
                  break;
               n_b = (n_b * 10) + (c_b - '0');
            }
            if (n_a != n_b)
               return n_b - n_a;
         }
         //
         // If we've reached this point, then we've reached the end of ONE OF the 
         // strings (and they don't end in digits).
         //
         if (i_a != a.end())
            return 1;
         if (i_b != b.end())
            return -1;
         return 0;
      }
   }
}