EventScript_UseFlash::
	animateflash 0
	setflashlevel 0
	end

EventScript_WantToUseFlash::
	checkpartylearnablemove MOVE_FLASH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CancelFlash
	setfieldeffectargument 0, VAR_RESULT
	setflag FLAG_ASKED_TO_USE_FLASH
	lockall
	msgbox Text_WantToUseFlash, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_CancelFlash
	closemessage
	dofieldeffect FLDEFF_USE_FLASH
	waitstate
	call EventScript_UseFlash
	releaseall
	end

EventScript_CancelFlash::
	closemessage
	releaseall
	end

Text_WantToUseFlash:
	.string "Would you like to use FLASH?$"
