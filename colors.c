#include "vhgui2015.h"

int GUI2015_COLOR_DOWN(int c, u8 prcr, u8 prcg, u8 prcb) {
	u8 r = prcr * GetRValue(c) / 100;
	u8 g = prcg * GetGValue(c) / 100;
	u8 b = prcb * GetBValue(c) / 100;
	return RGB(r, g, b);
}

static u8 deltizer(u8 from, u8 to,u8 alpha) {
	return to - ((alpha * (to-from)) / 256);
}

int GUI2015_Color_Delta(int from, int to, int a) {
	u8 r = deltizer(GetRValue(from),GetRValue(to),a);
	u8 g = deltizer(GetGValue(from),GetGValue(to),a);
	u8 b = deltizer(GetBValue(from),GetBValue(to),a);
	return RGB(r,g,b);
}
