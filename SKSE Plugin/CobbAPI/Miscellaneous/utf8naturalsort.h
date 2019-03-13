#pragma once
#include "strings.h"

namespace cobb {
   //
   template<typename String> SInt32 utf8_naturalcompare(const String& a, const String& b) {
      static_assert(is_basic_string<String>::value, "utf8_naturalcompare must receive a basic_string specialization.");
      //
      // NOTE: This function doesn't handle case-insensitivity. At present, that is the 
      // responsibility of whatever String class you use. We don't (and can't) rely on 
      // the char_traits of a basic_string when doing UTF-8, so your class has to alter 
      // the data as its stored.
      //
      struct _ {
         inline static bool isNumber(const char c) {
            return c >= '0' && c <= '9';
         };
      };
      //
      auto i_a = utf8iterator<String>(a);
      auto i_b = utf8iterator<String>(b);
      for (; !i_a.is_end() && !i_b.is_end(); ++i_a, ++i_b) {
         UInt32 c_a = i_a.as_unicode();
         UInt32 c_b = i_b.as_unicode();
         if (!_::isNumber(c_a) || !_::isNumber(c_b)) {
            if (c_a == c_b)
               continue;
            return c_b - c_a;
         }
         UInt32 n_a = c_a - '0';
         UInt32 n_b = c_b - '0';
         ++i_a;
         ++i_b;
         for (; !i_a.is_end(); ++i_a) {
            c_a = i_a.as_unicode();
            if (!_::isNumber(c_a))
               break;
            n_a = (n_a * 10) + (c_a - '0');
         }
         for (; !i_b.is_end(); ++i_b) {
            c_b = i_b.as_unicode();
            if (!_::isNumber(c_b))
               break;
            n_b = (n_b * 10) + (c_b - '0');
         }
         if (n_a != n_b)
            return n_b - n_a;
      }
      if (!i_a.is_end())
         return 1;
      if (!i_b.is_end())
         return -1;
      return 0;
   };
};