﻿#include "Papyrus/String.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#define COBB_PAPYRUS_OFFER_REGEX_STRING_APIS 1
#ifdef COBB_PAPYRUS_OFFER_REGEX_STRING_APIS
   #include <regex> // adds, like 80KB to the DLL :(
#endif

#include <algorithm>
#include <string>
#include "skse/GameForms.h"
#include "Miscellaneous/strings.h"
#include "Miscellaneous/utf8string.h"
#include "Miscellaneous/utf8naturalsort.h"

/********************************************************************************************\

   SOME EXPLANATION:

   Skyrim's script engine use UTF-8 strings. If you open a Japanese ESP file in an Eng-
   lish Creation Kit, you'll probably only see question-mark characters... but if you 
   use xEdit to examine any Japanese string's binary data, you'll find that that data 
   only produces Japanese glyphs when interpreted as UTF-8.

   That begs a question: how do you handle UTF-8 strings? Can you use null-terminated 
   strings to store UTF-8? Why don't Google search results seem to agree on this topic? 
   Why are so many authoritative reference documents so seemingly inconsistent?

   Well, you absolutely can use null-terminated strings to hold UTF-8. The people who 
   say you can't are all pedantic dorks. But then, why would they say that? Why would  
   I call them that? Allow me to explain.

   The following string is valid UTF-8 and valid ASCII:

   "abc\0def\0ghi"

   That value cannot be treated as a null-terminated string, because it contains nulls 
   in the middle of the text. That's it. That's literally it. The only UTF-8 byte seq-
   uence that produces null bytes is THE null byte. Non-ASCII symbols in UTF-8 are 
   represented with multiple bytes, and each of those bytes have their high bits set. 
   The pedants are recommending against null-terminated strings because strings with 
   nulls are technically -- and that really is the operative word -- valid.
   
   So!

   WHAT YOU ASKED:
   "Can you use null-terminated strings to hold UTF-8?"

   WHAT YOU MEANT:
   "Is it practical to use null-terminated strings to hold UTF-8?"

   WHAT PEDANTIC DORKS ASSUME YOU MEANT:
   "Can you use null-terminated strings to hold UTF-8 text even if it contains nulls?"

   THE ANSWER YOU ACTUALLY NEED:
   "Null-terminated strings are fine for any sane data."

   THE TRUE ANSWER:
   "Null-terminated strings can safely hold any UTF-8 data that is known to be free of 
   early '\0' glyphs. If you're receiving data from a system that has already either 
   filtered out null bytes or simply truncated at them (e.g. the Papyrus Compiler), 
   then you are not responsible for handling null bytes and can use a null-terminated 
   string, as long as you handle the case of multi-byte character sequences being cut 
   off in the middle. Otherwise, you should probably use STL strings (std::string) as 
   your container, as those store a length instead of relying on a terminator."

\********************************************************************************************/
namespace CobbPapyrus {
   namespace String {
      namespace Numbers {
         UInt32 BinaryToInt32(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString str) {
            const char* s = str.data;
            char* discard;
            return strtoul(s, &discard, 2);
         }
         UInt32 HexToInt32(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString str) {
            const char* s = str.data;
            char* discard;
            return strtoul(s, &discard, 16);
         }
         BSFixedString ToBinary(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value, SInt32 length) {
            if (length < 1) {
               registry->LogWarning("Cannot format a binary value to a negative number of bits. Defaulting to 32.", stackId);
               length = 32;
            } else if (length > 32) {
               registry->LogWarning("Cannot format a binary value longer than 32 bits.", stackId);
               length = 32;
            }
            char bin[33];
            memset(bin, '0', sizeof(bin));
            bin[length] = '\0';
            for (UInt8 i = 0; i < length; i++)
               if (value & (1 << i))
                  bin[length - i - 1] = '1';
            return bin; // passes through BSFixedString constructor, which I believe caches the string, so returning local vars should be fine
         }
         BSFixedString ToHex(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value, SInt32 length) {
            if (length < 1) {
               registry->LogWarning("Cannot format a hexadecimal valueinteger to a negative number of digits. Defaulting to eight.", stackId);
               length = 8;
            } else if (length > 8) {
               registry->LogWarning("Cannot format a hexadecimal integer longer than eight digits.", stackId);
               length = 8;
            }
            char hex[9];
            memset(hex, '0', sizeof(hex));
            hex[length] = '\0';
            while (value > 0 && length--) {
               UInt8 digit = value % 0x10;
               value /= 0x10;
               if (digit < 0xA) {
                  hex[length] = digit + '0';
               } else {
                  hex[length] = digit + 0x37;
               }
            }
            return hex; // passes through BSFixedString constructor, which I believe caches the string, so returning local vars should be fine
         }
      }
      namespace Regex {
         //
         // Regular expressions are case-insensitive, but Papyrus stores all strings 
         // case-insensitively (per the "C" locale). To work around this, we have the 
         // scripter supply a "case sentinel" string along with their regex, wherein 
         // glyphs that would be uppercase in the regex are '1' in the sentinel, and 
         // glyphs that would be lowercase in the regex are '0' in the sentinel. Some 
         // examples:
         //
         // REGEX            | SENTINEL
         // th\w             | 00-0
         // \w\w\W\w\W       | -0-0-1-0-1
         // Jarl Elisif      | 1000-100000
         //
         // The _fix_regex function applies the sentinel to a regex.
         //
         void _fix_regex(std::string& out, const std::string& regex, const std::string& sentinel) {
            out.clear();
            out.reserve(regex.size());
            auto a = regex.begin();
            auto b = sentinel.begin();
            for (; a != regex.end() && b != sentinel.end(); cobb::utf8::advance(regex, a), cobb::utf8::advance(sentinel, b)) {
               auto x = cobb::utf8::get(regex, a);
               auto y = cobb::utf8::get(sentinel, b);
               if (y == '0')
                  x = towlower(x);
               else if (y == '1')
                  x = towupper(x);
               cobb::utf8::append(out, x);
            }
         };
         //
         bool Search(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString regex, BSFixedString caseSentinel) {
            if (!haystack.data || !regex.data)
               return false;
            ERROR_AND_RETURN_0_IF(!caseSentinel.data, "You must specify a non-empty case sentinel.", registry, stackId);
            std::string rxFixed  = regex.data;
            std::string sentinel = caseSentinel.data;
            ERROR_AND_RETURN_0_IF(cobb::utf8::count(rxFixed) != cobb::utf8::count(sentinel), "The case sentinel must have the same number of code points as the regex.", registry, stackId);
            _fix_regex(rxFixed, regex.data, sentinel);
            std::regex ex(rxFixed.c_str(), std::regex::ECMAScript | std::regex::icase);
            return std::regex_search(haystack.data, ex);
         };
         VMResultArray<BSFixedString> Match(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString regex, BSFixedString caseSentinel, bool wholeMustMatch) {
            VMResultArray<BSFixedString> result;
            if (!haystack.data || !regex.data)
               return result;
            if (!caseSentinel.data) {
               registry->LogError("You must specify a non-empty case sentinel.", stackId);
               return result;
            }
            std::string rxFixed = regex.data;
            std::string sentinel = caseSentinel.data;
            if (cobb::utf8::count(rxFixed) != cobb::utf8::count(sentinel)) {
               registry->LogError("The case sentinel must have the same number of code points as the regex.", stackId);
               return result;
            }
            _fix_regex(rxFixed, regex.data, sentinel);
            //
            std::regex  ex(rxFixed.c_str(), std::regex::ECMAScript | std::regex::icase);
            std::cmatch matches;
            if (wholeMustMatch) {
               std::regex_match(haystack.data, matches, ex);
               auto size = matches.size();
               result.reserve(size);
               for (size_t i = 0; i < size; i++)
                  result.push_back(matches[i].str().c_str());
            }  else {
               std::string hay(haystack.data);
               std::sregex_iterator iter(hay.begin(), hay.end(), ex);
               std::sregex_iterator end;
               //
               while (iter != end) {
                  auto& results = *iter;
                  for (size_t i = 0; i < results.size(); ++i) {
                     auto submatch = results[i];
                     result.push_back(submatch.str().c_str());
                  }
                  ++iter;
               }
            }
            return result;
         };
         BSFixedString Replace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString regex, BSFixedString caseSentinel, BSFixedString replaceWith) {
            if (!haystack.data || !regex.data || !replaceWith.data)
               return haystack;
            ERROR_AND_RETURN_0_IF(!caseSentinel.data, "You must specify a non-empty case sentinel.", registry, stackId);
            std::string rxFixed = regex.data;
            std::string sentinel = caseSentinel.data;
            ERROR_AND_RETURN_0_IF(cobb::utf8::count(rxFixed) != cobb::utf8::count(sentinel), "The case sentinel must have the same number of code points as the regex.", registry, stackId);
            _fix_regex(rxFixed, regex.data, sentinel);
            std::regex  ex(rxFixed.c_str(), std::regex::ECMAScript | std::regex::icase);
            std::string haystackSTL(haystack.data);
            std::string result;
            std::regex_replace(std::back_inserter(result), haystackSTL.begin(), haystackSTL.end(), ex, replaceWith.data);
            return result.c_str();
         };
      }
      namespace Sort {
         namespace Compare {
            SInt32 NaturalCompare_ASCII(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString a, BSFixedString b) {
               return cobb::utf8::naturalcompare(a.data, b.data);
            }
         }
         //
         VMResultArray<BSFixedString> NaturalSort_ASCII(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<BSFixedString> arr, bool descending) {
            VMResultArray<BSFixedString> result;
            {  // Copy input array into output array
               UInt32 size = arr.Length();
               result.reserve(size);
               for (UInt32 i = 0; i < size; i++) {
                  BSFixedString x;
                  arr.Get(&x, i);
                  result.push_back(x);
               }
            }
            std::sort(
               result.begin(),
               result.end(),
               [descending](const BSFixedString& x, const BSFixedString& y) {
                  std::string a(x.data);
                  std::string b(y.data);
                  if (descending)
                     std::swap(a, b);
                  return cobb::utf8::naturalcompare(a, b) > 0;
               }
            );
            return result;
         }
         template<typename T> VMResultArray<BSFixedString> NaturalSortPair_ASCII(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<BSFixedString> arr, VMArray<T> second, bool descending) {
            UInt32 size = arr.Length();
            if (size != second.Length()) {
               registry->LogError("The two arrays must be the same length.", stackId);
               //
               VMResultArray<BSFixedString> result;
               result.reserve(size);
               for (UInt32 i = 0; i < size; i++) {
                  BSFixedString x;
                  arr.Get(&x, i);
                  result.push_back(x);
               }
               return result;
            }
            //
            typedef std::pair<BSFixedString, T> _pair;
            std::vector<_pair> pairs;
            //
            VMResultArray<BSFixedString> result;
            {  // Copy input array into output array
               result.reserve(size);
               for (UInt32 i = 0; i < size; i++) {
                  BSFixedString x;
                  T y;
                  arr.Get(&x, i);
                  second.Get(&y, i);
                  pairs.emplace_back(x, y);
               }
            }
            std::sort(
               pairs.begin(),
               pairs.end(),
               [descending](const _pair& x, const _pair& y) {
                  auto result = cobb::utf8::naturalcompare(std::string(x.first.data), std::string(y.first.data));
                  if (descending)
                     result = -result;
                  return result > 0;
               }
            );
            for (UInt32 i = 0; i < size; i++) {
               result.push_back(pairs[i].first);
               second.Set(&pairs[i].second, i);
            }
            return result;
         }
      }
      BSFixedString Trim(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString str) {
         if (!str.data)
            return nullptr;
         std::string output = str.data;
         auto start = output.find_first_not_of(' ');
         if (start == std::string::npos)
            return "";
         auto end = output.find_last_not_of(' ') + 1;
         output = output.substr(start, end);
         return output.c_str();
      };
      SInt32 Length(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString str) {
         if (!str.data)
            return 0;
         std::string s(str.data);
         return cobb::utf8::count(s);
      };
   }
}

bool CobbPapyrus::String::Register(VMClassRegistry* registry) {
   // /*//
   {  // UNIT TESTS
      using namespace cobb;
      std::string testASCII   = u8"Test string";
      std::string testWin1252 = "Test • Test";
      std::string testChinese = u8"一周有七天。";
      //
      UInt32 count = 0;
      _MESSAGE("UTF8 TEST: ASCII: %s", testASCII.c_str());
      for (auto it = testASCII.begin(); it != testASCII.end(); utf8::advance(testASCII, it)) {
         auto distance = std::distance(testASCII.begin(), it);
         _MESSAGE("[%d==%d]: %04X", count, distance, utf8::get(testASCII, it));
         ++count;
      }
      _MESSAGE("[Length]: %d", utf8::count(testASCII));
      //
      count = 0;
      _MESSAGE("UTF8 TEST: WINDOWS-1252: %s", testWin1252.c_str());
      for (auto it = testWin1252.begin(); it != testWin1252.end(); utf8::advance(testWin1252, it)) {
         auto distance = std::distance(testWin1252.begin(), it);
         _MESSAGE("[%d==%d]: %04X", count, distance, utf8::get(testWin1252, it));
         ++count;
      }
      _MESSAGE("[Length]: %d", utf8::count(testWin1252));
      //
      count = 0;
      _MESSAGE("UTF8 TEST: CHINESE: %s", testChinese.c_str());
      for (auto it = testChinese.begin(); it != testChinese.end(); utf8::advance(testChinese, it)) {
         auto distance = std::distance(testChinese.begin(), it);
         _MESSAGE("[%d==%d]: %04X", count, distance, utf8::get(testChinese, it));
         ++count;
      }
      _MESSAGE("[Length]: %d", utf8::count(testChinese));
   }
   //*/
   {  // Numbers
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>(
         "BinaryToInt32",
         PapyrusPrefixString("String"),
         String::Numbers::BinaryToInt32,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "BinaryToInt32", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>(
         "HexToInt32",
         PapyrusPrefixString("String"),
         String::Numbers::HexToInt32,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "HexToInt32", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, BSFixedString, UInt32, SInt32>(
         "ToBinary",
         PapyrusPrefixString("String"),
         String::Numbers::ToBinary,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "ToBinary", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, BSFixedString, UInt32, SInt32>(
         "ToHex",
         PapyrusPrefixString("String"),
         String::Numbers::ToHex,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "ToHex", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Regex
      #ifdef COBB_PAPYRUS_OFFER_REGEX_STRING_APIS
         registry->RegisterFunction(
            new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, BSFixedString>(
               "RegexSearch", PapyrusPrefixString("String"), String::Regex::Search, registry
            )
         );
         registry->SetFunctionFlags(PapyrusPrefixString("String"), "RegexSearch", VMClassRegistry::kFunctionFlag_NoWait);
         registry->RegisterFunction(
            new NativeFunction4<StaticFunctionTag, VMResultArray<BSFixedString>, BSFixedString, BSFixedString, BSFixedString, bool>(
               "RegexMatch", PapyrusPrefixString("String"), String::Regex::Match, registry
            )
         );
         registry->SetFunctionFlags(PapyrusPrefixString("String"), "RegexMatch", VMClassRegistry::kFunctionFlag_NoWait);
         registry->RegisterFunction(
            new NativeFunction4<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString, BSFixedString>(
               "RegexReplace", PapyrusPrefixString("String"), String::Regex::Replace, registry
            )
         );
         registry->SetFunctionFlags(PapyrusPrefixString("String"), "RegexReplace", VMClassRegistry::kFunctionFlag_NoWait);
      #endif
   }
   {  // Sorts
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, SInt32, BSFixedString, BSFixedString>(
         "NaturalCompare_ASCII",
         PapyrusPrefixString("String"),
         String::Sort::Compare::NaturalCompare_ASCII,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "NaturalCompare_ASCII", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction2<StaticFunctionTag, VMResultArray<BSFixedString>, VMArray<BSFixedString>, bool>(
         "NaturalSort_ASCII",
         PapyrusPrefixString("String"),
         String::Sort::NaturalSort_ASCII,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "NaturalSort_ASCII", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMResultArray<BSFixedString>, VMArray<BSFixedString>, VMArray<SInt32>, bool>(
         "NaturalSortPairInt_ASCII",
         PapyrusPrefixString("String"),
         String::Sort::NaturalSortPair_ASCII<SInt32>,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "NaturalSortPairInt_ASCII", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, VMResultArray<BSFixedString>, VMArray<BSFixedString>, VMArray<TESForm*>, bool>(
         "NaturalSortPairForm_ASCII",
         PapyrusPrefixString("String"),
         String::Sort::NaturalSortPair_ASCII<TESForm*>,
         registry
      ));
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "NaturalSortPairForm_ASCII", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Utilities
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>(
            "Trim",
            PapyrusPrefixString("String"),
            String::Trim,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "Trim", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(
         new NativeFunction1<StaticFunctionTag, SInt32, BSFixedString>(
            "CPLength",
            PapyrusPrefixString("String"),
            String::Length,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "CPLength", VMClassRegistry::kFunctionFlag_NoWait);
   }
   return true;
};