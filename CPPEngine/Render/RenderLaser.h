#pragma once

#include <glide.h>
#include "Math/Math3D.h"
#include "Render/Renderer.h"
#include "Glide/GlideManager.h"
#include "Glide/GlideTexture.h"

class RenderLaser
{	
	static void DrawLaserQuad(GrVertex *quadVerts);

public:
	static void Render(Transformation *t, float length, FxColor_t color);
};
