#include "ExtendedEditorIDService.h"
//
#include "ReverseEngineered/Forms/TESForm.h"
//
#include "ReverseEngineered/Forms/BGSLightingTemplate.h"

namespace {
   typedef ExtendedEditorIDService::_EditorIDList _EditorIDList;
}
_EditorIDList* ExtendedEditorIDService::_GetListByFormType(UInt8 formType) {
   switch (formType) {
      case RE::BGSLightingTemplate::kTypeID:
         return &(this->editorIDs.lightingTemplates);
   }
   return nullptr;
}
//
ExtendedEditorIDService::ExtendedEditorIDService() {
   this->editorIDs.lightingTemplates.reserve(150); // vanilla has just over 130 lighting templates
}
//
void ExtendedEditorIDService::GetEditorID(const RE::TESForm* form, std::string& out) {
   out.clear();
   if (!form)
      return;
   auto list = this->_GetListByFormType(form->formType);
   if (!list)
      return;
   try {
      out = list->at(form->formID);
   } catch (std::out_of_range) {}
}
void ExtendedEditorIDService::StoreEditorID(const RE::TESForm* form, const char* editorID) {
   if (!form)
      return;
   auto list = this->_GetListByFormType(form->formType);
   if (!list)
      return;
   list->emplace(form->formID, editorID);
   list->at(form->formID).shrink_to_fit();
}
//
void ExtendedEditorIDService::DebugDump() const {
   _MESSAGE("Dumping ExtendedEditorIDService contents...");
   {
      auto& list = this->editorIDs.lightingTemplates;
      for (auto it = list.begin(); it != list.end(); ++it)
         _MESSAGE("[%s:%08X] %s", "LGTM", it->first, it->second.c_str());
   }
   _MESSAGE("Finished dumping ExtendedEditorIDService contents.");
}