#ifndef GUARD_REGISTERED_ITEMS_H
#define GUARD_REGISTERED_ITEMS_H

u16 GetRegisteredItem(u8);
void RemoveRegisteredItem(u16);
u8 GetAmountOfItemsRegistered(void);
void ClearItemsRegistered(void);
bool8 IsItemRegistered(u16 itemId);
bool8 ToggleItemRegistered(u16 itemId);
bool8 SwapRegisteredItems(u16 item1, u16 item2);

#endif // GUARD_REGISTERED_ITEMS_H