#include "BMP.h"

BMP::BMP(const char *path, GrLfbSrcFmt_t grFormat)
{
	fstream fileStream;
	fileStream.open(path, ios::in|ios::binary);

	fileStream.read((char *)&bfHeader, sizeof(leBITMAPFILEHEADER));
	fileStream.read((char *)&biHeader, sizeof(leBITMAPINFOHEADER));

	pixels = malloc(bfHeader.bfSize);
	fileStream.seekg(this->bfHeader.bfOffBits);
	fileStream.read((char *)pixels, bfHeader.bfSize);

	this->grFormat = grFormat;

	switch(grFormat)
	{
	case GR_LFB_SRC_FMT_565:
		grStride = biHeader.biWidth * 2;
		break;
	default:
		printf("Unknown pixel format specified, don't know the stride of %s!\n", path);
		grStride = biHeader.biWidth;
	}
}

BMP::~BMP(void)
{
	if(pixels != NULL) delete pixels;
}

BMP *debugFont;