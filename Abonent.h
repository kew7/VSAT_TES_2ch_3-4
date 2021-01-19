// Occ.h: interface for the Occ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCC_H__875EAC26_FC0E_11D4_AB61_00A0CCD50301__INCLUDED_)
#define AFX_OCC_H__875EAC26_FC0E_11D4_AB61_00A0CCD50301__INCLUDED_

//#include "Mirag_VvodDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LENG_SHAB 400
#define VAR_A   0
#define VAR_B   1
#define T_0   0
#define T_90  1
#define T_180 2
#define T_270 3

class CAbonent
{
public:
	BOOL Demultiplex(BYTE*buf,int len);
	BYTE Variant1,Variant2,Variant;
	BYTE simv_desdvig,simv_desdvig1,simv_desdvig2;
	BYTE buf_save[2],buf_save1[2],buf_save2[2];
	int desdvig(BYTE*,int,BYTE,BYTE Num);
	int init(BYTE);
	CFile fout1,fout2,fout;
    BYTE *buf_temp,*buf_out,*buf_deper,*buf_dec,*buf_dif,*buf_dskr;  
    BYTE*massiv[112];
	BYTE reg_shift,reg_shift1,reg_shift2;
	BYTE prev_shift,prev_shift1,prev_shift2;
	BYTE sdwig,sdwig1,sdwig2;
	BYTE reg_decode;
	BOOL Obrabotka(BYTE*,int,BYTE Num);
	unsigned char turn_faza,turn_faza1,turn_faza2;
    int sdvig(BYTE*buf, int len,int i);
	int Num_kan;
	BOOL synchro_search(BYTE*buf_in,int lenb,BYTE Num);
    BYTE ch_sind(BYTE*, unsigned int,BYTE Num);
	BYTE* buf_in1,*buf_in2;
	unsigned int SizeBuf_kan; 
	CAbonent();
	virtual ~CAbonent();

protected:

};

#endif // !defined(AFX_OCC_H__875EAC26_FC0E_11D4_AB61_00A0CCD50301__INCLUDED_)
