#pragma once
#include "skse/Utilities.h"

class BGSTextureSet;
class NiAVObject;
class SpellItem;
namespace RE {
   enum TaskType : UInt32 {
      kTaskType_RemoveSpell = 0x5A,
   };

   struct BSTask;
   class BSTaskPool {
      public:
         static BSTaskPool* GetSingleton(void) {
            return *((BSTaskPool**)0x01B38308);
         }
         //
         MEMBER_FN_PREFIX(BSTaskPool);
         DEFINE_MEMBER_FN(SetNiGeometryTexture, UInt32, 0x006A4590, NiAVObject* geometry, BGSTextureSet * textureSet);
         DEFINE_MEMBER_FN(ProcessTaskQueue_HookTarget, void, 0x006A0B30);
         //
         DEFINE_MEMBER_FN(QueueBSTask,          void, 0x006A2B30, BSTask*);
         DEFINE_MEMBER_FN(QueueRemoveSpellTask, void, 0x006A3EA0, UInt32 actorRefHandle, SpellItem* spell);
   };
   struct BSTask {
      UInt32 type;
   };
   struct RemoveSpellTask : public BSTask {
      inline RemoveSpellTask() {
         this->type = kTaskType_RemoveSpell;
      };
      UInt32     actorRefHandle; // 04
      SpellItem* spell; // 08
   };
};
