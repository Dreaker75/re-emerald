#ifndef GUARD_CONFIG_POKEMON_H
#define GUARD_CONFIG_POKEMON_H

// Species data settings
#define P_UPDATED_TYPES             GEN_3  // Since Gen 6, several Pokémon were changed to be partially or fully Fairy type.
#define P_UPDATED_STATS             GEN_3  // Since Gen 6, Pokémon stats are updated with each passing generation.
#define P_UPDATED_ABILITIES         GEN_3  // Since Gen 6, certain Pokémon have their abilities changed.
#define P_UPDATED_EGG_GROUPS        GEN_3  // Since Gen 8, certain Pokémon have gained new egg groups.

// Breeding settings
#define P_NIDORAN_M_DITTO_BREED         GEN_LATEST  // Since Gen 5, when Nidoran♂ breeds with Ditto it can produce Nidoran♀ offspring. Before, it would only yield male offspring. This change also applies to Volbeat.
#define P_INCENSE_BREEDING              GEN_3  // Since Gen 9, cross-generation Baby Pokémon don't require Incense being held by the parents to be obtained via breeding.
#define P_EGG_HATCH_LEVEL               GEN_3  // Since Gen 4, Pokémon will hatch from eggs at level 1 instead of 5.
#define P_BALL_INHERITING               GEN_LATEST  // Since Gen 6, Eggs from the Daycare will inherit the Poké Ball from their mother. From Gen 7 onwards, the father can pass it down as well, as long as it's of the same species as the mother.
#define P_TM_INHERITANCE                GEN_LATEST  // Since Gen 6, the father no longer passes down TMs to the baby.
#define P_MOTHER_EGG_MOVE_INHERITANCE   GEN_LATEST  // Since Gen 6, the mother can also pass down Egg Moves.
// P_NATURE_INHERITANCE gives error when set to GEN_3 due to the check related to the IS_DITTO function, probably
#define P_NATURE_INHERITANCE            GEN_LATEST  // In Gen 3, Everstone grants Ditto and mothers a 50% chance to pass on Nature. Since Gen 4, anyone can pass on nature. Since Gen 5, the chance is 100%.
#define P_ABILITY_INHERITANCE           GEN_3  // In B2W2, a female Pokémon has an 80% chance of passing down their ability if bred with a male. Since Gen 6, the chance is 80% for normal ability and 60% for Hidden Ability, and anyone can pass down their abilities if bred with Ditto. NOTE: BW's effect: 60% chance to pass down HA and random for normal ability has been omitted.
#define P_EGG_MOVE_TRANSFER             GEN_3  // Starting in Gen 8, if two Pokémon of the same species are together in the Daycare, one knows an Egg Move, and the other has an empty slot, the other Pokémon will receive the Egg Move in the empty slot. In Gen 9, if a Pokémon holds a Mirror Herb, it will receive Egg Moves from the other regardless of species.

// Species-specific settings
#define P_SHEDINJA_BALL             GEN_3  // Since Gen 4, Shedinja requires a Poké Ball for its evolution. In Gen 3, Shedinja inherits Nincada's Ball.
#define P_KADABRA_EVERSTONE         GEN_3  // Since Gen 4, Kadabra can evolve even when holding an Everstone.
#define P_HIPPO_GENDER_DIFF_ICONS   TRUE        // If TRUE, will give Hippopotas and Hippowdon custom icons for their female forms.
#define P_SHUCKLE_BERRY_JUICE       FALSE        // In Gen 2, Shuckle had a 1/16 chance of converting Berry that it's holding into Berry Juice. Setting this to TRUE will allow to do this with an Oran Berry, which is the spiritual succesor of the Berry item.

// Other settings
#define P_LEGENDARY_PERFECT_IVS     GEN_3  // Since Gen 6, Legendaries, Mythicals and Ultra Beasts found in the wild or given through gifts have at least 3 perfect IVs.
#define P_EV_CAP                    GEN_LATEST  // Since Gen 6, the max EVs per stat is 252 instead of 255.

// Flag settings
// To use the following features in scripting, replace the 0s with the flag ID you're assigning it to.
// Eg: Replace with FLAG_UNUSED_0x264 so you can use that flag to toggle the feature.
#define P_FLAG_FORCE_SHINY      0     // If this flag is set, all wild and gift Pokémon will forced into being Shiny.
#define P_FLAG_FORCE_NO_SHINY   0     // If this flag is set, all wild and gift Pokémon will forced into NOT being Shiny.

// Modifying the latest generation WILL change the saveblock due to Dex flags and will require a new save file.
// Forms are kept based on the base species, Eg: Meowth and Persian will keep all of their forms, but Perrserker will not be available if P_GEN_8_POKEMON is set to FALSE.
// If you're disabling a generation previous to others (eg: Gen 5 but not Gen 6, 7 and 8),
// remember to update NATIONAL_DEX enum in include/constants/pokedex.h to avoid players from softlocking in the non-existant entries.
#define P_GEN_4_POKEMON             FALSE        // Generation 4 Pokémon (DPPt, HGSS)
#define P_GEN_5_POKEMON             FALSE        // Generation 5 Pokémon (BW, B2W2)
#define P_GEN_6_POKEMON             FALSE        // Generation 6 Pokémon (XY, ORAS)
#define P_GEN_7_POKEMON             FALSE        // Generation 7 Pokémon (SM, USUM, LGPE)
#define P_GEN_8_POKEMON             FALSE        // Generation 8 Pokémon (SwSh, BDSP, LA)

#endif // GUARD_CONFIG_POKEMON_H
