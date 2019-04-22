#pragma once
#include "_includes.h"

class TESForm;
namespace LuaSkyrim {
   //
   // Interface for basic forms. Not suitable for forms that can be unloaded or 
   // deleted at run-time.
   //
   // When defining subclasses, you should:
   //
   //  - Modify a switch-case in IForm::make, to associate your subclass with the 
   //    proper form type.
   // 
   //  - Have your subclass inherit from IForm.
   // 
   //  - Have your subclass create its metatable similarly to IForm, but passing 
   //    IForm::metatableName as the superclass name.
   //
   // TODO:
   //
   //  - Switch-cases suck. Is there a better way to associate classes (or their 
   //    metatables) with form types?
   //
   //     - We could have the I[Whatever]::setupMetatable function register the 
   //       metatable in some table on the Lua registry mapping form types to 
   //       metatables, and then have IForm::make look the metatable up from 
   //       that table (defaulting to the IForm metatable if it's not found).
   //
   //  - We should have a singleton, _G["skyrim"], for the majority of game API 
   //    functions (e.g. "getFormByID"). Inside this singleton, we should have 
   //    one singleton for each form interface (e.g. skyrim.IForm); these form 
   //    interface singletons could hold useful methods (e.g. "for each form of 
   //    this type") and constants (e.g. IActorBase.GENDER_MALE = 0).
   //
   class IForm {
      public:
         static constexpr char* metatableName    = "Skyrim.IForm";
         static constexpr char* subclassListName = "Skyrim.IForm.subclasses"; // name of a table mapping form types to subclass metatables
         static constexpr char* registryName     = "Skyrim.IForm.wrappers"; // name of a weakmap holding existing form wrappers, so they can be reused

         static void setupMetatable(lua_State* luaVM);
         static void mapFormTypeToMetatable(lua_State* luaVM, uint8_t formType, const char* metatableName); // must have called IForm::setupMetatable first

         static IForm* fromStack(lua_State* luaVM, UInt32 stackPos = 1);

         static luastackchange_t make(lua_State* luaVM, TESForm*);

         TESForm* wrapped = nullptr;
   };

   //
   // Here's the approach that needs to be taken for providing access to forms to Lua 
   // scripts:
   //
   // First, we need to define the Lua-side wrapper. When we return a form, we will 
   // return it as a "heavy userdata:" a piece of arbitrary C data which is managed 
   // by the Lua VM, and which (for the script side of things) has a metatable that 
   // we define and provide. For forms, the C data will simply be a pointer, and the 
   // metatable will be what provides our various member functions; we'll create the 
   // metatable in C.
   //
   // To create an instance of heavy userdata at the top of the Lua stack, we call:
   //
   //    lua_newuserdata  (lua_state, size_of_c_data_in_bytes)
   //    luaL_getmetatable(lua_state, "name_with_which_the_metatable_was_registered");
   //    lua_setmetatable (lua_state, -2);
   //
   // There is one slight problem with that, however: if we run that code every time 
   // we wish to return a form, then we can end up creating duplicate heavy userdatas 
   // for the same form. To work around this, we create a list of existing wrappers 
   // for forms. Specifically:
   //
   //  - We create a "weak table," analogous to a JavaScript WeakMap, and store it on 
   //    the "Lua registry." The registry is sort of like a global scope, but... more.
   //
   //  - The keys to this weakmap will be raw form pointers (which we offer to Lua in 
   //    the form of "light userdata"). The values will be heavy userdata.
   //
   //  - When it's time to return a form, we look it up in the weakmap. We return a 
   //    preexisting heavy userdata if there is one, or create, store, and return a 
   //    new one otherwise.
   //
   // What is a light userdata? Well, it's literally just a C pointer. Light userdata 
   // have value semantics -- akin to numbers, strings, and booleans -- while heavy 
   // userdata have table semantics, with the ability to define metatables and the 
   // like. This means that our weakmap can exist purely as a Lua construct.
   //
   // There's one wrinkle: if we're reusing heavy userdata for forms, then what's to 
   // stop one script from overriding member functions on a form's wrapper in order 
   // to mess with other scripts? Well, heavy userdata have "table semantics," but 
   // they aren't actual tables. They can't store key/value pairs, so rawget and 
   // rawset don't affect them; only their metatables can have data.
   //
}