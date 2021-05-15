#include "Render/RenderLaser.h"

// Don't want to change GLIDE.H just to add constructors...
GrVertex PositionToGrVertex(Transformation *t)
{
	GrVertex v;

	v.x = t->position.x;
	v.y = t->position.y;
	v.z = t->position.z;
	v.ooz = 1/v.z;

	return v;
}

void RenderLaser::DrawLaserQuad(GrVertex *quadVerts)
{
	g_GlideManager.PushState();

	grChromakeyMode(GR_CHROMAKEY_ENABLE);
	grChromakeyValue(0); // black pixels are not drawn as part of the blast	

	grColorCombine( 
		GR_COMBINE_FUNCTION_SCALE_OTHER,
		GR_COMBINE_FACTOR_ONE,
		GR_COMBINE_LOCAL_NONE,
		GR_COMBINE_OTHER_TEXTURE,
		FXFALSE
	);

	placeholderLaser->SetAsSource();

	int ilist[4] = { 1, 0, 2, 3 };
	grCullMode(GR_CULL_DISABLE);
	grDrawPlanarPolygon(4, ilist, quadVerts);

	g_GlideManager.PopState();
}

void RenderLaser::Render(Transformation *t, float length, FxColor_t color)
{
	Vector3f forward = t->ForwardVec();
	Vector3f right = t->RightVec() * 1;

	Vector3f tl, tr, bl, br;
	tl = tr = bl = br = t->position;

	bl = bl - (forward * length) - right;
	br = br - (forward * length) + right;
	tl = tl - right;
	tr = tr + right;

	GrVertex grvTL = tl.ToGrVertex(),
			 grvTR = tr.ToGrVertex(),
			 grvBL = bl.ToGrVertex(), 
			 grvBR = br.ToGrVertex();
	
	// Apply the world and projection matrixes to the quad.
	Matrix44 mTransform;
	mTransform.Identity();
	mTransform = mTransform.Multiply(&g_Renderer.mtxWorld);
	mTransform = mTransform.Multiply(&g_Renderer.mtxProjection);

	GrVertex transformed[4];

	mTransform.TransformPoint(&grvTL, &transformed[0]);
	mTransform.TransformPoint(&grvTR, &transformed[1]);
	mTransform.TransformPoint(&grvBL, &transformed[2]);
	mTransform.TransformPoint(&grvBR, &transformed[3]);
	g_Renderer.ProjectPolygonToCameraSpace(transformed, 4);

	for(int i=0; i<2; i++)
	{
		transformed[0].tmuvtx[i].sow = 0;
		transformed[0].tmuvtx[i].tow = 64;
		transformed[0].tmuvtx[i].oow = 1;
		transformed[0].oow = 1;

		transformed[1].tmuvtx[i].sow = 0;
		transformed[1].tmuvtx[i].tow = 192;
		transformed[1].tmuvtx[i].oow = 1;
		transformed[1].oow = 1;

		transformed[2].tmuvtx[i].sow = 256;
		transformed[2].tmuvtx[i].tow = 64;
		transformed[2].tmuvtx[i].oow = 1;
		transformed[2].oow = 1;

		transformed[3].tmuvtx[i].sow = 256;
		transformed[3].tmuvtx[i].tow = 192;
		transformed[3].tmuvtx[i].oow = 1;
		transformed[3].oow = 1;
	}

	DrawLaserQuad(transformed);
}