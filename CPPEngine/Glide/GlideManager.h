// GlideManager.h: interface for the GlideManager class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <glide.h>
#include <assert.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define STATE_STACK_SIZE 16

// #ifndef _MSC_VER
// typedef struct _GrState_s {
//   char pad[GLIDE_STATE_PAD_SIZE];
// } GrState;
// #endif

class GlideManager  
{
	GrHwConfiguration hwconfig;

	GrState state_stack[STATE_STACK_SIZE];
	int state_stack_position;
	
	bool isRunning;

public:
	GlideManager();
	virtual ~GlideManager();

	bool StartGlide();
	void StopGlide();

	void PushState();
	void PopState();
	
	void PrintSSTStatus();

	bool GlideIsRunning() { return isRunning; }
};

extern GlideManager g_GlideManager;
