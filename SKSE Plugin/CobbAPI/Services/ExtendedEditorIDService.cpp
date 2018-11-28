#include "ExtendedEditorIDService.h"
//
#include "ReverseEngineered/Forms/TESForm.h"
//
#include "ReverseEngineered/Forms/BGSAcousticSpace.h"
#include "ReverseEngineered/Forms/BGSLightingTemplate.h"
#include "ReverseEngineered/Forms/TESImageSpace.h"

namespace {
   typedef ExtendedEditorIDService::_EditorIDList _EditorIDList;
}
_EditorIDList* ExtendedEditorIDService::_GetListByFormType(UInt8 formType) {
   switch (formType) {
      case RE::BGSAcousticSpace::kTypeID:
         return &(this->editorIDs.acousticSpaces);
      case RE::TESImageSpace::kTypeID:
         return &(this->editorIDs.imageSpaces);
      case RE::BGSLightingTemplate::kTypeID:
         return &(this->editorIDs.lightingTemplates);
   }
   return nullptr;
}
//
ExtendedEditorIDService::ExtendedEditorIDService() {
   this->editorIDs.acousticSpaces.reserve(75);     // vanilla has  52 acoustic spaces
   this->editorIDs.imageSpaces.reserve(350);       // vanilla has 288 image spaces
   this->editorIDs.lightingTemplates.reserve(200); // vanilla has 130 lighting templates
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
const std::string& ExtendedEditorIDService::GetEditorID(const RE::TESForm* form) {
   if (!form)
      throw std::invalid_argument("Cannot get the editor ID of a nullptr form.");
   auto list = this->_GetListByFormType(form->formType);
   if (!list)
      throw std::out_of_range("We do not preserve editor IDs for this form type.");
   return list->at(form->formID);
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
      auto& list = this->editorIDs.acousticSpaces;
      for (auto it = list.begin(); it != list.end(); ++it)
         _MESSAGE("[%s:%08X] %s", "ASPC", it->first, it->second.c_str());
   }
   {
      auto& list = this->editorIDs.imageSpaces;
      for (auto it = list.begin(); it != list.end(); ++it)
         _MESSAGE("[%s:%08X] %s", "IMGS", it->first, it->second.c_str());
   }
   {
      auto& list = this->editorIDs.lightingTemplates;
      for (auto it = list.begin(); it != list.end(); ++it)
         _MESSAGE("[%s:%08X] %s", "LGTM", it->first, it->second.c_str());
   }
   _MESSAGE("Finished dumping ExtendedEditorIDService contents.");
}