#pragma once
#include <string>
#include <unordered_map>

namespace RE {
   class TESForm;
};
class ExtendedEditorIDService {
   public:
      typedef UInt32 FormID;
      typedef std::unordered_map<FormID, std::string> _EditorIDList;
   protected:
      //
      // We sort loaded editor IDs by form type, to make lookups somewhat 
      // faster.
      //
      struct {
         //
         // REMINDER: When you add more form types to this service, be sure 
         // to also edit the Papyrus script file to list them.
         //
         _EditorIDList lightingTemplates; // BGSLightingTemplate
      } editorIDs;
      //
      _EditorIDList* _GetListByFormType(UInt8 formType);
      //
   public:
      static ExtendedEditorIDService& GetInstance() {
         static ExtendedEditorIDService instance;
         return instance;
      };
      ExtendedEditorIDService();
      void GetEditorID(const RE::TESForm*, std::string& out);
      void StoreEditorID(const RE::TESForm*, const char*);
      //
      void DebugDump() const;
};