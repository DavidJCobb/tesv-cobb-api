#pragma once

namespace RE {
   class BSResource {
      public:
         virtual ~BSResource();

         class ArchiveStreamOpenedEvent {
            public:
               // ...
         };

         template<typename D>
         class EntryDBBase {
            public:
               virtual ~EntryDBBase();
               virtual void Unk_01(void);	// Pure
               virtual void Unk_02(void);
               virtual void Unk_03(void);
               virtual void Unk_04(void);
               virtual void Unk_05(void);
         };

         template<typename D>
         class EntryDB : public EntryDBBase<D> {
            public:
               virtual ~EntryDB();
               virtual void Unk_01(void);
               virtual void Unk_02(void);
               virtual void Unk_03(void);
               virtual void Unk_04(void);
               virtual void Unk_05(void);
               //
               D data;	// 04
         };
   };
};