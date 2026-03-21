.set LOCALID_GEMMA, 3

GraniteCave_1F_MapScripts::
	.byte 0

GraniteCave_1F_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, GraniteCave_1F_EventScript_ReceivedFlash
	msgbox GraniteCave_1F_Text_GetsDarkAheadHereYouGo, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	setflag FLAG_RECEIVED_HM_FLASH
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_ReceivedFlash::
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_Gemma::
	msgbox GraniteCave_1F_Text_GemmaYouCame, MSGBOX_NPC
	delay 30
	msgbox GraniteCave_1F_Text_GemmaOnYourMarks, MSGBOX_NPC
	goto_if_eq VAR_FACING, DIR_EAST, GraniteCave_1F_EventScript_GemmaLeaveDown
GraniteCave_1F_EventScript_GemmaLeaveLeft:
	applymovement LOCALID_GEMMA, GraniteCave_1F_Movement_GemmaLeaveLeft
	goto GraniteCave_1F_EventScript_GemmaHide
GraniteCave_1F_EventScript_GemmaLeaveDown:
	applymovement LOCALID_GEMMA, GraniteCave_1F_Movement_GemmaLeaveDown
GraniteCave_1F_EventScript_GemmaHide::
	waitmovement 0
	removeobject LOCALID_GEMMA
	setflag FLAG_HIDE_GRANITE_CAVE_1F_GEMMA
	addvar VAR_GEMMA_QUEST_STATE, 1
	end

GraniteCave_1F_Movement_GemmaLeaveLeft:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

GraniteCave_1F_Movement_GemmaLeaveDown:
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

GraniteCave_1F_Text_GetsDarkAheadHereYouGo:
	.string "Hey, you.\n"
	.string "It gets awfully dark ahead.\l"
	.string "It'll be tough trying to explore.\p"
	.string "That guy who came by earlier…\n"
	.string "STEVEN, I think it was.\p"
	.string "He knew how to use FLASH, so he ought\n"
	.string "to be all right, but…\p"
	.string "Well, for us HIKERS, helping out those\n"
	.string "that we meet is our motto.\p"
	.string "Here you go, I'll pass this on to you.$"

GraniteCave_1F_Text_ExplainFlash:
	.string "FLASH lights up even the inky darkness\n"
	.string "of caves.\p"
	.string "But using it will scare the POKéMON\n"
	.string "away, so don't use it if you're trying\l"
	.string "to find wild POKéMON.\p"
	.string "To use FLASH, you need the GYM BADGE\n"
	.string "from DEWFORD's POKéMON GYM.$"

GraniteCave_1F_Text_GemmaYouCame:
	.string "Hey, you came! Thank you.\p"
	.string "Alright, now for the rules.\n"
	.string "The target is the rock on the\l"
	.string "deepest room of GRANITE CAVE.\p"
	.string "You need to climb the slope in\n"
	.string "B1F to reach it.$"

GraniteCave_1F_Text_GemmaOnYourMarks:
	.string "Alright, ready?\p"
	.string "On your marks...\n"
	.string "Get set...\l"
	.string "GO!$"
