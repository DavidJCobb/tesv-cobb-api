#pragma once
#include <exception>
#include "ReverseEngineered/Systems/ChangeForms.h"

static bool ReferenceWithIDExists(UInt32 formID){
   if (LookupFormByID(formID) != NULL)
      return true;
   if ((formID & 0xFF000000) != 0xFF000000)
      return false;
   //
   // LookupFormByID can't retrieve unloaded temporary references and returns NULL for them. We'll check the changeforms.
   //
   {
      RE::Unknown01B2E38C* Unk01B2E38C = RE::Unknown01B2E38C::GetInstance();
      if (Unk01B2E38C == NULL)
         throw new std::runtime_error("Couldn't access changeFlags.");
      UInt32 changeFlags;
      return CALL_MEMBER_FN(Unk01B2E38C->unk3E8, GetChangeFlags)(&changeFlags, formID); // returns true if it can retrieve the change flags
   }
};