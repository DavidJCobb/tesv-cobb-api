#include "IEffectShader.h"
#include "LuaSkyrim/_classes.h"
#include "LuaSkyrim/_utilities.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESEffectShader.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _helpers {
         template<UInt32 RE::TESEffectShader::Data::*member>
         luastackchange_t getColor(lua_State* L) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            UInt32 color = form->data.*member;
            UInt8 r = color & 0xFF;
            UInt8 g = (color >> 0x08) & 0xFF;
            UInt8 b = (color >> 0x10) & 0xFF;
            UInt8 a = color >> 0x18;
            lua_pushinteger(L, r);
            lua_pushinteger(L, g);
            lua_pushinteger(L, b);
            lua_pushinteger(L, a);
            return 4;
         };
         template<UInt32 RE::TESEffectShader::Data::*member>
         luastackchange_t setColor(lua_State* L) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            UInt8 r = util::getColorComponentArg(L, 2, 2);
            UInt8 g = util::getColorComponentArg(L, 3, 3);
            UInt8 b = util::getColorComponentArg(L, 4, 4);
            UInt32 color = r | (g << 0x08) | (b << 0x10) | 0xFF000000;
            form->data.*member = color;
            return 0;
         };
      }
      namespace _methods {
         /*//
         luastackchange_t getValue(lua_State* L) {
            IForm* wrapper = IEffectShader::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->value);
            return 1;
         };
         luastackchange_t setValue(lua_State* L) {
            IForm* wrapper = IEffectShader::fromStack(L);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            float value = util::getNumberArg(L, 2, 2);
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (form) {
               if (form->IsConstant())
                  luaL_error(L, "Cannot set the value of a constant global-variable form. If you need to change a constant global-variable form's value at run-time, use forceValue.");
               form->value = value;
            }
            return 0;
         };
         //*/
      }
   }
   static const luaL_Reg _metatableMethods[] = {
      { "getEdgeColor", _helpers::getColor<&RE::TESEffectShader::Data::edgeColor> },
      { "setEdgeColor", _helpers::setColor<&RE::TESEffectShader::Data::edgeColor> },
      /*//
      { "getValue", _methods::getValue },
      { "setValue", _methods::setValue },
      //*/
      { NULL, NULL }
   };
   void IEffectShader::setupClass(lua_State* luaVM) {
      if (_isClassDefined(luaVM, metatableName))
         return;
      _defineClass(luaVM, metatableName, IForm::metatableName, _metatableMethods);
      mapFormTypeToFactory  (kFormType_EffectShader, formWrapperFactory<IEffectShader>);
      mapFormTypeToMetatable(luaVM, kFormType_EffectShader, metatableName);
   };
   IEffectShader* IEffectShader::fromStack(lua_State* luaVM, SInt32 stackPos) {
      return (IEffectShader*) _asClass(luaVM, stackPos, metatableName);
   };
};