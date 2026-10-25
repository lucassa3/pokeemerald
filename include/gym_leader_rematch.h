#ifndef GUARD_TRAINER_REMATCH_H
#define GUARD_TRAINER_REMATCH_H

#include "constants/rematches.h"

// After the round 2 chain, a random Gym Leader rematch is rolled every this many trainer/wild battles
#define GYM_REMATCH_TRAINER_BATTLE_INTERVAL 3   // Vanilla: 20
#define GYM_REMATCH_WILD_BATTLE_INTERVAL    10  // Vanilla: 60
#define GYM_REMATCH_CHANCE_PERCENT          50  // Vanilla: 31

void UpdateGymLeaderRematch(void);
void UpdateGymLeaderRematchChain(void);
bool32 IsGymLeaderRematchChainComplete(void);

#endif //GUARD_TRAINER_REMATCH_H
