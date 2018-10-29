#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Types.h"

#include "skse/GameBSExtraData.h"
#include "Miscellaneous/memory.h"

namespace RE {
   static const UInt32 VTBL_ExtraCollisionData = 0x010794B8;
   static const UInt32 VTBL_ExtraLightData     = 0x010794D8;
   static const UInt32 VTBL_ExtraPrimitive     = 0x01079128;
   static const UInt32 VTBL_ExtraRadius        = 0x01079438;

   ExtraCollisionData* ExtraCollisionData::Create() {
      auto extra    = (ExtraCollisionData*) BSExtraData::Create(sizeof(ExtraCollisionData), VTBL_ExtraCollisionData);
      ExtraCollisionData::Data* contents = (ExtraCollisionData::Data*) cobb::skyrim::heap_alloc(sizeof(ExtraCollisionData::Data));
      extra->data   = contents;
      return extra;
   };
   UInt8 InventoryEntryData::GetSoulSize() const {
      auto form = this->type;
      if (!type || type->formType != kFormType_SoulGem)
         return 0;
      UInt8 size = ((TESSoulGem*)type)->soulSize;
      if (size)
         return size;
      if (!this->extendDataList)
         return 0;
      auto extra = &((RE::tList<RE::BaseExtraList>*)this->extendDataList)->items;
      for (; extra; extra = extra->next) {
         auto list = extra->data;
         if (!list)
            continue;
         size = CALL_MEMBER_FN(list, GetExtraSoulSize)();
         if (size)
            return size;
      }
      return 0;
   };
   ExtraLightData*   ExtraLightData::Create() {
      auto data = (ExtraLightData*) BSExtraData::Create(sizeof(ExtraLightData), VTBL_ExtraLightData);
      {
         data->modifiedFOV = 0;
         data->modifiedFade = 0;
         data->unk10 = 0;
         data->depthBias = 0;
         data->unk18 = 0;
         data->pad19[0] = 0;
         data->pad19[1] = 0;
         data->pad19[2] = 0;
      }
      return data;
   };
   ExtraPrimitive* ExtraPrimitive::Create() {
      auto data = (ExtraPrimitive*) BSExtraData::Create(sizeof(ExtraPrimitive), VTBL_ExtraPrimitive);
      return data;
   };
   ExtraRadius*      ExtraRadius::Create() {
      auto data = (ExtraRadius*) BSExtraData::Create(sizeof(ExtraRadius), VTBL_ExtraRadius);
      {
         data->radiusModifier = 0.0;
      }
      return data;
   };
};