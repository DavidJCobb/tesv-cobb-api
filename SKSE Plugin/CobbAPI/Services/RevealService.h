#pragma once

#include <array>
#include <mutex>
#include <unordered_map>
#include <string>
using namespace std;

#include "skse/GameObjects.h"
#include "skse/GameTypes.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectLIGH.h"

#include "Services/Messaging.h"
#include "Types/bounds.h"

//
// A service for revealing lights and customs.
//
// To reveal LIGHTS, we maintain a set of five models and bounds -- one for every light 
// type, including one for an impossible light type (non-shadow-casting spotlight). When 
// it comes time to set lights' reveal state, we iterate over all light base forms in 
// the game and modify their model paths; we then loop over all references in the loaded 
// area and force any light references to reload 3D.
//
// When multiple DLLs have RevealServices, only one shall take responsibility for lights. 
// The DLL that most recently defined five light models shall be the one held responsible 
// for revealing and hiding lights. DLLs shall synchronize light bounds definitions so 
// that alternate services, like CellSearcher, can get the right bounds when lights are 
// revealed.
//
// A CUSTOM is any base form that has had an alternate model and set of bounds registered 
// with the RevealService. Customs can be "revealed" (swapped to the alternate data) or 
// "reverted" (swapped back to their original data). The intended purpose of customs is 
// to allow mods to define editor markers for references, for use with Cobb Positioner. 
// Customs are referred to in code as the "main" reveal state.
//
// When multiple DLLs have RevealServices, each needs to be aware of the others' regist-
// rations so that CellSearcher can function. As such, RevealService maintains a set of 
// "owned" and "foreign" registrations. Reveal-paths are not needed for foreign registr-
// ations, and are not stored at this time.
//

class RevealServiceMessage;
class RevealService { // RENAME ME
   //
   // Class to "reveal" forms by changing their models. There are two types of revealing:
   //
   //  - MAIN REVEALING is essentially a comparable feature to the Creation Kit's editor 
   //    markers. You can register a form to be main-revealed with a specific "revealed" 
   //    model and bounds.
   //
   //  - LIGHT REVEALING is used for lights specifically. You can register a model and 
   //    bounds to be used with a specific type of light.
   //
   // Main-revealing takes precedence over light-revealing.
   //
   public:
      static RevealService& GetInstance() {
         static RevealService instance;
         return instance;
      };
      //
      struct LightModel {
         std::string path = "";
         struct {
            NiPoint3 min;
            NiPoint3 max;
         } bounds;
         //
         void SetFromForm(TESForm*);
      };
      //
      LightModel lightModelOmni;
      LightModel lightModelOmniShadow;
      LightModel lightModelHemi;
      LightModel lightModelSpot;
      bool       thisDLLDoesLights = false;
      //
      typedef UInt32 FormID;
      struct Registration {
         TESForm*    form = nullptr;
         struct {
            NiPoint3 min;
            NiPoint3 max;
         } bounds;
         std::string pathRevert; // path prior to our intervention; grabbed and stored when the form is registered
         std::string pathReveal;
      };
      typedef std::vector<Registration> Transaction;
      //
   protected:
      std::recursive_mutex lock;
      typedef std::lock_guard<decltype(lock)> svc_scoped_lock;
      //
      struct {
         std::unordered_map<FormID, Registration> owned;
         std::unordered_map<FormID, Registration> foreign; // NOTE: model paths are unavailable for these registrations; see OnMessage for details
      } registrations;
      bool revealStateMain   = false;
      bool revealStateLights = false;
      //
      inline bool FormIsCustomMarker(UInt32 formID) {
         try {
            this->registrations.owned.at(formID);
            return true;
         } catch (std::out_of_range) {
            try {
               this->registrations.foreign.at(formID);
               return true;
            } catch (std::out_of_range) {}
         }
         return false;
      };
      //
      const LightModel& GetLightModelForLight(RE::TESObjectLIGH*) const;
      void Reload3DForArea(bool modifiedLights = true, bool modifiedMain = true) const;
      void Reload3DForReference(TESObjectREFR*, bool modifiedLights, bool modifiedMain) const;
      void SetCustomMarkerModel(TESForm* form, const std::string& path, bool state) const;
      //
   public:
      void DefineLightModels(TESForm* omni, TESForm* omniShadow, TESForm* hemi, TESForm* spot, TESForm* spotShadow);
      void GetFormBounds(TESForm* baseForm, Bounds& outBounds) const; // outBounds will not be modified if we are not revealing the form
      bool GetRevealStateLights() const { return this->revealStateLights; };
      bool GetRevealStateMain() const { return this->revealStateMain; };
      //
      void RegisterForm(const Registration& add, bool isBulk = false);
      void RegisterFormsInBulk(const Transaction& forms);
      //
      void SetAllRevealStates (bool state, bool reload = true, bool local = true); // "synchronize" determines whether we send messages to other DLLs
      void SetLightRevealState(bool state, bool reload = true);
      void SetMainRevealState (bool state, bool reload = true, bool local = true);
      //
      void MassReset(); // does not synchronize with other DLLs
      //
      void OnMessage(const CobbAPIMessage*);
};
class RevealServiceMessage : public CobbAPIMessage { // move to RevealService.h?
   public:
      static constexpr UInt32 typeCode = 'REVL';
      enum Subtype : UInt32 {
         kSubtype_FormsAdded     = 'FADD',
         kSubtype_SetLightModels = 'LIGH',
         kSubtype_RevealStateChanged      = 'ST8_',
         kSubtype_RequestLightStateChange = 'ST8L',
         kSubtype_RequestAddForms = 'ADRQ',
         kSubtype_PostMassReset   = 'RSET',
      };
   protected:
      Subtype _subtype;
      RevealServiceMessage(Subtype t) : CobbAPIMessage(typeCode), _subtype(t) {};
   public:
      inline const decltype(_subtype) subtype() const { return this->_subtype; };
      //
      inline static RevealServiceMessage* convert(const CobbAPIMessage* m) {
         if (m->type() == typeCode)
            return (RevealServiceMessage*) m;
         return nullptr;
      };
};
class RevealServiceMessagePostMassReset : public RevealServiceMessage { // Not used for synchronization between services, but provided for external callers
   public:
      RevealServiceMessagePostMassReset() : RevealServiceMessage(kSubtype_PostMassReset) {};
};
class RevealServiceMessageFormsAdded : public RevealServiceMessage {
   public:
      RevealServiceMessageFormsAdded() : RevealServiceMessage(kSubtype_FormsAdded) {};
      //
      const RevealService::Transaction* transaction = nullptr;
      //
      bool wasBeingRevealed = false; // if another DLL is revealing any of these forms, it will set this
};
class RevealServiceMessageLightModelsSet : public RevealServiceMessage {
   public:
      RevealServiceMessageLightModelsSet() : RevealServiceMessage(kSubtype_SetLightModels) {};
      //
      TESForm* omni = nullptr;
      TESForm* omniShadow = nullptr;
      TESForm* hemi = nullptr;
      TESForm* spot = nullptr;
      TESForm* spotShadow = nullptr;
      //
      bool lightsActive = false; // if another DLL is revealing lights, it will set this
};
class RevealServiceMessageMarkerRevealStateChanged : public RevealServiceMessage {
   public:
      RevealServiceMessageMarkerRevealStateChanged() : RevealServiceMessage(kSubtype_RevealStateChanged) {};
      //
      bool state;
};
class RevealServiceMessageRequestLightStateChange : public RevealServiceMessage {
   public:
      RevealServiceMessageRequestLightStateChange() : RevealServiceMessage(kSubtype_RequestLightStateChange) {};
      //
      bool state;
      bool reload3D;
};
class RevealServiceMessageRequestAddForms : public RevealServiceMessage {
   public:
      typedef RevealService::Transaction Transaction;
      RevealServiceMessageRequestAddForms(Transaction* t) : transaction(t), RevealServiceMessage(kSubtype_RequestAddForms) {};
      //
      const Transaction* transaction = nullptr;
};