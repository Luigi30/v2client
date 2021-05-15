#pragma once

#include "Hud/HudElement.h"
#include "Simulation/SimulationCore.h"
#include "Glide/GlidePrimitives.h"
#include <assert.h>
#include <vector>

class HudManager
{
	std::vector<HudElement *> hudElements;

public:
	HudManager(void);
	~HudManager(void);

	bool AddElement(HudElement *);
	bool RemoveElement(HudElement *);
	HudElement * FindElementByName(std::string name);

	void CreateHud();
	void UpdateHud();
	void DrawHud();
};

extern HudManager g_HudManager;