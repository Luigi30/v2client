#pragma once

#include <cstdio>
#include "assert.h"
#include "globals.h"
#include "glide.h"
#include "Types3D.h"

class GlideTexture
{
	Gu3dfInfo gu3dfInfo;
	GrTexInfo texinfo;
	FxU32 loadAddress;
	FxBool isLoaded;

public:
	static FxU32 nextTextureLoadPoint;

	FxBool WillFitInTMEM();
	FxBool Download();
	void SetAsSource();

	GlideTexture(const char *path);
	~GlideTexture(void);
};

extern GlideTexture *font8;
extern GlideTexture *font16;

extern GlideTexture *placeholderLaser;