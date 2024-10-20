# Battle changes
 - The loss of money when losing a battle is calculated like in FRLG
 - Gen VI+ Exp boost for Pokemon that are above the level they evolve at
 - Level 100 Pokemon can gain EVs as normal, like in Gen V+
 - Pokemon stats are recaculated (EVs are applied) after every battle, like in Gen V+
 - All non-berry items are restored after a battle ends, like in Gen IX+
 - When an ability activates, a pop-up appears like in Gen V+
 - Moves that hit multiple Pokemon in double battles show all the Pokemon that will be hit before confirming the attack
 - Pressing B in a wild battle will move the cursor to the Run option
 - The last used Ball can be used by pressing the R button during wild battles, or changed by holding the R button and using the left and right buttons
 - Pokemon will evolve even after losing a battle, if the conditions are met, like in Gen VI+

# Items changes
 - Items were added and moved around in the game to provide ways to get some previously unobtainable items, as well as repeatable methods for items that didn't previously have them (Check the [ITEM_CHANGES.md](https://github.com/Dreaker75/re-emerald/blob/main/info_hub/ITEM_CHANGES.md) for the full details)
 - Vitamins no longer have a limit of being usable until 100 EVs in the corresponding stat, like in Gen 8+
 - The player receives a Premier Ball for every 10 of any Poke Ball bought in one purchase, like in LGPE+
 - TMs have infinite uses like in Gen V-VIII and their locations have been changed to accomodate for this change:
    - The TMs in the pickup table have been replaced by other items: TM44 Rest -> Upgrade, TM01 FocusPunch -> Brightpowder, TM26 Earthquake -> Sacred Ash
    - All the Trainer Hill TMs have been removed and the rewards have been overhauled
    - All TMs in the Game Corner have been replaced by type-held items: Silk Scarf, Charcoal, Mystic Water and Miracle Seed
 - Modified the Item storage system to work like Gen 4:
    - The PC Item storage has been removed (The slots form here have been moved to the bag) 
    - The Bag capacity has been modified to allow for storing 1 of each item in all Pockets except the main items pocket
    - Each item can only have 1 slot in the bag. If the maximum amount (x999) is reached, no more of that item can be obtained, even if the bag has a free slot
    - The main item pocket can now store up to 93 different items (this is still not enough to fit all different items, as there are 139 in Emerald. To add the missing 46 slots I'll have to mess around with the save memory a lot more, which I'll leave for a later time)
 - Once a Repel runs out, a pop-up will appear asking which Repel you would like to use, if any

# Gameplay changes
 - Running is allowed indoors, as well as super tall grass and wooden bridges like the ones on Pacifidlog Town
 - You can now toggle the default moving speed (walking or running) by pressing the R button in the overworld after getting the running shoes. Holding B will let you run at the opposite speed as usual. The toggle has no visual cue at the momment, but does have a sound effect attached to it.
 - Synchronize now also forces the nature of Stationary and Gift Pokemon to be the same one as the Pokemon with Synchronize.
 - Lightning Rod now has the same effect as Static in the overworld
 - Flash Fire now has a 50% chance to force an encounter a Fire-type Pokemon, if one exists in the 
 - The fishing minigame has been removed for all Rods and using one will now always land a Pokemon
 - Move tutors are no longer a one-time use
 - PokeRus no longer stops being contagius after a few days have passed
 - The Roaming Legendary is now guaranteed to be the first encounter in the route if it's there

# Pokemon changes
 - Pokemon that evolve through trading now also evolve when leveling up while knowing a specific move:
   - Kadabra: Psychic
   - Machoke: DynamicPunch
   - Graveler: Earthquake
   - Haunter: Shadow Ball
 - Pokemon that evolve through trading while holding an item now also evolve by leveling up while holding said item
 - Friendship evolutions now require 160 Friendship to happen, rather than 220
 - Legendary Pokemon will have at least 3 perfect IVs
 - The maximum amount of EV a Pokemon can have in a single stat is now 252, instead of 255, like in Gen VI+

## Location changes:
 - Table slots and slot % have been tweaked all around (The lowest chance a Pokemon will have to appear is now 5%, although usually they're 10% or higher)
 - All RS exclusive Pokemon have been re-added to the wild in their respective locations and the encounters in those places have been tweaked accordingly
 - A focus has been put on Emerald being the "shared version" and now, all previous RS version exclusives will have the same chance to appear in their respective location (ex. Lotad and Seedot, Zangoose and Seviper, etc)
 - Non-Hoenn mons have been added to many post-game areas, although only a few have been added at the moment. Some examples include Diglett in Desert Underpass or Unown in Altering Cave
 - All the changes implemented can be found on the [wild_encounters.json](https://github.com/Dreaker75/re-emerald/blob/main/src/data/wild_encounters.json) file
 - The Feebas mechanic has been reworked. Feebas will still only appear in specific tiles of Route 119 50% of the time, but said tiles are no longer random. Instead, they're based on Feebas' Emerald Pokedex entry.

## Missing Pokemon:
 - Check [POKEMON_LIST.md](https://github.com/Dreaker75/re-emerald/blob/main/info_hub/POKEMON_LIST.md) to see a list of all the Pokemon changes in Re:Emerald as of version 0.1.0 and which ones are left to add. If a Pokemon is not mentioned, then it can be found in the same way as vanilla Emerald.

# Breeding changes
 - Breeding Nidoran♂ or Volbeat with a Ditto can now produce Nidoran♀ or Illumise eggs, respectively
 - Breeding parents can now pass down the PokeBall to the children, like in Gen VII+
 - When breeding, the male parent won't pass down TM moves to the child, like in Gen VI+ (TMs are reusable, anyways)
 - The female parent can also pass down Egg Moves
 - The Eversonte now passes down natures 100% of the time regardless of who holds it, like in Gen V+

# Field move changes:
 - Rock Smash rocks have been divided in two sub-types:
   - Rocks that block the path in some way will now never generate an encounter, and they will not respawn upon leaving the area.
   - Rocks that don't block access to any place now have a 100% chance to generate an encounter, and will respawn upon leaving the area
 - Cut trees will stay cut even after reloading the map
 - Strength boulders will now stay in place when moved onto the appropiate switches (If it reset after being placed in a switch, it's because the puzzle wouldn't be solvable and it needs to be placed in another switch)
 - HMs and field moves can be used by any Pokemon that can learn them, regardless of whether they currently know it (Badges are still needed for HMs). Smeargle is the exception, being the only Pokemon that still needs to know the move to be able to use it.
 - HMs can now be replaced just like any other moves
 - Field moves show less messages when used
 - Waterfall is now automatically used when you try to scale a Waterfall (No more having to stand in front of it and press A)
 - Flash has been revamped in a few ways
    - Using Flash in a cave will now light up the entire room
    - The speed of the Flash raidus expanding has been increased to triple its speed to accomodate for the change
    - Flash will now reduce the encounter chance of wild Pokemon on areas where it has been used to a whopping 1/5 of the original
    - A message is displayed the first time you enter a dark cave in each area asking if you want to use Flash
 - Teleport has been updated and now works like the old Fly HM, with the addition that it can be used indoors too! (Except in certain locations where it could break the game, like the Elite Four or the Battle Facilities). It still does NOT need a badge to be used, so you can now effectively Fly as soon as you get a Pokemon that can learn Teleport (Like Ralts or Abra)!
 - Fly now lets you land on certain routes (those that are far from any towns or cities, or that have useful locations)

# Facilities changes:
 - Though the Contests themselves remain entirely untouched, a few of the mechanics surrounding them have been changed:
    - Pokemon can now continue eating PokeBlocks after reaching max Sheen, allowing to easily reach max stats on all five contest stats like ORAS
    - Beating a Contest Rank now unlocks the next tier for all Pokemon, much like ORAS
    - Winning a Contest will also award the Ribbons for the lower ranks, and an amount of Luxury Balls depending on the Rank won (The Luxury Balls are awarded regardless of whether the Pokemon got a Ribbon for that Rank)
 - The Trainer Hill now gives more battle-related items and does not require beating the mode in under a certain time to get them. Full list on [ITEM_CHANGES.md](https://github.com/Dreaker75//info_hub/ITEM_CHANGES.md#facility-reward-items)
 - The prizes for all Battle Tents have been updated, and you can now choose whether you want to claim them or discard them if your bag is full. If you choose to discard them, you will automatically claim as many of the items as you have room for in your bag (Note: These rewards are opened for rebalancing later on if they seem too good/bad). Full list on [ITEM_CHANGES.md](https://github.com/Dreaker75/re-emerald/blob/main/info_hub/ITEM_CHANGES.md#facility-reward-items).
    
## Battle Frontier
 - Increased the amount of BP you get from every Facility. The points awarded now increase infinitely while the streak goes on (Up to the maximum amount of BPs)
 - All costs for items in the Battle Frontier shop and all Tutor Moves from the Battle Frontier have been singificantly reduced
 - New items have been added to the Battle Frontier shop (check [ITEM_CHANGES.md](https://github.com/Dreaker75/re-emerald/blob/main/info_hub/ITEM_CHANGES.md#battle-frontier) for the full list)
 - The amount of BP Scott gives you upon first arriving in the Battle Frontier has been changed. It now gives 5, 10, 15 or 20 BP depending on how many times you've talked to him before arriving.
    - If you talk to Scott all 13 times before talking to him in the Battle Frontier, he will give you a Choice Band as an extra gift

# Map changes:
 - Added a Rock Smash and a Strength Boulder to Route 115 that allows for easier backtracking to Meteor Falls
 - Moved the Sudowoodo in the Battle Frontier 1 tile up so it doesn't block the way to Artisan Cave. This is to deal with the fact that it can now respawn every day
 - Modified the ledges section to Lavaridge City to make it possible to reach it from Route 112 after getting access to Strength
 - Added a few respawnable breakable rocks to Victory Road for anyone wanting to grind off of them
 - Made some slight tweaks to some of the Seafloor Cavern rooms

# Other changes
 - Added an extra text speed option that display all the text instantly, which is turned on by default
 - Defeating the Elite Four now resets a few daily events like gifts and the Lillycove lottery
 - All Legendary Pokemon (Including the roamer) will now respawn the following day if they are defeated (This is also affected by defeating the Elite Four)
 - Sudowoodo will respawn every day whether it was defeated or caught
 - Releasing a Legendary Pokemon will allow them to be caught again starting the following day
 - The prizes for the Pokemon Lottery Corner have been updated and also give a reward for matching 1 number. Full list on [ITEM_CHANGES.md](https://github.com/Dreaker75/re-emerald/blob/main/info_hub/ITEM_CHANGES.md#facility-reward-items)
 - Return, Frustration and Hidden Power now show their real power in the Pokemon Summary
 - Hidden Power now shows its real type and category in the Pokemon Summary and the in-battle menu
 - The Pokemon Center Nurses now have less dialogue and animations for healing your Pokemon. Getting all 4 Trainer Stars also reduces the dialogue further like in vanilla Emerald
 - The player automatically faces down after healing at a Pokemon Center.
 - The NPC at the rest house on Route 111 no longer asks you if you want to keep resting until you say no.
 - Move Pokemon and Move Item options in the PC now appear before Withdraw and Deposit Pokemon like in newer games
 - Pressing B while moving a Pokemon on the PC deposits the Pokemon
 - Added the Japan exclusive wallpapers for the PC Boxes
 - Removed the second save confirmation popup, the game will now save after the first one. The warning for saving when a different file exists was left untouched.
 - Removed Wally's tutorial battle and the PokeNav's tutorial call to Mr. Stone
 - Petalburg Gym's doors can now be opened just by walking like the rest of the doors
 - Fixed the bug that caused the Mart Employee in Oldale Town to walk through the Shop's wall and into a tree under certain circumstances
 - Scientist in Devon Corp that revives the Fossils can now revive Kanto Fossils, and no longer requires talking to someone else or leaving the room to claim the Pokemon.
 - Using a PP Up or PP Max no longer returns you to the bag after using them or when trying to use them in a move with maxed out PP.
 - ADDED VERSION NUMBER TO THE MAIN MENU TO EASILY KNOW THE CURRENT VERSION OF THE GAME
