#pragma once
#include "Services/RevealService.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Systems/GameData.h"
#include "skse/GameData.h"
#include "skse/GameRTTI.h"

void RevealService::LightModel::SetFromForm(TESForm* form) {
   TESModel* pWorldModel = DYNAMIC_CAST(form, TESObjectACTI, TESModel);
   TESBoundObject* pWorldBounds = DYNAMIC_CAST(form, TESObjectACTI, TESBoundObject);
   if (!pWorldModel || !pWorldBounds)
      return;
   this->path = pWorldModel->GetModelName();
   this->bounds.min.x = (float)(SInt16)pWorldBounds->bounds.x;
   this->bounds.min.y = (float)(SInt16)pWorldBounds->bounds.y;
   this->bounds.min.z = (float)(SInt16)pWorldBounds->bounds.z;
   this->bounds.max.x = (float)(SInt16)pWorldBounds->bounds2.x;
   this->bounds.max.y = (float)(SInt16)pWorldBounds->bounds2.y;
   this->bounds.max.z = (float)(SInt16)pWorldBounds->bounds2.z;
};
//
const RevealService::LightModel& RevealService::GetLightModelForLight(RE::TESObjectLIGH* light) const {
   switch (light->unk78.flags & RE::TESObjectLIGH::kFlags_Type) {
      case RE::TESObjectLIGH::kFlag_TypeSpot:
      case RE::TESObjectLIGH::kFlag_TypeSpotShadow:
         return this->lightModelSpot;
      case RE::TESObjectLIGH::kFlag_TypeHemiShadow:
         return this->lightModelHemi;
      case RE::TESObjectLIGH::kFlag_TypeOmniShadow:
         return this->lightModelOmniShadow;
      case RE::TESObjectLIGH::kFlag_TypeOmni:
         return this->lightModelOmni;
   }
   return this->lightModelOmni;
};
void RevealService::Reload3DForReference(TESObjectREFR* subject, bool modifiedLights, bool modifiedMain) const {
   auto base = subject->baseForm;
   if (!base)
      return;
   if (modifiedLights && base->formType == kFormType_Light) {
      ((RE::TESObjectREFR*)subject)->Reload3D();
      return;
   }
   if (modifiedMain) {
      bool render = false;
      try {
         this->registrations.owned.at(base->formID);
         render = true;
      } catch (std::out_of_range) {
         try {
            this->registrations.foreign.at(base->formID);
            render = true;
         } catch (std::out_of_range) {}
      }
      if (render)
         ((RE::TESObjectREFR*)subject)->Reload3D();
   }
};
void RevealService::Reload3DForArea(bool modifiedLights, bool modifiedMain) const {
   TESObjectCELL* parentCell;
   bool           alsoDoLoadedGrid = false;
   {  // Identify the cell to search in, and lock its object list.
      parentCell = (*g_TES)->currentCell;
      if (parentCell == nullptr) {
         //
         // Didn't work? Get the current cell from the loadedAreaBound, then. (The SKSE team 
         // seem to have decoded that wrong; the cell property is always zero, while unk20 
         // seems to always be a pointer to the current cell.)
         //
         parentCell = reinterpret_cast<TESObjectCELL*>((*g_TES)->loadedAreaBound->unk20);
      }
      if (parentCell == nullptr)
         return;
      if ((parentCell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior) == 0)
         alsoDoLoadedGrid = true;
   }
   {  // Iterate over the cell's objects.
      CALL_MEMBER_FN((RE::TESObjectCELL*)parentCell, CellRefLockEnter)();
      UInt32 count = parentCell->objectList.count;
      for (UInt32 i = 0; i < count; i++) {
         TESObjectREFR* current = nullptr;
         parentCell->objectList.GetNthItem(i, current);
         if (!current)
            continue;
         this->Reload3DForReference(current, modifiedLights, modifiedMain);
      }
      CALL_MEMBER_FN((RE::TESObjectCELL*)parentCell, CellRefLockExit)();
   }
   if (alsoDoLoadedGrid) {
      UInt32 count = 0;
      TESObjectCELL** cells = ((RE::TES*)*g_TES)->CopyGridCells(&count);
      if (cells) {
         for (UInt32 i = 0; i < count; i++) {
            TESObjectCELL* currentCell = cells[i];
            if (currentCell && currentCell != parentCell) {
               CALL_MEMBER_FN((RE::TESObjectCELL*)currentCell, CellRefLockEnter)();
               UInt32 count = currentCell->objectList.count;
               for (UInt32 i = 0; i < count; i++) {
                  TESObjectREFR* current = nullptr;
                  currentCell->objectList.GetNthItem(i, current);
                  if (!current)
                     continue;
                  this->Reload3DForReference(current, modifiedLights, modifiedMain);
               }
               CALL_MEMBER_FN((RE::TESObjectCELL*)currentCell, CellRefLockExit)();
            }
         }
         free(cells);
      }
   }
};
//
void RevealService::DefineLightModels(TESForm* omni, TESForm* omniShadow, TESForm* hemi, TESForm* spot, TESForm* spotShadow) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   this->lightModelOmni.SetFromForm(omni);
   this->lightModelOmniShadow.SetFromForm(omniShadow);
   this->lightModelHemi.SetFromForm(hemi);
   this->lightModelSpot.SetFromForm(spot);
   if (spotShadow)
      this->lightModelSpot.SetFromForm(spotShadow);
   //
   this->thisDLLDoesLights = true;
   {  // Send message to CobbPos.
      auto m = RevealServiceMessageLightModelsSet();
      m.omni = omni;
      m.omniShadow = omniShadow;
      m.hemi = hemi;
      m.spot = spot;
      m.spotShadow = spotShadow;
      m.send();
      //
      if (m.is_acknowledged() && m.lightsActive) {
         //
         // One of the other DLLs had already revealed the lights. It has reverted 
         // its changes to their models, so now we need to reveal the lights on our 
         // end so that these new models are used.
         //
//_MESSAGE("RevealServiceMessageLightModelsSet replied to; another DLL was revealing lights; taking over");
         this->SetLightRevealState(true, true);
      }
   }
};
void RevealService::GetFormBounds(TESForm* baseForm, Bounds& outBounds) const {
   UInt32 formID = baseForm->formID;
   if (this->revealStateMain) {
      try {
         auto& reg = this->registrations.owned.at(formID);
         outBounds.min = reg.bounds.min;
         outBounds.max = reg.bounds.max;
         return;
      } catch (std::out_of_range) {
         try {
            auto& reg = this->registrations.foreign.at(formID);
            outBounds.min = reg.bounds.min;
            outBounds.max = reg.bounds.max;
            return;
         } catch (std::out_of_range) {}
      }
   }
   if (this->revealStateLights && baseForm->formType == kFormType_Light) {
      const auto light = (RE::TESObjectLIGH*) DYNAMIC_CAST(baseForm, TESForm, TESObjectLIGH);
      const auto model = this->GetLightModelForLight(light);
      if (!model.path.empty()) {
         outBounds.min = model.bounds.min;
         outBounds.max = model.bounds.max;
      }
   }
}
void RevealService::RegisterForm(const Registration& add, bool isBulk) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   auto form = add.form;
   if (!form || add.pathReveal.empty())
      return;
   UInt32 id = form->formID;
//_MESSAGE("Attempting to register form with ID %08X.", id);
   try {
      auto& r = this->registrations.owned.at(id);
      //
      // Update an existing registration.
      //
      r.bounds     = add.bounds;
      r.pathReveal = add.pathReveal;
      return;
   } catch (std::out_of_range) {}
   Registration toAdd = add; // needed because if (add) came from another DLL, we can't safely modify it in any capacity (STL doesn't account for different memory heaps)
   //
   // Get revert path:
   //
   try {
      auto& foreign = this->registrations.foreign.at(id);
      toAdd.pathRevert = foreign.pathRevert;
   } catch (std::out_of_range) {
      if (form->formType == kFormType_Light) {
         auto light = DYNAMIC_CAST(form, TESForm, TESObjectLIGH);
         if (!light)
            return;
         toAdd.pathRevert = light->texSwap.GetModelName();
         //
         if (this->revealStateLights) {
            auto model = this->GetLightModelForLight((RE::TESObjectLIGH*) light);
            if (model.path == toAdd.pathRevert)
               toAdd.pathRevert = "";
         }
      } else {
         TESModel* pWorldModel = DYNAMIC_CAST(form, TESForm, TESModel);
         if (pWorldModel)
            toAdd.pathRevert = pWorldModel->GetModelName();
      }
   }
   //
   this->registrations.owned[id] = toAdd;
   this->registrations.foreign.erase(id);
   //
   if (!isBulk) { // Send message to other DLLs.
      auto m = RevealServiceMessageFormsAdded();
      Transaction shim;
      shim.push_back(toAdd);
      m.transaction = &shim;
      m.send();
      if ((m.is_acknowledged() && m.wasBeingRevealed) || this->revealStateMain) {
         this->SetCustomMarkerModel(toAdd.form, toAdd.pathReveal, true);
         this->Reload3DForArea();
      }
   }
};
void RevealService::RegisterFormsInBulk(const Transaction& forms) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   for (auto it = forms.begin(); it != forms.end(); ++it) {
      auto& t = *it;
      this->RegisterForm(t, true);
   }
   {  // Send message to other DLLs.
      auto m = RevealServiceMessageFormsAdded();
      m.transaction = &forms;
      m.send();
      if ((m.is_acknowledged() && m.wasBeingRevealed) || this->revealStateMain) {
         for (auto it = forms.begin(); it != forms.end(); ++it) {
            auto& r = *it;
            this->SetCustomMarkerModel(r.form, r.pathReveal, true);
         }
         this->Reload3DForArea();
      }
   }
};
void RevealService::SetAllRevealStates(bool state, bool reload, bool local) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   bool reloadMain   = state != this->revealStateMain;
   bool reloadLights = state != this->revealStateLights;
   if (!reloadMain && !reloadLights) {
      //
      // force, just to be safe.
      //
      reloadMain = reloadLights = true;
   }
   //
   this->SetMainRevealState(state, false, false); // faster to set this first, to avoid map lookups
   this->SetLightRevealState(state, false);
   //
   if (local) {
      RevealServiceMessageMarkerRevealStateChanged message;
      message.state = this->revealStateMain;
      message.send();
   }
   if (reload)
      this->Reload3DForArea(reloadLights, reloadMain);
};
void RevealService::SetLightRevealState(bool state, bool reload) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   if (!this->thisDLLDoesLights) {
      RevealServiceMessageRequestLightStateChange m;
      m.state    = state;
      m.reload3D = reload;
      m.send();
      if (m.is_acknowledged())
         this->revealStateLights = state;
      return;
   }
   if (this->revealStateLights == state)
      return;
   //
   // When updating how this function works, be sure to reproduce any relevant 
   // changes in OnMessage.
   //
   DataHandler* dataHandler = DataHandler::GetSingleton();
   tArray<TESObjectLIGH*>& lights = dataHandler->lights;
   if (lights.count == 0 || lights.arr.entries == nullptr)
      return;
   for (UInt32 i = 0; i < lights.count; i++) {
      TESObjectLIGH* light = lights.arr.entries[i];
      if (!light)
         continue;
      if (this->revealStateMain) { // ensure we don't conflict with the "main" reveal functionality
         //
         // Skip this form if it's being handled as a custom marker, 
         // whether by this DLL or another DLL with a RevealService.
         //
         if (this->FormIsCustomMarker(light->formID))
            continue;
      }
      auto  model = this->GetLightModelForLight((RE::TESObjectLIGH*) light); // model we want to apply
      auto& swap  = light->texSwap;
      std::string currentModel = swap.GetModelName();
      if (state) {
         if (currentModel.empty())
            swap.SetModelName(model.path.c_str());
      } else {
         if (!currentModel.empty() && currentModel == model.path.c_str())
            swap.SetModelName("");
      }
   }
   this->revealStateLights = state;
   //
   if (reload)
      this->Reload3DForArea(true, false);
};
void RevealService::SetCustomMarkerModel(TESForm* form, const std::string& path, bool state) const {
   if (form->formType == kFormType_Light) {
      auto  light = DYNAMIC_CAST(form, TESForm, TESObjectLIGH);
      auto& swap = light->texSwap;
      if (!state && this->revealStateLights && path.empty()) {
         //
         // We are reverting main-state, but light-state is still active. Use the 
         // reveal-light model.
         //
         auto model = this->GetLightModelForLight((RE::TESObjectLIGH*) light);
         swap.SetModelName(model.path.c_str());
      } else {
         //
         // We are reverting main-state, and light-state is not active.
         //
         swap.SetModelName(path.c_str());
      }
   } else {
      TESModel* pWorldModel = DYNAMIC_CAST(form, TESForm, TESModel);
      if (pWorldModel)
         pWorldModel->SetModelName(path.c_str());
   }
};
void RevealService::SetMainRevealState(bool state, bool reload, bool local) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   if (this->revealStateMain == state)
      return;
   for (auto it = this->registrations.owned.begin(); it != this->registrations.owned.end(); ++it) {
      auto& r = it->second;
      this->SetCustomMarkerModel(r.form, state ? r.pathReveal : r.pathRevert, state);
   }
   this->revealStateMain = state;
   //
   if (local) {
      RevealServiceMessageMarkerRevealStateChanged message;
      message.state = this->revealStateMain;
      message.send();
      //
      if (reload)
         this->Reload3DForArea(false, true);
   }
};
//
void RevealService::MassReset() {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   this->SetAllRevealStates(false, false, false);
   this->registrations.owned.clear();
   this->registrations.foreign.clear();
   //
   // For now, we don't reset the light-models.
   //
   RevealServiceMessagePostMassReset().send();
};
//
void RevealService::OnMessage(const CobbAPIMessage* m) {
   svc_scoped_lock scopedLock(this->lock);  // scoped lock
   //
   const RevealServiceMessage* message = RevealServiceMessage::convert(m);
   if (!message)
      return;
   _MESSAGE("RevealService received message of type %08X.", message->subtype());
   switch (message->subtype()) {
      case RevealServiceMessage::kSubtype_FormsAdded:
         {  // Relinquish our control of the forms that the other DLL added
            auto casted = (RevealServiceMessageFormsAdded*) message;
            if (!casted->transaction)
               break;
            const auto& forms = *casted->transaction;
            for (auto it = forms.begin(); it != forms.end(); ++it) {
               auto& t = *it;
               auto  form = t.form;
               if (!form)
                  continue;
               auto id = form->formID;
               if (this->revealStateMain && !casted->wasBeingRevealed) {
                  //
                  // If another DLL takes ownership of the form while we're revealing it, 
                  // then we need to revert it to its original state and then signal to 
                  // the other DLL that it needs to alter the form.
                  //
                  auto it = this->registrations.owned.find(id);
                  if (it != this->registrations.owned.end()) {
                     auto& r = it->second;
                     this->SetCustomMarkerModel(r.form, r.pathRevert, false);
                     //
                     this->registrations.owned.erase(it);
                     casted->wasBeingRevealed = true;
                  }
               } else {
                  this->registrations.owned.erase(id);
               }
               this->registrations.foreign[id] = t;
            }
         }
         break;
      case RevealServiceMessage::kSubtype_RevealStateChanged:
         {
            auto casted = (RevealServiceMessageMarkerRevealStateChanged*) message;
            if (casted->state != this->revealStateMain)
               this->SetMainRevealState(casted->state, false, false);
         }
         break;
      case RevealServiceMessage::kSubtype_SetLightModels:
         {
            auto casted = (RevealServiceMessageLightModelsSet*)message;
            if (this->thisDLLDoesLights) {
               this->thisDLLDoesLights = false;
               if (this->revealStateLights) {
//_MESSAGE("Another DLL has assumed responsibility for lights while we're revealing them. Reverting our changes and signaling the other DLL...");
                  //
                  // Another DLL has taken responsibility for handling light changes, and 
                  // has changed which models will be used for lights. Because we had al-
                  // ready revealed lights, we need to forcibly revert our changes, and 
                  // then signal to that other DLL that it needs to apply its own changes.
                  //
                  // This code is modeled on SetLightRevealState(...).
                  //
                  DataHandler* dataHandler = DataHandler::GetSingleton();
                  tArray<TESObjectLIGH*>& lights = dataHandler->lights;
                  if (lights.count == 0 || lights.arr.entries == nullptr)
                     return;
                  for (UInt32 i = 0; i < lights.count; i++) {
                     TESObjectLIGH* light = lights.arr.entries[i];
                     if (!light)
                        continue;
                     if (this->revealStateMain && this->FormIsCustomMarker(light->formID)) // ensure we don't conflict with the "main" reveal functionality
                        continue;
                     auto  model = this->GetLightModelForLight((RE::TESObjectLIGH*) light); // model we want to apply
                     auto& swap = light->texSwap;
                     std::string currentModel = swap.GetModelName();
                     if (!currentModel.empty() && currentModel == model.path.c_str())
                        swap.SetModelName("");
                  }
                  //
                  casted->lightsActive = true;
               }
            }
            this->lightModelOmni.SetFromForm(casted->omni);
            this->lightModelOmniShadow.SetFromForm(casted->omniShadow);
            this->lightModelHemi.SetFromForm(casted->hemi);
            this->lightModelSpot.SetFromForm(casted->spot);
            if (casted->spotShadow)
               this->lightModelSpot.SetFromForm(casted->spotShadow);
         }
         break;
      case RevealServiceMessage::kSubtype_RequestLightStateChange:
         {
            auto casted = (RevealServiceMessageRequestLightStateChange*) message;
            if (this->thisDLLDoesLights)
               this->SetLightRevealState(casted->state, casted->reload3D);
            else
               this->revealStateLights = casted->state;
         }
         break;
      case RevealServiceMessage::kSubtype_RequestAddForms:
         {
            auto casted = (RevealServiceMessageRequestAddForms*) message;
            this->RegisterFormsInBulk(*casted->transaction);
            //
            // The RegisterFormsInBulk call will send synchronization messages; this is intentional. 
            // RevealServiceMessageRequestAddForms is only intended to be sent to a specific, known 
            // DLL, and that DLL has to synchronize with any other DLLs that have RevealServices. 
            // As a bonus, if a sender does things wrong and sends their request to all DLLs, then 
            // the synchronization messages will ensure that the last DLL to receive the request is 
            // the only one to be considered "owning" the forms (though the other DLLs will still 
            // end up doing wasted work).
         }
         break;
   };
};