// Occ.cpp: implementation of the Occ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mirag_Vvod.h"
#include "Abonent.h"
#include "T90_plus.h"
#include "deperem.h"
#include "decode.h"
#include "dif_dec.h"
#include "des2_v35.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbonent::CAbonent()
{
Num_kan=2;
}

CAbonent::~CAbonent()
{

}

int CAbonent::init(BYTE Num)
{
	if(Num==0)
		 simv_desdvig1=0;
	else simv_desdvig2=0;
/*   	switch(Num){
	case 0: simv_desdvig1=0; memset(rez1,0,12);
           next1=0;
		   registr1=0l;
		   break;
    case 1: simv_desdvig2=0; memset(rez2,0,12);
           next2=0;
		   registr2=0l;
		   break;
	 }*/
	return 0;
}

BOOL CAbonent::Demultiplex(BYTE *buf, int len)  // функция разделения потока на 2 подпотока
{

int  k=0,
	 i=0;

   if(Num_kan==2)
   {
                              
	 while(i<(len>>3))
	 {
       if((buf[2+8*i]>1)&&(buf[6+8*i]>1))  // проверяем наличие данных в обоих каналах
	   {
           Num_kan=0;     
           break;                          // выходим если есть данные
 	   }
	   i++;
	 }
   };

    if(Num_kan==0)
	{
    while(i<(len>>3))
	{
		if(buf[8*i]==0) 
		{
			buf_in1[k]=buf[2+8*i];   buf_in2[k]=buf[6+8*i];  // порядок каналов в потоке 0k-1k-0k-1k
			k++;
			buf_in1[k]=buf[3+8*i];   buf_in2[k]=buf[7+8*i];   
			k++;
		}
		if(buf[8*i]==1)                                      // порядок каналов в потоке 1k-0k-1k-0k
		{
			buf_in1[k]=buf[6+8*i];   buf_in2[k]=buf[2+8*i];
			k++;
			buf_in1[k]=buf[7+8*i];   buf_in2[k]=buf[3+8*i];
			k++;
		}
		if(buf[8*i]>1) ::MessageBeep(0);
			i++;
	}
	SizeBuf_kan=k;
//	fout1.Write(buf_in1,SizeBuf_kan);
//	fout2.Write(buf_in2,SizeBuf_kan);
	return TRUE;
	}	

   
return FALSE;
}
 

BOOL CAbonent::synchro_search(BYTE * buf_in, int lenb,BYTE Num)          //поиск синхронизации
{

unsigned int l,i;
    
		// Генерируем последовательности для определения фазы декодера
		// Используется операция сдвига sdvig(BYTE*,int,unsigned i)
init(Num);
	    
  turn_faza=T_0;
  prev_shift=0;
  for (i=0; i<16; i++)                      // Исходный + 16 сдвигов
  {
	memcpy(massiv[i],buf_in,lenb);
	l=sdvig(massiv[i], lenb, i);
	deperem(massiv[i],buf_deper,lenb);
	for(int j=0;j<4;j++)
	{
    sdvig(buf_deper,lenb,j); 
	if(Num==0) Variant1=VAR_A; else Variant2=VAR_A;
	if(ch_sind(buf_deper,lenb,Num))
		 { 
		  if(Num==0) turn_faza1=T_0; else turn_faza2=T_0;
          if(Num==0) prev_shift1=0;  else prev_shift2=0;
		  if(Num==0) reg_shift1=j; else reg_shift2=j;
		  if(Num==0) sdwig1=i;     else sdwig2=i;
          return TRUE;
		 } 
	if(Num==0) Variant1=VAR_B; else Variant2=VAR_B;
	if(ch_sind(buf_deper,lenb,Num))
		 { 
		  if(Num==0) turn_faza1=T_0; else turn_faza2=T_0;
          if(Num==0) prev_shift1=0;  else prev_shift2=0;
		  if(Num==0) reg_shift1=j; else reg_shift2=j;
		  if(Num==0) sdwig1=i;     else sdwig2=i;
		  return TRUE;
		 } 
	}
  }
  
    turn_faza=T_90;               
	turn_faza_90(buf_in, buf_out, lenb, turn_faza); 
 	for (i=0; i<16; i++)                           // поворот на 90 + 16 сдвигов
	{
	 memcpy(massiv[i+16],buf_out,lenb);	
	 l=sdvig(massiv[i+16],lenb,i);
	 deperem(massiv[i+16],buf_deper,lenb);
	 for(int j=0;j<4;j++)
	 {
        sdvig(buf_deper,lenb,j); 
		if(Num==0) Variant1=VAR_A; else Variant2=VAR_A;
		if(ch_sind(buf_deper,lenb,Num))
		{
         if(Num==0) turn_faza1=T_90; else turn_faza2=T_90;
         if(Num==0) prev_shift1=0;  else prev_shift2=0;
         if(Num==0) reg_shift1=j; else reg_shift2=j;
		 if(Num==0) sdwig1=i;     else sdwig2=i;
     	 return TRUE;
		}
		if(Num==0) Variant1=VAR_B; else Variant2=VAR_B;
	    if(ch_sind(buf_deper,lenb,Num))
		{
		 if(Num==0) turn_faza1=T_90; else turn_faza2=T_90;
         if(Num==0) prev_shift1=0;  else prev_shift2=0;	
         if(Num==0) reg_shift1=j; else reg_shift2=j;
		 if(Num==0) sdwig1=i;     else sdwig2=i;
         return TRUE;
		} 
	 }
	}
    
	memcpy(buf_temp,buf_in, lenb);
	sdvig(buf_temp, lenb, 1)                      ;	// сдвиг на 1 бит, поворот на 90
	turn_faza_90(buf_temp,buf_out,lenb, turn_faza);   // + 16 сдвигов
	prev_shift=1;
	for (i=0; i<16; i++)
	{                            			     	      
        memcpy(massiv[i+32],buf_out, lenb);
		l=sdvig(massiv[i+32],lenb, i);
		deperem(massiv[i+32],buf_deper,lenb);
		for(int j=0;j<4;j++)
		{
        sdvig(buf_deper,lenb,j); 
		if(Num==0) Variant1=VAR_A; else Variant2=VAR_A;
		if(ch_sind(buf_deper,lenb,Num))
		{
         if(Num==0) turn_faza1=T_90; else turn_faza2=T_90;
         if(Num==0) prev_shift1=1;  else prev_shift2=1;
	     if(Num==0) reg_shift1=j; else reg_shift2=j;
		 if(Num==0) sdwig1=i;     else sdwig2=i;
		 return TRUE;
		}
		if(Num==0) Variant1=VAR_B; else Variant2=VAR_B;
	    if(ch_sind(buf_deper,lenb,Num))
		 { 
          if(Num==0) turn_faza1=T_90; else turn_faza2=T_90;
          if(Num==0) prev_shift1=1;  else prev_shift2=1;
          if(Num==0) reg_shift1=j; else reg_shift2=j;
		  if(Num==0) sdwig1=i;     else sdwig2=i;
          return TRUE;
		 } 
		}
	}

return FALSE;
}

BYTE CAbonent::ch_sind(BYTE*buf_inp, unsigned int len,BYTE Num)
{
unsigned int	i,j,k,l;
double nu=0,ed=0;

BYTE reg=0,*Buf_temp;

static unsigned int mask_a[18]={0x01, 0x02, 0x10, 0x40, 0x80, 0x200, 0x400,
				    0x1000, 0x2000, 0x10000, 0x40000, 0x100000,
				    0x200000, 0x800000, 0x1000000, 0x2000000,
				    0x4000000, 0x8000000},
			mask_b[18]={0x01, 0x02, 0x20, 0x40, 0x80, 0x100, 0x800,
				    0x1000, 0x2000, 0x20000, 0x80000, 0x100000,
				    0x200000, 0x400000, 0x1000000, 0x2000000,
				    0x4000000, 0x8000000};
unsigned int registr=0x00000000;

Buf_temp=new BYTE[len<<1];

        if(Num==0) Variant=Variant1; else Variant=Variant2; //разделение подканалов

        if(Variant==VAR_A)     //Вариант "а"
		for (i=0, l=0; i<len; i++)
			for (j=0; j<5; j+=4, l++) {
				registr=(registr<<4)|((buf_inp[i]>>j)&0xF);
				for (k=0; k<18; k++)
					if (registr&mask_a[k])
						reg^=1;
				Buf_temp[l]=reg;
				reg=0;
			}
   
    	if(Variant==VAR_B)  //Вариант "б"
		for (i=0, l=0; i<len; i++)
			for (j=0; j<5; j+=4, l++) {
				registr=(registr<<4)|((buf_inp[i]>>j)&0xF);
				for (k=0; k<18; k++)
					if (registr&mask_b[k])
						reg^=1;
				Buf_temp[l]=reg;
				reg=0;
			}
    j=0;
	for (i=0 ; i<l; ++i)
		if (!Buf_temp[i])
			++j;
	nu=j;
	ed=l-j;
	if ((!nu)||((ed/nu)>0.01)) ;
	else
	{
		delete [] Buf_temp;
		return 1;	//фаза верна
	}

    delete [] Buf_temp;
	return 0; //фаза неверна

}

                 //функция обработки входного потока(синхронизация и декодирование)

BOOL CAbonent::Obrabotka(BYTE *buf_inp,int lenb,BYTE Num)
{

    int i,s,m;

	if(Num==0) sdwig=sdwig1; else sdwig=sdwig2;
	if(Num==0) simv_desdvig=simv_desdvig1; else simv_desdvig=simv_desdvig2;
    if(Num==0) reg_shift=reg_shift1;    else reg_shift=reg_shift2;
    if(Num==0) turn_faza=turn_faza1;    else turn_faza=turn_faza2;
    if(Num==0) prev_shift=prev_shift1;  else prev_shift=prev_shift2;
    if(Num==0){
	             buf_save[0]=buf_save1[0];
                 buf_save[1]=buf_save1[1];
	} else {
                 buf_save[0]=buf_save2[0];
                 buf_save[1]=buf_save2[1];
	}
    
	if((sdwig+=reg_shift)>=16) sdwig=sdwig-16; //учитываем сдвиг для нимбов
    s=sdwig;    	
	if(prev_shift==1) {
     desdvig(buf_inp,lenb,simv_desdvig,Num); //сдвигаем поток вправо на 1 бит
	 s+=2;			         // +2 сдвига
		if (s==16)
			s=0;
		else
			if (s==17)
				s=1;
	}
            turn_faza_90(buf_inp, buf_out, lenb, turn_faza);
           
			if(s==0); else
			if(s<8)
			{
             buf_inp[0]=buf_save[0];
    		 buf_inp[1]=(buf_save[1])|(buf_out[0]<<(8-s));
			 for (i=0; i<lenb-2; i++)
			 buf_inp[i+2]=(buf_out[i]>>s)|(buf_out[i+1]<<(8-s));
			 buf_save[0]=(buf_out[lenb-2]>>s)|(buf_out[lenb-1]<<(8-s));
			 buf_save[1]=buf_out[lenb-1]>>s;
			 memcpy(buf_out, buf_inp, lenb);
			}
            if(s==8) 
			{
             buf_inp[0]=buf_save[0];
    		 memcpy(&(buf_inp[1]), buf_out, lenb-1);
			 buf_save[0]=buf_out[lenb-1];
			 memcpy(buf_out, buf_inp, lenb);
			}
            if(s>8)
			{
             buf_inp[0]=(buf_save[0])|(buf_out[0]<<(16-s));
			 for (i=0; i<lenb-1; i++)
			 buf_inp[i+1]=(buf_out[i]>>(s-8))|(buf_out[i+1]<<(16-s));
			 buf_save[0]=buf_out[lenb-1]>>(s-8);
			 memcpy(buf_out, buf_inp, lenb);
			}
			if(Num==0) memcpy(buf_save1,buf_save,2);
			if(Num==1) memcpy(buf_save2,buf_save,2);

            deperem(buf_out, buf_deper, lenb);  // cнимаем перемежение

			if(!ch_sind(buf_deper,400,Num))   // проверяем наличие синхронизации 
			{ 
				if(Num==0 ) {buf_save1[0]=0; buf_save1[1]=0;}
				else {buf_save2[0]=0; buf_save2[1]=0;}
				return FALSE; 
			}
    	    m=decode(buf_deper,buf_dec,lenb,Variant); // декодируем
			dif_decoder(buf_dec,buf_dif,m); // диф.декодируем
            descrembler_V35(buf_dif,buf_dskr,m); // дескремблируем
       		if(Num==0) fout1.Write(buf_dskr,m); // записываем в файл
			else fout2.Write(buf_dskr,m);

return TRUE;

}


int CAbonent::sdvig(BYTE *buf, int len , int s)  // операция сдвига потока 
{                                           // в конце блока на месте <s> сдвинутых бит нули
	int n;
	if(s<=8)
	   for(n=0; n<len; n++) 
	      if(n<(len-1)) buf[n]=((buf[n]>>s)|(buf[n+1]<<(8-s)));
          else buf[n]>>=s;

	if(s>8)
       for(n=0; n<len; n++) 
	       if(n<(len-2)) buf[n]=((buf[n+1]>>(s-8))|(buf[n+2]<<(16-s))); 
		   else
		   {
		   if(n==(len-2)) buf[n]>>=(s-8); else buf[n]=0;
		   }
	          
return n;	
}


 int CAbonent::desdvig(BYTE *buf, int len,BYTE simv,BYTE Num)
{
int	i;
unsigned char simv_p;
    if(Num==0) simv_desdvig1=(((buf[len-1])>>7)&1);
	if(Num==1) simv_desdvig2=(((buf[len-1])>>7)&1);
	for (i=0; i<len; i++) {
		simv_p=(buf[i]>>7)&1;
		buf[i]=((buf[i]<<1)&0xFE)|(simv&1);
		simv=simv_p;
return 0;
}
return 0;
}

