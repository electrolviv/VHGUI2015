#ifndef _VHGUI2015_LABEL_H_
#define _VHGUI2015_LABEL_H_

typedef enum {
	eGUI2015Redraw_single = 0,
	eGUI2015Redraw_dynamic
} enGUI2015Redraw;

typedef struct {
	int objectType;
	int version;
	int idxRect;

	int typeText;
	int valueText;

	int idxStyle;
	int redrawTimeout;
	u8 forceRedraw;
	enGUI2015Align align;
} stGUI2015Label;

// stGUI2015Rect sRect;
// stGUI2015OutStyle sFontStyle;
// stGUI2015TXT sTXT;
// enGUI2015Redraw redraw;
// u16 dynBkColor; // Background color for dynamic update
// void GUI2015_DRAW_Label(stGUI2015Label * pLabel);

#endif
