#include "Services/DisplayOptionService.h"

#include <ctime>
using namespace std;

#include "ReverseEngineered/Forms/TESEffectShader.h"
#include "ReverseEngineered/Forms/TESGlobal.h"
#include "skse/GameRTTI.h"

const DisplayOptionService::Date DisplayOptionService::GetDate() const {
   time_t t = time(0); // get time now
   struct tm now;
   localtime_s(&now, &t);
   //
   // now.tm_mon  == zero-indexed month (i.e. add one to get the "normal" month)
   // now.tm_mday == date of the month
   //
   if ((now.tm_mon + 1 == 10) && (now.tm_mday == 30 || now.tm_mday == 31)) {
      _MESSAGE("DisplayOptionService thinks it's Halloween or the day prior.");
      return kDate_Halloween;
   } else if ((now.tm_mon + 1 == 11) && (now.tm_mday <= 20)) {
      _MESSAGE("DisplayOptionService thinks it's T.A. week(s).");
      return kDate_TransAwareness;
   } else if ((now.tm_mon + 1 == 3) && (now.tm_mday == 31)) {
      _MESSAGE("DisplayOptionService thinks it's TDOV.");
      return kDate_TransAwareness;
   }
   _MESSAGE("DisplayOptionService thinks it's no special occasion.");
   return kDate_None;
};

void DisplayOptionService::SetGizmoForms(TESObjectACTI* normal, TESObjectACTI* large) {
   this->forms.gizmos.normal = normal;
   this->forms.gizmos.large = large;
};
void DisplayOptionService::SetShaderForms(TESEffectShader* selection, TESEffectShader* snap) {
   this->forms.shaders.selection = selection;
   this->forms.shaders.snap = snap;
};
void DisplayOptionService::SetGizmoGlobal(TESGlobal* form) {
   this->forms.globals.gizmoOption = form;
};
void DisplayOptionService::SetShaderGlobal(TESGlobal* form) {
   this->forms.globals.shaderOption = form;
};

void DisplayOptionService::_CacheShaderState(TESEffectShader* shader, OriginalShaderState* state) {
   if (shader == NULL || state == NULL)
      return;
   if (state->cached)
      return;
   state->cached = true;
   state->edgeColor     = ((RE::TESEffectShader*)shader)->data.edgeColor;
   state->fillColorKey1 = ((RE::TESEffectShader*)shader)->data.fillColorKey1;
   state->fillColorKey2 = ((RE::TESEffectShader*)shader)->data.fillColorKey2;
   state->fillColorKey3 = ((RE::TESEffectShader*)shader)->data.fillColorKey3;
};
void DisplayOptionService::_RestoreShaderState(TESEffectShader* shader, OriginalShaderState* state) const {
   if (shader == NULL || state == NULL)
      return;
   if (!state->cached)
      return;
   ((RE::TESEffectShader*)shader)->data.edgeColor     = state->edgeColor;
   ((RE::TESEffectShader*)shader)->data.fillColorKey1 = state->fillColorKey1;
   ((RE::TESEffectShader*)shader)->data.fillColorKey2 = state->fillColorKey2;
   ((RE::TESEffectShader*)shader)->data.fillColorKey3 = state->fillColorKey3;
};

bool DisplayOptionService::GetModel(TESForm* form, std::string* out) const {
   if (form == NULL)
      return false;
   TESModel* model = DYNAMIC_CAST(form, TESForm, TESModel);
   if (model) {
      out->assign(model->GetModelName());
      return true;
   }
   return false;
};
bool DisplayOptionService::SetModel(TESForm* form, const std::string path) const {
   if (form == NULL)
      return false;
   TESModel* model = DYNAMIC_CAST(form, TESForm, TESModel);
   if (!path.empty() && model) {
      model->SetModelName(path.c_str());
      return true;
   }
   return false;
};

void DisplayOptionService::UpdateForms() {
   this->lock.Lock();

   _MESSAGE("DisplayOptionService is about to apply your display settings (UpdateForms).");

   {  // Diagnostic logs.
      if (this->forms.globals.gizmoOption == NULL)
         _MESSAGE("DisplayOptionSevice: Can't find option global: %s.", "gizmos");
      if (this->forms.globals.shaderOption == NULL)
         _MESSAGE("DisplayOptionSevice: Can't find option global: %s.", "shaders");

      if (this->forms.gizmos.normal == NULL)
         _MESSAGE("DisplayOptionSevice: Can't find edit gizmo: %s.", "normal");
      if (this->forms.gizmos.large == NULL)
         _MESSAGE("DisplayOptionSevice: Can't find edit gizmo: %s.", "large");

      if (this->forms.shaders.selection == NULL)
         _MESSAGE("DisplayOptionSevice: Can't find edit shader: %s.", "selection");
      if (this->forms.shaders.snap == NULL)
         _MESSAGE("DisplayOptionSevice: Can't find edit shader: %s.", "snap");
   }

   Date date = this->GetDate();
   if (this->forms.gizmos.normal || this->forms.gizmos.large) { // Update gizmo.
      UInt32 option = kGizmoOption_StandardOnly;
      if (this->forms.globals.gizmoOption)
         option = (UInt32) ((RE::TESGlobal*)this->forms.globals.gizmoOption)->value;
      switch (option) {
         case kGizmoOption_ColorblindProtanopia:
            this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalColorblindProtanopia.nif");
            this->SetModel(this->forms.gizmos.large,  "Cobb Positioner/edit-gizmos/LargeColorblindProtanopia.nif");
            break;
         case kGizmoOption_ColorblindTritanopia:
            this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalColorblindTritanopia.nif");
            this->SetModel(this->forms.gizmos.large,  "Cobb Positioner/edit-gizmos/LargeColorblindTritanopia.nif");
            break;
         case kGizmoOption_Occasions:
            switch (date) {
               case kDate_Halloween:
                  this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalThemeHalloween.nif");
                  this->SetModel(this->forms.gizmos.large,  "Cobb Positioner/edit-gizmos/LargeThemeHalloween.nif");
                  break;
               case kDate_TransAwareness:
                  this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalThemeTA.nif");
                  this->SetModel(this->forms.gizmos.large,  "Cobb Positioner/edit-gizmos/LargeThemeTA.nif");
                  break;
               case kDate_None:
               default:
                  this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalStandard.nif");
                  this->SetModel(this->forms.gizmos.large,  "Cobb Positioner/edit-gizmos/LargeStandard.nif");
                  break;
            };
            break;
         case kGizmoOption_OutlinedLabeled:
            switch (date) {
               case kDate_TransAwareness:
                  this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalLabeledThemeTA.nif");
                  this->SetModel(this->forms.gizmos.large, "Cobb Positioner/edit-gizmos/LargeLabeledThemeTA.nif");
                  break;
               case kDate_None:
               default:
                  this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalLabeledColor.nif");
                  this->SetModel(this->forms.gizmos.large, "Cobb Positioner/edit-gizmos/LargeLabeledColor.nif");
                  break;
            }
            break;
         case kGizmoOption_OutlinedLabeledWhite:
            this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalLabeledWhite.nif");
            this->SetModel(this->forms.gizmos.large, "Cobb Positioner/edit-gizmos/LargeLabeledWhite.nif");
            break;
         case kGizmoOption_StandardOnly:
         default:
            this->SetModel(this->forms.gizmos.normal, "Cobb Positioner/edit-gizmos/NormalStandard.nif");
            this->SetModel(this->forms.gizmos.large,  "Cobb Positioner/edit-gizmos/LargeStandard.nif");
            break;
      };
   }
   if (this->forms.shaders.selection || this->forms.shaders.snap) { // Update shaders.
      //
      // First, let's preserve the original shader colors, if we haven't touched them yet.
      //
      this->_CacheShaderState(this->forms.shaders.selection, &this->originalFormStates.shaders.selection);
      this->_CacheShaderState(this->forms.shaders.snap,      &this->originalFormStates.shaders.snap);
      //
      // Now let's make our changes.
      //
      UInt32 option = kShaderOption_StandardOnly;
      if (this->forms.globals.shaderOption)
         option = (UInt32)((RE::TESGlobal*)this->forms.globals.shaderOption)->value;
      switch (option) {
         case kShaderOption_ColorblindTritanopia:
            //
            // - Default the selection shader; it shows fine for tritanopia.
            // - Make the snap shader compliment selection; green/orange -> teal/salmon
            //
            this->_RestoreShaderState(this->forms.shaders.selection, &this->originalFormStates.shaders.selection);
            if (RE::TESEffectShader* snap = (RE::TESEffectShader*)this->forms.shaders.snap) {
               snap->data.fillColorKey1 = 0x002389FF; // rgb(255, 189, 35)
               snap->data.fillColorKey2 = 0x002389FF; // rgb(255, 189, 35)
               snap->data.fillColorKey3 = 0x002389FF; // rgb(255, 189, 35)
               snap->data.edgeColor     = 0x002389FF; // rgb(255, 189, 35)
            }
            break;
         case kShaderOption_StandardOnly:
         default:
            //
            // Set shader colors to default, if we have changed them from the default and cached that change.
            //
            this->_RestoreShaderState(this->forms.shaders.selection, &this->originalFormStates.shaders.selection);
            this->_RestoreShaderState(this->forms.shaders.snap,      &this->originalFormStates.shaders.snap);
            break;
      };
   }

   _MESSAGE("DisplayOptionService has applied your display settings (UpdateForms).");

   this->lock.Release();
};