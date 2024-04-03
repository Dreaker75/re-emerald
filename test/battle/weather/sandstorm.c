#include "global.h"
#include "test/battle.h"

// Please add Sandstorm interactions with move, item and ability effects on their respective files.
SINGLE_BATTLE_TEST("Sandstorm deals 1/16 damage per turn")
{
    s16 sandstormDamage;

    GIVEN {
        PLAYER(SPECIES_SANDSLASH);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {MOVE(player, MOVE_SANDSTORM);}
    } SCENE {
        MESSAGE("Foe Wobbuffet is buffeted by the sandstorm!");
        HP_BAR(opponent, captureDamage: &sandstormDamage);
   } THEN { EXPECT_EQ(sandstormDamage, opponent->maxHP / 16); }
}

SINGLE_BATTLE_TEST("Sandstorm multiplies the special defense of Rock-types by 1.5x", s16 damage)
{
    u16 move;
    PARAMETRIZE{ move = MOVE_SANDSTORM; }
    PARAMETRIZE{ move = MOVE_CELEBRATE; }
    GIVEN {
        ASSUME(gMovesInfo[MOVE_SWIFT].category == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_WOBBUFFET) ;
        OPPONENT(SPECIES_NOSEPASS);
    } WHEN {
        TURN { MOVE(opponent, move); }
        TURN { MOVE(player, MOVE_SWIFT); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Sandstorm damage does not hurt Ground, Rock, and Steel-type Pokémon")
{
    u32 mon;
    PARAMETRIZE { mon = SPECIES_SANDSLASH; }
    PARAMETRIZE { mon = SPECIES_NOSEPASS; }
    PARAMETRIZE { mon = SPECIES_REGISTEEL; }
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_SANDSLASH].types[0] == TYPE_GROUND);
        ASSUME(gSpeciesInfo[SPECIES_NOSEPASS].types[0] == TYPE_ROCK);
        ASSUME(gSpeciesInfo[SPECIES_REGISTEEL].types[0] == TYPE_STEEL);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(mon);
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); }
    } SCENE {
        switch (mon)
        {
        case SPECIES_SANDSLASH:
            NOT MESSAGE("Foe Sandslash is buffeted by the sandstorm!");
            break;
        case SPECIES_NOSEPASS:
            NOT MESSAGE("Foe Nosepass is buffeted by the sandstorm!");
            break;
        case SPECIES_REGISTEEL:
            NOT MESSAGE("Foe Registeel is buffeted by the sandstorm!");
            break;
        }
    }
}
