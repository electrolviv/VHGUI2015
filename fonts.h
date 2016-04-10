#ifndef _GUI2015_FONTS_H_
#define _GUI2015_FONTS_H_

#include "vhgui2015.h"

typedef enum {
	FONT_1=0,
	FONT_2,
	FONT_3,
	FONT_Verdana,
	FONT_Arial,
	FONT_Courier
} eFontFamily;

#define SCALE_1 1
#define SCALE_2 2
#define SCALE_3 3

typedef struct { u32 uiOFFSET; u8 ucSYMWIDTH; u8 ucSYMHEIGHT;  u8 ucTOP; u8 ucBYTESINLINE; } stFontSymProp;
typedef struct { char szName[30]; char szZ; char attrs; } stBFN2HDR;
typedef struct { u32 offs; u8 t; u8 w; u8 h; u8 res; } stBFN2CHR;

void FONT_GETSYMPROP(u8 sym, u8 font, stFontSymProp * pResult);
u8 FONT_GET_SYM_B( u32 uiOFFSET, u8 ucFONT );
u8 FONT_GET_SYM_W( u8 ucSYM, u8 ucFONT );  
u8 FONT_GET_SYM_H( u8 ucSYM, u8 ucFONT );
u8 FONT_GET_SYM_T( u8 ucSYM, u8 ucFONT );

#endif
