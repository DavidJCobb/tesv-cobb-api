#include "IActor.h"
#include "LuaSkyrim/_classes.h"
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/GameRTTI.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _methods {
         luastackchange_t sneaking(lua_State* L) {
            IForm* wrapper = IActor::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IActor' expected");
            auto form = (RE::Actor*) wrapper->unwrap();
            if (!form)
               return 0;
            bool result = CALL_MEMBER_FN(form, IsSneaking)();
            lua_pushboolean(L, result);
            return 1;
         };
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "sneaking", _methods::sneaking },
      { NULL, NULL }
   };
   void IActor::setupMetatable(lua_State* luaVM) {
      static bool isDefined = false;
      if (isDefined)
         return;
      _defineClass(luaVM, metatableName, IReference::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_Character, formWrapperFactory<IActor>);
      mapFormTypeToMetatable(luaVM, kFormType_Character, metatableName);
      //
      // TODO: It would be valuable to import a singleton for each form interface. These 
      // singletons could hold useful static functions (e.g. "for each form of this type") 
      // and relevant constants.
      //
      isDefined = true;
   };
   IActor* IActor::fromStack(lua_State* luaVM, UInt32 stackPos) {
      return (IActor*) _asClass(luaVM, stackPos, metatableName);
   };
};