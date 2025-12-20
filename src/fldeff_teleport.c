#include "global.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "fldeff.h"
#include "party_menu.h"
#include "region_map.h"
#include "overworld.h"
#include "task.h"
#include "constants/field_effects.h"

static void FieldCallback_Teleport(void);
static void StartTeleportFieldEffect(void);

bool8 CanUseTeleport(void)
{
    // VAR_ELITE_4_STATE indicates whether the player is currently challenging the Elite Four. 0 means they are not.
    return VarGet(VAR_ELITE_4_STATE) == 0 && FlagGet(FLAG_CAN_TELEPORT);
}

bool8 SetUpFieldMove_Teleport(void)
{
    if (!CanUseTeleport())
    {
        return FALSE;
    }

    gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
    gPostMenuFieldCallback = FieldCallback_Teleport;
    return TRUE;
}

static void FieldCallback_Teleport(void)
{
    Overworld_ResetStateAfterTeleport();
    FieldEffectStart(FLDEFF_USE_TELEPORT);
    gFieldEffectArguments[0] = (u32)(IsWarpingFromShortcuts() ? GetMonUsingFieldMoveIndex() : GetCursorSelectionMonId());
}

bool8 FldEff_UseTeleport(void)
{
    u8 taskId = CreateFieldMoveTask();
    gTasks[taskId].data[8] = (u32)StartTeleportFieldEffect >> 16;
    gTasks[taskId].data[9] = (u32)StartTeleportFieldEffect;
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
    return FALSE;
}

static void StartTeleportFieldEffect(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_TELEPORT);
    FldEff_TeleportWarpOut();
}
