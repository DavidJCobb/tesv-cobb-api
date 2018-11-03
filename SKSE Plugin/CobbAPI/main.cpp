#include "Shared.h" // So we can share them to files that need them.

#define COBB_NAVCUT_ENABLED 0

#include "skse/PluginAPI.h"		// super
#include "skse/skse_version.h"	// What version of SKSE is running?
#include "skse/SafeWrite.h"
#include "skse/GameAPI.h"
#include <shlobj.h>				// CSIDL_MYCODUMENTS
#include <psapi.h>  // MODULEINFO, GetModuleInformation
#pragma comment( lib, "psapi.lib" ) // needed for PSAPI to link properly
#include <string>

#include "Patches/Exploratory.h"
#include "Patches/CellDefaultData.h"
#include "Patches/SetMotionType.h"
#include "Patches/MessageQuitGame.h"
//#include "Patches/FormRestoreState.h" // started in CobbPos 1.6, but not yet ready
#include "Patches/PlaceableCollisionPrimitives.h"

#include "Services/Messaging.h"
#include "Services/CellInteriorDataService.h"
#include "Services/DisplayOptionService.h"
#include "Services/RevealService.h"
#include "Services/TeleportMarkerService.h"
#include "Services/WeakReferenceService.h"

#include "Papyrus/Actor.h"
#include "Papyrus/Array.h"
#include "Papyrus/Cell.h"
#include "Papyrus/Detection.h"
#include "Papyrus/Editor.h"
#include "Papyrus/Form.h"
#include "Papyrus/Furniture.h"
#include "Papyrus/Game.h"
#include "Papyrus/Light.h"
#include "Papyrus/Message.h"
#include "Papyrus/ObjectReference.h"
#include "Papyrus/RevealService.h"
#include "Papyrus/Rotations.h"
#include "Papyrus/Shout.h"
#include "Papyrus/SimpleSearches.h"
#include "Papyrus/String.h"
#include "Papyrus/Utility.h"
#include "Papyrus/WeakReference.h" // 1.4.2 (unfinished; maybe won't ever be finished)
#include "Papyrus/Worldspace.h"
#include "Papyrus/CompoundOperations/CellSearcher.h"
#include "Papyrus/CompoundOperations/Move.h"
#include "Papyrus/CompoundOperations/MoveGroup.h"
#include "Papyrus/CompoundOperations/MoveRel.h"
#include "Papyrus/CompoundOperations/SetMotionType.h" // 1.4.2
#include "Papyrus/CompoundOperations/SpawnComplex.h"
#include "Papyrus/CompoundOperations/SpawnRel.h"
#ifdef _DEBUG
#include "Papyrus/SKSEDebug.h"
#endif
#ifndef _DEBUG
#ifdef FORCEDEBUG
#include "Papyrus/SKSEDebug.h"
#endif
#endif

static UInt32 g_pluginSerializationSignature = 'cAPI'; // CONFIRM THIS WITH THE SKSE TEAM

PluginHandle			    g_pluginHandle = kPluginHandle_Invalid;
SKSEPapyrusInterface*       g_papyrus = nullptr;
SKSEMessagingInterface*     g_ISKSEMessaging     = nullptr;
SKSESerializationInterface* g_ISKSESerialization = nullptr;
SKSETaskInterface*          g_ISKSETask          = nullptr;

void Callback_Messaging_SKSE(SKSEMessagingInterface::Message* message);
void Callback_Messaging_Cobb(SKSEMessagingInterface::Message* message);
void Callback_Serialization_Save(SKSESerializationInterface * intfc);
void Callback_Serialization_Load(SKSESerializationInterface * intfc);
void Callback_Serialization_FormDelete(UInt64 handle);

static const char* g_pluginName = "CobbAPI";

static bool g_TESVVersionSupported = false;
static bool g_SKSEVersionSupported = false;

extern "C" {
   //
   // SKSEPlugin_Query: Called by SKSE to learn about this plug-in and check that it's safe to load.
   //
   bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info) {
      gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim\\SKSE\\CobbAPI.log");
      gLog.SetPrintLevel(IDebugLog::kLevel_Error);
      gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
      //
      _MESSAGE("Query");
      //
      // Populate info structure.
      //
      // Plug-in versions:
      //   1 = 1.0
      //   2 = 1.2
      //   3 = 1.3
      //   4 = 1.4
      //   5 = 1.4.1 // NUMBERING SCHEME DIVERGES HERE
      //
      // 0xAABBCCDD = AA.BB.CC.DD with values converted to decimal // major.minor.update.internal-build-or-zero
      //
      info->infoVersion = PluginInfo::kInfoVersion;
      info->name        = g_pluginName;
      info->version     = 0x00000000;
      {  // Log version number
         auto  v     = info->version;
         UInt8 main  = v >> 0x18;
         UInt8 major = v >> 0x10;
         UInt8 minor = v >> 0x08;
         _MESSAGE("Current version is %d.%d.%d.", main, major, minor);
      }
      {  // Get run-time information
         HMODULE    baseAddr = GetModuleHandle("CobbAPI"); // DLL filename
         MODULEINFO info;
         if (baseAddr && GetModuleInformation(GetCurrentProcess(), baseAddr, &info, sizeof(info)))
            _MESSAGE("We're loaded to the span of memory at %08X - %08X.", info.lpBaseOfDll, (UInt32)info.lpBaseOfDll + info.SizeOfImage);
      }
      //
      // Store plugin handle so we can identify ourselves later.
      //
      g_pluginHandle = skse->GetPluginHandle();
      //
      g_TESVVersionSupported = (skse->runtimeVersion == RUNTIME_VERSION_1_9_32_0);
      g_SKSEVersionSupported = (skse->skseVersion >= 0x01070300); // 1.7.3.0
      //
      if (skse->isEditor) {
         _MESSAGE("CobbPos was loaded in the Creation Kit. Marking as incompatible.");
         return false;
      } else if (!g_TESVVersionSupported) {
         _MESSAGE("Unsupported Skyrim version: %08X.", skse->runtimeVersion);
         return false;
      }
      {  // Get the object interface.
         g_ISKSETask = (SKSETaskInterface *)skse->QueryInterface(kInterface_Task);
         if (!g_ISKSETask) {
            _MESSAGE("Couldn't get task interface.");
            return false;
         } else if (g_ISKSETask->interfaceVersion < SKSETaskInterface::kInterfaceVersion) {
            _MESSAGE("Task interface too old (%d; we expected %d).", g_ISKSETask->interfaceVersion, SKSETaskInterface::kInterfaceVersion);
            return false;
         }
      }
      {  // Get the messaging interface and query its version.
         g_ISKSEMessaging = (SKSEMessagingInterface *)skse->QueryInterface(kInterface_Messaging);
         if (!g_ISKSEMessaging) {
            _MESSAGE("Couldn't get messaging interface.");
            return false;
         } else if (g_ISKSEMessaging->interfaceVersion < SKSEMessagingInterface::kInterfaceVersion) {
            _MESSAGE("Messaging interface too old (%d; we expected %d).", g_ISKSEMessaging->interfaceVersion, SKSEMessagingInterface::kInterfaceVersion);
            return false;
         }
      }
      {  // Get the serialization interface and query its version.
         g_ISKSESerialization = (SKSESerializationInterface *)skse->QueryInterface(kInterface_Serialization);
         if (!g_ISKSESerialization) {
            _MESSAGE("Couldn't get serialization interface.");
            return false;
         } else if (g_ISKSESerialization->version < SKSESerializationInterface::kVersion) {
            _MESSAGE("Serialization interface too old (%d; we expected %d).", g_ISKSESerialization->version, SKSESerializationInterface::kVersion);
            return false;
         }
      }
      //
      // This plug-in supports the current Skyrim and SKSE versions:
      //
      return true;
   }
   //
   // SKSEPlugin_Load: Called by SKSE to load this plug-in.
   //
   void _RegisterAndEchoPapyrus(SKSEPapyrusInterface::RegisterFunctions callback, char* module) {
      bool status = g_papyrus->Register(callback);
      if (status)
         _MESSAGE("Papyrus registration %s for %s.", "succeeded", module);
      else
         _MESSAGE("Papyrus registration %s for %s.", "FAILED",    module);
   };
   bool SKSEPlugin_Load(const SKSEInterface* skse)	{
      _MESSAGE("Load.");
      //
      {  // Patches
         CobbPatches::Exploratory::Apply();
         CobbPatches::CellDefaultData::Apply();
         CobbPatches::MessageQuitGame::Apply();
         //CobbPatches::FormRestoreState::Apply(); // started in 1.6, but not yet ready
         #if COBB_NAVCUT_ENABLED == 1
            CobbPatches::PlaceableCollisionPrimitives::Apply(); // 2.0.5
         #endif
         CobbPatches::SetMotionType::Apply();
      }
      {  // Messaging callbacks.
         g_ISKSEMessaging->RegisterListener(g_pluginHandle, "SKSE", Callback_Messaging_SKSE);
         //
         // NOTE: callbacks from other plugins (or ourselves) must be registered in SKSE's post-load event.
         //
         //g_ISKSEMessaging->RegisterListener(g_pluginHandle, "CobbPos", Callback_Messaging_Cobb);
         //g_ISKSEMessaging->RegisterListener(g_pluginHandle, "CobbAPI", Callback_Messaging_Cobb); // maybe in the future
      }
      {  // Serialization callbacks.
         g_ISKSESerialization->SetUniqueID(g_pluginHandle, g_pluginSerializationSignature);
         //g_ISKSESerialization->SetRevertCallback  (g_pluginHandle, Callback_Serialization_Revert);
         g_ISKSESerialization->SetSaveCallback      (g_pluginHandle, Callback_Serialization_Save);
         g_ISKSESerialization->SetLoadCallback      (g_pluginHandle, Callback_Serialization_Load);
         g_ISKSESerialization->SetFormDeleteCallback(g_pluginHandle, Callback_Serialization_FormDelete); // 1.5
      }
      //
      // Register Papyrus APIs:
      //
      g_papyrus = (SKSEPapyrusInterface*) skse->QueryInterface(kInterface_Papyrus);
      _MESSAGE("CobbPos: Verifying that my Papyrus prefix was compiled properly: " PapyrusPrefixString("TEST"));
      //-----------------------------------------------------------------------------------------------
      //   UTILITIES:
      //-----------------------------------------------------------------------------------------------
      _RegisterAndEchoPapyrus(CobbPapyrus::Array::Register,          "Array");
      _RegisterAndEchoPapyrus(CobbPapyrus::Detection::Register,      "Detection");
      _RegisterAndEchoPapyrus(CobbPapyrus::Game::Register,           "Game");
      _RegisterAndEchoPapyrus(CobbPapyrus::Rotations::Register,      "Rotations");
      _RegisterAndEchoPapyrus(CobbPapyrus::SimpleSearches::Register, "SimpleSearches");
      _RegisterAndEchoPapyrus(CobbPapyrus::String::Register,         "String");
      _RegisterAndEchoPapyrus(CobbPapyrus::Utility::Register,        "Utility");
      _RegisterAndEchoPapyrus(CobbPapyrus::WeakReference::Register,  "WeakReference");
      //-----------------------------------------------------------------------------------------------
      //   EXTENSIONS TO NATIVE CLASSES:
      //-----------------------------------------------------------------------------------------------
      _RegisterAndEchoPapyrus(CobbPapyrus::Actor::Register,           "Actor");
      _RegisterAndEchoPapyrus(CobbPapyrus::Cell::Register,            "Cell");
      _RegisterAndEchoPapyrus(CobbPapyrus::Form::Register,            "Form");
      _RegisterAndEchoPapyrus(CobbPapyrus::Furniture::Register,       "Furniture");
      _RegisterAndEchoPapyrus(CobbPapyrus::Light::Register,           "Light");
      _RegisterAndEchoPapyrus(CobbPapyrus::Message::Register,         "Message");
      _RegisterAndEchoPapyrus(CobbPapyrus::ObjectReference::Register, "ObjectReference");
      _RegisterAndEchoPapyrus(CobbPapyrus::Shout::Register,           "Shout");
      _RegisterAndEchoPapyrus(CobbPapyrus::Worldspace::Register,      "Worldspace");
      //-----------------------------------------------------------------------------------------------
      //   BATCH OPERATIONS:
      //-----------------------------------------------------------------------------------------------
      _RegisterAndEchoPapyrus(CobbPapyrus::BatchMove::Register,          "BatchMove");
      _RegisterAndEchoPapyrus(CobbPapyrus::BatchMoveGroup::Register,     "BatchMoveGroup");
      _RegisterAndEchoPapyrus(CobbPapyrus::BatchMoveRel::Register,       "BatchMoveRel");
      _RegisterAndEchoPapyrus(CobbPapyrus::BatchSpawnComplex::Register,  "BatchSpawnComplex");
      _RegisterAndEchoPapyrus(CobbPapyrus::BatchSpawnRel::Register,      "BatchSpawnRel");
      _RegisterAndEchoPapyrus(CobbPapyrus::BatchSetMotionType::Register, "BatchSetMotionTpye");
      //-----------------------------------------------------------------------------------------------
      //   COMPLEX OPERATIONS:
      //-----------------------------------------------------------------------------------------------
      _RegisterAndEchoPapyrus(CobbPapyrus::CellSearcher::Register, "CellSearcher");
      //-----------------------------------------------------------------------------------------------
      //   SERVICES:
      //-----------------------------------------------------------------------------------------------
      _RegisterAndEchoPapyrus(CobbPapyrus::Editor::Register,        "Editor"); // generic methods for CobbPos
      _RegisterAndEchoPapyrus(CobbPapyrus::RevealService::Register, "RevealService");
      //-----------------------------------------------------------------------------------------------
      //   PLUG-IN DEBUGGING:
      //-----------------------------------------------------------------------------------------------
      #ifdef _DEBUG
         _RegisterAndEchoPapyrus(PapyrusPrefix(Papyrus)::SKSEDebug::Register, "SKSEDebug");
         _MESSAGE("\n\nWARNING: If this is a public release, notify the mod author -- they shipped the wrong build!\n\n");
      #endif
      #ifndef _DEBUG
      #ifdef FORCEDEBUG
         _RegisterAndEchoPapyrus(PapyrusPrefix(Papyrus)::SKSEDebug::Register, "SKSEDebug");
         _MESSAGE("\n\nWARNING: If this is a public release, notify the mod author -- they shipped a build with 'force debug' enabled!\n\n");
      #endif
      #endif
      //-----------------------------------------------------------------------------------------------
      return true;
   }
};

void Callback_Messaging_SKSE(SKSEMessagingInterface::Message* message) {
   DEBUG_ONLY_MESSAGE("Received message from SKSE. Type is 0x%08X.", message->type);
   if (message->type == SKSEMessagingInterface::kMessage_PostLoad) {
      DEBUG_ONLY_MESSAGE("Message from SKSE is of type: kMessage_PostLoad.");
      {  // "New Session" hook: register for message from ourselves and from any other DLLs we make that need the hook:
         g_ISKSEMessaging->RegisterListener(g_pluginHandle, "CobbPos", Callback_Messaging_Cobb);
         g_ISKSEMessaging->RegisterListener(g_pluginHandle, "CobbAPI", Callback_Messaging_Cobb); // maybe in the future
      }
   } else if (message->type == SKSEMessagingInterface::kMessage_PostPostLoad) {
      //
   } else if (message->type == SKSEMessagingInterface::kMessage_DataLoaded) {
      CobbPatches::PlaceableCollisionPrimitives::OnFormsLoaded();
   } else if (message->type == SKSEMessagingInterface::kMessage_NewGame) {
      DEBUG_ONLY_MESSAGE("Message from SKSE is of type: kMessage_NewGame.");
      CellInteriorDataService::GetInstance().ResetAll();
      WeakReferenceService::GetInstance().NewGame();
      TeleportMarkerService::GetInstance().NewGame();
      RevealService::GetInstance().MassReset();
   } else if (message->type == SKSEMessagingInterface::kMessage_PreLoadGame) {
      CellInteriorDataService::GetInstance().ResetAll();
      RevealService::GetInstance().MassReset();
   }
};
void Callback_Messaging_Cobb(SKSEMessagingInterface::Message* message) {
   if (message->type == 'EvCh') { // just an event name
      DEBUG_ONLY_MESSAGE("Received message from CobbAPI or from self. Message is an event name.");
      std::string event((char*)message->data, message->dataLen);
      DEBUG_ONLY_MESSAGE("   Event name is: %s", event.c_str());
      if (event.compare(0, std::string::npos, "quit game", message->dataLen) == 0) { // event == "before new session" should work, but it doesn't
         CellInteriorDataService::GetInstance().ResetAll();
         TeleportMarkerService::GetInstance().QuitGame();
      }
      return;
   } else if (message->type == CobbAPIMessage::skseMessageType) { // 'cAPI'
      auto m = (CobbAPIMessage*) message->data;
      if (message->sender) {
         if (strcmp(message->sender, g_pluginName) == 0) // ignore messages from ourselves
            return;
      }
      if (!m || !m->is_safe())
         return;
      switch (m->type()) {
         case RevealServiceMessage::typeCode:
            m->acknowledge();
            RevealService::GetInstance().OnMessage(m);
            break;
         case TeleportMarkerMessage::typeCode:
            m->acknowledge();
            TeleportMarkerService::GetInstance().OnMessage(m);
            break;
      }
      return;
   }
   DEBUG_ONLY_MESSAGE("Received message from CobbAPI or from self. Type is 0x%08X.", message->type);
};

void Callback_Serialization_FormDelete(UInt64 handle) {
   //
   // This callback actually fires whenever a form is deleted from the Papyrus VM. This can 
   // happen for reasons besides actual deletion; for example, on startup, we get TONS of 
   // calls for leveled NPCs, unique NPCs, merchant chests, and even quests. It's really 
   // only safe to handle "deletion" callbacks for temporary forms.
   //
   //_MESSAGE("CobbPos notified of form deletion. Handle: 0x%16X", handle);
   UInt32 argUnknown = handle & 0xFFFFFFFF00000000;
   UInt32 argFormID  = handle & 0x00000000FFFFFFFF;
   //
   /*if (argFormID & 0xFF000000 == 0xFF000000)*/ { // Temporary form deleted.
      WeakReferenceService& service = WeakReferenceService::GetInstance();
      service.FormDeletedHandler(argFormID);
   }
};
void Callback_Serialization_Save(SKSESerializationInterface* intfc) {
   _MESSAGE("Saving...");

   //SaveClassHelper(intfc, 'COMG', StorableObjectRegistryInstance());
   if (intfc->OpenRecord('COMG', StorablePersistentObjectStorage::kSaveVersion)) {
      _MESSAGE("Saving object storage...");
      StorableObjectStorageInstance().Save(intfc);
   } else
      _MESSAGE("Couldn't save our object storage. Record didn't open.");
   if (intfc->OpenRecord('CLIN', CellInteriorDataService::kSaveVersion)) {
      _MESSAGE("Saving cell interior storage...");
      CellInteriorDataService::GetInstance().Save(intfc);
   } else
      _MESSAGE("Couldn't save our cell interior storage. Record didn't open.");
   {  // Save weak references.
      WeakReferenceService& service = WeakReferenceService::GetInstance();
      if (!service.IsEmpty()) {
         if (intfc->OpenRecord('CWRS', WeakReferenceService::kSaveVersion)) {
            _MESSAGE("Saving weak references...");
            WeakReferenceService::GetInstance().Save(intfc);
         } else
            _MESSAGE("Couldn't save weak references. Record didn't open.");
      }
   }
   {  // Save teleport marker changes.
      TeleportMarkerService& service = TeleportMarkerService::GetInstance();
      if (!service.IsEmpty()) {
         if (intfc->OpenRecord('CTMS', TeleportMarkerService::kSaveVersion)) {
            _MESSAGE("Saving teleport marker changes...");
            TeleportMarkerService::GetInstance().Save(intfc);
         } else
            _MESSAGE("Couldn't save teleport marker changes. Record didn't open.");
      }
   }
   _MESSAGE("Saving done!");
}
void Callback_Serialization_Load(SKSESerializationInterface* intfc) {
   _MESSAGE("Loading...");
   //
   UInt32	type;    // This IS correct. A UInt32 and a four-character ASCII string have the same length (and can be read interchangeably, it seems).
   UInt32	version;
   UInt32	length;
   bool	error = false;
   //
   bool executedTeleportMarkerService = false;
   //
   while (!error && intfc->GetNextRecordInfo(&type, &version, &length)) {
      DEBUG_ONLY_MESSAGE("Serialization: Loaded record of type, version, length: %c%c%c%c, %d, %d.", (char)(type >> 0x18), (char)(type >> 0x10), (char)(type >> 0x8), (char)type, version, length);
      switch (type) {
         case 'COMG':
            DEBUG_ONLY_MESSAGE("Loading object storage...");
            error = !StorableObjectStorageInstance().Load(intfc, version);
            if (error)
               _MESSAGE("Loading object storage FAILED.");
            else
               _MESSAGE("Loaded object storage!");
            break;
         case 'CLIN':
            DEBUG_ONLY_MESSAGE("Loading cell interior changes...");
            error = !CellInteriorDataService::GetInstance().Load(intfc, version);
            if (error)
               _MESSAGE("Loading cell interior changes FAILED.");
            else
               _MESSAGE("Loaded cell interior changes!");
            break;
         case 'CTMS':
            DEBUG_ONLY_MESSAGE("Loading teleport marker changes...");
            error = !TeleportMarkerService::GetInstance().Load(intfc, version);
            if (error)
               _MESSAGE("Loading teleport marker changes FAILED.");
            else
               _MESSAGE("Loaded teleport marker changes!");
            executedTeleportMarkerService = true;
            break;
         case 'CWRS':
            DEBUG_ONLY_MESSAGE("Loading weak references...");
            error = !WeakReferenceService::GetInstance().Load(intfc, version);
            if (error)
               _MESSAGE("Loading weak references FAILED.");
            else
               _MESSAGE("Loaded weak references!");
            break;
         default:
            _MESSAGE("Loading: Unhandled type %c%c%c%c", (char)(type >> 0x18), (char)(type >> 0x10), (char)(type >> 0x8), (char)type);
            error = true;
            break;
      }
   }
   //
   // Make sure that the Teleport Marker Service cleans after itself if we go 
   // from a savegame with changes to a savegame with no changes (i.e. no 
   // serialized data).
   //
   if (!executedTeleportMarkerService) {
      TeleportMarkerService::GetInstance().EmptyLoad();
   }
   //
   _MESSAGE("Loading done!");
}
