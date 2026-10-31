@ Route 23, ported from FireRed (porting/). Victory Road and the gate to Route 22 aren't ported: Scott stands in
@ front of Victory Road's exit, blocking it, where he drops the player off (data/scripts/kanto_league.inc), and the
@ other entrances, out of reach south of the cave, turn the player back.
Route23_MapScripts::
	.byte 0

@ FireRed's badge guards check Kanto badges; here they let the League's guest through
Route23_EventScript_Guard::
	msgbox Route23_Text_GuardGoRightAhead, MSGBOX_NPC
	end

Route23_EventScript_VictoryRoadClosed::
	lockall
	msgbox Route23_Text_VictoryRoadClosed, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Route23_Movement_WalkDown
	waitmovement 0
	releaseall
	end

Route23_EventScript_Route22GateClosed::
	lockall
	msgbox Route23_Text_Route22GateClosed, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Route23_Movement_WalkUp
	waitmovement 0
	releaseall
	end

Route23_EventScript_VictoryRoadGateSign::
	msgbox Route23_Text_VictoryRoadGateSign, MSGBOX_SIGN
	end

Route23_Movement_WalkDown:
	walk_down
	step_end

Route23_Movement_WalkUp:
	walk_up
	step_end

Route23_Text_GuardGoRightAhead:
	.string "Only truly skilled TRAINERS are\n"
	.string "allowed through.\p"
	.string "Oh! You're the CHAMPION from HOENN\n"
	.string "the LEAGUE invited!\p"
	.string "Okay, then.\n"
	.string "Please, go right ahead.$"

Route23_Text_VictoryRoadClosed:
	.string "Fallen rocks block the cave.\n"
	.string "There's no way through.$"

Route23_Text_Route22GateClosed:
	.string "The gate to ROUTE 22 is closed.$"

Route23_Text_VictoryRoadGateSign:
	.string "VICTORY ROAD GATE -\n"
	.string "POKéMON LEAGUE$"
