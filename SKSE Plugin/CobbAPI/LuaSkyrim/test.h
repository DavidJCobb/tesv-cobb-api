#pragma once

namespace LuaSkyrim {
   void Test();

   //
   // NOTES:
   //
   // TO GET LUA WORKING IN VISUAL STUDIO:
   //
   //  - "Console Application" project; select "static library" in the wizard
   //
   //  - Change compile options from "Multi-Threaded DLL" to "Multi-Threaded" 
   //    (no, Visual Studio doesn't do this for you; were you expecting it to 
   //    be helpful?). This is in C/C++ -> Code Generation -> "Runtime Library" 
   //    pref.
   //
   //  - Paste source files into project -- REMEMBER TO ENABLE THEM IN VS
   //
   //     - Disable lua.c and luac.c; they're just for command-line testing.
   //
   //  - Edit including project (e.g. CobbAPI): add a "reference" to the Lua 
   //    project.
   //
   //     - Include "lua.hpp" to use Lua.
   //
   //  - Modify Lua project to compile as "C" instead of "Default." This is in 
   //    C/C++ -> Advanced -> "Compile As" pref.
   //
   //  - Modify Lua project: turn off Precompiled Headers.
   //
   // TROUBLESHOOTING:
   //
   //  - ERRORS FROM CSTDLIB: The Lua project isn't compiling as C. Make sure 
   //    you aren't including any files besides "lua.hpp," and make sure that 
   //    the project is configured properly.
   //
   //  - CANNOT OPEN SOURCE FILE: Double-check that the Lua source files are 
   //    enabled. It's easy to forget that after copying them into the project.
   //
   //  - UNRESOLVED EXTERNAL SYMBOL __IMP__FREOPEN: The Lua project needs to 
   //    have its multi-threading option changed.
   //
}