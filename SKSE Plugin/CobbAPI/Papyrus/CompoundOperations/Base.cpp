#include "Papyrus/CompoundOperations/Base.h"

namespace CobbPapyrus {
   IThreadSafeBasicMemPool<CompoundOperationBase::Delegate, 40> CompoundOperationBase::_delegatePool; // This literally doesn't add anything, but we need it or the compiler will choke
}