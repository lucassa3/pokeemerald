OneIsland_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OneIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

OneIsland_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_ONE_ISLAND
	end

OneIsland_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_ONE_ISLAND_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

OneIsland_PokemonCenter_1F_EventScript_Hiker::
	msgbox OneIsland_PokemonCenter_1F_Text_MtEmberHot, MSGBOX_NPC
	end

OneIsland_PokemonCenter_1F_EventScript_LittleBoy::
	msgbox OneIsland_PokemonCenter_1F_Text_SevenIslands, MSGBOX_NPC
	end

OneIsland_PokemonCenter_1F_Text_MtEmberHot:
	.string "I came to climb MT. EMBER, but it's\n"
	.string "even hotter than MT. CHIMNEY!\p"
	.string "I'd better rest up here first.$"

OneIsland_PokemonCenter_1F_Text_SevenIslands:
	.string "ONE ISLAND is just one of the\n"
	.string "SEVII ISLANDS.\p"
	.string "There are seven of them, you know!\n"
	.string "Well, that's what everyone says.$"
