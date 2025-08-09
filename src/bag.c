#include "global.h"
#include "bag.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "constants/item.h"
#include "event_data.h"
#include "item_menu.h"
#include "malloc.h"
#include "secret_base.h"

EWRAM_DATA struct Bag gBag;

static u16 GetBagItemQuantity(u16 *quantity)
{
    return gSaveBlock2Ptr->encryptionKey ^ *quantity;
}

static void SetBagItemQuantity(u16 *quantity, u16 newValue)
{
    *quantity =  newValue ^ gSaveBlock2Ptr->encryptionKey;
}

static bool8 CheckPyramidBagHasItem(u16 itemId, u16 count);

/********************
* Helper Functions
/********************/
u8 GetPocketCapacity(u8 pocketId)
{
    switch (pocketId)
    {
    case POCKET_ITEMS:
        return BAG_ITEMS_COUNT;
    case POCKET_POKE_BALLS:
        return BAG_POKEBALLS_COUNT;
    case POCKET_TM_HM:
        return BAG_TMHM_COUNT;
    case POCKET_BERRIES:
        return BAG_BERRIES_COUNT;
    case POCKET_KEY_ITEMS:
        return BAG_KEYITEMS_COUNT;
    }

    return 0;
}

u8 GetTMHMIdFromItemId(u16 itemId)
{
    u8 tmhmId = itemId - ITEM_TM01 + 1;

    if (itemId >= ITEM_HM01)
    {
        tmhmId -= 50;
    }

    return tmhmId;
}

u16 GetItemIdFromTMHMId(u8 tmhmId)
{
    u16 itemId = (u16)tmhmId + ITEM_TM01 - 1;

    if (tmhmId > 50)
    {
        itemId += 50;
    }

    return itemId;
}

static void SwapItemSlots(struct ItemSlot *a, struct ItemSlot *b)
{
    struct ItemSlot temp;
    SWAP(*a, *b, temp);
}

static void SwapU8(u8 *a, u8 *b)
{
    u8 temp;
    SWAP(*a, *b, temp);
}

static void SwapU16(u16 *a, u16 *b)
{
    u16 temp;
    SWAP(*a, *b, temp);
}

#pragma region Check for Free Item Space 
u32 GetFreeSpaceInNormalItemsPocket(u16 itemId, struct ItemSlot *slots, u8 capacity)
{
    u8 i;
    u16 ownedCount;
#if I_STORE_SYSTEM == GEN_3
    u32 spaceForItem = 0;
#else
    u8 emptySpaceFound = FALSE;
#endif

    // Check if the item exists in a pocket already
    for (i = 0; i < capacity; i++)
    {
        if (slots[i].itemId == itemId)
        {
            ownedCount = GetBagItemQuantity(&slots[i].quantity);
#if I_STORE_SYSTEM == GEN_3
            spaceForItem += max(0, MAX_BAG_ITEM_CAPACITY - ownedCount);
#else
            return max(0, MAX_BAG_ITEM_CAPACITY - ownedCount);
#endif
        }
        else if (slots[i].itemId == ITEM_NONE)
        {
#if I_STORE_SYSTEM == GEN_3
            spaceForItem += MAX_BAG_ITEM_CAPACITY;
#else
            emptySpaceFound = TRUE;
#endif
        }
    }

#if I_STORE_SYSTEM == GEN_3
    return spaceForItem;
#else
    // Otherwise, return whether an empty slot was found for it
    return emptySpaceFound ? MAX_BAG_ITEM_CAPACITY : 0;
#endif
}

u32 GetFreeSpaceInTMHMPocket(u16 itemId)
{
    u8 i;
    u8 tmhmId = GetTMHMIdFromItemId(itemId);

    // Checks all the pockets for this tm/hm
    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        // If the TM already exists, there is no need to add the new one
        if (gBag.tmhmSlots[i] == tmhmId)
        {
            return 0;
        }
        // If an empty spot was found, you can store the item (Should not be possible to have an empty slot between 2 existing tm/hms anyways)
        else if (gBag.tmhmSlots[i] == ITEM_NONE)
        {
            return 1;
        }
    }

    // The bag was full of different tm/hms, no room for the item. Should never be hit since there's enough space to store 1 of every TM/HM
    return 0;
}

u32 GetFreeSpaceInKeyItemsPocket(u16 itemId)
{
    u8 i;

    // Checks all the pockets for this Key Item
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        // The Key Item obtained should never repeat, so there's something wrong with the logic of the game
        if (gBag.keyItemSlots[i] == itemId)
        {
            return 0;
        }
        // If an empty spot was found, you can store the item (Should always be hit for every new Key Item)
        else if (gBag.keyItemSlots[i] == ITEM_NONE)
        {
            return 1;
        }
    }

    // The bag was full of different Key Items, no room for the item. Could be hit if there's exchangable Key Items yet to be turned in
    return 0;
}
#pragma endregion

#pragma region Check Amount of Item in Bag
bool8 CheckBagHasNormalItem(u16 itemId, u16 count, struct ItemSlot *pocketSlot, u8 pocketCapacity)
{
    u8 i;

    // Check for the item slot that contains the item
    for (i = 0; i < pocketCapacity; i++)
    {
        // Found the slot containing the item
        if (pocketSlot[i].itemId == itemId)
        {
            // Does this item slot contain enough of the item?
            if (GetBagItemQuantity(&pocketSlot[i].quantity) >= count)
                return TRUE;
        }
    }

    return FALSE;
}

bool8 CheckBagHasTMHM(u16 itemId)
{
    u8 i;
    u8 tmhmId = GetTMHMIdFromItemId(itemId);

    // Check for the item slot that contains the item
    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        // Found the slot containing the item
        if (gBag.tmhmSlots[i] == tmhmId)
        {
            // The bag contains the TM/HM requested already
            return TRUE;
        }
    }

    return FALSE;
}

bool8 CheckBagHasKeyItem(u16 itemId)
{
    u8 i;

    // Check for the item slot that contains the item
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        // Found the slot containing the item
        if (gBag.keyItemSlots[i] == itemId)
        {
            // The bag contains the Key Item requested already
            return TRUE;
        }
    }

    return FALSE;
}
#pragma endregion

#pragma region Add Item to Bag
bool8 AddNormalItemToBag(u16 itemId, u16 count, struct ItemSlot *pocketSlots, u8 pocketCapacity)
{
    struct ItemSlot *newItems;
    u16 ownedCount;
#if I_STORE_SYSTEM == GEN_3
    u8 pocket = ItemId_GetPocket(itemId);
#endif
    u8 i;

    // NOTE: It's possible that due to tmhm and key pockets needing to be stored outside of saveslot, whenever the newItems is freed up, these 2 are freed up along with it
    newItems = AllocZeroed(pocketCapacity * sizeof(struct ItemSlot));
    memcpy(newItems, pocketSlots, pocketCapacity * sizeof(struct ItemSlot));

    for (i = 0; i < pocketCapacity; i++)
    {
        if (newItems[i].itemId == itemId)
        {
            ownedCount = GetBagItemQuantity(&newItems[i].quantity);
            // check if won't exceed max slot capacity
            if (ownedCount + count <= MAX_BAG_ITEM_CAPACITY)
            {
                // successfully added to already existing item's count
                SetBagItemQuantity(&newItems[i].quantity, ownedCount + count);
                memcpy(pocketSlots, newItems, pocketCapacity * sizeof(struct ItemSlot));
                Free(newItems);
                return TRUE;
            }
            // If the item's slot is full, and I_STORE_SYSTEM != GEN_3, don't create a new one
            else
            {
#if I_STORE_SYSTEM == GEN_3
                // try creating another instance of the item if possible
                if (pocket == TMHM_POCKET || pocket == BERRIES_POCKET)
                {
#endif
                    Free(newItems);
                    return FALSE;
#if I_STORE_SYSTEM == GEN_3
                }
                else
                {
                    count -= MAX_BAG_ITEM_CAPACITY - ownedCount;
                    SetBagItemQuantity(&newItems[i].quantity, MAX_BAG_ITEM_CAPACITY);
                    // don't create another instance of the item if it's at max slot capacity and count is equal to 0
                    if (count == 0)
                    {
                        break;
                    }
                }
#endif
            }
        }
    }

    // we're done if quantity is equal to 0
    if (count > 0)
    {
        // either no existing item was found, or I_STORE_SYSTEM == GEN_3 and we have to create another instance, because the capacity was exceeded
        for (i = 0; i < pocketCapacity; i++)
        {
            if (newItems[i].itemId == ITEM_NONE)
            {
                newItems[i].itemId = itemId;
#if I_STORE_SYSTEM == GEN_3
                if (count > MAX_BAG_ITEM_CAPACITY)
                {
                    // try creating a new slot with max capacity if duplicates are possible
                    if (pocket == TMHM_POCKET || pocket == BERRIES_POCKET)
                    {
                        Free(newItems);
                        return FALSE;
                    }
                    count -= MAX_BAG_ITEM_CAPACITY;
                    SetBagItemQuantity(&newItems[i].quantity, MAX_BAG_ITEM_CAPACITY);
                }
                else
                {
                    // created a new slot and added quantity
                    SetBagItemQuantity(&newItems[i].quantity, count);
#else
                // if the quantity received is higher than the max item capacity, give as many items as possible (realistically won't happen, since it would require giving 999 items at once)
                SetBagItemQuantity(&newItems[i].quantity, min(count, MAX_BAG_ITEM_CAPACITY));
#endif
                    count = 0;
                    break;
#if I_STORE_SYSTEM == GEN_3
                }
#endif
            }
        }

        if (count > 0)
        {
            Free(newItems);
            return FALSE;
        }
    }
    memcpy(pocketSlots, newItems, pocketCapacity * sizeof(struct ItemSlot));
    Free(newItems);
    return TRUE;
}

// TODO: Add a TMHM sorting? Shouldn't be necessary, since the game sorted them by default even without it here
bool8 AddTMHMToBag(u16 itemId)
{
    u8 i, tmhmId = GetTMHMIdFromItemId(itemId);

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        // TM/HM found, don't need to add another one
        if (gBag.tmhmSlots[i] == tmhmId)
        {
            return FALSE;
        }
    }

    // If we reached this point, the TM/HM didn't exist in the bag, so add it
    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        if (gBag.tmhmSlots[i] == 0)
        {
            gBag.tmhmSlots[i] = tmhmId;
            return TRUE;
        }
    }

    return FALSE;
}

bool8 AddKeyItemToBag(u16 itemId)
{
    u8 i;

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        // Key Item found, don't need to add another one
        if (gBag.keyItemSlots[i] == itemId)
        {
            return FALSE;
        }
    }

    // If we reached this point, the Key Item didn't exist in the bag, so add it
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (gBag.keyItemSlots[i] == 0)
        {
            gBag.keyItemSlots[i] = itemId;
            return TRUE;
        }
    }

    return FALSE;
}
#pragma endregion

#pragma region Remove Item from Bag
bool8 RemoveNormalItemFromBag(u16 itemId, u16 count, struct ItemSlot *pocketSlots, u8 pocketCapacity)
{
    u8 i, pocket;
    u16 totalQuantity = 0;

    u8 var;
    u16 ownedCount;

    pocket = ItemId_GetPocket(itemId) - 1;

    for (i = 0; i < pocketCapacity; i++)
    {
        if (pocketSlots[i].itemId == itemId)
            totalQuantity += GetBagItemQuantity(&pocketSlots[i].quantity);
    }

    if (totalQuantity < count)
        return FALSE; // We don't have enough of the item

    if (CurMapIsSecretBase() == TRUE)
    {
        VarSet(VAR_SECRET_BASE_LOW_TV_FLAGS, VarGet(VAR_SECRET_BASE_LOW_TV_FLAGS) | SECRET_BASE_USED_BAG);
        VarSet(VAR_SECRET_BASE_LAST_ITEM_USED, itemId);
    }

    var = GetItemListPosition(pocket);
    if (pocketCapacity > var && pocketSlots[var].itemId == itemId)
    {
        ownedCount = GetBagItemQuantity(&pocketSlots[var].quantity);
        if (ownedCount >= count)
        {
            SetBagItemQuantity(&pocketSlots[var].quantity, ownedCount - count);
            count = 0;
        }
        else
        {
#if I_STORE_SYSTEM == GEN_3
            count -= ownedCount;
            SetBagItemQuantity(&pocketSlots[var].quantity, 0);
#else
            // Not enough of the requested item
            return FALSE;
#endif
        }

        if (GetBagItemQuantity(&pocketSlots[var].quantity) == 0)
            pocketSlots[var].itemId = ITEM_NONE;

        if (count == 0)
            return TRUE;
    }

    for (i = 0; i < pocketCapacity; i++)
    {
        if (pocketSlots[i].itemId == itemId)
        {
            ownedCount = GetBagItemQuantity(&pocketSlots[i].quantity);
            if (ownedCount >= count)
            {
                SetBagItemQuantity(&pocketSlots[i].quantity, ownedCount - count);
                count = 0;
            }
            else
            {
#if I_STORE_SYSTEM == GEN_3
                count -= ownedCount;
                SetBagItemQuantity(&pocketSlots[var].quantity, 0);
#else
                // Not enough of the requested item
                return FALSE;
#endif
            }

            if (GetBagItemQuantity(&pocketSlots[i].quantity) == 0)
                pocketSlots[i].itemId = ITEM_NONE;

            if (count == 0)
                return TRUE;
        }
    }
    return TRUE;
}

bool8 RemoveTMHMFromBag(u16 itemId)
{
    u8 i, pocket, tmhmId = GetTMHMIdFromItemId(itemId);
    u8 var;
    bool8 haveItem = FALSE;

    pocket = ItemId_GetPocket(itemId) - 1;

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        if (gBag.tmhmSlots[i] == tmhmId)
        {
            haveItem = TRUE;
            break;
        }
    }

    if (!haveItem)
        return FALSE; // We don't have the TM/HM item

    if (CurMapIsSecretBase() == TRUE)
    {
        VarSet(VAR_SECRET_BASE_LOW_TV_FLAGS, VarGet(VAR_SECRET_BASE_LOW_TV_FLAGS) | SECRET_BASE_USED_BAG);
        VarSet(VAR_SECRET_BASE_LAST_ITEM_USED, itemId);
    }

    var = GetItemListPosition(pocket);
    if (BAG_TMHM_COUNT > var && gBag.tmhmSlots[var] == tmhmId)
    {
        gBag.tmhmSlots[i] = ITEM_NONE;
        return TRUE;
    }

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        if (gBag.tmhmSlots[i] == tmhmId)
        {
            gBag.tmhmSlots[i] = ITEM_NONE;
            return TRUE;
        }
    }

    return FALSE;
}

bool8 RemoveKeyItemFromBag(u16 itemId)
{
    u8 i, pocket;
    u8 var;
    bool8 haveItem = FALSE;

    pocket = ItemId_GetPocket(itemId) - 1;

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (gBag.keyItemSlots[i] == itemId)
        {
            haveItem = TRUE;
            break;
        }
    }

    if (!haveItem)
        return FALSE; // We don't have the key item

    if (CurMapIsSecretBase() == TRUE)
    {
        VarSet(VAR_SECRET_BASE_LOW_TV_FLAGS, VarGet(VAR_SECRET_BASE_LOW_TV_FLAGS) | SECRET_BASE_USED_BAG);
        VarSet(VAR_SECRET_BASE_LAST_ITEM_USED, itemId);
    }

    var = GetItemListPosition(pocket);
    if (BAG_KEYITEMS_COUNT > var && gBag.keyItemSlots[var] == itemId)
    {
        gBag.keyItemSlots[i] = ITEM_NONE;
        return TRUE;
    }

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (gBag.keyItemSlots[i] == itemId)
        {
            gBag.keyItemSlots[i] = ITEM_NONE;
            return TRUE;
        }
    }

    return FALSE;
}
#pragma endregion

#pragma region Compact Pockets
void CompactItemsInNormalItemPocket(struct ItemSlot* pocketSlots, u8 pocketCapacity)
{
    u16 i, j;

    for (i = 0; i < pocketCapacity - 1; i++)
    {
        for (j = i + 1; j < pocketCapacity; j++)
        {
            if (GetBagItemQuantity(&pocketSlots[i].quantity) == 0)
                SwapItemSlots(&pocketSlots[i], &pocketSlots[j]);
        }
    }
}

void CompactItemsInTMHMPocket()
{
    u16 i, j;

    for (i = 0; i < BAG_TMHM_COUNT - 1; i++)
    {
        for (j = i + 1; j < BAG_TMHM_COUNT; j++)
        {
            if (gBag.tmhmSlots[i] == 0)
            {
                SwapU8(&gBag.tmhmSlots[i], &gBag.tmhmSlots[j]);
            }
        }
    }
}

void CompactItemsInKeyItemsPocket()
{
    u16 i, j;

    for (i = 0; i < BAG_KEYITEMS_COUNT - 1; i++)
    {
        for (j = i + 1; j < BAG_KEYITEMS_COUNT; j++)
        {
            if (gBag.keyItemSlots[i] == 0)
            {
                SwapU16(&gBag.keyItemSlots[i], &gBag.keyItemSlots[j]);
            }
        }
    }
}
#pragma endregion

#pragma region Sort Pockets
void SortNormalItemPocket(struct ItemSlot* pocketSlots, u8 pocketCapacity)
{
    u16 i, j;

    for (i = 0; i < pocketCapacity - 1; i++)
    {
        for (j = i + 1; j < pocketCapacity; j++)
        {
            if (GetBagItemQuantity(&pocketSlots[i].quantity) != 0)
            {
                if (GetBagItemQuantity(&pocketSlots[j].quantity) == 0)
                    continue;
                if (pocketSlots[i].itemId <= pocketSlots[j].itemId)
                    continue;
            }
            SwapItemSlots(&pocketSlots[i], &pocketSlots[j]);
        }
    }
}

void SortTMHMsPocket()
{
    u16 i, j;

    for (i = 0; i < BAG_TMHM_COUNT - 1; i++)
    {
        for (j = i + 1; j < BAG_TMHM_COUNT; j++)
        {
            if (gBag.tmhmSlots[i] != 0)
            {
                if (gBag.tmhmSlots[j] == 0)
                    continue;
                if (gBag.tmhmSlots[i] <= gBag.tmhmSlots[j])
                    continue;
            }
            SwapU8(&gBag.tmhmSlots[i], &gBag.tmhmSlots[j]);
        }
    }
}

void SortKeyItemsPocket()
{
    u16 i, j;

    for (i = 0; i < BAG_KEYITEMS_COUNT - 1; i++)
    {
        for (j = i + 1; j < BAG_KEYITEMS_COUNT; j++)
        {
            if (gBag.keyItemSlots[i] != 0)
            {
                if (gBag.keyItemSlots[j] == 0)
                    continue;
                if (gBag.keyItemSlots[i] <= gBag.keyItemSlots[j])
                    continue;
            }
            SwapU16(&gBag.keyItemSlots[i], &gBag.keyItemSlots[j]);
        }
    }
}
#pragma endregion

#pragma region Get Previous Item
u16 GetPreviousNormalItem(u16 itemId, struct ItemSlot* pocketSlots, u8 pocketCapacity)
{
    u16 prevItem;
    s32 i, j;
    for (i = 0; i < pocketCapacity; i++)
    {
        if (itemId == pocketSlots[i].itemId)
        {
            if (i <= 0)
            {
                for (j = pocketCapacity - 1; j >= 0; j--)
                {
                    prevItem = pocketSlots[j].itemId;
                    if (prevItem != ITEM_NONE)
                        return prevItem;
                }
            }
            i--;
            break;
        }
    }
    return pocketSlots[i].itemId;
}

u16 GetPreviousTMHM(u16 itemId)
{
    u16 prevItem;
    s32 i, j;
    u8 tmhmId = GetTMHMIdFromItemId(itemId);

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        if (tmhmId == gBag.tmhmSlots[i])
        {
            if (i <= 0)
            {
                for (j = BAG_TMHM_COUNT - 1; j >= 0; j--)
                {
                    prevItem = gBag.tmhmSlots[i];
                    if (prevItem != ITEM_NONE)
                        return prevItem;
                }
            }
            i--;
            break;
        }
    }
    return GetItemIdFromTMHMId(gBag.tmhmSlots[i]);
}

u16 GetPreviousKeyItem(u16 itemId)
{
    u16 prevItem;
    s32 i, j;

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (itemId == gBag.keyItemSlots[i])
        {
            if (i <= 0)
            {
                for (j = BAG_KEYITEMS_COUNT - 1; j >= 0; j--)
                {
                    prevItem = gBag.keyItemSlots[i];
                    if (prevItem != ITEM_NONE)
                        return prevItem;
                }
            }
            i--;
            break;
        }
    }
    return gBag.keyItemSlots[i];
}

#pragma endregion

#pragma region Get Next Item
u16 GetNextNormalItem(u16 itemId, struct ItemSlot* pocketSlots, u8 pocketCapacity)
{
    u16 nextItem = ITEM_NONE;
    s32 i;
    for (i = 1; i < pocketCapacity; i++)
    {
        if (itemId == pocketSlots[i - 1].itemId)
        {
            nextItem = pocketSlots[i].itemId;
            break;
        }
    }
    if (nextItem == ITEM_NONE)
        return pocketSlots[0].itemId; // Zeroth slot
    else
        return nextItem;
}

u16 GetNextTMHM(u16 itemId)
{
    u16 nextItem = 0;
    s32 i;
    u8 tmhmId = GetTMHMIdFromItemId(itemId);
    for (i = 1; i < BAG_TMHM_COUNT; i++)
    {
        if (tmhmId == gBag.tmhmSlots[i - 1])
        {
            nextItem = gBag.tmhmSlots[i];
            break;
        }
    }

    if (nextItem == 0)
        return GetItemIdFromTMHMId(gBag.tmhmSlots[0]); // Zeroth slot
    else
        return GetItemIdFromTMHMId(nextItem);
}

u16 GetNextKeyItem(u16 itemId)
{
    u16 nextItem = ITEM_NONE;
    s32 i;
    for (i = 1; i < BAG_KEYITEMS_COUNT; i++)
    {
        if (itemId == gBag.keyItemSlots[i - 1])
        {
            nextItem = gBag.keyItemSlots[i];
            break;
        }
    }

    if (nextItem == ITEM_NONE)
        return gBag.keyItemSlots[0]; // Zeroth slot
    else
        return nextItem;
}

#pragma endregion

#pragma region Move Item to Slot
void MoveItemToSlot(u32 from, u32 to, struct ItemSlot* pocketSlots)
{
    if (from != to)
    {
        s16 i, count;
        struct ItemSlot firstSlot = pocketSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                pocketSlots[i] = pocketSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                pocketSlots[i] = pocketSlots[i - 1];
        }
        pocketSlots[to] = firstSlot;
    }
}

void MoveTMHMToSlot(u32 from, u32 to)
{
    if (from != to)
    {
        s16 i, count;
        u8 itemToMove = gBag.tmhmSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                gBag.tmhmSlots[i] = gBag.tmhmSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                gBag.tmhmSlots[i] = gBag.tmhmSlots[i - 1];
        }
        gBag.tmhmSlots[to] = itemToMove;
    }
}

void MoveKeyItemToSlot(u32 from, u32 to)
{
    if (from != to)
    {
        s16 i, count;
        u16 itemToMove = gBag.keyItemSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                gBag.keyItemSlots[i] = gBag.keyItemSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                gBag.keyItemSlots[i] = gBag.keyItemSlots[i - 1];
        }
        gBag.keyItemSlots[to] = itemToMove;
    }
}
#pragma endregion

u16 CountTotalItemQuantityInPocket(u16 itemId, struct ItemSlot* pocketSlots, u8 pocketCapacity)
{
    u16 i;
    u16 ownedCount = 0;

    for (i = 0; i < pocketCapacity; i++)
    {
        if (pocketSlots[i].itemId == itemId)
            ownedCount += GetBagItemQuantity(&pocketSlots[i].quantity);
    }

    return ownedCount;
}

void ClearNormalItemsSlots(struct ItemSlot *itemSlots, u8 slotCapacity)
{
    u16 i;

    for (i = 0; i < slotCapacity; i++)
    {
        itemSlots[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&itemSlots[i].quantity, 0);
    }
}

/********************
* Functions
/********************/
bool8 IsBagPocketEmpty(u8 pocket)
{
    u8 i;

    switch (pocket)
    {
    case POCKET_ITEMS:
        for (i = 0; i < BAG_ITEMS_COUNT; i++)
        {
            if (gBag.itemSlots[i].itemId != ITEM_NONE)
            {
                return FALSE;
            }
        }
        break;
    case POCKET_POKE_BALLS:
        for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
        {
            if (gBag.pokeballSlots[i].itemId != ITEM_NONE)
            {
                return FALSE;
            }
        }
        break;
    case POCKET_TM_HM:
        for (i = 0; i < BAG_TMHM_COUNT; i++)
        {
            if (gBag.tmhmSlots[i] == 0)
            {
                return FALSE;
            }
        }
        break;
    case POCKET_BERRIES:
        for (i = 0; i < BAG_BERRIES_COUNT; i++)
        {
            if (gBag.berrySlots[i].itemId != ITEM_NONE)
            {
                return FALSE;
            }
        }
        break;
    case POCKET_KEY_ITEMS:
        for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        {
            if (gBag.keyItemSlots[i] == ITEM_NONE)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

u32 GetFreeSpaceForItemInBag(u16 itemId)
{
    u8 pocket = ItemId_GetPocket(itemId);

    if (pocket == POCKET_NONE)
        return 0;

    switch (pocket)
    {
    case POCKET_ITEMS:
        return GetFreeSpaceInNormalItemsPocket(itemId, gBag.itemSlots, BAG_ITEMS_COUNT);
    case POCKET_POKE_BALLS:
        return GetFreeSpaceInNormalItemsPocket(itemId, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    case POCKET_TM_HM:
        return GetFreeSpaceInTMHMPocket(itemId);
    case POCKET_BERRIES:
        return GetFreeSpaceInNormalItemsPocket(itemId, gBag.berrySlots, BAG_BERRIES_COUNT);
    case POCKET_KEY_ITEMS:
        return GetFreeSpaceInKeyItemsPocket(itemId);
    }

    return 0;
}

u8 GetNumItemStacks(u8 pocketId)
{
    u8 i, numItemStacks = 0;

    switch (pocketId)
    {
    case POCKET_ITEMS:
        for (i = 0; i < BAG_ITEMS_COUNT && gBag.itemSlots[i].itemId; i++)
            numItemStacks++;
        break;
    case POCKET_POKE_BALLS:
        for (i = 0; i < BAG_POKEBALLS_COUNT && gBag.pokeballSlots[i].itemId; i++)
            numItemStacks++;
        break;
    case POCKET_TM_HM:
        for (i = 0; i < BAG_TMHM_COUNT && gBag.tmhmSlots[i]; i++)
            numItemStacks++;
        break;
    case POCKET_BERRIES:
        for (i = 0; i < BAG_BERRIES_COUNT && gBag.berrySlots[i].itemId; i++)
            numItemStacks++;
        break;
    case POCKET_KEY_ITEMS:
        for (i = 0; i < BAG_KEYITEMS_COUNT && gBag.keyItemSlots[i]; i++)
            numItemStacks++;
        break;
    }

    return numItemStacks;
}

bool8 CheckBagHasItem(u16 itemId, u16 count)
{
    if (ItemId_GetPocket(itemId) == 0)
        return FALSE;
    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
        return CheckPyramidBagHasItem(itemId, count);

    switch (ItemId_GetPocket(itemId))
    {
    case POCKET_ITEMS:
        return CheckBagHasNormalItem(itemId, count, gBag.itemSlots, BAG_ITEMS_COUNT);
    case POCKET_POKE_BALLS:
        return CheckBagHasNormalItem(itemId, count, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    case POCKET_TM_HM:
        return CheckBagHasTMHM(itemId);
    case POCKET_BERRIES:
        return CheckBagHasNormalItem(itemId, count, gBag.berrySlots, BAG_BERRIES_COUNT);
    case POCKET_KEY_ITEMS:
        return CheckBagHasKeyItem(itemId);
    }

    return FALSE;
}

bool8 AddBagItem(u16 itemId, u16 count)
{
    if (ItemId_GetPocket(itemId) == POCKET_NONE)
        return FALSE;

    // check Battle Pyramid Bag
    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
    {
        return AddPyramidBagItem(itemId, count);
    }
    else
    {
        switch (ItemId_GetPocket(itemId))
        {
        case POCKET_ITEMS:
            return AddNormalItemToBag(itemId, count, gBag.itemSlots, BAG_ITEMS_COUNT);
        case POCKET_POKE_BALLS:
            return AddNormalItemToBag(itemId, count, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
        case POCKET_TM_HM:
            return AddTMHMToBag(itemId);
        case POCKET_BERRIES:
            return AddNormalItemToBag(itemId, count, gBag.berrySlots, BAG_BERRIES_COUNT);
        case POCKET_KEY_ITEMS:
            return AddKeyItemToBag(itemId);
        }
    }

    return FALSE;
}

bool8 RemoveBagItem(u16 itemId, u16 count)
{
    if (ItemId_GetPocket(itemId) == POCKET_NONE || itemId == ITEM_NONE)
        return FALSE;

    // check Battle Pyramid Bag
    if (InBattlePyramid() || FlagGet(FLAG_STORING_ITEMS_IN_PYRAMID_BAG) == TRUE)
    {
        return RemovePyramidBagItem(itemId, count);
    }
    else
    {
        switch (ItemId_GetPocket(itemId))
        {
        case POCKET_ITEMS:
            return RemoveNormalItemFromBag(itemId, count, gBag.itemSlots, BAG_ITEMS_COUNT);
        case POCKET_POKE_BALLS:
            return RemoveNormalItemFromBag(itemId, count, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
        case POCKET_TM_HM:
            return RemoveTMHMFromBag(itemId);
        case POCKET_BERRIES:
            return RemoveNormalItemFromBag(itemId, count, gBag.berrySlots, BAG_BERRIES_COUNT);
        case POCKET_KEY_ITEMS:
            return RemoveKeyItemFromBag(itemId);
        }
    }

    return FALSE;
}

u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 pocketPos)
{
    switch (pocketId)
    {
    case POCKET_ITEMS:
        return gBag.itemSlots[pocketPos].itemId;
    case POCKET_POKE_BALLS:
        return gBag.pokeballSlots[pocketPos].itemId;
    case POCKET_TM_HM:
        return (gBag.tmhmSlots[pocketPos] == 0 ? 0 : GetItemIdFromTMHMId(gBag.tmhmSlots[pocketPos]));
    case POCKET_BERRIES:
        return gBag.berrySlots[pocketPos].itemId;
    case POCKET_KEY_ITEMS:
        return gBag.keyItemSlots[pocketPos];
    }

    return ITEM_NONE;
}

u16 BagGetQuantityByPocketPosition(u8 pocketId, u16 pocketPos)
{
    u16 quantity = 0;

    switch (pocketId)
    {
    case POCKET_ITEMS:
        return GetBagItemQuantity(&gBag.itemSlots[pocketPos].quantity);
    case POCKET_POKE_BALLS:
        return GetBagItemQuantity(&gBag.pokeballSlots[pocketPos].quantity);
    case POCKET_TM_HM:
        quantity = gBag.tmhmSlots[pocketPos] == 0 ? 0 : 1;
    case POCKET_BERRIES:
        return GetBagItemQuantity(&gBag.berrySlots[pocketPos].quantity);
    case POCKET_KEY_ITEMS:
        quantity = gBag.keyItemSlots[pocketPos] == ITEM_NONE ? 0 : 1;
    }

    return GetBagItemQuantity(&quantity);
}

void CompactItemsInBagPocket(u8 pocketId)
{
    switch (pocketId)
    {
    case POCKET_ITEMS:
        CompactItemsInNormalItemPocket(gBag.itemSlots, BAG_ITEMS_COUNT);
        break;
    case POCKET_POKE_BALLS:
        CompactItemsInNormalItemPocket(gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
        break;
    case POCKET_TM_HM:
        CompactItemsInTMHMPocket();
        break;
    case POCKET_BERRIES:
        CompactItemsInNormalItemPocket(gBag.berrySlots, BAG_BERRIES_COUNT);
        break;
    case POCKET_KEY_ITEMS:
        CompactItemsInKeyItemsPocket();
        break;
    }
}

void SortPocket(u8 pocketId)
{
    switch (pocketId)
    {
    case POCKET_ITEMS:
        SortNormalItemPocket(gBag.itemSlots, BAG_ITEMS_COUNT);
        break;
    case POCKET_POKE_BALLS:
        SortNormalItemPocket(gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
        break;
    case POCKET_TM_HM:
        SortTMHMsPocket();
        break;
    case POCKET_BERRIES:
        SortNormalItemPocket(gBag.berrySlots, BAG_BERRIES_COUNT);
        break;
    case POCKET_KEY_ITEMS:
        SortKeyItemsPocket();
        break;
    }
}

u16 CountTotalItemQuantityInBag(u16 itemId)
{
    switch (ItemId_GetPocket(itemId))
    {
    case POCKET_ITEMS:
        return CountTotalItemQuantityInPocket(itemId, gBag.itemSlots, BAG_ITEMS_COUNT);
    case POCKET_POKE_BALLS:
        return CountTotalItemQuantityInPocket(itemId, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    case POCKET_TM_HM:
        return CheckBagHasTMHM(itemId) == TRUE ? 1 : 0;
    case POCKET_BERRIES:
        return CountTotalItemQuantityInPocket(itemId, gBag.berrySlots, BAG_BERRIES_COUNT);
    case POCKET_KEY_ITEMS:
        return CheckBagHasKeyItem(itemId) == TRUE ? 1 : 0;
    }

    return 0;
}

void ClearPocket(u8 pocketId)
{
    u16 i;

    switch (pocketId)
    {
    case POCKET_ITEMS:
        return ClearNormalItemsSlots(gBag.itemSlots, BAG_ITEMS_COUNT);
    case POCKET_POKE_BALLS:
        return ClearNormalItemsSlots(gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    case POCKET_TM_HM:
        for (i = 0; i < BAG_TMHM_COUNT; i++)
        {
            gBag.tmhmSlots[i] = ITEM_NONE;
        }
    case POCKET_BERRIES:
        return ClearNormalItemsSlots(gBag.berrySlots, BAG_BERRIES_COUNT);
    case POCKET_KEY_ITEMS:
        for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        {
            gBag.keyItemSlots[i] = ITEM_NONE;
        }
    }
}

void ClearBag(void)
{
    u16 i;

    ClearNormalItemsSlots(gBag.itemSlots, BAG_ITEMS_COUNT);
    ClearNormalItemsSlots(gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    ClearNormalItemsSlots(gBag.berrySlots, BAG_BERRIES_COUNT);

    for (i = 0; i < BAG_TMHM_COUNT; i++)
    {
        gBag.tmhmSlots[i] = 0;
    }

    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
    {
        gBag.keyItemSlots[i] = ITEM_NONE;
    }
}

// Called to show the previous and next PokeBalls during battle, but it could be used for anything
u16 GetPreviousItem(u16 itemId)
{
    u8 pocket = ItemId_GetPocket(itemId);

    CompactItemsInBagPocket(pocket);
    
    switch (pocket)
    {
    case POCKET_ITEMS:
        return GetPreviousNormalItem(itemId, gBag.itemSlots, BAG_ITEMS_COUNT);
    case POCKET_POKE_BALLS:
        return GetPreviousNormalItem(itemId, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    case POCKET_TM_HM:
        return GetPreviousTMHM(itemId);
    case POCKET_BERRIES:
        return GetPreviousNormalItem(itemId, gBag.berrySlots, BAG_BERRIES_COUNT);
    case POCKET_KEY_ITEMS:
        return GetPreviousKeyItem(itemId);
    }

    return ITEM_NONE;
}

u16 GetNextItem(u16 itemId)
{
    u8 pocket = ItemId_GetPocket(itemId);

    CompactItemsInBagPocket(pocket);

    switch (pocket)
    {
    case POCKET_ITEMS:
        return GetNextNormalItem(itemId, gBag.itemSlots, BAG_ITEMS_COUNT);
    case POCKET_POKE_BALLS:
        return GetNextNormalItem(itemId, gBag.pokeballSlots, BAG_POKEBALLS_COUNT);
    case POCKET_TM_HM:
        return GetNextTMHM(itemId);
    case POCKET_BERRIES:
        return GetNextNormalItem(itemId, gBag.berrySlots, BAG_BERRIES_COUNT);
    case POCKET_KEY_ITEMS:
        return GetNextKeyItem(itemId);
    }

    return ITEM_NONE;
}

void MoveItemSlotInList(u8 pocketId, u32 from, u32 to)
{
    switch (pocketId)
    {
    case POCKET_ITEMS:
        return MoveItemToSlot(from, to, gBag.itemSlots);
    case POCKET_POKE_BALLS:
        return MoveItemToSlot(from, to, gBag.pokeballSlots);
    case POCKET_TM_HM:
        return MoveTMHMToSlot(from, to);
    case POCKET_BERRIES:
        return MoveItemToSlot(from, to, gBag.berrySlots);
    case POCKET_KEY_ITEMS:
        return MoveKeyItemToSlot(from, to);
    }
}

/************************
* Pyramid Bag Functions
/************************/

static bool8 CheckPyramidBagHasItem(u16 itemId, u16 count)
{
    u8 i;
    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
#if MAX_PYRAMID_BAG_ITEM_CAPACITY > 255
    u16 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];
#else
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];
#endif

    for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
    {
        if (items[i] == itemId)
        {
            if (quantities[i] >= count)
                return TRUE;

            count -= quantities[i];
            if (count == 0)
                return TRUE;
        }
    }

    return FALSE;
}

bool8 AddPyramidBagItem(u16 itemId, u16 count)
{
    u16 i;

    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
    u16 *newItems = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));

#if MAX_PYRAMID_BAG_ITEM_CAPACITY > 255
    u16 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];
    u16 *newQuantities = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));
#else
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];
    u8 *newQuantities = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));
#endif

    memcpy(newItems, items, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
    memcpy(newQuantities, quantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));

    for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
    {
        if (newItems[i] == itemId && newQuantities[i] < MAX_PYRAMID_BAG_ITEM_CAPACITY)
        {
            newQuantities[i] += count;
            if (newQuantities[i] > MAX_PYRAMID_BAG_ITEM_CAPACITY)
            {
                count = newQuantities[i] - MAX_PYRAMID_BAG_ITEM_CAPACITY;
                newQuantities[i] = MAX_PYRAMID_BAG_ITEM_CAPACITY;
            }
            else
            {
                count = 0;
            }

            if (count == 0)
                break;
        }
    }

    if (count > 0)
    {
        for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
        {
            if (newItems[i] == ITEM_NONE)
            {
                newItems[i] = itemId;
                newQuantities[i] = count;
                if (newQuantities[i] > MAX_PYRAMID_BAG_ITEM_CAPACITY)
                {
                    count = newQuantities[i] - MAX_PYRAMID_BAG_ITEM_CAPACITY;
                    newQuantities[i] = MAX_PYRAMID_BAG_ITEM_CAPACITY;
                }
                else
                {
                    count = 0;
                }

                if (count == 0)
                    break;
            }
        }
    }

    if (count == 0)
    {
        memcpy(items, newItems, PYRAMID_BAG_ITEMS_COUNT * sizeof(*items));
        memcpy(quantities, newQuantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*quantities));
        Free(newItems);
        Free(newQuantities);
        return TRUE;
    }
    else
    {
        Free(newItems);
        Free(newQuantities);
        return FALSE;
    }
}

bool8 RemovePyramidBagItem(u16 itemId, u16 count)
{
    u16 i;

    u16 *items = gSaveBlock2Ptr->frontier.pyramidBag.itemId[gSaveBlock2Ptr->frontier.lvlMode];
#if MAX_PYRAMID_BAG_ITEM_CAPACITY > 255
    u16 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];
#else
    u8 *quantities = gSaveBlock2Ptr->frontier.pyramidBag.quantity[gSaveBlock2Ptr->frontier.lvlMode];
#endif

    i = gPyramidBagMenuState.cursorPosition + gPyramidBagMenuState.scrollPosition;
    if (items[i] == itemId && quantities[i] >= count)
    {
        quantities[i] -= count;
        if (quantities[i] == 0)
            items[i] = ITEM_NONE;
        return TRUE;
    }
    else
    {
        u16 *newItems = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
    #if MAX_PYRAMID_BAG_ITEM_CAPACITY > 255
        u16 *newQuantities = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));
    #else
        u8 *newQuantities = Alloc(PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));
    #endif

        memcpy(newItems, items, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newItems));
        memcpy(newQuantities, quantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*newQuantities));

        for (i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
        {
            if (newItems[i] == itemId)
            {
                if (newQuantities[i] >= count)
                {
                    newQuantities[i] -= count;
                    count = 0;
                    if (newQuantities[i] == 0)
                        newItems[i] = ITEM_NONE;
                }
                else
                {
                    count -= newQuantities[i];
                    newQuantities[i] = 0;
                    newItems[i] = ITEM_NONE;
                }

                if (count == 0)
                    break;
            }
        }

        if (count == 0)
        {
            memcpy(items, newItems, PYRAMID_BAG_ITEMS_COUNT * sizeof(*items));
            memcpy(quantities, newQuantities, PYRAMID_BAG_ITEMS_COUNT * sizeof(*quantities));
            Free(newItems);
            Free(newQuantities);
            return TRUE;
        }
        else
        {
            Free(newItems);
            Free(newQuantities);
            return FALSE;
        }
    }
}
