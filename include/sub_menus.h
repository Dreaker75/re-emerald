#ifndef GUARD_START_MENU_H
#define GUARD_START_MENU_H

extern bool8 (*gMenuCallback)(void);

void ShowReturnToFieldStartMenu(void);
void ShowReturnToFieldShortcutsMenu(void);
void ShowLastSubMenu(void);
void Task_ShowSubMenu(u8 taskId);
void ShowStartMenu(void);
void ShowKeyItemsMenu(void);
void ShowShortcutsMenu(void);
void ShowSubMenu(TaskFunc currFunc, TaskFunc followupFunc);
void ShowBattlePyramidStartMenu(void);
void SaveGame(void);
void CB2_SetUpSaveAfterLinkBattle(void);
void SaveForBattleTowerLink(void);
void HideSubMenu(bool8 unlockControls);
void AppendToList(u8 *list, u8 *pos, u8 newEntry);

#endif // GUARD_START_MENU_H
