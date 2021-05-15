#pragma once

#include "Hud/HudElement.h"
#include "Bitmap/BMFont.h"

typedef struct {
	std::string text;
} HudTextValue;

class HudText : public HudElement
{
public:
	HudTextValue *value;
	BMFont *font;

	HudText(std::string name, POINT position, BMFont *font) : HudElement(name, position) { this->font = font; };
	~HudText(void) {};

	void SetValue(void *value);
	void Draw();
};
