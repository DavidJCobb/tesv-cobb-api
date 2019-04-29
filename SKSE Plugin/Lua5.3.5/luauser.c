// as recommended by http://lua-users.org/wiki/ThreadsTutorial

#include <windows.h>
#include "lua.h"
#include "luauser.h"

static struct {
   CRITICAL_SECTION LockSct;
   BOOL  Init;
} Gl;

static CobbDebugBadThreadHandler s_cdbth;
void CobbDebugSetBadThreadHandler(CobbDebugBadThreadHandler a) {
   s_cdbth = a;
}

void LuaLockInitial(lua_State * L)
{
   if (!Gl.Init)
   {
      /* Create a mutex */
      InitializeCriticalSection(&Gl.LockSct);
      Gl.Init = TRUE;
   }
}

void LuaLockFinal(lua_State * L) /* Not called by Lua. */
{
   /* Destroy a mutex. */
   if (Gl.Init)
   {
      DeleteCriticalSection(&Gl.LockSct);
      Gl.Init = FALSE;
   }
}

void LuaLock(lua_State * L)
{
   if (!Gl.Init) { // accessing an uninitialized critical section -- this will crash!
      if (s_cdbth)
         s_cdbth(GetCurrentThreadId());
   }
   /* Wait for control of mutex */
   EnterCriticalSection(&Gl.LockSct);
}

void LuaUnlock(lua_State * L)
{
   /* Release control of mutex */
   LeaveCriticalSection(&Gl.LockSct);
}