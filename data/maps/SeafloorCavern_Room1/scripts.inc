SeafloorCavern_Room1_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_1, SeafloorCavern_Room1_Text_Grunt1Intro, SeafloorCavern_Room1_Text_Grunt1Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_2, SeafloorCavern_Room1_Text_Grunt2Intro, SeafloorCavern_Room1_Text_Grunt2Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_Text_Grunt1Intro:
	.string "We don't need a kid around!\n"
	.string "Go on home already!$"

SeafloorCavern_Room1_Text_Grunt1Defeat:
	.string "I want to go home…$"

SeafloorCavern_Room1_Text_Grunt1PostBattle:
	.string "I want to get a promotion so I can\n"
	.string "boss around the GRUNTS…$"

SeafloorCavern_Room1_Text_Grunt2Intro:
	.string "That submarine… It's tiny inside.\n"
	.string "I'm sore all over!$"

SeafloorCavern_Room1_Text_Grunt2Defeat:
	.string "Losing makes me sore!$"

SeafloorCavern_Room1_Text_Grunt2PostBattle:
	.string "That submarine we jacked, man,\n"
	.string "it's brutal as a ride.\l"
	.string "It's way too tight in there!$"

SeafloorCavern_Room1_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafloorCavern_Room1_OnTransition
	.byte 0


SeafloorCavern_Room1_OnTransition::
	call SeafloorCavern_Room1_EventScript_SetBoulderPositions
	return


SeafloorCavern_Room1_EventScript_SetBoulderPositions::
	goto_if_set FLAG_MOVE_SEAFLOORCAVERN_ROOM1_BOULDER_1, SeafloorCavern_Room1_EventScript_SetBoulderPositions_2
SeafloorCavern_Room1_EventScript_SetBoulderPositions_1:
	goto_if_set FLAG_MOVE_SEAFLOORCAVERN_ROOM1_BOULDER_2, SeafloorCavern_Room1_EventScript_SetBoulderPositions_4
	return

SeafloorCavern_Room1_EventScript_SetBoulderPositions_2:
	setobjectxyperm 1, 5, 10
	goto SeafloorCavern_Room1_EventScript_SetBoulderPositions_1

SeafloorCavern_Room1_EventScript_SetBoulderPositions_4:
	setobjectxyperm 2, 13, 11
	return


SeafloorCavern_Room1_EventScript_BoulderTrigger1::
	isobjectincoordinate 1, 5, 10
	compare VAR_RESULT, TRUE
	goto_if_eq SeafloorCavern_Room1_EventScript_BoulderTrigger1_1
	return

SeafloorCavern_Room1_EventScript_BoulderTrigger1_1:
	setflag FLAG_MOVE_SEAFLOORCAVERN_ROOM1_BOULDER_1
	call SeafloorCavern_Room1_EventScript_UpdateBoulders
	return


SeafloorCavern_Room1_EventScript_BoulderTrigger2::
	isobjectincoordinate 2, 13, 11
	compare VAR_RESULT, TRUE
	goto_if_eq SeafloorCavern_Room1_EventScript_BoulderTrigger2_1
	return

SeafloorCavern_Room1_EventScript_BoulderTrigger2_1:
	setflag FLAG_MOVE_SEAFLOORCAVERN_ROOM1_BOULDER_2
	call SeafloorCavern_Room1_EventScript_UpdateBoulders
	return


SeafloorCavern_Room1_EventScript_UpdateBoulders::
	setvar VAR_0x8000, 0
	goto_if_set FLAG_MOVE_SEAFLOORCAVERN_ROOM1_BOULDER_1, SeafloorCavern_Room1_EventScript_UpdateBoulders_2
SeafloorCavern_Room1_EventScript_UpdateBoulders_1:
	goto_if_set FLAG_MOVE_SEAFLOORCAVERN_ROOM1_BOULDER_2, SeafloorCavern_Room1_EventScript_UpdateBoulders_5
SeafloorCavern_Room1_EventScript_UpdateBoulders_4:
	compare VAR_0x8000, 2
	goto_if_eq SeafloorCavern_Room1_EventScript_UpdateBoulders_7
	return

SeafloorCavern_Room1_EventScript_UpdateBoulders_2:
	setobjectxyperm 1, 5, 10
	addvar VAR_0x8000, 1
	goto SeafloorCavern_Room1_EventScript_UpdateBoulders_1

SeafloorCavern_Room1_EventScript_UpdateBoulders_5:
	setobjectxyperm 2, 13, 11
	addvar VAR_0x8000, 1
	goto SeafloorCavern_Room1_EventScript_UpdateBoulders_4

SeafloorCavern_Room1_EventScript_UpdateBoulders_7:
	addvar VAR_SEAFLOORCAVERN_ROOM1_BOULDERS_STATE, 1
	return

