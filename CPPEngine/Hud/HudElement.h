#pragma once

#include <glide.h>
#include <string>
#include "Types3D.h"
#include "Glide/GlidePrimitives.h"

class HudElement
{
public:
	HudElement(std::string name, POINT position);
	virtual ~HudElement(void) { if(value != NULL) delete value; }

	std::string name;						// for referring to this through the HudManager
	POINT position;							// upper left most pixel
	void *value;							// polymorphic

	virtual void SetValue(void *value) = 0; // pointer to whatever value this needs
	virtual void Draw() = 0;				// HUD elements know how to draw themselves
};
