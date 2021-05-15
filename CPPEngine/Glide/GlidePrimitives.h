#pragma once

#include <glide.h>
#include "Glide/GlideManager.h"

class GlidePrimitives
{
public:
	static void DrawLine(int x1, int y1, int x2, int y2, GrColor_t color);
	static void DrawRect(int x, int y, int w, int h, GrColor_t color);
	static void DrawTextFromDebugTexture(const char *str, int x, int y);
};
