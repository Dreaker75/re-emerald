#include "global.h"
#include "load_save.h"
#include "registered_items.h"

u16 GetRegisteredItem(u8 index)
{
    return gSaveBlock1Ptr->registeredItems[index];
}

// Called when an item is removed from the key items pocket
void RemoveRegisteredItem(u16 item)
{
    u8 i = 0;
    while (i < REGISTERED_ITEMS_COUNT && gSaveBlock1Ptr->registeredItems[i] != item) i++;
    
    // If i has passed the last index, it means the item was not found.
    if (i >= REGISTERED_ITEMS_COUNT)
    {
        return;
    }

    for (; i < REGISTERED_ITEMS_COUNT - 1; i++)
    {
        gSaveBlock1Ptr->registeredItems[i] = gSaveBlock1Ptr->registeredItems[i + 1];
    }
    
    gSaveBlock1Ptr->registeredItems[REGISTERED_ITEMS_COUNT - 1] = ITEM_NONE;
}

u8 GetAmountOfItemsRegistered(void)
{
    u8 amount = 0;

    for (; amount < REGISTERED_ITEMS_COUNT; amount++)
    {
        if (gSaveBlock1Ptr->registeredItems[amount] == ITEM_NONE)
        {
            break;            
        }
    }
    
    return amount;
}

void ClearItemsRegistered(void)
{
    u8 i = 0;
    for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
    {
        gSaveBlock1Ptr->registeredItems[i] = ITEM_NONE;
    }
}

bool8 IsItemRegistered(u16 itemId)
{
    u8 i = 0;
    for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->registeredItems[i] == itemId)
            return TRUE;
    }

    return FALSE;
}

// Registers the item if there's an open spot for it, or deselects it if it's already registered
// - returns FALSE if there was no spot to add the item to, TRUE otherwise
bool8 ToggleItemRegistered(u16 itemId)
{
    u8 i = 0;

    // If the player is trying to register the item
    if (!IsItemRegistered(itemId))
    {
        // If there is no open spot for the item
        if (gSaveBlock1Ptr->registeredItems[REGISTERED_ITEMS_COUNT - 1] != ITEM_NONE)
        {
            // We can't add the item to the registered list
            return FALSE;
        }

        // Loop through all the registered items
        for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
        {
            // When an open spot is found
            if (gSaveBlock1Ptr->registeredItems[i] == ITEM_NONE)
            {
                // Register the item to this spot
                gSaveBlock1Ptr->registeredItems[i] = itemId;

                break;
            }
        }
    }
    // The player is trying to deselect the item
    else
    {
        // Loop through all the items
        for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
        {
            // Once we find the registered item
            if (gSaveBlock1Ptr->registeredItems[i] == itemId)
            {
                // Continue moving through the array, until we reach the last spot OR an empty spot is found
                for (; i < REGISTERED_ITEMS_COUNT - 1 && gSaveBlock1Ptr->registeredItems[i + 1] != ITEM_NONE; i++)
                {
                    // Move the next item backwards 1 position
                    gSaveBlock1Ptr->registeredItems[i] = gSaveBlock1Ptr->registeredItems[i + 1];
                }
                
                // Free up the last spot that held an item
                gSaveBlock1Ptr->registeredItems[i] = ITEM_NONE;
                
                break;
            }
        }
    }

    return TRUE;
}

// Swaps a registered item with an item that's not registered (otherwise the function doesn't work correctly). If neither item is found, no swap is made.
// Returns TRUE if a swap was made, FALSE otherwise.
bool8 SwapRegisteredItems(u16 item1, u16 item2)
{
    for (u8 i = 0; i < REGISTERED_ITEMS_COUNT; i++)
    {
        // Neither item was found in the registered list
        if (gSaveBlock1Ptr->registeredItems[i] == ITEM_NONE)
        {
            break;
        }

        // The first item was found, swap it with the second item
        if (gSaveBlock1Ptr->registeredItems[i] == item1)
        {
            gSaveBlock1Ptr->registeredItems[i] = item2;
            return TRUE;
        }
        
        // The second item was found, swap it with the first item
        if (gSaveBlock1Ptr->registeredItems[i] == item2)
        {
            gSaveBlock1Ptr->registeredItems[i] = item1;
            return TRUE;
        }
    }
    
    // The loop has finished without finding either item
    return FALSE;
}