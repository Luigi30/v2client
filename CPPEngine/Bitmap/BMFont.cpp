#include "BMFont.h"

BMFont::BMFont(const char *path, const char *bmp_path)
{
	char blockType;
	uint32 blockSize;
	char *blockData;

	charsBlocks = NULL;
	kerningBlocks = NULL;
	numKerningBlocks = 0;
	numCharBlocks = 0;

	// Export fonts from Photoshop in 16-bit RGB565 mode!
	bitmap = new BMP(bmp_path, GR_LFB_SRC_FMT_565);

	fstream fileStream;
	fileStream.open(path, ios::in|ios::binary);

	// Skip header. Read to first block.
	fileStream.seekg(4);
	bool moreBlocks = true;

	while(moreBlocks)
	{
		fileStream.read((char *)&blockType, 1);
		fileStream.read((char *)&blockSize, 4);

		blockData = (char *)malloc(blockSize);
		fileStream.read(blockData, blockSize);

		if(!fileStream) break;

		switch(blockType)
		{
			case 1: 
				memcpy(&infoBlock, blockData, blockSize);
				break;
			case 2:
				memcpy(&commonBlock, blockData, blockSize);
				break;
			case 3:
				break;
			case 4:
				LoadCharsBlock((BMFontCharsBlock *)blockData, blockSize);
				break;
			case 5:
				LoadKerningBlock((BMFontKerningBlock *)blockData, blockSize);
				break;
			default:
				moreBlocks = false;
				break;
		}

		if(blockData != NULL) delete blockData;
	}

	if(blockData != NULL) delete blockData;
}

BMFont::~BMFont(void)
{
}

void BMFont::LoadCharsBlock(BMFontCharsBlock *data, FxU32 length)
{
	int numChars = length / sizeof(BMFontCharsBlock);
	charsBlocks = new BMFontCharsBlock[256];

	for(int i=0; i<numChars; i++)
	{
		memcpy(&charsBlocks[data[i].id], &data[i], sizeof(BMFontCharsBlock));
	}

	numCharBlocks = numChars;
}

void BMFont::LoadKerningBlock(BMFontKerningBlock *data, FxU32 length)
{
	int numKernings = length / sizeof(BMFontKerningBlock);
	kerningBlocks = new BMFontKerningBlock[numKernings];

	for(int i=0; i<numKernings; i++)
	{
		memcpy(&kerningBlocks[i], &data[i], sizeof(BMFontKerningBlock));
	}
	
	numKerningBlocks = numKernings;
}

void BMFont::PutString(const char *str, int dest_x, int dest_y)
{
	for(int i=0; i<strlen(str); i++)
	{
		dest_x += BlitChar(str[i], dest_x, dest_y);
	}
}

FxU16 BMFont::BlitChar(char c, int dest_x, int dest_y)
{
	BMFontCharsBlock *c_block = GetCharBlock(c);
	grLfbWriteRegion(GR_BUFFER_BACKBUFFER, dest_x, dest_y + c_block->yoffset, 
		bitmap->GetGRFormat(), 
		c_block->width, c_block->height,
		bitmap->GetStride(),
		&((FxU16 *)bitmap->GetPixels())[(bitmap->GetWidth() * c_block->y) + c_block->x]);

	return c_block->xadvance; // Returns the advance value so we can use this directly in PutString.
}

BMFont *bmFontMonogram32;
BMFont *bmFontMonogram16;