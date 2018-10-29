#pragma once
#include <string> using namespace std;

namespace cobb {
   typedef   UInt32 utf8char; // todo: rename to "unicodechar"? or "utf8unpackedchar"?
   constexpr utf8char invalid_utf8char = 0xFFFD;

   class utf8charpos { // becomes invalid if the std::string it's initialized with has its iterators invalidated
      //
      // This is essentially an iterator that exists outside of the string it 
      // iterates on, and maintains a reference to that string. The reference 
      // is used to check whether the iterator has traversed past the end of 
      // the string. This is necessary because UTF-8 characters are variable-
      // width and do not correspond to the width of items in the string -- 
      // that is, this iterator can't ever be smoothly compared to string::end.
      //
      private:
         const std::string& pos; // string we're iterating over
         const char* p; // pointer to a char within pos
         //
         inline UInt8 intended_bytecount() const {
            char c = *this->p;
            if (!(c & 0x80))
               return 1;
            if ((c & 0xE0) == 0xC0)
               return 2;
            if ((c & 0xF0) == 0xE0)
               return 3;
            return 4;
         };
         void _validate() {
            if (this->p > this->pos.data() + this->pos.size())
               this->p = nullptr;
         };
      public:
         utf8charpos(const std::string& s) : pos(s) {
            this->p = s.data();
         };
         //
         utf8charpos& operator++();
         inline bool operator==(const utf8charpos& o) const { return this->p == o.p && this->pos == o.pos; };
         inline bool operator==(const char* o) const { return this->p == o; };
         utf8char operator*() const;
         //
         void get_raw(UInt32& outBytes, UInt8& outSize) const;
         //
         inline bool is_end() const { return this->p == nullptr; };
   };
};