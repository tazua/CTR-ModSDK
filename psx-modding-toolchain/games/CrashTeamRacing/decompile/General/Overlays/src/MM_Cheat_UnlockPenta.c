#include <common.h>

void OtherFX_Play(int sfxID, int flag);

void MM_Cheat_UnlockPenta()
{
	// unlock penta penguin
	sdata.gameProgress.Unlocks.characters_tracks |= 0x40;
	
	// play cheat sound
	OtherFX_Play(0x67, 1);
	return;
}