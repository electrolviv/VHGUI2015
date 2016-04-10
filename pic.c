/*
 * pic.c
 *
 *  Created on: Dec 1, 2015
 *      Author: Viktor
 */

#include "vhgui2015.h"
#include "lib_lcd.h"


void GUI2015_GetBMPProps(const u8 *pBMP, stGUI2015Rect *pResult) {
	BITMAPINFOHEADER *pBMPHDR = (BITMAPINFOHEADER *)(pBMP + sizeof(BITMAPFILEHEADER));
	pResult->sPoint1.x = 0; pResult->sPoint1.y = 0;
	pResult->sPoint2.x = pBMP ? (u16)pBMPHDR->biWidth : 0;
	pResult->sPoint2.y = pBMP ? (u16)pBMPHDR->biHeight : 0;
}

int GUI2015_Picture_GetWH( const stGUI2015Picture  * pPicture, u16 *pw, u16 *ph ) {
	u8 * picAddr = GUI_BRIDGE_GetPictureBin(pPicture->idxPicture);
	BITMAPINFOHEADER *pBMPHDR;
	pBMPHDR = (BITMAPINFOHEADER *)(picAddr + sizeof(BITMAPFILEHEADER));
	*pw = (u16)pBMPHDR->biWidth; *ph = (u16)pBMPHDR->biHeight;
	return 0;
}

static int gfxbuff[400];
static int gfxbuffi = 0;
static int gfxx,gfxy;

void GUI2015_DrawBMP(const u8 *pBMP,const stGUI2015Rect *pRect) {

	BITMAPFILEHEADER *pFILHDR;
	BITMAPINFOHEADER *pBMPHDR;
	u8 *PAL;
	u8 *pDATA;
	int BMPBytesPerLine;
	int i,j;

	u8 * picAddr = (u8 *) pBMP; 

	// GUI2015_DRAW_Rect(pRect);

	if(!picAddr) {
		GUI2015_SetFill(1); GUI2015_SetForeColor(RGB(255,0,0));
		GUI2015_DRAW_Rect(pRect);
		// GUI_BRIDGE_Rect(pRect->sPoint1.x,pRect->sPoint1.y,pRect->sPoint1.x + 10, pRect->sPoint1.y + 10, ,1);
		return;
	}

	pFILHDR = (BITMAPFILEHEADER *)picAddr;
	pBMPHDR = (BITMAPINFOHEADER *)(picAddr + sizeof(BITMAPFILEHEADER));
	pDATA = (u8 *)(picAddr + pFILHDR->bfOffBits);
	
	// (u8 *)(pPic->picAddr + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) + ((pBMPHDR->biBitCount == 8) ? (256*4) : 0);
	// BMPBytesPerLine+=  > 0 ? (pBMPHDR->biWidth >> 2)+1 : 0;
	// BMPbytesPerLine = (bmphdr.biWidth % 4) > 0 ? ((bmphdr.biWidth >> 2)+1) << 2 : bmphdr.biWidth;

	if(pBMPHDR->biBitCount == 8) {
		int r = pBMPHDR->biWidth % 4;
		BMPBytesPerLine = pBMPHDR->biWidth + ( r ? (4-r) : 0);
		PAL = pDATA - (256*4); // pPic->picAddr + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	} else {
		BMPBytesPerLine = pBMPHDR->biWidth*3;
		PAL = 0;
	}

	for(j=0;j<pBMPHDR->biHeight;j++) {
		u8 *pLine = pDATA + ((pBMPHDR->biHeight - j - 1) * BMPBytesPerLine);
		gfxbuffi = 0; gfxx = pRect->sPoint1.x; gfxy = pRect->sPoint1.y + j;
		
		for(i=0;i<pBMPHDR->biWidth;i++) {
			u8 r,g,b;
			
			if(pBMPHDR->biBitCount == 8) {
				u8 idx = pLine[i]; 	r = PAL[(idx*4)+2]; g = PAL[(idx*4)+1]; b = PAL[(idx*4)+0];
			} else {
				r = *(pLine + (i*3) + 2); g = *(pLine + (i*3) + 1); b = *(pLine + (i*3) + 0);
			}

			gfxbuff[gfxbuffi++] = RGB(r,g,b);
		} 
		LCD_Stream32(gfxx,gfxy,gfxbuffi, gfxbuff);
	}

}

void GUI2015_DRAW_Picture(  const stGUI2015Picture  * pPicture ) {
	
	u8 * picAddr = GUI_BRIDGE_GetPictureBin(pPicture->idxPicture);
	const stGUI2015Rect *pRect = GUI2015_Rect_GetRef(pPicture->idxRect);
	GUI2015_DrawBMP(picAddr,pRect);

}
