@ Kanto Exhibition. After Master Rank is cleared, Scott calls about a rumoured show match, and a fan appears in the
@ League lobby. Telling them the rumours are true sets VAR_LEAGUE_CHALLENGE to LEAGUE_CHALLENGE_KANTO_BLUE for the
@ next run: the Kanto Elite Four (TRAINER_*_EXHIBITION) with Blue as Champion. The Hall of Fame sets it back to
@ Master Rank. The Elite Four rooms swap their member's sprite (VAR_OBJ_GFX_ID_0) and branch here before Master Rank.
@ Once Blue has been beaten, Red waits at the summit of Mt. Pyre.

@ Started by ShouldDoScottKantoExhibitionCall (src/field_specials.c)
KantoExhibition_EventScript_ScottCall::
	lockall
	pokenavcall KantoExhibition_Text_ScottCall
	waitmessage
	setflag FLAG_KANTO_EXHIBITION_UNLOCKED
	releaseall
	end

@ League lobby
KantoExhibition_EventScript_SetupLeagueLobby::
	call_if_gt VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_KANTO_BLUE, KantoExhibition_EventScript_ResetLeagueChallenge
	setflag FLAG_HIDE_LEAGUE_CHALLENGE_ATTENDANT
	goto_if_unset FLAG_KANTO_EXHIBITION_UNLOCKED, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_LEAGUE_CHALLENGE_ATTENDANT
	return

@ Also called by the Hall of Fame, so each Kanto run has to be asked for again
KantoExhibition_EventScript_ResetLeagueChallenge::
	setvar VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_MASTER_RANK
	return

EverGrandeCity_PokemonLeague_1F_EventScript_ChallengeAttendant::
	lock
	faceplayer
	goto_if_eq VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_KANTO_BLUE, KantoExhibition_EventScript_FanKantoRunSet
	msgbox KantoExhibition_Text_FanRumours, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoExhibition_EventScript_FanJustRumours
	setvar VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_KANTO_BLUE
	msgbox KantoExhibition_Text_FanRumoursTrue, MSGBOX_DEFAULT
	release
	end

KantoExhibition_EventScript_FanKantoRunSet::
	msgbox KantoExhibition_Text_FanStillOn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoExhibition_EventScript_FanJustRumours
	msgbox KantoExhibition_Text_FanGoGetThem, MSGBOX_DEFAULT
	release
	end

KantoExhibition_EventScript_FanJustRumours::
	setvar VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_MASTER_RANK
	msgbox KantoExhibition_Text_FanJustRumours, MSGBOX_DEFAULT
	release
	end

@ Elite Four rooms: ON_TRANSITION sprite for the room's member
KantoExhibition_EventScript_SetupSidneysRoomGfx::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_SIDNEY
	goto_if_eq VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_MASTER_RANK, Common_EventScript_NopReturn
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_LORELEI
	return

KantoExhibition_EventScript_SetupPhoebesRoomGfx::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_PHOEBE
	goto_if_eq VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_MASTER_RANK, KantoExhibition_EventScript_End
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_BRUNO
	end

KantoExhibition_EventScript_SetupGlaciasRoomGfx::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_GLACIA
	goto_if_eq VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_MASTER_RANK, KantoExhibition_EventScript_End
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_AGATHA
	end

KantoExhibition_EventScript_SetupDrakesRoomGfx::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_DRAKE
	goto_if_eq VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_MASTER_RANK, KantoExhibition_EventScript_End
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_LANCE
	end

KantoExhibition_EventScript_End::
	end

@ Elite Four battles. Each room keeps its own defeated flag and door metatiles, so the run works the same way.
KantoExhibition_EventScript_Lorelei::
	goto_if_set FLAG_DEFEATED_ELITE_4_SIDNEY, KantoExhibition_EventScript_LoreleiPostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoExhibition_Text_LoreleiIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LORELEI_EXHIBITION, KantoExhibition_Text_LoreleiDefeat
	setflag FLAG_DEFEATED_ELITE_4_SIDNEY
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
KantoExhibition_EventScript_LoreleiPostBattleSpeech::
	msgbox KantoExhibition_Text_LoreleiPostBattle, MSGBOX_DEFAULT
	release
	end

KantoExhibition_EventScript_Bruno::
	goto_if_set FLAG_DEFEATED_ELITE_4_PHOEBE, KantoExhibition_EventScript_BrunoPostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoExhibition_Text_BrunoIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_BRUNO_EXHIBITION, KantoExhibition_Text_BrunoDefeat
	setflag FLAG_DEFEATED_ELITE_4_PHOEBE
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
KantoExhibition_EventScript_BrunoPostBattleSpeech::
	msgbox KantoExhibition_Text_BrunoPostBattle, MSGBOX_DEFAULT
	release
	end

KantoExhibition_EventScript_Agatha::
	goto_if_set FLAG_DEFEATED_ELITE_4_GLACIA, KantoExhibition_EventScript_AgathaPostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoExhibition_Text_AgathaIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_AGATHA_EXHIBITION, KantoExhibition_Text_AgathaDefeat
	setflag FLAG_DEFEATED_ELITE_4_GLACIA
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
KantoExhibition_EventScript_AgathaPostBattleSpeech::
	msgbox KantoExhibition_Text_AgathaPostBattle, MSGBOX_DEFAULT
	release
	end

KantoExhibition_EventScript_Lance::
	goto_if_set FLAG_DEFEATED_ELITE_4_DRAKE, KantoExhibition_EventScript_LancePostBattleSpeech
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, FALSE
	msgbox KantoExhibition_Text_LanceIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_LANCE_EXHIBITION, KantoExhibition_Text_LanceDefeat
	setvar VAR_0x8004, FANCOUNTER_DEFEATED_DRAKE
	special Script_TryGainNewFanFromCounter
	setflag FLAG_DEFEATED_ELITE_4_DRAKE
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
KantoExhibition_EventScript_LancePostBattleSpeech::
	msgbox KantoExhibition_Text_LancePostBattle, MSGBOX_DEFAULT
	release
	end

@ Champion's room: the champion object (LOCALID_CHAMPIONS_ROOM_WALLACE) shows Blue, see
@ MasterRank_EventScript_SetupChampionGfxId
KantoExhibition_EventScript_SetupChampionGfxId::
	goto_if_ne VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_KANTO_BLUE, Common_EventScript_NopReturn
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_BLUE
	return

@ Blue's team counters the player's Hoenn starter, like the rival in FRLG
KantoExhibition_EventScript_Blue::
	playbgm MUS_RG_ENCOUNTER_RIVAL, FALSE
	msgbox KantoExhibition_Text_BlueIntro, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 1, KantoExhibition_EventScript_BattleBlueBlastoise
	case 2, KantoExhibition_EventScript_BattleBlueVenusaur
	trainerbattle_no_intro TRAINER_BLUE_EXHIBITION_CHARIZARD, KantoExhibition_Text_BlueDefeat
	goto KantoExhibition_EventScript_BlueDefeated
	end

KantoExhibition_EventScript_BattleBlueBlastoise::
	trainerbattle_no_intro TRAINER_BLUE_EXHIBITION_BLASTOISE, KantoExhibition_Text_BlueDefeat
	goto KantoExhibition_EventScript_BlueDefeated
	end

KantoExhibition_EventScript_BattleBlueVenusaur::
	trainerbattle_no_intro TRAINER_BLUE_EXHIBITION_VENUSAUR, KantoExhibition_Text_BlueDefeat
	goto KantoExhibition_EventScript_BlueDefeated
	end

KantoExhibition_EventScript_BlueDefeated::
	playse SE_DOOR
	setmetatile 6, 1, METATILE_EliteFour_OpenDoorChampion_Frame, FALSE
	setmetatile 6, 2, METATILE_EliteFour_OpenDoorChampion_Opening, FALSE
	special DrawWholeMapView
	msgbox KantoExhibition_Text_BluePostBattle, MSGBOX_DEFAULT
	call_if_unset FLAG_RECEIVED_KANTO_STARTER, KantoExhibition_EventScript_BlueGiveStarter
	msgbox KantoExhibition_Text_BlueRedHint, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart
	waitmovement 0
	msgbox KantoExhibition_Text_BlueThisWay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExit
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExit
	waitmovement 0
	setflag FLAG_HIDE_PETALBURG_GYM_GREETER
	warp MAP_EVER_GRANDE_CITY_HALL_OF_FAME, 7, 16
	waitstate
	releaseall
	end

@ First win against Blue: Professor Oak's Kanto starters, one per save. If the party and PC are both full the flag
@ stays unset and Blue offers again next time.
KantoExhibition_EventScript_BlueGiveStarter::
	msgbox KantoExhibition_Text_BlueOfferStarter, MSGBOX_DEFAULT
	message KantoExhibition_Text_BlueWhichStarter
	waitmessage
	multichoice 18, 6, MULTI_KANTO_STARTER, TRUE
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_BULBASAUR
	call_if_eq VAR_RESULT, 1, KantoExhibition_EventScript_ChooseCharmander
	call_if_eq VAR_RESULT, 2, KantoExhibition_EventScript_ChooseSquirtle
	givemon VAR_TEMP_TRANSFERRED_SPECIES, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, KantoExhibition_EventScript_StarterToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, KantoExhibition_EventScript_StarterToPC
	msgbox gText_NoMoreRoomForPokemon, MSGBOX_DEFAULT
	return

KantoExhibition_EventScript_ChooseCharmander::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHARMANDER
	return

KantoExhibition_EventScript_ChooseSquirtle::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_SQUIRTLE
	return

KantoExhibition_EventScript_StarterToParty::
	call KantoExhibition_EventScript_ReceivedStarterFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoExhibition_EventScript_ReceivedStarter
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto KantoExhibition_EventScript_ReceivedStarter

KantoExhibition_EventScript_StarterToPC::
	call KantoExhibition_EventScript_ReceivedStarterFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Common_EventScript_NameReceivedBoxMon
	call Common_EventScript_TransferredToPC
	goto KantoExhibition_EventScript_ReceivedStarter

KantoExhibition_EventScript_ReceivedStarterFanfare::
	bufferspeciesname STR_VAR_2, VAR_TEMP_TRANSFERRED_SPECIES
	playfanfare MUS_OBTAIN_ITEM
	message KantoExhibition_Text_ReceivedStarter
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	return

KantoExhibition_EventScript_ReceivedStarter::
	setflag FLAG_RECEIVED_KANTO_STARTER
	msgbox KantoExhibition_Text_BlueTakeCareOfIt, MSGBOX_DEFAULT
	return

@ Hall of Fame: Blue speaks in the champion's place. VAR_RESULT is TRUE if he did.
KantoExhibition_EventScript_HallOfFameHereWeHonor::
	setvar VAR_RESULT, FALSE
	goto_if_ne VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_KANTO_BLUE, Common_EventScript_NopReturn
	msgbox KantoExhibition_Text_BlueHereWeHonor, MSGBOX_DEFAULT
	setvar VAR_RESULT, TRUE
	return

KantoExhibition_EventScript_HallOfFameLetsRecord::
	setvar VAR_RESULT, FALSE
	goto_if_ne VAR_LEAGUE_CHALLENGE, LEAGUE_CHALLENGE_KANTO_BLUE, Common_EventScript_NopReturn
	msgbox KantoExhibition_Text_BlueLetsRecord, MSGBOX_DEFAULT
	setvar VAR_RESULT, TRUE
	return

@ Mt. Pyre summit: Red appears once Blue has been beaten in any of his three teams, and can be fought any time
KantoExhibition_EventScript_SetupMtPyreRed::
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_RED
	goto_if_defeated TRAINER_BLUE_EXHIBITION_CHARIZARD, KantoExhibition_EventScript_ShowMtPyreRed
	goto_if_defeated TRAINER_BLUE_EXHIBITION_BLASTOISE, KantoExhibition_EventScript_ShowMtPyreRed
	goto_if_defeated TRAINER_BLUE_EXHIBITION_VENUSAUR, KantoExhibition_EventScript_ShowMtPyreRed
	return

KantoExhibition_EventScript_ShowMtPyreRed::
	clearflag FLAG_HIDE_MT_PYRE_SUMMIT_RED
	return

MtPyre_Summit_EventScript_Red::
	lock
	faceplayer
	goto_if_defeated TRAINER_RED_EXHIBITION, KantoExhibition_EventScript_RedRematch
	msgbox KantoExhibition_Text_RedFirstMeeting, MSGBOX_DEFAULT
	goto KantoExhibition_EventScript_RedAskBattle
	end

KantoExhibition_EventScript_RedRematch::
	msgbox KantoExhibition_Text_RedRematch, MSGBOX_DEFAULT
KantoExhibition_EventScript_RedAskBattle::
	msgbox KantoExhibition_Text_RedAskBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoExhibition_EventScript_RedDeclined
	msgbox KantoExhibition_Text_RedIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_RED_EXHIBITION, KantoExhibition_Text_RedDefeat
	msgbox KantoExhibition_Text_RedPostBattle, MSGBOX_DEFAULT
	release
	end

KantoExhibition_EventScript_RedDeclined::
	msgbox KantoExhibition_Text_RedDeclined, MSGBOX_DEFAULT
	release
	end

KantoExhibition_Text_ScottCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "SCOTT: Hey, {PLAYER}! It's me,\n"
	.string "SCOTT!\p"
	.string "Word travels fast! You beat STEVEN\n"
	.string "at MASTER RANK, didn't you?\p"
	.string "Well, here's something that'll\n"
	.string "make your heart race…\p"
	.string "There's a rumor that KANTO's\n"
	.string "LEAGUE came to HOENN for a\l"
	.string "show match!\p"
	.string "Their ELITE FOUR, and their\n"
	.string "CHAMPION BLUE too!\p"
	.string "Folks at the LEAGUE lobby can't\n"
	.string "stop talking about it.\l"
	.string "Go see what they know!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

KantoExhibition_Text_FanRumours:
	.string "Hey! I heard KANTO's LEAGUE is\n"
	.string "putting on a show match this week!\p"
	.string "Their ELITE FOUR, right behind\n"
	.string "that door, and BLUE as CHAMPION!\p"
	.string "Are the rumors really true?$"

KantoExhibition_Text_FanRumoursTrue:
	.string "No way! I knew it!\p"
	.string "Then KANTO's ELITE FOUR are the\n"
	.string "ones waiting for you in there!\p"
	.string "Show them what HOENN's CHAMPION\n"
	.string "can do, {PLAYER}!$"

KantoExhibition_Text_FanStillOn:
	.string "The KANTO show match is on!\n"
	.string "It's on, right?$"

KantoExhibition_Text_FanGoGetThem:
	.string "I knew it! BLUE won't know what\n"
	.string "hit him. Go get them!$"

KantoExhibition_Text_FanJustRumours:
	.string "Aww… Just rumors, then.\p"
	.string "MASTER RANK as usual. STEVEN's\n"
	.string "waiting for you in there!$"

KantoExhibition_Text_LoreleiIntro:
	.string "Welcome to the POKéMON LEAGUE…\n"
	.string "HOENN's, this time.\p"
	.string "I am LORELEI of KANTO's\n"
	.string "ELITE FOUR.\p"
	.string "No one can best me when it comes\n"
	.string "to icy POKéMON.\p"
	.string "Once your POKéMON are frozen\n"
	.string "solid, they're at my mercy.\p"
	.string "Hahaha!\n"
	.string "Are you ready?$"

KantoExhibition_Text_LoreleiDefeat:
	.string "You're better than I thought!$"

KantoExhibition_Text_LoreleiPostBattle:
	.string "Go on ahead.\p"
	.string "You've only had a taste of the\n"
	.string "KANTO ELITE FOUR's power!$"

KantoExhibition_Text_BrunoIntro:
	.string "I am BRUNO of the ELITE FOUR!\p"
	.string "Through rigorous training, people\n"
	.string "and POKéMON grow stronger\l"
	.string "without limit.\p"
	.string "HOENN's mountains made for fine\n"
	.string "training on the way here!\p"
	.string "{PLAYER}! We will grind you down\n"
	.string "with our superior power!\p"
	.string "Hoo hah!$"

KantoExhibition_Text_BrunoDefeat:
	.string "Why? How could I lose?$"

KantoExhibition_Text_BrunoPostBattle:
	.string "My job is done.\n"
	.string "Go face your next challenge!$"

KantoExhibition_Text_AgathaIntro:
	.string "I am AGATHA of the ELITE FOUR.\p"
	.string "So you're HOENN's CHAMPION?\n"
	.string "You look like a child to me.\p"
	.string "OAK sent his regards. Hmph!\n"
	.string "He should come battle himself.\p"
	.string "POKéMON are for battling!\p"
	.string "{PLAYER}! I'll show you how a real\n"
	.string "TRAINER battles!$"

KantoExhibition_Text_AgathaDefeat:
	.string "Oh, my!\n"
	.string "You're something special, child!$"

KantoExhibition_Text_AgathaPostBattle:
	.string "You win! I see what SCOTT saw in\n"
	.string "you now.\p"
	.string "I have nothing else to say.\n"
	.string "Run along now, child!$"

KantoExhibition_Text_LanceIntro:
	.string "Ah! So you're {PLAYER}.\p"
	.string "I lead KANTO's ELITE FOUR.\n"
	.string "You can call me LANCE the dragon\l"
	.string "TRAINER.\p"
	.string "Dragons are mythical POKéMON.\n"
	.string "Their powers are superior.\p"
	.string "They're virtually indestructible.\n"
	.string "There's no being clever with them.\p"
	.string "Well, are you ready to lose?$"

KantoExhibition_Text_LanceDefeat:
	.string "That's it! I hate to admit it,\n"
	.string "but you are a POKéMON master!$"

KantoExhibition_Text_LancePostBattle:
	.string "I still can't believe my dragons\n"
	.string "lost to you, {PLAYER}!\p"
	.string "The CHAMPION is waiting beyond\n"
	.string "that door.\p"
	.string "BLUE has been looking forward to\n"
	.string "this since the day SCOTT called.$"

KantoExhibition_Text_BlueIntro:
	.string "BLUE: Hey! {PLAYER}!\p"
	.string "So you're the HOENN CHAMPION\n"
	.string "everyone keeps talking about.\p"
	.string "I'm BLUE. I was KANTO's CHAMPION\n"
	.string "once. For a few minutes, anyway.\p"
	.string "Since then, I've beaten everyone\n"
	.string "who ever got the better of me.\p"
	.string "And you're next!\p"
	.string "{PLAYER}! Do you know what that\n"
	.string "means?\p"
	.string "I'm the most powerful TRAINER\n"
	.string "in the world!$"

KantoExhibition_Text_BlueDefeat:
	.string "NO! That can't be!\n"
	.string "You beat my best!$"

KantoExhibition_Text_BluePostBattle:
	.string "BLUE: Why? Why did I lose?\p"
	.string "I never made any mistakes raising\n"
	.string "my POKéMON…\p"
	.string "Darn it! You're the real deal,\n"
	.string "{PLAYER}.$"

KantoExhibition_Text_BlueOfferStarter:
	.string "BLUE: Oh, before I forget…\p"
	.string "Gramps asked me to bring these\n"
	.string "for a TRAINER in HOENN.\p"
	.string "Pick one. Don't make me regret it.$"

KantoExhibition_Text_BlueWhichStarter:
	.string "Which POKéMON will you take?$"

KantoExhibition_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_2}\n"
	.string "from BLUE!$"

KantoExhibition_Text_BlueTakeCareOfIt:
	.string "BLUE: Raise it well. Gramps will\n"
	.string "want to hear all about it.$"

KantoExhibition_Text_BlueRedHint:
	.string "BLUE: …You know, RED came to\n"
	.string "HOENN with us.\p"
	.string "He took off the moment we landed.\n"
	.string "Didn't say a word, as usual.\p"
	.string "Last I saw, he was heading for\n"
	.string "some mountain where POKéMON are\l"
	.string "laid to rest.\p"
	.string "If you think you're the best,\n"
	.string "go find him.$"

KantoExhibition_Text_BlueThisWay:
	.string "BLUE: Come on, the HALL OF FAME\n"
	.string "is this way. Don't make me wait.$"

KantoExhibition_Text_BlueHereWeHonor:
	.string "BLUE: So this is HOENN's\n"
	.string "HALL OF FAME.\p"
	.string "Not bad. The one at INDIGO\n"
	.string "PLATEAU is bigger, though.$"

KantoExhibition_Text_BlueLetsRecord:
	.string "BLUE: Go on, record your name and\n"
	.string "your partners'. You earned it…\p"
	.string "This time, anyway.$"

KantoExhibition_Text_RedFirstMeeting:
	.string "… … …\p"
	.string "A silent TRAINER stands among\n"
	.string "the graves, staring into the fog.\p"
	.string "It's RED.$"

KantoExhibition_Text_RedRematch:
	.string "RED: …$"

KantoExhibition_Text_RedAskBattle:
	.string "RED is looking at you.\n"
	.string "Will you battle?$"

KantoExhibition_Text_RedIntro:
	.string "RED: …!$"

KantoExhibition_Text_RedDefeat:
	.string "…!$"

KantoExhibition_Text_RedPostBattle:
	.string "RED: …\p"
	.string "RED smiled, then turned back\n"
	.string "toward the fog.$"

KantoExhibition_Text_RedDeclined:
	.string "RED: …$"
