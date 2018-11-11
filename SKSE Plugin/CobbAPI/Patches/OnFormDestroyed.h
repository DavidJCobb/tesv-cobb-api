#pragma once

//
// This set of hooks was experimental and is, as of this writing, non-functional.
//
//  - They do not detect deletions caused by MarkForDelete.
//
//  - They fire false-positives on created references left in cells that have 
//    reset or unloaded.
//
// Do not use this hook. The code and files are kept only for reference's sake, 
// in case I can salvage something from them.
//
namespace CobbPatches {
   namespace OnFormDestroyed {
      void Apply();
   };
};