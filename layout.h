#ifndef _VHGUI2015_LAYOUT_H_
#define _VHGUI2015_LAYOUT_H_

typedef enum {
	
		eLayout2015_LinearLists,
		eLayout2015_1X4,
		eLayout2015_1X5,
		eLayout2015_1X6,

		eLayout2015_2X4,
		eLayout2015_2X5,
		eLayout2015_2X6,

		eLayout2015_SystemLine

} enLayoutMap;

void GUI2015_Layout_Rect(enLayoutMap map, int pos, stGUI2015Rect * pResult);

#endif
