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
         luastackchange_t setPosFloat(lua_State* L, float RE::TESEffectShader::Data::*member) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            float val = lua_tonumber(L, 2);
            luaL_argcheck(L, !isnan(val), 2, "the value used cannot be NaN");
            // TODO: infinity checks
            if (val < 0.0F) {
               val = 0.0F;
               util::argwarn(L, 2, "the value must be positive or zero");
            }
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
         luastackchange_t setBlendMode(lua_State* L, RE::TESEffectShader::BlendMode RE::TESEffectShader::Data::*member) {
            IForm* wrapper = IEffectShader::fromStack(L, 1);
            luaL_argcheck(L, wrapper != nullptr, 1, "'IEffectShader' expected");
            auto form = (RE::TESEffectShader*) wrapper->unwrap();
            if (!form)
               return 0;
            luaL_argcheck(L, lua_type(L, 2) == LUA_TNUMBER, 2, "number (enum) expected");
            UInt32 value = lua_tonumber(L, 2);
            if (value < 1 || value > RE::TESEffectShader::kBlendMode_SourceAlphaSAT) {
               luaL_argerror(L, 2, "Invalid blend mode specified.");
            }
            form->data.*member = (RE::TESEffectShader::BlendMode)value;
            return 0;
         };
      }
      namespace _methods {
         template<float RE::TESEffectShader::Data::*member>
         luastackchange_t getFloat(lua_State* L) {
            return _helpers::getFloat(L, member);
         }
         template<float RE::TESEffectShader::Data::*member>
         luastackchange_t setFloat(lua_State* L) {
            return _helpers::setFloat(L, member);
         }
         template<float RE::TESEffectShader::Data::*member>
         luastackchange_t setPosFloat(lua_State* L) {
            return _helpers::setPosFloat(L, member);
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
         template<RE::TESEffectShader::BlendMode RE::TESEffectShader::Data::*member>
         luastackchange_t setBlendMode(lua_State* L) {
            return _helpers::setBlendMode(L, member);
         }

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
      { "getFillColorKey1", _methods::getColor<&RE::TESEffectShader::Data::fillColorKey1> },
      { "setFillColorKey1", _methods::setColor<&RE::TESEffectShader::Data::fillColorKey1> },
      { "getFillColorKey2", _methods::getColor<&RE::TESEffectShader::Data::fillColorKey2> },
      { "setFillColorKey2", _methods::setColor<&RE::TESEffectShader::Data::fillColorKey2> },
      { "getFillColorKey3", _methods::getColor<&RE::TESEffectShader::Data::fillColorKey3> },
      { "setFillColorKey3", _methods::setColor<&RE::TESEffectShader::Data::fillColorKey3> },
      // TODO: getMembraneSourceBlendMode
      { "setMembraneSourceBlendMode", _methods::setBlendMode<&RE::TESEffectShader::Data::membraneSourceBlendMode> },
      // TODO: getMembraneDestinationBlendMode
      { "setMembraneDestinationBlendMode", _methods::setBlendMode<&RE::TESEffectShader::Data::membraneDestinationBlendMode> },
      { "getEdgeFalloff", _methods::getFloat<&RE::TESEffectShader::Data::edgeFalloff> },
      { "setEdgeFalloff", _methods::setPosFloat<&RE::TESEffectShader::Data::edgeFalloff> },

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