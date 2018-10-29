#pragma once

class CobbAPIMessage {
   public:
      static constexpr UInt32 skseMessageType = 'cAPI';
      static constexpr void*  gameModuleBase = (void*) 0x00400000;
   protected:
      char   _sentinel[8] = "CobbAPI";
      UInt32 _type;
   private:
      bool acknowledged = false;
   public:
      CobbAPIMessage(UInt32 t) : _type(t) {};
      inline void acknowledge() { this->acknowledged = true; };
      inline bool is_acknowledged() const { return this->acknowledged; };
      inline bool is_safe() const { // check the sentinel bytes to ensure this is one of our messages
         {  // Ensure that the (this) pointer is actually a pointer.
            //
            // This function is intended to handle the fact that we're receiving 'cAPI' messages from 
            // any SKSE plug-in, and those messages may not always be valid CobbAPIMessages. As such, 
            // we need safety checks on the (this) pointer.
            //
            // We allocate a dummy variable on the stack and then check its address. Stacks allocate 
            // downwards (i.e. {SUB ESP, 8} to allocate two vars), so if we're below our new variable, 
            // then we can't have been allocated on the stack. The stack is usually below the heap, so 
            // this also serves as a safety check for heap allocation as well.
            //
            // This is necessarily far from perfect. If I didn't want to support messages from other 
            // DLLs, then I wouldn't have to resort to this kind of hack. This solution will hold up 
            // for the specific environment it's been designed for: Skyrim Classic SKSE x86 DLLs on a 
            // Windows system.
            //
            void* x;
            if ((void*)this < (&x))
               return false;
         }
         return strcmp(this->_sentinel, "CobbAPI") == 0;
      };
      inline decltype(_type) type() const { return this->_type; };
      void send(const char* target = nullptr);
};

/*//
class SampleMessageSubclass : CobbAPIMessage {
   public:
      static constexpr UInt32 typeCode = 'CODE';
      SampleMessageSubclass() : CobbAPIMessage(typeCode) {};
      //
      // ...
      //
};
//*/