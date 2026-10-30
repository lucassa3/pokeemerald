@ The two Hoenn starters the player didn't pick, so the Hoenn Pokédex needs no trading. Once Birch has upgraded the
@ Pokédex to National Mode (VAR_DEX_UPGRADE_JOHTO_STARTER_STATE >= 2), the rival gives an Egg of their own starter
@ and Birch gives the one nobody picked. Both happen right after the upgrade, or later by talking to them in the lab.
@ If the party and PC are full, the flag stays unset and they offer again next time.

@ Called at the end of LittlerootTown_ProfessorBirchsLab_EventScript_UpgradeToNationalDex
HoennStarters_EventScript_AfterNationalDexUpgrade::
	call HoennStarters_EventScript_RivalGiveEgg
	call HoennStarters_EventScript_BirchGiveStarter
	return

HoennStarters_EventScript_RivalTalk::
	goto_if_lt VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_RivalAfterHoennStarter
	call HoennStarters_EventScript_RivalGiveEgg
	release
	end

HoennStarters_EventScript_BirchTalk::
	goto_if_lt VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_BirchAfterHoennStarter
	call HoennStarters_EventScript_BirchGiveStarter
	release
	end

@ The rival chose the starter strong against the player's: Treecko -> Torchic -> Mudkip -> Treecko
HoennStarters_EventScript_RivalGiveEgg::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, HoennStarters_EventScript_MayOfferEgg
	call_if_eq VAR_RESULT, FEMALE, HoennStarters_EventScript_BrendanOfferEgg
	setvar VAR_0x8004, SPECIES_TORCHIC
	call_if_eq VAR_STARTER_MON, 1, HoennStarters_EventScript_SetMudkip
	call_if_eq VAR_STARTER_MON, 2, HoennStarters_EventScript_SetTreecko
	giveegg VAR_0x8004
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, HoennStarters_EventScript_NoRoom
	playfanfare MUS_OBTAIN_ITEM
	message HoennStarters_Text_ReceivedEgg
	waitmessage
	waitfanfare
	call_if_eq VAR_RESULT, MON_GIVEN_TO_PC, HoennStarters_EventScript_EggTransferredToPC
	setflag FLAG_RECEIVED_RIVAL_STARTER_EGG
	checkplayergender
	call_if_eq VAR_RESULT, MALE, HoennStarters_EventScript_MayAfterEgg
	call_if_eq VAR_RESULT, FEMALE, HoennStarters_EventScript_BrendanAfterEgg
	return

@ Common_EventScript_TransferredToPC, but naming the Egg instead of its species
HoennStarters_EventScript_EggTransferredToPC::
	bufferboxname STR_VAR_1, VAR_PC_BOX_TO_SEND_MON
	bufferstring STR_VAR_2, gText_EggNickname
	call_if_unset FLAG_SYS_PC_LANETTE, EventScript_TransferredSomeonesPC
	call_if_set FLAG_SYS_PC_LANETTE, EventScript_TransferredLanettesPC
	return

HoennStarters_EventScript_MayOfferEgg::
	msgbox HoennStarters_Text_MayOfferEgg, MSGBOX_DEFAULT
	return

HoennStarters_EventScript_BrendanOfferEgg::
	msgbox HoennStarters_Text_BrendanOfferEgg, MSGBOX_DEFAULT
	return

HoennStarters_EventScript_MayAfterEgg::
	msgbox HoennStarters_Text_MayAfterEgg, MSGBOX_DEFAULT
	return

HoennStarters_EventScript_BrendanAfterEgg::
	msgbox HoennStarters_Text_BrendanAfterEgg, MSGBOX_DEFAULT
	return

@ Birch's third Poké Ball: Treecko -> Mudkip, Torchic -> Treecko, Mudkip -> Torchic
HoennStarters_EventScript_BirchGiveStarter::
	msgbox HoennStarters_Text_BirchOfferStarter, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_MUDKIP
	call_if_eq VAR_STARTER_MON, 1, HoennStarters_EventScript_SetTreeckoGift
	call_if_eq VAR_STARTER_MON, 2, HoennStarters_EventScript_SetTorchicGift
	givemon VAR_TEMP_TRANSFERRED_SPECIES, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, HoennStarters_EventScript_StarterToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, HoennStarters_EventScript_StarterToPC
HoennStarters_EventScript_NoRoom::
	msgbox gText_NoMoreRoomForPokemon, MSGBOX_DEFAULT
	return

HoennStarters_EventScript_SetMudkip::
	setvar VAR_0x8004, SPECIES_MUDKIP
	return

HoennStarters_EventScript_SetTreecko::
	setvar VAR_0x8004, SPECIES_TREECKO
	return

HoennStarters_EventScript_SetTreeckoGift::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TREECKO
	return

HoennStarters_EventScript_SetTorchicGift::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TORCHIC
	return

HoennStarters_EventScript_StarterToParty::
	call HoennStarters_EventScript_ReceivedStarterFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HoennStarters_EventScript_ReceivedStarter
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto HoennStarters_EventScript_ReceivedStarter

HoennStarters_EventScript_StarterToPC::
	call HoennStarters_EventScript_ReceivedStarterFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Common_EventScript_NameReceivedBoxMon
	call Common_EventScript_TransferredToPC
	goto HoennStarters_EventScript_ReceivedStarter

HoennStarters_EventScript_ReceivedStarterFanfare::
	bufferspeciesname STR_VAR_2, VAR_TEMP_TRANSFERRED_SPECIES
	playfanfare MUS_OBTAIN_ITEM
	message HoennStarters_Text_ReceivedStarter
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	return

HoennStarters_EventScript_ReceivedStarter::
	setflag FLAG_RECEIVED_BIRCH_HOENN_STARTER
	msgbox HoennStarters_Text_BirchAfterStarter, MSGBOX_DEFAULT
	return

HoennStarters_Text_MayOfferEgg:
	.string "MAY: Oh, {PLAYER}{KUN}! Before you go,\n"
	.string "I've got something for you!\p"
	.string "My POKéMON from the very first day\n"
	.string "had an EGG at the DAY CARE!\p"
	.string "I want you to raise it.\n"
	.string "Here, it's yours!$"

HoennStarters_Text_BrendanOfferEgg:
	.string "BRENDAN: Hey, {PLAYER}, wait up.\p"
	.string "My first partner left an EGG at\n"
	.string "the DAY CARE.\p"
	.string "I figure the CHAMPION should\n"
	.string "be the one to raise it.\l"
	.string "Here, take it!$"

HoennStarters_Text_ReceivedEgg:
	.string "{PLAYER} received the EGG.$"

HoennStarters_Text_MayAfterEgg:
	.string "MAY: Take good care of it!\n"
	.string "I can't wait to see it hatch!$"

HoennStarters_Text_BrendanAfterEgg:
	.string "BRENDAN: Raise it strong, okay?\n"
	.string "Then we'll have a rematch!$"

HoennStarters_Text_BirchOfferStarter:
	.string "PROF. BIRCH: Ah, that reminds me!\p"
	.string "One of the three POKéMON from\n"
	.string "my bag never found a TRAINER.\p"
	.string "I can't think of anyone better\n"
	.string "than the CHAMPION to raise it.$"

HoennStarters_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_2}\n"
	.string "from PROF. BIRCH!$"

HoennStarters_Text_BirchAfterStarter:
	.string "PROF. BIRCH: Now you have all\n"
	.string "three of HOENN's first partners.\p"
	.string "Take good care of them!$"
