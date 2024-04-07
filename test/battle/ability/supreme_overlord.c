#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Supreme Overlord boosts Attack by an additive 10% per fainted mon on the side", s16 damage)
{
    bool32 switchMon = 0;
    PARAMETRIZE { switchMon = FALSE; }
    PARAMETRIZE { switchMon = TRUE; }
    GIVEN {
        PLAYER(SPECIES_KINGAMBIT) { Ability(ABILITY_SUPREME_OVERLORD); }
        PLAYER(SPECIES_PAWNIARD);
        PLAYER(SPECIES_PAWNIARD);
        PLAYER(SPECIES_PAWNIARD);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        if (switchMon)
            TURN { SWITCH(playerLeft, 3); }
        TURN { MOVE(playerRight, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerRight, 2); }
        if (switchMon)
            TURN { SWITCH(playerLeft, 0); }
        TURN { MOVE(playerLeft, MOVE_TACKLE, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.1), results[1].damage);
    }
}

DOUBLE_BATTLE_TEST("Supreme Overlord's boost caps at a 1.5x multipler", s16 damage)
{
    u32 faintCount = 0;
    PARAMETRIZE { faintCount = 5; }
    PARAMETRIZE { faintCount = 6; }
    GIVEN {
        PLAYER(SPECIES_PAWNIARD);
        PLAYER(SPECIES_PAWNIARD);
        PLAYER(SPECIES_PAWNIARD);
        PLAYER(SPECIES_KINGAMBIT) { Ability(ABILITY_SUPREME_OVERLORD); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerLeft, 2); }
        TURN { MOVE(playerLeft, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerLeft, 0); USE_ITEM(playerRight, ITEM_REVIVE, 0); }
        TURN { MOVE(playerLeft, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerLeft, 2); USE_ITEM(playerRight, ITEM_REVIVE, 2); }
        TURN { MOVE(playerLeft, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerLeft, 0); USE_ITEM(playerRight, ITEM_REVIVE, 0); }
        TURN { MOVE(playerLeft, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerLeft, 2); USE_ITEM(playerRight, ITEM_REVIVE, 2); }
        if (faintCount == 6)
            TURN { MOVE(playerLeft, MOVE_MEMENTO, target: opponentRight); SEND_OUT(playerLeft, 0); USE_ITEM(playerRight, ITEM_REVIVE, 0); }
        TURN { SWITCH(playerRight, 3); }
        TURN { MOVE(playerRight, MOVE_TACKLE, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, playerRight);
        HP_BAR(opponentLeft, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Supreme Overlord does not boost attack if party members are already fainted at the start of the battle", s16 damage)
{
    u32 fainted = 0;

    PARAMETRIZE { fainted = FALSE; }
    PARAMETRIZE { fainted = TRUE; }
    GIVEN {
        PLAYER(SPECIES_KINGAMBIT) { Ability(ABILITY_SUPREME_OVERLORD); }
        PLAYER(SPECIES_PAWNIARD) { HP(fainted ? 0 : 1); }
        PLAYER(SPECIES_PAWNIARD) { HP(fainted ? 0 : 1); }
        PLAYER(SPECIES_PAWNIARD) { HP(fainted ? 0 : 1); }
        PLAYER(SPECIES_PAWNIARD) { HP(fainted ? 0 : 1); }
        PLAYER(SPECIES_PAWNIARD) { HP(fainted ? 0 : 1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, target: opponent); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_SUPREME_OVERLORD);
            MESSAGE("Kingambit gained strength from the fallen!");
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}
