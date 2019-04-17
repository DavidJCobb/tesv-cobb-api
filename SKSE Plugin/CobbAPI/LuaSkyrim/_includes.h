#pragma once
#include "Lua5.3.5/lua.hpp"

//
// Lua CFunctions should push their Lua-side return values onto the stack, 
// and then return the number of return values (or more accurately, the 
// amount by which they have changed the stack size). Typedeffing this 
// isn't strictly necessary; I just prefer to be able to tell at a glance, 
// "no, this isn't, like, a *number*; it's this specific thing for this 
// specific system." Kinda like how errno_t and size_t are things.
//
typedef int luastackchange_t;