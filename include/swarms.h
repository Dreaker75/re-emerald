#ifndef GUARD_SWARMS_H
#define GUARD_SWARMS_H

// #include "constants/swarms.h"

bool8 GetSwarmsDone(void);
// This is called upon loading a game, to count the amount of flags that have been activated
void UpdateSwarmsActivated(void);
u16 GetSwarmSpeciesFlag(u16 species);
void ActivateNewSwarm();
void ActivateSwarmSpecies(u16 species);
bool8 HasSpeciesSwarmHappened(u16 species);

#endif // GUARD_SWARMS_H
