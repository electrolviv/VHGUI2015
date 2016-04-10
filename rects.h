#ifndef _VHGUI2015_RECTS_H_
#define _VHGUI2015_RECTS_H_

#include "vhgeom2d.h"

#define cstGUI2015Rect const  stGUI2015Rect
#define GRECT stGUI2015Rect
#define GRECTCP const stGUI2015Rect *

// -------------------------------------------
// Index based
// -------------------------------------------
u16 GUI2015_Rect_W(int idx);
u16 GUI2015_Rect_H(int idx);
const stGUI2015Rect * GUI2015_Rect_GetRef(int idx);

// -------------------------------------------
// Content
// -------------------------------------------
void GUI2015_Rect_ResetContent();
int GUI2015_Rect_CurNew();
int GUI2015_Rect_Add(const stGUI2015Rect *pRect);
void GUI2015_Rect_RemoveLast();

int GUI2015_Rect_AddSplitX(int idx, int prc);
int GUI2015_Rect_AddSplitY(int idx, int prc);
void GUI2015_Rect_Redefine(int idx, const stGUI2015Rect *pNewRect);

// -------------------------------------------
// Misc
// -------------------------------------------
void GUI2015_RectCentering( enGUI2015Align align, const stGUI2015Rect *pRectBase, const stGUI2015Rect *pRectSource, stGUI2015Rect *pRectResult);
void GUI2015_RECT_SplitX(const stGUI2015Rect * src, stGUI2015Rect *pres1, stGUI2015Rect *pres2, int prc);
void GUI2015_RECT_SplitY(const stGUI2015Rect * src, stGUI2015Rect *pres1, stGUI2015Rect *pres2, int prc);
void GUI2015_RECT_SplitXA(const stGUI2015Rect * src, stGUI2015Rect *result, int n, int max);
void GUI2015_RECT_SplitYA(const stGUI2015Rect * src, stGUI2015Rect *result, int n, int max);
void GUI2015_MOVE_Rect(stGUI2015Rect *pRect, int dx, int dy);
void GUI2015_RECT_Shrink(const stGUI2015Rect *pSrc, stGUI2015Rect *pResult, int prcx, int prcy);
u16 GUI2015_RectW(const stGUI2015Rect *pRect);
u16 GUI2015_RectH(const stGUI2015Rect *pRect);
void GUI2015_Rect_Bounds(int rectidx, int x, int y, int mindelta, int *px, int *py);
int GUI2015_RectDistanceQ( const stGUI2015Rect *pRect1, const stGUI2015Rect *pRect2 );
void GUI2015_RECT_DXDY(const stGUI2015Rect *pSrc, stGUI2015Rect *pResult, int dx1,int dy1, int dx2,int dy2);
void GUI2015_Rect_Inside(const stGUI2015Rect *pSrc, int w, int h, int prcFromOriginal, stGUI2015Rect *pResult);

// Defines

#define VHRectSplitX GUI2015_RECT_SplitX
#define VHRectSplitY GUI2015_RECT_SplitY

// -------------------------------------------
// Static Part
// -------------------------------------------

/*
// int GUI2015_AddRect(const stGUI2015Rect *pRect);
// void GUI2015_RECT_Shrink(const stGUI2015Rect *pSrc, stGUI2015Rect *pResult, int prcx, int prcy);
// u16 GUI2015_RectW(stGUI2015Rect *pRect);
// u16 GUI2015_RectH(stGUI2015Rect *pRect);
// void GUI2015_RECT_Copy(stGUI2015Rect * dst, stGUI2015Rect *src);
*/

#endif
