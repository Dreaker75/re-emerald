VictoryRoad_B1F_EventScript_Samuel::
	trainerbattle_single TRAINER_SAMUEL, VictoryRoad_B1F_Text_SamuelIntro, VictoryRoad_B1F_Text_SamuelDefeat
	msgbox VictoryRoad_B1F_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Shannon::
	trainerbattle_single TRAINER_SHANNON, VictoryRoad_B1F_Text_ShannonIntro, VictoryRoad_B1F_Text_ShannonDefeat
	msgbox VictoryRoad_B1F_Text_ShannonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Michelle::
	trainerbattle_single TRAINER_MICHELLE, VictoryRoad_B1F_Text_MichelleIntro, VictoryRoad_B1F_Text_MichelleDefeat
	msgbox VictoryRoad_B1F_Text_MichellePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Mitchell::
	trainerbattle_single TRAINER_MITCHELL, VictoryRoad_B1F_Text_MitchellIntro, VictoryRoad_B1F_Text_MitchellDefeat
	msgbox VictoryRoad_B1F_Text_MitchellPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Halle::
	trainerbattle_single TRAINER_HALLE, VictoryRoad_B1F_Text_HalleIntro, VictoryRoad_B1F_Text_HalleDefeat
	msgbox VictoryRoad_B1F_Text_HallePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_Text_SamuelIntro:
	.string "The thought that I'm getting closer to\n"
	.string "the POKéMON LEAGUE…\p"
	.string "I'm getting stage fright…$"

VictoryRoad_B1F_Text_SamuelDefeat:
	.string "I couldn't do a thing…$"

VictoryRoad_B1F_Text_SamuelPostBattle:
	.string "The POKéMON LEAGUE became distant\n"
	.string "again… What a letdown…$"

VictoryRoad_B1F_Text_ShannonIntro:
	.string "To win your way through the POKéMON\n"
	.string "LEAGUE, you need the trust of your\l"
	.string "POKéMON.$"

VictoryRoad_B1F_Text_ShannonDefeat:
	.string "Your relationship is based on\n"
	.string "solid trust.$"

VictoryRoad_B1F_Text_ShannonPostBattle:
	.string "By being together all the time, trust\n"
	.string "grows between POKéMON and TRAINERS.$"

VictoryRoad_B1F_Text_MichelleIntro:
	.string "This isn't the goal. It's only a place\n"
	.string "on the way to the POKéMON LEAGUE.$"

VictoryRoad_B1F_Text_MichelleDefeat:
	.string "That's the way!$"

VictoryRoad_B1F_Text_MichellePostBattle:
	.string "You'll do fine, for sure!\n"
	.string "Your POKéMON are all eager to go!$"

VictoryRoad_B1F_Text_MitchellIntro:
	.string "My POKéMON are cosmically\n"
	.string "awe inspiring!$"

VictoryRoad_B1F_Text_MitchellDefeat:
	.string "I've never met anyone like you before.$"

VictoryRoad_B1F_Text_MitchellPostBattle:
	.string "Even outside of battle, I sense\n"
	.string "incredible power emanating from you\l"
	.string "and your POKéMON.$"

VictoryRoad_B1F_Text_HalleIntro:
	.string "Okay, no need to get your back up!\n"
	.string "Relax, let's take it easy!$"

VictoryRoad_B1F_Text_HalleDefeat:
	.string "Whoa!\n"
	.string "Wonderful!$"

VictoryRoad_B1F_Text_HallePostBattle:
	.string "Sure, this is VICTORY ROAD.\p"
	.string "But it's not all that different from\n"
	.string "the path you've taken so far.\p"
	.string "Try to enjoy things the rest of\n"
	.string "the way!$"

VictoryRoad_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VictoryRoad_B1F_OnTransition
	.byte 0


VictoryRoad_B1F_OnTransition::
	call VictoryRoad_B1F_EventScript_SetBoulderPositions
	return


VictoryRoad_B1F_EventScript_SetBoulderPositions::
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_1, VictoryRoad_B1F_EventScript_SetBoulderPositions_2
VictoryRoad_B1F_EventScript_SetBoulderPositions_1:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_2, VictoryRoad_B1F_EventScript_SetBoulderPositions_5
VictoryRoad_B1F_EventScript_SetBoulderPositions_4:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_3, VictoryRoad_B1F_EventScript_SetBoulderPositions_8
VictoryRoad_B1F_EventScript_SetBoulderPositions_7:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_4, VictoryRoad_B1F_EventScript_SetBoulderPositions_11
VictoryRoad_B1F_EventScript_SetBoulderPositions_10:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_5, VictoryRoad_B1F_EventScript_SetBoulderPositions_14
VictoryRoad_B1F_EventScript_SetBoulderPositions_13:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_6, VictoryRoad_B1F_EventScript_SetBoulderPositions_17
VictoryRoad_B1F_EventScript_SetBoulderPositions_16:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_7, VictoryRoad_B1F_EventScript_SetBoulderPositions_20
VictoryRoad_B1F_EventScript_SetBoulderPositions_19:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_8, VictoryRoad_B1F_EventScript_SetBoulderPositions_22
	return

VictoryRoad_B1F_EventScript_SetBoulderPositions_2:
	setobjectxyperm 3, 3, 7
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_1

VictoryRoad_B1F_EventScript_SetBoulderPositions_5:
	setobjectxyperm 4, 10, 10
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_4

VictoryRoad_B1F_EventScript_SetBoulderPositions_8:
	setobjectxyperm 1, 20, 6
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_7

VictoryRoad_B1F_EventScript_SetBoulderPositions_11:
	setobjectxyperm 2, 21, 3
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_10

VictoryRoad_B1F_EventScript_SetBoulderPositions_14:
	setobjectxyperm 7, 34, 5
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_13

VictoryRoad_B1F_EventScript_SetBoulderPositions_17:
	setobjectxyperm 13, 33, 4
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_16

VictoryRoad_B1F_EventScript_SetBoulderPositions_20:
	setobjectxyperm 5, 20, 27
	goto VictoryRoad_B1F_EventScript_SetBoulderPositions_19

VictoryRoad_B1F_EventScript_SetBoulderPositions_22:
	setobjectxyperm 6, 22, 20
	return


VictoryRoad_B1F_EventScript_BoulderTrigger1::
	isobjectincoordinate 3, 3, 7
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger1_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger1_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_1
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger2::
	isobjectincoordinate 4, 10, 10
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger2_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger2_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_2
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger3::
	isobjectincoordinate 1, 20, 6
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger3_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger3_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_3
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger4::
	isobjectincoordinate 2, 21, 3
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger4_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger4_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_4
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger5::
	isobjectincoordinate 7, 34, 5
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger5_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger5_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_5
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger6::
	isobjectincoordinate 13, 33, 4
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger6_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger6_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_6
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger7::
	isobjectincoordinate 5, 20, 27
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger7_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger7_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_7
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_BoulderTrigger8::
	isobjectincoordinate 6, 22, 20
	compare VAR_RESULT, TRUE
	goto_if_eq VictoryRoad_B1F_EventScript_BoulderTrigger8_1
	return

VictoryRoad_B1F_EventScript_BoulderTrigger8_1:
	setflag FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_8
	call VictoryRoad_B1F_EventScript_UpdateBoulders
	return


VictoryRoad_B1F_EventScript_UpdateBoulders::
	setvar VAR_0x8000, 0
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_1, VictoryRoad_B1F_EventScript_UpdateBoulders_2
VictoryRoad_B1F_EventScript_UpdateBoulders_1:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_2, VictoryRoad_B1F_EventScript_UpdateBoulders_5
VictoryRoad_B1F_EventScript_UpdateBoulders_4:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_3, VictoryRoad_B1F_EventScript_UpdateBoulders_8
VictoryRoad_B1F_EventScript_UpdateBoulders_7:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_4, VictoryRoad_B1F_EventScript_UpdateBoulders_11
VictoryRoad_B1F_EventScript_UpdateBoulders_10:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_5, VictoryRoad_B1F_EventScript_UpdateBoulders_14
VictoryRoad_B1F_EventScript_UpdateBoulders_13:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_6, VictoryRoad_B1F_EventScript_UpdateBoulders_17
VictoryRoad_B1F_EventScript_UpdateBoulders_16:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_7, VictoryRoad_B1F_EventScript_UpdateBoulders_20
VictoryRoad_B1F_EventScript_UpdateBoulders_19:
	goto_if_set FLAG_MOVE_VICTORY_ROAD_B1F_BOULDER_8, VictoryRoad_B1F_EventScript_UpdateBoulders_23
VictoryRoad_B1F_EventScript_UpdateBoulders_22:
	compare VAR_0x8000, 8
	goto_if_eq VictoryRoad_B1F_EventScript_UpdateBoulders_25
	return

VictoryRoad_B1F_EventScript_UpdateBoulders_2:
	setobjectxyperm 3, 3, 7
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_1

VictoryRoad_B1F_EventScript_UpdateBoulders_5:
	setobjectxyperm 4, 10, 10
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_4

VictoryRoad_B1F_EventScript_UpdateBoulders_8:
	setobjectxyperm 1, 20, 6
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_7

VictoryRoad_B1F_EventScript_UpdateBoulders_11:
	setobjectxyperm 2, 21, 3
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_10

VictoryRoad_B1F_EventScript_UpdateBoulders_14:
	setobjectxyperm 7, 34, 5
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_13

VictoryRoad_B1F_EventScript_UpdateBoulders_17:
	setobjectxyperm 13, 33, 4
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_16

VictoryRoad_B1F_EventScript_UpdateBoulders_20:
	setobjectxyperm 5, 20, 27
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_19

VictoryRoad_B1F_EventScript_UpdateBoulders_23:
	setobjectxyperm 6, 22, 20
	addvar VAR_0x8000, 1
	goto VictoryRoad_B1F_EventScript_UpdateBoulders_22

VictoryRoad_B1F_EventScript_UpdateBoulders_25:
	addvar VAR_VICTORY_ROAD_B1F_BOULDERS_STATE, 1
	return

