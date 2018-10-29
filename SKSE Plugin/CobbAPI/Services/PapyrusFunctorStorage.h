#pragma once
#include <map>
#include <string>
#include <vector>
#include "common/ICriticalSection.h"

struct SKSESerializationInterface;
class TESForm;
class VMClassRegistry;

// Serializable classes deriving from IStorableObject must define a
// serialization constructor that takes SerializationTag as the single argument.
// After it has been called, either Load() is used to fill in the data,
// or in case the load failed, the instance is destroyed again.
struct StorableSerializationTag {};

// IStorageObject: Interface for a heap-allocated, co-save-serializable object.
//
class IStorableObject { // Copy of IStorableObject
   public:
	  virtual ~IStorableObject() {}
	  virtual const char*	ClassName() const    = 0;
	  virtual UInt32		ClassVersion() const = 0;
      //
	  virtual bool Save(SKSESerializationInterface* intfc) = 0;
	  virtual bool Load(SKSESerializationInterface* intfc, UInt32 version) = 0;
};

class IStorableObjectFactory { // Copy of IStorableObjectFactory
   public:
	  virtual ~IStorableObjectFactory() {}
      //
	  virtual IStorableObject* Create() const = 0;
};

template <typename T>
class ConcreteStorableObjectFactory : public IStorableObjectFactory { // Copy of ConcreteSKSEObjectFactory
   public:
	  virtual IStorableObject* Create() const {
         StorableSerializationTag tag;
		 return new T(tag);
	  }
};

class StorableObjectRegistry { // Copy of SKSEObjectRegistry
   private:
	  typedef std::map<std::string,UInt32> FactoryMapT;
   public:
	  template <typename T>
	  bool RegisterClass() {
		 ConcreteStorableObjectFactory<T> factory;
		 UInt32 vtbl = *reinterpret_cast<UInt32*>(&factory);
         //
         StorableSerializationTag tag;
		 T tempInstance(tag);
         //
		 std::string className(tempInstance.ClassName());
         //
		 factoryMap_[className] = vtbl;
         //
		 return true;
	  }
	  const IStorableObjectFactory* GetFactoryByName(const char* name) const;
   private:
	  FactoryMapT factoryMap_; // Stores the vtables directly
};

class StorablePersistentObjectStorage { // Copy of SKSEPersistentObjectStorage
   // Note: handle = index + 1
   // +1, because in previous versions the invalid handle was 0 so people
   // might test for != 0
   private:
      struct Entry {
         IStorableObject* obj;
         UInt32 owningStackId;
      };
      typedef std::vector<Entry>  DataT;
      typedef std::vector<size_t> IndexCacheT;
      //
      ICriticalSection lock_;
      DataT       data_;
      IndexCacheT freeIndices_;
   public:
      template <typename T>
      SInt32 StoreObject(T* obj, UInt32 owningStackId) { // Transfer ownership of object to the registry
         IScopedCriticalSection scopedLock( &lock_ );

         Entry e = { obj, owningStackId };
         SInt32 index;

         if (freeIndices_.empty()) {
	        index = data_.size();
	        data_.push_back(e);
         } else {
	        index = freeIndices_.back();
	        freeIndices_.pop_back();
	        data_[index] = e;
         }
         return index + 1;
      };
      //
      template <typename T>
      T* AccessObject(SInt32 handle) { // Access object while keeping it in storage.
         IScopedCriticalSection scopedLock(&lock_);
         //
         SInt32 index = handle - 1;
         if (index < 0 || index >= data_.size()) {
	        _MESSAGE("StorablePersistentObjectStorage::AccessObject(%d): Invalid handle.", handle);
	        return nullptr;
         }
         Entry& e = data_[index];
         if (e.obj == nullptr) {
	        _MESSAGE("StorablePersistentObjectStorage::AccessObject(%d): Object was NULL.", handle);
	        return nullptr;
         }
         T* result = dynamic_cast<T*>(e.obj);
         if (result == nullptr) {
	        _MESSAGE("StorablePersistentObjectStorage::AccessObject(%d): Invalid type (%s).", handle, e.obj->ClassName());
	        return nullptr;
         }
         return result;
      };
      //
      template <typename T>
      T* TakeObject(SInt32 handle) { // Remove object from storage and take ownership of the pointer.
         IScopedCriticalSection scopedLock( &lock_ );
         //
         SInt32 index = handle - 1;
         if (index < 0 || index >= data_.size()) {
	        _MESSAGE("StorablePersistentObjectStorage::AccessObject(%d): Invalid handle.", handle);
	        return nullptr;
         }
         Entry& e = data_[index];
         if (e.obj == nullptr) {
	        _MESSAGE("StorablePersistentObjectStorage::TakeObject(%d): Object was NULL.", handle);
	        return nullptr;
         }
         T* result = dynamic_cast<T*>(e.obj);
         if (result != nullptr) {
	        e.obj = nullptr;
	        freeIndices_.push_back(index);
         } else {
	        _MESSAGE("StorablePersistentObjectStorage::TakeObject(%d): Invalid type (%s).", handle, e.obj->ClassName());
	        return nullptr;
         }
         return result;
      };
      //
      void CleanDroppedStacks();
      void ClearAndRelease();
      //
      enum { kSaveVersion = 2 };
      bool Save(SKSESerializationInterface* intfc);
      bool Load(SKSESerializationInterface* intfc, UInt32 version);
};

// Serialization helpers:
//
bool WriteStorableObject(SKSESerializationInterface* intfc, IStorableObject* obj);
bool ReadStorableObject(SKSESerializationInterface* intfc, IStorableObject*& objOut);

// Global instances:
//
StorableObjectRegistry& StorableObjectRegistryInstance();
StorablePersistentObjectStorage& StorableObjectStorageInstance();