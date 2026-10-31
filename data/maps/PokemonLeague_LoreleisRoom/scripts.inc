@ Lorelei's room, ported from FireRed (porting/). The Kanto Elite Four share Ever Grande's progress
@ (VAR_ELITE_4_STATE, FLAG_DEFEATED_ELITE_4_SIDNEY): both reset on whiting out and in the Hall of Fame.
PokemonLeague_LoreleisRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_LoreleisRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_LoreleisRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_LoreleisRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_LoreleisRoom_OnFrame
	.byte 0

PokemonLeague_LoreleisRoom_OnResume:
	special DoPokemonLeagueLightingEffect
	end

PokemonLeague_LoreleisRoom_OnLoad:
	call_if_set FLAG_DEFEATED_ELITE_4_SIDNEY, KantoLeague_EventScript_SetDoorOpen
	call_if_eq VAR_ELITE_4_STATE, 1, KantoLeague_EventScript_CloseEntry
	end

PokemonLeague_LoreleisRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_LoreleisRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_LoreleisRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_LoreleisRoom_OnFrame:
	map_script_2 VAR_ELITE_4_STATE, 0, PokemonLeague_LoreleisRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_LoreleisRoom_EventScript_EnterRoom::
	lockall
	call KantoLeague_EventScript_EnterRoom
	setvar VAR_ELITE_4_STATE, 1
	releaseall
	end

PokemonLeague_LoreleisRoom_EventScript_Lorelei::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ELITE_4_SIDNEY, PokemonLeague_LoreleisRoom_EventScript_PostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoLeague_Text_LoreleiIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_LORELEI_EXHIBITION, KantoLeague_Text_LoreleiDefeat
	clearflag FLAG_TEMP_5
	setflag FLAG_DEFEATED_ELITE_4_SIDNEY
	call KantoLeague_EventScript_OpenDoor
PokemonLeague_LoreleisRoom_EventScript_PostBattleSpeech::
	msgbox KantoLeague_Text_LoreleiPostBattle, MSGBOX_DEFAULT
	release
	end
