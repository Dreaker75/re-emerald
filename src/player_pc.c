#include "global.h"
#include "constants/songs.h"
#include "bg.h"
#include "decoration.h"
#include "event_scripts.h"
#include "event_object_movement.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "international_string_util.h"
#include "item.h"
#include "item_icon.h"
#include "item_menu.h"
#include "constants/items.h"
#include "list_menu.h"
#include "mail.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "player_pc.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "window.h"
#include "menu_specialized.h"

// Top level PC menu options
enum {
    MENU_ITEMSTORAGE,
    MENU_MAILBOX,
    MENU_DECORATION,
    MENU_TURNOFF
};

// Item storage menu options
enum {
    MENU_WITHDRAW,
    MENU_DEPOSIT,
    MENU_TOSS,
    MENU_EXIT
};

// Windows for the main menus (top level menu, and item storage menu)
enum {
    WIN_MAIN_MENU,
    WIN_MAIN_MENU_BEDROOM,
    WIN_ITEM_STORAGE_MENU,
};

// Windows for item storage (while viewing the PC's item inventory)
enum {
    ITEMPC_WIN_LIST,
    ITEMPC_WIN_MESSAGE,
    ITEMPC_WIN_ICON,
    ITEMPC_WIN_TITLE,
    ITEMPC_WIN_QUANTITY,
    ITEMPC_WIN_YESNO,
    ITEMPC_WIN_COUNT
};
 // When showing the main list, the first window to this window are drawn
#define ITEMPC_WIN_LIST_END ITEMPC_WIN_TITLE

// Message IDs for Item Storage
enum {
    MSG_SWITCH_WHICH_ITEM = 0xFFF7,
    MSG_OKAY_TO_THROW_AWAY,
    MSG_TOO_IMPORTANT,
    MSG_NO_MORE_ROOM,
    MSG_THREW_AWAY_ITEM,
    MSG_HOW_MANY_TO_TOSS,
    MSG_WITHDREW_ITEM,
    MSG_HOW_MANY_TO_WITHDRAW,
    MSG_GO_BACK_TO_PREV
};

#define TAG_ITEM_ICON    5110
#define TAG_SCROLL_ARROW 5112

// Item list ID for toSwapPos to indicate an item is not currently being swapped
#define NOT_SWAPPING 0xFF

#define SWAP_LINE_LENGTH 7

static void InitPlayerPCMenu(u8);
static void PlayerPCProcessMenuInput(u8);

static u8 GetMailboxMailCount(void);
static void Mailbox_CompactMailList(void);
static void Mailbox_DrawMailboxMenu(u8);
static void Mailbox_ProcessInput(u8);
static void Mailbox_PrintWhatToDoWithPlayerMailText(u8);
static void Mailbox_ReturnToPlayerPC(u8);
static void Mailbox_PrintMailOptions(u8);
static void Mailbox_MailOptionsProcessInput(u8);

static void PlayerPC_Mailbox(u8);
static void PlayerPC_Decoration(u8);
static void PlayerPC_TurnOff(u8);

static void Mailbox_DoMailMoveToBag(u8);
static void Mailbox_DoMailRead(u8);
static void Mailbox_MoveToBag(u8);
static void Mailbox_Give(u8);
static void Mailbox_Cancel(u8);

static void Mailbox_CancelMoveToBag(u8);
static void Mailbox_HandleConfirmMoveToBag(u8);
static void Mailbox_AskConfirmMoveToBag(u8);
static void Mailbox_DoGiveMailPokeMenu(u8);
static void Mailbox_NoPokemonForMail(u8);

static void Mailbox_FadeAndReadMail(u8);
static void Mailbox_ReturnToFieldFromReadMail(void);
static void Mailbox_ReshowAfterMail(void);
static void Mailbox_HandleReturnToProcessInput(u8);
static void Mailbox_UpdateMailListAfterDeposit(void);

static void SetPlayerPCListCount(u8);

static EWRAM_DATA const u8 *sTopMenuOptionOrder = NULL;
static EWRAM_DATA u8 sTopMenuNumOptions = 0;
EWRAM_DATA struct PlayerPCItemPageStruct gPlayerPCItemPageInfo = {};

static const struct MenuAction sPlayerPCMenuActions[] =
{
    [MENU_MAILBOX]     = { gText_Mailbox,     {PlayerPC_Mailbox} },
    [MENU_DECORATION]  = { gText_Decoration,  {PlayerPC_Decoration} },
    [MENU_TURNOFF]     = { gText_TurnOff,     {PlayerPC_TurnOff} }
};

static const u8 sBedroomPC_OptionOrder[] =
{
    MENU_MAILBOX,
    MENU_DECORATION,
    MENU_TURNOFF
};
#define NUM_BEDROOM_PC_OPTIONS ARRAY_COUNT(sBedroomPC_OptionOrder)

static const u8 sPlayerPC_OptionOrder[] =
{
    MENU_MAILBOX,
    MENU_TURNOFF
};
#define NUM_PLAYER_PC_OPTIONS ARRAY_COUNT(sPlayerPC_OptionOrder)

static const u16 sNewGameItems[][2] =
{
    { ITEM_POTION, 1 },
    { ITEM_NONE, 0 }
};

const struct MenuAction gMailboxMailOptions[] =
{
    { gText_Read,      {Mailbox_DoMailRead} },
    { gText_MoveToBag, {Mailbox_MoveToBag} },
    { gText_Give2,     {Mailbox_Give} },
    { gText_Cancel2,   {Mailbox_Cancel} }
};

static const struct WindowTemplate sWindowTemplates_MainMenus[] =
{
    [WIN_MAIN_MENU] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 9,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_MAIN_MENU_BEDROOM] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 9,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_ITEM_STORAGE_MENU] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 10,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 1
    }
};

static const struct WindowTemplate sWindowTemplates_ItemStorage[ITEMPC_WIN_COUNT] =
{
    [ITEMPC_WIN_LIST] = {
        .bg = 0,
        .tilemapLeft = 16,
        .tilemapTop = 1,
        .width = 13,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 0x0001
    },
    [ITEMPC_WIN_MESSAGE] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 13,
        .width = 13,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 0x00EB
    },
    [ITEMPC_WIN_ICON] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 8,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 0x0153
    },
    [ITEMPC_WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 13,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x0139
    },
    [ITEMPC_WIN_QUANTITY] = {
        .bg = 0,
        .tilemapLeft = 8,
        .tilemapTop = 9,
        .width = 6,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x015C
    },
    [ITEMPC_WIN_YESNO] = {
        .bg = 0,
        .tilemapLeft = 9,
        .tilemapTop = 7,
        .width = 5,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x0168
    }
};

static const u8 sSwapArrowTextColors[] = {TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY};

void NewGameInitItems(void)
{
    u8 i = 0;
    ClearBag();

    while (TRUE)
    {
        if (sNewGameItems[i][0] == ITEM_NONE || sNewGameItems[i][1] == 0)
            break;
        if (AddBagItem(sNewGameItems[i][0], sNewGameItems[i][1]) != TRUE)
            break;
        i++;
    }
}

void BedroomPC(void)
{
    sTopMenuOptionOrder = sBedroomPC_OptionOrder;
    sTopMenuNumOptions = NUM_BEDROOM_PC_OPTIONS;
    DisplayItemMessageOnField(CreateTask(TaskDummy, 0), gText_WhatWouldYouLike, InitPlayerPCMenu);
}

void PlayerPC(void)
{
    sTopMenuOptionOrder = sPlayerPC_OptionOrder;
    sTopMenuNumOptions = NUM_PLAYER_PC_OPTIONS;
    DisplayItemMessageOnField(CreateTask(TaskDummy, 0), gText_WhatWouldYouLike, InitPlayerPCMenu);
}

#define tUsedSlots  data[1]
#define tQuantity   data[2]
#define tInTossMenu data[3]
#define tWindowId   data[4]
#define tListTaskId data[5]

static void InitPlayerPCMenu(u8 taskId)
{
    s16 *data;
    struct WindowTemplate windowTemplate;
    data = gTasks[taskId].data;

    if (sTopMenuNumOptions == NUM_PLAYER_PC_OPTIONS)
        windowTemplate = sWindowTemplates_MainMenus[WIN_MAIN_MENU];
    else // Bedroom PC
        windowTemplate = sWindowTemplates_MainMenus[WIN_MAIN_MENU_BEDROOM];

    windowTemplate.width = GetMaxWidthInSubsetOfMenuTable(sPlayerPCMenuActions, sTopMenuOptionOrder, sTopMenuNumOptions);
    tWindowId = AddWindow(&windowTemplate);
    SetStandardWindowBorderStyle(tWindowId, FALSE);
    PrintMenuActionTextsInUpperLeftCorner(tWindowId, sTopMenuNumOptions, sPlayerPCMenuActions, sTopMenuOptionOrder);
    InitMenuInUpperLeftCornerNormal(tWindowId, sTopMenuNumOptions, 0);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = PlayerPCProcessMenuInput;
}

static void PlayerPCProcessMenuInput(u8 taskId)
{
    s16 *data;
    s8 inputOptionId;

    data = gTasks[taskId].data;
    if (sTopMenuNumOptions > 3)
        inputOptionId = Menu_ProcessInput();
    else
        inputOptionId = Menu_ProcessInputNoWrap();

    switch (inputOptionId)
    {
    case MENU_NOTHING_CHOSEN:
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        ClearStdWindowAndFrameToTransparent(tWindowId, FALSE);
        ClearWindowTilemap(tWindowId);
        RemoveWindow(tWindowId);
        ScheduleBgCopyTilemapToVram(0);
        gTasks[taskId].func = PlayerPC_TurnOff;
        break;
    default:
        ClearStdWindowAndFrameToTransparent(tWindowId, FALSE);
        ClearWindowTilemap(tWindowId);
        RemoveWindow(tWindowId);
        ScheduleBgCopyTilemapToVram(0);
        gTasks[taskId].func = sPlayerPCMenuActions[sTopMenuOptionOrder[inputOptionId]].func.void_u8;
        break;
    }
}

void ReshowPlayerPC(u8 var)
{
    DisplayItemMessageOnField(var, gText_WhatWouldYouLike, InitPlayerPCMenu);
}

static void PlayerPC_Mailbox(u8 taskId)
{
    gPlayerPCItemPageInfo.count = GetMailboxMailCount();

    if (gPlayerPCItemPageInfo.count == 0)
    {
        // Mailbox cannot be opened if no mail is in PC
        DisplayItemMessageOnField(taskId, gText_NoMailHere, ReshowPlayerPC);
    }
    else
    {
        gPlayerPCItemPageInfo.cursorPos = 0;
        gPlayerPCItemPageInfo.itemsAbove = 0;
        gPlayerPCItemPageInfo.scrollIndicatorTaskId = TASK_NONE;
        Mailbox_CompactMailList();
        SetPlayerPCListCount(taskId);
        if (MailboxMenu_Alloc(gPlayerPCItemPageInfo.count) == TRUE)
        {
            ClearDialogWindowAndFrame(0, FALSE);
            Mailbox_DrawMailboxMenu(taskId);
            gTasks[taskId].func = Mailbox_ProcessInput;
        }
        else
        {
            // Alloc failed, exit Mailbox
            DisplayItemMessageOnField(taskId, gText_NoMailHere, ReshowPlayerPC);
        }
    }
}

static void PlayerPC_Decoration(u8 taskId)
{
    DoPlayerRoomDecorationMenu(taskId);
}

static void PlayerPC_TurnOff(u8 taskId)
{
    if (sTopMenuNumOptions == NUM_BEDROOM_PC_OPTIONS) // Flimsy way to determine if Bedroom PC is in use
    {
        if (gSaveBlock2Ptr->playerGender == MALE)
            ScriptContext_SetupScript(LittlerootTown_BrendansHouse_2F_EventScript_TurnOffPlayerPC);
        else
            ScriptContext_SetupScript(LittlerootTown_MaysHouse_2F_EventScript_TurnOffPlayerPC);
    }
    else
    {
        ScriptContext_Enable();
    }
    DestroyTask(taskId);
}

// Used by Item Storage and the Mailbox
static void SetPlayerPCListCount(u8 taskId)
{
    if (gPlayerPCItemPageInfo.count > 7)
        gPlayerPCItemPageInfo.pageItems = 8;
    else
        gPlayerPCItemPageInfo.pageItems = gPlayerPCItemPageInfo.count + 1;
}

static u8 GetMailboxMailCount(void)
{
    u8 mailInPC, i;

    // Count mail in PC (by first skipping over mail in party)
    for (mailInPC = 0, i = PARTY_SIZE; i < MAIL_COUNT; i++)
        if (gSaveBlock1Ptr->mail[i].itemId != ITEM_NONE)
            mailInPC++;

    return mailInPC;
}

static void Mailbox_CompactMailList(void)
{
    struct Mail temp;
    u8 i, j;

    for (i = PARTY_SIZE; i < MAIL_COUNT - 1; i++)
    {
        for (j = i + 1; j < MAIL_COUNT; j++)
        {
            if (gSaveBlock1Ptr->mail[i].itemId == ITEM_NONE)
                SWAP(gSaveBlock1Ptr->mail[i], gSaveBlock1Ptr->mail[j], temp);
        }
    }
}

static void Mailbox_DrawMailboxMenu(u8 taskId)
{
    u8 windowId = MailboxMenu_AddWindow(MAILBOXWIN_TITLE);
    MailboxMenu_AddWindow(MAILBOXWIN_LIST);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_Mailbox, GetStringCenterAlignXOffset(FONT_NORMAL, gText_Mailbox, 0x40), 1, 0, NULL);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].tListTaskId = MailboxMenu_CreateList(&gPlayerPCItemPageInfo);
    MailboxMenu_AddScrollArrows(&gPlayerPCItemPageInfo);
}

static void Mailbox_ProcessInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        s32 inputOptionId = ListMenu_ProcessInput(tListTaskId);
        ListMenuGetScrollAndRow(tListTaskId, &gPlayerPCItemPageInfo.itemsAbove, &gPlayerPCItemPageInfo.cursorPos);

        switch (inputOptionId)
        {
        case LIST_NOTHING_CHOSEN:
            break;
        case LIST_CANCEL:
            PlaySE(SE_SELECT);
            RemoveScrollIndicatorArrowPair(gPlayerPCItemPageInfo.scrollIndicatorTaskId);
            Mailbox_ReturnToPlayerPC(taskId);
            break;
        default:
            // Selected mail, ask what to do with it
            PlaySE(SE_SELECT);
            MailboxMenu_RemoveWindow(MAILBOXWIN_TITLE);
            MailboxMenu_RemoveWindow(MAILBOXWIN_LIST);
            DestroyListMenuTask(tListTaskId, &gPlayerPCItemPageInfo.itemsAbove, &gPlayerPCItemPageInfo.cursorPos);
            ScheduleBgCopyTilemapToVram(0);
            RemoveScrollIndicatorArrowPair(gPlayerPCItemPageInfo.scrollIndicatorTaskId);
            gTasks[taskId].func = Mailbox_PrintWhatToDoWithPlayerMailText;
            break;
        }
    }
}

static void Mailbox_PrintWhatToDoWithPlayerMailText(u8 taskId)
{
    StringCopy(gStringVar1, gSaveBlock1Ptr->mail[gPlayerPCItemPageInfo.itemsAbove + PARTY_SIZE + gPlayerPCItemPageInfo.cursorPos].playerName);
    ConvertInternationalPlayerNameStripChar(gStringVar1, CHAR_SPACE);
    StringExpandPlaceholders(gStringVar4, gText_WhatToDoWithVar1sMail);
    DisplayItemMessageOnField(taskId, gStringVar4, Mailbox_PrintMailOptions);
}

static void Mailbox_ReturnToPlayerPC(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    MailboxMenu_RemoveWindow(MAILBOXWIN_TITLE);
    MailboxMenu_RemoveWindow(MAILBOXWIN_LIST);
    DestroyListMenuTask(tListTaskId, NULL, NULL);
    ScheduleBgCopyTilemapToVram(0);
    MailboxMenu_Free();
    ReshowPlayerPC(taskId);
}

static void Mailbox_PrintMailOptions(u8 taskId)
{
    u8 windowId = MailboxMenu_AddWindow(MAILBOXWIN_OPTIONS);
    PrintMenuTable(windowId, ARRAY_COUNT(gMailboxMailOptions), gMailboxMailOptions);
    InitMenuInUpperLeftCornerNormal(windowId, ARRAY_COUNT(gMailboxMailOptions), 0);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = Mailbox_MailOptionsProcessInput;
}

static void Mailbox_MailOptionsProcessInput(u8 taskId)
{
    s8 inputOptionId = ProcessMenuInput_other();

    switch (inputOptionId)
    {
    case MENU_NOTHING_CHOSEN:
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        Mailbox_Cancel(taskId);
        break;
    default:
        PlaySE(SE_SELECT);
        gMailboxMailOptions[inputOptionId].func.void_u8(taskId);
        break;
    }
}

static void Mailbox_DoMailRead(u8 taskId)
{
    FadeScreen(FADE_TO_BLACK, 0);
    gTasks[taskId].func = Mailbox_FadeAndReadMail;
}

static void Mailbox_FadeAndReadMail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        MailboxMenu_Free();
        CleanupOverworldWindowsAndTilemaps();
        ReadMail(&gSaveBlock1Ptr->mail[gPlayerPCItemPageInfo.itemsAbove + PARTY_SIZE + gPlayerPCItemPageInfo.cursorPos], Mailbox_ReturnToFieldFromReadMail, TRUE);
        DestroyTask(taskId);
    }
}

static void Mailbox_ReturnToFieldFromReadMail(void)
{
    gFieldCallback = Mailbox_ReshowAfterMail;
    SetMainCallback2(CB2_ReturnToField);
}

static void Mailbox_ReshowAfterMail(void)
{
    u8 taskId;

    LoadMessageBoxAndBorderGfx();
    taskId = CreateTask(Mailbox_HandleReturnToProcessInput, 0);
    if (MailboxMenu_Alloc(gPlayerPCItemPageInfo.count) == TRUE)
        Mailbox_DrawMailboxMenu(taskId);
    else
        DestroyTask(taskId);
    FadeInFromBlack();
}

static void Mailbox_HandleReturnToProcessInput(u8 taskId)
{
    if (IsWeatherNotFadingIn() == TRUE)
        gTasks[taskId].func = Mailbox_ProcessInput;
}

static void Mailbox_MoveToBag(u8 taskId)
{
    DisplayItemMessageOnField(taskId, gText_MessageWillBeLost, Mailbox_AskConfirmMoveToBag);
}

static void Mailbox_AskConfirmMoveToBag(u8 taskId)
{
    DisplayYesNoMenuDefaultYes();
    gTasks[taskId].func = Mailbox_HandleConfirmMoveToBag;
}

static void Mailbox_HandleConfirmMoveToBag(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        Mailbox_DoMailMoveToBag(taskId);
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
    case 1: // No
        Mailbox_CancelMoveToBag(taskId);
        break;
    case MENU_NOTHING_CHOSEN:
    default:
        break;
    }
}

static void Mailbox_DoMailMoveToBag(u8 taskId)
{
    struct Mail *mail = &gSaveBlock1Ptr->mail[gPlayerPCItemPageInfo.itemsAbove + PARTY_SIZE + gPlayerPCItemPageInfo.cursorPos];
    if (!AddBagItem(mail->itemId, 1))
    {
        DisplayItemMessageOnField(taskId, gText_BagIsFull, Mailbox_Cancel);
    }
    else
    {
        DisplayItemMessageOnField(taskId, gText_MailToBagMessageErased, Mailbox_Cancel);
        ClearMail(mail);
        Mailbox_CompactMailList();
        gPlayerPCItemPageInfo.count--;
        if (gPlayerPCItemPageInfo.count < (gPlayerPCItemPageInfo.pageItems + gPlayerPCItemPageInfo.itemsAbove) && gPlayerPCItemPageInfo.itemsAbove != 0)
            gPlayerPCItemPageInfo.itemsAbove--;
        SetPlayerPCListCount(taskId);
    }
}

static void Mailbox_CancelMoveToBag(u8 taskId)
{
    Mailbox_Cancel(taskId);
}

static void Mailbox_Give(u8 taskId)
{
    if (CalculatePlayerPartyCount() == 0)
        Mailbox_NoPokemonForMail(taskId);
    else
    {
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Mailbox_DoGiveMailPokeMenu;
    }
}

static void Mailbox_DoGiveMailPokeMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        MailboxMenu_Free();
        CleanupOverworldWindowsAndTilemaps();
        ChooseMonToGiveMailFromMailbox();
        DestroyTask(taskId);
    }
}

void Mailbox_ReturnToMailListAfterDeposit(void)
{
    gFieldCallback = Mailbox_UpdateMailListAfterDeposit;
    SetMainCallback2(CB2_ReturnToField);
}

static void Mailbox_UpdateMailListAfterDeposit(void)
{
    u8 taskId;
    u8 prevCount;
    taskId = CreateTask(Mailbox_HandleReturnToProcessInput, 0);
    prevCount = gPlayerPCItemPageInfo.count;
    gPlayerPCItemPageInfo.count = GetMailboxMailCount();
    Mailbox_CompactMailList();
    if (prevCount != gPlayerPCItemPageInfo.count && (gPlayerPCItemPageInfo.count < (gPlayerPCItemPageInfo.pageItems + gPlayerPCItemPageInfo.itemsAbove))
       && gPlayerPCItemPageInfo.itemsAbove != 0)
        gPlayerPCItemPageInfo.itemsAbove--;
    SetPlayerPCListCount(taskId);
    LoadMessageBoxAndBorderGfx();
    if (MailboxMenu_Alloc(gPlayerPCItemPageInfo.count) == TRUE)
        Mailbox_DrawMailboxMenu(taskId);
    else
        DestroyTask(taskId);
    FadeInFromBlack();
}

static void Mailbox_NoPokemonForMail(u8 taskId)
{
    DisplayItemMessageOnField(taskId, gText_NoPokemon, Mailbox_Cancel);
}

static void Mailbox_Cancel(u8 taskId)
{
    MailboxMenu_RemoveWindow(MAILBOXWIN_OPTIONS);
    ClearDialogWindowAndFrame(0, FALSE);
    Mailbox_DrawMailboxMenu(taskId);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = Mailbox_ProcessInput;
}
