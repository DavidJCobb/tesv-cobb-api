#pragma once

namespace cobb {
   extern bool string_has_content(const char* str); // any characters besides '\0' and std::isspace-positive glyphs
   extern bool string_says_false(const char* str); // the string spells the case-insensitive word "false", ignoring whitespace
   extern bool string_to_int(const char* str, SInt32& out);  // returns true if it's a valid integer and no non-whitespace follows the number; out is not modified otherwise
   extern bool string_to_float(const char* str, float& out); // returns true if it's a valid float   and no non-whitespace follows the number; out is not modified otherwise
   //
   extern void replace_all_of(std::string& subject, const std::string& token, const std::string& target);
   extern std::string& ltrim(std::string& subject);
   extern std::string& rtrim(std::string& subject);
   extern std::string& trim(std::string& subject);
   extern void snprintf(std::string& out, const char* format, ...);
   extern SInt32 stricmp(const std::string& a, const std::string& b);
   extern bool strieq(const std::string& a, const std::string& b);
   extern bool striendswith(const std::string& haystack, const std::string& needle);

   //
   // Given a format like "example %s %s %d", the valid tokens would be "ssd". For 
   // "example %.*s" or "example %*d" the tokens would be "*s". Note that format 
   // tokens with multiple flags (e.g. "%- d") are not supported, unless the last 
   // flag is zero (in which case it's taken as part of the width field e.g. "%-0d").
   //
   extern __declspec(noinline) bool validate_format(const std::string& format, const std::string& tokens);

   /*class istring : public std::string {
      //
      // TODO: finish me; I want this so we can work around issues with case-sensitivity in 
      // our String.cpp Papyrus methods
      //
      private:
         typedef std::string base_container;
      public:
         //
         // todo: constructors here
         //
         //
         static value_type to_lower(const value_type subject) {
            if (subject >= 'a' && subject <= 'z')
               return subject - 0x20;
            return subject;
         };
         //
         size_type find(const value_type* p, size_type offset, size_type count) const {
            _DEBUG_POINTER_IF(count != 0, p);
            if (count == 0 && offset <= this->size())
               return offset;
            size_type nM;
            if (offset > this->size() || count > (nM = this->size() - offset))
               return std::string::npos;
            value_type target = to_lower(*p);
            for (auto it = this->begin() + offset; it != this->end(); ++it) {
               if (target == to_lower(*it))
                  return it - this->begin();
            }
            return std::string::npos;
         };
         size_type rfind(const value_type* p, size_type offset, size_type count) const {
            _DEBUG_POINTER_IF(count != 0, p);
            if (count == 0)
               return (offset < this->size() ? offset : this->size());
            if (count > this->size())
               return std::string::npos;
            value_type target = to_lower(*p);
            static_assert(false, "how do we do this damn loop");
            return std::string::npos;
         };
   };*/
};