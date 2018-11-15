#pragma once
#include "strings.h"
#include <string> using namespace std;

namespace cobb {
   typedef   UInt32 utf8char; // todo: rename to "unicodechar"? or "utf8unpackedchar"?
   constexpr utf8char invalid_utf8char = 0xFFFD;

   void   utf8append(std::string& target, UInt32 ch);
   size_t utf8count(std::string&);

   class _utf8iterator_internals {
      protected:
         const unsigned char* p;
      protected:
         inline static bool _is_bytecount(char c) {
            return c < 0xF8;
         }
         inline static bool _is_continuation(char c) {
            return ((c & 0xC0) == 0x80);
         }
      public:
         _utf8iterator_internals(const unsigned char* p) : p(p) {};
         //
         _declspec(noinline) UInt32 as_unicode() const;
         _declspec(noinline) UInt32 as_utf8() const;
         bool is_end() const = delete;
         //
         _utf8iterator_internals& operator++() {
            if (!p)
               return *this;
            if (*p) {
               p++;
               if ((*p & 0xC0) == 0x80) p++;
               if ((*p & 0xC0) == 0x80) p++;
               if ((*p & 0xC0) == 0x80) p++;
            }
            return *this;
         };
   };

   template<typename T> class utf8iterator : public _utf8iterator_internals { // becomes invalid if the std::string it's initialized with has its iterators invalidated
      //
      // This is essentially an iterator that exists outside of the string it 
      // iterates on, and maintains a reference to that string. The reference 
      // is used to check whether the iterator has traversed past the end of 
      // the string. This is necessary because UTF-8 characters are variable-
      // width and do not correspond to the width of items in the string -- 
      // that is, this iterator can't ever be smoothly compared to string::end.
      //
      // This iterator is read-only.
      //
      private:
         static_assert(is_basic_string<T>::value, "utf8iterator must be created for a basic_string specialization that uses chars as its char type.");
      protected:
         const T& pos;
         //
         void _validate() {
            if (this->pos.data() + this->pos.size() <= (const char*)this->p)
               this->p = nullptr;
         };
      public:
         utf8iterator(const T& s) : _utf8iterator_internals(reinterpret_cast<const unsigned char*>(s.data())), pos(s) {
            if (!s.size())
               this->p = nullptr;
         };
         utf8iterator(const utf8iterator& s) : _utf8iterator_internals(s.p), pos(s.pos) {};
         utf8iterator(const char* s) = delete;
         //
         UInt32 as_utf8() const {
            auto result = _utf8iterator_internals::as_utf8();
            if (!result && !this->is_end())
               return 0xC080;
            return result;
         };
         bool is_end() const {
            return this->p == nullptr;
         };
         utf8iterator<T>& operator++() {
            _utf8iterator_internals::operator++();
            this->_validate();
            return *this;
         };
         inline bool operator==(const utf8iterator& o) const { return this->p == o.p && this->pos == o.pos; };
         inline bool operator==(const char* o) const { return this->p == o; };
   };
};