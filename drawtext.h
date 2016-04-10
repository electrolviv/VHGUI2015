#ifndef _VHGUI2015_DRAWTEXT_H_
#define _VHGUI2015_DRAWTEXT_H_

#define FONT_SOLID 1
#define FONT_TRANS 1

#define FONT_SPACING_1 1
#define FONT_SPACING_2 2

typedef struct { int idxtxt; stGUI2015Rect *pRect; stGUI2015OutStyle *pOutStyle; enGUI2015Align align; } stGUI2015DrawText;

// Style props
void GUI2015_STYLE_Copy(stGUI2015OutStyle *dst, stGUI2015OutStyle *src);

// Text Params
u16 GUI2015_TextWidth(const char *pText, u8 font);
u16 GUI2015_TextHeight(const char *pText, u8 font);
void GUI2015_GetTextRect(const char *pText, const stGUI2015OutStyle *pFontStyle, stGUI2015Rect *pResult);

// Graph Output
u16 GUI2015_DRAW_TEXT( stGUI2015DrawText *pStruct );
u16 GUI2015_DRAW_TEXTINRECT(const char *pText, const stGUI2015Rect *pRect, const stGUI2015OutStyle *pFontStyle, enGUI2015Align align );

#endif
