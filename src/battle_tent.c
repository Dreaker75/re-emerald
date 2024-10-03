#include "global.h"
#include "battle_tent.h"
#include "overworld.h"
#include "event_data.h"
#include "region_map.h"
#include "battle.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "random.h"
#include "item.h"
#include "battle_factory_screen.h"
#include "frontier_util.h"
#include "string_util.h"
#include "constants/battle_tent.h"
#include "constants/battle_tent_trainers.h"
#include "constants/battle_tent_mons.h"
#include "constants/items.h"
#include "constants/layouts.h"
#include "constants/region_map_sections.h"
#include "constants/trainers.h"

// This file's functions.
static void InitVerdanturfTentChallenge(void);
static void GetVerdanturfTentPrizeHeld(void);
static void SetVerdanturfTentPrizeHeld(void);
static void SetVerdanturfTentTrainerGfx(void);
static void BufferVerdanturfTentTrainerIntro(void);
static void SaveVerdanturfTentChallenge(void);
static void GiveVerdanturfTentPrize(void);
static void InitFallarborTentChallenge(void);
static void GetFallarborTentPrizeHeld(void);
static void SetFallarborTentPrizeHeld(void);
static void SaveFallarborTentChallenge(void);
static void GiveFallarborTentPrize(void);
static void BufferFallarborTentTrainerName(void);
static void InitSlateportTentChallenge(void);
static void GetSlateportTentPrizeHeld(void);
static void SetSlateportTentPrizeHeld(void);
static void SaveSlateportTentChallenge(void);
static void GiveSlateportTentPrize(void);
static void SelectInitialRentalMons(void);
static void SwapRentalMons(void);
static void GenerateOpponentMons(void);
static void GenerateInitialRentalMons(void);

/*
 * Battle Tents are mini versions of particular Battle Frontier facilities
 * As such they each share some scripts and functions with their counterpart
 *
 * Verdanturf Battle Tent: Battle Palace
 * Fallarbor Battle Tent:  Battle Arena
 * Slateport Battle Tent:  Battle Factory
 *
 */

static u16 sRandMonId;

void static (*const sVerdanturfTentFuncs[])(void) =
{
    [VERDANTURF_TENT_FUNC_INIT]               = InitVerdanturfTentChallenge,
    [VERDANTURF_TENT_FUNC_GET_PRIZE_HELD]     = GetVerdanturfTentPrizeHeld,
    [VERDANTURF_TENT_FUNC_SET_PRIZE_HELD]     = SetVerdanturfTentPrizeHeld,
    [VERDANTURF_TENT_FUNC_SET_OPPONENT_GFX]   = SetVerdanturfTentTrainerGfx,
    [VERDANTURF_TENT_FUNC_GET_OPPONENT_INTRO] = BufferVerdanturfTentTrainerIntro,
    [VERDANTURF_TENT_FUNC_SAVE]               = SaveVerdanturfTentChallenge,
    [VERDANTURF_TENT_FUNC_GIVE_PRIZE]         = GiveVerdanturfTentPrize
};

static const u16 sVerdanturfTentReward = ITEM_RARE_CANDY;
static const u8 sVerdanturfTentRewardAmount = 5;

void static (*const sFallarborTentFuncs[])(void) =
{
    [FALLARBOR_TENT_FUNC_INIT]              = InitFallarborTentChallenge,
    [FALLARBOR_TENT_FUNC_GET_PRIZE_HELD]    = GetFallarborTentPrizeHeld,
    [FALLARBOR_TENT_FUNC_SET_PRIZE_HELD]    = SetFallarborTentPrizeHeld,
    [FALLARBOR_TENT_FUNC_SAVE]              = SaveFallarborTentChallenge,
    [FALLARBOR_TENT_FUNC_GIVE_PRIZE]        = GiveFallarborTentPrize,
    [FALLARBOR_TENT_FUNC_GET_OPPONENT_NAME] = BufferFallarborTentTrainerName
};

static const u16 sFallarborTentReward = ITEM_PP_MAX;
static const u8 sFallarborTentRewardAmount = 4;

void static (*const sSlateportTentFuncs[])(void) =
{
    [SLATEPORT_TENT_FUNC_INIT]                   = InitSlateportTentChallenge,
    [SLATEPORT_TENT_FUNC_GET_PRIZE_HELD]         = GetSlateportTentPrizeHeld,
    [SLATEPORT_TENT_FUNC_SET_PRIZE_HELD]         = SetSlateportTentPrizeHeld,
    [SLATEPORT_TENT_FUNC_SAVE]                   = SaveSlateportTentChallenge,
    [SLATEPORT_TENT_FUNC_GIVE_PRIZE]             = GiveSlateportTentPrize,
    [SLATEPORT_TENT_FUNC_SELECT_RENT_MONS]       = SelectInitialRentalMons,
    [SLATEPORT_TENT_FUNC_SWAP_RENT_MONS]         = SwapRentalMons,
    [SLATEPORT_TENT_FUNC_GENERATE_OPPONENT_MONS] = GenerateOpponentMons,
    [SLATEPORT_TENT_FUNC_GENERATE_RENTAL_MONS]   = GenerateInitialRentalMons
};

static const u16 sSlateportTentRewards[] = {ITEM_HP_UP, ITEM_PROTEIN, ITEM_IRON, ITEM_CALCIUM, ITEM_ZINC, ITEM_CARBOS};
static const u8 sSlateportTentRewardsAmount = 2;

// code
void CallVerdanturfTentFunction(void)
{
    sVerdanturfTentFuncs[gSpecialVar_0x8004]();
}

static void InitVerdanturfTentChallenge(void)
{
    gSaveBlock2Ptr->frontier.challengeStatus = 0;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 0;
    gSaveBlock2Ptr->frontier.challengePaused = FALSE;
    SetDynamicWarp(0, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, WARP_ID_NONE);
}

static void GetVerdanturfTentPrizeHeld(void)
{
    gSpecialVar_Result = gSaveBlock2Ptr->frontier.verdanturfTentPrizeHeld;
}

static void SetVerdanturfTentPrizeHeld(void)
{
    gSaveBlock2Ptr->frontier.verdanturfTentPrizeHeld = gSpecialVar_0x8005;
}

static void SetVerdanturfTentTrainerGfx(void)
{
    gTrainerBattleOpponent_A = (u32)((Random() % 255) * 5) / 64;
    SetBattleFacilityTrainerGfxId(gTrainerBattleOpponent_A, 0);
}

static void BufferVerdanturfTentTrainerIntro(void)
{
    if (gTrainerBattleOpponent_A < FRONTIER_TRAINERS_COUNT)
        FrontierSpeechToString(gFacilityTrainers[gTrainerBattleOpponent_A].speechBefore);
}

static void SaveVerdanturfTentChallenge(void)
{
    gSaveBlock2Ptr->frontier.challengeStatus = gSpecialVar_0x8005;
    VarSet(VAR_TEMP_CHALLENGE_STATUS, 0);
    gSaveBlock2Ptr->frontier.challengePaused = TRUE;
    SaveGameFrontier();
}

static void GiveVerdanturfTentPrize(void)
{
    u8 amountToGive = sVerdanturfTentRewardAmount;

    if (gSpecialVar_0x8005 == TRUE)
    {
        amountToGive = min(GetFreeSpaceForItemInBag(sVerdanturfTentReward), amountToGive);
        // The item's capacity is full
        if (amountToGive == 0)
        {
            // Set VAR_RESULT to FALSE to not display any message and return
            gSpecialVar_Result = FALSE;
            return;
        }
    }

    if (AddBagItem(sVerdanturfTentReward, amountToGive) == TRUE)
    {
        CopyItemNameHandlePlural(sVerdanturfTentReward, gStringVar1, sVerdanturfTentRewardAmount);
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

void CallFallarborTentFunction(void)
{
    sFallarborTentFuncs[gSpecialVar_0x8004]();
}

static void InitFallarborTentChallenge(void)
{
    gSaveBlock2Ptr->frontier.challengeStatus = 0;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 0;
    gSaveBlock2Ptr->frontier.challengePaused = FALSE;
    SetDynamicWarp(0, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, WARP_ID_NONE);
}

static void GetFallarborTentPrizeHeld(void)
{
    gSpecialVar_Result = gSaveBlock2Ptr->frontier.fallarborTentPrizeHeld;
}

static void SetFallarborTentPrizeHeld(void)
{
    gSaveBlock2Ptr->frontier.fallarborTentPrizeHeld = gSpecialVar_0x8005;
}

static void SaveFallarborTentChallenge(void)
{
    gSaveBlock2Ptr->frontier.challengeStatus = gSpecialVar_0x8005;
    VarSet(VAR_TEMP_CHALLENGE_STATUS, 0);
    gSaveBlock2Ptr->frontier.challengePaused = TRUE;
    SaveGameFrontier();
}

static void GiveFallarborTentPrize(void)
{
    u8 amountToGive = sFallarborTentRewardAmount;

    if (gSpecialVar_0x8005 == TRUE)
    {
        amountToGive = min(GetFreeSpaceForItemInBag(sFallarborTentReward), amountToGive);
        // The item's capacity is full
        if (amountToGive == 0)
        {
            // Set VAR_RESULT to FALSE to not display any message and return
            gSpecialVar_Result = FALSE;
            return;
        }
    }

    if (AddBagItem(sFallarborTentReward, amountToGive) == TRUE)
    {
        CopyItemNameHandlePlural(sFallarborTentReward, gStringVar1, sFallarborTentRewardAmount);
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

static void BufferFallarborTentTrainerName(void)
{
    GetFrontierTrainerName(gStringVar1, gTrainerBattleOpponent_A);
}

void CallSlateportTentFunction(void)
{
    sSlateportTentFuncs[gSpecialVar_0x8004]();
}

static void InitSlateportTentChallenge(void)
{
    gSaveBlock2Ptr->frontier.challengeStatus = 0;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 0;
    gSaveBlock2Ptr->frontier.challengePaused = FALSE;
    SetDynamicWarp(0, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, WARP_ID_NONE);
}

static void GetSlateportTentPrizeHeld(void)
{
    gSpecialVar_Result = gSaveBlock2Ptr->frontier.slateportTentPrizeHeld;
}

static void SetSlateportTentPrizeHeld(void)
{
    gSaveBlock2Ptr->frontier.slateportTentPrizeHeld = gSpecialVar_0x8005;
}

static void SaveSlateportTentChallenge(void)
{
    gSaveBlock2Ptr->frontier.challengeStatus = gSpecialVar_0x8005;
    VarSet(VAR_TEMP_CHALLENGE_STATUS, 0);
    gSaveBlock2Ptr->frontier.challengePaused = TRUE;
    SaveGameFrontier();
}

static void GiveSlateportTentPrize(void)
{
    u8 timesBagFull = 0;
    u8 amountToGive = sSlateportTentRewardsAmount;
    gSpecialVar_Result = FALSE;

    for (u8 i = 0; i < ARRAY_COUNT(sSlateportTentRewards); i++)
    {
        if (gSpecialVar_0x8005 == TRUE)
        {
            amountToGive = min(GetFreeSpaceForItemInBag(sSlateportTentRewards[i]), sSlateportTentRewardsAmount);
        }

        // amountToGive will only ever be different than sSlateportTentRewardsAmount if gSpecialVar_0x8005 == TRUE
        //  if it's 0, there's no room in the bag for the item, so no need to try to add it
        if (amountToGive == 0 || !AddBagItem(sSlateportTentRewards[i], amountToGive)){
            timesBagFull++;
        }
    }
        
    if (timesBagFull < ARRAY_COUNT(sSlateportTentRewards))
    {
        StringCopy(gStringVar1, COMPOUND_STRING("2x SETS OF VITAMINS"));
        gSpecialVar_Result = TRUE;
    }
}

static void SelectInitialRentalMons(void)
{
    ZeroPlayerPartyMons();
    DoBattleFactorySelectScreen();
}

static void SwapRentalMons(void)
{
    DoBattleFactorySwapScreen();
}

bool8 InSlateportBattleTent(void)
{
    return gMapHeader.regionMapSectionId == MAPSEC_SLATEPORT_CITY
           && (gMapHeader.mapLayoutId == LAYOUT_BATTLE_TENT_CORRIDOR || gMapHeader.mapLayoutId == LAYOUT_BATTLE_TENT_BATTLE_ROOM);
}

static void GenerateInitialRentalMons(void)
{
    s32 i, j;
    u8 firstMonId;
    u16 monSetId;
    u16 currSpecies;
    u16 species[PARTY_SIZE];
    u16 monIds[PARTY_SIZE];
    u16 heldItems[PARTY_SIZE];

    firstMonId = 0;
    gFacilityTrainers = gSlateportBattleTentTrainers;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        species[i] = 0;
        monIds[i] = 0;
        heldItems[i] = 0;
    }
    gFacilityTrainerMons = gSlateportBattleTentMons;
    currSpecies = SPECIES_NONE;
    i = 0;
    while (i != PARTY_SIZE)
    {
        // Cannot have two Pokémon of the same species.
        monSetId = Random() % NUM_SLATEPORT_TENT_MONS;
        for (j = firstMonId; j < firstMonId + i; j++)
        {
            if (monIds[j] == monSetId)
                break;
            if (species[j] == gFacilityTrainerMons[monSetId].species)
            {
                if (currSpecies == SPECIES_NONE)
                    currSpecies = gFacilityTrainerMons[monSetId].species;
                else
                    break;
            }
        }
        if (j != i + firstMonId)
            continue;

        // Cannot have two same held items.
        for (j = firstMonId; j < i + firstMonId; j++)
        {
            if (heldItems[j] != 0 && heldItems[j] == gBattleFrontierHeldItems[gFacilityTrainerMons[monSetId].itemTableId])
            {
                if (gFacilityTrainerMons[monSetId].species == currSpecies)
                    currSpecies = SPECIES_NONE;
                break;
            }
        }
        if (j != i + firstMonId)
            continue;

        gSaveBlock2Ptr->frontier.rentalMons[i].monId = monSetId;
        species[i] = gFacilityTrainerMons[monSetId].species;
        heldItems[i] = gBattleFrontierHeldItems[gFacilityTrainerMons[monSetId].itemTableId];
        monIds[i] = monSetId;
        i++;
    }
}

static void GenerateOpponentMons(void)
{
    u16 trainerId;
    s32 i, j, k;
    const u16 *monSet;
    u16 species[FRONTIER_PARTY_SIZE];
    u16 heldItems[FRONTIER_PARTY_SIZE];
    s32 numMons = 0;

    gFacilityTrainers = gSlateportBattleTentTrainers;
    gFacilityTrainerMons = gSlateportBattleTentMons;

    while (1)
    {
        do
        {
            // Choose a random trainer, ensuring no repeats in this challenge
            trainerId = Random() % NUM_BATTLE_TENT_TRAINERS;
            for (i = 0; i < gSaveBlock2Ptr->frontier.curChallengeBattleNum; i++)
            {
                if (gSaveBlock2Ptr->frontier.trainerIds[i] == trainerId)
                    break;
            }
        } while (i != gSaveBlock2Ptr->frontier.curChallengeBattleNum);

        gTrainerBattleOpponent_A = trainerId;
        monSet = gFacilityTrainers[gTrainerBattleOpponent_A].monSet;
        while (monSet[numMons] != 0xFFFF)
            numMons++;
        if (numMons > 8)
            break;
        numMons = 0;
    }

    if (gSaveBlock2Ptr->frontier.curChallengeBattleNum < TENT_STAGES_PER_CHALLENGE - 1)
        gSaveBlock2Ptr->frontier.trainerIds[gSaveBlock2Ptr->frontier.curChallengeBattleNum] = gTrainerBattleOpponent_A;

    monSet = gFacilityTrainers[gTrainerBattleOpponent_A].monSet;
    i = 0;
    while (i != FRONTIER_PARTY_SIZE)
    {
        sRandMonId = monSet[Random() % numMons];

        // Ensure none of the opponent's Pokémon are the same as the potential rental Pokémon for the player
        for (j = 0; j < (int)ARRAY_COUNT(gSaveBlock2Ptr->frontier.rentalMons); j++)
        {
            if (gFacilityTrainerMons[sRandMonId].species == gFacilityTrainerMons[gSaveBlock2Ptr->frontier.rentalMons[j].monId].species)
                break;
        }
        if (j != (int)ARRAY_COUNT(gSaveBlock2Ptr->frontier.rentalMons))
            continue;

        // Ensure this species hasn't already been chosen for the opponent
        for (k = 0; k < i; k++)
        {
            if (species[k] == gFacilityTrainerMons[sRandMonId].species)
                break;
        }
        if (k != i)
            continue;

        // Ensure held items don't repeat on the opponent's team
        for (k = 0; k < i; k++)
        {
            if (heldItems[k] != ITEM_NONE && heldItems[k] == gBattleFrontierHeldItems[gFacilityTrainerMons[sRandMonId].itemTableId])
                break;
        }
        if (k != i)
            continue;

        // Successful selection
        species[i] = gFacilityTrainerMons[sRandMonId].species;
        heldItems[i] = gBattleFrontierHeldItems[gFacilityTrainerMons[sRandMonId].itemTableId];
        gFrontierTempParty[i] = sRandMonId;
        i++;
    }
}
