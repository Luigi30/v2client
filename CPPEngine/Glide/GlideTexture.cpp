#include "GlideTexture.h"

FxU32 GlideTexture::nextTextureLoadPoint = 0;

GlideTexture::GlideTexture(const char *path)
{
	char dbg[512];
	
	sprintf(dbg, "Loading texture %s\n", path);

	printf(dbg);
	gu3dfGetInfo(path, &gu3dfInfo);
	// This provides the buffer size we need, so allocate it.
	gu3dfInfo.data = malloc(gu3dfInfo.mem_required);
	FxBool success = gu3dfLoad(path, &gu3dfInfo);
	isLoaded = false;

	if(!success)
	{
		printf("Couldn't load texture %s!\n", path);
		assert(false);
	}

	texinfo.largeLod = gu3dfInfo.header.large_lod;
	texinfo.smallLod = gu3dfInfo.header.small_lod;
	texinfo.aspectRatio = gu3dfInfo.header.aspect_ratio;
	texinfo.format = gu3dfInfo.header.format;
	texinfo.data = gu3dfInfo.data;
}

FxBool GlideTexture::WillFitInTMEM()
{
	FxU32 reqBytes = grTexCalcMemRequired(texinfo.smallLod, texinfo.largeLod, texinfo.aspectRatio, texinfo.format);
	return FXTRUE;
}

FxBool GlideTexture::Download()
{
	FxU32 reqBytes = grTexCalcMemRequired(texinfo.smallLod, texinfo.largeLod, texinfo.aspectRatio, texinfo.format);
	grTexDownloadMipMap(GR_TMU0, nextTextureLoadPoint, GR_MIPMAPLEVELMASK_BOTH, &texinfo);
	loadAddress = nextTextureLoadPoint;
	nextTextureLoadPoint += reqBytes; // TODO: Eventually we'll run out of texture memory.
	isLoaded = true;

	return FXTRUE;
}

void GlideTexture::SetAsSource()
{
	grTexSource(GR_TMU0, loadAddress, GR_MIPMAPLEVELMASK_BOTH, &texinfo);
}

GlideTexture::~GlideTexture(void)
{
}

GlideTexture *font16;
GlideTexture *font8;
GlideTexture *placeholderLaser;