#include "Messaging.h"
#include "Shared.h"

void CobbAPIMessage::send(const char* target) {
   if (!g_ISKSEMessaging)
      return;
   g_ISKSEMessaging->Dispatch(g_pluginHandle, CobbAPIMessage::skseMessageType, this, sizeof(this), target);
};