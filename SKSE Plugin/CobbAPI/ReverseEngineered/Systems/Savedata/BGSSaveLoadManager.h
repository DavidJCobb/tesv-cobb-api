#pragma once
#include "skse/GameTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Systems/ChangeForms.h"

namespace RE {
   class BGSSaveLoadManager { // sizeof == 0x58
      public:
         enum {
            kEvent_Autosave =	1 << 0,
            kEvent_Save =		1 << 1,
            kEvent_Unk02 =		1 << 2,
            kEvent_Unk03 =		1 << 3,
            kEvent_Unk04 =		1 << 4,
            //
            kEvent_Unk07 =		1 << 7,
         };

         static BGSSaveLoadManager* GetSingleton(void) {
            return *(BGSSaveLoadManager**)0x01B2E38C;
         };

         void Save(const char* name);
         void Load(const char* name);

         // used by Hooks_SaveLoad
         void	SaveGame_Hook(const char * saveName);
         bool	LoadGame_Hook(const char * saveName, bool unk1);
         void	ProcessEvents_Hook(void);

         // use these when calling from a papyrus thread
         void	RequestSave(const char * name);
         void	RequestLoad(const char * name);


         tList<const char*>* saveList;			// 00
         UInt32				unk04;				// 04
         UInt32				unk08;				// 08
         UInt32				unk0C;				// 0C
         UInt8				unk10;				// 10
         UInt8				unk11;				// 11
         UInt8				pad12[2];			// 12
         UInt32				pendingEvents;		// 14
         UInt32				unk18;				// 18
         UInt32				startTickCount;		// 1C - GetTickCount when constructed
         UInt8				unk20;				// 20 - init'd to 0x01
         UInt8				pad21[3];			// 21
         UInt32				unk24;				// 24
         UInt32				unk28;				// 28 - init'd to 0xFFFFFFFF
         bool				unk2C;				// 2C
         UInt8				pad2E[3];			// 2E
         void				* unk30;			// 30
         UInt8				unk34;				// 34 - init'd to 0x01
         UInt8				unk35;				// 35
         UInt8				pad36[2];			// 36
         UInt32				unk38;				// 38
         UInt32				unk3C;				// 3C
         UInt32				unk40;				// 40
         UInt32				unk44;				// 44
         UInt32				unk48;				// 48
         UInt32				unk4C;				// 4C
         UInt32				unk50;				// 50
         void				* unk54;			// 54
         UInt32 unk58[(0x3E8 - 0x58) / sizeof(UInt32)];
         ChangeFormManager* unk3E8; // 3E8
         UInt32 unk3EC; // 3EC
         UInt32 unk3F0; // 3F0 // bitmask

      public:
         MEMBER_FN_PREFIX(BGSSaveLoadManager);
      private:
         DEFINE_MEMBER_FN(Save_Internal, bool, 0x006814D0, const char * name, int unk1, UInt32 unk2);
         DEFINE_MEMBER_FN(Load_Internal, bool, 0x006821C0, const char * name, int unk1, UInt32 unk2, UInt32 unk3);
         //
         DEFINE_MEMBER_FN(SaveGame_HookTarget, void, 0x00679200, const char * fileName);
         DEFINE_MEMBER_FN(LoadGame_HookTarget, bool, 0x0067B720, const char * fileName, bool unk0);
         //
         DEFINE_MEMBER_FN(ProcessEvents_Internal, void, 0x00682400);
         //
      public:
         DEFINE_MEMBER_FN(ChangeDataExistsForFormID, bool, 0x00677FA0, UInt32 formID); // calls ChangeFormManager::HasFormID
         DEFINE_MEMBER_FN(RemoveChangeFlags,         void, 0x00678280, TESForm*, UInt32 flagsToRemove);
         DEFINE_MEMBER_FN(Subroutine00678B50, void, 0x00678B50, TESObjectREFR* formID); // destroys the reference
         DEFINE_MEMBER_FN(Subroutine00678EC0, void, 0x00678EC0, UInt32 formID); // acts on a loaded form
   };
}