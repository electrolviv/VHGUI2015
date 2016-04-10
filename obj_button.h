#ifndef _VHGUI2015_BUTTON_H_
#define _VHGUI2015_BUTTON_H_

typedef enum {
	eGUI2015Button_Lockable = 1,
	eGUI2015Button_Locked = 2,
	eGUI2015Button_Disabled = 4,
	eGUI2015Button_dynamicText = 8

} enGUI2015BtnFlag;

typedef struct {

	int objectType;
	int version;

	int idxRect;

	int typeText; int valueText;

	int idxStyle;
	int idxIcon;

	enGUI2015Align align;

	int color1;
	int color2;

	int iCmdType; int iCmdArg1; int iCmdArg2; 	int iCmdArg3;
	int attr; // int redrawTimeout; 	u8 forceRedraw; u8 lockable; u8 locked;

} stGUI2015Button;

#endif

// u8 type;
// stGUI2015Rect sRect;
// stGUI2015OutStyle sFontStyle;
// stGUI2015Rect sRectIco; // picture location
//
// stGUI2015TXT sTXT;
// stGUI2015CMD sCMD;
