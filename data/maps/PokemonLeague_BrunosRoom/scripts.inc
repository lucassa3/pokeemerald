@ Bruno's room, ported from FireRed (porting/). The Kanto Elite Four share Ever Grande's progress
@ (VAR_ELITE_4_STATE, FLAG_DEFEATED_ELITE_4_PHOEBE): both reset on whiting out and in the Hall of Fame.
PokemonLeague_BrunosRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_BrunosRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_BrunosRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_BrunosRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_BrunosRoom_OnFrame
	.byte 0

PokemonLeague_BrunosRoom_OnResume:
	special DoPokemonLeagueLightingEffect
	end

PokemonLeague_BrunosRoom_OnLoad:
	call_if_set FLAG_DEFEATED_ELITE_4_PHOEBE, KantoLeague_EventScript_SetDoorOpen
	call_if_eq VAR_ELITE_4_STATE, 2, KantoLeague_EventScript_CloseEntry
	end

PokemonLeague_BrunosRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_BrunosRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_BrunosRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_BrunosRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 1, PokemonLeague_BrunosRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_BrunosRoom_EventScript_EnterRoom::
	lockall
	call KantoLeague_EventScript_EnterRoom
	setvar VAR_ELITE_4_STATE, 2
	releaseall
	end

PokemonLeague_BrunosRoom_EventScript_Bruno::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_PHOEBE, PokemonLeague_BrunosRoom_EventScript_PostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoLeague_Text_BrunoIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_BRUNO_EXHIBITION, KantoLeague_Text_BrunoDefeat
	clearflag FLAG_TEMP_5
	setflag FLAG_DEFEATED_ELITE_4_PHOEBE
	call KantoLeague_EventScript_OpenDoor
PokemonLeague_BrunosRoom_EventScript_PostBattleSpeech::
	msgbox KantoLeague_Text_BrunoPostBattle, MSGBOX_DEFAULT
	release
	end
