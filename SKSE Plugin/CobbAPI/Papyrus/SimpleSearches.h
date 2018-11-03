#pragma once

#include "Papyrus/_Prefixes.h"
#include "Papyrus/CompoundOperations/base.h"
#include "skse/PapyrusArgs.h"

#include "skse/GameReferences.h"
#include "skse/Serialization.h"

#include <vector>
#include "CobbRotation.h"

namespace CobbPapyrus {
   namespace SimpleSearches {
      bool Register(::VMClassRegistry* registry);

      class GetAllOfBaseFormInCellFunctor : public CompoundOperationBase {
         public:
            explicit GetAllOfBaseFormInCellFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit GetAllOfBaseFormInCellFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("GetAllOfBaseFormInCellFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 2 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual bool OnStart();
            virtual void Run(VMValue& resultValue);
            virtual void OnEnd();
            //
            TESForm*       form;
            TESObjectCELL* cell;
            bool           searchLoadedArea = false;
         private:
            virtual void _SearchCell(TESObjectCELL*, VMResultArray<TESObjectREFR*>* out);
      };
      class GetObjectWithLinkedRefInCellFunctor : public CompoundOperationBase {
         public:
            explicit GetObjectWithLinkedRefInCellFunctor(UInt32 stackId) : CompoundOperationBase(stackId) {};
            explicit GetObjectWithLinkedRefInCellFunctor(StorableSerializationTag tag) : CompoundOperationBase(tag) {};
            //
            virtual const char*  ClassName() const { return PapyrusPrefixString("GetObjectWithLinkedRefInCellFunctor"); }
            virtual UInt32    ClassVersion() const { return 1; }
            //
            enum { kSaveVersion = 2 };
            virtual bool OnSave(SKSESerializationInterface* intfc);
            virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version);
            //
            virtual bool OnStart();
            virtual void Run(VMValue& resultValue);
            virtual void OnEnd();
            //
            TESObjectCELL* cell;
            TESObjectREFR* linked;
            BGSKeyword*    linkedKeyword;
            bool           searchLoadedArea = false;
         private:
            virtual void _SearchCell(TESObjectCELL*, VMResultArray<TESObjectREFR*>* out);
      };
   }
}