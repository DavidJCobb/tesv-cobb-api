#include <windows.h>
extern "C" {
   #include "lua.h"
   #include "luauser.h"
}

//
// In Visual Studio, this specific file must be set to compile as C++. 
// To do that in a project where the default is C, right-click the file 
// in the Solution Explorer and edit its Properties. You want "Compile 
// As" under the "Advanced" section of the C/C++ properties.
//
// VS doesn't care about the C/CPP file extensions, apparently.
//

class LuaLockState {
   protected:
      CRITICAL_SECTION section;
   public:
      LuaLockState() {
         InitializeCriticalSection(&this->section);
      };
      ~LuaLockState() {
         DeleteCriticalSection(&this->section);
      }
      void lock() {
         EnterCriticalSection(&this->section);
      }
      void unlock() {
         LeaveCriticalSection(&this->section);
      }
} s_LuaLockState;

extern "C" {
   void LuaLock(lua_State * L) {
      s_LuaLockState.lock();
   }
   void LuaUnlock(lua_State * L) {
      s_LuaLockState.unlock();
   }
}