#pragma once
#include "Shared.h"

#include "skse/GameForms.h"
#include "skse/GameFormComponents.h"

class BGSLoadFormBuffer;
class BGSSaveFormBuffer;
namespace RE {
   class TESForm : public BaseFormComponent {
      public:
         operator ::TESForm*() const { return (::TESForm*) this; }
         enum { kTypeID = 0 };	// special-case
         //
         enum {
            kFormFlag_Unk00000001 = 0x00000001, // set when marking for delete, but could be more general
            kFormFlag_Unk00000002 = 0x00000002, // "modified?" // setting this on a REFR also sets it on its CELL, if the refr doesn't have flag 0x8000; setting it on a CELL sets it on the cell's parent WRLD
            //
            kFormFlag_IsDeleted   = 0x00000020, // indicates that deletion has already been carried out
            kFlagPlayerKnows  = 0x000040,
            kFlagUnk_0x400    = 0x000400,
            kFlagUnk_0x800    = 0x000800,
            kFlagUnk_0x4000   = 0x004000, // prevents the form from being saved
            kFlag_0x100000    = 0x100000, // Actor: is ignoring friendly hits
            kFlagIsMarker     = 0x800000,
         };
         enum FormFlagsCELL : UInt32 {
            kCellFlag_Persistent      = 0x00000400,
            kCellFlag_OffLimits       = 0x00020000,
            kCellFlag_DisallowWaiting = 0x00080000,
         };
         enum FormFlagsREFR : UInt32 {
            //
            kRefrFlag_MarkedForDelete    = 0x00000020,
            kRefrFlag_HideFromLocalMap   = 0x00000040,
            //
            kRefrFlag_Inaccessible       = 0x00000100, // for DOOR references
            kRefrFlag_DontLightWater     = 0x00000100, // for LIGH references
            kRefrFlag_MotionBlur         = 0x00000200, // for MSTT references
            kRefrFlag_CastsShadows       = 0x00000200, // for LIGH references
            kRefrFlag_Persistent         = 0x00000400,
            kRefrFlag_Disabled           = 0x00000800, // same flag is used in ESPs for "initially disabled"
            //
            kRefrFlag_Unk00040000        = 0x00040000, // possibly "marked for disable"
            //
            kRefrFlag_LightNeverFades    = 0x00010000,
            kRefrFlag_DontLightLandscape = 0x00020000, // for LIGH references
            //
            kRefrFlag_Unk00080000        = 0x00080000, // getter at 00401A60
            //
            kRefrFlag_IgnoringFriendlyHits = 0x00100000, // maybe actor only?
            //
            kRefrFlag_Unk00400000        = 0x00400000,
            kRefrFlag_Destroyed          = 0x00800000,
            //
            kRefrFlag_NoAIAcquire        = 0x02000000, // for item references
            //
            kRefrFlag_Unk01000000        = 0x01000000,
            kRefrFlag_DontHavokSettle    = 0x20000000,
            kRefrFlag_NoRespawn          = 0x40000000,
            kRefrFlag_Multibound         = 0x80000000,
         };
         enum FormFlagsWATR : UInt32 {
            kWatrFlag_Dangerous = 0x00020000,
         };
         //
         virtual void         Unk_04(void);		// reset/init? would leak pointers if called on a live object
         virtual void         Unk_05();		// release pointers?
         virtual bool         LoadForm(BGSLoadFormBuffer*); // 06 // Loads the form from the ESP/ESM files. Note that if the form has fields referring to other forms, those may be loaded as just UInt32 form IDs at first, in which case they'll be resolved during TESForm::InitItem.
         virtual bool         Unk_07(UInt32 arg); // 07 // no-op for Actor
         virtual bool         Unk_08(BGSLoadFormBuffer*); // 08 // for Actor, just calls LoadForm
         virtual TESForm*     Unk_09(UInt32 arg1, void * arg2);
         virtual bool         MarkChanged(UInt32 changeFlags);   // 0A
         virtual void         UnmarkChanged(UInt32 changeFlags); // 0B
         virtual bool         Unk_0C(UInt32 arg); // no-op for Actor
         virtual bool         Unk_0D(BGSSaveFormBuffer*); // 0D // possibly a "should save?" function // called by BGSSaveLoadManager::SaveGame_HookTarget+0x264 // TESObjectREFR has override; Actor doesn't
         virtual void         WriteSavedata(BGSSaveFormBuffer*); // 0E // handles actually saving the form's data // called by BGSSaveLoadManager::SaveGame_HookTarget+0x2DC if the Unk_0D call returns true
         virtual void         ReadSavedata(void* savedata); // 0F // restores form state from savedata. for TESObjectREFR*, restores extra data, flora flags and 3D state, etc.
         virtual void         Unk_10(UInt32 arg);
         virtual void         Unk_11(UInt32 arg);
         virtual void         Unk_12(UInt32 arg);
         virtual void         InitItem(void); // 13 // called once all dependent forms are loaded
         virtual void         Unk_14(void);
         virtual UInt32       GetFormType(); // 15
         virtual void         GetFormDesc(char* buf, UInt32 bufLen); // 16
         virtual bool         GetFormFlag00000040(); // 17
         virtual bool         Unk_18(); // 18
         virtual bool         IsPlayable();      // 19
         virtual bool         Unk_1A();          // 1A
         virtual bool         GetFormFlag00020000(); // 1B // if called on TESObjectREFR, retrieves the base form's flag
         virtual bool         Unk_1C(); // 1C
         virtual bool         GetFormFlag00000200(); // 1D // if called on TESObjectREFR, retrieves the base form's flag
         virtual bool         GetFormFlag00000100(void); // 1E
         virtual void         SetFormFlag00000200(bool set); // 1F
         virtual bool         Unk_20(void);              // 20
         virtual void         SetFormFlag00000020(bool set); // 21
         virtual bool         Unk_22();                  // 22 // This function was inserted into the VTBL after the SKSE team last mapped it.
         virtual void         SetDeletedFlag(bool); // 23 // Just sets the flag; doesn't carry out any other tasks. // This function was inserted into the VTBL after the SKSE team last mapped it.
         virtual void         SetFormFlag00000002(bool set); // 24
         virtual void         Unk_25();
         virtual void         Unk_26(BGSLoadFormBuffer*); // 26 // loads OBND?
         virtual bool         CanBePlaced(); // 27
         virtual bool         Unk_28(); // 28
         virtual bool         Unk_29(); // 29
         virtual bool         IsWaterActivator(); // 2A // returns true if the form (or its base) is a TESObjectACTI with a water type
         virtual TESObjectREFR* Unk_2B(); // 2B // Appears to be a cast of some kind. Identical to 2C for most subclasses I've checked.
         virtual TESObjectREFR* Unk_2C(); // 2C // Appears to be a cast of some kind. 2C is usually used; what's the difference between it and 2B?
         virtual UInt32       Unk_2D(); // 2D
         virtual const char*  GetFullName(UInt32 arg); // 2E
         virtual void         CopyFrom(TESForm* srcForm); // 2F // guessed
         virtual void         Unk_30(UInt32 arg0, UInt32 arg1, UInt32 arg2);
         virtual void         Unk_31(void* dst, UInt32 unk);
         virtual const char*  GetName();            // 32 // gets an actor's short name?
         virtual bool         SetName(const char*); // 33 // used to set a cell's (full?) name when loading/initializing it? // actually editor ID?
         virtual bool         Unk_34();
         virtual bool         Unk_35();
         virtual bool         Unk_36(UInt32 arg);
         virtual bool         HandleActivation(TESObjectREFR* refBeingActivated, TESObjectREFR* actorActivatingRef, UInt32 arg3, UInt32 arg4, UInt32 arg5); // 37 // TESObjectREFR::Activate calls this on the base form of the reference being activated
         virtual void         SetFormID(UInt32 newFormID, bool); // 38
         virtual const char*  Unk_39(); // 39 // This function was inserted into the VTBL after the SKSE team last mapped it.
         virtual bool         Unk_3A(); // 3A // This function was inserted into the VTBL after the SKSE team last mapped it.
         //
         // Based on a look at the VTBL and some errors in SKSE's documentation, it looks like Bethesda patched 
         // two virtual functions halfway into the VTBL -- Unk_22 and Unk_23 -- and the SKSE team didn't notice 
         // it. I've listed Unk_39 and Unk_3A here, but if we ever rename them for TESForm* OR ANY SUBCLASS, then 
         // we'll need to rename them for ALL OF THEM.
         //
         // This also means that calls to anything after Unk_21 on a non-RE pointer are probably going to be bad, 
         // unless they're on a subclass (in which case MAYBE they'll be fine?).

         bool IsWeapon(){ return formType == kFormType_Weapon; }
         bool IsAmmo()  { return formType == kFormType_Ammo; }
         bool IsArmor() { return formType == kFormType_Armor; }

         // Bethesda removed most of the functionality from their code; this reimplements it for simple classes.
         void CopyFromEx(TESForm * rhs);

         void*    unk04;    // 04
         UInt32   flags;    // 08
         UInt32   formID;   // 0C
         UInt16   unk10;    // 10 // more flags
         UInt8    formType;	// 12
         UInt8    pad13;    // 13

         MEMBER_FN_PREFIX(TESForm);
         DEFINE_MEMBER_FN(SetFormFlags_Internal, void, 0x00450A60, UInt32 flagsMask, bool clearOrSet);
   };
   //static DEFINE_SUBROUTINE(::BGSDestructibleObjectForm*, GetBGSDestructibleObjectForm, 0x00448090, RE::TESForm*);
   DEFINE_SUBROUTINE_EXTERN(::BGSKeywordForm*, GetKeywordListFor, 0x0044B2D0, ::TESForm*);
};