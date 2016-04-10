#ifndef _VHGUI2015_COLORS_H_
#define _VHGUI2015_COLORS_H_

#define _OLDRGB16(r,g,b) (u16)(((r>>3)<<11)|((g>>2)<<5)|(b>>3))
#define _OLDRGB1616(r,g,b) ( (r<<11) | (g<<5) | b )

// 
#define COLOR_VALR(X)	((X>>11)&0x1F)
#define COLOR_VALG(X)	((X>>5)&0x3F)
#define COLOR_VALB(X)	(X&0x1F)

// Extract 8-bit values
#define R_RGB16(C) (COLOR_VALR(C)<<3)
#define G_RGB16(C) (COLOR_VALG(C)<<2)
#define B_RGB16(C) (COLOR_VALB(C)<<3) 

// 
#define COLOR_CDOWN(X) 
#define COLOR_RDOWN(X)
#define COLOR_GDOWN(X)
#define COLOR_BDOWN(X)

//
#define COLOR_WHITE RGB(255, 255, 255)

#define COLOR_YEL RGB(255,255,100)

#define COLOR_RED RGB(255, 0, 0)
#define COLOR_REDD RGB(180, 0, 0)
#define COLOR_REDL RGB(180, 100, 100)

#define COLOR_GREEN RGB(0, 255, 0)
#define COLOR_GREEND RGB(0, 180, 0)
#define COLOR_GREENDD RGB(0, 128, 0)

#define COLOR_BLUE RGB(0, 0, 255)
#define COLOR_BLUED RGB(0, 0, 155)
#define COLOR_BLUEDD RGB(0, 0, 124)
#define COLOR_BLACK RGB(0, 0, 0)
#define COLOR_BLUEDTMP RGB(0, 0, 154)

#define COLOR_GRAYLL  RGB(240,240,240)
#define COLOR_GRAYL  RGB(220,220,220)
#define COLOR_GRAY  RGB(127,127,127)
#define COLOR_GRAYDD RGB(86,86,86)

#define COLOR_LGREEN RGB(200,255,200)
#define COLOR_DGREEN  RGB(100,155,100)
#define COLOR_DGREEN2 RGB(80,140,80)

int GUI2015_COLOR_DOWN(int c, u8 prcr, u8 prcg, u8 prcb);
int GUI2015_Color_Delta(int from, int to, int a);

#endif
