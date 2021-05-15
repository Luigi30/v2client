#pragma once

#include <glide.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "Bitmap/BMP.h"

// The binary font format of BMFonts.
// https://www.angelcode.com/products/bmfont/doc/file_format.html

#pragma pack(push, 1)
typedef struct
{
	int16_t	fontSize;
	uint8_t	bitField;
	uint8_t	charSet;
	uint16_t	stretchH;
	uint8_t	aa;
	uint8_t	paddingUp;
	uint8_t	paddingDown;
	uint8_t	paddingLeft;
	uint8_t	spacingHoriz;
	uint8_t	spacingVert;
	uint8_t	outline;
	char	fontName[64];
} BMFontInfoBlock;

typedef struct
{
	uint16_t	lineHeight;
	uint16_t	base;
	uint16_t	scaleW;
	uint16_t	scaleH;
	uint16_t	pages;
	uint8_t		bitField;
	uint8_t		alphaChnl;
	uint8_t		redChnl;
	uint8_t		greenChnl;
	uint8_t		blueChnl;
} BMFontCommonBlock;

typedef struct
{
	uint32_t	id;
	uint16_t	x;
	uint16_t	y;
	uint16_t	width;
	uint16_t	height;
	int16_t		xoffset;
	int16_t		yoffset;
	int16_t		xadvance;
	uint8_t		page;
	uint8_t		chnl;
} BMFontCharsBlock;

typedef struct
{
	uint32_t	first;
	uint32_t	second;
	int16_t	amount;
} BMFontKerningBlock;
#pragma pack(pop)

class BMFont
{
private:
	void LoadCharsBlock(BMFontCharsBlock *data, FxU32 length);
	void LoadKerningBlock(BMFontKerningBlock *data, FxU32 length);

public:
	BMP *bitmap;

	BMFontInfoBlock infoBlock;

	BMFontCommonBlock commonBlock;

	BMFontCharsBlock *charsBlocks;
	int numCharBlocks;

	BMFontKerningBlock *kerningBlocks;
	int numKerningBlocks;

	BMFont(const char *path, const char *bmp_path);
	~BMFont(void);

	BMFontCharsBlock *GetCharBlock(char c) { return &charsBlocks[c]; }

	void PutString(const char *str, int dest_x, int dest_y);
	FxU16 BlitChar(char c, int dest_x, int dest_y);
};

extern BMFont *bmFontMonogram32;
extern BMFont *bmFontMonogram16;