#ifndef _VHGUI2015_H_
#define _VHGUI2015_H_

#ifdef __cplusplus__
extern "C" {
#endif

#ifdef _WIN32
#include "vhboard.h"
#endif

#ifdef STM32F4XX
#include "vhboard.h"
#include "Bitmap.h"
#define GetRValue(X) ((X>>16)&0xFF)
#define GetGValue(X) ((X>>8)&0xFF)
#define GetBValue(X) (X&0xFF)
#define RGB(R,G,B) ((R<<16)|(G<<8)|(B))
#endif

#define vhmemcpy memcpy

// Settings
#define GUI2015_SCREEN_W			800
#define GUI2015_SCREEN_H			480

#define GUI2015_OBJECTS_MAX 	164
#define GUI2015_RECTS_MAX		164
#define GUI2015_STYLES_MAX		64

typedef enum {
	eGUI2015Event_UP, eGUI2015Event_DOWN, eGUI2015Event_LEFT, eGUI2015Event_RIGHT, eGUI2015Event_CENTER,
	eGUI2015Event_B1ON, eGUI2015Event_B1OFF, eGUI2015Event_B2ON, eGUI2015Event_B2OFF } enGUI2015Event;

#include "colors.h"

#include "align.h"
#include "objtypes.h"
#include "bridge.h"

#include "fonts.h"
#include "rects.h"
#include "outstyle.h"
#include "drawchar.h"
#include "drawtext.h"
#include "layout.h"

#include "obj_window.h"
#include "obj_label.h"
#include "obj_button.h"
#include "obj_pic.h"
#include "obj_chbox.h"
#include "obj_pbar.h"

#include "graph.h"

// ----------------------------
// Content
// ----------------------------
void GUI2015_ResetContent();
int GUI2015_AddObj(void *ptr);
int GUI2015_ObjsCnt();

void GUI2015_CutoffObjects(int newidx); // Cut all objects before pos

void GUI2015_EnableFocus();	 	// Highlight selected button
void GUI2015_PressedFocus();	// Object pressed
void GUI2015_DisableFocus(); 	// Disable selected button highlight

void GUI2015_DrawWindowContent();
void GUI2015_DrawWindowContentFrom(int startidx);
void GUI2015_UpdateWindowContent();

void * GUI2015_GetSelectedObjectPtr();
void GUI2015_GetSelectedObjectFct(int *pCmdType, int *pCmdArg1, int *pCmdArg2, int *pCmdArg3, enGUI2015Obj *pObjType);

void GUI2015_Event(enGUI2015Event evt);

// ---------------------
// Hardware bridge
// ---------------------
void GUI2015_DrawObject(void *ptr, enGUI2015Selector optional );
void GUI2015_Stream32(int x, int y, int pxls, void *ptr);
void GUI2015_StreamBW(int x, int y, int pxls, void *ptr);


#ifdef __cplusplus__
}
#endif

#endif
