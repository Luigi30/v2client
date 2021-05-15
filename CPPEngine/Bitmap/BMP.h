#pragma once

#include <glide.h>
#include <fstream>
#include "stdint.h"

#pragma pack(push, 1)
typedef struct leBITMAPFILEHEADER {
     uint16_t	bfType;
     uint32_t	bfSize;
     uint16_t	bfReserved1;
     uint16_t	bfReserved2;
     uint32_t	bfOffBits;
} leBITMAPFILEHEADER;

typedef struct leBITMAPINFOHEADER {
      uint32_t	biSize;
      uint32_t	biWidth;
      uint32_t	biHeight;
      uint16_t	biPlanes;
      uint16_t	biBitCount;
      uint32_t	biCompression;
      uint32_t	biSizeImage;
      uint32_t	biXPelsPerMeter;
      uint32_t	biYPelsPerMeter;
      uint32_t	biClrUsed;
      uint32_t	biClrImportant;
} leBITMAPINFOHEADER;
#pragma pack(pop)
   
class BMP
{
	leBITMAPFILEHEADER bfHeader;
	leBITMAPINFOHEADER biHeader;

	// Glide stuff
	GrLfbSrcFmt_t grFormat;
	FxI32 grStride;

	void *pixels;

public:
	void *GetPixels() { return pixels; }
	FxI32 GetStride() { return grStride; }
	FxI32 GetWidth()  { return biHeader.biWidth; }
	FxI32 GetHeight()  { return biHeader.biHeight; }
	GrLfbSrcFmt_t GetGRFormat() { return grFormat; }

	BMP(const char *path, GrLfbSrcFmt_t grFormat);
	~BMP(void);
};

extern BMP *debugFont;