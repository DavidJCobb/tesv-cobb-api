#include "IEffectShader.h"
#include "LuaSkyrim/_classes.h"
#include "LuaSkyrim/_utilities.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/TESEffectShader.h"

namespace LuaSkyrim {
   namespace { // metatable methods
      namespace _helpers {
         luastackchange_t getFloat(lua_State* L, float RE::TESEffectShader::Data::*member) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            lua_pushnumber(L, form->data.*member);
            return 1;
         };
         luastackchange_t setFloat(lua_State* L, float RE::TESEffectShader::Data::*member) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            float val = lua_tonumber(L, 2);
            luaL_argcheck(L, !isnan(val), 2, "the value used cannot be NaN");
            // TODO: infinity checks
            form->data.*member = val;
            return 0;
         };
         luastackchange_t getColor(lua_State* L, UInt32 RE::TESEffectShader::Data::*member) {
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
         luastackchange_t setColor(lua_State* L, UInt32 RE::TESEffectShader::Data::*member) {
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
         luastackchange_t setRatio(lua_State* L, float RE::TESEffectShader::Data::*member) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            float val = lua_tonumber(L, 2);
            if (val < 0.0F || val > 1.0F) {
               util::argwarn(L, 2, "The value should be between 0 and 1 inclusive; clamping to the range...");
               if (val < 0.0F)
                  val = 0.0F;
               else
                  val = 1.0F;
            }
            form->data.*member = val;
            return 0;
         };
      }
      namespace _methods {
         template<float RE::TESEffectShader::Data::*member>
         luastackchange_t getFloat(lua_State* L) {
            return _helpers::getFloat(L, member);
         }
         //
         template<UInt32 RE::TESEffectShader::Data::*member>
         luastackchange_t getColor(lua_State* L) {
            return _helpers::getColor(L, member);
         }
         template<UInt32 RE::TESEffectShader::Data::*member>
         luastackchange_t setColor(lua_State* L) {
            return _helpers::setColor(L, member);
         }
         //
         template<float RE::TESEffectShader::Data::*member>
         luastackchange_t setRatio(lua_State* L) {
            return _helpers::setRatio(L, member);
         }
         //
         luastackchange_t setFillColorKeys(lua_State* L) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            for (int i = 0; i < 3; i++) {
               auto arg = 1 + i;
               if (lua_isnoneornil(L, arg))
                  continue;
               UInt32 color = 0;
               for (int j = 0; j < 4; j++) {
                  auto t = lua_geti(L, arg, j + 1);
                  if (t != LUA_TNUMBER) {
                     if (j == 4) { // alpha is optional
                        if (t == LUA_TNONE || t == LUA_TNIL)
                           continue;
                     }
                     char err[256];
                     snprintf(err, sizeof(err), "color component #%u is not a number", j);
                     luaL_argerror(L, i, err);
                  }
                  UInt8 component = lua_tointeger(L, -1);
                  lua_pop(L, 1);
                  color |= component << (0x8 * j);
               }
               UInt32 RE::TESEffectShader::Data::*member;
               switch (i) {
                  case 0:
                     member = &RE::TESEffectShader::Data::fillColorKey1;
                     break;
                  case 1:
                     member = &RE::TESEffectShader::Data::fillColorKey2;
                     break;
                  case 2:
                     member = &RE::TESEffectShader::Data::fillColorKey3;
                     break;
               }
               form->data.*member = color;
            }
            return 0;
         };

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
      { "getEdgeColor", _methods::getColor<&RE::TESEffectShader::Data::edgeColor> },
      { "setEdgeColor", _methods::setColor<&RE::TESEffectShader::Data::edgeColor> },
      { "getFillFullAlphaRatio", _methods::getFloat<&RE::TESEffectShader::Data::fillAlphaRatioFull> },
      { "setFillFullAlphaRatio", _methods::setRatio<&RE::TESEffectShader::Data::fillAlphaRatioFull> },
      { "getFillPersistentAlphaRatio", _methods::getFloat<&RE::TESEffectShader::Data::fillAlphaRatioPersistent> },
      { "setFillPersistentAlphaRatio", _methods::setRatio<&RE::TESEffectShader::Data::fillAlphaRatioPersistent> },
      { "setFillColorKeys", _methods::setFillColorKeys }, // args are three arrays of color components { r, g, b, optional_a }; nil instead of an array == don't change this color

      // TODO: commit everything to Github; then change your mind and split 
      // setFillColorKeys into setFillColorKey1, ...2, and ...3

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