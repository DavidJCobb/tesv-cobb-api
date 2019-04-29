#define lua_lock(L) LuaLock(L)
#define lua_unlock(L) LuaUnlock(L)

struct lua_State;
#ifdef __cplusplus
extern "C" {
#endif
   extern void LuaLock(lua_State* L);
   extern void LuaUnlock(lua_State* L);
#ifdef __cplusplus
}
#endif