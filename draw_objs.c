/*
 * draw_objs.c
 *
 *  Created on: Jan 13, 2016
 *      Author: Viktor
 */

#include "vhgui2015.h"
#include "lib_lcd.h"

static u32 color_fore;
static u32 color_fill;
static u8 fill;

stLCDShapeRect sLCDRect;

void GUI2015_SetForeColor(u32 clr) { color_fore = clr; }
void GUI2015_SetBackColor(u32 clr) { color_fill = clr; }
void GUI2015_SetFill(u8 val) { fill = val; }
// void GUI2015_SetWidth(u8 w) { width = w; }

void GUI2015_DRAW_Rect(const stGUI2015Rect *pRect) {
	sLCDRect.type = eLCDType_Rect;
	sLCDRect.fromx = pRect->sPoint1.x;
	sLCDRect.fromy = pRect->sPoint1.y;
	sLCDRect.tox = pRect->sPoint2.x;
	sLCDRect.toy = pRect->sPoint2.y;
	sLCDRect.color = fill ? color_fill : color_fore;
	sLCDRect.colormode = fill ? 1 : 0;
	LCD_Shape(&sLCDRect);
}

void GUI2015_DRAW_RectFade(GRECTCP pRect, int iColorFrom, int iColorTo) {

	stGUI2015Rect sRect;
	int grads = 16;
	int i;

	GUI2015_SetFill(1);

	for(i=0;i<grads;i++) {
		int iColor = GUI2015_Color_Delta(iColorFrom,iColorTo,(grads - i)*255/grads);
		GUI2015_RECT_SplitYA(pRect,&sRect,i,grads);
		GUI2015_SetBackColor(iColor); GUI2015_SetForeColor(iColor);
		GUI2015_DRAW_Rect(&sRect);
	}

}

void GUI2015_DRAW_RectIdx(int idx) {
	GUI2015_DRAW_Rect( GUI2015_Rect_GetRef(idx));
}

void GUI2015_DRAW_Label(const stGUI2015Label *pLabel) {
	const char *pText = GUI_BRIDGE_GetTextPtr(pLabel->typeText, pLabel->valueText );
	const stGUI2015Rect *pRect = GUI2015_Rect_GetRef(pLabel->idxRect);
	const stGUI2015OutStyle *pStyle = (const stGUI2015OutStyle *)GUI_BRIDGE_GetStylePtr(pLabel->idxStyle);

	GUI2015_SetFill(1);
	GUI2015_SetForeColor(pStyle->uiCOLB);
	GUI2015_SetBackColor(pStyle->uiCOLB);
	GUI2015_DRAW_Rect(pRect);
	// GUI2015_SetFill(1); GUI2015_SetForeColor(RGB(255,0,0)); GUI2015_DRAW_Rect(pRect);
	GUI2015_DRAW_TEXTINRECT(pText,pRect,pStyle,pLabel->align);

}

void GUI2015_DRAW_Button(const stGUI2015Button * pButton, enGUI2015Selector flags) {
	const char *pText = GUI_BRIDGE_GetTextPtr(pButton->typeText,pButton->valueText);
	const stGUI2015Rect *pRect = (const stGUI2015Rect *)GUI2015_Rect_GetRef(pButton->idxRect);

	stGUI2015OutStyle *pStyle;

	if(pButton->attr & eGUI2015Button_Locked) {
		pStyle = (stGUI2015OutStyle *)GUI_BRIDGE_GetStylePtr(2);
	} else if( pButton->attr &  eGUI2015Button_Disabled) {
		pStyle = (stGUI2015OutStyle *)GUI_BRIDGE_GetStylePtr(3);
	} else {
		pStyle =  (stGUI2015OutStyle *)GUI_BRIDGE_GetStylePtr(pButton->idxStyle);
	}

	stGUI2015Rect left, right;

	GUI2015_SetFill(1);
	GUI2015_SetForeColor(pStyle->uiCOLB);
	GUI2015_SetBackColor(pStyle->uiCOLB);
	GUI2015_DRAW_Rect(pRect);

	GUI2015_RECT_SplitX(pRect,&left,&right,10);

	GUI2015_DRAW_TEXTINRECT(pText, &right, pStyle, pButton->align);

	if(pButton->idxIcon) { // Icon available ?
		stGUI2015Rect sRectIcon,sRectWH;
		const u8 * picAddr = GUI_BRIDGE_GetPictureBin(pButton->idxIcon);
		const stGUI2015Rect *pRect = GUI2015_Rect_GetRef(pButton->idxRect);
		GUI2015_GetBMPProps(picAddr, &sRectWH );
		GUI2015_RectCentering(eGUI2015_Align4,pRect,&sRectWH,&sRectIcon);
		sRectIcon.sPoint1.x += 12;
		GUI2015_DrawBMP(picAddr,&sRectIcon);
	}

	{
		stGUI2015Rect t, underline;
		stGUI2015Rect b1,b2;
		int bclr = GUI2015_COLOR_DOWN(pStyle->uiCOLB,90,90,90);
		GUI2015_SetForeColor(bclr);
		GUI2015_SetBackColor(bclr);


		// Underline
		GUI2015_RECT_SplitY(pRect,&t,&underline,90);

		GUI2015_SetFill(1);
		GUI2015_DRAW_Rect(&underline);
		GUI2015_SetFill(0);

		// Border
		if(flags & eGUI2015State_Selected) {
			GUI2015_SetForeColor(RGB(100,0,0)); GUI2015_DRAW_Rect(pRect);
			GUI2015_RECT_DXDY(pRect,&b1,1,1,-1,-1); GUI2015_SetForeColor(RGB(200,0,0)); GUI2015_DRAW_Rect(&b1);
			GUI2015_RECT_DXDY(&b1,&b2,1,1,-1,-1); GUI2015_SetForeColor(RGB(255,0,0));
			GUI2015_DRAW_Rect(&b2);

		} else {
			GUI2015_SetForeColor(GUI2015_COLOR_DOWN(pStyle->uiCOLB,65,65,65));
			GUI2015_DRAW_Rect(pRect);
		}


	}
}

void GUI2015_DRAW_CheckBox( const stGUI2015Checkbox *pBox, enGUI2015Selector flags) {
	const char *pText = GUI_BRIDGE_GetTextPtr(pBox->typeText,pBox->valueText);
	const stGUI2015Rect *pRect = GUI2015_Rect_GetRef(pBox->idxRect);
	const stGUI2015OutStyle *pStyle = (const stGUI2015OutStyle *)GUI_BRIDGE_GetStylePtr(pBox->idxStyle);
	stGUI2015Rect sRectIcon,sRectWH;
	u8 * picAddr = GUI_BRIDGE_GetPictureBin(pBox->val ? 2:1 ); // eIcon_CHK1 : eIcon_CHK0

	GUI2015_SetFill(1); GUI2015_SetBackColor( pStyle->uiCOLB ); GUI2015_DRAW_Rect(pRect);
	// GUI2015_RECT_SplitX(pRect,&left,&right,10);
	GUI2015_SetBackColor(pStyle->uiCOLF); GUI2015_DRAW_TEXTINRECT(pText, pRect, pStyle, pBox->align);

	GUI2015_GetBMPProps(picAddr, &sRectWH );
	GUI2015_RectCentering(eGUI2015_Align4, pRect, &sRectWH, &sRectIcon);
	sRectIcon.sPoint1.x += 12;
	GUI2015_DrawBMP(picAddr,&sRectIcon);

	if(flags & eGUI2015State_Selected ) {
		GUI2015_SetFill(0);
		GUI2015_SetForeColor( RGB(255,0,0) ); // pStyle->uiCOLB;
		GUI2015_DRAW_Rect(pRect);
	}

}

void GUI2015_DRAW_Graph(const stGUI2015Graph *pGraph) {

	if(!(pGraph->tag)) {
		GUI2015_SetFill(pGraph->filled);
		GUI2015_SetForeColor(pGraph->color);
		GUI2015_SetBackColor(pGraph->fillColor);
		GUI2015_DRAW_RectIdx(pGraph->idxRect);
	}

}

void GUI2015_DRAW_Window(const stGUI2015Window *pWnd) {
	stGUI2015Rect rc, rw, underl, tmp;
	const char *pCaptionText = GUI_BRIDGE_GetTextPtr( pWnd->typeText, pWnd->valueText);
	const stGUI2015OutStyle * pCaptionStyle = (const stGUI2015OutStyle *)GUI_BRIDGE_GetStylePtr(pWnd->idxCaptionStyle);

	GUI2015_RECT_SplitY( GUI2015_Rect_GetRef(pWnd->idxRect), &rc, &rw, 15);
	GUI2015_RECT_SplitY( &rc, &tmp, &underl, 90);

	int cColor = pWnd->colorCaption;
	int cColor2 = GUI2015_COLOR_DOWN(cColor,70,70,70);

	GUI2015_DRAW_RectFade(&rc, cColor, cColor2);

	GUI2015_SetBackColor(pWnd->colorWindow); GUI2015_DRAW_Rect(&rw);
	GUI2015_SetBackColor(GUI2015_COLOR_DOWN(pWnd->colorCaption,90,90,90)); GUI2015_DRAW_Rect(&underl);

	GUI2015_DRAW_TEXTINRECT(pCaptionText,&rc,pCaptionStyle,eGUI2015_Align4d);
}

/*
void GUI2015_Layout_Rect(enLayoutMap map, int pos, stGUI2015Rect * pResult) {
	switch(map) {
		case eLayout2015_1X5: pResult->sPoint1.x = 200; pResult->sPoint1.y = 120 + (pos*60); pResult->sPoint2.x = 600; pResult->sPoint2.y = pResult->sPoint1.y + 40;
		break;
		default: { pResult->sPoint1.x = 20; pResult->sPoint1.y = 200; pResult->sPoint2.x = 400; pResult->sPoint2.y = 240; 	} }
}

void GUI2015_DRAW_Label2(stGUI2015Label * pLabel) {

	char *pText = GUI2015_BRIDGE_FormatString(&pLabel->sTXT);
	// stGUI2015Rect sTxtRect, sFinalTxtRect;
	// GUI2015_GetTextRect(pText, &pLabel->sFontStyle, &sTxtRect); // Text props
	// GUI2015_RectCentering(pLabel->align, &pLabel->sRect, &sTxtRect, &sFinalTxtRect);  // centering text
	// GUI2015_DRAW_Rect(&r2, GUI2015_COLOR_DOWN(pButton->color1, 3, 3, 3), 1);
	GUI2015_DRAW_TEXTINRECT(pText, &pLabel->sRect, &pLabel->sFontStyle, pLabel->align);

}

static void GUI2015_DRAW_Button_V1(stGUI2015Button * pButton) {

	stGUI2015Rect r1, r2;
	stGUI2015Rect sTxtRect, sFinalTxtRect;

	GUI2015_RECT_SplitH(&pButton->sRect, &r1, &r2, 90);
	GUI2015_DRAW_Rect(&r1, pButton->color1, 1);
	GUI2015_DRAW_Rect(&r2, GUI2015_COLOR_DOWN(pButton->color1, 3, 3, 3), 1);

	// stGUI2015DrawText sDrawText;
	// sDrawText.align = eGUI2015_Align5;
	// sDrawText.psText =
	// GUI2015_DRAW_TEXT(&sDrawText);
	// align = { text, { sFinalTxtRect.sPoint1.x, sFinalTxtRect.sPoint1.y } };
	// memcpy(&sDrawText.sOutStyle, &pButton->sFontStyle, sizeof(stGUI2015OutStyle));
	// sDrawText.sOutStyle.uiCOLF = GUI2015_COLOR_DOWN(pButton->sFontStyle.uiCOLF, 8, 16, 8);
	// GUI2015_DRAW_TEXT(&sDrawText);
	// sDrawText.sPoint.x--; sDrawText.sPoint.y--;
	// sDrawText.sOutStyle.uiCOLF = pButton->sFontStyle.uiCOLF;

}

static void GUI2015_DRAW_Button_V2(stGUI2015Button * pButton) {
	stGUI2015DrawText sDrawText;
	GUI2015_DRAW_Rect( &pButton->sRect, pButton->color1, 1);

	GUI2015_TextRect((char *)pButton->textPtr, &pButton->sFontStyle, &sTxtRect); // Text props
	GUI2015_RectCentering(&pButton->sRect, &sTxtRect, &sFinalTxtRect);  // centering text
	stGUI2015DrawText sDrawText = { (char *)pButton->textPtr, { pButton->sRect.sPoint1.x + 10, sFinalTxtRect.sPoint1.y } };
	memcpy(&sDrawText.sOutStyle, &pButton->sFontStyle, sizeof(stGUI2015OutStyle));
	GUI2015_DRAW_TEXT(&sDrawText);

 */

// stGUI2015Rect r1,r2,r3,r4;

// GUI2015_RECT_SplitH(&r1, &r3, &r4, 90); // r1 > r3+r4
// GUI2015_DRAW_Rect(&r3, pWindow->colorCaption, 1);
// GUI2015_DRAW_Rect(&r4, GUI2015_COLOR_DOWN(pWindow->colorCaption, 2, 2, 2), 1);
// GUI2015_DRAW_Rect(&r2, pWindow->colorWindow, 1);
// stGUI2015DrawText sDrawText = { &pWindow->sTxt, &r1, &pWindow->sCaptionStyle, eGUI2015_Align4d };
// GUI2015_DRAW_TEXT(&sDrawText);
// { r1.sPoint1.x + 12, r1.sPoint1.y + 14 }  };
// sDrawText.sText.addr = 0;  // pWindow->textCaption
// sDrawText.sText.type =
// GUI2015_STYLE_Copy(&sDrawText.sOutStyle, &pWindow->sCaptionStyle);
// sDrawText.sOutStyle.uiCOLF = GUI2015_COLOR_DOWN(pWindow->sCaptionStyle.uiCOLF, 5, 10, 5);
// sDrawText.sPoint.x--; sDrawText.sPoint.y--;
// sDrawText.sOutStyle.uiCOLF = pWindow->sCaptionStyle.uiCOLF;
// GUI2015_DRAW_TEXT(&sDrawText);

void GUI2015_DrawObject(void *ptr, enGUI2015Selector optional ) {
	int iType = *((int *)ptr);

	switch( iType ) {
		case eGUI2015_OBJ_WINDOW: GUI2015_DRAW_Window( (const stGUI2015Window*)ptr); break;
		case eGUI2015_OBJ_LAB: GUI2015_DRAW_Label((const stGUI2015Label*)ptr); break;
		case eGUI2015_OBJ_BUT: GUI2015_DRAW_Button((const stGUI2015Button*)ptr, optional ); break;
		case eGUI2015_OBJ_CHBOX: GUI2015_DRAW_CheckBox((const stGUI2015Checkbox*)ptr, optional ); break;
		case eGUI2015_OBJ_PIC: GUI2015_DRAW_Picture((const stGUI2015Picture*)ptr); break;
		case eGUI2015_OBJ_GRAPH: GUI2015_DRAW_Graph((const stGUI2015Graph*)ptr); break;
		case eGUI2015_OBJ_PBAR: break;
		default: { }
	}

}

void GUI2015_Stream32(int x, int y, int pxls, void *ptr) { LCD_Stream32(x,y,pxls,ptr); }
void GUI2015_StreamBW(int x, int y, int pxls, void *ptr) { LCD_StreamBW(x,y,pxls,ptr); }

