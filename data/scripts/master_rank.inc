@ Master Rank League. After all eight round 2 Gym Leader rematches (src/gym_leader_rematch.c), Wally calls and the
@ League becomes Master Rank: enhanced Elite Four (TRAINER_*_MASTER), and Steven as Champion in Wallace's place.
@ VAR_MASTER_RANK_STATE: MASTER_RANK_LOCKED -> MASTER_RANK_UNLOCKED (this call) -> MASTER_RANK_CLEARED (Steven beaten).
@ Juan hints at it in SootopolisCity_Gym_1F; the League changes are in the EverGrandeCity_* room scripts.

@ Started by ShouldDoWallyMasterRankCall (src/field_specials.c)
MasterRank_EventScript_WallyCall::
	lockall
	pokenavcall MasterRank_Text_WallyCall
	waitmessage
	setvar VAR_MASTER_RANK_STATE, MASTER_RANK_UNLOCKED
	releaseall
	end

MasterRank_Text_WallyCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "WALLY: {PLAYER}! It's me, WALLY!\p"
	.string "I was training at VICTORY ROAD, and\n"
	.string "I saw the ELITE FOUR heading to\l"
	.string "EVER GRANDE CITY together!\p"
	.string "Everyone's whispering about\n"
	.string "something called MASTER RANK…\p"
	.string "They say there's a new CHAMPION\n"
	.string "now, but nobody will say who!\p"
	.string "I'm not strong enough yet…\n"
	.string "But you are, {PLAYER}!\l"
	.string "Go and see for yourself!\p"
	.string "Then tell me everything, okay?\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

@ The Champion's room and Hall of Fame show their champion with VAR_OBJ_GFX_ID_1: Wallace, or Steven in Master Rank,
@ or Blue or Red in the Kanto Exhibition (data/scripts/kanto_exhibition.inc)
MasterRank_EventScript_SetupChampionGfxId::
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_WALLACE
	goto_if_eq VAR_MASTER_RANK_STATE, MASTER_RANK_LOCKED, Common_EventScript_NopReturn
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_STEVEN
	goto KantoExhibition_EventScript_SetupChampionGfxId

MasterRank_EventScript_SetupChampionGfxIdEnd::
	call MasterRank_EventScript_SetupChampionGfxId
	end
