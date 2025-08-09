#ifndef GUARD_SWARMS_H
#define GUARD_SWARMS_H

bool8 AreSwarmsDone(void);
// This is called upon loading a game, to count the amount of flags that have been activated
void UpdateSwarmsActivated(void);
u16 GetSwarmSpeciesFlag(u16 species);
u16 GetSpeciesFromSwarmFlag(u16 swarmFlag);
void ActivateNewSwarm();
void ActivateSwarmSpecies(u16 species);
bool8 HasSpeciesSwarmHappened(u16 species);
u16 GetNextUnshowSpecies();

#endif // GUARD_SWARMS_H
