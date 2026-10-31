@ Agatha's room, ported from FireRed (porting/). The Kanto Elite Four share Ever Grande's progress
@ (VAR_ELITE_4_STATE, FLAG_DEFEATED_ELITE_4_GLACIA): both reset on whiting out and in the Hall of Fame.
PokemonLeague_AgathasRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_AgathasRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_AgathasRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_AgathasRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_AgathasRoom_OnFrame
	.byte 0

PokemonLeague_AgathasRoom_OnResume:
	special DoPokemonLeagueLightingEffect
	end

PokemonLeague_AgathasRoom_OnLoad:
	call_if_set FLAG_DEFEATED_ELITE_4_GLACIA, KantoLeague_EventScript_SetDoorOpen
	call_if_eq VAR_ELITE_4_STATE, 3, KantoLeague_EventScript_CloseEntry
	end

PokemonLeague_AgathasRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_AgathasRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_AgathasRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_AgathasRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 2, PokemonLeague_AgathasRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_AgathasRoom_EventScript_EnterRoom::
	lockall
	call KantoLeague_EventScript_EnterRoom
	setvar VAR_ELITE_4_STATE, 3
	releaseall
	end

PokemonLeague_AgathasRoom_EventScript_Agatha::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_GLACIA, PokemonLeague_AgathasRoom_EventScript_PostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoLeague_Text_AgathaIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_AGATHA_EXHIBITION, KantoLeague_Text_AgathaDefeat
	clearflag FLAG_TEMP_5
	setflag FLAG_DEFEATED_ELITE_4_GLACIA
	call KantoLeague_EventScript_OpenDoor
PokemonLeague_AgathasRoom_EventScript_PostBattleSpeech::
	msgbox KantoLeague_Text_AgathaPostBattle, MSGBOX_DEFAULT
	release
	end
