#include "vhgui2015.h"
#include "lib_lcd.h"

#define SPACEW 10

static void GUI2015_BITDRAW_CHARLINE(stGUI2015DrawChar *pStruct, stFontSymProp * pSymProp, u8 line, u8 trans); // Bit-Font
static void GUI2015_BYTEDRAW_CHARLINE(stGUI2015DrawChar *pStruct, stFontSymProp * pSymProp, u8 line, u8 trans); // Byte-Font

u8 GUI2015_DRAW_CHAR(stGUI2015DrawChar *pStruct) {
    
	stFontSymProp sSymProp;
	u8 ucFullHeight;
	u8 ucFromLine;
	u8 j;
	u8 scale_shift = (pStruct->pOutStyle->scale-1);

    FONT_GETSYMPROP(pStruct->ucSYM, pStruct->pOutStyle->ucFont, &sSymProp);
    ucFullHeight = (sSymProp.ucTOP + sSymProp.ucSYMHEIGHT) << scale_shift;
	ucFromLine = pStruct->pOutStyle->transparent ? sSymProp.ucTOP << scale_shift : 0;

	if(pStruct->pOutStyle->ucFont<3) {
	for (j = ucFromLine; j < ucFullHeight; j++) { GUI2015_BITDRAW_CHARLINE(pStruct, &sSymProp, j, pStruct->pOutStyle->transparent ); }
	} else {
	for (j = ucFromLine; j < ucFullHeight; j++) { GUI2015_BYTEDRAW_CHARLINE(pStruct, &sSymProp, j, pStruct->pOutStyle->transparent ); }
	}

	return ((pStruct->ucSYM == ' ') ? SPACEW : sSymProp.ucSYMWIDTH) * pStruct->pOutStyle->scale;
}

static int gfxbuff[64];
static int gfxbuffi = 0;
static int gfxx,gfxy;

/** Draw syngle line of selected symbol according to style props */
static void GUI2015_BITDRAW_CHARLINE(stGUI2015DrawChar *pStruct, stFontSymProp * pSymProp, u8 line, u8 trans ) {
	u8 scale_shift = (pStruct->pOutStyle->scale - 1);
	u8 activeLine = pSymProp->ucTOP << scale_shift;
	u8 width = pSymProp->ucSYMWIDTH << scale_shift;
	u8 cnt = 0, trf = 0;
	u8 i,z;
    u8 ucMASK = 0x80;
    u8 ucOFFSETBYTE = 0;
    u8 ucVOFFS = ((line>>scale_shift) - pSymProp->ucTOP ) * pSymProp->ucBYTESINLINE; // Vertical offset

	// Empty Line ?
	if( line < activeLine ) {
		if(!trans) { for(i=0;i<width;i++)  { gfxbuff[i] = pStruct->pOutStyle->uiCOLB; } LCD_Stream32( pStruct->pPoint->x, pStruct->pPoint->y+line,width, gfxbuff); }
		return;
	} 

	// Solid Mode Start Line
	if(!trans) { gfxbuffi = 0; gfxx = pStruct->pPoint->x; gfxy = pStruct->pPoint->y + line; }

	// Line with content
    for(i=0;i< pSymProp->ucSYMWIDTH;i++) {
		
		u8 ucBYTE = FONT_GET_SYM_B(  pSymProp->uiOFFSET +   ucVOFFS +  ucOFFSETBYTE, pStruct->pOutStyle->ucFont );

		if(trans) { // TRANS Mode 

				if((ucBYTE & ucMASK) == ucMASK) {
					// First non-transparent pixel ?
					if (!cnt) {  gfxbuffi = 0; gfxx = pStruct->pPoint->x + (i << scale_shift); gfxy = pStruct->pPoint->y + line; }
					cnt += 1 << scale_shift;
					// Last pixel ? Some pixels to put ? Print !
					if (i == (pSymProp->ucSYMWIDTH - 1)) { if (cnt) { trf = 1; }  }
				} else { if (cnt) { trf = 1; } }

				// Transfer need ?
				if (trf == 1) {  
					for (z = 0; z < cnt; z++) { gfxbuff[gfxbuffi++] =  pStruct->pOutStyle->uiCOLF;}
					LCD_Stream32(gfxx, gfxy, gfxbuffi,gfxbuff);
					trf = 0; cnt = 0;
				}

		} else { // SOLID Mode

			u16 color = (ucBYTE & ucMASK) == ucMASK ? pStruct->pOutStyle->uiCOLF : pStruct->pOutStyle->uiCOLB;
			gfxbuff[gfxbuffi++] = color;
			if(scale_shift) { gfxbuff[gfxbuffi++] = color; }
		}

		// Next fontbit
        if( ucMASK == 0x01 ) { ucMASK = 0x80; ucOFFSETBYTE++; } else ucMASK >>= 1;
	}
    
	// Solid Mode Send Line
	if(!trans) { LCD_Stream32(gfxx, gfxy, gfxbuffi,gfxbuff); }

}

static void GUI2015_BYTEDRAW_CHARLINE(stGUI2015DrawChar *pStruct, stFontSymProp * pSymProp, u8 line, u8 trans ) {

	int i;
	u8 act_line;
	u32 offset;

	// Empty line ?
	if( line < pSymProp->ucTOP ) { 
		if(!trans) {  gfxbuffi = 0; gfxx = pStruct->pPoint->x; gfxy = pStruct->pPoint->y+line;
			for(i=0;i<pSymProp->ucSYMWIDTH;i++) gfxbuff[i] = pStruct->pOutStyle->uiCOLB;
			LCD_Stream32(gfxx, gfxy, gfxbuffi,gfxbuff);
		}
		return;
	} 

	act_line = line - pSymProp->ucTOP;

	gfxbuffi = 0; gfxx = pStruct->pPoint->x; gfxy = pStruct->pPoint->y+line;

	// offset = sizeof(stBFN2HDR);
	// offset += sizeof(stBFN2CHR)*256;
	offset = pSymProp->uiOFFSET;
	offset += act_line * pSymProp->ucSYMWIDTH;

	for(i=0;i<pSymProp->ucSYMWIDTH;i++) {
		u8 a = FONT_GET_SYM_B(offset + i, pStruct->pOutStyle->ucFont);
		int color = GUI2015_Color_Delta(pStruct->pOutStyle->uiCOLF, pStruct->pOutStyle->uiCOLB, a );
		gfxbuff[gfxbuffi++] = color;
	}

	LCD_Stream32(gfxx, gfxy, gfxbuffi,gfxbuff);
}
