@ Post-game delivery of the four event island tickets by the Mystery Gift delivery man on Pokémon Center 2F,
@ recreating the original distributions: same messages, same items, same flags. After the Elite Four he hands
@ out one ticket per visit (the original events were one Wonder Card at a time), in this order:
@   EON TICKET    -> SOUTHERN ISLAND (Latios/Latias)   as Emerald's built-in Eon Ticket distribution
@   AURORA TICKET -> BIRTH ISLAND (Deoxys)              as MysteryGiftScript_AuroraTicket
@   MYSTIC TICKET -> NAVEL ROCK (Ho-Oh, Lugia)           as MysteryGiftScript_MysticTicket
@   OLD SEA MAP   -> FARAWAY ISLAND (Mew)               as MysteryGiftScript_OldSeaMap
@ A ticket counts as delivered if the original script would have refused it (already received, already in
@ the bag, or its legendary already battled/caught).

@ Sets VAR_0x8004 to the next ticket to deliver, or ITEM_NONE
EventTicket_EventScript_GetNextTicket::
	setvar VAR_0x8004, ITEM_NONE
	goto_if_unset FLAG_SYS_GAME_CLEAR, Common_EventScript_NopReturn
	goto_if_set FLAG_ENABLE_SHIP_SOUTHERN_ISLAND, EventTicket_EventScript_CheckAuroraTicket
	checkitem ITEM_EON_TICKET
	goto_if_eq VAR_RESULT, TRUE, EventTicket_EventScript_CheckAuroraTicket
	setvar VAR_0x8004, ITEM_EON_TICKET
	return

EventTicket_EventScript_CheckAuroraTicket::
	goto_if_set FLAG_RECEIVED_AURORA_TICKET, EventTicket_EventScript_CheckMysticTicket
	goto_if_set FLAG_BATTLED_DEOXYS, EventTicket_EventScript_CheckMysticTicket
	checkitem ITEM_AURORA_TICKET
	goto_if_eq VAR_RESULT, TRUE, EventTicket_EventScript_CheckMysticTicket
	setvar VAR_0x8004, ITEM_AURORA_TICKET
	return

EventTicket_EventScript_CheckMysticTicket::
	goto_if_set FLAG_RECEIVED_MYSTIC_TICKET, EventTicket_EventScript_CheckOldSeaMap
	goto_if_set FLAG_CAUGHT_LUGIA, EventTicket_EventScript_CheckOldSeaMap
	goto_if_set FLAG_CAUGHT_HO_OH, EventTicket_EventScript_CheckOldSeaMap
	checkitem ITEM_MYSTIC_TICKET
	goto_if_eq VAR_RESULT, TRUE, EventTicket_EventScript_CheckOldSeaMap
	setvar VAR_0x8004, ITEM_MYSTIC_TICKET
	return

EventTicket_EventScript_CheckOldSeaMap::
	goto_if_set FLAG_RECEIVED_OLD_SEA_MAP, Common_EventScript_NopReturn
	goto_if_set FLAG_CAUGHT_MEW, Common_EventScript_NopReturn
	checkitem ITEM_OLD_SEA_MAP
	goto_if_eq VAR_RESULT, TRUE, Common_EventScript_NopReturn
	setvar VAR_0x8004, ITEM_OLD_SEA_MAP
	return

@ Talking to the delivery man. Falls through to the normal Mystery Gift behavior when no ticket is due
EventTicket_EventScript_TryDeliverTicket::
	goto_if_set FLAG_TEMP_EVENT_TICKET_DELIVERED, EventTicket_EventScript_ComeBackLater
	call EventTicket_EventScript_GetNextTicket
	goto_if_eq VAR_0x8004, ITEM_EON_TICKET, EventTicket_EventScript_DeliverEonTicket
	goto_if_eq VAR_0x8004, ITEM_AURORA_TICKET, EventTicket_EventScript_DeliverAuroraTicket
	goto_if_eq VAR_0x8004, ITEM_MYSTIC_TICKET, EventTicket_EventScript_DeliverMysticTicket
	goto_if_eq VAR_0x8004, ITEM_OLD_SEA_MAP, EventTicket_EventScript_DeliverOldSeaMap
	return

EventTicket_EventScript_DeliverEonTicket::
	lock
	faceplayer
	msgbox MysteryGift_Text_TheresATicketForYou, MSGBOX_DEFAULT
	checkitemspace ITEM_EON_TICKET
	goto_if_eq VAR_RESULT, FALSE, EventTicket_EventScript_BagFull
	giveitem ITEM_EON_TICKET
	setflag FLAG_ENABLE_SHIP_SOUTHERN_ISLAND
	setvar VAR_DISTRIBUTE_EON_TICKET, 0
	setflag FLAG_TEMP_EVENT_TICKET_DELIVERED
	msgbox MysteryGift_Text_TryUsingItAtLilycovePort, MSGBOX_DEFAULT
	release
	end

EventTicket_EventScript_DeliverAuroraTicket::
	lock
	faceplayer
	msgbox EventTicket_Text_TicketForYou, MSGBOX_DEFAULT
	checkitemspace ITEM_AURORA_TICKET
	goto_if_eq VAR_RESULT, FALSE, EventTicket_EventScript_BagFull
	giveitem ITEM_AURORA_TICKET
	setflag FLAG_ENABLE_SHIP_BIRTH_ISLAND
	setflag FLAG_RECEIVED_AURORA_TICKET
	setflag FLAG_TEMP_EVENT_TICKET_DELIVERED
	msgbox EventTicket_Text_UseAtLilycovePort, MSGBOX_DEFAULT
	release
	end

EventTicket_EventScript_DeliverMysticTicket::
	lock
	faceplayer
	msgbox EventTicket_Text_TicketForYou, MSGBOX_DEFAULT
	checkitemspace ITEM_MYSTIC_TICKET
	goto_if_eq VAR_RESULT, FALSE, EventTicket_EventScript_BagFull
	giveitem ITEM_MYSTIC_TICKET
	setflag FLAG_ENABLE_SHIP_NAVEL_ROCK
	setflag FLAG_RECEIVED_MYSTIC_TICKET
	setflag FLAG_TEMP_EVENT_TICKET_DELIVERED
	msgbox EventTicket_Text_UseAtLilycovePort, MSGBOX_DEFAULT
	release
	end

EventTicket_EventScript_DeliverOldSeaMap::
	lock
	faceplayer
	msgbox EventTicket_Text_OldSeaMapForYou, MSGBOX_DEFAULT
	checkitemspace ITEM_OLD_SEA_MAP
	goto_if_eq VAR_RESULT, FALSE, EventTicket_EventScript_BagFull
	giveitem ITEM_OLD_SEA_MAP
	setflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	setflag FLAG_RECEIVED_OLD_SEA_MAP
	setflag FLAG_TEMP_EVENT_TICKET_DELIVERED
	msgbox EventTicket_Text_UseAtLilycovePort, MSGBOX_DEFAULT
	release
	end

EventTicket_EventScript_BagFull::
	msgbox EventTicket_Text_BagFull, MSGBOX_DEFAULT
	release
	end

@ One delivery per visit: the next one is waiting next time the player comes up to 2F
EventTicket_EventScript_ComeBackLater::
	msgbox EventTicket_Text_ThankYou, MSGBOX_NPC
	end

@ Texts from the original Mystery Gift distribution scripts (data/scripts/gift_*.inc)
EventTicket_Text_TicketForYou:
	.string "Thank you for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "You must be {PLAYER}.\n"
	.string "There is a ticket here for you.$"

EventTicket_Text_OldSeaMapForYou:
	.string "Thank you for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "Let me confirm--you are {PLAYER}?\p"
	.string "We received this OLD SEA MAP\n"
	.string "addressed to you.$"

EventTicket_Text_UseAtLilycovePort:
	.string "It appears to be for use at the\n"
	.string "LILYCOVE CITY port.\p"
	.string "Why not give it a try and see what\n"
	.string "it is about?$"

EventTicket_Text_ThankYou:
	.string "Thank you for using the MYSTERY\n"
	.string "GIFT System.$"

EventTicket_Text_BagFull:
	.string "Oh, I'm sorry, {PLAYER}.\n"
	.string "Your BAG's KEY ITEMS POCKET is full.\p"
	.string "Please store something on your PC,\n"
	.string "then come back for this.$"
