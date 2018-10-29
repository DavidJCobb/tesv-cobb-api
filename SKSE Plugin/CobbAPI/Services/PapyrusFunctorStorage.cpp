#include <algorithm>

#include "Services/PapyrusFunctorStorage.h"
#include "skse/Serialization.h"
#include "skse/PapyrusVM.h"

namespace {
   static const size_t kMaxNameLen  = 1024;
}

const IStorableObjectFactory* StorableObjectRegistry::GetFactoryByName(const char* name) const {
   std::string t(name);
   //
   const IStorableObjectFactory* result = nullptr;
   //
   FactoryMapT::const_iterator it = factoryMap_.find(t);
   if (it != factoryMap_.end()) {
      result = reinterpret_cast<const IStorableObjectFactory*>(&it->second);
   }
   return result;
}

void StorablePersistentObjectStorage::CleanDroppedStacks() {
   VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
   for (UInt32 i=0; i<data_.size(); i++) {
      Entry& e = data_[i];
      if (e.obj == nullptr)
	     continue;
      if (registry->GetStackInfo(e.owningStackId) != nullptr)
	     continue;
      //
      // Stack no longer active; drop this entry.
      //
      delete e.obj;
      e.obj = nullptr;
      freeIndices_.push_back(i);
      _MESSAGE("StorablePersistentObjectStorage::CleanDroppedStacks: Freed object at index %d.", i);
   }
}
void StorablePersistentObjectStorage::ClearAndRelease() {
   IScopedCriticalSection scopedLock(&lock_);
   freeIndices_.clear();
   for (DataT::iterator it = data_.begin(); it != data_.end(); ++it) {
	  Entry& e = *it;
	  if (e.obj != nullptr)
		 delete e.obj;
   }
   data_.clear();
}
bool StorablePersistentObjectStorage::Save(SKSESerializationInterface* intfc) {
   using namespace Serialization;
   //
   // Before saving, remove entries whose owning stack is no longer running, so that 
   // if someone forgets to release an object, resource leaks don't pile up in the 
   // co-save.
   //
   CleanDroppedStacks();
   //
   // Save data.
   //
   UInt32 dataSize = this->data_.size();
   if (!WriteData(intfc, &dataSize))
      return false;
   {  // Write number of objects actually in use.
      UInt32 filledSize = 0;
      UInt32 freedIndices = this->freeIndices_.size();
      if (freedIndices <= dataSize) { // Bugfix!
         filledSize = dataSize - freedIndices;
      }
      if (!WriteData(intfc, &filledSize))
         return false;
   }
   for (UInt32 i = 0; i < dataSize; i++) { // Write objects.
      Entry& e = this->data_[i];
	  if (e.obj == nullptr) // index is not in use
		 continue;
	  if (!WriteStorableObject(intfc, e.obj)) // skip to next entry if write fails.
		 continue;
	  WriteData(intfc, &e.owningStackId);
	  UInt32 index = i;
	  WriteData(intfc, &index);
   }
   return true;
}
bool StorablePersistentObjectStorage::Load(SKSESerializationInterface* intfc, UInt32 loadedVersion) {
   using namespace Serialization;
   //
   this->ClearAndRelease();
   UInt32 dataSize;
   if (!ReadData(intfc,&dataSize))
	  return false;
   this->data_.resize(dataSize);
   UInt32 filledSize;
   if (!ReadData(intfc,&filledSize))
	  return false;
   if (loadedVersion < 2 && filledSize > 0xFEFFFFFF) {
      //
      // This save is affected by a bug in the old (SKSE) code. We've 
      // serialized a negative number; there are no stored objects.
      //
      filledSize = 0;
   }
   for (UInt32 i = 0; i < filledSize; i++)	{
	  Entry e = { 0 };
	  if (!ReadStorableObject(intfc, e.obj))
		 continue;
	  ReadData(intfc, &e.owningStackId);
	  UInt32 index;
	  ReadData(intfc, &index);
	  this->data_[index] = e;
   }
   //
   // Rebuild the free index list.
   //
   for (UInt32 i = 0; i < this->data_.size(); i++)
      if (this->data_[i].obj == nullptr)
         this->freeIndices_.push_back(i);
   //
   return true;
}

// Serialization helpers:
//
bool WriteStorableObject(SKSESerializationInterface* intfc, IStorableObject* obj) {
   using namespace Serialization;
   const char*  name    = obj->ClassName();
   const UInt32 version = obj->ClassVersion();
   //
   intfc->OpenRecord('OBJE', version);
   //
   size_t rawLen = strlen(name);
   UInt32 len    = (std::min)(rawLen, kMaxNameLen);
   //
   if (!WriteData(intfc, &len))
	  return false;
   if (! intfc->WriteRecordData(name, len))
	  return false;
   //
   return obj->Save(intfc);
}
bool ReadStorableObject (SKSESerializationInterface* intfc, IStorableObject*& objOut) {
   UInt32 type, length, objVersion;
   if (!intfc->GetNextRecordInfo(&type, &objVersion, &length))
	  return false;
   if (type != 'OBJE') {
	  _MESSAGE("ReadStorableObject: Error loading unexpected chunk type %08X (%.4s)", type, &type);
	  return false;
   }
   //
   // Read the name of the serialized class.
   //
   UInt32 len;
   if (!intfc->ReadRecordData(&len, sizeof(len)))
	  return false;
   if (len > kMaxNameLen) {
	  _MESSAGE("ReadStorableObject: Serialization error. Class name len extended kMaxNameLen.");
	  return false;
   }
   char buf[kMaxNameLen+1] = { 0 };
   if (!intfc->ReadRecordData(&buf, len))
	  return false;
   //
   // Get the factory.
   //
   const IStorableObjectFactory* factory = StorableObjectRegistryInstance().GetFactoryByName(buf);
   if (factory == nullptr) {
	  _MESSAGE("ReadStorableObject: Serialization error. Factory missing for %s.", &buf);
	  return false;
   }
   //
   // Instantiate and load the actual data.
   //
   IStorableObject* obj = factory->Create();
   if (!obj->Load(intfc, objVersion)) {
	  // Load failed. clean up.
	  objOut = nullptr;
	  delete obj;
	  return false;
   }
   objOut = obj;
   return true;
}

// Global instances:
//
StorableObjectRegistry& StorableObjectRegistryInstance() {
	static StorableObjectRegistry instance;
	return instance;
}
StorablePersistentObjectStorage& StorableObjectStorageInstance() {
	static StorablePersistentObjectStorage instance;
	return instance;
}
