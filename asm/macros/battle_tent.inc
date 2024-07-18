@ Verdanturf Tent

	@ Initialize the Verdanturf Battle Tent challenge
	.macro verdanturftent_init
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_INIT
	special CallVerdanturfTentFunction
	.endm

	@ Get the whether the prize is currently being held for the player
	.macro verdanturftent_getprize_held
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_GET_PRIZE_HELD
	special CallVerdanturfTentFunction
	.endm

	@ Set whether the prize is currently being held for the player
	.macro verdanturftent_setprizeheld isSaved:req
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_SET_PRIZE_HELD
	setvar VAR_0x8005, \isSaved
	special CallVerdanturfTentFunction
	.endm

	@ Unused. Set the opponent trainer and gfx. SetNextBattleTentOpponent is used through tower_setopponent instead
	.macro verdanturftent_setopponentgfx
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_SET_OPPONENT_GFX
	special CallVerdanturfTentFunction
	.endm

	@ Buffers the opponents intro speech to STR_VAR_4. Despite being a Verdanturf Tent function, it serves the same purpose for all 3 tents.
	.macro battletent_getopponentintro
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_GET_OPPONENT_INTRO
	special CallVerdanturfTentFunction
	.endm

	@ Save the game and set the challenge status
	.macro verdanturftent_save challengeStatus:req
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_SAVE
	setvar VAR_0x8005, \challengeStatus
	special CallVerdanturfTentFunction
	.endm

	@ Give the current prize item. FALSE if no room for prize
	@ giveUntilMax is a bool to know whether to give the player as many items as possible and ignoring the rest
	.macro verdanturftent_giveprize, giveUntilMax:req
	setvar VAR_0x8004, VERDANTURF_TENT_FUNC_GIVE_PRIZE
	setvar VAR_0x8005, \giveUntilMax
	special CallVerdanturfTentFunction
	.endm


@ Fallarbor Tent

	@ Initialize the Fallarbor Battle Tent challenge
	.macro fallarbortent_init
	setvar VAR_0x8004, FALLARBOR_TENT_FUNC_INIT
	special CallFallarborTentFunction
	.endm

	@ Get the whether the prize is currently being held for the player
	.macro fallarbortent_getprize_held
	setvar VAR_0x8004, FALLARBOR_TENT_FUNC_GET_PRIZE_HELD
	special CallFallarborTentFunction
	.endm

	@ Toggle whether the prize is currently being held for the player
	.macro fallarbortent_setprizeheld isSaved:req
	setvar VAR_0x8004, FALLARBOR_TENT_FUNC_SET_PRIZE_HELD
	setvar VAR_0x8005, \isSaved
	special CallFallarborTentFunction
	.endm

	@ Save the game and set the challenge status
	.macro fallarbortent_save challengeStatus:req
	setvar VAR_0x8004, FALLARBOR_TENT_FUNC_SAVE
	setvar VAR_0x8005, \challengeStatus
	special CallFallarborTentFunction
	.endm

	@ Give the current prize item. FALSE if no room for prize
	@ giveUntilMax is a bool to know whether to give the player as many items as possible and ignoring the rest
	.macro fallarbortent_giveprize, giveUntilMax:req
	setvar VAR_0x8004, FALLARBOR_TENT_FUNC_GIVE_PRIZE
	setvar VAR_0x8005, \giveUntilMax
	special CallFallarborTentFunction
	.endm

	@ Buffer the opponent trainers name to STR_VAR_1
	.macro fallarbortent_getopponentname
	setvar VAR_0x8004, FALLARBOR_TENT_FUNC_GET_OPPONENT_NAME
	special CallFallarborTentFunction
	.endm


@ Slateport Tent

	@ Initialize the Slateport Battle Tent challenge
	.macro slateporttent_init
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_INIT
	special CallSlateportTentFunction
	.endm

	@ Get the whether the prize is currently being held for the player
	.macro slateporttent_getprize_held
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_GET_PRIZE_HELD
	special CallSlateportTentFunction
	.endm

	@ Toggle whether the prize is currently being held for the player
	.macro slateporttent_setprizeheld isSaved:req
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_SET_PRIZE_HELD
	setvar VAR_0x8005, \isSaved
	special CallSlateportTentFunction
	.endm

	@ Save the game and set the challenge status
	.macro slateporttent_save challengeStatus:req
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_SAVE
	setvar VAR_0x8005, \challengeStatus
	special CallSlateportTentFunction
	.endm

	@ Give the current prize item. FALSE if no room for prize
	@ giveUntilMax is a bool to know whether to give the player as many items as possible and ignoring the rest
	.macro slateporttent_giveprize, giveUntilMax:req
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_GIVE_PRIZE
	setvar VAR_0x8005, \giveUntilMax
	special CallSlateportTentFunction
	.endm

	@ Slateport Tent's version of factory_rentmons
	.macro slateporttent_rentmons
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_SELECT_RENT_MONS
	special CallSlateportTentFunction
	.endm

	@ Slateport Tent's version of factory_swapmons
	.macro slateporttent_swapmons
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_SWAP_RENT_MONS
	special CallSlateportTentFunction
	.endm

	@ Slateport Tent's version of factory_generateopponentmons
	.macro slateporttent_generateopponentmons
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_GENERATE_OPPONENT_MONS
	special CallSlateportTentFunction
	.endm

	@ Slateport Tent's version of factory_generaterentalmons
	.macro slateporttent_generaterentalmons
	setvar VAR_0x8004, SLATEPORT_TENT_FUNC_GENERATE_RENTAL_MONS
	special CallSlateportTentFunction
	.endm

