@ Kanto's Champion's room, ported from FireRed (porting/): Blue, then Prof. Oak walks the player to the Hall of Fame.
@ The Kanto run ends in Ever Grande's Hall of Fame (FLAG_KANTO_HALL_OF_FAME), with Oak in the champion's place.
PokemonLeague_ChampionsRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonLeague_ChampionsRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_ChampionsRoom_OnWarp
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_ChampionsRoom_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_ChampionsRoom_OnFrame
	.byte 0

PokemonLeague_ChampionsRoom_OnTransition:
	setflag FLAG_HIDE_KANTO_CHAMPIONS_ROOM_OAK
	end

PokemonLeague_ChampionsRoom_OnResume:
	special DoPokemonLeagueLightingEffect
	end

PokemonLeague_ChampionsRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_ChampionsRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_ChampionsRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_ChampionsRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_ChampionsRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_ChampionsRoom_EventScript_EnterRoom::
	lockall
	setflag FLAG_TEMP_2
	applymovement LOCALID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerEnter
	waitmovement 0
	setvar VAR_TEMP_1, 1
	delay 20
	playbgm MUS_RG_ENCOUNTER_RIVAL, FALSE
	msgbox KantoLeague_Text_BlueIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	call KantoLeague_EventScript_BattleBlue
	clearflag FLAG_TEMP_5
	setflag FLAG_TEMP_4
	msgbox KantoLeague_Text_BluePostBattle, MSGBOX_DEFAULT
	call_if_unset FLAG_RECEIVED_KANTO_STARTER, KantoLeague_EventScript_BlueGiveStarter
	msgbox KantoLeague_Text_BlueRedHint, MSGBOX_DEFAULT
	playbgm MUS_RG_SLOW_PALLET, FALSE
	addobject LOCALID_KANTO_CHAMPIONS_ROOM_OAK
	msgbox KantoLeague_Text_OakPlayer, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerWatchOakEnter
	applymovement LOCALID_KANTO_CHAMPIONS_ROOM_BLUE, PokemonLeague_ChampionsRoom_Movement_BlueWatchOakEnter
	applymovement LOCALID_KANTO_CHAMPIONS_ROOM_OAK, PokemonLeague_ChampionsRoom_Movement_OakEnter
	waitmovement 0
	delay 25
	msgbox KantoLeague_Text_OakCongratulations, MSGBOX_DEFAULT
	applymovement LOCALID_KANTO_CHAMPIONS_ROOM_OAK, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_KANTO_CHAMPIONS_ROOM_BLUE, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox KantoLeague_Text_OakDisappointedInBlue, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_KANTO_CHAMPIONS_ROOM_OAK, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 20
	msgbox KantoLeague_Text_OakComeWithMe, MSGBOX_DEFAULT
	closemessage
	delay 13
	applymovement LOCALID_KANTO_CHAMPIONS_ROOM_OAK, PokemonLeague_ChampionsRoom_Movement_OakExit
	applymovement LOCALID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerExit
	waitmovement 0
	setflag FLAG_KANTO_HALL_OF_FAME
	setflag FLAG_HIDE_PETALBURG_GYM_GREETER
	warp MAP_EVER_GRANDE_CITY_HALL_OF_FAME, 7, 16
	waitstate
	releaseall
	end

PokemonLeague_ChampionsRoom_Movement_PlayerEnter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PokemonLeague_ChampionsRoom_Movement_PlayerExit:
	delay_16
	walk_left
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	delay_8
	set_invisible
	step_end

PokemonLeague_ChampionsRoom_Movement_PlayerWatchOakEnter:
	delay_8
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

PokemonLeague_ChampionsRoom_Movement_OakEnter:
	delay_16
	delay_16
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_down
	delay_16
	step_end

PokemonLeague_ChampionsRoom_Movement_OakExit:
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	delay_8
	set_invisible
	step_end

PokemonLeague_ChampionsRoom_Movement_BlueWatchOakEnter:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end
