#include "utf8string.h"

namespace cobb {
   namespace utf8 {
      inline static UInt8 _get_bytecount(char c) {
         if ((c & 0b11011111) == c)
            return 2;
         if ((c & 0b11101111) == c)
            return 3;
         if ((c & 0b11110111) == c)
            return 4;
         // if ((c & 0b10111111) == c) then it's a continuation byte; can't know what of
         // HTML5 spec says that on dispay, any invalid bytes should be treated as Windows-1252
         // so that's how we oughta handle an out-of-place continuation byte
         return 1;
      }
      inline static bool _is_bytecount(char c) {
         if ((c & 0b11011111) == c)
            return true;
         if ((c & 0b11101111) == c)
            return true;
         return ((c & 0b11110111) == c);
      }
      inline static bool _is_continuation(char c) {
         return ((c & 0b10111111) == c);
      }
      inline static UInt32 _win1252_to_unicode(char c) {
         if (c < 130 || c > 173) // Everything outside of this range maps 1:1 between Windows-1252 and Unicode.
            return c;
         switch (c) {
            case 0x80:
               return 0x20AC; // Euro Sign
            case 0x82:
               return 0x201A; // Single Low-9 Quotation Mark
            case 0x83:
               return 0x0192; // Latin Smaller Letter F with Hook
            case 0x84:
               return 0x201E; // Double Low-9 Quotation Mark
            case 0x85:
               return 0x2026; // Horizontal Ellipsis
            case 0x86:
               return 0x2020; // Dagger
            case 0x87:
               return 0x2021; // Double Dagger
            case 0x88:
               return 0x02C6; // Modifier Letter Circumflex Accent
            case 0x89:
               return 0x2030; // Per Mille Sign
            case 0x8A:
               return 0x0160; // Latin Capital LEtter S with Caron
            case 0x8B:
               return 0x2039; // Single Left-Pointing Angle Quotation Mark
            case 0x8C:
               return 0x0152; // Latin Capital Ligature Oe
            case 0x8E:
               return 0x017D; // Latin Capital Letter Z with Caron
            case 0x91:
               return 0x2018; // Left Single Quotation Mark
            case 0x92:
               return 0x2019; // Right Single Quotation Mark
            case 0x93:
               return 0x201C; // Left Double Quotation Mark
            case 0x94:
               return 0x201D; // Right Double Quotation Mark
            case 0x95:
               return 0x2022; // Bullet
            case 0x96:
               return 0x2013; // En Dash
            case 0x97:
               return 0x2014; // Em Dash
            case 0x98:
               return 0x02DC; // Small Tilde
            case 0x99:
               return 0x2122; // Trademark Sign
            case 0x9A:
               return 0x0161; // Latin Smaller Letter S with Caron
            case 0x9B:
               return 0x203A; // Single Right-Pointing Angle Quotation Mark
            case 0x9C:
               return 0x0153; // Latin Small Ligature Oe
            case 0x9E:
               return 0x017E; // Latin Smaller Letter Z with Caron
            case 0x9F:
               return 0x0178; // Latin Capital Letter Y with Diaresis
         }
         return c;
      }
      //
      void advance(const std::string& container, std::string::iterator& iterator) {
         auto end = container.end();
         if (iterator == end)
            return;
         unsigned char c;
         //
         c = iterator[0];
         ++iterator;
         auto working = iterator;
         auto count = _get_bytecount(c);
         bool good = true;
         while (good && --count) {
            c = iterator[0];
            ++working;
            if (!_is_continuation(c) || working == end) // error
               good = false;
         }
         if (good)
            iterator = working;
         //
         // If we encounter a prefix byte that doesn't have enough continuation bytes, 
         // then we assume that the prefix and any found continuation bytes are just 
         // Windows-1252 characters, and we move the iterator to just after the prefix 
         // byte.
         //
         // If the byte we're currently on *is* a continuation byte, then we assume 
         // that it's an orphan and treat it as a single-byte Windows-1252 glyph.
         //
      }
      void advance(const std::string& container, std::string::const_iterator& iterator) {
         auto end = container.end();
         if (iterator == end)
            return;
         unsigned char c;
         //
         c = iterator[0];
         ++iterator;
         auto working = iterator;
         auto count = _get_bytecount(c);
         bool good = true;
         while (good && --count) {
            c = iterator[0];
            ++working;
            if (!_is_continuation(c) || working == end) // error
               good = false;
         }
         if (good)
            iterator = working;
         //
         // If we encounter a prefix byte that doesn't have enough continuation bytes, 
         // then we assume that the prefix and any found continuation bytes are just 
         // Windows-1252 characters, and we move the iterator to just after the prefix 
         // byte.
         //
         // If the byte we're currently on *is* a continuation byte, then we assume 
         // that it's an orphan and treat it as a single-byte Windows-1252 glyph.
         //
      }
      void append(std::string& target, const unicodechar c) {
         if (c < 0x80) {
            target.push_back(c);
            return;
         }
         if (c < 0x800) {
            target.push_back(0xE0 | ((c >> 6) & 0x1F));
            target.push_back(0x80 | (c & 0x3F));
            return;
         }
         if (c < 0x10000) {
            target.push_back(0xE0 | ((c >> 12) & 0xF));
            target.push_back(0x80 | ((c >> 6) & 0x3F));
            target.push_back(0x80 | (c & 0x3F));
            return;
         }
         target.push_back(0xE0 | ((c >> 18) & 0x7));
         target.push_back(0x80 | ((c >> 12) & 0x3F));
         target.push_back(0x80 | ((c >> 6) & 0x3F));
         target.push_back(0x80 | (c & 0x1F));
      }
      size_t count(std::string& container) {
         return count_from(container, container.begin());
      }
      size_t count_from(const std::string& container, const std::string::const_iterator& iterator) {
         std::string::const_iterator working = iterator;
         size_t count = 0;
         while (working != container.end()) {
            advance(container, working);
            count++;
         }
         return count;
      }
      size_t count_from(const std::string& container, const std::string::iterator& iterator) {
         std::string::iterator working = iterator;
         size_t count = 0;
         while (working != container.end()) {
            advance(container, working);
            count++;
         }
         return count;
      }
      unicodechar get(const std::string& container, const std::string::const_iterator& iterator) {
         auto end = container.end();
         if (iterator == end)
            return 0;
         unsigned char a = iterator[0];
         if (a < 0x80)
            return a;
         if (_is_continuation(a))
            return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
         UInt32 result;
         char b;
         char c;
         char d;
         auto count = _get_bytecount(a);
         if (iterator + count >= end)
            return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
         switch (count) {
            case 1:
               return a;
            case 2:
               b = iterator[1];
               if (!_is_continuation(b))
                  return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
               result = (a & 0x1F) << 6;
               result |= (b & 0x3F);
               return result;
            case 3:
               b = iterator[1];
               c = iterator[2];
               if (!_is_continuation(b) || !_is_continuation(c))
                  return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
               result = (a & 0xF) << 12;
               result |= (b & 0x3F) << 6;
               result |= (c & 0x3F);
               if (result >= 0xD800 && result <= 0xDFFF)
                  return invalid_glyph;
               return result;
            case 4:
               b = iterator[1];
               c = iterator[2];
               d = iterator[3];
               if (!_is_continuation(b) || !_is_continuation(c) || !_is_continuation(d))
                  return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
               result = (a & 7) << 18;
               result |= (b & 0x3F) << 12;
               result |= (c & 0x3F) << 6;
               result |= (d & 0x3F);
               if (result > 0x10FFFF)
                  return invalid_glyph;
               return result;
         }
         return invalid_glyph;
      }
      unicodechar get(const std::string& container, const std::string::iterator& iterator) {
         auto end = container.end();
         if (iterator == end)
            return 0;
         unsigned char a = iterator[0];
         if (a < 0x80)
            return a;
         if (_is_continuation(a))
            return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
         UInt32 result;
         char b;
         char c;
         char d;
         auto count = _get_bytecount(a);
         if (iterator + count >= end)
            return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
         switch (count) {
            case 1:
               return a;
            case 2:
               b = iterator[1];
               if (!_is_continuation(b))
                  return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
               result = (a & 0x1F) << 6;
               result |= (b & 0x3F);
               return result;
            case 3:
               b = iterator[1];
               c = iterator[2];
               if (!_is_continuation(b) || !_is_continuation(c))
                  return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
               result = (a & 0xF) << 12;
               result |= (b & 0x3F) << 6;
               result |= (c & 0x3F);
               if (result >= 0xD800 && result <= 0xDFFF)
                  return invalid_glyph;
               return result;
            case 4:
               b = iterator[1];
               c = iterator[2];
               d = iterator[3];
               if (!_is_continuation(b) || !_is_continuation(c) || !_is_continuation(d))
                  return _win1252_to_unicode(a); // HTML5 error handling: Treat (a) as a Windows-1252-encoded character and return its Unicode representation.
               result = (a & 7) << 18;
               result |= (b & 0x3F) << 12;
               result |= (c & 0x3F) << 6;
               result |= (d & 0x3F);
               if (result > 0x10FFFF)
                  return invalid_glyph;
               return result;
         }
         return invalid_glyph;
      }
      rawchar get_raw(const std::string& container, const std::string::const_iterator& iterator) {
         if (iterator == container.end())
            return 0;
         char a = iterator[0];
         char b = iterator[1];
         char c = iterator[2];
         char d = iterator[3];
         if (!_is_bytecount(a))
            return invalid_glyph;
         {  // 2 bytes
            if (!_is_continuation(b))
               return invalid_glyph;
            if ((a & 0xE0) == 0xC0)
               return (a << 0x08) | b;
         }
         {  // 3 bytes
            if (!_is_continuation(c))
               return invalid_glyph;
            if ((a & 0xF0) == 0xE0)
               return (a << 0x10) | (b << 0x08) | c;
         }
         {  // 4 bytes
            if (!_is_continuation(d))
               return invalid_glyph;
            return (a << 0x18) | (b << 0x10) | (c << 0x08) | d;
         }
      }
      rawchar get_raw(const std::string& container, const std::string::iterator& iterator) {
         if (iterator == container.end())
            return 0;
         char a = iterator[0];
         char b = iterator[1];
         char c = iterator[2];
         char d = iterator[3];
         if (!_is_bytecount(a))
            return invalid_glyph;
         {  // 2 bytes
            if (!_is_continuation(b))
               return invalid_glyph;
            if ((a & 0xE0) == 0xC0)
               return (a << 0x08) | b;
         }
         {  // 3 bytes
            if (!_is_continuation(c))
               return invalid_glyph;
            if ((a & 0xF0) == 0xE0)
               return (a << 0x10) | (b << 0x08) | c;
         }
         {  // 4 bytes
            if (!_is_continuation(d))
               return invalid_glyph;
            return (a << 0x18) | (b << 0x10) | (c << 0x08) | d;
         }
      }
      bool is_end(const std::string& container, const std::string::iterator& iterator) {
         auto end = container.end();
         if (iterator == end)
            return true;
         auto working = iterator;
         char c;
         //
         c = working[0];
         ++working;
         auto count = _get_bytecount(c);
         bool good = true;
         while (good && --count) {
            c = working[0];
            ++working;
            if (!_is_continuation(c) || working == end) // error
               return false;
         }
         return working == end;
      }
      bool is_end(const std::string& container, const std::string::const_iterator& iterator) {
         auto end = container.end();
         if (iterator == end)
            return true;
         auto working = iterator;
         char c;
         //
         c = working[0];
         ++working;
         auto count = _get_bytecount(c);
         bool good = true;
         while (good && --count) {
            c = working[0];
            ++working;
            if (!_is_continuation(c) || working == end) // error
               return false;
         }
         return working == end;
      }
   }
};