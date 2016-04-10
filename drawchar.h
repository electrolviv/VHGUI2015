#ifndef _VHGUI2015_TEXT_H_
#define _VHGUI2015_TEXT_H_

typedef struct { 
	u8 ucSYM;
	stGUI2015Point *pPoint;
	stGUI2015OutStyle *pOutStyle;
} stGUI2015DrawChar;

u8 GUI2015_DRAW_CHAR(stGUI2015DrawChar *pStruct);

#endif
