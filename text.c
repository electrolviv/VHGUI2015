#include "vhgui2015.h"
#include "drawtext.h"
#include "bridge.h"

// const u16 fontsHeight[3];
#define LETTERSW 2

void GUI2015_STYLE_Copy(stGUI2015OutStyle *dst, stGUI2015OutStyle *src) {
	memcpy(dst,src,sizeof(stGUI2015OutStyle));
}

u16 GUI2015_TextWidth(const char *pText, u8 font ) {
	u16 r = 0; int i=0;
	while (*(pText + i) != 0) {
		r += FONT_GET_SYM_W(*(pText + i++), font);
		if (*(pText + i) == ' ') { r += FONT_GET_SYM_W('A',font)/4; }
	}
	return r;
}

u16 GUI2015_TextHeight(const char *pText, u8 font) {
	return FONT_GET_SYM_H('A',font) + FONT_GET_SYM_T('A',font);
}

void GUI2015_GetTextRect(const char *pText, const stGUI2015OutStyle *pFontStyle, stGUI2015Rect *pResult) {
	pResult->sPoint1.x = 0;
	pResult->sPoint1.y = 0;
	pResult->sPoint2.x = GUI2015_TextWidth(pText, pFontStyle->ucFont)*pFontStyle->scale + (strlen(pText)*pFontStyle->charSpacing);
	pResult->sPoint2.y = GUI2015_TextHeight(pText, pFontStyle->ucFont)*pFontStyle->scale;
}

u16 GUI2015_DRAW_TEXTINRECT(const char *pText, const stGUI2015Rect *pRect, const stGUI2015OutStyle *pFontStyle, enGUI2015Align align ) {

	stGUI2015Rect sTextRect, sFinalRect;
	stGUI2015DrawChar sDrawChar;
	u8 ucTEXTLEN;
	u16 uiCurrentX;
	u8 i;
	u8 ucW;
	
	GUI2015_GetTextRect( pText, pFontStyle, &sTextRect);
    GUI2015_RectCentering(align, pRect, &sTextRect, &sFinalRect);
	
	sDrawChar.pOutStyle = (stGUI2015OutStyle *)pFontStyle;

	ucTEXTLEN = strlen(pText);
	uiCurrentX = sFinalRect.sPoint1.x;

	for(i=0;i<ucTEXTLEN;i++ ) {
		stGUI2015Point p;
		p.x = uiCurrentX;
		p.y =  sFinalRect.sPoint1.y-2;
		sDrawChar.ucSYM = pText[i];
		sDrawChar.pPoint = &p;
		ucW = GUI2015_DRAW_CHAR( &sDrawChar );
		uiCurrentX += ucW + pFontStyle->charSpacing;
	}

    return uiCurrentX;
}

/*
u16 GUI2015_DRAW_TEXT( stGUI2015DrawText *pStruct ) {
	char *pText = GUI2015_BRIDGE_FormatString(pStruct->psText);
	return GUI2015_DRAW_TEXTINRECT(pText,pStruct->pRect,pStruct->pOutStyle,pStruct->align);
}
*/

// memcpy( , & pStruct->sOutStyle, sizeof(stGUI2015OutStyle) );
// return GUI2015_DRAW_TEXTINRECT()

// if(pStruct->pOutStyle->scale>1) { GUI_BRIDGE_SetGraphicsCursor(pStruct->point->x,pStruct->point->y+(j*pStruct->pOutStyle->scale)+1); GUI_BRIDGE_SendBuff(); }
// if(pStruct->pOutStyle->scale>1) GUI_BRIDGE_SetPXL( color );
// else INTERFACE_SetPXL( uiX+i, uiY+j, 0x00 );

/*
u8 GUI2015_DRAW_CHAR( u8 ucSYM, u16 uiX, u16 uiY, u8 ucFont, u16 uiCOLF, u16 uiCOLB )
u8 GUI2015_DRAW_CHAR( stGUI2015DrawChar *pStruct ) {

    u8 ucSYMWIDTH 	= FONT_GET_SYM_W( pStruct->ucSYM, pStruct->pOutStyle->ucFont );
	u8 ucSYMHEIGHT = FONT_GET_SYM_H( pStruct->ucSYM, pStruct->pOutStyle->ucFont);
	uint16 uiOFFSET = FONT_GET_SYM_O(pStruct->ucSYM, pStruct->pOutStyle->ucFont);
	u8 ucBYTESINLINE 	= ucSYMWIDTH/8; if( ucSYMWIDTH % 8 ) ucBYTESINLINE++;
	u8 ucOFFS = 0;

	for( u8 j=0;j<ucSYMHEIGHT;j++) 	{
		u8 ucMASK = 0x80;
		u8 ucOFFSETBYTE = 0;
        GUI_BRIDGE_SetGraphicsCursor(pStruct->point->x,pStruct->point->y+j);
        GUI_BRIDGE_ClearBuff();
	 	for( u8 i=0;i<ucSYMWIDTH;i++) {
			uint16 uiOFF = uiOFFSET + ucOFFSETBYTE + ucOFFS;
			u8 ucBYTE = FONT_GET_SYM_B( uiOFF, pStruct->pOutStyle->ucFont );
			GUI_BRIDGE_SetPXL(  ((ucBYTE & ucMASK) == ucMASK ) ? pStruct->pOutStyle->uiCOLF : pStruct->pOutStyle->uiCOLB ); // else INTERFACE_SetPXL( uiX+i, uiY+j, 0x00 );
			if( ucMASK == 0x01 ) { ucMASK = 0x80; ucOFFSETBYTE++; } else ucMASK >>= 1;
		}
        GUI_BRIDGE_SendBuff();
		ucOFFS += ucBYTESINLINE;
	}

	return (pStruct->ucSYM == ' ') ? 12 : ucSYMWIDTH;
}
*/
