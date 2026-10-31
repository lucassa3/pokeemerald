@ Kanto's Pokémon League. After Master Rank is cleared, Scott calls: Kanto's Elite Four want the player to challenge
@ their League. Scott waits by Lilycove's harbor and takes the player to Route 23, just outside Victory Road's exit
@ (ported from FireRed with the Indigo Plateau, see porting/). He waits there, in front of the cave, for the way back.
@ Once Blue has been beaten, Red waits at the summit of Mt. Pyre.

@ Started by ShouldDoScottKantoLeagueCall (src/field_specials.c)
KantoLeague_EventScript_ScottCall::
	lockall
	pokenavcall KantoLeague_Text_ScottCall
	waitmessage
	setflag FLAG_KANTO_LEAGUE_INVITED
	releaseall
	end

@ Lilycove City, by the harbor door
KantoLeague_EventScript_SetupLilycoveScott::
	setflag FLAG_HIDE_LILYCOVE_CITY_SCOTT
	goto_if_unset FLAG_KANTO_LEAGUE_INVITED, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_LILYCOVE_CITY_SCOTT
	return

LilycoveCity_EventScript_ScottKantoLeague::
	lock
	faceplayer
	msgbox KantoLeague_Text_ScottReadyToGo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoLeague_EventScript_ScottNotYet
	msgbox KantoLeague_Text_ScottLetsGo, MSGBOX_DEFAULT
	closemessage
	playse SE_SHIP
	warp MAP_ROUTE23, 18, 30
	waitstate
	release
	end

KantoLeague_EventScript_ScottNotYet::
	msgbox KantoLeague_Text_ScottNotYet, MSGBOX_DEFAULT
	release
	end

@ Route 23: Scott stands in front of Victory Road's exit (the cave isn't ported) and takes the player back
Route23_EventScript_Scott::
	lock
	faceplayer
	msgbox KantoLeague_Text_ScottGoBack, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoLeague_EventScript_ScottStay
	msgbox KantoLeague_Text_ScottBackToHoenn, MSGBOX_DEFAULT
	closemessage
	playse SE_SHIP
	warp MAP_LILYCOVE_CITY, 12, 33
	waitstate
	release
	end

KantoLeague_EventScript_ScottStay::
	msgbox KantoLeague_Text_ScottStay, MSGBOX_DEFAULT
	release
	end

@ Indigo Plateau's Elite Four rooms (data/maps/PokemonLeague_*), as in FireRed: walking in closes the entry behind
@ the player, beating the room's trainer opens the door to the next one
KantoLeague_EventScript_EnterRoom::
	applymovement LOCALID_PLAYER, KantoLeague_Movement_WalkUp5
	waitmovement 0
	setflag FLAG_TEMP_2
	playse SE_UNLOCK
	call KantoLeague_EventScript_CloseEntry
	special DrawWholeMapView
	return

KantoLeague_EventScript_OpenDoor::
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_DOOR
	call KantoLeague_EventScript_SetDoorOpen
	special DrawWholeMapView
	setflag FLAG_TEMP_4
	return

KantoLeague_EventScript_SetDoorOpen::
	setmetatile 6, 1, METATILE_FrlgPokemonLeague_Door_Top_Open, TRUE
	setmetatile 6, 2, METATILE_FrlgPokemonLeague_Door_Mid_Open, FALSE
	return

KantoLeague_EventScript_CloseEntry::
	setmetatile 5, 11, METATILE_FrlgPokemonLeague_Entry_TopLeft_Closed, TRUE
	setmetatile 6, 11, METATILE_FrlgPokemonLeague_Entry_TopMid_Closed, TRUE
	setmetatile 7, 11, METATILE_FrlgPokemonLeague_Entry_TopRight_Closed, TRUE
	setmetatile 5, 12, METATILE_FrlgPokemonLeague_Entry_BottomLeft_Closed, TRUE
	setmetatile 6, 12, METATILE_FrlgPokemonLeague_Entry_BottomMid_Closed, TRUE
	setmetatile 7, 12, METATILE_FrlgPokemonLeague_Entry_BottomRight_Closed, TRUE
	return

KantoLeague_Movement_WalkUp5:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

@ Blue's team counters the player's Hoenn starter, like the rival in FireRed
KantoLeague_EventScript_BattleBlue::
	switch VAR_STARTER_MON
	case 1, KantoLeague_EventScript_BattleBlueBlastoise
	case 2, KantoLeague_EventScript_BattleBlueVenusaur
	trainerbattle_no_intro TRAINER_BLUE_EXHIBITION_CHARIZARD, KantoLeague_Text_BlueDefeat
	return

KantoLeague_EventScript_BattleBlueBlastoise::
	trainerbattle_no_intro TRAINER_BLUE_EXHIBITION_BLASTOISE, KantoLeague_Text_BlueDefeat
	return

KantoLeague_EventScript_BattleBlueVenusaur::
	trainerbattle_no_intro TRAINER_BLUE_EXHIBITION_VENUSAUR, KantoLeague_Text_BlueDefeat
	return


@ First win against Blue: Professor Oak's Kanto starters, one per save. If the party and PC are both full the flag
@ stays unset and Blue offers again next time.
KantoLeague_EventScript_BlueGiveStarter::
	msgbox KantoLeague_Text_BlueOfferStarter, MSGBOX_DEFAULT
	message KantoLeague_Text_BlueWhichStarter
	waitmessage
	multichoice 18, 6, MULTI_KANTO_STARTER, TRUE
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_BULBASAUR
	call_if_eq VAR_RESULT, 1, KantoLeague_EventScript_ChooseCharmander
	call_if_eq VAR_RESULT, 2, KantoLeague_EventScript_ChooseSquirtle
	givemon VAR_TEMP_TRANSFERRED_SPECIES, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, KantoLeague_EventScript_StarterToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, KantoLeague_EventScript_StarterToPC
	msgbox gText_NoMoreRoomForPokemon, MSGBOX_DEFAULT
	return

KantoLeague_EventScript_ChooseCharmander::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHARMANDER
	return

KantoLeague_EventScript_ChooseSquirtle::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_SQUIRTLE
	return

KantoLeague_EventScript_StarterToParty::
	call KantoLeague_EventScript_ReceivedStarterFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoLeague_EventScript_ReceivedStarter
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto KantoLeague_EventScript_ReceivedStarter

KantoLeague_EventScript_StarterToPC::
	call KantoLeague_EventScript_ReceivedStarterFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Common_EventScript_NameReceivedBoxMon
	call Common_EventScript_TransferredToPC
	goto KantoLeague_EventScript_ReceivedStarter

KantoLeague_EventScript_ReceivedStarterFanfare::
	bufferspeciesname STR_VAR_2, VAR_TEMP_TRANSFERRED_SPECIES
	playfanfare MUS_OBTAIN_ITEM
	message KantoLeague_Text_ReceivedStarter
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	return

KantoLeague_EventScript_ReceivedStarter::
	setflag FLAG_RECEIVED_KANTO_STARTER
	msgbox KantoLeague_Text_BlueTakeCareOfIt, MSGBOX_DEFAULT
	return

@ Mt. Pyre summit: Red appears once Blue has been beaten in any of his three teams, and can be fought any time
KantoLeague_EventScript_SetupMtPyreRed::
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_RED
	goto_if_defeated TRAINER_BLUE_EXHIBITION_CHARIZARD, KantoLeague_EventScript_ShowMtPyreRed
	goto_if_defeated TRAINER_BLUE_EXHIBITION_BLASTOISE, KantoLeague_EventScript_ShowMtPyreRed
	goto_if_defeated TRAINER_BLUE_EXHIBITION_VENUSAUR, KantoLeague_EventScript_ShowMtPyreRed
	return

KantoLeague_EventScript_ShowMtPyreRed::
	clearflag FLAG_HIDE_MT_PYRE_SUMMIT_RED
	return

MtPyre_Summit_EventScript_Red::
	lock
	faceplayer
	goto_if_defeated TRAINER_RED_EXHIBITION, KantoLeague_EventScript_RedRematch
	msgbox KantoLeague_Text_RedFirstMeeting, MSGBOX_DEFAULT
	goto KantoLeague_EventScript_RedAskBattle
	end

KantoLeague_EventScript_RedRematch::
	msgbox KantoLeague_Text_RedRematch, MSGBOX_DEFAULT
KantoLeague_EventScript_RedAskBattle::
	msgbox KantoLeague_Text_RedAskBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoLeague_EventScript_RedDeclined
	msgbox KantoLeague_Text_RedIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_RED_EXHIBITION, KantoLeague_Text_RedDefeat
	msgbox KantoLeague_Text_RedPostBattle, MSGBOX_DEFAULT
	release
	end

KantoLeague_EventScript_RedDeclined::
	msgbox KantoLeague_Text_RedDeclined, MSGBOX_DEFAULT
	release
	end

KantoLeague_Text_ScottCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "SCOTT: {PLAYER}! SCOTT here!\p"
	.string "Word of your MASTER RANK win made\n"
	.string "it all the way to KANTO!\p"
	.string "And their ELITE FOUR were\n"
	.string "impressed. Very impressed!\p"
	.string "They want you to challenge the\n"
	.string "POKéMON LEAGUE at INDIGO PLATEAU!\p"
	.string "I'll be waiting by the harbor in\n"
	.string "LILYCOVE CITY to take you there!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

KantoLeague_Text_ScottReadyToGo:
	.string "SCOTT: There you are, {PLAYER}!\p"
	.string "I've got us a boat to KANTO.\p"
	.string "It'll drop us right by VICTORY\n"
	.string "ROAD's exit, a short walk from\l"
	.string "the INDIGO PLATEAU.\p"
	.string "I'll wait for you there whenever\n"
	.string "you want to come back.\p"
	.string "Ready to head to KANTO?$"

KantoLeague_Text_ScottLetsGo:
	.string "SCOTT: That's the spirit!\n"
	.string "Let's go!$"

KantoLeague_Text_ScottNotYet:
	.string "SCOTT: No rush. I'll be right here\n"
	.string "whenever you're ready.$"

KantoLeague_Text_ScottGoBack:
	.string "SCOTT: The INDIGO PLATEAU is just\n"
	.string "up ahead. KANTO's POKéMON LEAGUE!\p"
	.string "Want to head back to HOENN?$"

KantoLeague_Text_ScottBackToHoenn:
	.string "SCOTT: All right!\n"
	.string "Back to LILYCOVE we go!$"

KantoLeague_Text_ScottStay:
	.string "SCOTT: Go show KANTO what HOENN's\n"
	.string "CHAMPION can do!$"

KantoLeague_Text_LoreleiIntro:
	.string "Welcome to the POKéMON LEAGUE.\n"
	.string "You've come a long way from HOENN.\p"
	.string "I am LORELEI of KANTO's\n"
	.string "ELITE FOUR.\p"
	.string "No one can best me when it comes\n"
	.string "to icy POKéMON.\p"
	.string "Once your POKéMON are frozen\n"
	.string "solid, they're at my mercy.\p"
	.string "Hahaha!\n"
	.string "Are you ready?$"

KantoLeague_Text_LoreleiDefeat:
	.string "You're better than I thought!$"

KantoLeague_Text_LoreleiPostBattle:
	.string "Go on ahead.\p"
	.string "You've only had a taste of the\n"
	.string "KANTO ELITE FOUR's power!$"

KantoLeague_Text_BrunoIntro:
	.string "I am BRUNO of the ELITE FOUR!\p"
	.string "Through rigorous training, people\n"
	.string "and POKéMON grow stronger\l"
	.string "without limit.\p"
	.string "SCOTT told us all about you.\n"
	.string "Show me that strength!\p"
	.string "{PLAYER}! We will grind you down\n"
	.string "with our superior power!\p"
	.string "Hoo hah!$"

KantoLeague_Text_BrunoDefeat:
	.string "Why? How could I lose?$"

KantoLeague_Text_BrunoPostBattle:
	.string "My job is done.\n"
	.string "Go face your next challenge!$"

KantoLeague_Text_AgathaIntro:
	.string "I am AGATHA of the ELITE FOUR.\p"
	.string "So you're HOENN's CHAMPION?\n"
	.string "You look like a child to me.\p"
	.string "OAK sent his regards. Hmph!\n"
	.string "He should come battle himself.\p"
	.string "POKéMON are for battling!\p"
	.string "{PLAYER}! I'll show you how a real\n"
	.string "TRAINER battles!$"

KantoLeague_Text_AgathaDefeat:
	.string "Oh, my!\n"
	.string "You're something special, child!$"

KantoLeague_Text_AgathaPostBattle:
	.string "You win! I see what SCOTT saw in\n"
	.string "you now.\p"
	.string "I have nothing else to say.\n"
	.string "Run along now, child!$"

KantoLeague_Text_LanceIntro:
	.string "Ah! So you're {PLAYER}.\p"
	.string "I lead KANTO's ELITE FOUR.\n"
	.string "You can call me LANCE the dragon\l"
	.string "TRAINER.\p"
	.string "Dragons are mythical POKéMON.\n"
	.string "Their powers are superior.\p"
	.string "They're virtually indestructible.\n"
	.string "There's no being clever with them.\p"
	.string "Well, are you ready to lose?$"

KantoLeague_Text_LanceDefeat:
	.string "That's it! I hate to admit it,\n"
	.string "but you are a POKéMON master!$"

KantoLeague_Text_LancePostBattle:
	.string "I still can't believe my dragons\n"
	.string "lost to you, {PLAYER}!\p"
	.string "The CHAMPION is waiting beyond\n"
	.string "that door.\p"
	.string "BLUE has been looking forward to\n"
	.string "this since the day SCOTT called.$"

KantoLeague_Text_BlueIntro:
	.string "BLUE: Hey! {PLAYER}!\p"
	.string "So you're the HOENN CHAMPION\n"
	.string "everyone keeps talking about.\p"
	.string "I'm BLUE, KANTO's CHAMPION.\n"
	.string "I won this seat back, fair and\l"
	.string "square.\p"
	.string "I'm not losing it to some kid\n"
	.string "from HOENN!\p"
	.string "{PLAYER}! Do you know what that\n"
	.string "means?\p"
	.string "I'm the most powerful TRAINER\n"
	.string "in the world!$"

KantoLeague_Text_BlueDefeat:
	.string "NO! That can't be!\n"
	.string "You beat my best!$"

KantoLeague_Text_BluePostBattle:
	.string "BLUE: Why? Why did I lose?\p"
	.string "I never made any mistakes raising\n"
	.string "my POKéMON…\p"
	.string "Darn it! You're the real deal,\n"
	.string "{PLAYER}.$"

KantoLeague_Text_BlueOfferStarter:
	.string "BLUE: Oh, before I forget…\p"
	.string "Gramps asked me to give these to\n"
	.string "the TRAINER from HOENN.\p"
	.string "Pick one. Don't make me regret it.$"

KantoLeague_Text_BlueWhichStarter:
	.string "Which POKéMON will you take?$"

KantoLeague_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_2}\n"
	.string "from BLUE!$"

KantoLeague_Text_BlueTakeCareOfIt:
	.string "BLUE: Raise it well. Gramps will\n"
	.string "want to hear all about it.$"

KantoLeague_Text_BlueRedHint:
	.string "BLUE: …You know, RED isn't here.\n"
	.string "He left for HOENN a while back.\p"
	.string "Didn't say a word, as usual.\p"
	.string "Last I heard, he was on some\n"
	.string "mountain there where POKéMON are\l"
	.string "laid to rest.\p"
	.string "If you think you're the best,\n"
	.string "go find him.$"

KantoLeague_Text_OakPlayer:
	.string "OAK: {PLAYER}!$"

KantoLeague_Text_OakCongratulations:
	.string "OAK: So you're the TRAINER from\n"
	.string "HOENN everyone's talking about!\p"
	.string "You beat KANTO's ELITE FOUR, and\n"
	.string "my grandson BLUE as well!\p"
	.string "Sincerely, congratulations!\p"
	.string "You and your POKéMON have a bond\n"
	.string "few TRAINERS ever reach.$"

KantoLeague_Text_OakDisappointedInBlue:
	.string "OAK: BLUE…\n"
	.string "I came as soon as I heard.\p"
	.string "You lost because you forgot to\n"
	.string "trust your POKéMON.\p"
	.string "Remember that, and you'll be\n"
	.string "stronger the next time.$"

KantoLeague_Text_OakComeWithMe:
	.string "OAK: {PLAYER}, come with me.\n"
	.string "Let's record your victory.$"

KantoLeague_Text_OakHereWeHonor:
	.string "OAK: This room…\p"
	.string "This is where we keep records of\n"
	.string "POKéMON that prevailed through\l"
	.string "harsh battles.\p"
	.string "HOENN's LEAGUE offered to honor\n"
	.string "you here, among its own CHAMPIONS.$"

KantoLeague_Text_OakLetsRecord:
	.string "OAK: Come, let's record your name\n"
	.string "and the names of the partners who\l"
	.string "battled with you.$"

KantoLeague_Text_RedFirstMeeting:
	.string "… … …\p"
	.string "A silent TRAINER stands among\n"
	.string "the graves, staring into the fog.\p"
	.string "It's RED.$"

KantoLeague_Text_RedRematch:
	.string "RED: …$"

KantoLeague_Text_RedAskBattle:
	.string "RED is looking at you.\n"
	.string "Will you battle?$"

KantoLeague_Text_RedIntro:
	.string "RED: …!$"

KantoLeague_Text_RedDefeat:
	.string "…!$"

KantoLeague_Text_RedPostBattle:
	.string "RED: …\p"
	.string "RED smiled, then turned back\n"
	.string "toward the fog.$"

KantoLeague_Text_RedDeclined:
	.string "RED: …$"
