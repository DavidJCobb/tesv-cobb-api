#pragma once
#include "ReverseEngineered/Forms/BaseForms/TESObjectACTI.h"
#include "skse/GameForms.h"
#include "skse/GameObjects.h"

class DisplayOptionService {
   private:
      SimpleLock lock;
      //
      struct {
         struct {
            TESObjectACTI* normal = NULL;
            TESObjectACTI* large = NULL;
         } gizmos;
         struct {
            TESEffectShader* selection = NULL;
            TESEffectShader* snap = NULL;
         } shaders;
         struct {
            TESGlobal* gizmoOption = NULL;
            TESGlobal* shaderOption = NULL;
         } globals;
      } forms;
      //
      struct OriginalShaderState {
         bool   cached = false;
         UInt32 edgeColor = 0;
         UInt32 fillColorKey1 = 0;
         UInt32 fillColorKey2 = 0;
         UInt32 fillColorKey3 = 0;
      };
      struct {
         struct {
            OriginalShaderState selection;
            OriginalShaderState snap;
         } shaders;
      } originalFormStates;
      //
      void _CacheShaderState(TESEffectShader*, OriginalShaderState*);
      void _RestoreShaderState(TESEffectShader*, OriginalShaderState*) const;
      //
      enum Date {
         kDate_None = 0,
         kDate_Halloween = 1,
         kDate_TransAwareness = 2,
      };
      //
      const Date GetDate() const;
      //
      bool GetModel(TESForm*, std::string* out) const;
      bool SetModel(TESForm*, const std::string) const;
   public:
      static DisplayOptionService& GetInstance() {
         static DisplayOptionService instance;
         return instance;
      };
      enum GizmoOptions : UInt32 {
         kGizmoOption_Occasions            = 0,
         kGizmoOption_StandardOnly         = 1,
         kGizmoOption_ColorblindProtanopia = 2,
         kGizmoOption_ColorblindTritanopia = 3,
         kGizmoOption_OutlinedLabeled      = 4,
         kGizmoOption_OutlinedLabeledWhite = 5,
      };
      enum ShaderOptions : UInt32 {
         kShaderOption_Occasions    = 0,
         kShaderOption_StandardOnly = 1,
         kShaderOption_ColorblindProtanopia = 2, // not used; green and grey should have enough contrast? reserved in case users tell me otherwise
         kShaderOption_ColorblindTritanopia = 3,
      };
      //
      void SetGizmoForms(TESObjectACTI* normal, TESObjectACTI* large);
      void SetShaderForms(TESEffectShader* selection, TESEffectShader* snap);
      void SetGizmoGlobal(TESGlobal*);
      void SetShaderGlobal(TESGlobal*);
      //
      void UpdateForms(); // uses lock
};