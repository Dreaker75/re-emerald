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
 - The Roamer is now guaranteed to be the first encounter in the route if it's there
    
## Field move changes:
 - Rock Smash rocks will either trigger a wild Pokemon battle, or they won't (No more random chance to trigger an encounter. If a rock doesn't give you an encounter, it never will)
 - Rock Smash rocks that block the path to certain areas or items will never respawn
 - Cut trees will stay cut even after reloading the map
 - Strength boulders will now stay in place when moved onto the appropiate switches (If it reset after being placed in a switch, it's because the puzzle wouldn't be solvable and it needs to be placed in another switch)
 - HMs and field moves can be used by any Pokemon that can learn them, regardless of whether they currently know it (Badges are still needed for HMs)
 - HMs can now be replaced just like any other moves
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
 - Modified the ledges section to Lavaridge City to make it possible to reach it from Route 112 after getting access to Strength
 - Modified the ledges on Route 115 to allow an easy way back to Meteor Falls without the need for Surf after getting Strength
 - Added a few respawnable breakable rocks to Victory Road for anyone wanting to grind off of them
 - Made some slight tweaks to some of the Seafloor Cavern rooms

## Other changes:
 - All trade evolutions now have an extra evolution method
   - Pokemon that evolve by trade while holding an item now also evolve by levelling up while holding said item
   - Pokemon that evolve by trade without an item now evolve by levelling up when knowing a specific move:
      - Kadabra - Psychic
      - Machoke - Dynamic Punch
      - Graveler - Earthquake
      - Haunter - Shadow Ball
 - Defeating the Elite Four now resets a few daily events like gifts and the Lillycove lottery
 - All Legendary Pokemon (Including the roamer) will now respawn the following day if they are defeated (This is also affected by defeating the Elite Four)
 - Sudowoodo will respawn every day whether it was defeated or caught
 - Releasing a Legendary Pokemon will clear its CAUGHT flag, allowing them to be caught again starting the following day
 - Running is allowed everywhere, including places like bridges and tall grass
 - Move Pokemon and Item options in the PC now appear before Withdraw and Deposit Pokemon like in newer games
 - Removed the second save confirmation popup, the game will now save after the first one. The warning for saving when a different file exists was left untouched.
 - Removed Wally's tutorial battle and the PokeNav's tutorial call to Mr. Stone
 - Petalburg Gym's doors can now be opened just by walking like the rest of the doors

# Future changes
 - To see the future changes planned for this romhack, check out [ToDoList.md](https://github.com/Dreaker75/re-emerald/blob/main/FUTURE_PLANS.md)

# Credits
Based off RHH's pokeemerald-expansion v1.8.0 https://github.com/rh-hideout/pokeemerald-expansion/
