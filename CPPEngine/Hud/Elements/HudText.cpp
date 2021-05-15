#include "HudText.h"
#include "Glide/GlideTexture.h"

#include <stdio.h>

void HudText::SetValue(void *value)
{
	this->value = (HudTextValue *)value;
}

void HudText::Draw()
{
	//GlidePrimitives::DrawTextFromDebugTexture(value->text.c_str(), position.x, position.y);

	font->PutString(value->text.c_str(), position.x, position.y);
}