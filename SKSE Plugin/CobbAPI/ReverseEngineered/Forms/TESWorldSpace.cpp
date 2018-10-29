#include "TESWorldSpace.h"
#include "TESObjectCELL.h"
#include "TESObjectREFR.h"
#include "HardcodedForms.h"
#include "ReverseEngineered/Systems/GameData.h"
#include "ReverseEngineered/Types.h"

namespace RE {
   void TESWorldSpace::GetAllRelatedWorldspaces(std::vector<TESWorldSpace*>& out) {
      out.clear();
      out.push_back(this);
      //
      auto myRoot = this->GetRootWorldspace();
      auto data = TESDataHandler::GetSingleton();
      if (!data)
         return;
      auto& worlds = data->worldSpaces;
      for (UInt32 i = 0; i < worlds.count; i++) {
         auto current = worlds.arr.entries[i];
         if (!current || current == this)
            continue;
         if (current->GetRootWorldspace() == myRoot)
            out.push_back(current);
      }
   }
   void TESWorldSpace::ForEachMapMarker(MapMarkerIteratorCallback func, void* state) {
      std::vector<TESWorldSpace*> worlds;
      this->GetAllRelatedWorldspaces(worlds);
      for (auto it = worlds.begin(); it != worlds.end(); ++it) {
         auto cell = (*it)->persistentCell;
         if (cell) {
            RE::simple_lock_guard scopedLock(cell->cellRefLock);
            auto& list = cell->objectList;
            for (size_t i = 0; i < list.count; i++) {
               auto ref = RE::refr_ptr(list.arr.entries[i]);
               if (!ref)
                  continue;
               if (ref->baseForm != (RE::TESForm*) *RE::ptrMapMarkerForm)
                  continue;
               auto extra = CALL_MEMBER_FN((RE::BaseExtraList*) &ref->extraData, GetExtraMapMarkerData)();
               if (!extra)
                  continue;
               if (!func(this, ref, extra, state))
                  break;
            }
         }
      }
   };
};