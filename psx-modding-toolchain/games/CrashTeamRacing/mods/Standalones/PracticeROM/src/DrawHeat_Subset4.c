#include "../p_rom.h"

void howl_TogglePause(int enable);

void SaveState()
{
	int i;
	int * driver = (int *)(gp->gameTracker.drivers[0]);
	int * selfInstance = (int *)(gp->gameTracker.drivers[0]->instSelf);
	int * saveBuffer = (int *)(gp->gameTracker.drivers[p_rom->slot + 1]);
	int * instanceBuffer = p_rom->firstInstance + (instanceSize * p_rom->slot);
	int * soundBuffer = p_rom->firstSound + (soundSize * p_rom->slot);
	int * sound = (int *)(gp->channelStatsCurr);

	for (i = 0; i < driverSize; i++)
		saveBuffer[i] = driver[i];

	for (i = 0; i < instanceSize; i++)
		instanceBuffer[i] = selfInstance[i];

	howl_TogglePause(1);
	p_rom->numSounds[p_rom->slot] = gp->numBackup_ChannelStats;
	for (i = 0; i < soundSize; i++)
		soundBuffer[i] = sound[i];
	howl_TogglePause(0);

	p_rom->clock[p_rom->slot][0] = gp->gameTracker.elapsedEventTime;
	p_rom->clock[p_rom->slot][1] = gp->gameTracker.lapTime[0];
	p_rom->clock[p_rom->slot][2] = gp->gameTracker.lapTime[1];
	p_rom->clock[p_rom->slot][3] = gp->gameTracker.lapTime[2];

	p_rom->ghostBufferPointer[p_rom->slot] = gp->GhostRecording.ptrCurrOffset;
}