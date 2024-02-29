#include <common.h>

void DECOMP_GhostReplay_ThTick(struct Thread *);

void DECOMP_GhostReplay_Init1(void)
{
	char i;
	u_short uVar1;
	int iVar2;
	u_int uVar3;
	struct Thread *t;
	struct Instance *inst;
	struct Instance *wakeInst;
	struct Driver *ghostDriver;
	struct Model* wake;
	int timeTrialFlags;
	
	struct GhostHeader* gh;
	struct GhostTape* tape;
	int charID;
	char* recordBuffer;
	
	struct GameTracker *gGT = sdata->gGT;
	
	// This has to run from MainInit_Drivers
	sdata->boolCanSaveGhost = 0;
	sdata->boolGhostsDrawing = 0;

	// only continue if you're in time trial, not main menu, and not cutscene
	if ((gGT->gameMode1 & 0x20022000) != 0x20000) return;
	
	// === Record Buffer ===
	
	// In the future, this can move to GhostTape_Start, when byte budget allows
	
	gh = DECOMP_MEMPACK_AllocMem(0x3e00/*, "ghost record buffer"*/);
	recordBuffer = GHOSTHEADER_GETRECORDBUFFER(gh);
	sdata->GhostRecording.ptrGhost = gh;
	sdata->GhostRecording.ptrStartOffset = &recordBuffer[0];
	sdata->GhostRecording.ptrEndOffset = &recordBuffer[0x3DD4];

	// === Replay Buffer ===
	// 0: human ghost
	// 1: N Tropy / Oxide ghost

	for (i = 0; i < 2; i++)
	{
		tape = DECOMP_MEMPACK_AllocMem(0x268/*, "ghost tape"*/);
		sdata->ptrGhostTape[i] = tape;

		// first ghost pointer is a ghost loaded by player
		if (i == 0)
		{
			// if not playing a human ghost, skip this ghost
			if (sdata->boolReplayHumanGhost == 0) continue;
			
			// assign the ghost you loaded
			gh = sdata->ptrGhostTapePlaying;
			
			charID = 1;
		}

		// second ghost pointer is n tropy or oxide
		else
		{
			timeTrialFlags = sdata->gameProgress.highScoreTracks[gGT->levelID].timeTrialFlags;
			
			#ifdef REBUILD_PC
			// 1 for n tropy, 3 for oxide
			timeTrialFlags = 3;
			#endif
			
			// if not opened N Tropy, skip this ghost
			if ((timeTrialFlags & 1) == 0) continue;
			
			void** pointers = ST1_GETPOINTERS(gGT->level1->ptrSpawnType1);
			
			// If you have not beaten N Tropy
			if ((timeTrialFlags & 2) == 0)
			{
				gh = pointers[ST1_NTROPY];	
				charID = 2;
			}

			// If you have beaten N Tropy
			else
			{
				gh = pointers[ST1_NOXIDE];	
				charID = 3;
			}
		}
		
		sdata->boolGhostsDrawing = 1;
		recordBuffer = GHOSTHEADER_GETRECORDBUFFER(gh);

		tape->gh = gh;
		tape->gh_again = gh;
		tape->constDEADC0ED = 0xDEADC0ED;
		tape->ptrStart = &recordBuffer[0];
		tape->ptrEnd = &recordBuffer[gh->size];
		tape->ptrCurr = tape->ptrStart;
		tape->timeElapsedInRace = 0;
		tape->timeInPacket32_backup = 0;
		tape->unk20 = 0;
		tape->timeInPacket01 = 0;
		tape->timeInPacket32 = 0;

		// if n tropy / oxide
		if (i == 1)
		{
			gGT->timeToBeatInTimeTrial_ForCurrentEvent = gh->timeElapsedInRace;
		}

		// characterID and model
		charID = data.characterIDs[charID];
		
// set in MainInit_Drivers for PC port
#ifndef REBUILD_PS1
		uVar3 = VehInit_GetModelByName(data.MetaDataCharacters[charID].name_Debug);
#else
		uVar3 = 0;
#endif

		inst = DECOMP_INSTANCE_Birth3D(uVar3, 0, 0);
		inst->unk51 = 0xc;
		inst->flags = 7;

		t = DECOMP_THREAD_BirthWithObject(
			
			// creation flags
			SIZE_RELATIVE_POOL_BUCKET(
				4, NONE, LARGE, GHOST
			), 
			
			DECOMP_GhostReplay_ThTick, 
			0, 
			0
		);
		
		t->modelIndex = 0x4b;	// ghost
		t->flags |= 0x1000;		// ignore collisions
		
		t->inst = inst;
		inst->thread = t;
		
		// ghost drivers are 0x638 bytes large
		ghostDriver = t->object;
		memset(ghostDriver, 0, 0x638);
		ghostDriver->ghostID = i;
		ghostDriver->driverID = i + 1;
		ghostDriver->ghostBoolInit = 1;
		ghostDriver->ghostBoolStarted = 0;
		ghostDriver->ghostTape = tape;
		ghostDriver->instSelf = inst;
		ghostDriver->actionsFlagSet |= 0x100000; // AI driver

		// Ptr Model "Wake"
		wake = gGT->modelPtr[0x43];

		// if "Wake" model exists
		if (wake)
		{
			wakeInst = DECOMP_INSTANCE_Birth3D(wake, 0, 0);
			ghostDriver->wakeInst = wakeInst;

			if (wakeInst != 0)
			{
				// make invisible, set to anim 1
				wakeInst->flags |= 0x90;
			}
		}

#ifndef REBUILD_PS1
		VehInit_TireSprites(t);
		VehInit_SetConsts(ghostDriver);
#endif
		
		if(charID == 0xF)
			ghostDriver->wheelSize = 0;

		// pointer to TrTire, for transparent tires
		ghostDriver->wheelSprites = ICONGROUP_GETICONS(gGT->iconGroup[0xc]);
		
		// advance ghost by one frame,
		// just so Oxide doesn't block your view
		DECOMP_GhostReplay_ThTick(t);
	}
		
	return;
}
