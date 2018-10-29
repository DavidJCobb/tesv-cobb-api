#pragma once
#include "Papyrus/CompoundOperations/CellSearcher.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameData.h"
#include "skse/GameRTTI.h"
#include "skse/GameReferences.h"
#include "skse/NiNodes.h"

#include "Shared.h" // for access to our object storage
#include "Types/bounds.h"
#include "Services/RevealService.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/NetImmerse/NiExtraData.h"
#include "ReverseEngineered/Systems/GameData.h"

#define ERROR_AND_RETURN_IF_WRONG_STACK(verb) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("CellSearcher") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return; \
   }
#define ERROR_AND_RETURN_VALUE_IF_WRONG_STACK(verb, retval) \
   if (stackId != func->StackId()) { \
      registry->LogError("A " PapyrusPrefixString("CellSearcher") " operation can only be " verb " by the Papyrus call stack that created it in the first place.", stackId); \
      return retval; \
   }

//#define COBBDEBUGSEARCHER 1

#define DEBUG_ONLY_CELLSEARCHER_MESSAGE(...) 
#define DEBUG_ONLY_CELLSEARCHER_WARN(condition, ...) 
#ifdef COBBDEBUGSEARCHER
#undef DEBUG_ONLY_CELLSEARCHER_MESSAGE
#undef DEBUG_ONLY_CELLSEARCHER_WARN
#define DEBUG_ONLY_CELLSEARCHER_MESSAGE(...) _MESSAGE(__VA_ARGS__);
#define DEBUG_ONLY_CELLSEARCHER_WARN(condition, ...) if ((condition)) _MESSAGE(__VA_ARGS__);
#endif

namespace PapyrusPrefix(Papyrus) {
   namespace CellSearcher {
      //
      // Functor methods:
      //
      bool CellSearchFunctor::OnSave(SKSESerializationInterface* intfc) {
         using namespace Serialization;
         //
         {  // Write search parameter: Cell
            UInt32 cellId = 0;
            if (this->searchParameters.cell)
               cellId = this->searchParameters.cell->formID;
            SERIALIZATION_ASSERT(WriteData(intfc, &cellId), "Failed to write search parameter: cell form ID.");
         }
         {  // Write general search parameters
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.coordinates),      "Failed to write search parameter: coordinates.");
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.tolerance),        "Failed to write search parameter: bounds tolerance.");
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.filterMode),       "Failed to write search parameter: filter mode.");
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.excludePlayer),    "Failed to write search parameter: exclude player.");
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.requireCollision), "Failed to write search parameter: require collision.");
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.searchLoadedArea), "Failed to write search parameter: search loaded area.");
            SERIALIZATION_ASSERT(WriteData(intfc, &this->searchParameters.types),            "Failed to write search parameter: types.");
         }
         {  // Write search parameter: form lists
            UInt32 count = this->searchParameters.formLists.size();
            if (!WriteData(intfc, &count))
               return false;
            for (UInt32 i = 0; i < count; i++) {
               UInt32 formId = 0;
               BGSListForm* current = this->searchParameters.formLists[i];
               if (current)
                  formId = current->formID;
               SERIALIZATION_ASSERT(WriteData(intfc, &formId), "Failed to write search parameter: form ID of form list %d.", i);
            }
         }
         //
         return true;
      };
      bool CellSearchFunctor::OnLoad(SKSESerializationInterface* intfc, UInt32 version) {
         using namespace Serialization;
         {  // Read search parameter: cell
            UInt32 cellId;
            UInt32 cellIdFixed;
            if (!ReadData(intfc, &cellId))
               return false;
            if (!intfc->ResolveFormId(cellId, &cellIdFixed))
               return false;
            this->searchParameters.cell = static_cast<TESObjectCELL*>(LookupFormByID(cellIdFixed));
         }
         {  // Read general search parameters
            SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.coordinates),      "Failed to read search parameter: coordinates.");
            SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.tolerance),        "Failed to read search parameter: bounds tolerance.");
            SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.filterMode),       "Failed to read search parameter: filter mode.");
            SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.excludePlayer),    "Failed to read search parameter: exclude player.");
            SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.requireCollision), "Failed to read search parameter: require collision.");
            if (version >= 2) {
               SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.searchLoadedArea), "Failed to read search parameter: search loaded area.");
            }
            SERIALIZATION_ASSERT(ReadData(intfc, &this->searchParameters.types),            "Failed to read search parameter: types.");
         }
         {  // Read search parameter: form lists
            UInt32 count;
            if (!ReadData(intfc, &count))
               return false;
            this->searchParameters.formLists.reserve(count);
            for (UInt32 i = 0; i < count; i++) {
               UInt32 formId;
               SERIALIZATION_ASSERT(ReadData(intfc, &formId), "Failed to read search parameter: form ID of form list %d.", i);
               UInt32 fixedFormId;
               if (!intfc->ResolveFormId(formId, &fixedFormId)) {
                  _MESSAGE(__FUNCTION__ ": FormID of FormList %d could not be resolved; skipping.", i);
                  continue;
               }
               this->searchParameters.formLists.push_back(static_cast<BGSListForm*>(LookupFormByID(fixedFormId)));
            }
         }
         //
         return true;
      };
      bool CellSearchFunctor::OnStart() {
         {  // Make sure we have a cell to search in, and if so, lock the cell's object list.
            TESObjectCELL* searchCell = this->searchParameters.cell;
            if (searchCell == nullptr) {
               //
               // No cell was specified. Try to get the current cell.
               //
               searchCell = (*g_TES)->currentCell;
               if (searchCell == NULL) {
                  //
                  // Didn't work? Get the current cell from the loadedAreaBound, then. (The SKSE team 
                  // seem to have decoded that wrong; the cell property is always zero, while unk20 
                  // seems to always be a pointer to the current cell.)
                  //
                  searchCell = reinterpret_cast<TESObjectCELL*>((*g_TES)->loadedAreaBound->unk20);
               }
               if (searchCell) {
                  this->searchParameters.cell = searchCell;
                  if ((searchCell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior) == 0) {
                     this->searchParameters.searchLoadedArea = true;
                  }
               }
            }
         }
         return true;
      };
      bool CellSearchFunctor::SearchCell(TESObjectCELL* cell, TESObjectREFR** match, float* volume) {
         if (cell == nullptr)
            return true;
         if (match == nullptr || volume == nullptr)
            return false;
         CALL_MEMBER_FN((RE::TESObjectCELL*)cell, CellRefLockEnter)();
         UInt32 count = cell->objectList.count;
         for (UInt32 i = 0; i < count; i++) {
            TESObjectREFR* current = nullptr;
            cell->objectList.GetNthItem(i, current);
            if (!current)
               continue;
            #ifdef COBBDEBUGSEARCHER
            bool debugIsConsoleRef = false;
            {
               UInt32 handle = (*g_consoleHandle);
               TESObjectREFR* refr = NULL;
               if (handle != 0 && handle != (*g_invalidRefHandle)) {
                  LookupREFRByHandle(&handle, &refr);
                  if (refr == current)
                     debugIsConsoleRef = true;
                  skyrim_re_clear_refr(refr);
               }
               if (debugIsConsoleRef)
                  _MESSAGE(PapyrusPrefixString("CellSearcher") "::SearchCell: Reference ID 0x%08X is the current console ref; we will echo detailed information for it.", current->formID);
            }
            #endif
            if (this->searchParameters.filterMode == CellSearchFunctor::SearchParameters::FilterMode::kFilterMode_Candidates) {
               if (!TypeTest(current)) {
                  #ifdef COBBDEBUGSEARCHER
                  if (debugIsConsoleRef)
                     _MESSAGE(PapyrusPrefixString("CellSearcher") "::Run: Ruling out reference ID 0x%08X; it is not of a type we're looking for.", current->formID);
                  #endif
                  continue;
               }
            }
            if (!IsSelectable(current))
               continue;
            TESBoundObject* baseForm = DYNAMIC_CAST(current->baseForm, TESForm, TESBoundObject);
            DEBUG_ONLY_CELLSEARCHER_WARN(!baseForm, PapyrusPrefixString("CellSearcher") "::SearchCell: ERROR: UNABLE TO GET TESBoundObject* FOR REFERENCE ID 0x%08X. CANNOT CHECK ITS BOUNDS FOR THE SEARCH ALGO.", current->formID);
            if (baseForm) {
               Bounds currentBounds;
               currentBounds.min.x = (float)(SInt16)baseForm->bounds.x;
               currentBounds.min.y = (float)(SInt16)baseForm->bounds.y;
               currentBounds.min.z = (float)(SInt16)baseForm->bounds.z;
               currentBounds.max.x = (float)(SInt16)baseForm->bounds2.x;
               currentBounds.max.y = (float)(SInt16)baseForm->bounds2.y;
               currentBounds.max.z = (float)(SInt16)baseForm->bounds2.z;
               {
                  RevealService& service = RevealService::GetInstance();
                  service.GetFormBounds(baseForm, currentBounds);
               }
               if (!currentBounds) {
                  #ifdef COBBDEBUGSEARCHER
                  if (debugIsConsoleRef)
                     _MESSAGE(PapyrusPrefixString("CellSearcher") "::SearchCell: Reference ID 0x%08X has a zero-size bounding box {(%f, %f, %f) to (%f, %f, %f)}. Skipping...", current->formID, currentBounds.min.x, currentBounds.min.y, currentBounds.min.z, currentBounds.max.x, currentBounds.max.y, currentBounds.max.z);
                  #endif
                  continue;
               }
               //
               // Apply the reference's scale.
               //
               //_MESSAGE(PapyrusPrefixString("CellSearcher") "::Run: Reference ID 0x%08X has scale %f.", current->formID, CALL_MEMBER_FN(current, GetBaseScale)());
               currentBounds *= CALL_MEMBER_FN(current, GetBaseScale)(); // is this correct?
               //
               // Bounds are defined. Apply tolerance.
               //
               currentBounds += this->searchParameters.tolerance;
               DEBUG_ONLY_CELLSEARCHER_WARN(debugIsConsoleRef, PapyrusPrefixString("CellSearcher") "::SearchCell: Reference ID 0x%08X bounds have volume %f...", current->formID, currentBounds.volume());
               DEBUG_ONLY_CELLSEARCHER_WARN(debugIsConsoleRef, PapyrusPrefixString("CellSearcher") "::SearchCell: Reference ID 0x%08X bounds are: {(%f, %f, %f) to (%f, %f, %f)}.", current->formID, currentBounds.min.x, currentBounds.min.y, currentBounds.min.z, currentBounds.max.x, currentBounds.max.y, currentBounds.max.z);
               if (currentBounds.volume() < *volume) {
                  //
                  // If the volume isn't smaller than our smallestVolume, then there's 
                  // no point in testing this one. If it IS smaller, then we need to 
                  // copy the object's rotation into our bounds object and then run 
                  // a separating axis test against the search point.
                  //
                  currentBounds.rotation = Cobb::Euler(current->rot.x, current->rot.y, current->rot.z, true);
                  DEBUG_ONLY_CELLSEARCHER_WARN(debugIsConsoleRef, PapyrusPrefixString("CellSearcher") "::SearchCell: Reference ID 0x%08X is about to be SAT'd. Its position is (%f, %f, %f).", current->formID, current->pos.x, current->pos.y, current->pos.z);
                  if (currentBounds.SeparatingAxisTest(current->pos, this->searchParameters.coordinates)) {
                     *match = current;
                     *volume = currentBounds.volume();
                  }
               }
            }
         }
         DEBUG_ONLY_CELLSEARCHER_MESSAGE("Checked %d references in cell %08X.", count, cell->formID);
         CALL_MEMBER_FN((RE::TESObjectCELL*)cell, CellRefLockExit)();
         return true;
      };
      void CellSearchFunctor::Run(VMValue& resultValue) {
         VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
         TESObjectCELL* searchCell = this->searchParameters.cell;
         if (searchCell == nullptr) {
            registry->LogWarning("A " PapyrusPrefixString("CellSearcher") " operation was unable to identify the cell to search in. It will return None.", _stackId);
            return;
         }
         DEBUG_ONLY_CELLSEARCHER_MESSAGE("===========================================================================\n  Running searcher task at point: (%f, %f, %f)\n===========================================================================", this->searchParameters.coordinates.x, this->searchParameters.coordinates.y, this->searchParameters.coordinates.z);
         //
         TESObjectREFR* smallestMatch = nullptr;
         float smallestVolume = 10000000000;
         //
         this->SearchCell(searchCell, &smallestMatch, &smallestVolume);
         if (this->searchParameters.searchLoadedArea && (searchCell->unk2C & RE::TESObjectCELL::kCellFlag_IsInterior) == 0) {
            DEBUG_ONLY_CELLSEARCHER_MESSAGE(PapyrusPrefixString("CellSearcher") "::Run: We will try to get and search the loaded grid.");
            UInt32 count = 0;
            TESObjectCELL** cells = ((RE::TES*)*g_TES)->CopyGridCells(&count);
            if (cells) {
               DEBUG_ONLY_CELLSEARCHER_MESSAGE(PapyrusPrefixString("CellSearcher") "::Run: We will search %d cells in the loaded area.", count);
               for (UInt32 i = 0; i < count; i++) {
                  TESObjectCELL* current = cells[i];
                  if (current && current != searchCell) {
                     DEBUG_ONLY_CELLSEARCHER_MESSAGE(PapyrusPrefixString("CellSearcher") "::Run: Searching cell %d (FORM:%08X, RAM:%08X).", i, current->formID, current);
                     this->SearchCell(current, &smallestMatch, &smallestVolume);
                  }
               }
               free(cells);
            }
         }
         //
         // Act on what we've found.
         //
         if (!smallestMatch) {
            PackValue(&resultValue, &smallestMatch, registry);
            return;
         }
         if (this->searchParameters.filterMode == CellSearchFunctor::SearchParameters::FilterMode::kFilterMode_Result) {
            if (!TypeTest(smallestMatch)) {
               DEBUG_ONLY_CELLSEARCHER_MESSAGE(PapyrusPrefixString("CellSearcher") "::Run: Reference ID 0x%08X IS OUR RESULT, but it doesn't match the filter; we're returning None.", smallestMatch->formID);
               return;
            }
         }
         PackValue(&resultValue, &smallestMatch, registry);
      };
      void CellSearchFunctor::OnEnd() {
      };
      bool CellSearchFunctor::IsSelectable(TESObjectREFR* reference) {
         {  // Reject the reference if it is disabled.
            if ((reference->unk50 & 0x10000) != 0 || (reference->flags & 0x800)) {
               //_MESSAGE(PapyrusPrefixString("CellSearchFunctor") "::IsSelectable: Ruling out reference ID 0x%08X; it is disabled.", reference->formID);
               return false;
            }
         }
         {  // Targets must have 3D.
            if (!reference->GetNiNode()) {
               //_MESSAGE(PapyrusPrefixString("CellSearchFunctor") "::IsSelectable: Ruling out reference ID 0x%08X; it has no 3D.", reference->formID);
               return false;
            }
         }
         if (this->searchParameters.excludePlayer) {
            //
            // Do not allow selection of the player.
            //
            PlayerCharacter* player = DYNAMIC_CAST(reference, TESObjectREFR, PlayerCharacter);
            if (player) {
               //_MESSAGE(PapyrusPrefixString("CellSearchFunctor") "::IsSelectable: Ruling out reference ID 0x%08X; it is the player.", reference->formID);
               return false;
            }
         }
         if (this->searchParameters.requireCollision) {
            {  // Rule out objects if they have the Is Marker flag set (as Activators or non-playable Furnitures).
               TESForm* baseForm = static_cast<TESForm*>(reference->baseForm);
               if (baseForm)
                  if ((baseForm->flags & 0x800000) != 0) { // "Is Marker" flag, bit 23 {
                     //_MESSAGE(PapyrusPrefixString("CellSearchFunctor") "::IsSelectable: Ruling out reference ID 0x%08X; it is a marker.", reference->formID);
                     return false;
                  }
            }
            {  // Rule out objects if their BSXFlags don't have the Enable Collision flag set.
               NiNode* node = reference->GetNiNode();
               if (node) {
                  NiIntegerExtraData* bsx = static_cast<NiIntegerExtraData*>(node->GetExtraData(BSFixedString("BSX")));
                  if (bsx == nullptr) {
                     //_MESSAGE(PapyrusPrefixString("CellSearchFunctor") "::IsSelectable: Ruling out reference ID 0x%08X; it has no BSXFlags.", reference->formID);
                     return false;
                  }
                  if ((bsx->m_data & 2) == 0) {
                     //_MESSAGE(PapyrusPrefixString("CellSearchFunctor") "::IsSelectable: Ruling out reference ID 0x%08X; its BSXFlags lack the collision bit.", reference->formID);
                     return false;
                  }
                  if (node->m_name == nullptr) {
                     //
                     // The root 3D node is nameless. This usually indicates an automatically-generated node, such 
                     // as the 3D for a trigger box. Let's loop over the node tree and make sure that there's actual 
                     // collision data; trigger boxes have been shown not to have any in my tests.
                     //
                     if (!((RE::TESObjectREFR*)reference)->HasCollision())
                        return false;
                  }
               }
            }
         }
         return true;
      };
      bool CellSearchFunctor::TypeTest(TESObjectREFR* reference) {
         bool bInclude = false;
         UInt32 iType = reference->formType;
         if (iType == 61)
            iType = reference->baseForm->formType;
         switch (iType) {
            case kFormType_Activator:     if (this->searchParameters.types.bIncludeACTI) bInclude = true; break;
            case kFormType_Container:     if (this->searchParameters.types.bIncludeCONT) bInclude = true; break;
            case kFormType_Door:          if (this->searchParameters.types.bIncludeDOOR) bInclude = true; break;
            case kFormType_Furniture:     if (this->searchParameters.types.bIncludeFURN) bInclude = true; break;
            case kFormType_Light:         if (this->searchParameters.types.bIncludeLIGH) bInclude = true; break;
            case kFormType_MovableStatic: if (this->searchParameters.types.bIncludeMSTT) bInclude = true; break;
            case kFormType_Character:
            case kFormType_NPC:           if (this->searchParameters.types.bIncludeNPC_) bInclude = true; break;
            case kFormType_Static:        if (this->searchParameters.types.bIncludeSTAT) bInclude = true; break;
            case kFormType_Tree:          if (this->searchParameters.types.bIncludeTREE) bInclude = true; break;
            case kFormType_Ammo:       // AMMO
            case kFormType_Armor:      // ARMO
            case kFormType_Book:       // BOOK
            case kFormType_Misc:       // MISC
            case kFormType_ScrollItem: // SCRL
            case kFormType_SoulGem:    // SLGM
            case kFormType_Weapon:     // WEAP
            case kFormType_Apparatus:  // APPA - Scrapped form type indicating objects used in crafting, such as the retort glasses on Alchemy Labs. Derives from MISC.
            case kFormType_Ingredient: // INGR
            case kFormType_Key:        // KEYM
            case kFormType_Potion:     // ALCH
               if (this->searchParameters.types.bIncludeItem)
                  bInclude = true;
               break;
         }
         if (!bInclude && this->searchParameters.formLists.size()) {
            //
            // The Papyrus-side caller has asked us to include objects whose 
            // base forms are in one of [one or more] FormLists.
            //
            TESForm* baseForm = reference->baseForm;
            //_MESSAGE("CellSearcherInstance is checking its added FormLists...");
            for (UInt32 i = 0; i < this->searchParameters.formLists.size(); i++) {
               BGSListForm* current = this->searchParameters.formLists[i];
               if (current == nullptr)
                  continue;
               //_MESSAGE("CellSearcherInstance is checking added FormList %d...", i);
               
               class FormListSearcherNested : public BGSListForm::Visitor {
                  public:
                     FormListSearcherNested(TESForm* needle) : _needle(needle) { };
                     virtual bool Accept(TESForm* form) {
                        if (this->_needle == form)
                           return true;
                        return false;
                     };
                  private:
                     TESForm* _needle; // find it in a haystack...
               };
               class FormListSearcher : public BGSListForm::Visitor {
                  public:
                     FormListSearcher(TESForm* needle) : _needle(needle) { };
                     virtual bool Accept(TESForm* form) {
                        if (this->_needle == form)
                           return true;
                        if (form && form->formType == kFormType_List) {
                           BGSListForm* list = DYNAMIC_CAST(form, TESForm, BGSListForm);
                           if (list) {
                              FormListSearcherNested walker(this->_needle);
                              return list->Visit(walker);
                           }
                        }
                        return false;
                     };
                  private:
                     TESForm* _needle; // find it in a haystack...
               };

               FormListSearcher walker(baseForm);
               if (current->Visit(walker)) {
                  //_MESSAGE("CellSearcherInstance: reference 0x%08X (base form ID 0x%08X) apparently found in FormList %d.", reference->formID, baseForm->formID, i);
                  return true;
               }
            }
         }
         return bInclude;
      };
      void CellSearchFunctor::AddFormList(BGSListForm* formList) {
         if (IsExecuting())
            return;
         if (formList)
            this->searchParameters.formLists.push_back(formList);
      }
      //
      // Papyrus APIs:
      //
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, float x, float y, float z) {
         CellSearchFunctor* func = new CellSearchFunctor(stackId);
         func->searchParameters.coordinates.x = x;
         func->searchParameters.coordinates.y = y;
         func->searchParameters.coordinates.z = z;
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         #ifdef COBBDEBUGSEARCHER
         _MESSAGE("===========================================================================");
         _MESSAGE("  Searcher task %d created at: (%f, %f, %f)", handle, x, y, z);
         _MESSAGE("===========================================================================");
         #endif
         return handle;
      }
      SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<float> afSearchPoint) {
         ERROR_AND_RETURN_0_IF(afSearchPoint.Length() < 3, "afSearchPoint must be an array with a length of 3 or more.", registry, stackId);
         CellSearchFunctor* func = new CellSearchFunctor(stackId);
         afSearchPoint.Get(&(func->searchParameters.coordinates.x), 0);
         afSearchPoint.Get(&(func->searchParameters.coordinates.y), 1);
         afSearchPoint.Get(&(func->searchParameters.coordinates.z), 2);
         UInt32 handle = StorableObjectStorageInstance().StoreObject(func, stackId);
         #ifdef COBBDEBUGSEARCHER
         _MESSAGE("===========================================================================");
         _MESSAGE("  Searcher task %d created at: (%f, %f, %f)", handle, func->searchParameters.coordinates.x, func->searchParameters.coordinates.y, func->searchParameters.coordinates.z);
         _MESSAGE("===========================================================================");
         #endif
         return handle;
      }
      void SetCell(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, TESObjectCELL* cell) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->searchParameters.cell = cell;
      }
      void SetTolerance(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, float tolerance) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->searchParameters.tolerance = tolerance;
      }
      void SetCriteriaA(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         SInt32 abIncludeACTI, // 1 to include; 0 to not include; -1 to leave unchanged
         SInt32 abIncludeCONT,
         SInt32 abIncludeDOOR,
         SInt32 abIncludeFURN,
         SInt32 abIncludeLIGH,
         SInt32 abIncludeMSTT,
         SInt32 abIncludeNPC_,
         SInt32 abIncludeSTAT,
         SInt32 abIncludeTREE
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         if (abIncludeACTI != -1) func->searchParameters.types.bIncludeACTI = (bool)abIncludeACTI;
         if (abIncludeCONT != -1) func->searchParameters.types.bIncludeCONT = (bool)abIncludeCONT;
         if (abIncludeDOOR != -1) func->searchParameters.types.bIncludeDOOR = (bool)abIncludeDOOR;
         if (abIncludeFURN != -1) func->searchParameters.types.bIncludeFURN = (bool)abIncludeFURN;
         if (abIncludeLIGH != -1) func->searchParameters.types.bIncludeLIGH = (bool)abIncludeLIGH;
         if (abIncludeMSTT != -1) func->searchParameters.types.bIncludeMSTT = (bool)abIncludeMSTT;
         if (abIncludeNPC_ != -1) func->searchParameters.types.bIncludeNPC_ = (bool)abIncludeNPC_;
         if (abIncludeSTAT != -1) func->searchParameters.types.bIncludeSTAT = (bool)abIncludeSTAT;
         if (abIncludeTREE != -1) func->searchParameters.types.bIncludeTREE = (bool)abIncludeTREE;
      }
      void SetCriteriaB(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 handle,
         SInt32 abIncludeItem
      ) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         //
         if (abIncludeItem != -1) func->searchParameters.types.bIncludeItem = (bool)abIncludeItem;
      }
      void SetFilterMode(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, UInt32 mode) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         ERROR_AND_RETURN_IF(!CellSearchFunctor::SearchParameters::FilterModeIsValid(mode), "You didn't specify a valid filter mode.", registry, stackId);
         //
         func->searchParameters.filterMode = (CellSearchFunctor::SearchParameters::FilterMode)(UInt8)mode;
      }
      void SetExcludesPlayer(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, bool flag) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->searchParameters.excludePlayer = flag;
      }
      void SetRequiresCollision(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, bool flag) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->searchParameters.requireCollision = flag;
      }
      void SetSearchLoadedArea(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, bool flag) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         func->searchParameters.searchLoadedArea = flag;
         DEBUG_ONLY_CELLSEARCHER_MESSAGE("CellSearcher handle %d loaded-area flag set to %d.", handle, flag);
      }
      void AddFormList(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, BGSListForm* formList) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("modified");
         ERROR_AND_RETURN_IF(formList == nullptr, "Cannot add a non-existent FormList.", registry, stackId);
         func->AddFormList(formList);
      }
      bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_0_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_VALUE_IF_WRONG_STACK("executed", false);
         func = StorableObjectStorageInstance().TakeObject<CellSearchFunctor>(handle); // remove it from storage
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("CellSearch") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled and its handle has been freed.", stackId);
            delete func;
            return false;
         }
      }
      bool CreateAndRun(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<float> afSearchPoint,
         VMArray<bool> abSearchCriteria,
         float tolerance,
         TESObjectCELL* cell
      ) {
         ERROR_AND_RETURN_0_IF(afSearchPoint.Length() < 3, "The search point must be a float array with at least three elements. Returning null...", registry, stackId);
         CellSearchFunctor* func = new CellSearchFunctor(stackId);
         //
         afSearchPoint.Get(&(func->searchParameters.coordinates.x), 0);
         afSearchPoint.Get(&(func->searchParameters.coordinates.y), 1);
         afSearchPoint.Get(&(func->searchParameters.coordinates.z), 2);
         //
         func->searchParameters.tolerance = tolerance;
         func->searchParameters.cell = cell;
         //
         {
            UInt32 length = abSearchCriteria.Length();
            if (length > 0) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeACTI), 0);
            if (length > 1) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeCONT), 1);
            if (length > 2) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeDOOR), 2);
            if (length > 3) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeFURN), 3);
            if (length > 4) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeLIGH), 4);
            if (length > 5) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeMSTT), 5);
            if (length > 6) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeNPC_), 6);
            if (length > 7) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeSTAT), 7);
            if (length > 8) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeTREE), 8);
            if (length > 9) abSearchCriteria.Get(&(func->searchParameters.types.bIncludeItem), 9);
         }
         //
         if (func->Queue()) {
            return true;
         } else {
            registry->LogError("Unable to queue " PapyrusPrefixString("CellSearch") " operation; too many batch operations have been queued for its SKSE plug-in. The operation has been canceled.", stackId);
            delete func;
            return false;
         }
         return true;
      };
      void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle) {
         ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId);
         CellSearchFunctor* func = StorableObjectStorageInstance().AccessObject<CellSearchFunctor>(handle);
         ERROR_AND_RETURN_IF(func == nullptr, "Failed to lookup object for given handle.", registry, stackId);
         ERROR_AND_RETURN_IF_WRONG_STACK("canceled");
         func = StorableObjectStorageInstance().TakeObject<CellSearchFunctor>(handle); // remove it from storage
         delete func;
      }
   }
};

bool PapyrusPrefix(Papyrus)::CellSearcher::Register(VMClassRegistry* registry) {
   //
   // OBJECT STORAGE SETUP
   //
   StorableObjectRegistry& skseObjRegistry = StorableObjectRegistryInstance();
   skseObjRegistry.RegisterClass<CellSearchFunctor>();
   //
   // PAPYRUS APIS
   //
   registry->RegisterFunction(
      new NativeFunction3<StaticFunctionTag, SInt32, float, float, float>(
         "Create",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::Create,
         registry
      )
   );
   //registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "Create", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, SInt32, VMArray<float>>(
         "CreateFromArray",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::Create,
         registry
      )
   );
   //registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "CreateFromArray", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, BGSListForm*>(
         "AddFormList",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::AddFormList,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "AddFormList", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, TESObjectCELL*>(
         "SetCell",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetCell,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetCell", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, float>(
         "SetTolerance",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetTolerance,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetTolerance", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction10<StaticFunctionTag, void, SInt32, SInt32, SInt32, SInt32, SInt32, SInt32, SInt32, SInt32, SInt32, SInt32>(
         "SetCriteriaA",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetCriteriaA,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetCriteriaA", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, SInt32>(
         "SetCriteriaB",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetCriteriaB,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetCriteriaB", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, UInt32>(
         "SetFilterMode",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetFilterMode,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetFilterMode", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, bool>(
         "SetExcludesPlayer",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetExcludesPlayer,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetExcludesPlayer", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, bool>(
         "SetRequiresCollision",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetRequiresCollision,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetRequiresCollision", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction2<StaticFunctionTag, void, SInt32, bool>(
         "SetSearchLoadedArea",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::SetSearchLoadedArea,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "SetSearchLoadedArea", VMClassRegistry::kFunctionFlag_NoWait);
   registry->RegisterFunction(
      new NativeFunction1<StaticFunctionTag, void, SInt32>(
         "Cancel",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::Cancel,
         registry
      )
   );
   registry->SetFunctionFlags(PapyrusPrefixString("CellSearcher"), "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
   //
   // LATENT FUNCTIONS:
   //
   registry->RegisterFunction(
      new LatentNativeFunction1<StaticFunctionTag, TESObjectREFR*, SInt32>(
         "Run",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::Run,
         registry
      )
   );
   registry->RegisterFunction(
      new LatentNativeFunction4<StaticFunctionTag, TESObjectREFR*, VMArray<float>, VMArray<bool>, float, TESObjectCELL*>(
         "CreateAndRun",
         PapyrusPrefixString("CellSearcher"),
         CellSearcher::CreateAndRun,
         registry
      )
   );
   //
   // DONE.
   //
   return true;
};