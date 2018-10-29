#pragma once
#include "skse/Utilities.h"

class MagicMenu;
namespace RE {
   class IMessageBoxCallback {
      public:
         virtual void Dispose(bool);
         virtual void Callback(UInt32) = 0;
   };
   //
   class SpendSoulCallback : public IMessageBoxCallback { // sizeof == 0xC // This is for DRAGON souls, not soul gems!
      public:
         virtual void Callback(UInt32);
         //
         UInt32     unk04;
         MagicMenu* owner;
   };

   namespace CraftingSubMenus {
      namespace EnchantConstructMenu {
         class EnchantMenuCallback : public IMessageBoxCallback {
            public:
               virtual void Callback(UInt32);
         };
         class EnchantMenuCraftCallback : public EnchantMenuCallback {
            public:
               virtual void Callback(UInt32);
               //
               UInt32 unk04;
               void*  unk08; // probably a class for the enchantment menu -- sorry, EnchantConstructMenu
         };
      };
   };
   typedef CraftingSubMenus::EnchantConstructMenu::EnchantMenuCraftCallback ConfirmEnchantItemCallback;
};