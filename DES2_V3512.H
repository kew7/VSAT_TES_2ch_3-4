#define	MASKA_DES       0x1FFFFFl

static ULONG registr_descr,registr_descr1,registr_descr2;

void descrembler_V35_12(unsigned char *buf_inp, unsigned char *buf_out, unsigned int len)
{
unsigned int	i,j;
	memset(buf_out, 0x00, len);
	for (i=0; i<len; i++)
		for (j=0; j<8; j++) {
			if ( (buf_inp[i]&mask_bit[j]) )
				registr_descr|=1;
			if ( ((registr_descr&1)^
			     ((registr_descr>>3)&1)^
			     ((registr_descr>>20)&1)) )
				buf_out[i]|=mask_bit[j];
			registr_descr=(registr_descr<<1)&MASKA_DES;
		}
}