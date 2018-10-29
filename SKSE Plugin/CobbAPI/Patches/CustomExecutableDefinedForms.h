#pragma once
#include "Shared.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectACTI.h"
#include "ReverseEngineered/Systems/GameData.h"

//
// I just wanna lead with how stupid and bad and very-not-good this idea is.
//
// TESV.exe has a small number of hardcoded forms with hardcoded form IDs. What we're basically doing is adding 
// our own, for reasons I'll get into in a moment. Now, in my case, I have a really convenient form ID scheme 
// that I can use: 0xC0BBxx. Get it? C0BB? COBB? But what we're doing REALLY isn't something that absolutely 
// everyone should get to doing.
//
// So why are we doing this very-bad not-good thing? Well, I want to modify Cobb Positioner so that we can 
// attach functional primitive data to any reference we please. Problem is, the part of the game that actually 
// makes collision primitives WORK only operates on references whose base form is the hardcoded CollisionMarker 
// form. So, I need to patch that code to check for base forms I want to work with.
//
// Thing is: if I make another mod that needs to be able to do this same thing -- or if I ever make a reusable 
// "API" DLL that other mod authors can use -- it would need to apply its own patch, so it and CobbPos would 
// end up fighting over the same hook site. If these mods are all patching to check some internal private list 
// of special base forms, then they'll conflict. However, if they--

//
// Well, that was fuckin' stupid. I didn't reason it out well: there's no HOOK conflict if all mods apply the 
// same patch to the same ASM; but there could be a FORM conflict if they all try to create the form with ID 
// 0x00C0BB00. I don't know if "get form by ID" calls work at the stage this code would run in.
//
// New plan: the hook will still be hardcoded to check for the form with ID 0x00C0BB00; but we'll just use a 
// conventional injected record to actually define that form.
//


//


/*
namespace CobbHardcodedForms {
   static TESObjectACTI* PlaceableCollisionPrimitive = NULL;
};

namespace CobbPatches {
   namespace CustomExecutableDefinedForms {
      namespace PlaceableCollisionPrimitive {
         void Create() {
            if (CobbHardcodedForms::PlaceableCollisionPrimitive)
               return;
            RE::TESObjectACTI* created = (RE::TESObjectACTI*) cobb::skyrim::heap_create_by_vtbl(sizeof(RE::TESObjectACTI), RE::TESObjectACTI::vtbl);
            CALL_MEMBER_FN(created, Constructor)();
            created->SetFormID(0x00C0BB00, true);
            created->SetName("CobbHardcoded_PlaceableNavcutPrimitive");
            created->SetFlag00000002(0);
            //
            CobbHardcodedForms::PlaceableCollisionPrimitive = (TESObjectACTI*) created;
            CALL_MEMBER_FN(RE::DataHandler::GetSingleton(), Subroutine0043FEC0)(created);
         };
      };

      __declspec(naked) void Patch() {
         __asm {
            //
            // Reproduce patched-over call:
            //
            mov eax, 0x005AD830;
            call eax;
            //
            // Custom code:
            //
            call PlaceableCollisionPrimitive::Create;
            //
            // Return.
            //
            retn;
         };
      };
   }
};

*/