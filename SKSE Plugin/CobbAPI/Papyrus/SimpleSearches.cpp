#pragma once
#include "Papyrus/SimpleSearches.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "Services/PapyrusFunctorStorage.h"
#include "skse/PapyrusVM.h"

#include "skse/GameData.h"
#include "skse/GameRTTI.h"
#include "skse/GameReferences.h"

#include "Shared.h" // for access to our object storage
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Systems/GameData.h"
#include "ReverseEngineered/Types.h"

namespace CobbPapyrus {
   namespace SimpleSearches {
      //
      // Helpers:
      //
      TESObjectCELL* _GetCurrentCell() {
         TESObjectCELL* cell = (*g_TES)->currentCell;
         if (cell == nullptr) {
            //
            // Get the current cell from the loadedAreaBound, then. (The SKSE team seem to have 
            // decoded that wrong; the cell property is always zero, while unk20 seems to always 
            // be a pointer to the current cell.)
            //
            cell = reinterpret_cast<TESObjectCELL*>((*g_TES)->loadedAreaBound->unk20);
            if (cell == nullptr)
               return nullptr;
         }
         return cell;
      }
      //
      // Functor methods:
      //
      bool GetAllOfBaseFormInCellFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         {  // Write search parameter: Cell
            UInt32 cellId = 0;
            if (this->cell)
               cellId = this->cell->formID;
            if (!WriteData(intfc, &cellId))
               return false;
         }
         {  // Write search parameter: Base Form
            UInt32 formId = 0;
            if (this->form)
               formId = this->form->formID;
            if (!WriteData(intfc, &formId))
               return false;
         }
         {  // Write search parameter: Search loaded area
            if (!WriteData(intfc, &this->searchLoadedArea))
               return false;
         }
         return true;
      };
      bool GetAllOfBaseFormInCellFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         {  // Read search parameter: cell
            UInt32 cellId;
            UInt32 cellIdFixed;
            if (!ReadData(intfc, &cellId))
               return false;
            if (!intfc->ResolveFormId(cellId, &cellIdFixed))
               return false;
            this->cell = (TESObjectCELL*)LookupFormByID(cellIdFixed);
         }
         {  // Read search parameter: base form
            UInt32 formId;
            UInt32 formIdFixed;
            if (!ReadData(intfc, &formId))
               return false;
            if (!intfc->ResolveFormId(formId, &formIdFixed))
               return false;
            this->form = (TESForm*)LookupFormByID(formIdFixed);
         }
         if (version >= 2) {
            if (!ReadData(intfc, &this->searchLoadedArea))
               return false;
         }
         return true;
      };
      bool GetAllOfBaseFormInCellFunctor::OnStart() {
         {  // Make sure we have a cell to search in.
            TESObjectCELL* cell = this->cell;
            if (this->cell == nullptr) {
               this->cell = _GetCurrentCell();
               if ((this->cell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior) == 0)
                  this->searchLoadedArea = true;
            }
            if (!this->cell) {
               //
               // Can't identify the cell to search in.
               //
               VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
               registry->LogWarning("A " PapyrusPrefixString("GetAllOfBaseFormInCell") " operation was unable to identify the cell to search in.", _stackId);
               return false;
            }
         }
         return true;
      };
      void GetAllOfBaseFormInCellFunctor::_SearchCell(TESObjectCELL* cell, VMResultArray<TESObjectREFR*>* out) {
         if (cell == nullptr)
            return;
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         CALL_MEMBER_FN((RE::TESObjectCELL*)cell, CellRefLockEnter)();
         UInt32 numRefs = cell->objectList.count;
         for (UInt32 i = 0; i < numRefs; i++) {
            TESObjectREFR* current = nullptr;
            cell->objectList.GetNthItem(i, current);
            if (!current)
               continue;
            if (current->baseForm == this->form) {
               out->push_back(current);
               if (out->size() > 127) {
                  registry->LogWarning("A " PapyrusPrefixString("GetAllOfBaseFormInCell") " operation found 128 results and will terminate early.", this->_stackId);
                  break;
               }
            }
         }
         CALL_MEMBER_FN((RE::TESObjectCELL*)cell, CellRefLockExit)();
      };
      void GetAllOfBaseFormInCellFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         VMResultArray<TESObjectREFR*> result;
         TESObjectCELL* searchCell = this->cell;
         if (searchCell == nullptr) {
            registry->LogWarning("A " PapyrusPrefixString("GetAllOfBaseFormInCell") " operation was unable to identify the cell to search in.", this->_stackId);
            return;
         }
         this->_SearchCell(searchCell, &result);
         if (this->searchLoadedArea && result.size() < 128) {
            UInt32 count = 0;
            TESObjectCELL** cells = ((RE::TES*)*g_TES)->CopyGridCells(&count);
            if (cells) {
               for (UInt32 i = 0; i < count; i++) {
                  TESObjectCELL* current = cells[i];
                  if (current && current != searchCell) {
                     this->_SearchCell(current, &result);
                     if (result.size() > 127)
                        break;
                  }
               }
               free(cells);
            }
         }
         PackValue(&resultValue, &result, registry);
      };
      void GetAllOfBaseFormInCellFunctor::OnEnd() {
      };
      //
      bool GetObjectWithLinkedRefInCellFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         {  // Write search parameter: Cell
            UInt32 cellId = 0;
            if (this->cell)
               cellId = this->cell->formID;
            if (!WriteData(intfc, &cellId))
               return false;
         }
         {  // Write search parameter: Reference
            UInt32 formId = 0;
            if (this->linked)
               formId = this->linked->formID;
            if (!WriteData(intfc, &formId))
               return false;
         }
         {  // Write search parameter: Keyword
            UInt32 formId = 0;
            if (this->linkedKeyword)
               formId = this->linkedKeyword->formID;
            if (!WriteData(intfc, &formId))
               return false;
         }
         {  // Write search parameter: Search loaded area
            if (!WriteData(intfc, &this->searchLoadedArea))
               return false;
         }
         return true;
      };
      bool GetObjectWithLinkedRefInCellFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         {  // Read search parameter: cell
            UInt32 cellId;
            UInt32 cellIdFixed;
            if (!ReadData(intfc, &cellId))
               return false;
            if (!intfc->ResolveFormId(cellId, &cellIdFixed))
               return false;
            this->cell = (TESObjectCELL*)LookupFormByID(cellIdFixed);
         }
         {  // Read search parameter: reference
            UInt32 formId;
            UInt32 formIdFixed;
            if (!ReadData(intfc, &formId))
               return false;
            if (!intfc->ResolveFormId(formId, &formIdFixed))
               return false;
            this->linked = DYNAMIC_CAST(LookupFormByID(formIdFixed), TESForm, TESObjectREFR);
         }
         {  // Read search parameter: keyword
            UInt32 formId;
            UInt32 formIdFixed;
            if (!ReadData(intfc, &formId))
               return false;
            if (!intfc->ResolveFormId(formId, &formIdFixed))
               return false;
            this->linkedKeyword = DYNAMIC_CAST(LookupFormByID(formIdFixed), TESForm, BGSKeyword);
         }
         if (version >= 2) {
            if (!ReadData(intfc, &this->searchLoadedArea))
               return false;
         }
         return true;
      };
      bool GetObjectWithLinkedRefInCellFunctor::OnStart() {
         {  // Make sure we have a cell to search in.
            TESObjectCELL* cell = this->cell;
            if (this->cell == nullptr) {
               this->cell = _GetCurrentCell();
               if ((this->cell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior) == 0)
                  this->searchLoadedArea = true;
            }
            if (!this->cell) {
               //
               // Can't identify the cell to search in.
               //
               VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
               registry->LogWarning("A " PapyrusPrefixString("GetAllWithLinkedRefInCell") " operation was unable to identify the cell to search in.", _stackId);
               return false;
            }
         }
         return true;
      };
      void GetObjectWithLinkedRefInCellFunctor::_SearchCell(TESObjectCELL* cell, VMResultArray<TESObjectREFR*>* out) {
         if (cell == nullptr)
            return;
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         CALL_MEMBER_FN((RE::TESObjectCELL*)cell, CellRefLockEnter)();
         UInt32 numRefs = cell->objectList.count;
         for (UInt32 i = 0; i < numRefs; i++) {
            TESObjectREFR* current = nullptr;
            cell->objectList.GetNthItem(i, current);
            if (!current)
               continue;
            TESObjectREFR* linked = (TESObjectREFR*)CALL_MEMBER_FN((RE::TESObjectREFR*)current, GetLinkedRef)(this->linkedKeyword);
            if (linked->formID == this->linked->formID) {
               out->push_back(current);
               if (out->size() > 127) {
                  registry->LogWarning("A " PapyrusPrefixString("GetAllWithLinkedRefInCell") " operation found 128 results and will terminate early.", _stackId);
                  break;
               }
            }
         }
         CALL_MEMBER_FN((RE::TESObjectCELL*)cell, CellRefLockExit)();
      };
      void GetObjectWithLinkedRefInCellFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         VMResultArray<TESObjectREFR*> result;
         TESObjectCELL* searchCell = this->cell;
         if (searchCell == nullptr) {
            registry->LogWarning("A " PapyrusPrefixString("GetAllWithLinkedRefInCell") " operation was unable to identify the cell to search in.", _stackId);
            return;
         }
         this->_SearchCell(searchCell, &result);
         if (this->searchLoadedArea && result.size() < 128) {
            UInt32 count = 0;
            TESObjectCELL** cells = ((RE::TES*)*g_TES)->CopyGridCells(&count);
            if (cells) {
               for (UInt32 i = 0; i < count; i++) {
                  TESObjectCELL* current = cells[i];
                  if (current && current != searchCell) {
                     this->_SearchCell(current, &result);
                     if (result.size() > 127)
                        break;
                  }
               }
               free(cells);
            }
         }
         PackValue(&resultValue, &result, registry);
      };
      void GetObjectWithLinkedRefInCellFunctor::OnEnd() {
      };
      //
      // Papyrus APIs:
      //
      bool GetAllOfBaseFormInCell(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         TESForm* baseForm,
         TESObjectCELL* cell,
         bool searchLoadedArea = false
      ) {
         ERROR_AND_RETURN_0_IF(baseForm == nullptr, "You must specify a base form for GetAllOfBaseFormInCell. Returning an array of [None].", registry, stackId);
         GetAllOfBaseFormInCellFunctor* func = new GetAllOfBaseFormInCellFunctor(stackId);
         func->form = baseForm;
         func->cell = cell;
         func->searchLoadedArea = searchLoadedArea;
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("GetAllOfBaseFormInCell") " operation; too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
      bool GetAllWithLinkedRefInCell(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         TESObjectREFR* target,
         BGSKeyword* targetKeyword,
         TESObjectCELL* cell,
         bool searchLoadedArea = false
      ) {
         ERROR_AND_RETURN_0_IF(target == nullptr, "You must specify a linked reference for GetAllWithLinkedRefInCell. Returning an array of [None].", registry, stackId);
         GetObjectWithLinkedRefInCellFunctor* func = new GetObjectWithLinkedRefInCellFunctor(stackId);
         func->linked = target;
         func->linkedKeyword = targetKeyword;
         func->cell = cell;
         func->searchLoadedArea = searchLoadedArea;
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("GetAllWithLinkedRefInCell") " operation; too many batch/compound operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
   }
};

bool CobbPapyrus::SimpleSearches::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<GetAllOfBaseFormInCellFunctor>();
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction3<StaticFunctionTag, VMResultArray<TESObjectREFR*>, TESForm*, TESObjectCELL*, bool>(
         "GetAllOfBaseFormInCell",
         PapyrusPrefixString("SimpleSearches"),
         SimpleSearches::GetAllOfBaseFormInCell,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction4<StaticFunctionTag, VMResultArray<TESObjectREFR*>, TESObjectREFR*, BGSKeyword*, TESObjectCELL*, bool>(
         "GetAllWithLinkedRefInCell",
         PapyrusPrefixString("SimpleSearches"),
         SimpleSearches::GetAllWithLinkedRefInCell,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};