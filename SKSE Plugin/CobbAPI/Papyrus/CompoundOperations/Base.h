#pragma once

#include "Shared.h"
#include "Papyrus/_Prefixes.h"

#include "common/IMemPool.h"
#include "skse/GameThreads.h"

#include "skse/PapyrusVM.h"
#include "skse/Serialization.h"

//
// This base class provides boilerplate functionality for compound Papyrus operations, 
// such as spawning or moving a collection of objects. Create subclasses for your 
// specific needs.
//

namespace PapyrusPrefix(Papyrus) {
   class CompoundOperationBase : public IStorableObject {
      public:
         class Delegate;
      public:
         explicit CompoundOperationBase(UInt32 stackId) : _stackId(stackId) {};
         explicit CompoundOperationBase(StorableSerializationTag tag) : _stackId(0) {};
         //
         // ClassName() must return a unique string per (sub)class.
         //
         virtual const char*  ClassName() const { return PapyrusPrefixString("CompoundOperationBase"); }
         virtual UInt32    ClassVersion() const { return 1; }
         //
         // Override these to handle serialization for subclass-specific data. You do not need to 
         // handle parts of this base class (e.g. the Papyrus stack ID); those are handled for you 
         // by the functions that wrap these methods.
         //
         virtual bool OnSave(SKSESerializationInterface* intfc) { return true; };
         virtual bool OnLoad(SKSESerializationInterface* intfc, UInt32 version) { return true; };
         //
         // OnStart() and OnEnd() allow subclasses to run certain setup procedures before and after 
         // executing their main loops (e.g. working with Bethesda-side locking mechanisms, should 
         // the need ever arise). If OnStart() fails to return true, the operation is canceled.
         //
         // The Run() function should actually carry out the batch operation. Clients should not 
         // call it directly; instead, call Queue() in order to run the batch operation async.
         //
         virtual bool OnStart() { return true; }
         virtual void Run(VMValue& resultValue) {};
         virtual void OnEnd() {};
         //
         // DeleteWhenDone() is used solely within this base class, to determine whether an instance 
         // should be deleted when it finishes running. If your Run method returns everything you 
         // need, you should stick with the default (delete the instance automatically).
         //
         // In some cases, you may wish to run an operation and then have your instance stick around, 
         // so that a Papyrus script can query it for further information; in these cases, you should 
         // override this method to return false, and set things up so that a Papyrus script can 
         // manually delete the instance when it's no longer needed. Refer to BatchSpawnComplex in 
         // /Papyrus/CompoundOperations/SpawnComplex.h for an example.
         //
         virtual volatile bool DeleteWhenDone() const { return true; };
         //
         UInt32 StackId() { return _stackId; };
         bool IsExecuting() { return _isExecuting; };
         //
         bool Queue() { // Queues the operation to run async. Returns false if this is not possible.
            if (_isExecuting)
               return true;
            //
            CompoundOperationBase::Delegate* delegate = CompoundOperationBase::_delegatePool.Allocate();
            if (delegate == NULL)
               return false;
            delegate->func = this;
            //
            g_ISKSETask->AddTask(delegate);
            _isExecuting = true;
            return true;
         };
         //
         // These methods wrap the OnSave and OnLoad methods above, and should not be overridden. They 
         // handle boilerplate serialization, but the Load method also re-queues operations that were 
         // queued when the game was saved and quit. (I'm not sure that actually helps...)
         //
         // I'd like to note that there definitely seems to be a gap, where functors can be lost if 
         // the user saves the game while they're queued. When you execute an SKSE SpawnerTask, the 
         // task functor is removed from object storage and given to SKSE's delay functor manager, 
         // which should handle serialization. However, SKSE's task interface (for plug-ins) doesn't 
         // seem to have similar functionality: we have to take our functors out of object storage 
         // and leave them solely in the custody of task delegates, which don't appear to have any 
         // serialization functionality. It's another bit of instability that we can't code around 
         // because SKSE doesn't offer DFM access; seems all we can do is hope for the best.
         //
         virtual bool Save(SKSESerializationInterface* intfc) {
            using namespace Serialization;
            {
               SInt32 stackId = StackId();
               if (!WriteData(intfc, &stackId))
                  return false;
            }
            {
               bool state = IsExecuting();
               if (!WriteData(intfc, &state))
                  return false;
            }
            return OnSave(intfc);
         };
         virtual bool Load(SKSESerializationInterface* intfc, UInt32 version) {
            using namespace Serialization;
            if (!ReadData(intfc, &this->_stackId))
               return false;
            if (!ReadData(intfc, &this->_isExecuting))
               return false;
            if (!this->OnLoad(intfc, version))
               return false;
            if (this->_isExecuting)
               //
               // This operation was in the queue when the game was saved. Add it to the queue again.
               //
               this->Queue();
            return true;
         };
      protected:
         UInt32 _stackId; // Papyrus call stack ID
         bool _isExecuting = false;
      private:
      public:
         class Delegate : public TaskDelegate {
            public:
               CompoundOperationBase* func = NULL;
               //
               Delegate() {};
               Delegate(CompoundOperationBase* func) { // not sure this ever gets used; seems like pool allocate doesn't allow for passing constructor args
                  this->func = func;
               };
               virtual void Run() {
                  VMValue result;
                  //
                  // We don't bother verifying that func still exists, because if things are hooked up 
                  // properly on the Papyrus side of things, then a batch-operation can only be queued 
                  // or deleted by the call stack that created it, and queuing suspends that stack -- 
                  // it should never be possible for func to be deleted out from under us.
                  //
                  // See the BatchMove subclass and its Papyrus APIs for a sample implementation, over 
                  // in /Papyrus/CompoundOperations/Move.h.
                  //
                  if (func->OnStart()) {
                     func->Run(result);
                     func->OnEnd();
                  }
                  //
                  // Resume the Papyrus stack.
                  //
                  (*g_skyrimVM)->GetClassRegistry()->ResumeStack(func->StackId(), &result);
                  if (func->DeleteWhenDone()) {
                     //
                     // TODO: Is this safe? Will it leave a dangling pointer in our object-storage?
                     //
                     delete func;
                  } else {
                     func->_isExecuting = false;
                     this->func = NULL;
                  }
               };
               virtual void Dispose(void) {
                  _delegatePool.Free(this);
               };
         };
         static IThreadSafeBasicMemPool<Delegate, 40> _delegatePool; // shared among all subclasses!
   };
}
