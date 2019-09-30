#pragma once
#include "LuaSkyrim/_includes.h"
#include "IForm.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"

class TESForm;
namespace LuaSkyrim {
   class IReference : public IForm {
      public:
         IReference(TESForm* form);

         virtual void resolve() override;
         virtual void abandon() override;
         virtual const char* signature() const override { return "REFR"; };
         virtual bool ruleOutForm(TESForm*) override; // TODO: We shouldn't need this for REFR due to the form-delete hook; we still need it for NPC_ and PACK, maybe others

         //
         // These fields serve as "directions," used to retrieve a reference after it 
         // has been unloaded and reloaded. The refHandle field is also used to tell 
         // the originally-wrapped reference apart from another reference, if both are 
         // created references, the former was deleted, and the latter came to occupy 
         // the former's form ID.
         //
         uint32_t formID    = 0;
         uint32_t refHandle = 0; // the unique portion of the reference's handle (i.e. bits excluding "active" and the "reuse" bits)

         static constexpr char* metatableName = "Skyrim.IReference";

         static void        setupClass(lua_State* luaVM);
         static IReference* fromStack(lua_State* luaVM, SInt32 stackPos = -1);
   };
}