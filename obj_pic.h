#ifndef _VHGUI2015_PIC_H_
#define _VHGUI2015_PIC_H_

typedef struct {
	int objectType;
	int version;
	int idxRect;
	int idxPicture;
	u8 flags;
} stGUI2015Picture;

typedef struct {
	int objectType;
	int version;
	int tag;
	int idxRect;
	int color;
	u8 forceRedraw;
	int fillColor;
	u8 filled;
} stGUI2015Graph;

void GUI2015_GetBMPProps(const u8 *pBMP, stGUI2015Rect *pResult );
void GUI2015_DrawBMP(const u8 *pBMP,const stGUI2015Rect *pRect) ;

int GUI2015_Picture_GetWH( const stGUI2015Picture  * pPicture, u16 *pw, u16 *ph );
void GUI2015_DRAW_Picture(  const stGUI2015Picture  * pPicture );

#endif
