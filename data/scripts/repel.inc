@ Started when the Repel steps run out (UpdateRepelCounter, src/wild_encounter.c). If the player still has the kind of
@ Repel they last used from the bag (VAR_REPEL_LAST_USED), they're asked whether to use another.
EventScript_RepelWoreOff::
	lockall
	checkitem VAR_REPEL_LAST_USED
	goto_if_eq VAR_RESULT, FALSE, EventScript_RepelWoreOffNoneLeft
	bufferitemname STR_VAR_1, VAR_REPEL_LAST_USED
	msgbox Text_RepelWoreOffUseAnother, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_RepelWoreOffEnd
	removeitem VAR_REPEL_LAST_USED
	copyvar VAR_0x8004, VAR_REPEL_LAST_USED
	special SetRepelStepsFromItem
	playse SE_REPEL
	waitse
	msgbox Text_UsedAnotherRepel, MSGBOX_DEFAULT
EventScript_RepelWoreOffEnd::
	releaseall
	end

EventScript_RepelWoreOffNoneLeft::
	msgbox Text_RepelWoreOff, MSGBOX_DEFAULT
	releaseall
	end

Text_RepelWoreOff:
	.string "REPEL's effect wore off…$"

Text_RepelWoreOffUseAnother:
	.string "REPEL's effect wore off…\p"
	.string "Use another {STR_VAR_1}?$"

Text_UsedAnotherRepel:
	.string "{PLAYER} used the\n"
	.string "{STR_VAR_1}.$"
