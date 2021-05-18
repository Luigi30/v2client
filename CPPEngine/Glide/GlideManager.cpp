// GlideManager.cpp: implementation of the GlideManager class.
//
//////////////////////////////////////////////////////////////////////

#include "GlideManager.h"
#include "GlideTexture.h"

//
GlideManager g_GlideManager;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern "C"
{
	extern void InitMacEnvironment(void);
}

GlideManager::GlideManager()
{
	isRunning = false;

#ifdef __APPLE__
	InitMacEnvironment();
#endif
	grGlideInit();
	grSstQueryHardware(&hwconfig);

	state_stack_position = 0;
	GlideTexture::nextTextureLoadPoint = grTexMinAddress(GR_TMU0);
}

void GlideManager::PrintSSTStatus()
{
	FxU32 sstStatus = grSstStatus();
	
	printf("*** SST Status ***\n");
	printf("PCI FIFO Free Space : %02X\n", sstStatus & 0x3F);
	printf("Vertical Retrace    : %d\n", (sstStatus >> 6) & 1);
	printf("PixelFX Busy        : %d\n", (sstStatus >> 7) & 1);
	printf("TMU Busy            : %d\n", (sstStatus >> 8) & 1);
	printf("Voodoo busy         : %d\n", (sstStatus >> 9) & 1);
	printf("Visible buffer      : %d\n", (sstStatus >> 10) & 3);
	printf("Memory FIFO space   : %04X\n", (sstStatus >> 12) & 0xFFFF);
	printf("Buffer swaps pending: %d\n", (sstStatus >> 28) & 7);
}

GlideManager::~GlideManager()
{

}

bool GlideManager::StartGlide()
{
	grSstSelect(0);
	printf("Selected Voodoo 0\n");
	assert(grSstWinOpen(NULL, GR_RESOLUTION_800x600, GR_REFRESH_60Hz,
						GR_COLORFORMAT_ARGB, GR_ORIGIN_LOWER_LEFT, 2, 1));
	
	isRunning = true;
	
	return true;
}

void GlideManager::StopGlide()
{
	grGlideShutdown();
}


void GlideManager::PushState()
{
	//printf("Pushing state onto stack, position now %d\n", state_stack_position + 1);
	assert(state_stack_position < STATE_STACK_SIZE);
	grGlideGetState(&state_stack[state_stack_position]);
	state_stack_position++;
}

void GlideManager::PopState()
{
	//printf("Popping state from stack, position now %d\n", state_stack_position - 1);

	assert(state_stack_position > 0);
	state_stack_position--;
	grGlideSetState(&state_stack[state_stack_position]);
}