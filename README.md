# Pokemon Re:Emerald

 - A QoL hack for Pokemon Emerald based on pokeemerald Expansion that tries to keep the essence of vanilla Emerald while at the same time fixing a lot of the outdated mechanics.
 - This hack will have all Pokemon, sprites, attacks, items, etc **as they appeared in vanilla Emerald** and only updates QoL stuff like breeding mechanics, EVs, etc.
 - This will be a 386-Romhack that will have repeatable methods to get every item in-game, will try to fix the Battle Frontier economy, and many other things.

## pokeemerald-expansion options:
 - Most of the out-of-battle mechanics have been updated to be the best QoL versions of themselves (Like the Gen VI Exp. Share), with some exceptions:
    - The overworld Poison effect has been left alone and can still faint your Pokemon outside of battle
 - Most of the in-battle mechanics have been left alone and work exactly like they did in vanilla Emerald, with some exceptions:
    - A Pokemon can now evolve after a battle even if it has fainted.
    - The player now loses money like in later games after losing a battle, rather than losing half their money.
    - Unevolved Pokemon that evolve through level now get bonus experience if they're past their level requirement.
    - All EVs now cap at 252, rather than 255, and the stats are recalculated after every battle, even at level 100.
    - All non-berry items are restored after battle.

## Pokemon location changes:
 - Table slots and slot % have been tweaked all around (The lowest change a Pokemon will have to appear is now 5%, although usually they're 10% or higher)
 - All RS exclusive Pokemon have been re-added to the wild in their respective locations and the encounters in those places have been tweaked accordingly
 - A focus has been put on Emerald being the "shared version" and now, all previous RS version exclusives will have the same chance to appear in their respective location (ex. Lotad and Seedot, Zangoose and Seviper, etc)
 - Non-Hoenn mons have been added to many post-game areas, although only a few have been added at the moment. Some examples include Diglett in Desert Underpass or Unown in Altering Cave
 - All the changes implemented can be found on the [wild encounters](https://github.com/Dreaker75/re-emerald/blob/main/src/data/wild_encounters.json) file

## Mechanics changes:
 - The fishing minigame has been removed for all Rods and using one will now always land a Pokemon
 - Move tutors are no longer a one-time use
 - TMs are now infinite use, and their locations have been changed to accomodate for this change:
    - The TMs in the pickup table have been replaced by other items: TM44 Rest -> Upgrade, TM01 FocusPunch -> Brightpowder, TM26 Earthquake -> Sacred Ash
    - All the Trainer Hill TMs have been removed and the rewards have been overhauled (more below)
    - All TMs in the Game Corner have been replaced by type-held items: Silk Scarf, Charcoal, Mystic Water and Miracle Seed
 - PokeRus no longer stops being contagius after a few days have passed
    
## Field move changes:
 - Rock Smash rocks will either trigger a wild Pokemon battle, or they won't (No more random chance to trigger an encounter. If a rock doesn't give you an encounter, it never will)
 - Rock Smash rocks that block the path to certain areas or items will never respawn
 - Cut trees will stay cut even after reloading the map
 - HMs and field moves can be used by any Pokemon that can learn them, regardless of whether they currently know it (Badges are still needed for HMs)
 - Field moves show less messages when used
 - Waterfall is now automatically used when you try to scale a Waterfall (No more having to stand in front of it and press A)
 - Flash has been revamped in a few ways
    - Using Flash in a cave will now light up the entire room
    - The speed of the Flash raidus expanding has been increased to triple its speed to accomodate for the change
    - Flash will now reduce the encounter chance of wild Pokemon on areas where it has been used by a whooping 1/5
    - A message is displayed the first time you enter a dark cave in each area asking if you want to use Flash

## Facilities changes:
 - Though the Contests themselves remain entirely untouched, a few of the mechanics surrounding them have been changed:
    - Pokemon can now continue eating PokeBlocks after reaching max Sheen, allowing to easily reach max stats on all five contest stats like ORAS
    - Beating a Contest Rank now unlocks the next tier for all Pokemon, much like ORAS
    - Winning a Contest will also award the Ribbons for the lower ranks, and an amount Luxury Balls depending on the Rank won (The Luxury Balls are awarded regardless of whether the Pokemon got a Ribbon for that Rank)
 - The Trainer Hill now gives more battle-related items and does not require beating the mode in under a certain time to get them. 

## Map changes:
 - Added a Rock Smash and a Strength Boulder to Route 115 that allows for easier backtracking to Meteor Falls
 - Moved the Sudowoodo in the Battle Frontier 1 tile up so it doesn't block the way to Artisan Cave. This is to deal with the fact that it can now respawn every day

## Other changes:
 - Defeating the Elite Four now resets a few daily events like gifts and the Lillycove lottery
 - All static Pokemon will now respawn the following day if they were defeated (This is also affected by defeating the Elite Four)
 - Sudowoodo will also respawn if it has been captured, allowing for multiples to be caught in a single file.

# Future changes
 - To see the future changes planned for this romhack, check out [ToDoList.md](https://github.com/Dreaker75/re-emerald/blob/main/FUTURE_PLANS.md)
============================================================================================

# pokeemerald-expansion

## What is pokeemerald-expansion?

pokeemerald-expansion is a decomp hack base project based off pret's [pokeemerald](https://github.com/pret/pokeemerald) decompilation project. It's recommended that any new projects that plan on using it, to clone this repository instead of pret's vanilla repository, as we regurlarly incorporate pret's documentation changes. This is ***NOT*** a standalone romhack, and as such, most features will be unavailable and/or unbalanced if played as is.

If you use pokeemerald-expansion in your hack, please add RHH (Rom Hacking Hideout) to your credits list. Optionally, you can list the version used, so it can help players know what features to expect.
You can phrase it as the following:
```
Based off RHH's pokeemerald-expansion v1.8.0 https://github.com/rh-hideout/pokeemerald-expansion/
```

## What features are included?
- ***IMPORTANT*❗❗ Read through these to learn what features you can toggle**:
    - [Battle configurations](/include/config/battle.h)
    - [Pokémon configurations](/include/config/pokemon.h)
    - [Item configurations](/include/config/item.h)
    - [Overworld configurations](/include/config/overworld.h)
    - [Debug configurations](/include/config/debug.h)
- ***Upgraded battle engine.***
    - Gen5+ damage calculation.
    - 2v2 Wild battles support.
    - 1v2/2v1 battles support.
    - Fairy Type (configurable).
    - Physical/Special/Status Category (configurable).
    - New moves and abilities up to Scarlet and Violet.
        - Custom Contest data up to SwSh, newer moves are WIP. ([source](https://pokemonurpg.com/info/contests/rse-move-list/))
    - Battle gimmick support:
        - Mega Evolution
        - Primal Reversion
        - Ultra Burst
        - Z-Moves
            - Gen 8+ damaging moves are given power extrapolated from Gen 7.
            - Gen 8+ status moves have no additional effects, like Healing Wish.
        - Dynamax and Gigantamax
    - Initial battle parameters
        - Queueing stat boosts (aka, Totem Boosts)
        - Setting Terrains.
    - Mid-turn speed recalculation.
    - Quick Poké Ball selection in Wild Battles
        - Hold `R` to change selection with the D-Pad.
        - Press `R` to use last selected Poké Ball.
    - Run option shortcut
    - Faster battle intro - Message and animation/cry happens at the same time.
    - Faster HP drain.
    - Battle Debug menu.
        - Accessed by pressing `Select` on the "Fight/Bag/Pokémon/Run" menu.
    - Option to use AI flags in wild Pokémon battles.
    - FRLG/Gen4+ whiteout money calculation.
    - Configurable experience settings
        - Experience on catch.
        - Splitting experience.
        - Trainer experience.
        - Scaled experience.
        - Unevolved experience boost.
    - Frostbite.
        - Doesn't replace freezing unless a config is enabled, so you can mix and match.
    - Critical capture.
    - Removed badge boosts (configurable).
    - Recalculating stats at the end of every battle.
    - Level 100 Pokémon can earn EVs.
    - Inverse battle support.
    - TONS of other features listed [here](/include/config/battle.h).
- ***Full Trainer customization***
    - Nickname, EVs, IVs, moves, ability, ball, friendship, nature, gender, shininess.
    - Custom tag battle support (teaming up an NPC in a double battle).
    - Sliding trainer messages.
    - Upgraded Trainer AI
        - Considers newer move effects.
        - New flag options to let you customize the intelligence of your trainers.
        - Faster calculations.
    - Specify Poké Balls by Trainer class.
- ***Pokémon Species from Generations 1-9.***
    - Simplified process to add new Pokémon.
    - Option to disable unwanted families.
    - Updated sprites to DS style.
    - Updated stats, types, abilities and egg groups (configurable).
    - Updated Hoenn's Regional Dex to match ORAS' (configurable).
    - Updated National Dex incorporating the new species.
    - Sprite and animation visualizer.
        - Accesible by pressing `Select` on a Pokémon's Summary screen.
    - Gen4+ evolution methods, with some changes:
        - Mossy Rock, Icy Rock and Magnetic Field locations match ORAS'.
            - Leaf, Ice and Thunder Stones may also be used.
        - Inkay just needs level 30 to evolve.
            - You can't physically have both the RTC and gyroscope, so we skip this requirement.
        - Sylveon uses Gen8+'s evolution method (friendship + Fairy Move).
        - Option to use hold evolution items directly like stones.
    - Hidden Abilities.
        - Available via Ability Patch.
        - Compatible with Ghoul's DexNav branch.
    - All gender differences.
        - Custom female icons for female Hippopotas Hippowdon, Pikachu and Wobbufett
    - 3 Perfect IVs on Legendaries, Mythicals and Ultra Beasts.
- ***Customizable form change tables. Full list of methods [here](/include/constants/form_change_types.h).***
    - Item holding (eg. Giratina/Arceus)
    - Item using (eg. Oricorio)
        - Time of day option for Shaymin
    - Fainting
    - Battle begin and end (eg. Xerneas)
        - Move change option for Zacian/Zamazenta
    - Battle end in terrains (eg. Burmy)
    - Switched in battle (eg. Palafin)
    - HP Threshold (eg. Darmanitan)
    - Weather (eg. Castform)
    - End of turn (eg. Morpeko)
    - Time of day (eg. Shaymin)
    - Fusions (eg. Kyurem)
- ***Breeding Improvements***
    - Incense Baby Pokémon now happen automatically (configurable).
    - Level 1 eggs (configurable).
    - Poké Ball inheriting (configurable).
    - Egg Move Transfer, including Mirror Herb (configurable).
    - Nature inheriting 100% of the time with Everstone (configurable)
    - Gen6+ Ability inheriting (configurable).
- ***Items from newer Generations. Full list [here](/include/constants/items.h).***
    - ***Gen 6+ Exp. Share*** (configurable)
    - Berserk Gene
    - Most battle items from Gen 4+
    - Existing item data but missing effects:
        - Gimmighoul Coin
        - Booster Energy
        - Tera Shards
        - Tera Orb
- ***Feature branches incorporated (with permission):***
    - [RHH intro credits](https://github.com/Xhyzi/pokeemerald/tree/rhh-intro-credits) by @Xhyzi.
        - A small signature from all of us to show the collective effort in the project :)
    - [Overworld debug](https://github.com/TheXaman/pokeemerald/tree/tx_debug_system) by @TheXaman
        - May be disabled.
        - Accesible by pressing `R + Start` in the overworld by default.
        - **Additional features**:
            - *Clear Boxes*: cleans every Pokémon from the Boxes.
            - *Hatch an Egg*: lets you choose an Egg in your party and immediately hatch it.
    - [HGSS Pokédex](https://github.com/TheXaman/pokeemerald/tree/tx_pokedexPlus_hgss) by @TheXaman
        - May be disabled.
        - **Additional features**:
            - *Support for new evolution methods*.
            - *Dark Mode*.
    - [Nature Colors](https://github.com/DizzyEggg/pokeemerald/tree/nature_color) in summary screen by @DizzyEggg
    - [Dynamic Multichoice](https://github.com/SBird1337/pokeemerald/tree/feature/dynmulti) by @SBird1337
- ***Other features***
    - Pressing B while holding a Pokémon drops them like in modern games (configurable).
    - Running indoors (configurable).
    - Configurable overworld poison damage.
    - Configurable flags for disabling Wild encounters and Trainer battles.
    - Configurable flags for forcing or disabling Shinies.
    - Reusable TM (configurable).
    - B2W2+ Repel system that also supports LGPE's Lures
    - Gen6+'s EV cap.
    - All bugfixes from pret included.
    - Fixed overworld snow effect.

There are some mechanics, moves and abilities that are missing and being developed. Check [the project's milestones](https://github.com/rh-hideout/pokeemerald-expansion/milestones) to see which ones.


### [Documentation on features can be found here](https://github.com/rh-hideout/pokeemerald-expansion/wiki)

## If I already have a project based on regular pokeemerald, can I use pokeemerald-expansion?
Yes! Keep in mind that we keep up with pret's documentation of pokeemerald, which means that if your project a bit old, you might get merge conflicts that you need to solve manually.
- If you haven't set up a remote, run the command `git remote add RHH https://github.com/rh-hideout/pokeemerald-expansion`.
- Once you have your remote set up, run the command `git pull RHH master`.

With this, you'll get the latest version of pokeemerald-expansion, plus a couple of bugfixes that haven't been released into the next patch version :)

## **How do I update my version of pokeemerald-expansion?**
- If you haven't set up a remote, run the command `git remote add RHH https://github.com/rh-hideout/pokeemerald-expansion`.
- Once you have your remote set up, run the command `git pull RHH expansion/1.8.1`.

### Please consider crediting the entire [list of contributors](https://github.com/rh-hideout/pokeemerald-expansion/wiki/Credits) in your project, as they have all worked hard to develop this project :)

## There's a bug in the project. How do I let you guys know?
Please submit any issues with the project [here](https://github.com/rh-hideout/pokeemerald-expansion/issues). Make sure that the issue wasn't reported by someone else by searching using the filters.

## Can I contribute even if I'm not a member of ROM Hacking Hideout?

Yes! Contributions are welcome via Pull Requests and they will be reviewed by maintainers. Don't feel discouraged if we take a bit to review your PR, we'll get to it.

## Who maintains the project?

The project was originally started by DizzyEgg alongside other contributors.

The project has now gotten larger and DizzyEgg is now maintaining the project as part of the ROM Hacking Hideout community. Some members of this community are taking on larger roles to help maintain the project.

## What is the ROM Hacking Hideout?

A Discord-based ROM hacking community that has many members who hack using the disassembly and decompilation projects for Pokémon. Quite a few contributors to the original feature branches by DizzyEgg were members of ROM Hacking Hideout. You can call it RHH for short!

[Click here to join the RHH Discord Server!](https://discord.gg/6CzjAG6GZk)
