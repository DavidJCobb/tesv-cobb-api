#pragma once
#include "Papyrus/Array.h"

#include "Papyrus/_Prefixes.h"

#include "skse/PapyrusNativeFunctions.h"
#include "skse/PapyrusObjects.h"
#include "skse/PapyrusVM.h"

#include "skse/GameReferences.h"
#include "skse/GameRTTI.h"

class TESForm;
class TESObjectREFR;

namespace CobbPapyrus {
   namespace Array {
      //
      template <class T>
      SInt32 CountTruthy(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<T> subject
      ) {
         SInt32 result = 0;
         UInt32 count  = subject.Length();
         for (UInt32 i = 0; i < count; i++) {
            T item;
            subject.Get(&item, i);
            if (item)
               result++;
         }
         return result;
      };
      //
      template <class T>
      VMResultArray<T> Create(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         SInt32 size, T fill
      ) {
         VMResultArray<T> result;
         result.resize(size);
         for (UInt32 i = 0; i < size; i++) {
            result[i] = fill;
         }
         return result;
      };
      //
      template <class T>
      bool HasTruthy(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<T> subject
      ) {
         UInt32 count = subject.Length();
         for (UInt32 i = 0; i < count; i++) {
            T item;
            subject.Get(&item, i);
            if (item)
               return true;
         }
         return false;
      };
      //
      template <class T>
      void Overwrite(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<T> target, VMArray<T> source, SInt32 targetStart, SInt32 sourceStart, SInt32 count
      ) {
         UInt32 targetLength = target.Length();
         if (targetLength < targetStart)
            return;
         targetLength -= targetStart;
         UInt32 sourceLength = source.Length();
         if (sourceLength < sourceStart)
            return;
         sourceLength -= sourceStart;
         if (count > sourceLength)
            count = sourceLength;
         for (UInt32 i = 0; i < count; i++) {
            T item;
            source.Get(&item, i + sourceStart);
            target.Set(&item, i + targetStart);
         }
      };
      //
      template <class T1, class T2>
      void OverwriteWithForeign(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<T1> target, VMArray<T2> source, SInt32 targetStart, SInt32 sourceStart, SInt32 count
      ) {
         UInt32 targetLength = target.Length();
         if (targetLength < targetStart)
            return;
         targetLength -= targetStart;
         UInt32 sourceLength = source.Length();
         if (sourceLength < sourceStart)
            return;
         sourceLength -= sourceStart;
         if (count > sourceLength)
            count = sourceLength;
         for (UInt32 i = 0; i < count; i++) {
            T1 converted;
            {
               T2 item;
               source.Get(&item, i + sourceStart);
               converted = item;
            }
            target.Set(&converted, i + targetStart);
         }
      };
      //
      template <class T>
      VMResultArray<T> Resize(
         VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*,
         VMArray<T> arr, SInt32 size, T fill
      ) {
         VMResultArray<T> result;
         result.resize(size);
         UInt32 length = arr.Length();
         for (UInt32 i = 0; i < size; i++) {
            if (i < length)
               arr.Get(&(result[i]), i);
            else
               result[i] = fill;
         };
         return result;
      };
      //
      template <class T>
      VMResultArray<T> Slice(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, VMArray<T> arr, SInt32 start, SInt32 count) {
         VMResultArray<T> result;
         result.resize(count);
         UInt32 length = arr.Length();
         for (UInt32 i = 0; i < count; i++) {
            if (start + i >= length)
               break;
            arr.Get(&(result[i]), start + i);
         };
         return result;
      };
   }
}

bool CobbPapyrus::Array::Register(VMClassRegistry* registry) {
   {  // CountTruthy
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, SInt32, VMArray<float>>(
            "CountTruthyFloat",
            PapyrusPrefixString("Array"),
            Array::CountTruthy,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, SInt32, VMArray<TESForm*>>(
            "CountTruthyForm",
            PapyrusPrefixString("Array"),
            Array::CountTruthy,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, SInt32, VMArray<SInt32>>(
            "CountTruthyInt",
            PapyrusPrefixString("Array"),
            Array::CountTruthy,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, SInt32, VMArray<TESObjectREFR*>>(
            "CountTruthyReference",
            PapyrusPrefixString("Array"),
            Array::CountTruthy,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CountTruthyFloat",     VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CountTruthyForm",      VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CountTruthyInt",       VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CountTruthyReference", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Create
      registry->RegisterFunction(
         new NativeFunction2 <StaticFunctionTag, VMResultArray<float>, SInt32, float>(
            "CreateFloat",
            PapyrusPrefixString("Array"),
            Array::Create,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction2 <StaticFunctionTag, VMResultArray<TESForm*>, SInt32, TESForm*>(
            "CreateForm",
            PapyrusPrefixString("Array"),
            Array::Create,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction2 <StaticFunctionTag, VMResultArray<SInt32>, SInt32, SInt32>(
            "CreateInt",
            PapyrusPrefixString("Array"),
            Array::Create,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction2 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32, TESObjectREFR*>(
            "CreateReference",
            PapyrusPrefixString("Array"),
            Array::Create,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CreateFloat", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CreateForm", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CreateInt", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "CreateReference", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // HasTruthy
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, bool, VMArray<float>>(
            "HasTruthyFloat",
            PapyrusPrefixString("Array"),
            Array::HasTruthy,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, bool, VMArray<TESForm*>>(
            "HasTruthyForm",
            PapyrusPrefixString("Array"),
            Array::HasTruthy,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, bool, VMArray<SInt32>>(
            "HasTruthyInt",
            PapyrusPrefixString("Array"),
            Array::HasTruthy,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction1 <StaticFunctionTag, bool, VMArray<TESObjectREFR*>>(
            "HasTruthyReference",
            PapyrusPrefixString("Array"),
            Array::HasTruthy,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "HasTruthyFloat",     VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "HasTruthyForm",      VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "HasTruthyInt",       VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "HasTruthyReference", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Overwrite
      registry->RegisterFunction(
         new NativeFunction5 <StaticFunctionTag, void, VMArray<float>, VMArray<float>, SInt32, SInt32, SInt32>(
            "OverwriteFloat",
            PapyrusPrefixString("Array"),
            Array::Overwrite,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction5 <StaticFunctionTag, void, VMArray<TESForm*>, VMArray<TESForm*>, SInt32, SInt32, SInt32>(
            "OverwriteForm",
            PapyrusPrefixString("Array"),
            Array::Overwrite,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction5 <StaticFunctionTag, void, VMArray<SInt32>, VMArray<SInt32>, SInt32, SInt32, SInt32>(
            "OverwriteInt",
            PapyrusPrefixString("Array"),
            Array::Overwrite,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction5 <StaticFunctionTag, void, VMArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, SInt32, SInt32, SInt32>(
            "OverwriteReference",
            PapyrusPrefixString("Array"),
            Array::Overwrite,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "OverwriteFloat",     VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "OverwriteForm",      VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "OverwriteInt",       VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "OverwriteReference", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // OverwriteWithType
      registry->RegisterFunction(
         new NativeFunction5 <StaticFunctionTag, void, VMArray<float>, VMArray<SInt32>, SInt32, SInt32, SInt32>(
            "OverwriteFloatWithInt",
            PapyrusPrefixString("Array"),
            Array::OverwriteWithForeign,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction5 <StaticFunctionTag, void, VMArray<SInt32>, VMArray<float>, SInt32, SInt32, SInt32>(
            "OverwriteIntWithFloat",
            PapyrusPrefixString("Array"),
            Array::OverwriteWithForeign,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "OverwriteFloatWithInt", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "OverwriteIntWithFloat", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Resize
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, SInt32, float>(
            "ResizeFloat",
            PapyrusPrefixString("Array"),
            Array::Resize,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<TESForm*>, VMArray<TESForm*>, SInt32, TESForm*>(
            "ResizeForm",
            PapyrusPrefixString("Array"),
            Array::Resize,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<SInt32>, VMArray<SInt32>, SInt32, SInt32>(
            "ResizeInt",
            PapyrusPrefixString("Array"),
            Array::Resize,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, SInt32, TESObjectREFR*>(
            "ResizeReference",
            PapyrusPrefixString("Array"),
            Array::Resize,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "ResizeFloat", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "ResizeForm", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "ResizeInt", VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "ResizeReference", VMClassRegistry::kFunctionFlag_NoWait);
   }
   {  // Slice
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, SInt32, SInt32>(
            "SliceFloat",
            PapyrusPrefixString("Array"),
            Array::Slice,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<TESForm*>, VMArray<TESForm*>, SInt32, SInt32>(
            "SliceForm",
            PapyrusPrefixString("Array"),
            Array::Slice,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<SInt32>, VMArray<SInt32>, SInt32, SInt32>(
            "SliceInt",
            PapyrusPrefixString("Array"),
            Array::Slice,
            registry
         )
      );
      registry->RegisterFunction(
         new NativeFunction3 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, VMArray<TESObjectREFR*>, SInt32, SInt32>(
            "SliceReference",
            PapyrusPrefixString("Array"),
            Array::Slice,
            registry
         )
      );
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "SliceFloat",     VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "SliceForm",      VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "SliceInt",       VMClassRegistry::kFunctionFlag_NoWait);
      registry->SetFunctionFlags(PapyrusPrefixString("Array"), "SliceReference", VMClassRegistry::kFunctionFlag_NoWait);
   }
   //
   // DONE.
   //
   return true;
};