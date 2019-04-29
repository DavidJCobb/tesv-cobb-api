#include "SkyrimLuaService.h"
#include "FormWrappers/IForm.h"

using namespace LuaSkyrim;

void SkyrimLuaService::OnReferenceDelete(UInt32 formID) {
   if (!this->state)
      return;
   auto luaVM = this->state;
   //
   lua_getfield(luaVM, LUA_REGISTRYINDEX, ce_formWrapperReuseKey); // STACK: [list]
   if (lua_rawgeti(luaVM, -1, formID) == LUA_TUSERDATA) {  // STACK: [wrapper, list]
      IForm* a = IForm::fromStack(luaVM, -1);
      if (a)
         a->isDeleted = true;
   }
   lua_pop(luaVM, 2);
}