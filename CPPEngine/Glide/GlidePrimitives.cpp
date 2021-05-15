#include "Glide/GlidePrimitives.h"
#include "Glide/GlideTexture.h"

void GlidePrimitives::DrawLine(int x1, int y1, int x2, int y2, GrColor_t color)
{
	g_GlideManager.PushState();

	grConstantColorValue(color);
	grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
		GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE);

	GrVertex v1, v2;
	v1.x = (float)x1;
	v1.y = (float)y1;
	v2.x = (float)x2;
	v2.y = (float)y2;
	grDrawLine(&v1, &v2);

	g_GlideManager.PopState();
}

void GlidePrimitives::DrawRect(int x, int y, int w, int h, GrColor_t color)
{
	g_GlideManager.PushState();

	grConstantColorValue(color);
	grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
		GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE);

	GrVertex r[4];
	int ilist[4] = { 0,1,2,3 };

	r[0].x = (float)x;
	r[0].y = (float)y;

	r[1].x = (float)x+w;
	r[1].y = (float)y;

	r[2].x = (float)x+w;
	r[2].y = (float)y+h;

	r[3].x = (float)x;
	r[3].y = (float)y+h;

	grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);
	grCullMode(GR_CULL_DISABLE);
	grDrawPlanarPolygon(4, ilist, r);

	g_GlideManager.PopState();
}

void GlidePrimitives::DrawTextFromDebugTexture(const char *str, int x, int y)
{
	g_GlideManager.PushState();

	// The font is https://opengameart.org/content/8x8-ascii-bitmap-font-with-c-source
	// TODO: I8 instead of RGB565 so I can use different colors and save lots of TMEM.

	for(unsigned int i=0; i<strlen(str); i++)
	{
		char c = str[i];
		// 256x128 texture, so 16 characters in a row.
		unsigned int s, t;

		s = (c & 15) * 16;
		t = (c / 16) * 16;

		GrVertex r[4];
		int ilist[4] = { 0,1,2,3 };

		int w = 16;
		int h = 16;

		r[0].x = (float)x + (16*i);
		r[0].y = (float)y;
		r[0].oow = 1;
		r[0].tmuvtx[0].oow = 1;
		r[0].tmuvtx[0].sow = s;
		r[0].tmuvtx[0].tow = t+h;

		r[1].x = (float)x+w + (16*i);
		r[1].y = (float)y;
		r[1].oow = 1;
		r[1].tmuvtx[0].oow = 1;
		r[1].tmuvtx[0].sow = s+w;
		r[1].tmuvtx[0].tow = t+h;

		r[2].x = (float)x+w + (16*i);
		r[2].y = (float)y+h;
		r[2].oow = 1;
		r[2].tmuvtx[0].oow = 1;
		r[2].tmuvtx[0].sow = s+w;
		r[2].tmuvtx[0].tow = t;

		r[3].x = (float)x + (16*i);
		r[3].y = (float)y+h;
		r[3].oow = 1;
		r[3].tmuvtx[0].oow = 1;
		r[3].tmuvtx[0].sow = s;
		r[3].tmuvtx[0].tow = t;

		font16->SetAsSource();
		grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);
		grCullMode(GR_CULL_DISABLE);
		grDrawPlanarPolygon(4, ilist, r);
	}

	g_GlideManager.PopState();
}