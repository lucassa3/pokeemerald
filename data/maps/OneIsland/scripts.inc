OneIsland_MapScripts::
	.byte 0

OneIsland_EventScript_OldMan::
	msgbox OneIsland_Text_FerryBringsVisitors, MSGBOX_NPC
	end

OneIsland_EventScript_Fisherman::
	msgbox OneIsland_Text_KindleRoad, MSGBOX_NPC
	end

OneIsland_EventScript_IslandSign::
	msgbox OneIsland_Text_IslandSign, MSGBOX_SIGN
	end

OneIsland_EventScript_PokemonNetCenterSign::
	msgbox OneIsland_Text_PokemonNetCenterSign, MSGBOX_SIGN
	end

OneIsland_Text_FerryBringsVisitors:
	.string "Since the S.S. TIDAL started stopping\n"
	.string "here, we've been getting visitors\l"
	.string "all the way from HOENN.\p"
	.string "It's nice to see new faces on\n"
	.string "our quiet little island.$"

OneIsland_Text_KindleRoad:
	.string "Past the beach to the east is\n"
	.string "KINDLE ROAD.\p"
	.string "Follow it far enough and you'll reach\n"
	.string "MT. EMBER, our island's volcano.$"

OneIsland_Text_IslandSign:
	.string "ONE ISLAND\n"
	.string "Friends Gather at Knot Island$"

OneIsland_Text_PokemonNetCenterSign:
	.string "Expanding the POKéMON World!\n"
	.string "POKéMON NET CENTER$"
