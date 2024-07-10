MagmaHideout_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_1F_OnTransition
	.byte 0

MagmaHideout_1F_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	call MagmaHideout_1F_EventScript_SetBoulderPositions
	end

MagmaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_1, MagmaHideout_1F_Text_Grunt1Intro, MagmaHideout_1F_Text_Grunt1Defeat
	msgbox MagmaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_2, MagmaHideout_1F_Text_Grunt2Intro, MagmaHideout_1F_Text_Grunt2Defeat
	msgbox MagmaHideout_1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_Text_Grunt1Intro:
	.string "When TEAM MAGMA has roll call, we get\n"
	.string "important guarding assignments in\l"
	.string "the order that we line up.\p"
	.string "That's why I'm stuck off in this corner.\n"
	.string "I'm always late to roll call!$"

MagmaHideout_1F_Text_Grunt1Defeat:
	.string "I'm always late for training sessions,\n"
	.string "too!\p"
	.string "I hate to say it, but I'm wimpy…$"

MagmaHideout_1F_Text_Grunt1PostBattle:
	.string "Okay, I'll try to put a little more\n"
	.string "effort into things from now on…$"

MagmaHideout_1F_Text_Grunt2Intro:
	.string "Our leader told us to dig into\n"
	.string "MT. CHIMNEY, so we dug and dug.\p"
	.string "And in the course of digging, we came\n"
	.string "across something that blew our minds!\p"
	.string "What did we find?\p"
	.string "Fuhahaha!\n"
	.string "I'll tell you if you beat me!$"

MagmaHideout_1F_Text_Grunt2Defeat:
	.string "Arrgh!\n"
	.string "Taken down!$"

MagmaHideout_1F_Text_Grunt2PostBattle:
	.string "I won't tell you after all.\n"
	.string "You'll find out when you get there!\p"
	.string "It'd be better if you saved surprises\n"
	.string "to the end, don't you think?$"

MagmaHideout_1F_EventScript_SetBoulderPositions::
	goto_if_set FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_1, MagmaHideout_1F_EventScript_SetBoulderPositions_2
MagmaHideout_1F_EventScript_SetBoulderPositions_1:
	goto_if_set FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_2, MagmaHideout_1F_EventScript_SetBoulderPositions_5
MagmaHideout_1F_EventScript_SetBoulderPositions_4:
	goto_if_set FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_3, MagmaHideout_1F_EventScript_SetBoulderPositions_7
	return

MagmaHideout_1F_EventScript_SetBoulderPositions_2:
	setobjectxyperm 4, 4, 22
	goto MagmaHideout_1F_EventScript_SetBoulderPositions_1

MagmaHideout_1F_EventScript_SetBoulderPositions_5:
	setobjectxyperm 5, 10, 22
	goto MagmaHideout_1F_EventScript_SetBoulderPositions_4

MagmaHideout_1F_EventScript_SetBoulderPositions_7:
	setobjectxyperm 6, 4, 23
	return


MagmaHideout_1F_EventScript_BoulderTrigger1::
	isobjectincoordinate 4, 4, 22
	compare VAR_RESULT, TRUE
	goto_if_eq MagmaHideout_1F_EventScript_BoulderTrigger1_1
	return

MagmaHideout_1F_EventScript_BoulderTrigger1_1:
	setflag FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_1
	call MagmaHideout_1F_EventScript_UpdateBoulders
	return


MagmaHideout_1F_EventScript_BoulderTrigger2::
	isobjectincoordinate 5, 10, 22
	compare VAR_RESULT, TRUE
	goto_if_eq MagmaHideout_1F_EventScript_BoulderTrigger2_1
	return

MagmaHideout_1F_EventScript_BoulderTrigger2_1:
	setflag FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_2
	call MagmaHideout_1F_EventScript_UpdateBoulders
	return


MagmaHideout_1F_EventScript_BoulderTrigger3::
	isobjectincoordinate 6, 4, 23
	compare VAR_RESULT, TRUE
	goto_if_eq MagmaHideout_1F_EventScript_BoulderTrigger3_1
	return

MagmaHideout_1F_EventScript_BoulderTrigger3_1:
	setflag FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_3
	call MagmaHideout_1F_EventScript_UpdateBoulders
	return


MagmaHideout_1F_EventScript_UpdateBoulders::
	setvar VAR_0x8000, 0
	goto_if_set FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_1, MagmaHideout_1F_EventScript_UpdateBoulders_2
MagmaHideout_1F_EventScript_UpdateBoulders_1:
	goto_if_set FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_2, MagmaHideout_1F_EventScript_UpdateBoulders_5
MagmaHideout_1F_EventScript_UpdateBoulders_4:
	goto_if_set FLAG_MOVE_MAGMA_HIDEOUT_BOULDER_3, MagmaHideout_1F_EventScript_UpdateBoulders_8
MagmaHideout_1F_EventScript_UpdateBoulders_7:
	compare VAR_0x8000, 3
	goto_if_eq MagmaHideout_1F_EventScript_UpdateBoulders_10
	return

MagmaHideout_1F_EventScript_UpdateBoulders_2:
	setobjectxyperm 4, 4, 22
	addvar VAR_0x8000, 1
	goto MagmaHideout_1F_EventScript_UpdateBoulders_1

MagmaHideout_1F_EventScript_UpdateBoulders_5:
	setobjectxyperm 5, 10, 22
	addvar VAR_0x8000, 1
	goto MagmaHideout_1F_EventScript_UpdateBoulders_4

MagmaHideout_1F_EventScript_UpdateBoulders_8:
	setobjectxyperm 6, 4, 23
	addvar VAR_0x8000, 1
	goto MagmaHideout_1F_EventScript_UpdateBoulders_7

MagmaHideout_1F_EventScript_UpdateBoulders_10:
	addvar VAR_MAGMA_HIDEOUT_BOULDERS_STATE, 1
	return

