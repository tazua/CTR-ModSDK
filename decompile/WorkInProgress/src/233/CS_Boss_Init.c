#include <common.h>

void CS_Boss_Init(int* cutscene)
{
    char i;
    unsigned int *ptrDestination;
    int *piVar4;
    int index;

    i = 1;

    index = 3 - sdata->gGT->activeMempackIndex;

    CDSYS_XAPauseRequest();

    ptrDestination = 0x800b777c;

    for (i = 1; -1 < i; i--)
        ptrDestination[i - 1] = 0;

    sdata->gGT.levID_in_each_mempack[index] = -1;

    MEMPACK_SwapPacks(index);

    MEMPACK_ClearLowMem();

    sdata->load_inProgress = 1;

    if (cutscene)
    {
        // add to load queue, '3' means vram
        LOAD_AppendQueue(sdata->ptrBigfileCdPos_2, 3, cutscene[0] - 1 + index, 0, 0);
    }

    piVar4 = cutscene + 2;
    for (i = 1; 0 < i; i--)
    {
        if (piVar4[i - 1])
        {
            // add to load queue, '2' means dram
            LOAD_AppendQueue(sdata->ptrBigfileCdPos_2, 2, piVar4[i - 1] - 1 + index, ptrDestination, 0xfffffffe);
        }
        ptrDestination -= 1;
    }
    // add to load queue, '2' means dram
    LOAD_AppendQueue(sdata->ptrBigfileCdPos_2, 2, cutscene[1] - 1 + index, 0, 0x800ae81c);
}