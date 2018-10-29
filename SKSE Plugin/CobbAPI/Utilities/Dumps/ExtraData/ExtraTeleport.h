#pragma once
#include "ReverseEngineered/ExtraData.h"
#include "skse/GameReferences.h"

void CobbDump(RE::ExtraTeleport* extra, const char* indent) {
   if (indent == NULL)
      indent = "";
   _MESSAGE("%sFriendly Name: ExtraTeleport", indent);
   if (extra->data) {
      {  // Echo reference handle and target reference
         UInt32 handle = extra->data->refHandle;
         TESObjectREFR* reference = NULL;
         LookupREFRByHandle(&handle, &reference);
         if (reference) {
            _MESSAGE("%sDestination Door: REFR:%08X", indent, reference->formID);
            skyrim_re_clear_refr(reference);
         } else {
            _MESSAGE("%sDestination Door: None (bad handle: %08X)", indent, extra->data->refHandle);
         }
      }
      NiPoint3* pos = &extra->data->markerPosition;
      NiPoint3* rot = &extra->data->markerRotation;
      _MESSAGE("%sPosition: (%f, %f, %f)", indent, pos->x, pos->y, pos->z);
      _MESSAGE("%sRotation: (%f, %f, %f)", indent, rot->x, rot->y, rot->z);
   } else {
      _MESSAGE("%sNo Data!", indent);
   }
};