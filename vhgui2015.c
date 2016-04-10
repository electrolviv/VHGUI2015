#include "vhgui2015.h"

static u8 GUIMEM[GUI2015_OBJECTS_MAX*32];
static void * OBJTBL[GUI2015_OBJECTS_MAX];
static int curptr, cnt, selected;

static int selectable[GUI2015_OBJECTS_MAX];
static int selectablecnt;

static const int tbl[eGUI2015_OBJ_LAST] = {
		0, // eGUI2015_OBJ_UNK
		sizeof(stGUI2015Window), // eGUI2015_OBJ_WINDOW
		sizeof(stGUI2015Label), // eGUI2015_OBJ_LAB
		sizeof(stGUI2015Button), // eGUI2015_OBJ_BUT
		sizeof(stGUI2015Picture), // eGUI2015_OBJ_PIC
		sizeof(stGUI2015Graph), // eGUI2015_OBJ_GRAPH
		sizeof(stGUI2015PBar), // eGUI2015_OBJ_PBAR
		sizeof(stGUI2015Checkbox) // eGUI2015_OBJ_CHBOX
};

static void * GUI2015_ObjPtr(int n) { return OBJTBL[n]; }
static enGUI2015Obj GUI2015_ObjType(int n) { void *selObjPtr = GUI2015_ObjPtr(n); int objType = *((int *)selObjPtr); return (enGUI2015Obj)objType; }
static void * GUI2015_NewPtr() { return  GUIMEM + curptr; }

void GUI2015_GetSelectedObjectFct(int *pCmd, int *pCmdArg1, int *pCmdArg2, int *pCmdArg3, enGUI2015Obj *pObjType) {

	// Skip on empty window ...
	if(selected == -1) {
		*pCmd = -1; *pObjType = eGUI2015_OBJ_UNK;
		return;
	}

	void *selObjPtr = GUI2015_ObjPtr( selectable[selected] );
	int objType = *((int *)selObjPtr);

	if(objType == eGUI2015_OBJ_BUT) {
		stGUI2015Button *btn = (stGUI2015Button *)selObjPtr;
		*pCmd = btn->iCmdType; *pCmdArg1 = btn->iCmdArg1; *pCmdArg2 = btn->iCmdArg2; *pCmdArg3 = btn->iCmdArg3;
		*pObjType = eGUI2015_OBJ_BUT;
		return;

	} else if(objType == eGUI2015_OBJ_CHBOX) {
		stGUI2015Checkbox *chk = (stGUI2015Checkbox *)selObjPtr;
		*pCmd = chk->iCmdType; *pCmdArg1 = chk->iCmdArg1; *pCmdArg2 = chk->iCmdArg2; *pCmdArg3 = chk->iCmdArg3;
		*pObjType = eGUI2015_OBJ_CHBOX;
		return;
	}

	*pCmd = -1;
	*pObjType = eGUI2015_OBJ_UNK;
}

void * GUI2015_GetSelectedObjectPtr() {
	return  GUI2015_ObjPtr( selectable[selected] );
}

void GUI2015_ResetContent() {
	GUI2015_Rect_ResetContent(); // Important !
	cnt = 0; curptr = 0; selectablecnt = 0; selected = -1;
}

void GUI_Assert(int casef) {
	if(!casef) {
		while(1) { Sleep(10); }
	}
}

int GUI2015_AddObj(void *ptr) {
	int iType = *((int *)ptr);
	int l;
	int inserted = cnt;

	GUI_Assert(cnt<GUI2015_OBJECTS_MAX);
	GUI_Assert(iType<eGUI2015_OBJ_LAST);
	l = tbl[iType];
	GUI_Assert(l);
	OBJTBL[cnt] = GUI2015_NewPtr();
	vhmemcpy((void *)OBJTBL[cnt], ptr, l );

	// Put some rectangles to the stack
	if(iType == eGUI2015_OBJ_WINDOW) {
		stGUI2015Rect rc,tmp,rw,rd;
		const stGUI2015Window * pWnd = (const stGUI2015Window *)ptr;

		GUI2015_RECT_SplitY( GUI2015_Rect_GetRef(pWnd->idxRect), &rc, &tmp, 15);
		GUI2015_Rect_Add(&rc);

		GUI2015_RECT_SplitY(&tmp, &rw, &rd, 80);
		GUI2015_Rect_Add(&rw);
		GUI2015_Rect_Add(&rd);

	} else if(iType == eGUI2015_OBJ_BUT) { selectable[selectablecnt]=cnt; selectablecnt++;
	} else if(iType == eGUI2015_OBJ_CHBOX) { selectable[selectablecnt]=cnt; selectablecnt++;
	} else if(iType == eGUI2015_OBJ_GRAPH) {
		VHSleep(0);
	}

	cnt++; curptr += l;
	return inserted;
}

int GUI2015_ObjsCnt() { return cnt; }

// Cut / leave only specefied counts in stack
void GUI2015_CutoffObjects(int newidx) {
// 	if(newidx>= cnt) { return; }
//	curptr = (int)OBJTBL[newidx] - (int)GUIMEM;
// cnt = newidx;

	while(cnt>newidx) {
		cnt--;
		curptr = (int)OBJTBL[cnt] - (int)GUIMEM;
		GUI2015_Rect_RemoveLast();
	}

	selectablecnt = 0;

}

void GUI2015_DrawWindowContentFrom(int startidx) {

	if(selectablecnt) { selected = 0; } // any available button ?
	else selected = -1;

	int i;
	for(i=startidx; i<cnt;i++) { GUI2015_DrawObject( GUI2015_ObjPtr(i) , selectable[selected] == i ? eGUI2015State_Selected : eGUI2015State_Normal ); }
}

void GUI2015_DrawWindowContent() {
	GUI2015_DrawWindowContentFrom(0);
}

void GUI2015_UpdateWindowContent() {
	int i;

	for(i=0;i<cnt;i++) {
		void *ptr = GUI2015_ObjPtr(i);
		int iType = *((int *)ptr);

		switch( iType ) {
			case eGUI2015_OBJ_LAB: 	if( ((stGUI2015Label *)ptr)->forceRedraw) { GUI2015_DRAW_Label((const stGUI2015Label *)ptr); } 	break;
			case eGUI2015_OBJ_GRAPH: 	if( ((stGUI2015Graph *)ptr)->forceRedraw) { 	GUI2015_DRAW_Graph((const stGUI2015Graph *)ptr); 	} 	break;
			default: { }
		}
	}

}

static int FindClosest(int mode) {
	stGUI2015Button *pbtn = (stGUI2015Button *)GUI2015_ObjPtr( selectable[selected]);

	const stGUI2015Rect * recto = GUI2015_Rect_GetRef(pbtn->idxRect);
	int bestd = -1;
	int besti = selected;
	int i;

	for(i=0;i<selectablecnt;i++)
	{
		int newd = -1;

		if(i!= selected)
		{
				stGUI2015Button *pcurbtn = (stGUI2015Button *)GUI2015_ObjPtr( selectable[i]);
				const stGUI2015Rect * rectc = GUI2015_Rect_GetRef(pcurbtn->idxRect);

				switch(mode) {
					case 2: if(rectc->sPoint2.y <= recto->sPoint1.y) { newd = GUI2015_RectDistanceQ(recto,rectc); } break; // up
					case 4: if(rectc->sPoint2.x <= recto->sPoint1.x) { newd = GUI2015_RectDistanceQ(recto,rectc); } break; // left
					case 6: if(rectc->sPoint1.x >= recto->sPoint2.x) { newd = GUI2015_RectDistanceQ(recto,rectc); } break; // right
					case 8: if(rectc->sPoint1.y >= recto->sPoint2.y) { newd = GUI2015_RectDistanceQ(recto,rectc); } break; // down
					default: { }
				}
		}

		// Select closest
		if(newd != -1) { 
			if(bestd == -1) { bestd = newd; besti = i; }
			else { if(newd<bestd) {bestd = newd; besti = i; } }
		}
	}

	return besti;
}

void GUI2015_Event(enGUI2015Event evt) {

	int bestfit = -1;

	switch(evt)
	{
		case eGUI2015Event_LEFT:		bestfit = FindClosest(4); break;
		case eGUI2015Event_RIGHT:	bestfit = FindClosest(6); break;
		case eGUI2015Event_UP:		bestfit = FindClosest(2); break;
		case eGUI2015Event_DOWN:	bestfit = FindClosest(8); break;

		case eGUI2015Event_B1ON: {
			if(selected != -1) {
				int objn = selectable[selected];
				enGUI2015Obj objtype = GUI2015_ObjType(objn);

				if(objtype == eGUI2015_OBJ_CHBOX) {
					void *objptr = GUI2015_ObjPtr(objn);
					stGUI2015Checkbox *chk = (stGUI2015Checkbox *)objptr;
					// chk->val ^= 1;
                                        if(! chk->val) chk->val = 1; else chk->val = 0;
					GUI2015_DrawObject( objptr,  eGUI2015State_Selected );
				} else if(objtype == eGUI2015_OBJ_BUT) {
					void *objptr = GUI2015_ObjPtr(objn);
					stGUI2015Button *p = (stGUI2015Button *)objptr;
					if(p->attr & eGUI2015Button_Locked) {  p->attr &= ~ eGUI2015Button_Locked; } else { p->attr |= eGUI2015Button_Locked; }
					GUI2015_DrawObject( objptr,  eGUI2015State_Selected );
				}
			}
		}
		break;

		case eGUI2015Event_B1OFF:	{ } break;
		case eGUI2015Event_B2ON: { } break;
		case eGUI2015Event_B2OFF: { } break;

		default: { }
	}

	if(bestfit != -1) {
		// Draw Previous
		GUI2015_DrawObject( GUI2015_ObjPtr( selectable[selected] ) ,  eGUI2015State_Normal );

		// Draw next
		selected = bestfit;
		GUI2015_DrawObject( GUI2015_ObjPtr( selectable[selected] ) , eGUI2015State_Selected );
	}

}
