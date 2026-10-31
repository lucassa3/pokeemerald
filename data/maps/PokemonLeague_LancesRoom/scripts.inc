@ Lance's room, ported from FireRed (porting/): a corridor leads from the entrance to Lance and the Champion's door.
@ Progress is shared with Ever Grande's League (VAR_ELITE_4_STATE, FLAG_DEFEATED_ELITE_4_DRAKE).
PokemonLeague_LancesRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_LancesRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_LancesRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_LancesRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_LancesRoom_OnFrame
	.byte 0

PokemonLeague_LancesRoom_OnResume:
	special DoPokemonLeagueLightingEffect
	end

PokemonLeague_LancesRoom_OnLoad:
	call_if_eq VAR_ELITE_4_STATE, 4, PokemonLeague_LancesRoom_EventScript_SetEntryClosed
	call_if_set FLAG_DEFEATED_ELITE_4_DRAKE, PokemonLeague_LancesRoom_EventScript_SetDoorOpen
	end

PokemonLeague_LancesRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_LancesRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_LancesRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_LancesRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 3, PokemonLeague_LancesRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_LancesRoom_EventScript_EnterRoom::
	lockall
	applymovement LOCALID_PLAYER, PokemonLeague_LancesRoom_Movement_WalkThroughCorridor
	waitmovement 0
	setflag FLAG_TEMP_2
	call PokemonLeague_LancesRoom_EventScript_SetEntryClosed
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	setvar VAR_ELITE_4_STATE, 4
	releaseall
	end

PokemonLeague_LancesRoom_EventScript_SetEntryClosed::
	setmetatile 5, 14, METATILE_FrlgPokemonLeague_Entry_TopLeft_Closed, TRUE
	setmetatile 6, 14, METATILE_FrlgPokemonLeague_Entry_TopMid_Closed, TRUE
	setmetatile 7, 14, METATILE_FrlgPokemonLeague_Entry_TopRight_Closed, TRUE
	setmetatile 5, 15, METATILE_FrlgPokemonLeague_Entry_BottomLeft_Closed, TRUE
	setmetatile 6, 15, METATILE_FrlgPokemonLeague_Entry_BottomMid_Closed, TRUE
	setmetatile 7, 15, METATILE_FrlgPokemonLeague_Entry_BottomRight_Closed, TRUE
	setmetatile 6, 16, METATILE_FrlgPokemonLeague_Floor_ShadeFull_Lance, FALSE
	setmetatile 7, 16, METATILE_FrlgPokemonLeague_Floor_ShadeFull_Lance, FALSE
	return

PokemonLeague_LancesRoom_EventScript_SetDoorOpen::
	setmetatile 6, 4, METATILE_FrlgPokemonLeague_Door_Top_Open, TRUE
	setmetatile 6, 5, METATILE_FrlgPokemonLeague_Door_Mid_Open, FALSE
	return

PokemonLeague_LancesRoom_EventScript_Lance::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_DRAKE, PokemonLeague_LancesRoom_EventScript_PostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoLeague_Text_LanceIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_LANCE_EXHIBITION, KantoLeague_Text_LanceDefeat
	clearflag FLAG_TEMP_5
	setvar VAR_0x8004, FANCOUNTER_DEFEATED_DRAKE
	special Script_TryGainNewFanFromCounter
	setflag FLAG_DEFEATED_ELITE_4_DRAKE
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_DOOR
	call PokemonLeague_LancesRoom_EventScript_SetDoorOpen
	special DrawWholeMapView
	setflag FLAG_TEMP_4
PokemonLeague_LancesRoom_EventScript_PostBattleSpeech::
	msgbox KantoLeague_Text_LancePostBattle, MSGBOX_DEFAULT
	release
	end

PokemonLeague_LancesRoom_Movement_WalkThroughCorridor:
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end
