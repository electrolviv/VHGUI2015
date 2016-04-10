/*
 * graph.h
 *
 *  Created on: Dec 9, 2015
 *      Author: Viktor
 */

#ifndef GUI2015INC_GRAPH_H_
#define GUI2015INC_GRAPH_H_

typedef enum { eGUI2015State_Normal = 0, eGUI2015State_Selected = 1, eGUI2015State_Locked = 2 } enGUI2015Selector;

void GUI2015_SetForeColor(u32 clr);
void GUI2015_SetBackColor(u32 clr);
void GUI2015_SetWidth(u8 w);
void GUI2015_SetFill(u8 val);

//
void GUI2015_DRAW_Rect(const stGUI2015Rect *pRect);
void GUI2015_DRAW_RectIdx(int idx);
void GUI2015_DRAW_Label(const stGUI2015Label *pLabel);
void GUI2015_DRAW_Button(const stGUI2015Button *pButton, enGUI2015Selector flags);
void GUI2015_DRAW_CheckBox( const stGUI2015Checkbox *pBox, enGUI2015Selector flags);
void GUI2015_DRAW_Graph(const stGUI2015Graph *pGraph);
void GUI2015_DRAW_Window(const stGUI2015Window * pWindow);

#endif /* GUI2015INC_GRAPH_H_ */
