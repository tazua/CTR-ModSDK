struct NavFrame
{
	// 0
	short pos[3];

	// 6
	char rot[4];

	// 0xA
	// 0xC
	
	// 0xE - flags
	// bits 0,1,2 ???
	// bits 3+: terrain
	
	// 0x10 -
	
	// 0x12 - count of some kind
	
	// search "+ 0x13" in BOTS.c
	// 0x13 - flags (again)
	// & 0x10 ???
	// & 0x20 (is reflective)
	// & 0x30 (not transparent)
	// & 0xF (transparency bits)
	
	// 0x14 - size of struct
	char data[0xA];
};

struct NavHeader
{
	// 0
	short magicNumber;
	short numPoints;

	// 4
	int unk_off4;

	// 8
	struct NavFrame* last;

	// 0xC
	char data[0x40];

	// 0x4C
	struct NavFrame frame[1];
};