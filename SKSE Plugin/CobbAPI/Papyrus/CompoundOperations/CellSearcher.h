#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>
#include "Types/bounds.h"
#include "CobbRotation.h"

namespace CobbPapyrus {
   namespace CellSearcher {
      bool Register(::VMClassRegistry* registry);

      class CellSearchFunctor : public CompoundOperationBase {
         public:
            explicit CellSearchFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit CellSearchFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("CellSearchFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 2 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual bool OnStart();
            virtual void Run(VMValue& resultValue);
            virtual void OnEnd();

            struct SearchParameters {
               enum FilterMode : UInt8 {
                  kFilterMode_Candidates = 0, // The search criteria determine what objects we ignore while searching.
                  kFilterMode_Result = 1      // The search criteria are applied after the search is complete; we return None if the result doesn't match.
               };
               static bool FilterModeIsValid(UInt8 mode) {
                  switch (mode) {
                     case kFilterMode_Candidates:
                     case kFilterMode_Result:
                        return true;
                     default:
                        return false;
                  };
               };
               struct FilterTypes {
                  bool bIncludeACTI = true;
                  bool bIncludeCONT = true;
                  bool bIncludeDOOR = true;
                  bool bIncludeFURN = true;
                  bool bIncludeLIGH = false;
                  bool bIncludeMSTT = true; // MovableStatics (must be checked indirectly)
                  bool bIncludeNPC_ = true;
                  bool bIncludeSTAT = true;
                  bool bIncludeTREE = true;
                  bool bIncludeItem = true; // AMMO, ARMO, BOOK, INGR, MISC, SCRL, SLGM, WEAP, and others
               };
               TESObjectCELL*              cell = NULL;
               NiPoint3                    coordinates;
               FilterTypes                 types;
               std::vector<BGSListForm*>   formLists;
               FilterMode  filterMode        = kFilterMode_Candidates;
               bool        excludePlayer     = true;
               bool        requireCollision  = true;
               float       tolerance         = 0.5;
               bool        searchLoadedArea  = false;
            } searchParameters;
            //
            void AddFormList(BGSListForm* formList);
         private:
            virtual bool SearchCell(TESObjectCELL* cell, TESObjectREFR** out, float* volume);
            virtual bool TypeTest(TESObjectREFR* reference);
            virtual bool IsSelectable(TESObjectREFR* reference);
      };
   }
}