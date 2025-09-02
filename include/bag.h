#ifndef GUARD_BAG_H
#define GUARD_BAG_H

struct Bag
{
    struct ItemSlot *itemSlots;
    struct ItemSlot *medicineSlots;
    struct ItemSlot *battleItemSlots;
    struct ItemSlot *pokeballSlots;
    struct ItemSlot *berrySlots;
    // u8 to save space, will need to cast from itemId to a 1-58 id every time (base 1 to account for empty slots when 0). Code has 100 TMs, so when accessing HMs, a -50 offset is needed
    u8 *tmhmSlots;
    u16 *keyItemSlots;
};

bool8 IsBagPocketEmpty(u8 pocket);
u32 GetFreeSpaceForItemInBag(u16 itemId);
u8 GetNumItemStacks(u8 pocketId);
bool8 CheckBagHasItem(u16 itemId, u16 count);
bool8 AddBagItem(u16 itemId, u16 count);
bool8 RemoveBagItem(u16 itemId, u16 count);
u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 pocketPos);
u16 BagGetQuantityByPocketPosition(u8 pocketId, u16 pocketPos);
void CompactItemsInBagPocket(u8 pocketId);
void SortPocket(u8 pocketId);
u16 CountTotalItemQuantityInBag(u16 itemId);
void ClearPocket(u8 pocketId);
void ClearBag(void);
u16 GetPreviousItem(u16 itemId);
u16 GetNextItem(u16 itemId);
void MoveItemSlotInList(u8 pocketId, u32 from, u32 to);

/************************
* Pyramid Bag Functions
/************************/
bool8 AddPyramidBagItem(u16 itemId, u16 count);
bool8 RemovePyramidBagItem(u16 itemId, u16 count);

#endif // GUARD_BAG_H