@ Indigo Plateau Pokémon Center, ported from FireRed (porting/). The Elite Four's door is at the top left.
IndigoPlateau_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IndigoPlateau_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

IndigoPlateau_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_INDIGO_PLATEAU
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_INDIGO_PLATEAU_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart IndigoPlateau_PokemonCenter_1F_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
IndigoPlateau_PokemonCenter_1F_Pokemart:
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	pokemartlistend

IndigoPlateau_PokemonCenter_1F_EventScript_GymGuy::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_DoorGuard::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_BlackBelt::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_AgathaWhuppedUs, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CooltrainerM::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_LancesCousinGymLeaderFarAway, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_PokemonJournal::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_PokemonJournalLance, MSGBOX_SIGN
	end

IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice:
	.string "Yo!\n"
	.string "Champ in the making!\p"
	.string "At the POKéMON LEAGUE, you will\n"
	.string "face the ELITE FOUR all in a row.\p"
	.string "If you lose, you have to start all\n"
	.string "over again!\p"
	.string "This is it!\n"
	.string "Go for it!$"

IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck:
	.string "From here on, you face the\n"
	.string "ELITE FOUR one by one.\p"
	.string "If you win, a door opens to the\n"
	.string "next TRAINER. Good luck!$"

IndigoPlateau_PokemonCenter_1F_Text_AgathaWhuppedUs:
	.string "AGATHA's GHOST-type POKéMON are\n"
	.string "horrifically terrifying in\l"
	.string "toughness.\p"
	.string "I took my FIGHTING-type POKéMON\n"
	.string "and raised them to the max.\p"
	.string "I went at AGATHA feeling pretty\n"
	.string "confident, but she whupped us.\p"
	.string "That old lady's also got a really\n"
	.string "short fuse, too.\p"
	.string "It doesn't take anything to get\n"
	.string "that scary lady hollering.$"

IndigoPlateau_PokemonCenter_1F_Text_LancesCousinGymLeaderFarAway:
	.string "Maybe becoming an ELITE FOUR\n"
	.string "member is in the blood.\p"
	.string "From what I've heard, LANCE has\n"
	.string "a cousin who's a GYM LEADER\l"
	.string "somewhere far away.$"

IndigoPlateau_PokemonCenter_1F_Text_PokemonJournalLance:
	.string "POKéMON JOURNAL\p"
	.string "Special Feature:\n"
	.string "ELITE FOUR's LANCE!\p"
	.string "LANCE's grandfather is thought to\n"
	.string "be the elder of a famous clan of\l"
	.string "dragon masters.$"
