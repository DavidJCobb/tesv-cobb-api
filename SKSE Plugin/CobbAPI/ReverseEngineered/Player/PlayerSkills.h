#pragma once
#include "skse/GameFormComponents.h"

namespace RE {
   class PlayerSkills {
      public:
         enum {
            kAdvanceableSkillOffset = 6,
            kNumAdvanceableSkills = 18
         };

         struct StatData {
            struct LevelData {
               float level;		// 00
               float points;		// 04
               float pointsMax;	// 08
            };
            //
            float		levelPoints;							// 00
            float		levelPointsMax;							// 04
            LevelData	levelData[kNumAdvanceableSkills];		// 08
            UInt32		legendaryLevel[kNumAdvanceableSkills];	// 90
         };

         StatData * data; // 00

         SInt32 ResolveAdvanceableSkillId(SInt32 actorValue);

         float GetSkillPoints(BSFixedString actorValue);
         void SetSkillPoints(BSFixedString actorValue, float points);

         SInt32 GetSkillLegendaryLevel(BSFixedString actorValue);
         void SetSkillLegendaryLevel(BSFixedString actorValue, UInt32 level);

         MEMBER_FN_PREFIX(PlayerSkills);
         DEFINE_MEMBER_FN(GetSkillData,        UInt32, 0x00760150, UInt32 actorValue, float* level, float* points, float* pointsMax, UInt32* unk6);
         DEFINE_MEMBER_FN(IncrementLegendary,  UInt32, 0x00760110, UInt32 actorValue);
         DEFINE_MEMBER_FN(PenalizeForJailTime, void,   0x007601E0, UInt32 bountyGold);
         DEFINE_MEMBER_FN(SetLevel,            void,   0x00760900, UInt32 level);
   };
};