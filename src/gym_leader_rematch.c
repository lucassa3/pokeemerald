#include "global.h"
#include "random.h"
#include "event_data.h"
#include "battle_setup.h"
#include "gym_leader_rematch.h"

static void UpdateGymLeaderRematchFromArray(const u16 *data, size_t size, u32 maxRematch);
static s32 GetRematchIndex(u32 trainerIdx);

// Each leader's second battle (index 1 in gRematchTable) is offered deterministically, one leader at a time in
// gym order, from the Hall of Fame on (UpdateGymLeaderRematchChain). Later battles stay random (UpdateGymLeaderRematch).
#define ROUND_2_REMATCH 1

// Also the order of the round 2 chain
static const u16 GymLeaderRematches_AfterNewMauville[] = {
    REMATCH_ROXANNE,
    REMATCH_BRAWLY,
    REMATCH_WATTSON,
    REMATCH_FLANNERY,
    REMATCH_NORMAN,
    REMATCH_WINONA,
    REMATCH_TATE_AND_LIZA,
    REMATCH_JUAN
};

static const u16 GymLeaderRematches_BeforeNewMauville[] = {
    REMATCH_ROXANNE,
    REMATCH_BRAWLY,
    // Wattson isn't available at this time
    REMATCH_FLANNERY,
    REMATCH_NORMAN,
    REMATCH_WINONA,
    REMATCH_TATE_AND_LIZA,
    REMATCH_JUAN
};

static bool32 HasBeatenRound2Rematch(u32 leader)
{
    return HasTrainerBeenFought(gRematchTable[leader].trainerIds[ROUND_2_REMATCH]);
}

// TRUE once every Gym Leader's second battle has been won
bool32 IsGymLeaderRematchChainComplete(void)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(GymLeaderRematches_AfterNewMauville); i++)
    {
        if (!HasBeatenRound2Rematch(GymLeaderRematches_AfterNewMauville[i]))
            return FALSE;
    }
    return TRUE;
}

// Offers the next Gym Leader's second battle, in gym order. Wattson only rematches after the New Mauville
// quest, so he's skipped until then and offered as soon as it's done
void UpdateGymLeaderRematchChain(void)
{
    u32 i;

    if (!FlagGet(FLAG_SYS_GAME_CLEAR))
        return;

    for (i = 0; i < ARRAY_COUNT(GymLeaderRematches_AfterNewMauville); i++)
    {
        u16 leader = GymLeaderRematches_AfterNewMauville[i];

        if (HasBeatenRound2Rematch(leader))
            continue;
        if (leader == REMATCH_WATTSON && !FlagGet(FLAG_WATTSON_REMATCH_AVAILABLE))
            continue;
        if (!gSaveBlock1Ptr->trainerRematches[leader])
            gSaveBlock1Ptr->trainerRematches[leader] = ROUND_2_REMATCH;
        return;
    }
}

// Randomly offers battles 3-5 once the round 2 chain is complete
void UpdateGymLeaderRematch(void)
{
    if (FlagGet(FLAG_SYS_GAME_CLEAR) && IsGymLeaderRematchChainComplete() && (Random() % 100) < GYM_REMATCH_CHANCE_PERCENT)
    {
        if (FlagGet(FLAG_WATTSON_REMATCH_AVAILABLE))
            UpdateGymLeaderRematchFromArray(GymLeaderRematches_AfterNewMauville, ARRAY_COUNT(GymLeaderRematches_AfterNewMauville), 5);
        else
            UpdateGymLeaderRematchFromArray(GymLeaderRematches_BeforeNewMauville, ARRAY_COUNT(GymLeaderRematches_BeforeNewMauville), 1);
    }
}

static void UpdateGymLeaderRematchFromArray(const u16 *data, size_t size, u32 maxRematch)
{
    s32 whichLeader = 0;
    s32 lowestRematchIndex = 5;
    u32 i;
    s32 rematchIndex;
    for (i = 0; i < size; i++)
    {
        if (!gSaveBlock1Ptr->trainerRematches[data[i]])
        {
            rematchIndex = GetRematchIndex(data[i]);
            if (lowestRematchIndex > rematchIndex)
                lowestRematchIndex = rematchIndex;
            whichLeader++;
        }
    }
    if (whichLeader != 0 && lowestRematchIndex <= maxRematch)
    {
        whichLeader = 0;
        for (i = 0; i < size; i++)
        {
            if (!gSaveBlock1Ptr->trainerRematches[data[i]])
            {
                rematchIndex = GetRematchIndex(data[i]);
                if (rematchIndex == lowestRematchIndex)
                    whichLeader++;
            }
        }
        if (whichLeader != 0)
        {
            whichLeader = Random() % whichLeader;
            for (i = 0; i < size; i++)
            {
                if (!gSaveBlock1Ptr->trainerRematches[data[i]])
                {
                    rematchIndex = GetRematchIndex(data[i]);
                    if (rematchIndex == lowestRematchIndex)
                    {
                        if (whichLeader == 0)
                        {
                            gSaveBlock1Ptr->trainerRematches[data[i]] = lowestRematchIndex;
                            break;
                        }
                        whichLeader--;
                    }
                }
            }
        }
    }
}

static s32 GetRematchIndex(u32 trainerIdx)
{
    s32 i;
    for (i = 0; i < 5; i++)
    {
        if (!HasTrainerBeenFought(gRematchTable[trainerIdx].trainerIds[i]))
        {
            return i;
        }
    }
    return 5;
}
