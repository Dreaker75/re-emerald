.set LOCALID_GEMMA, 3

SafariZone_Southwest_MapScripts::
	.byte 0

SafariZone_Southwest_EventScript_Woman::
	msgbox SafariZone_Southwest_Text_Woman, MSGBOX_NPC
	end

SafariZone_Southwest_EventScript_Gemma::
	specialvar VAR_RESULT, GetPlayerAvatarBike
	goto_if_eq VAR_RESULT, 0, SafariZone_Southwest_EventScript_GemmaNoBike
	msgbox SafariZone_Southwest_Text_GemmaOhThatBike, MSGBOX_NPC
	delay 30
	msgbox SafariZone_Southwest_Text_GemmaIKnowThisIsSudden, MSGBOX_NPC
	goto_if_eq VAR_FACING, DIR_EAST, SafariZone_Southwest_EventScript_GemmaLeaveDown
SafariZone_Southwest_EventScript_GemmaLeaveLeft:
	applymovement LOCALID_GEMMA, SafariZone_Southwest_Gemma_Movement_LeaveLeft
	goto SafariZone_Southwest_EventScript_GemmaHide
SafariZone_Southwest_EventScript_GemmaLeaveDown:
	applymovement LOCALID_GEMMA, SafariZone_Southwest_Gemma_Movement_LeaveDown
SafariZone_Southwest_EventScript_GemmaHide::
	waitmovement 0
	removeobject LOCALID_GEMMA
	setflag FLAG_HIDE_SAFARI_ZONE_GEMMA
	clearflag FLAG_HIDE_GRANITE_CAVE_1F_GEMMA
	addvar VAR_GEMMA_QUEST_STATE, 1
	end

SafariZone_Southwest_EventScript_GemmaNoBike::
	msgbox SafariZone_Southwest_Text_Gemma, MSGBOX_NPC
	end

SafariZone_Southwest_EventScript_RestHouseSign::
	msgbox SafariZone_Southwest_Text_RestHouseSign, MSGBOX_SIGN
	end

SafariZone_Southwest_Gemma_Movement_LeaveLeft:
	walk_fast_left
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

SafariZone_Southwest_Gemma_Movement_LeaveDown:
	walk_fast_down
	walk_fast_left
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

SafariZone_Southwest_Text_Gemma::
	.string "Oh, hi. Are you playing the\n"
	.string "SAFARI game?\p"
	.string "Huh? Me? No, I'm here for a\n"
	.string "different reason.$"

SafariZone_Southwest_Text_GemmaOhThatBike::
	.string "Oh, is that a RYDEL BIKE?\n"
	.string "Hmm, I wonder...$"

SafariZone_Southwest_Text_GemmaIKnowThisIsSudden::
	.string "Sorry, I know this is sudden,\n"
	.string "but I need your help.\p"
	.string "You see, I'm training for the next\n"
	.string "POKéMON TRIATHLON TOURNAMENT.\l"
	.string "However, I seem to be stuck in a rut.\p"
	.string "Could you please help me? I want to\n"
	.string "see how my bike fares in a race.\p"
	.string "I'll be in GRANITE CAVE, so please\n"
	.string "come see me when you can.\p"
	.string "Oh! And make sure you bring your\n"
	.string "fastest bike!$"
