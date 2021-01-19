#define	MASKA		0x1FFFFFFEl

static unsigned char nexte,nexte1,nexte2;
static unsigned long	registrP,registrP1,registrP2,
		                registrQ,registrQ1,registrQ2;

static BYTE	mask_bit[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

char decode12(BYTE *buf_inp, BYTE *buf_out, unsigned int len, BYTE reg_decode)
{
unsigned int	i,
		j,
		k,
		l=0;

double		leng,
		weight=0,WES_MET,
		numb=8;

if(len<=50) WES_MET=0.18; else WES_MET=0.04;


unsigned long	
		mask_5_P[6]={0x01l, 0x02l, 0x80l, 0x00l, 0x00l, 0x00l},
		mask_5_Q[6]={0x01l, 0x08l, 0x400l, 0x800l, 0x1000l, 0x8000l},
		mask_4_P[6]={0x100l, 0x200l, 0x800l, 0x1000l, 0x8000l, 0x00l},
		mask_4_Q[6]={0x100l, 0x4000l, 0x00l, 0x00l, 0x00l, 0x00l},
		mask_3_P[6]={0x2000l, 0x4000l, 0x10000l, 0x20000l, 0x40000l, 0x00l},
		mask_3_Q[6]={0x2000l, 0x40000l, 0x00l, 0x00l, 0x00l, 0x00l},
		mask_2_P[6]={0x80000l, 0x4000000l, 0x00l, 0x00l, 0x00l, 0x00l},
		mask_2_Q[6]={0x80000l, 0x100000l, 0x400000l, 0x2000000l, 0x4000000l, 0x00l},
		mask_1_P[6]={0x400000l, 0x1000000l, 0x10000000l, 0x00l, 0x00l, 0x00l},
		mask_1_Q[6]={0x200000l, 0x800000l, 0x8000000l, 0x10000000l, 0x00l, 0x00l};

unsigned char	mask0_0[4]={0x01, 0x04, 0x10, 0x40},
    			mask0_1[4]={0x02, 0x08, 0x20, 0x80};
unsigned char	mask1[4],
	        	mask2[4],
		        A[5]={0x00, 0x00, 0x00, 0x00, 0x00},
				rez[8],
		        rezult=0;

	if (reg_decode==1)
		for (i=0; i<4; i++) {
			mask1[i]=mask0_0[i];
			mask2[i]=mask0_1[i];
		}
	else
		for (i=0; i<4; i++) {
			mask1[i]=mask0_1[i];
			mask2[i]=mask0_0[i];
		}
	memset(buf_out, 0x00, len);
	for (i=0; i<len; i++) 
	{
		for (j=0; j<4; j++) {
			(buf_inp[i]&mask1[j])?(registrP|=1):(registrP|=0);
			(buf_inp[i]&mask2[j])?(registrQ|=1):(registrQ|=0);
			for (k=0; k<6; k++) {
				(registrP&mask_5_P[k])?(A[4]^=1):(A[4]^=0);
				(registrQ&mask_5_Q[k])?(A[4]^=1):(A[4]^=0);
				(registrP&mask_4_P[k])?(A[3]^=1):(A[3]^=0);
				(registrQ&mask_4_Q[k])?(A[3]^=1):(A[3]^=0);
				(registrP&mask_3_P[k])?(A[2]^=1):(A[2]^=0);
				(registrQ&mask_3_Q[k])?(A[2]^=1):(A[2]^=0);
				(registrP&mask_2_P[k])?(A[1]^=1):(A[1]^=0);
				(registrQ&mask_2_Q[k])?(A[1]^=1):(A[1]^=0);
				(registrP&mask_1_P[k])?(A[0]^=1):(A[0]^=0);
				(registrQ&mask_1_Q[k])?(A[0]^=1):(A[0]^=0);
			}
			for (k=0; k<5; k++)
				if ( A[k] ) {
					++rezult;
					A[k]=0x00;
				}
			switch (rezult) {
				case 0:
				case 1:	rez[nexte]=0;
					break;
				case 2: rez[nexte]=0;
					++weight;
					break;
				case 3: rez[nexte]=1;
					++weight;
					break;
				case 4:
				case 5:	rez[nexte]=1;
					break;
			}
			rezult=0;
			++nexte;
			registrP=(registrP<<1)&MASKA;
			registrQ=(registrQ<<1)&MASKA;
		}
		if (nexte==8) {
			for (j=0; j<8; j++)
			      (rez[j])?(buf_out[l]|=mask_bit[j]):(buf_out[l]|=0);
			++l;
			nexte=0;
		}
	}
	leng=len;
	leng*=numb;
	if ( (weight/leng)<=WES_MET )
		return 0;	//фаза верна
	else
		return 1;	//фаза неверна
}

void init(BYTE Num)
{
	if(Num==0)
	{
	 nexte1=0;
	 registrP1=registrQ1=0l;
	}
	if(Num==1)
	{
     nexte2=0;
	 registrP2=registrQ2=0l;
	}
	
}