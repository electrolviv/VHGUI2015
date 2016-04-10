#include "vhgui2015.h"

static stGUI2015Rect rects[GUI2015_RECTS_MAX];
static stGUI2015Rect tmp1, tmp2;
const stGUI2015Rect rectAbsFullScreen = { {0,0}, {GUI2015_SCREEN_W-1, GUI2015_SCREEN_H-1} };
static int currentRect;

// ------------------------------
// Static part
// ------------------------------
u16 GUI2015_RectW(const stGUI2015Rect *pRect) { return pRect->sPoint2.x - pRect->sPoint1.x; }

u16 GUI2015_RectH(const stGUI2015Rect *pRect) { return pRect->sPoint2.y - pRect->sPoint1.y; }

static void GUI2015_RECT_Copy(stGUI2015Rect * dst, const stGUI2015Rect *src) { vhmemcpy(dst, src, sizeof(stGUI2015Rect)); }

void GUI2015_MOVE_Rect(stGUI2015Rect *pRect, int dx, int dy) { pRect->sPoint1.x += dx; pRect->sPoint1.y += dy; pRect->sPoint2.x += dx; pRect->sPoint2.y += dy; }

void GUI2015_RECT_SplitX(const stGUI2015Rect * src, stGUI2015Rect *pres1, stGUI2015Rect *pres2, int prc) {
	u16 a = ((GUI2015_RectW(src) * prc) / 100);
	pres1->sPoint1.x = src->sPoint1.x; pres1->sPoint1.y = src->sPoint1.y;
	pres1->sPoint2.x = src->sPoint1.x + a; pres1->sPoint2.y = src->sPoint2.y;
	pres2->sPoint1.x = pres1->sPoint2.x; pres2->sPoint1.y = pres1->sPoint1.y;
	pres2->sPoint2.x = src->sPoint2.x; pres2->sPoint2.y = src->sPoint2.y;
}

void GUI2015_RECT_SplitY(const stGUI2015Rect * src, stGUI2015Rect *pres1, stGUI2015Rect *pres2, int prc) {
	pres1->sPoint1.x = src->sPoint1.x; pres1->sPoint1.y = src->sPoint1.y;
	pres1->sPoint2.x = src->sPoint2.x; pres1->sPoint2.y = src->sPoint1.y + ((GUI2015_RectH(src) * prc) / 100);
	pres2->sPoint1.x = src->sPoint1.x; pres2->sPoint1.y = pres1->sPoint2.y;
	pres2->sPoint2.x = src->sPoint2.x; pres2->sPoint2.y = src->sPoint2.y;
}

void GUI2015_RECT_SplitXA(const stGUI2015Rect * src, stGUI2015Rect *result, int n, int max) {
	u16 dx = GUI2015_RectW(src) / max;
	result->sPoint1.x = src->sPoint1.x + (n * dx); result->sPoint1.y = src->sPoint1.y;
	result->sPoint2.x = result->sPoint1.x + dx; result->sPoint2.y = src->sPoint2.y;
}

void GUI2015_RECT_SplitYA(const stGUI2015Rect * src, stGUI2015Rect *result, int n, int max) {
	u16 dy = GUI2015_RectH(src) / max;
	result->sPoint1.x = src->sPoint1.x; result->sPoint1.y = src->sPoint1.y  + (n * dy);
	result->sPoint2.x = src->sPoint2.x; result->sPoint2.y = result->sPoint1.y + dy;
}

void GUI2015_RECT_Shrink(const stGUI2015Rect *pSrc, stGUI2015Rect *pResult, int prcx, int prcy) {
	int w = GUI2015_RectW(pSrc);
	int h = GUI2015_RectH(pSrc);
	int dw = w * prcx / 100;
	int dh = h * prcy / 100;

	pResult->sPoint1.x = pSrc->sPoint1.x + (dw>>1);
	pResult->sPoint1.y = pSrc->sPoint1.y + (dh>>1);
	pResult->sPoint2.x = pSrc->sPoint2.x - (dw>>1);
	pResult->sPoint2.y = pSrc->sPoint2.y - (dh>>1);
}

void GUI2015_RECT_DXDY(const stGUI2015Rect *pSrc, stGUI2015Rect *pResult, int dx1,int dy1, int dx2,int dy2) {
	pResult->sPoint1.x = pSrc->sPoint1.x + dx1;
	pResult->sPoint1.y = pSrc->sPoint1.y + dy1;
	pResult->sPoint2.x = pSrc->sPoint2.x + dx2;
	pResult->sPoint2.y = pSrc->sPoint2.y + dy2;
}

// ------------------------------
// index table
// ------------------------------
u16 GUI2015_Rect_W(int idx) { return GUI2015_RectW(&rects[idx]); }
u16 GUI2015_Rect_H(int idx) { return GUI2015_RectH(&rects[idx]); }
const stGUI2015Rect * GUI2015_Rect_GetRef(int idx) { if(idx<0) return &rectAbsFullScreen; return (const stGUI2015Rect *)&rects[idx]; }

void GUI2015_Rect_Bounds(int rectidx, int x, int y, int mindelta, int *px, int *py) {
	const stGUI2015Rect *p = GUI2015_Rect_GetRef(rectidx);

	int minx = p->sPoint1.x + mindelta;
	int maxx = p->sPoint2.x - mindelta;
	int miny = p->sPoint1.y + mindelta;
	int maxy = p->sPoint2.y - mindelta;

	if(x<minx) { *px = minx; } else if(x>maxx) { *px = maxx; } else *px = x;
	if(y<miny) { *py = miny; } else if(y>maxy) { *py = maxy; } else *py = y;
}


// -------------------------------------------
// Content
// -------------------------------------------
void GUI2015_Rect_ResetContent() { currentRect = 0; }
int GUI2015_Rect_CurNew() { return currentRect; }
int GUI2015_Rect_Add(const stGUI2015Rect *pRect) {
	int r = currentRect;
	if(currentRect<GUI2015_RECTS_MAX) { GUI2015_RECT_Copy(&rects[currentRect], pRect); currentRect++; 	}
	return r;
}

void GUI2015_Rect_RemoveLast() { currentRect--; }

int GUI2015_Rect_AddSplitX(int idx, int prc) {
	GUI2015_RECT_SplitX(&rects[idx], &tmp1, &tmp2, prc);
	GUI2015_Rect_Add(&tmp1);
	return GUI2015_Rect_Add(&tmp2);
}

int GUI2015_Rect_AddSplitY(int idx, int prc) {
	GUI2015_RECT_SplitY(&rects[idx], &tmp1, &tmp2, prc);
	GUI2015_Rect_Add(&tmp1);
	return GUI2015_Rect_Add(&tmp2);
}

void GUI2015_Rect_Redefine(int idx, const stGUI2015Rect *pNewRect) {
	if(idx<currentRect) { GUI2015_RECT_Copy(&rects[idx], pNewRect); }
}


void GUI2015_RectCentering(
		enGUI2015Align align,
		const stGUI2015Rect *pRectBase,
		const stGUI2015Rect *pRectSource,
		stGUI2015Rect *pRectResult) {

	u16 w_base = GUI2015_RectW(pRectBase);
	u16 w_src = GUI2015_RectW(pRectSource);

	u16 h_base = GUI2015_RectH(pRectBase);
	u16 h_src = GUI2015_RectH(pRectSource);

	switch(align)
	{
		case eGUI2015_Align4:
		pRectResult->sPoint1.x = pRectBase->sPoint1.x;
		pRectResult->sPoint1.y = pRectBase->sPoint1.y + ((h_base - h_src) >> 1);
		pRectResult->sPoint2.x = pRectResult->sPoint1.x + w_src;
		pRectResult->sPoint2.y = pRectResult->sPoint1.y + h_src;
		break;

		case eGUI2015_Align4d: 
		pRectResult->sPoint1.x = pRectBase->sPoint1.x + h_base;
		pRectResult->sPoint1.y = pRectBase->sPoint1.y + ((h_base - h_src) >> 1);
		pRectResult->sPoint2.x = pRectResult->sPoint1.x + w_src;
		pRectResult->sPoint2.y = pRectResult->sPoint1.y + h_src;
		break;

		case eGUI2015_Align6:
		pRectResult->sPoint1.x = pRectBase->sPoint2.x - w_src;
		pRectResult->sPoint2.x = pRectBase->sPoint2.x;
		pRectResult->sPoint1.y = pRectBase->sPoint1.y + ((h_base - h_src) >> 1);
		pRectResult->sPoint2.y = pRectResult->sPoint1.y + h_src;
		break;

		case eGUI2015_Align7:
		pRectResult->sPoint1.x = pRectBase->sPoint2.x - w_src;
		pRectResult->sPoint1.y = pRectBase->sPoint2.y - h_src;
		pRectResult->sPoint2.x = pRectBase->sPoint2.x;
		pRectResult->sPoint2.y = pRectBase->sPoint2.y;
		break;

		default: {
		pRectResult->sPoint1.x = pRectBase->sPoint1.x + (w_base/2) - (w_src/2);
		pRectResult->sPoint1.y = pRectBase->sPoint1.y + ((h_base - h_src) >> 1);
		pRectResult->sPoint2.x = pRectResult->sPoint1.x + w_src;
		pRectResult->sPoint2.y = pRectResult->sPoint1.y + h_src;
		}
	}
}

static void GUI2015_Rect_Center(const stGUI2015Rect *pRect, stGUI2015Point * pResult ) {
	pResult->x = pRect->sPoint1.x + (pRect->sPoint2.x - pRect->sPoint1.x)/2;
	pResult->y = pRect->sPoint1.y + (pRect->sPoint2.y - pRect->sPoint1.y)/2;
}

int GUI2015_RectDistanceQ( const stGUI2015Rect *pRect1, const stGUI2015Rect *pRect2 ) {

	int dx,dy;

	stGUI2015Point b1c, b2c;
	GUI2015_Rect_Center(pRect1,&b1c);
	GUI2015_Rect_Center(pRect2,&b2c);

	dx = b1c.x - b2c.x;
	dy = b1c.y - b2c.y;

	return (dx*dx)+(dy*dy);
}

void GUI2015_Rect_Inside(const stGUI2015Rect *pSrc, int w, int h, int prcFromOriginal, stGUI2015Rect *pResult) {

	// Graph Box area
	int area_w = GUI2015_RectW(pSrc);
	int area_h = GUI2015_RectH(pSrc);
	int lim_w = (area_w * prcFromOriginal) / 100; // Absolute limits, w
	int lim_h = (area_h * prcFromOriginal) / 100;	// Absolute limits,	h

	int fin_w, fin_h;

	// Option #1 Resize by X
	// Option #2 Resize by Y
	double k1 = w * 1.0 / lim_w;
	int new_h = (int)(h/k1);

	if(new_h<lim_h) {
		fin_w = lim_w;
		fin_h = new_h;
	} else {
			double k2 = h * 1.0 / lim_h;
			fin_w = (int)(w/k2);
			fin_h = lim_h;
	}


	stGUI2015Point sCenter;

	GUI2015_Rect_Center(pSrc,&sCenter);

	pResult->sPoint1.x = sCenter.x - (fin_w>>1);
	pResult->sPoint1.y = sCenter.y - (fin_h>>1);

	pResult->sPoint2.x = sCenter.x + (fin_w>>1);
	pResult->sPoint2.y = sCenter.y + (fin_h>>1);

	// double k;
	// fin_w = area_w; //  * 96 / 100;
	// k = 1.0 * fin_w / image_w;
	// fin_h = (int)(k * image_h); //  / (k*100); // fin_h = area_h * 80 / 100;
		// { area->sPoint1.x + (area_w>>1) - (fin_w>>1), area->sPoint1.y + (area_h>>1) - (fin_h>>1) },
		// { area->sPoint1.x + (area_w>>1) + (fin_w>>1), area->sPoint1.y + (area_h>>1) + (fin_h>>1) } };


}

// const stGUI2015Rect * Engraver_Rect_GetFullScreen() { return &rectFullScreen; }

/*
static const stGUI2015Rect srectFULLSCR = { {0,0}, {800,480} };
static const stGUI2015Rect srectMainScr = { {0,64}, {800, 400} };
static const stGUI2015Rect srectWinCaption = { {0,0}, {800, 64} };
static const stGUI2015Rect srectINVALID = { { 790,0}, { 799, 9 } };

static const stEngraverRects sEngraverRects[EngraverRect_Last] = { { EngraverRect_FullScreen, &srectFULLSCR }, { EngraverRect_MainScreen, &srectMainScr }, { EngraverRect_WinCaption, &srectWinCaption } };

typedef struct { enEngraverRect eRect; const stGUI2015Rect * pRect; } stEngraverRects;
typedef enum { EngraverRect_FullScreen = 0, EngraverRect_MainScreen, EngraverRect_WinCaption, EngraverRect_Last } enEngraverRect;

const stGUI2015Rect * Engraver_GetRect( enEngraverRect eRect ) {
	int i;

	for(i=0;i<EngraverRect_Last;i++) {
		stEngraverRects *pcur = &sEngraverRects[i];
		if(eRect == pcur->eRect) { return pcur->pRect; }
	}

	return &srectINVALID;
}

*/
