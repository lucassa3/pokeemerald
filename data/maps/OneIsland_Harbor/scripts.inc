OneIsland_Harbor_MapScripts::
	.byte 0

OneIsland_Harbor_EventScript_Sailor::
	lock
	faceplayer
	goto_if_eq VAR_ONE_ISLAND_FERRY_PORT, ONE_ISLAND_FERRY_FROM_LILYCOVE, OneIsland_Harbor_EventScript_ReturnToLilycove
	msgbox OneIsland_Harbor_Text_ReturnToSlateport, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_Harbor_EventScript_AsYouLike
	call OneIsland_Harbor_EventScript_Depart
	warp MAP_SLATEPORT_CITY_HARBOR, 8, 11
	waitstate
	release
	end

OneIsland_Harbor_EventScript_ReturnToLilycove::
	msgbox OneIsland_Harbor_Text_ReturnToLilycove, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_Harbor_EventScript_AsYouLike
	call OneIsland_Harbor_EventScript_Depart
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

OneIsland_Harbor_EventScript_Depart::
	msgbox EventTicket_Text_SailHome, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	delay 30
	hideobjectat LOCALID_ONE_ISLAND_HARBOR_SAILOR, MAP_ONE_ISLAND_HARBOR
	setvar VAR_0x8004, LOCALID_ONE_ISLAND_HARBOR_SS_TIDAL
	call Common_EventScript_FerryDepartIsland
	return

OneIsland_Harbor_EventScript_AsYouLike::
	msgbox EventTicket_Text_AsYouLike, MSGBOX_DEFAULT
	release
	end

OneIsland_Harbor_Text_ReturnToSlateport:
	.string "Welcome to ONE ISLAND!\n"
	.string "Do you want to return to SLATEPORT?$"

OneIsland_Harbor_Text_ReturnToLilycove:
	.string "Welcome to ONE ISLAND!\n"
	.string "Do you want to return to LILYCOVE?$"
