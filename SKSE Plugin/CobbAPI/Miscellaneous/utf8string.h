#pragma once
#include "strings.h"
#include <string>

namespace cobb {
   typedef uint32_t unicodechar;
   namespace utf8 {
      typedef uint32_t rawchar;
      constexpr unicodechar invalid_glyph = 0xFFFD;

      void        advance(const std::string&, std::string::iterator&);
      void        advance(const std::string&, std::string::const_iterator&);
      void        append(std::string&, const unicodechar);
      size_t      count(std::string&);
      size_t      count_from(const std::string&, const std::string::iterator&);
      size_t      count_from(const std::string&, const std::string::const_iterator&);
      unicodechar get(const std::string&, const std::string::iterator&);
      unicodechar get(const std::string&, const std::string::const_iterator&);
      rawchar     get_raw(const std::string&, const std::string::iterator&);       // blindly reads a char with almost no validation
      rawchar     get_raw(const std::string&, const std::string::const_iterator&); // blindly reads a char with almost no validation
   }
};