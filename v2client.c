#define __NEW_STARLET 1

#include <FAR_POINTERS.H>
#include <stdio.h>
#include <stdlib.h>
#include <ssdef.h>
#include <stsdef.h>
#include <descrip.h>
#include <GEN64DEF.H>
#include <lib$routines.h>
#include <STARLET.H>
#include <iodef.h>
#include <ints.h>
#include <VADEF.H>
#include <PSLDEF.H>
#include <SECDEF.H>
#include <VA_RANGEDEF.H>

#include "glide/glide2x/cvg/glide/src/glide.h"
#include "glide/glide2x/cvg/glide/src/fxglide.h"

#include "errchk.h"

// static int r0_status;
// static $DESCRIPTOR (voodoo2_dev, "FXA0:");
// static $DESCRIPTOR (voodoo2_section_name, "voodoo2_global");
// unsigned short int channel;
// static uint64 buf;

// VOID_PQ mapped_addr_base;
// static uint64 mapped_addr_length;

// static uint32 data_to_read;

#define MMIO8(addr)		(*(volatile uint8 *)(addr))
#define MMIO16(addr)	(*(volatile uint16 *)(addr))
#define MMIO32(addr)	(*(volatile uint32 *)(addr))
#define MMIO64(addr)	(*(volatile uint64 *)(addr))

// Make sure Glide is either enabled or disabled in the synchronizer settings as needed.
int main()
{
    printf("OpenVMS Voodoo2 client program 0.01\n");

    
    GrHwConfiguration hwconfig;
    grSstQueryBoards(&hwconfig);
    printf("grSstQueryBoards sees %d SSTs\n", hwconfig.num_sst);

    if(hwconfig.num_sst == 0)
    {
        printf("Couldn't find any SSTs in your system.\n");
        return SS$_ABORT;
    }

    printf("performing grGlideInit\n");
    grGlideInit();
    grSstSelect(0);

    if(!grSstWinOpen(
        0,
        GR_RESOLUTION_640x480,
        GR_REFRESH_60Hz,
        GR_COLORFORMAT_ABGR,
        GR_ORIGIN_UPPER_LEFT,
        2,
        0
    ))
    {
        printf("grSstWinOpen failed!\n");
    }

    GrColor_t clearColor;
    grBufferClear(0xFF400000, 0, GR_WDEPTHVALUE_FARTHEST);

    float color = 255.0;
    
    grColorCombine( GR_COMBINE_FUNCTION_LOCAL,
                    GR_COMBINE_FACTOR_NONE,
                    GR_COMBINE_LOCAL_ITERATED,
                    GR_COMBINE_OTHER_NONE,
                    FXFALSE );
    
    grConstantColorValue( 0xFFFFFF );

    GrVertex vtx1, vtx2, vtx3;
    vtx1.x = 320; vtx1.y = 40;
    vtx1.r = color; vtx1.g = 0; vtx1.b = 0; vtx1.a = 0;
    vtx2.x = 100; vtx2.y = 440;
    vtx2.r = 0; vtx2.g = color; vtx2.b = 0; vtx2.a = 128;
    vtx3.x = 540; vtx3.y = 440;
    vtx3.r = 0; vtx3.g = 0; vtx3.b = color; vtx3.a = 255;

    grDrawTriangle(&vtx1, &vtx2, &vtx3);

    grBufferSwap(1);

    char c = getchar();

    grSstWinClose();
}