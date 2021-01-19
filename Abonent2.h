// Abonent2.h: interface for the CAbonent2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABONENT2_H__784DE3B3_63D3_11D5_AD55_00A0CCD50301__INCLUDED_)
#define AFX_ABONENT2_H__784DE3B3_63D3_11D5_AD55_00A0CCD50301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAbonent2  // класс для обработки потока с НСК 1/2
{
public:
	int decoder(BYTE*,int,BYTE);
	BOOL Demultiplex(BYTE*buf,int len);
	BYTE Variant1,Variant2,Variant;
	BYTE simv_desdvig,simv_desdvig1,simv_desdvig2;
	BYTE buf_save[2],
		 buf_save1[2],
		 buf_save2[2],
		 buf_saveG728[2][2],
		 buf_saveG726[2],
		 m_MaxSynIndex[2];
	desdvig(BYTE*,int,BYTE,BYTE Num);
	CFile fout1,fout2,fout;
    BYTE *buf_temp,*buf_out,*buf_deper,*buf_dec,*buf_dif,*buf_dskr;  
    BYTE*massiv[112];
	BYTE prev_shift,prev_shift1,prev_shift2;
	BYTE sdwig,sdwig1,sdwig2;
	BYTE reg_decode,reg_decode1,reg_decode2;
	BOOL Obrabotka(BYTE*,int,BYTE Num);
	unsigned char turn_faza,turn_faza1,turn_faza2;
    int sdvig(BYTE*buf, int len,int i);
	int Num_kan;
	BOOL synchro_search(BYTE*buf_in,int lenb,BYTE Num);
	BYTE* buf_in1,*buf_in2;
	BOOL FlagG728,SynchroG728[2],
		 FlagG726,SynchroG726[2];
	unsigned int SizeBuf_kan; 
	CAbonent2();
	virtual ~CAbonent2();

protected:
	void SdvigG728(int , UINT);
	void SinchroSearhG728(int);
	void SdvigG726(int ,UINT);
};

#endif // !defined(AFX_ABONENT2_H__784DE3B3_63D3_11D5_AD55_00A0CCD50301__INCLUDED_)
