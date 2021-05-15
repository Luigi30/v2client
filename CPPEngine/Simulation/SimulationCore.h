#pragma once

#include <vector>
#include "glide.h"
#include "Glide/GlideText.h"
#include "Glide/GlideManager.h"

#include "Math/Math3D.h"
#include "Math/Matrix.h"
#include "Math/Vector3f.h"

#include "Glide/GlideTexture.h"

#include "Model/WavefrontObj.h"
#include "Model/Model3D.h"

#include "Hud/HudManager.h"

#include "Weapons/Projectile.h"
#include "SimObject/SimulationObjectList.h"

enum SIMSTATE {
	STATE_PAUSED,
	STATE_SIMULATING,
	STATE_MAINMENU,
};

extern SimulationObject *playerSimobj;

class SimulationCore
{
	enum SIMSTATE simState;
	bool simIsInitialized;

public:

	SimulationCore();
	virtual ~SimulationCore();

	void ConsoleDebugInfo(Vector3f eye, Vector3f rot, Vector3f target);

	/* Called by WinMain */
	void FrameCallback();

	/* Called by constructor? */
	void SetupSimulation();

	/* Called once per frame. */
	void SimOneFrame();

};

extern SimulationCore g_SimulationCore;