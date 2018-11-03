#pragma once
#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   class TESObjectCELL; // Forward-declare instead of #including, so the compiler doesn't choke on a circular dependency
   class TESObjectREFR;

   struct MapMarkerOperation {
      UInt32 unk00;
      tArray<UInt32> markerHandles; // 04
      UInt32 unk10;
      UInt32 unk14; // show/hide
      UInt32 unk18; // enable travel?
      UInt32 unk1C;
      UInt32 unk20; // affect already-hidden markers?
   };

   class TESWorldSpace;
   typedef bool(*MapMarkerIteratorCallback)(RE::TESWorldSpace* currentWorld, RE::TESObjectREFR* ref, RE::ExtraMapMarker::Data*, void* state);

   class TESWorldSpace : public TESForm {
      public:
         operator ::TESWorldSpace*() const { return (::TESWorldSpace*) this; }
         enum { kTypeID = kFormType_WorldSpace };
         //
         // Parents:
         TESFullName fullName; // 14
         TESTexture  texture;  // 1C

         struct UnkRecurring { // sizeof == 0x10
            UInt32 unk00 = 0;
            UInt32 unk04 = 0;
            UInt32 unk08 = 0; // actually an int
            void*  unk0C = (void*) 0x012456E8; // *member == 0xDEADBEEF
         };
         UInt32 unk24;
         UInt32 unk28;
         UInt32 unk2C;
         UnkRecurring unk30;
         UInt32 unk40;
         void*  unk44; // points to 0xDEADBEEF
         UInt32 unk48;
         UInt32 unk4C = 0; // 4C // related to loaded cells and CRCs
         TESObjectCELL* persistentCell = nullptr; // 50 // related to map markers
         UInt32 unk54 = 0;
         UInt32 unk58 = 0;
         UInt8  unk5C = 0; // flags; flag 1 means form ID is in the "hardcoded" space (i.e. form ID <= 0x7FF)
         UInt8  unk5D;
         UInt16 unk5E = 0;
         UInt16 unk60 = 0;
         UInt16 unk62 = 0;
         UInt32 unk64;
         UInt32 unk68;
         UnkRecurring unk6C;
         UInt32 unk7C;
         UInt32 unk80 = 0;
         tArray<UInt32> unk84;
         UInt32 unk90 = 0;
         UInt32 unk94 = 0;
         UInt32 unk98;
         UInt32 unk9C;
         UnkRecurring unkA0;
         UInt32 unkB0;
         UInt32 unkB4 = 0;
         void*  unkB8 = nullptr; // NiRefObject* subclass?
         UInt32 unkBC;
         UInt32 unkC0;
         UInt32 unkC4 = 0;
         UInt32 unkC8 = 0;
         TESWorldSpace* parentWorld = nullptr; // CC // related to map markers
         UInt32 unkD0 = 0;
         UInt32 unkD4 = 0;
         UInt32 unkD8 = 0;
         UInt32 unkDC = 0;
         UInt32 unkE0 = 0;
         UInt32 unkE4 = 0;
         UInt32 unkE8 = 0;
         UInt32 unkEC = 0;
         UInt32 unkF0 = 0;
         float  unkF4 = 50000.0F;
         float  unkF8 = 80000.0F;
         float  unkFC = 50.0F;
         float  unk100 = 1.0F;
         UInt32 unk104 = 0;
         UInt32 unk108 = 0;
         UInt32 unk10C = 0;
         UInt32 unk110 = 0;
         float  unk114 = FLT_MAX;
         float  unk118 = FLT_MAX;
         float  unk11C = FLT_MIN;
         float  unk120 = FLT_MIN;
         UInt32 unk124;
         UInt32 unk128;
         UnkRecurring unk12C; // 12C
         UInt32 unk13C;
         UInt32 unk140 = 0;
         BSString unk144;
         float  unk14C = -2048.0F;
         UInt32 unk150 = 0;
         float  unk154 = 1.0F;
         UInt32 unk158 = 0;
         UInt32 unk15C = 0;
         TESTexture unk160; // 160
         TESTexture unk168; // 160
         struct Struct004AD6A0 { // sizeof == 0x60
            UInt32 unk00;
            UInt32 unk04;
            UInt32 unk08 = 0;
            UInt32 unk0C = 0;
            UInt32 unk10 = 0;
            void*  unk14 = (void*) 0x01243754; // *member == 0xDEADBEEF
            UInt32 unk18;
            UInt32 unk1C = 0;
            UInt32 unk20;
            UInt32 unk24;
            UInt32 unk28 = 0;
            UInt32 unk2C = 0;
            UInt32 unk30 = 0;
            void*  unk34 = (void*)0x01243754; // *member == 0xDEADBEEF
            UInt32 unk38;
            UInt32 unk3C = 0;
            UInt32 unk40;
            UInt32 unk44;
            UInt32 unk48 = 0;
            UInt32 unk4C = 0;
            UInt32 unk50 = 0;
            void*  unk54 = (void*)0x01243754; // *member == 0xDEADBEEF
            UInt32 unk58;
            UInt32 unk5C = 0;
         } unk170; // 170
         float northRotation = 0; // 1D0
         UInt32 unk1D4 = 0;
         //
         MEMBER_FN_PREFIX(TESWorldSpace);
         DEFINE_MEMBER_FN(GetMapMarkerHandles,        void,               0x004F2450, MapMarkerOperation* out, bool shouldBeFalse); // if the bool is true, we pass the markers to DataHandler?
         DEFINE_MEMBER_FN(GetUnk2CFlagFlag00000010,   bool,               0x004C0C50); 
         DEFINE_MEMBER_FN(GetCellAtMergedCoordinates, RE::TESObjectCELL*, 0x004375D0, UInt32); // Argument is: 0xXXXXYYYY.
         DEFINE_MEMBER_FN(GetCellThatContainsPoint,   RE::TESObjectCELL*, 0x004F3410, NiPoint3*); // Assumed, but it's a pretty good assumption to make
         DEFINE_MEMBER_FN(GetWaterLevel,              float,              0x004F0930); // There is also a similar subroutine at 0x004F0900, but I don't know what it returns.
         //
         DEFINE_MEMBER_FN(AddToPersistentCell,       void,           0x004F5600, TESObjectREFR*);
         DEFINE_MEMBER_FN(RemoveFromPersistentCell,  void,           0x004F0A80, TESObjectREFR*); // purpose assumed; needs verification
         DEFINE_MEMBER_FN(GetOrCreatePersistentCell, TESObjectCELL*, 0x004F4420);
         DEFINE_MEMBER_FN(SetPersistentCell,         void,           0x004F3F10, TESObjectCELL*);
         //
         DEFINE_MEMBER_FN(Subroutine004F5330, void*, 0x004F5330, UInt32, UInt32); // Arguments are cell coordinates.

         void ForEachMapMarker(MapMarkerIteratorCallback, void* state); // all map markers in all worlds found by GetAllRelatedWorldspaces
         void GetAllRelatedWorldspaces(std::vector<TESWorldSpace*>& out); // includes self
         TESWorldSpace* GetRootWorldspace() {
            auto root = this;
            while (root->parentWorld)
               root = root->parentWorld;
            return root;
         };
   };
   static_assert(sizeof(TESWorldSpace) >= 0x1D8, "RE::TESWorldSpace is too small.");
   static_assert(sizeof(TESWorldSpace) <= 0x1D8, "RE::TESWorldSpace is too large.");
};