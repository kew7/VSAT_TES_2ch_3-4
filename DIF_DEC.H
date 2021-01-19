BYTE	mask_bit[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
//BYTE reg_d_c1=0,reg_d_c2=0;
void dif_decoder(unsigned char *buf_inp, unsigned char *buf_out, unsigned int len)
{

unsigned int	i,j;
unsigned char	registr1=0,registr=0;
    //(Num==0)?(registr=reg_d_c1):(registr=reg_d_c2);
	memset(buf_out, 0x00, len);
	for (i=0; i<len; i++)
		for (j=0; j<8; j++) {
			(buf_inp[i]&mask_bit[j])?(registr1=1):(registr1=0);
			(!(registr^registr1))?(buf_out[i]|=mask_bit[j]):(0);
			registr=registr1;
		}
	//(Num==0)?(reg_d_c1=registr):(reg_d_c2=registr);
}