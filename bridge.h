#ifndef _BRIDGE_GUI_
#define _BRIDGE_GUI_

// Human Interface
void GUI_BRIDGE_Keyboard(int key, int state);
void GUI_BRIDGE_Touch(int x, int y, int state);

// Text resources
const char * GUI_BRIDGE_GetTextPtr(int typeText, int valueText);
const void * GUI_BRIDGE_GetStylePtr(int styleIdx);

// Embedded icons: BMP Format supported
u8 * GUI_BRIDGE_GetPictureBin(int pictureIDX);

// System
// void GUI_BRIDGE_Assert(u8 val);

// Pixels I/O
/*
void GUI_BRIDGE_ClearBuff();
void GUI_BRIDGE_SetGraphicsCursor(u16 x, u16 y);
void GUI_BRIDGE_SetPXL(u32 x);
void GUI_BRIDGE_SendBuff();
*/

// Misc Graphic
/*
void GUI_BRIDGE_Rect(u16 x, u16 y, u16 xx, u16 yy, u32 color, u8 fill);
void GUI_BRIDGE_Circle(u16 x, u16 y, u16 r);
void GUI_BRIDGE_Line(u16 x, u16 y, u16 xx, u16 yy);
void GUI_BRIDGE_Arc(u16 x, u16 y, u16 r, u16 sa, u16 ea, u8 dir);
*/

// REMOVED: Custom user graph
// void GUI_BRIDGE_DRAWGraph( int rectidx, int tag, int color);


#endif
