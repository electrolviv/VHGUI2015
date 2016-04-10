#include "fonts.h"

#include "font1.h"
#include "font2.h"
#include "font3.h"

#define PLVER1

#ifndef PLVER
#include "cyr/font4.h"
#include "cyr/font5.h"
#include "cyr/font6.h"
#include "cyr/font7.h"
#include "cyr/font8.h"
#include "cyr/font9.h"
#endif

#ifdef PLVER
#include "pl/fnt4.h"
#include "pl/font7.h"
#endif

// Bitfield fonts: 0,1,2
// Bytefield fonts: 3,4 ... N
#define uint16 u16

const u8 *pPTR[3] = { &arrayDATAFONT1[0], &arrayDATAFONT2[0], &arrayDATAFONT3[0] };

// const u8 *pBYTEFNTS[] = { fnt4_bf2, 	fnt4_bf2, 	fnt4_bf2, fnt_Lucida_Console_13_bf2, fnt4_bf2, fnt4_bf2 };
const u8 *pBYTEFNTS[] = {
		fnt_Calibri_19_bf2, 	fnt_Candara_21_bf2, 	fnt_Consolas_15_bf2,
		fnt_Lucida_Console_13_bf2, fnt_Tahoma_13_bf2, 	fnt_Tahoma_21_bf2
};

u8 FONT_GET_SYM_T( u8 ucSYM, u8 ucFONT ) {
	u8 r;
	if(ucFONT>=3) {
		stBFN2CHR *pCHR = (stBFN2CHR *)(pBYTEFNTS[ucFONT-3] + sizeof(stBFN2HDR)); r = pCHR[ucSYM].t;
	} else {
		const u8 *pFONT = pPTR[ucFONT];
		r = *(pFONT + 2 + ucSYM );
	}
	
	return r;
}

u8 FONT_GET_SYM_W( u8 ucSYM, u8 ucFONT ) {
	u8 r;

	if(ucFONT>=3) {
		stBFN2CHR *pCHR = (stBFN2CHR *)(pBYTEFNTS[ucFONT-3] + sizeof(stBFN2HDR));
		r = pCHR[ucSYM].w;
	} else {
		const u8 *pFONT = pPTR[ucFONT];
		r = *(pFONT + 2 + ucSYM + 256);
	}
	
	return r; 
}

u8 FONT_GET_SYM_H( u8 ucSYM, u8 ucFONT ) {
	u8 r;

	if(ucFONT>=3) {
		stBFN2CHR *pCHR = (stBFN2CHR *)(pBYTEFNTS[ucFONT-3] + sizeof(stBFN2HDR));
		r = pCHR[ucSYM].h;
	} else {
		const u8 *pFONT = pPTR[ucFONT];
		r = *(pFONT + 2 + ucSYM + (256*2) );
	}
	return r; 
}

u32 FONT_GET_SYM_O( u8 ucSYM, u8 ucFONT ) {
 
	u32 uiOFFSET;

	if(ucFONT<3) {
		const u8 *pFONT = pPTR[ucFONT];
		u8 ucOFFSET1 = *( pFONT + 2 + (ucSYM*2) + (256*3) + 0 );
		u8 ucOFFSET2 = *( pFONT + 2 + (ucSYM*2) + (256*3) + 1 );
		uiOFFSET = (((uint16)ucOFFSET2)<<8) + ucOFFSET1;
	} else {
		stBFN2CHR *pCHR = (stBFN2CHR *)(pBYTEFNTS[ucFONT-3] + sizeof(stBFN2HDR));
		uiOFFSET = pCHR[ucSYM].offs;
	}

 return uiOFFSET;
}

// BYTE
u8 FONT_GET_SYM_B( u32 uiOFFSET, u8 ucFONT ) {
	u8  r;

	if( ucFONT<3) {
		const u8 *pFONT = pPTR[ucFONT]; r = *(pFONT + uiOFFSET);
	} else {
		const u8 *pFONT = pBYTEFNTS[ucFONT-3]; pFONT += uiOFFSET; r = *pFONT;
	}
	
	return r;
}

void FONT_GETSYMPROP(u8 sym, u8 font, stFontSymProp * pResult) {
	pResult->uiOFFSET = FONT_GET_SYM_O(sym,font);
    pResult->ucSYMWIDTH = FONT_GET_SYM_W(sym,font);
    pResult->ucSYMHEIGHT = FONT_GET_SYM_H(sym,font);
    pResult->ucTOP = FONT_GET_SYM_T(sym,font);
	
	if(font<3) {
		pResult->ucBYTESINLINE = (pResult->ucSYMWIDTH >> 3);
	 	pResult->ucBYTESINLINE+= (pResult->ucSYMWIDTH % 8) ? 1:0;
	} else {
		pResult->ucBYTESINLINE = 0;
	}

}


// TODO: Direct drawing interface

// Font located in SPI-FLASH @ addr
// Load symbol to the RAM



