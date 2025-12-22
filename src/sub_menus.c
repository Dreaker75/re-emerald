#include "global.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "bg.h"
#include "debug.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "field_screen_effect.h"
#include "fldeff.h"
#include "frontier_pass.h"
#include "frontier_util.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "item_use.h"
#include "link.h"
#include "load_save.h"
#include "main.h"
#include "menu.h"
#include "constants/moves.h"
#include "new_game.h"
#include "option_menu.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokenav.h"
#include "region_map.h"
#include "registered_items.h"
#include "safari_zone.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sub_menus.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_card.h"
#include "window.h"
#include "union_room.h"
#include "constants/battle_frontier.h"
#include "constants/menus.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#if (DECAP_ENABLED) && (DECAP_MIRRORING) && !(DECAP_START_MENU)
#define AddTextPrinterParameterized (AddTextPrinterFixedCaseParameterized)
#endif

// Menu actions
enum
{
    MENU_ACTION_POKEDEX,
    MENU_ACTION_POKEMON,
    MENU_ACTION_BAG,
    MENU_ACTION_POKENAV,
    MENU_ACTION_PLAYER,
    MENU_ACTION_SAVE,
    MENU_ACTION_OPTION,
    MENU_ACTION_EXIT,
    MENU_ACTION_RETIRE_SAFARI,
    MENU_ACTION_PLAYER_LINK,
    MENU_ACTION_REST_FRONTIER,
    MENU_ACTION_RETIRE_FRONTIER,
    MENU_ACTION_PYRAMID_BAG,
    MENU_ACTION_DEBUG,
};

// Save status
enum
{
    SAVE_IN_PROGRESS,
    SAVE_SUCCESS,
    SAVE_CANCELED,
    SAVE_ERROR
};

// Shortcuts menu actions
enum
{
    MENU_ACTION_PREVENT_ENCOUNTERS,
    MENU_ACTION_SWEET_SCENT,
    MENU_ACTION_AUTO_RUN_TOGGLE,
    MENU_ACTION_EXIT_DUNGEON,
    MENU_ACTION_FAST_TRAVEL
};

// IWRAM common
bool8 (*gMenuCallback)(void);

// EWRAM
EWRAM_DATA static u8 sGameVersionWindowId = 0;
EWRAM_DATA static u8 sSafariBallsWindowId = 0;
EWRAM_DATA static u8 sBattlePyramidFloorWindowId = 0;
EWRAM_DATA static u8 sSubMenuCursorPos = 0;
EWRAM_DATA static u8 sNumSubMenuActions = 0;
EWRAM_DATA static u8 sCurrentSubMenuActions[9] = {0};
EWRAM_DATA static s8 sInitSubMenuData[2] = {0};

EWRAM_DATA static u8 (*sSaveDialogCallback)(void) = NULL;
EWRAM_DATA static u8 sSaveDialogTimer = 0;
EWRAM_DATA static bool8 sSavingComplete = FALSE;
EWRAM_DATA static u8 sSaveInfoWindowId = 0;
u8 sMenuOpened = MENU_NONE;

// Menu action callbacks
static bool8 StartMenuPokedexCallback(void);
static bool8 StartMenuPokemonCallback(void);
static bool8 StartMenuBagCallback(void);
static bool8 StartMenuPokeNavCallback(void);
static bool8 StartMenuPlayerNameCallback(void);
static bool8 StartMenuSaveCallback(void);
static bool8 StartMenuOptionCallback(void);
static bool8 StartMenuExitCallback(void);
static bool8 StartMenuSafariZoneRetireCallback(void);
static bool8 StartMenuLinkModePlayerNameCallback(void);
static bool8 StartMenuBattlePyramidRetireCallback(void);
static bool8 StartMenuBattlePyramidBagCallback(void);
static bool8 StartMenuDebugCallback(void);

// Shortcuts menu action callbacks
static bool8 ShortcutsMenuPreventEncountersCallback(void);
static bool8 ShortcutsMenuSweetScentCallback(void);
static bool8 ShortcutsMenuAutoRunToggleCallback(void);
static bool8 ShortcutsMenuExitDungeonCallback(void);
static bool8 ShortcutsMenuFastTravelCallback(void);

// Menu callbacks
static bool8 SaveStartCallback(void);
static bool8 SaveCallback(void);
static bool8 BattlePyramidRetireStartCallback(void);
static bool8 BattlePyramidRetireReturnCallback(void);
static bool8 BattlePyramidRetireCallback(void);
void HandleSubMenuInput(void);
static bool8 HandleStartMenuInput(void);
static bool8 HandleKeyItemsMenuInput(void);
static bool8 HandleShortcutsMenuInput(void);

// Save dialog callbacks
static u8 SaveConfirmSaveCallback(void);
static u8 SaveYesNoCallback(void);
static u8 SaveConfirmInputCallback(void);
static u8 SaveFileExistsCallback(void);
static u8 SaveConfirmOverwriteDefaultNoCallback(void);
static u8 SaveOverwriteInputCallback(void);
static u8 SaveSavingMessageCallback(void);
static u8 SaveDoSaveCallback(void);
static u8 SaveSuccessCallback(void);
static u8 SaveReturnSuccessCallback(void);
static u8 SaveErrorCallback(void);
static u8 SaveReturnErrorCallback(void);
static u8 BattlePyramidConfirmRetireCallback(void);
static u8 BattlePyramidRetireYesNoCallback(void);
static u8 BattlePyramidRetireInputCallback(void);

// Task callbacks
static void StartMenuTask(u8 taskId);
static void KeyItemsMenuTask(u8 taskId);
static void ShortcutsMenuTask(u8 taskId);
static void SaveGameTask(u8 taskId);
static void Task_SaveAfterLinkBattle(u8 taskId);
static void Task_WaitForBattleTowerLinkSave(u8 taskId);
static bool8 FieldCB_ReturnToFieldStartMenu(void);
static bool8 FieldCB_ReturnToFieldShortcutsMenu(void);

static const struct WindowTemplate sWindowTemplate_GameVersion = {
    .bg = 0,
    .tilemapLeft = 10,
    .tilemapTop = 15,
    .width = 9,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x48
};

static const struct WindowTemplate sWindowTemplate_SafariBalls = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 9,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x8
};

static const u8 *const sPyramidFloorNames[FRONTIER_STAGES_PER_CHALLENGE + 1] =
{
    gText_Floor1,
    gText_Floor2,
    gText_Floor3,
    gText_Floor4,
    gText_Floor5,
    gText_Floor6,
    gText_Floor7,
    gText_Peak
};

static const struct WindowTemplate sWindowTemplate_PyramidFloor = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 10,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x8
};

static const struct WindowTemplate sWindowTemplate_PyramidPeak = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 12,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x8
};

static const u8 sText_MenuDebug[] = _("DEBUG");

static const struct MenuAction sStartMenuItems[] =
{
    [MENU_ACTION_POKEDEX]         = {gText_MenuPokedex, {.u8_void = StartMenuPokedexCallback}},
    [MENU_ACTION_POKEMON]         = {gText_MenuPokemon, {.u8_void = StartMenuPokemonCallback}},
    [MENU_ACTION_BAG]             = {gText_MenuBag,     {.u8_void = StartMenuBagCallback}},
    [MENU_ACTION_POKENAV]         = {gText_MenuPokenav, {.u8_void = StartMenuPokeNavCallback}},
    [MENU_ACTION_PLAYER]          = {gText_MenuPlayer,  {.u8_void = StartMenuPlayerNameCallback}},
    [MENU_ACTION_SAVE]            = {gText_MenuSave,    {.u8_void = StartMenuSaveCallback}},
    [MENU_ACTION_OPTION]          = {gText_MenuOption,  {.u8_void = StartMenuOptionCallback}},
    [MENU_ACTION_EXIT]            = {gText_MenuExit,    {.u8_void = StartMenuExitCallback}},
    [MENU_ACTION_RETIRE_SAFARI]   = {gText_MenuRetire,  {.u8_void = StartMenuSafariZoneRetireCallback}},
    [MENU_ACTION_PLAYER_LINK]     = {gText_MenuPlayer,  {.u8_void = StartMenuLinkModePlayerNameCallback}},
    [MENU_ACTION_REST_FRONTIER]   = {gText_MenuRest,    {.u8_void = StartMenuSaveCallback}},
    [MENU_ACTION_RETIRE_FRONTIER] = {gText_MenuRetire,  {.u8_void = StartMenuBattlePyramidRetireCallback}},
    [MENU_ACTION_PYRAMID_BAG]     = {gText_MenuBag,     {.u8_void = StartMenuBattlePyramidBagCallback}},
    [MENU_ACTION_DEBUG]           = {sText_MenuDebug,   {.u8_void = StartMenuDebugCallback}},
};

static const struct MenuAction sShortcutsMenuItems[] =
{
    [MENU_ACTION_PREVENT_ENCOUNTERS] = {gText_MenuPreventEncounters, {.u8_void = ShortcutsMenuPreventEncountersCallback}},
    [MENU_ACTION_SWEET_SCENT]        = {gText_MenuSweetScent,        {.u8_void = ShortcutsMenuSweetScentCallback}},
    [MENU_ACTION_AUTO_RUN_TOGGLE]    = {gText_MenuAutoRun,           {.u8_void = ShortcutsMenuAutoRunToggleCallback}},
    [MENU_ACTION_EXIT_DUNGEON]       = {gText_MenuExitDungeon,       {.u8_void = ShortcutsMenuExitDungeonCallback}},
    [MENU_ACTION_FAST_TRAVEL]        = {gText_MenuFastTravel,        {.u8_void = ShortcutsMenuFastTravelCallback}},
};

static bool8 bShortcutsMenuItemsUsable[] =
    {
        [MENU_ACTION_PREVENT_ENCOUNTERS] = FALSE,
        [MENU_ACTION_SWEET_SCENT]        = FALSE,
        [MENU_ACTION_AUTO_RUN_TOGGLE]    = FALSE,
        [MENU_ACTION_EXIT_DUNGEON]       = FALSE,
        [MENU_ACTION_FAST_TRAVEL]        = FALSE,
    };

static const struct BgTemplate sBgTemplates_LinkBattleSave[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    }
};

static const struct WindowTemplate sWindowTemplates_LinkBattleSave[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x194
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sSaveInfoWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 8
};

// Local functions
static void BuildStartMenuActions(void);
static void BuildKeyItemsMenuActions(void);
static void AddSubMenuAction(u8 action);
static void BuildNormalStartMenu(void);
static void BuildDebugStartMenu(void);
static void BuildSafariZoneStartMenu(void);
static void BuildLinkModeStartMenu(void);
static void BuildUnionRoomStartMenu(void);
static void BuildBattlePikeStartMenu(void);
static void BuildBattlePyramidStartMenu(void);
static void BuildMultiPartnerRoomStartMenu(void);
static void ShowGameVersionWindow(void);
static void ShowSafariBallsWindow(void);
static void ShowPyramidFloorWindow(void);
static void RemoveExtraStartMenuWindows(void);
static bool32 PrintStartMenuActions(s8 *pIndex, u32 count);
static bool32 PrintKeyItemsMenuActions(s8 *pIndex, u32 count);
static bool32 PrintShortcutsMenuActions(s8 *pIndex, u32 count);
static bool32 InitStartMenuStep(void);
static bool32 InitKeyItemsMenuStep(void);
static bool32 InitShortcutsMenuStep(void);
static void InitStartMenu(void);
static void CreateSubMenuTask(TaskFunc currFunc, TaskFunc followupFunc);
static void InitSave(void);
static u8 RunSaveCallback(void);
static void ShowSaveMessage(const u8 *message, u8 (*saveCallback)(void));
static void HideSaveMessageWindow(void);
static void HideSaveInfoWindow(void);
static void SaveStartTimer(void);
static bool8 SaveSuccesTimer(void);
static bool8 SaveErrorTimer(void);
static void InitBattlePyramidRetire(void);
static void VBlankCB_LinkBattleSave(void);
static bool32 InitSaveWindowAfterLinkBattle(u8 *par1);
static void CB2_SaveAfterLinkBattle(void);
static void ShowSaveInfoWindow(void);
static void RemoveSaveInfoWindow(void);
static void HideSubMenuWindow(void);
static void HideStartMenuDebug(void);

void DrawShortcutAction(u8 index)
{
    u8 textColor = TEXT_COLOR_DARK_GRAY;
    u8 shadowColor = TEXT_COLOR_LIGHT_GRAY;
    // If the action is unusable, draw it disabled
    if (bShortcutsMenuItemsUsable[sCurrentSubMenuActions[index]] == FALSE)
    {
        textColor = TEXT_COLOR_LIGHT_RED;
        shadowColor = TEXT_COLOR_LIGHT_GRAY;
    }
    // If the action is Auto Run Toggle, draw its current status
    else if (index == MENU_ACTION_AUTO_RUN_TOGGLE)
    {
        if (FlagGet(FLAG_SYS_IS_RUNNING_TOGGLED))
        {
            // Added 2 extra spaces at the end to clear up the "OFF" leftover text
            StringCopy(gStringVar1, COMPOUND_STRING("ON  "));
            textColor = TEXT_COLOR_GREEN;
            shadowColor = TEXT_COLOR_LIGHT_GREEN;
        }
        else
        {
            StringCopy(gStringVar1, COMPOUND_STRING("OFF"));
            textColor = TEXT_COLOR_RED;
            shadowColor = TEXT_COLOR_LIGHT_RED;
        }
    }
    
    StringExpandPlaceholders(gStringVar4, sShortcutsMenuItems[sCurrentSubMenuActions[index]].text);
    AddTextPrinterParameterizedWithColor(GetSubMenuWindowId(), FONT_NORMAL, gStringVar4, 8, (index << 4) + 9, TEXT_SKIP_DRAW, textColor, shadowColor, NULL);
}

void SetDexPokemonPokenavFlags(void) // unused
{
    FlagSet(FLAG_SYS_POKEDEX_GET);
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_SYS_POKENAV_GET);
}

static void BuildStartMenuActions(void)
{
    sNumSubMenuActions = 0;
    sMenuOpened = MENU_START;

    if (IsOverworldLinkActive() == TRUE)
    {
        BuildLinkModeStartMenu();
    }
    else if (InUnionRoom() == TRUE)
    {
        BuildUnionRoomStartMenu();
    }
    else if (GetSafariZoneFlag() == TRUE)
    {
        BuildSafariZoneStartMenu();
    }
    else if (InBattlePike())
    {
        BuildBattlePikeStartMenu();
    }
    else if (InBattlePyramid())
    {
        BuildBattlePyramidStartMenu();
    }
    else if (InMultiPartnerRoom())
    {
        BuildMultiPartnerRoomStartMenu();
    }
    else
    {
        if (DEBUG_OVERWORLD_MENU == TRUE && DEBUG_OVERWORLD_IN_MENU == TRUE)
            BuildDebugStartMenu();
        else
            BuildNormalStartMenu();
    }
}

static void BuildKeyItemsMenuActions(void)
{
    sNumSubMenuActions = GetAmountOfItemsRegistered();
    sMenuOpened = MENU_KEY_ITEMS;
}

static void BuildShortcutsMenuActions(void)
{
    sNumSubMenuActions = 0;
    sMenuOpened = MENU_SHORTCUTS;

    // Set the Prevent Encounters usable flag based on whether the player has any REPEL items
    bShortcutsMenuItemsUsable[MENU_ACTION_PREVENT_ENCOUNTERS] = CheckBagHasItem(ITEM_REPEL, 1) ||
                                                                CheckBagHasItem(ITEM_SUPER_REPEL, 1) ||
                                                                CheckBagHasItem(ITEM_MAX_REPEL, 1);

    // Set the Sweet Scent usable flag based on whether the player has a Pokemon that can use SWEET SCENT
    // NOTE: Sweet Scent is usable anywhere. Might make it unusable in a location where it doesn't have an effect
    bShortcutsMenuItemsUsable[MENU_ACTION_SWEET_SCENT] = IsMoveInParty(MOVE_SWEET_SCENT);

    // Set the Auto Run usable flag based on whether the player has the Running Shoes
    bShortcutsMenuItemsUsable[MENU_ACTION_AUTO_RUN_TOGGLE] = FlagGet(FLAG_SYS_B_DASH);

    // Set the Exit Dungeon usable flag based on whether the player can use DIG or ESCAPE ROPE here, including whether they have a Pokemon with DIG or an Escape Rope
    bShortcutsMenuItemsUsable[MENU_ACTION_EXIT_DUNGEON] = CanUseDigOrEscapeRopeOnCurMap() && (IsMoveInParty(MOVE_DIG) || CheckBagHasItem(ITEM_ESCAPE_ROPE, 1));

    // Set the Fast Travel usable flag based on whether the player can use FLY or TELEPORT here, including whether they have a Pokemon with the move
    bShortcutsMenuItemsUsable[MENU_ACTION_FAST_TRAVEL] = (CanUseTeleport() && IsMoveInParty(MOVE_TELEPORT)) || (CanUseFly() && IsMoveInParty(MOVE_FLY));

    // We check whether Auto Run is enabled, and copy the appropriate string into gStringVar1
    StringCopy(gStringVar1, (FlagGet(FLAG_SYS_B_DASH) && FlagGet(FLAG_SYS_IS_RUNNING_TOGGLED) ? COMPOUND_STRING("ON") : COMPOUND_STRING("OFF")));

    AddSubMenuAction(MENU_ACTION_PREVENT_ENCOUNTERS);
    AddSubMenuAction(MENU_ACTION_SWEET_SCENT);
    AddSubMenuAction(MENU_ACTION_AUTO_RUN_TOGGLE);
    AddSubMenuAction(MENU_ACTION_EXIT_DUNGEON);
    AddSubMenuAction(MENU_ACTION_FAST_TRAVEL);
}

static void AddSubMenuAction(u8 action)
{
    AppendToList(sCurrentSubMenuActions, &sNumSubMenuActions, action);
}

static void BuildNormalStartMenu(void)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
    {
        AddSubMenuAction(MENU_ACTION_POKEDEX);
    }
    if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
    {
        AddSubMenuAction(MENU_ACTION_POKEMON);
    }

    AddSubMenuAction(MENU_ACTION_BAG);

    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
    {
        AddSubMenuAction(MENU_ACTION_POKENAV);
    }

    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_SAVE);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void BuildDebugStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_DEBUG);
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
        AddSubMenuAction(MENU_ACTION_POKEDEX);
    if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
        AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_BAG);
    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
        AddSubMenuAction(MENU_ACTION_POKENAV);
    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_SAVE);
    AddSubMenuAction(MENU_ACTION_OPTION);
}

static void BuildSafariZoneStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_RETIRE_SAFARI);
    AddSubMenuAction(MENU_ACTION_POKEDEX);
    AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_BAG);
    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void BuildLinkModeStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_BAG);

    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
    {
        AddSubMenuAction(MENU_ACTION_POKENAV);
    }

    AddSubMenuAction(MENU_ACTION_PLAYER_LINK);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void BuildUnionRoomStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_BAG);

    if (FlagGet(FLAG_SYS_POKENAV_GET) == TRUE)
    {
        AddSubMenuAction(MENU_ACTION_POKENAV);
    }

    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void BuildBattlePikeStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_POKEDEX);
    AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void BuildBattlePyramidStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_PYRAMID_BAG);
    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_REST_FRONTIER);
    AddSubMenuAction(MENU_ACTION_RETIRE_FRONTIER);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void BuildMultiPartnerRoomStartMenu(void)
{
    AddSubMenuAction(MENU_ACTION_POKEMON);
    AddSubMenuAction(MENU_ACTION_PLAYER);
    AddSubMenuAction(MENU_ACTION_OPTION);
    AddSubMenuAction(MENU_ACTION_EXIT);
}

static void ShowGameVersionWindow(void)
{
    sGameVersionWindowId = AddWindow(&sWindowTemplate_GameVersion);
    PutWindowTilemap(sGameVersionWindowId);
    DrawStdWindowFrame(sGameVersionWindowId, FALSE);
    StringCopy(gStringVar1, C_GAME_VERSION);
    StringExpandPlaceholders(gStringVar4, gText_GameVersion);
    AddTextPrinterParameterized(sGameVersionWindowId, FONT_NORMAL, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sGameVersionWindowId, COPYWIN_GFX);
}

static void ShowSafariBallsWindow(void)
{
    sSafariBallsWindowId = AddWindow(&sWindowTemplate_SafariBalls);
    PutWindowTilemap(sSafariBallsWindowId);
    DrawStdWindowFrame(sSafariBallsWindowId, FALSE);
    ConvertIntToDecimalStringN(gStringVar1, gNumSafariBalls, STR_CONV_MODE_RIGHT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, gText_SafariBallStock);
    AddTextPrinterParameterized(sSafariBallsWindowId, FONT_NORMAL, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sSafariBallsWindowId, COPYWIN_GFX);
}

static void ShowPyramidFloorWindow(void)
{
    if (gSaveBlock2Ptr->frontier.curChallengeBattleNum == FRONTIER_STAGES_PER_CHALLENGE)
        sBattlePyramidFloorWindowId = AddWindow(&sWindowTemplate_PyramidPeak);
    else
        sBattlePyramidFloorWindowId = AddWindow(&sWindowTemplate_PyramidFloor);

    PutWindowTilemap(sBattlePyramidFloorWindowId);
    DrawStdWindowFrame(sBattlePyramidFloorWindowId, FALSE);
    StringCopy(gStringVar1, sPyramidFloorNames[gSaveBlock2Ptr->frontier.curChallengeBattleNum]);
    StringExpandPlaceholders(gStringVar4, gText_BattlePyramidFloor);
    AddTextPrinterParameterized(sBattlePyramidFloorWindowId, FONT_NORMAL, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sBattlePyramidFloorWindowId, COPYWIN_GFX);
}

static void RemoveExtraStartMenuWindows(void)
{
    if (GetSafariZoneFlag())
    {
        ClearStdWindowAndFrameToTransparent(sSafariBallsWindowId, FALSE);
        CopyWindowToVram(sSafariBallsWindowId, COPYWIN_GFX);
        RemoveWindow(sSafariBallsWindowId);
    }
    if (InBattlePyramid())
    {
        ClearStdWindowAndFrameToTransparent(sBattlePyramidFloorWindowId, FALSE);
        RemoveWindow(sBattlePyramidFloorWindowId);
    }

    ClearStdWindowAndFrameToTransparent(sGameVersionWindowId, FALSE);
    RemoveWindow(sGameVersionWindowId);
}

static bool32 PrintStartMenuActions(s8 *pIndex, u32 count)
{
    s8 index = *pIndex;

    do
    {
        if (sStartMenuItems[sCurrentSubMenuActions[index]].func.u8_void == StartMenuPlayerNameCallback)
        {
            PrintPlayerNameOnWindow(GetSubMenuWindowId(), sStartMenuItems[sCurrentSubMenuActions[index]].text, 8, (index << 4) + 9);
        }
        else
        {
            StringExpandPlaceholders(gStringVar4, sStartMenuItems[sCurrentSubMenuActions[index]].text);
            AddTextPrinterParameterized(GetSubMenuWindowId(), FONT_NORMAL, gStringVar4, 8, (index << 4) + 9, TEXT_SKIP_DRAW, NULL);
        }

        index++;
        if (index >= sNumSubMenuActions)
        {
            *pIndex = index;
            return TRUE;
        }

        count--;
    } while (count != 0);

    *pIndex = index;
    return FALSE;
}

static bool32 PrintKeyItemsMenuActions(s8 *pIndex, u32 count)
{
    s8 index = *pIndex;

    do
    {
        StringExpandPlaceholders(gStringVar4, gItemsInfo[GetRegisteredItem(index)].name);
        AddTextPrinterParameterized(GetSubMenuWindowId(), FONT_NORMAL, gStringVar4, 8, (index << 4) + 9, TEXT_SKIP_DRAW, NULL);

        index++;
        if (index >= sNumSubMenuActions)
        {
            *pIndex = index;
            return TRUE;
        }

        count--;
    } while (count != 0);

    *pIndex = index;
    return FALSE;
}

static bool32 PrintShortcutsMenuActions(s8 *pIndex, u32 count)
{
    s8 index = *pIndex;

    do
    {
        DrawShortcutAction(index);

        index++;
        if (index >= sNumSubMenuActions)
        {
            *pIndex = index;
            return TRUE;
        }

        count--;
    } while (count != 0);

    *pIndex = index;
    return FALSE;
}

// Handles the common submenu initialization steps
bool32 InitSubMenuStep(void)
{
    s8 state = sInitSubMenuData[0];

    switch (state)
    {
    case 0:
        sInitSubMenuData[0]++;
        break;
    case 1:
        sInitSubMenuData[0]++;
        break;
    case 2:
        LoadMessageBoxAndBorderGfx();
        sInitSubMenuData[1] = 0;
        sInitSubMenuData[0]++;
        break;
    case 3:
        sInitSubMenuData[0]++;
        break;
    // case 4:
    // Only increases after all Menu actions are printed
    case 5:
        sSubMenuCursorPos = InitMenuNormal(GetSubMenuWindowId(), FONT_NORMAL, 0, 9, 16, sNumSubMenuActions, sSubMenuCursorPos);
        CopyWindowToVram(GetSubMenuWindowId(), COPYWIN_MAP);
        return TRUE;
    }

    return FALSE;
}

static bool32 InitStartMenuStep(void)
{
    s8 state = sInitSubMenuData[0];

    switch (state)
    {
    case 1:
        BuildStartMenuActions();
        break;
    case 2:
        DrawStdWindowFrame(AddStartMenuWindow(sNumSubMenuActions), FALSE);
        break;
    case 3:
        if (GetSafariZoneFlag())
            ShowSafariBallsWindow();
        if (InBattlePyramid())
            ShowPyramidFloorWindow();
        ShowGameVersionWindow();
        break;
    case 4:
        if (PrintStartMenuActions(&sInitSubMenuData[1], 2))
            sInitSubMenuData[0]++;
        break;
    // case 5:
    }

    return InitSubMenuStep();
}

static bool32 InitKeyItemsMenuStep(void)
{
    s8 state = sInitSubMenuData[0];

    switch (state)
    {
    // case 0:
    //     break;
    case 1:
        BuildKeyItemsMenuActions();
        break;
    case 2:
        DrawStdWindowFrame(AddKeyItemsMenuWindow(sNumSubMenuActions), FALSE);
        break;
    // case 3:
    // Only the Start Menu has extra functionality here
    case 4:
        if (PrintKeyItemsMenuActions(&sInitSubMenuData[1], 2))
            sInitSubMenuData[0]++;
        break;
    // case 5:
    }

    return InitSubMenuStep();
}

static bool32 InitShortcutsMenuStep(void)
{
    s8 state = sInitSubMenuData[0];

    switch (state)
    {
    case 1:
        BuildShortcutsMenuActions();
        break;
    case 2:
        DrawStdWindowFrame(AddShortcutsMenuWindow(sNumSubMenuActions), FALSE);
        break;
    // case 3:
    // Only the Start Menu has extra functionality here
    case 4:
        if (PrintShortcutsMenuActions(&sInitSubMenuData[1], 2))
            sInitSubMenuData[0]++;
        break;
    // case 5:
    }

    return InitSubMenuStep();
}

static void InitStartMenu(void)
{
    sInitSubMenuData[0] = 0;
    sInitSubMenuData[1] = 0;
    while (!InitStartMenuStep())
        ;
}

static void StartMenuTask(u8 taskId)
{
    if (InitStartMenuStep() == TRUE)
        SwitchTaskToFollowupFunc(taskId);
}

static void KeyItemsMenuTask(u8 taskId)
{
    if (InitKeyItemsMenuStep() == TRUE)
        SwitchTaskToFollowupFunc(taskId);
}

static void ShortcutsMenuTask(u8 taskId)
{
    if (InitShortcutsMenuStep() == TRUE)
        SwitchTaskToFollowupFunc(taskId);
}

static void CreateSubMenuTask(TaskFunc currFunc, TaskFunc followupFunc)
{
    u8 taskId;

    sInitSubMenuData[0] = 0;
    sInitSubMenuData[1] = 0;
    taskId = CreateTask(currFunc, 0x50);
    SetTaskFuncWithFollowupFunc(taskId, currFunc, followupFunc);
}

static bool8 FieldCB_ReturnToFieldSubMenu(void)
{
    switch (sMenuOpened)
    {
    case MENU_START:
        return FieldCB_ReturnToFieldStartMenu();
    case MENU_SHORTCUTS:
        return FieldCB_ReturnToFieldShortcutsMenu();
    default:
        return FALSE;
    }
}

static bool8 FieldCB_ReturnToFieldStartMenu(void)
{
    if (InitStartMenuStep() == FALSE)
    {
        return FALSE;
    }

    ReturnToFieldOpenSubMenu();
    return TRUE;
}

static bool8 FieldCB_ReturnToFieldShortcutsMenu(void)
{
    if (InitShortcutsMenuStep() == FALSE)
    {
        return FALSE;
    }

    ReturnToFieldOpenSubMenu();
    return TRUE;
}

void ShowReturnToFieldSubMenu(void)
{
    sInitSubMenuData[0] = 0;
    sInitSubMenuData[1] = 0;
    gFieldCallback2 = FieldCB_ReturnToFieldSubMenu;
}

void ShowReturnToFieldStartMenu(void)
{
    sMenuOpened = MENU_START;
    ShowReturnToFieldSubMenu();
}

void ShowReturnToFieldShortcutsMenu(void)
{
    sMenuOpened = MENU_SHORTCUTS;
    ShowReturnToFieldSubMenu();
}

void ShowLastSubMenu()
{
    switch (sMenuOpened)
    {
    case MENU_START:
        ShowStartMenu();
        break;
    case MENU_KEY_ITEMS:
        ShowKeyItemsMenu();
        break;
    case MENU_SHORTCUTS:
        ShowShortcutsMenu();
        break;
    }
}

void Task_ShowSubMenu(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch(task->data[0])
    {
    case 0:
        if (InUnionRoom() == TRUE && sMenuOpened == MENU_START)
            SetUsingUnionRoomStartMenu();

        switch (sMenuOpened)
        {
        case MENU_KEY_ITEMS:
            gMenuCallback = HandleKeyItemsMenuInput;
            break;
        case MENU_SHORTCUTS:
            gMenuCallback = HandleShortcutsMenuInput;
            break;
        default:
            gMenuCallback = HandleStartMenuInput;
            break;
        }
        task->data[0]++;
        break;
    case 1:
        if (gMenuCallback() == TRUE)
        {
            if (sMenuOpened == MENU_SHORTCUTS)
            {
                HideSubMenu(TRUE);
            }
            
            DestroyTask(taskId);
        }
        break;
    }
}

void ShowStartMenu(void)
{
    ShowSubMenu(StartMenuTask, Task_ShowSubMenu);
}

void ShowKeyItemsMenu(void)
{
    ShowSubMenu(KeyItemsMenuTask, Task_ShowSubMenu);
}

void ShowShortcutsMenu(void)
{
    ShowSubMenu(ShortcutsMenuTask, Task_ShowSubMenu);
}

void ShowSubMenu(TaskFunc currFunc, TaskFunc followupFunc)
{
    if (!IsOverworldLinkActive())
    {
        FreezeObjectEvents();
        PlayerFreeze();
        StopPlayerAvatar();
    }
    CreateSubMenuTask(currFunc, followupFunc);
    LockPlayerFieldControls();
}

void HandleSubMenuInput(void)
{
    if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        sSubMenuCursorPos = Menu_MoveCursor(-1);
    }

    if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        sSubMenuCursorPos = Menu_MoveCursor(1);
    }
}

static bool8 HandleStartMenuInput(void)
{
    HandleSubMenuInput();

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sStartMenuItems[sCurrentSubMenuActions[sSubMenuCursorPos]].func.u8_void == StartMenuPokedexCallback)
        {
            if (GetNationalPokedexCount(FLAG_GET_SEEN) == 0)
                return FALSE;
        }

        gMenuCallback = sStartMenuItems[sCurrentSubMenuActions[sSubMenuCursorPos]].func.u8_void;

        if (gMenuCallback != StartMenuSaveCallback
            && gMenuCallback != StartMenuExitCallback
            && gMenuCallback != StartMenuDebugCallback
            && gMenuCallback != StartMenuSafariZoneRetireCallback
            && gMenuCallback != StartMenuBattlePyramidRetireCallback)
        {
            FadeScreen(FADE_TO_BLACK, 0);
        }

        return FALSE;
    }

    if (JOY_NEW(START_BUTTON | B_BUTTON))
    {
        RemoveExtraStartMenuWindows();
        HideSubMenu(TRUE);
        return TRUE;
    }

    return FALSE;
}

static bool8 HandleKeyItemsMenuInput(void)
{
    HandleSubMenuInput();

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);

        // Try to use the Key Item selected
        UseRegisteredKeyItemOnField(GetRegisteredItem(sSubMenuCursorPos));

        // If a Key Item was used, we need to keep the player's controls locked until after the item effect is done. Otherwise, just close the menu.
        HideSubMenu(FALSE);

        return TRUE;
    }

    if (JOY_NEW(B_BUTTON))
    {
        HideSubMenu(TRUE);
        return TRUE;
    }

    return FALSE;
}

static bool8 HandleShortcutsMenuInput(void)
{
    HandleSubMenuInput();

    if (JOY_NEW(A_BUTTON))
    {
        // If the selected shortcut is usable, call its callback function
        if (bShortcutsMenuItemsUsable[sCurrentSubMenuActions[sSubMenuCursorPos]])
        {
            PlaySE(SE_SELECT);

            gMenuCallback = sShortcutsMenuItems[sCurrentSubMenuActions[sSubMenuCursorPos]].func.u8_void;
        }
        else
        {
            PlaySE(SE_FAILURE);
        }

        return FALSE;
    }

    if (JOY_NEW(B_BUTTON))
    {
        // It reaches here properly after coming back from fast travel, so the TRUE is no longer working correctly somehow
        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuPokedexCallback(void)
{
    if (!gPaletteFade.active)
    {
        IncrementGameStat(GAME_STAT_CHECKED_POKEDEX);
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_OpenPokedex);

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuPokemonCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_PartyMenuFromStartMenu); // Display party menu

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuBagCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_BagMenuFromStartMenu); // Display bag menu

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuPokeNavCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_InitPokeNav); // Display PokéNav

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuPlayerNameCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();

        if (IsOverworldLinkActive() || InUnionRoom())
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
        else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
            ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
        else
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuSaveCallback(void)
{
    if (InBattlePyramid())
        RemoveExtraStartMenuWindows();

    gMenuCallback = SaveStartCallback; // Display save menu

    return FALSE;
}

static bool8 StartMenuOptionCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_InitOptionMenu); // Display option menu
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuExitCallback(void)
{
    RemoveExtraStartMenuWindows();
    HideSubMenu(TRUE); // Hide start menu

    return TRUE;
}

static bool8 StartMenuDebugCallback(void)
{
    RemoveExtraStartMenuWindows();
    HideStartMenuDebug(); // Hide start menu without enabling movement

#if DEBUG_OVERWORLD_MENU == TRUE
    FreezeObjectEvents();
    Debug_ShowMainMenu();
#endif

    return TRUE;
}

static bool8 StartMenuSafariZoneRetireCallback(void)
{
    RemoveExtraStartMenuWindows();
    HideSubMenu(TRUE);
    SafariZoneRetirePrompt();

    return TRUE;
}

static void HideStartMenuDebug(void)
{
    PlaySE(SE_SELECT);
    ClearStdWindowAndFrame(GetSubMenuWindowId(), TRUE);
    RemoveSubMenuWindow();
}

static bool8 StartMenuLinkModePlayerNameCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        CleanupOverworldWindowsAndTilemaps();
        ShowTrainerCardInLink(gLocalLinkPlayerId, CB2_ReturnToFieldWithOpenMenu);

        return TRUE;
    }

    return FALSE;
}

static bool8 StartMenuBattlePyramidRetireCallback(void)
{
    gMenuCallback = BattlePyramidRetireStartCallback; // Confirm retire

    return FALSE;
}

// Functionally unused
void ShowBattlePyramidStartMenu(void)
{
    ClearDialogWindowAndFrameToTransparent(0, FALSE);
    ScriptUnfreezeObjectEvents();
    CreateSubMenuTask(StartMenuTask, Task_ShowSubMenu);
    LockPlayerFieldControls();
}

static bool8 StartMenuBattlePyramidBagCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveExtraStartMenuWindows();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_PyramidBagMenuFromStartMenu);

        return TRUE;
    }

    return FALSE;
}

static bool8 SaveStartCallback(void)
{
    InitSave();
    gMenuCallback = SaveCallback;

    return FALSE;
}

static bool8 SaveCallback(void)
{
    switch (RunSaveCallback())
    {
    case SAVE_IN_PROGRESS:
        return FALSE;
    case SAVE_CANCELED: // Back to start menu
        ClearDialogWindowAndFrameToTransparent(0, FALSE);
        InitStartMenu();
        gMenuCallback = HandleStartMenuInput;
        return FALSE;
    case SAVE_SUCCESS:
    case SAVE_ERROR: // Close start menu
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ScriptUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        SoftResetInBattlePyramid();
        return TRUE;
    }

    return FALSE;
}

static bool8 BattlePyramidRetireStartCallback(void)
{
    InitBattlePyramidRetire();
    gMenuCallback = BattlePyramidRetireCallback;

    return FALSE;
}

static bool8 BattlePyramidRetireReturnCallback(void)
{
    InitStartMenu();
    gMenuCallback = HandleStartMenuInput;

    return FALSE;
}

static bool8 BattlePyramidRetireCallback(void)
{
    switch (RunSaveCallback())
    {
    case SAVE_SUCCESS: // No (Stay in battle pyramid)
        RemoveExtraStartMenuWindows();
        gMenuCallback = BattlePyramidRetireReturnCallback;
        return FALSE;
    case SAVE_IN_PROGRESS:
        return FALSE;
    case SAVE_CANCELED: // Yes (Retire from battle pyramid)
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ScriptUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        ScriptContext_SetupScript(BattlePyramid_Retire);
        return TRUE;
    }

    return FALSE;
}

static bool8 ShortcutsMenuPreventEncountersCallback(void)
{
    // SetMainCallback2(DrawSprayMenu);

    return FALSE;
}

static bool8 ShortcutsMenuSweetScentCallback(void)
{
    // We call the Sweet Scent setup passing the first Pokemon in the party that knows the move
    FieldCallback_SweetScentSetup(GetFirstMonWithMoveInParty(MOVE_SWEET_SCENT));
    
    return TRUE;
}

// Returns TRUE if the menu should close, FALSE if it should stay open
static bool8 ShortcutsMenuAutoRunToggleCallback(void)
{
    // Toggles the running shoes' speed
    FlagToggle(FLAG_SYS_IS_RUNNING_TOGGLED);
    if (FlagGet(FLAG_SYS_IS_RUNNING_TOGGLED))
    {
        PlaySE(SE_PC_LOGIN);
    }
    else
    {
        PlaySE(SE_PC_OFF);
    }

    // Redraw the Auto Run menu line
    DrawShortcutAction(MENU_ACTION_AUTO_RUN_TOGGLE);
    sSubMenuCursorPos = InitMenuNormal(GetSubMenuWindowId(), FONT_NORMAL, 0, 9, 16, sNumSubMenuActions, sSubMenuCursorPos);
    CopyWindowToVram(GetSubMenuWindowId(), COPYWIN_MAP);

    gMenuCallback = HandleShortcutsMenuInput;

    return FALSE;
}

static bool8 ShortcutsMenuExitDungeonCallback(void)
{
    // Check to see if the player can use DIG here. If they can, use it.
    if (IsMoveInParty(MOVE_DIG))
    {
        FieldCallback_DigSetup(GetFirstMonWithMoveInParty(MOVE_DIG));
        return TRUE;
    }
    // If they can't, check to see if they can use ESCAPE ROPE here. If they can, use it.
    else if (CheckBagHasItem(ITEM_ESCAPE_ROPE, 1))
    {
        // We hide the submenu now because the window is used when using an item
        UseItemFromShortcutMenu(ITEM_ESCAPE_ROPE);
        return TRUE;
    }
    
    // If they can't, then do nothing and return (Though this is checked beforehand)
    return FALSE;
}

static bool8 ShortcutsMenuFastTravelCallback(void)
{
    // Check to see if the player can use FLY here. If they can, use it.
    if (CanUseFly() && IsMoveInParty(MOVE_FLY))
    {
        // Open the Fly map here
        OpenFastTravelMapFromShortcut(TRUE, GetFirstMonWithMoveInParty(MOVE_FLY));
        return TRUE;
    }
    // If they can't, check to see if they can use TELEPORT here. If they can, use it.
    if (CanUseTeleport() && IsMoveInParty(MOVE_TELEPORT))
    {
        // Open the Teleport map here
        OpenFastTravelMapFromShortcut(FALSE, GetFirstMonWithMoveInParty(MOVE_TELEPORT));
        return TRUE;
    }

    return TRUE;
}

static void InitSave(void)
{
    SaveMapView();
    sSaveDialogCallback = SaveConfirmSaveCallback;
    sSavingComplete = FALSE;
}

static u8 RunSaveCallback(void)
{
    // True if text is still printing
    if (RunTextPrintersAndIsPrinter0Active() == TRUE)
    {
        return SAVE_IN_PROGRESS;
    }

    sSavingComplete = FALSE;
    return sSaveDialogCallback();
}

void SaveGame(void)
{
    InitSave();
    CreateTask(SaveGameTask, 0x50);
}

static void ShowSaveMessage(const u8 *message, u8 (*saveCallback)(void))
{
    StringExpandPlaceholders(gStringVar4, message);
    LoadMessageBoxAndFrameGfx(0, TRUE);
    AddTextPrinterForMessage_2(TRUE);
    sSavingComplete = TRUE;
    sSaveDialogCallback = saveCallback;
}

static void SaveGameTask(u8 taskId)
{
    u8 status = RunSaveCallback();

    switch (status)
    {
    case SAVE_CANCELED:
    case SAVE_ERROR:
        gSpecialVar_Result = 0;
        break;
    case SAVE_SUCCESS:
        gSpecialVar_Result = status;
        break;
    case SAVE_IN_PROGRESS:
        return;
    }

    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void HideSaveMessageWindow(void)
{
    ClearDialogWindowAndFrame(0, TRUE);
}

static void HideSaveInfoWindow(void)
{
    RemoveSaveInfoWindow();
}

static void SaveStartTimer(void)
{
    sSaveDialogTimer = 60;
}

static bool8 SaveSuccesTimer(void)
{
    sSaveDialogTimer--;

    if (JOY_HELD(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        return TRUE;
    }
    if (sSaveDialogTimer == 0)
    {
        return TRUE;
    }

    return FALSE;
}

static bool8 SaveErrorTimer(void)
{
    if (sSaveDialogTimer != 0)
    {
        sSaveDialogTimer--;
    }
    else if (JOY_HELD(A_BUTTON))
    {
        return TRUE;
    }

    return FALSE;
}

static u8 SaveConfirmSaveCallback(void)
{
    ClearStdWindowAndFrame(GetSubMenuWindowId(), FALSE);
    RemoveSubMenuWindow();
    ShowSaveInfoWindow();

    if (InBattlePyramid())
    {
        ShowSaveMessage(gText_BattlePyramidConfirmRest, SaveYesNoCallback);
    }
    else
    {
        ShowSaveMessage(gText_ConfirmSave, SaveYesNoCallback);
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveYesNoCallback(void)
{
    DisplayYesNoMenuDefaultYes(); // Show Yes/No menu
    sSaveDialogCallback = SaveConfirmInputCallback;
    return SAVE_IN_PROGRESS;
}

static u8 SaveConfirmInputCallback(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        switch (gSaveFileStatus)
        {
        case SAVE_STATUS_EMPTY:
        case SAVE_STATUS_CORRUPT:
            if (gDifferentSaveFile == FALSE)
            {
                sSaveDialogCallback = SaveFileExistsCallback;
                return SAVE_IN_PROGRESS;
            }

            sSaveDialogCallback = SaveSavingMessageCallback;
            return SAVE_IN_PROGRESS;
        default:
            sSaveDialogCallback = SaveFileExistsCallback;
            return SAVE_IN_PROGRESS;
        }
    case MENU_B_PRESSED:
    case 1: // No
        HideSaveInfoWindow();
        HideSaveMessageWindow();
        return SAVE_CANCELED;
    }

    return SAVE_IN_PROGRESS;
}

// A different save file exists
static u8 SaveFileExistsCallback(void)
{
    if (gDifferentSaveFile == TRUE)
    {
        ShowSaveMessage(gText_DifferentSaveFile, SaveConfirmOverwriteDefaultNoCallback);
    }
    else
    {
        sSaveDialogCallback = SaveSavingMessageCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveConfirmOverwriteDefaultNoCallback(void)
{
    DisplayYesNoMenuWithDefault(1); // Show Yes/No menu (No selected as default)
    sSaveDialogCallback = SaveOverwriteInputCallback;
    return SAVE_IN_PROGRESS;
}

static u8 SaveOverwriteInputCallback(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        sSaveDialogCallback = SaveSavingMessageCallback;
        return SAVE_IN_PROGRESS;
    case MENU_B_PRESSED:
    case 1: // No
        HideSaveInfoWindow();
        HideSaveMessageWindow();
        return SAVE_CANCELED;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveSavingMessageCallback(void)
{
    ShowSaveMessage(gText_SavingDontTurnOff, SaveDoSaveCallback);
    return SAVE_IN_PROGRESS;
}

static u8 SaveDoSaveCallback(void)
{
    u8 saveStatus;

    IncrementGameStat(GAME_STAT_SAVED_GAME);
    PausePyramidChallenge();

    if (gDifferentSaveFile == TRUE)
    {
        saveStatus = TrySavingData(SAVE_OVERWRITE_DIFFERENT_FILE);
        gDifferentSaveFile = FALSE;
    }
    else
    {
        saveStatus = TrySavingData(SAVE_NORMAL);
    }

    if (saveStatus == SAVE_STATUS_OK)
        ShowSaveMessage(gText_PlayerSavedGame, SaveSuccessCallback);
    else
        ShowSaveMessage(gText_SaveError, SaveErrorCallback);

    SaveStartTimer();
    return SAVE_IN_PROGRESS;
}

static u8 SaveSuccessCallback(void)
{
    if (!IsTextPrinterActive(0))
    {
        PlaySE(SE_SAVE);
        sSaveDialogCallback = SaveReturnSuccessCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveReturnSuccessCallback(void)
{
    if (!IsSEPlaying() && SaveSuccesTimer())
    {
        HideSaveInfoWindow();
        return SAVE_SUCCESS;
    }
    else
    {
        return SAVE_IN_PROGRESS;
    }
}

static u8 SaveErrorCallback(void)
{
    if (!IsTextPrinterActive(0))
    {
        PlaySE(SE_BOO);
        sSaveDialogCallback = SaveReturnErrorCallback;
    }

    return SAVE_IN_PROGRESS;
}

static u8 SaveReturnErrorCallback(void)
{
    if (!SaveErrorTimer())
    {
        return SAVE_IN_PROGRESS;
    }
    else
    {
        HideSaveInfoWindow();
        return SAVE_ERROR;
    }
}

static void InitBattlePyramidRetire(void)
{
    sSaveDialogCallback = BattlePyramidConfirmRetireCallback;
    sSavingComplete = FALSE;
}

static u8 BattlePyramidConfirmRetireCallback(void)
{
    ClearStdWindowAndFrame(GetSubMenuWindowId(), FALSE);
    RemoveSubMenuWindow();
    ShowSaveMessage(gText_BattlePyramidConfirmRetire, BattlePyramidRetireYesNoCallback);

    return SAVE_IN_PROGRESS;
}

static u8 BattlePyramidRetireYesNoCallback(void)
{
    DisplayYesNoMenuWithDefault(1); // Show Yes/No menu (No selected as default)
    sSaveDialogCallback = BattlePyramidRetireInputCallback;

    return SAVE_IN_PROGRESS;
}

static u8 BattlePyramidRetireInputCallback(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        return SAVE_CANCELED;
    case MENU_B_PRESSED:
    case 1: // No
        HideSaveMessageWindow();
        return SAVE_SUCCESS;
    }

    return SAVE_IN_PROGRESS;
}

static void VBlankCB_LinkBattleSave(void)
{
    TransferPlttBuffer();
}

static bool32 InitSaveWindowAfterLinkBattle(u8 *state)
{
    switch (*state)
    {
    case 0:
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
        SetVBlankCallback(NULL);
        ScanlineEffect_Stop();
        DmaClear16(3, PLTT, PLTT_SIZE);
        DmaFillLarge16(3, 0, (void *)VRAM, VRAM_SIZE, 0x1000);
        break;
    case 1:
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        ScanlineEffect_Clear();
        break;
    case 2:
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates_LinkBattleSave, ARRAY_COUNT(sBgTemplates_LinkBattleSave));
        InitWindows(sWindowTemplates_LinkBattleSave);
        LoadUserWindowBorderGfx_(0, 8, BG_PLTT_ID(14));
        Menu_LoadStdPalAt(BG_PLTT_ID(15));
        break;
    case 3:
        ShowBg(0);
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        SetVBlankCallback(VBlankCB_LinkBattleSave);
        EnableInterrupts(1);
        break;
    case 4:
        return TRUE;
    }

    (*state)++;
    return FALSE;
}

void CB2_SetUpSaveAfterLinkBattle(void)
{
    if (InitSaveWindowAfterLinkBattle(&gMain.state))
    {
        CreateTask(Task_SaveAfterLinkBattle, 0x50);
        SetMainCallback2(CB2_SaveAfterLinkBattle);
    }
}

static void CB2_SaveAfterLinkBattle(void)
{
    RunTasks();
    UpdatePaletteFade();
}

static void Task_SaveAfterLinkBattle(u8 taskId)
{
    s16 *state = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        switch (*state)
        {
        case 0:
            FillWindowPixelBuffer(0, PIXEL_FILL(1));
            AddTextPrinterParameterized2(0,
                                         FONT_NORMAL,
                                         gText_SavingDontTurnOffPower,
                                         TEXT_SKIP_DRAW,
                                         NULL,
                                         TEXT_COLOR_DARK_GRAY,
                                         TEXT_COLOR_WHITE,
                                         TEXT_COLOR_LIGHT_GRAY);
            DrawTextBorderOuter(0, 8, 14);
            PutWindowTilemap(0);
            CopyWindowToVram(0, COPYWIN_FULL);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);

            if (gWirelessCommType != 0 && InUnionRoom())
            {
                if (Link_AnyPartnersPlayingFRLG_JP())
                {
                    *state = 1;
                }
                else
                {
                    *state = 5;
                }
            }
            else
            {
                gSoftResetDisabled = TRUE;
                *state = 1;
            }
            break;
        case 1:
            SetContinueGameWarpStatusToDynamicWarp();
            WriteSaveBlock2();
            *state = 2;
            break;
        case 2:
            if (WriteSaveBlock1Sector())
            {
                ClearContinueGameWarpStatus2();
                *state = 3;
                gSoftResetDisabled = FALSE;
            }
            break;
        case 3:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            *state = 4;
            break;
        case 4:
            FreeAllWindowBuffers();
            SetMainCallback2(gMain.savedCallback);
            DestroyTask(taskId);
            break;
        case 5:
            CreateTask(Task_LinkFullSave, 5);
            *state = 6;
            break;
        case 6:
            if (!FuncIsActiveTask(Task_LinkFullSave))
            {
                *state = 3;
            }
            break;
        }
    }
}

static void ShowSaveInfoWindow(void)
{
    struct WindowTemplate saveInfoWindow = sSaveInfoWindowTemplate;
    u8 gender;
    u8 color;
    u32 xOffset;
    u32 yOffset;

    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
    {
        saveInfoWindow.height -= 2;
    }

    sSaveInfoWindowId = AddWindow(&saveInfoWindow);
    DrawStdWindowFrame(sSaveInfoWindowId, FALSE);

    gender = gSaveBlock2Ptr->playerGender;
    color = TEXT_COLOR_RED; // Red when female, blue when male.

    if (gender == MALE)
    {
        color = TEXT_COLOR_BLUE;
    }

    // Print region name
    yOffset = 1;
    BufferSaveMenuText(SAVE_MENU_LOCATION, gStringVar4, TEXT_COLOR_GREEN);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, 0, yOffset, TEXT_SKIP_DRAW, NULL);

    // Print player name
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingPlayer, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_NAME, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    PrintPlayerNameOnWindow(sSaveInfoWindowId, gStringVar4, xOffset, yOffset);

    // Print badge count
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingBadges, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_BADGES, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
    {
        // Print Pokédex count
        yOffset += 16;
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingPokedex, 0, yOffset, TEXT_SKIP_DRAW, NULL);
        BufferSaveMenuText(SAVE_MENU_CAUGHT, gStringVar4, color);
        xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
        AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);
    }

    // Print play time
    yOffset += 16;
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gText_SavingTime, 0, yOffset, TEXT_SKIP_DRAW, NULL);
    BufferSaveMenuText(SAVE_MENU_PLAY_TIME, gStringVar4, color);
    xOffset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar4, 0x70);
    AddTextPrinterParameterized(sSaveInfoWindowId, FONT_NORMAL, gStringVar4, xOffset, yOffset, TEXT_SKIP_DRAW, NULL);

    CopyWindowToVram(sSaveInfoWindowId, COPYWIN_GFX);
}

static void RemoveSaveInfoWindow(void)
{
    ClearStdWindowAndFrame(sSaveInfoWindowId, FALSE);
    RemoveWindow(sSaveInfoWindowId);
}

static void Task_WaitForBattleTowerLinkSave(u8 taskId)
{
    if (!FuncIsActiveTask(Task_LinkFullSave))
    {
        DestroyTask(taskId);
        ScriptContext_Enable();
    }
}

#define tInBattleTower data[2]

void SaveForBattleTowerLink(void)
{
    u8 taskId = CreateTask(Task_LinkFullSave, 5);
    gTasks[taskId].tInBattleTower = TRUE;
    gTasks[CreateTask(Task_WaitForBattleTowerLinkSave, 6)].data[1] = taskId;
}

#undef tInBattleTower

// Hides the sub menu (we don't unlock the player's controls here because some menus (like key items) need to keep them locked)
static void HideSubMenuWindow(void)
{
    ClearStdWindowAndFrame(GetSubMenuWindowId(), TRUE);
    RemoveSubMenuWindow();
    ScriptUnfreezeObjectEvents();
}

void HideSubMenu(bool8 unlockControls)
{
    PlaySE(SE_SELECT);
    HideSubMenuWindow();
    if (unlockControls)
    {
        UnlockPlayerFieldControls();
    }
    sMenuOpened = MENU_NONE;
}

void AppendToList(u8 *list, u8 *pos, u8 newEntry)
{
    list[*pos] = newEntry;
    (*pos)++;
}
