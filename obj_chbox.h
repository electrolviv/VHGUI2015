/*
 * chbox.h
 *
 *  Created on: 9 но€б. 2015 г.
 *      Author: user
 */

#ifndef GUI2015_VHGUI2015_CHBOX_H_
#define GUI2015_VHGUI2015_CHBOX_H_

typedef struct {
	int objectType;
	int version;
	int idxRect;
	int typeText; int valueText;
	int idxStyle;
	enGUI2015Align align;

	int iCmdType;
	int iCmdArg1;
	int iCmdArg2;
	int iCmdArg3;

	u8 val;

} stGUI2015Checkbox;

#endif /* GUI2015_VHGUI2015_CHBOX_H_ */
