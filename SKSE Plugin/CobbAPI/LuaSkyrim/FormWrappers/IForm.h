#pragma once
#include "LuaSkyrim/_includes.h"
#include "skse/GameForms.h" // for easy access to kFormType enum

class TESForm;
namespace LuaSkyrim {
   //
   // IFORM
   //
   // Interface for forms; the base type used for heavy-userdata that serve 
   // as wrappers for passing TESForm pointers to Lua. It can and should be 
   // subclassed for different form types, and there are some facilities 
   // built-in to allow for form types whose forms can be unloaded or deleted 
   // from memory during Skyrim's normal operation.
   //
   // WHEN DEFINING SUBCLASSES, YOU SHOULD:
   // 
   //  - Have your subclass inherit from IForm.
   //
   //     - If your subclass represents a form type that is always in memory, 
   //       then define its constructor as so:
   //
   //          YourSubclassHere(TESForm* f) : IForm(f) {};
   //
   //     - If your subclass represents a form type that can be unloaded or 
   //       deleted, then you should manually define a constructor that calls 
   //       TESForm's private constructor, passing true as the boolean, e.g.
   //
   //          YourSubclassHere(TESForm* f) : IForm(f, true) {};
   //
   //       If only some forms of your subclass can be unloaded, then you can 
   //       pass that condition as your boolean. For example, only actor-bases 
   //       that were created at run-time (0xFF load order index) can be deleted, 
   //       so IActorBase passes the result of a form ID check as the boolean.
   //
   //       Typically, wrappers for form types that can unload will contain 
   //       extra information that can be used to relocate the wrapped form, in 
   //       the case that it is unloaded and later reloaded. For example, a 
   //       wrapper for TESObjectREFR should store the reference form ID, while 
   //       a wrapper for an exterior TESObjectCELL should store the grid coord-
   //       inates and parent TESWorldSpace of the cell. You'll want to be doing 
   //       this in the constructor.
   //
   //  - Have your subclass override signature().
   //
   //     - If your subclass represents a form that can be unloaded or deleted 
   //       at run-time, then you must also override resolve(), abandon(), and 
   //       possibly ruleOutForm(...).
   //
   //       The general strategy for these form types is to store "directions to 
   //       the form" alongside the wrapped form pointer. Whenever Lua code isn't 
   //       running, we set the pointer to nullptr; the next time a Lua script 
   //       attempts to access the form, we "follow the directions" to retrieve 
   //       the form again. If the form has unloaded since the last time Lua 
   //       worked with it, then we'll get nullptr and we know that the form is 
   //       currently unavailable. If the form unloaded and then reloaded (such 
   //       that it's now located at a different address), then we'll get the 
   //       fresh, up-to-date pointer.
   //
   //       As for ruleOutForm(TESForm*)? Well, consider this case: we get a 
   //       wrapper for a created reference -- a TESObjectREFR with an 0xFF 
   //       load order index. The reference is later deleted, and the form ID 
   //       is reused for another reference. Then, Lua attempts to access the 
   //       new reference. Well, we already have a wrapper for that form ID... 
   //       How would we be able to tell that that wrapper is for a different 
   //       form? Well, every reference gets a unique(ish) handle, separate 
   //       from the form ID, so what if we also stored the reference handle? 
   //       And what if the "reuse existing wrappers" code could pass the new 
   //       reference to some function on the existing wrapper, which could 
   //       then compare the reference handles? Thus, ruleOutForm(TESForm*).
   //
   //       ruleOutForm should return true if the argument CANNOT BE the form 
   //       that the wrapper was originally created for.
   //
   //       Note that ruleOutForm is only relevant for forms that can be 
   //       created and deleted, since those are the only ones that should be 
   //       able to end up sharing formTypes and formIDs at run-time. Of course, 
   //       in practice, I think every form type that can be created can also be 
   //       deleted.
   // 
   //  - Have your subclass create its metatable similarly to IForm, but passing 
   //    IForm::metatableName as the superclass name.
   //
   //  - After creating the metatable, map it to its form type by way of a call 
   //    to IForm::mapFormTypeToMetatable. This will ensure that anything that 
   //    returns a form to Lua (by way of IForm::make) will use your subclass 
   //    if the form is of your subclass's form type.
   //
   //  - Map the subclass itself to its form type using mapFormTypeToFactory. 
   //    The factory argument should be formWrapperFactory<YourSubclassHere>.
   //
   // WHEN DEFINING MEMBER FUNCTIONS ON A SUBCLASS, YOU SHOULD:
   //
   //  - Begin them with this code:
   // 
   //       YourSubclassHere* wrapper = YourSubclassHere::fromStack(L);
   //       luaL_argcheck(L, wrapper != nullptr, 1, "'YourSubclassHere' expected");
   //       auto form = wrapper->unwrap();
   //
   //  - Check whether the form is nullptr.
   //
   // THINGS TO NOTE:
   //
   //  - Because we're mapping subclasses to form types, things can get a little 
   //    weird. Suppose you define a subclass for TESObjectREFR, but no subclass 
   //    for Actor. If Lua attempts to return an Actor, then it will use the IForm 
   //    wrapper, not the IReference wrapper, because IReference only maps to the 
   //    form type for TESObjectREFR and not the form type for Actor.
   //
   // TODO:
   //
   //  - We should have a singleton, _G["skyrim"], for the majority of game API 
   //    functions (e.g. "getFormByID"). Inside this singleton, we should have 
   //    one singleton for each form interface (e.g. skyrim.IForm); these form 
   //    interface singletons could hold useful methods (e.g. "for each form of 
   //    this type") and constants (e.g. IActorBase.GENDER_MALE = 0).
   //
   //  - We should probably move form interfaces to a subfolder, and have a 
   //    single file that includes all of them.
   //
   //     - That file should also define a function that sets up all form 
   //       metatables and similar for a given lua_State.
   //
   //  - If we hook the game's reference-delete event, then we don't need to 
   //    guard against new references reusing the form IDs of old references. 
   //    Likewise if we hook every gameplay-related deletion of other created 
   //    forms.
   //
   //     - We have code for this in place for references, although currently, 
   //       that isn't hooked up to our specific test (it's in SkyrimLuaService).
   //
   class IForm {
      protected:
         IForm(TESForm* form, bool canUnload) : canUnload(canUnload), wrapped(form), formType(form ? form->formType : 0) {};
      public:
         IForm(TESForm* form) : wrapped(form), formType(form ? form->formType : 0) {};
         //
         virtual void resolve() {};
         virtual void abandon() {};
         virtual const char* signature() const { return "FORM"; }; // return a four-character string matching the form type's record signature
         virtual bool ruleOutForm(TESForm*) { return false; };
         //
         static constexpr char* metatableName = "Skyrim.IForm";

         TESForm*      wrapped   = nullptr;
         const bool    canUnload = false; // whether this form can be unloaded/deleted at run-time
         bool          isDeleted = false; // indicates whether this wrapper wrapped a form that we know to have since been deleted
         const uint8_t formType  = 0;     // formType of the wrapped form

         static void   setupClass(lua_State* luaVM);
         static IForm* fromStack(lua_State* luaVM, SInt32 stackPos = -1);

         TESForm* unwrap() {
            if (this->isDeleted)
               return nullptr;
            if (this->wrapped == nullptr && this->canUnload)
               this->resolve();
            return this->wrapped;
         };
   };

   template<typename T> IForm* formWrapperFactory(lua_State* luaVM, TESForm* form) {
      void* a = lua_newuserdata(luaVM, sizeof(T));
      new (a) T(form);
      return (IForm*) a;
   };
   typedef IForm* (*FormWrapperFactoryFunction)(lua_State*, TESForm*);

   // Given a TESForm pointer, this function checks if a heavy-userdata has been 
   // created for the form; if so, that userdata is pushed onto the top of the Lua 
   // stack; otherwise, a heavy-userdata is created for the form and pushed onto 
   // the top of the stack.
   //
   // This function will create an IForm if the form is of an unrecognized type. 
   // You can register other heavy-userdata classes (which should subclass IForm) 
   // using IForm::mapFormTypeToMetatable.
   //
   extern luastackchange_t wrapForm(lua_State* luaVM, TESForm*);

   //
   // Map a form type to a constructor for an IForm subclass, so that we can 
   // probably create wrappers for that subclass without having to switch-case 
   // on the form type. This is needed because subclasses can vary in size, e.g. 
   // ICell which will need to store the worldspace pointer and grid coordinates 
   // for exterior cells, since those can unload unpredictably; or TESObjectREFR, 
   // which will need to store the form ID for similar reasons.
   //
   extern void mapFormTypeToFactory(uint8_t formType, FormWrapperFactoryFunction);

   //
   // Map a form type to a Lua metatable, so that wrapForm uses that metatable 
   // when passing a form to Lua. The metatable should be designed for a heavy 
   // userdata whose C buffer is an IForm instance, i.e. you should subclass 
   // IForm.
   //
   extern void mapFormTypeToMetatable(lua_State* luaVM, uint8_t formType, const char* metatableName);

   //
   // These keys are used in the Lua registry to power specific features for form 
   // wrappers.
   //
   extern constexpr char* ce_formSubclassListKey = "Skyrim.IForm.subclasses";
   extern constexpr char* ce_formWrapperReuseKey = "Skyrim.IForm.wrappers";

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