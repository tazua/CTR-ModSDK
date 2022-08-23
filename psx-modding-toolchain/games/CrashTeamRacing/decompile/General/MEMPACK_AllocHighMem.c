#include <common.h>

int MEMPACK_GetFreeBytes();

void* DECOMP_MEMPACK_AllocHighMem(int allocSize)
{
	register struct sData* sdata asm("gp"); // r28

	int newLastFreeByte;

	// if out of memory
	while(MEMPACK_GetFreeBytes() < allocSize)
	{
		// enter infinite loop
	}

	// align up
	allocSize = (allocSize + 3) & 0xfffffffc;

	// dont use sizeOfPrevAlloc, cause you can't Realloc
	// on HighMem, that's only for low end allocations

	// address to return
	newLastFreeByte = (int)sdata->PtrMempack->lastFreeByte - allocSize;

	// allocator goes backwards for high end
	sdata->PtrMempack->lastFreeByte = (void *)newLastFreeByte;

	// new last free byte
	return (void *)newLastFreeByte;
}