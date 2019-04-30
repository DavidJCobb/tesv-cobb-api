#pragma once

namespace cobb {
   class scoped_no_recurse {
      //
      // Consider this:
      //
      //    void myFunc() {
      //       static bool s_noRecurse = false;
      //       if (s_noRecurse)
      //          return;
      //       s_noRecurse = true;
      //       //
      //       // Do something that might cause this function to execute 
      //       // as a side-effect...
      //       //
      //       s_noRecurse = false;
      //    }
      //
      // If (myFunc) can return at multiple points during whatever process 
      // it performs, then you have to clear s_noRecurse on every return 
      // statement. Aside from being ugly, this also introduces room for 
      // error.
      //
      // scoped_no_recurse is a structure similar to std::lock_gaurd, 
      // intended to address this problem.
      //
      protected:
         bool& flag;
      public:
         inline scoped_no_recurse(bool& b) : flag(b) {
            if (this->flag)
               return;
            this->flag = true;
         };
         inline ~scoped_no_recurse() {
            this->flag = false;
         };
   };
}