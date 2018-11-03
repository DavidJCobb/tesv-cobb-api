#include "Papyrus/String.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

//#define COBB_PAPYRUS_OFFER_REGEX_STRING_APIS 1
#ifdef COBB_PAPYRUS_OFFER_REGEX_STRING_APIS
   #include <regex> // adds, like 80KB to the DLL
#endif

#include <string>
#include "Miscellaneous/utf8string.h"

/********************************************************************************************\

   SOME EXPLANATION:

   Skyrim and the Creation Kit use UTF-8 strings. If you open a Japanese ESP file in an 
   English Creation Kit, you'll probably only see question-mark characters... but if you 
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
            if (length < 0) {
               registry->LogWarning("Cannot format a binary value to a negative number of bits. Defaulting to 32.", stackId);
               length = 32;
            } else if (length > 32) {
               registry->LogWarning("Cannot format a binary value longer than 32 bits.", stackId);
               length = 32;
            }
            char bin[33];
            memset(bin, '0', length);
            bin[length] = '\0';
            for (UInt8 i = 0; i < length; i++)
               if (value & (1 << i))
                  bin[length - i] = '1';
            return bin; // passes through BSFixedString constructor, which I believe caches the string, so returning local vars should be fine
         }
         BSFixedString ToHex(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, UInt32 value, SInt32 length) {
            if (length < 0) {
               registry->LogWarning("Cannot format a hexadecimal valueinteger to a negative number of digits. Defaulting to eight.", stackId);
               length = 8;
            } else if (length > 8) {
               registry->LogWarning("Cannot format a hexadecimal integer longer than eight digits.", stackId);
               length = 8;
            }
            char hex[9];
            memset(hex, '0', length);
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
      //
      // BUG: ALL std::string FIND CALLS ARE BROKEN BECAUSE THEY ARE CASE-SENSITIVE
      //
      SInt32 Compare(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString a, BSFixedString b) {
         if (!a.data) {
            if (!b.data)
               return 0;
            return -1;
         }
         if (!b.data)
            return 1;
         return strcmp(a.data, b.data);
      };
      SInt32 FindFirstOf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString needles, SInt32 startAt) {
         if (!haystack.data || !needles.data)
            return -1;
         return std::string(haystack.data).find_first_of(needles.data, startAt);
      };
      SInt32 FindFirstNotOf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString needles, SInt32 startAt) {
         if (!haystack.data || !needles.data)
            return -1;
         return std::string(haystack.data).find_first_not_of(needles.data, startAt);
      };
      SInt32 FindLastOf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString needles, SInt32 startAt) {
         if (!haystack.data || !needles.data)
            return -1;
         return std::string(haystack.data).find_last_of(needles.data, startAt);
      };
      SInt32 FindLastNotOf(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString needles, SInt32 startAt) {
         if (!haystack.data || !needles.data)
            return -1;
         return std::string(haystack.data).find_last_not_of(needles.data, startAt);
      };
      #ifdef COBB_PAPYRUS_OFFER_REGEX_STRING_APIS
         bool RegexSearch(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString regex) {
            if (!haystack.data || !regex.data)
               return false;
            std::regex ex(regex.data, std::regex::ECMAScript | std::regex::icase);
            return std::regex_match(haystack.data, ex);
         };
         VMResultArray<BSFixedString> RegexMatch(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString regex) {
            VMResultArray<BSFixedString> result;
            if (!haystack.data || !regex.data)
               return result;
            std::regex  ex(regex.data, std::regex::ECMAScript | std::regex::icase);
            std::cmatch matches;
            std::regex_match(haystack.data, matches, ex);
            result.reserve(matches.size());
            for (UInt32 i = 0; i < matches.size(); i++)
               result.push_back(matches[i].second);
            return result;
         };
         BSFixedString RegexReplace(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString haystack, BSFixedString regex, BSFixedString replaceWith) {
            if (!haystack.data || !regex.data || !replaceWith.data)
               return haystack;
            std::regex  ex(regex.data, std::regex::ECMAScript | std::regex::icase);
            std::string haystackSTL(haystack.data);
            std::string result;
            std::regex_replace(std::back_inserter(result), haystackSTL.begin(), haystackSTL.end(), ex, replaceWith.data);
            return result.c_str();
         };
      #endif
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
      //
      /*// Not meaningful; "compare" should be a tool for an alph sort, but that's harder to implement in Unicode
      SInt32 UTF8Compare(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString a, BSFixedString b) {
         if (!a.data) {
            if (!b.data)
               return 0;
            return -1;
         }
         if (!b.data)
            return 1;
         std::string sA(a.data);
         std::string sB(b.data);
         auto iA = cobb::utf8charpos(sA);
         auto iB = cobb::utf8charpos(sB);
         for (; !iA.is_end() && !iB.is_end(); ++iA, ++iB) {
            auto cA = *iA;
            auto cB = *iB;
            if (cA != cB)
               return cA - cB;
         }
         if (!iA.is_end())
            return 1;
         if (!iB.is_end())
            return -1;
         return 0;
      };*/
      SInt32 UTF8Length(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, BSFixedString str) {
         if (!str.data)
            return 0;
         SInt32 result = 0;
         std::string s(str.data);
         auto i = cobb::utf8charpos(s);
         for (; !i.is_end(); ++i)
            result++;
         return result;
      };
   }
}

bool CobbPapyrus::String::Register(VMClassRegistry* registry) {
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

   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, SInt32, BSFixedString, BSFixedString>(
         "Compare",
         PapyrusPrefixString("String"),
         String::Compare,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "Compare", VMClassRegistry::kFunctionFlag_NoWait);
   //
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>(
         "FindFirstOf", PapyrusPrefixString("String"), String::FindFirstOf, registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "FindFirstOf", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>(
         "FindFirstNotOf", PapyrusPrefixString("String"), String::FindFirstOf, registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "FindFirstNotOf", VMClassRegistry::kFunctionFlag_NoWait);
   //
   
   //
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>(
         "FindLastOf", PapyrusPrefixString("String"), String::FindLastOf, registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "FindLastOf", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>(
         "FindLastNotOf", PapyrusPrefixString("String"), String::FindLastOf, registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "FindLastNotOf", VMClassRegistry::kFunctionFlag_NoWait);
   //
   #ifdef COBB_PAPYRUS_OFFER_REGEX_STRING_APIS
      registry->RegisterFunction(
         new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>(
            "RegexSearch", PapyrusPrefixString("String"), String::RegexSearch, registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "RegexSearch", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(
         new NativeFunction2<StaticFunctionTag, VMResultArray<BSFixedString>, BSFixedString, BSFixedString>(
            "RegexMatch", PapyrusPrefixString("String"), String::RegexMatch, registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "RegexMatch", VMClassRegistry::kFunctionFlag_NoWait);
      registry->RegisterFunction(
         new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString>(
            "RegexReplace", PapyrusPrefixString("String"), String::RegexReplace, registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("String"), "RegexReplace", VMClassRegistry::kFunctionFlag_NoWait);
   #endif
   //
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>(
         "Trim",
         PapyrusPrefixString("String"),
         String::Trim,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "Trim", VMClassRegistry::kFunctionFlag_NoWait);
   //
   /*registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, SInt32, BSFixedString, BSFixedString>(
         "UTF8Compare",
         PapyrusPrefixString("String"),
         String::UTF8Compare,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "UTF8Compare", VMClassRegistry::kFunctionFlag_NoWait);*/
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, BSFixedString>(
         "UTF8Length",
         PapyrusPrefixString("String"),
         String::UTF8Length,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("String"), "UTF8Length", VMClassRegistry::kFunctionFlag_NoWait);
   //
   return true;
};