#pragma once
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "Utilities/Dumps/ExtraData/Base.h"

bool CobbDumpFormFilter(TESObjectREFR* subject) {
   if (subject && subject->baseForm && subject->baseForm->formType == kFormType_Door) {
      return true;
   }
   return false;
};

void CobbDumpForm(TESObjectREFR* subject) {
   if (CobbDumpFormFilter(subject) == false) {
      return;
   }
   if (subject == NULL) {
      _MESSAGE("Unable to dump TESObjectREFR. It does not exist.");
      return;
   }
   subject->handleRefObject.IncRef();
   //
   _MESSAGE("Dumping TESObjectREFR with form ID 0x%08X...", subject->formID);
   {  // Echo base form details.
      UInt32 formID = 0;
      UInt8  formType = 0;
      if (subject->baseForm) {
         formID = subject->baseForm->formID;
         formType = subject->baseForm->formType;
      }
      _MESSAGE("   Base form: [%02X]%08X", formType, formID);
   }
   _MESSAGE("   Coordinates:");
   _MESSAGE("      Position: %f, %f, %f", subject->pos.x, subject->pos.y, subject->pos.z);
   _MESSAGE("      Rotation: %f, %f, %f", subject->rot.x, subject->rot.y, subject->rot.z);
   {  // Echo parent cell details.
      TESObjectCELL* cell = subject->parentCell;
      if (cell) {
         _MESSAGE("   Parent Cell:");
         _MESSAGE("      Form ID: %08X", cell->formID);
         if (cell->unk2C & 1) {
            _MESSAGE("      Interior.");
         } else {
            try {
               SInt32 x = 0;
               SInt32 y = 0;
               CALL_MEMBER_FN((RE::TESObjectCELL*)cell, GetCellCoordinates)(&x, &y);
               _MESSAGE("      Exterior: %d, %d", x, y);
            } catch (std::exception &e) {
               _MESSAGE("      Exterior: Unknown Coordinates (exception thrown on retrieval)");
            }
         }
      } else {
         _MESSAGE("   Parent Cell: Irretrievable");
      }
   }
   {  // Echo loaded state details.
      if (subject->loadedState) {
         _MESSAGE("   Loaded State:");
         _MESSAGE("      Node Memory Address: %08X", (UInt32)subject->loadedState->node);
      } else {
         _MESSAGE("   Loaded State: None");
      }
   }
   {  // Echo extra data.
      BSExtraData* data = subject->extraData.m_data;
      if (data) {
         _MESSAGE("   Extra Data:");
         for (; data; data = data->next) {
            _MESSAGE("      Data, Type %02X:", data->GetType());
            CobbDump(data, "         ");
         }
      } else {
         _MESSAGE("   Extra Data: None.");
      }
   }
   _MESSAGE("   Reference Flags: %08X", subject->unk50);
   //
   subject->handleRefObject.DecRef();
}