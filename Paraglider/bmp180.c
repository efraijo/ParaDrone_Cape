#include "bmp180.h"
#include <linux_glue.h>

BMPCtl CtlReg; 
BMPCalib Calib_param;

unsigned char bmp_get_id()
{
	unsigned char data;
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_DID,1,&data);
	return data;
}

unsigned char bmp_get_ver()
{
	unsigned char data;
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_VER,1,&data);
	return data;	
}

BMPCtl bmp_get_ctl_reg()
{
	BMPCtl result;
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_CTL,1,&result);
	return result;
}

void bmp_reset()
{
	unsigned char command = BMP_RESET;
	i2c_write(BMP_DEVICE_ADDR,BMP_REG_RST,1,&command);
	delay_ms(1000);
}

int bmp_init()
{
	unsigned char tmp = bmp_get_id();
	if(tmp!=BMP_DEVICE_ID)
	{
		printf("Unidentified device ID!\n");
		return -1;
	}
	Calib_param = bmp_get_calib();
	printf("Valid device ID: %hu\n",tmp);
	tmp = bmp_get_ver();
	printf("Device version: %hu\n",tmp);
	//CtlReg = bmp_get_ctl_reg();
	printf("OSS: %u\n",CtlReg.bit.OSS);
	
	printf("Calibration parameters:\n");
	printf("AC1: %hi AC2: %hi AC3 %hi\n",Calib_param.ac1,Calib_param.ac2,Calib_param.ac3);
	printf("AC4: %hu AC5: %hu AC6 %hu\n",Calib_param.ac4,Calib_param.ac5,Calib_param.ac6);
	printf("B1: %hi B2: %hi\n",Calib_param.b1,Calib_param.b2);
	printf("MB: %hi MC: %hi MD %hi\n",Calib_param.mb,Calib_param.mc,Calib_param.md);
	return 0;
}

BMPCalib bmp_get_calib()
{
	BMPCalib result;
	unsigned char data[BMP_CALIB_DATA_SIZE];
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_AC1_MSB,BMP_CALIB_DATA_SIZE,data);
	result.ac1 = (short)((((unsigned short)((unsigned short)data[0]))<< 8)| data[1]);
	result.ac2 = (short)((((unsigned short)((unsigned short)data[2]))<< 8)| data[3]);
	result.ac3 = (short)((((unsigned short)((unsigned short)data[4]))<< 8)| data[5]);
	result.ac4 = (unsigned short)((((unsigned short)((unsigned short)data[6]))<< 8)| data[7]);
	result.ac5 = (unsigned short)((((unsigned short)((unsigned short)data[8]))<< 8)| data[9]);
	result.ac6 = (unsigned short)((((unsigned short)((unsigned short)data[10]))<< 8)| data[11]);
	result.b1  = (short)((((unsigned short)((unsigned short)data[12]))<< 8)| data[13]);
	result.b2  = (short)((((unsigned short)((unsigned short)data[14]))<< 8)| data[15]);
	result.mb  = (short)((((unsigned short)((unsigned short)data[16]))<< 8)| data[17]);
	result.mc  = (short)((((unsigned short)((unsigned short)data[18]))<< 8)| data[19]);
	result.md  = (short)((((unsigned short)((unsigned short)data[20]))<< 8)| data[21]);
	return result;
}

long bmp_read_temp()
{
	unsigned char command = BMP_CTL_TEMP;
	i2c_write(BMP_DEVICE_ADDR,BMP_REG_CTL,1,&command);	
	delay_ms(BMP_TEMP_CONVERSION_TIME);
	unsigned char data[2];
	i2c_read(BMP_DEVICE_ADDR,BMP_REG_MSB,2,data);
	long result;
	result = (long)(((long)(data[0]))<<8)|(data[1]);
	return result;
}

long bmp_read_press()
{
	long result;
	if(CtlReg.bit.OSS==0)
		{
			unsigned char command = BMP_CTL_PRE0+(CtlReg.bit.OSS<<6);
			i2c_write(BMP_DEVICE_ADDR,BMP_REG_CTL,1,&command);
			delay_ms(BMP_2MS_DELAY_U8X+BMP_3MS_DELAY_U8X);
			unsigned char data[3];
			i2c_read(BMP_DEVICE_ADDR,BMP_REG_MSB,3,data);
			result = (long)(((((long)(data[0]))<<16)|(((long)(data[1]))<<8)|((long)(data[0])))>>(8-CtlReg.bit.OSS));			
		}
	else
		{
			/*Oversample*/
		}
	return result;
}

long bmp_calc_temp(long data)
{
	long result = 0;
	long x1, x2 = 0;
	x1 = (( data -	(long) Calib_param.ac6) * (long) Calib_param.ac5)>> 15;
	if (x1 == 0 && Calib_param.md == 0)
		return 0; //Invalid data
	x2 = ((long) Calib_param.mc << 11) /(x1 + Calib_param.md);
	Calib_param.b5 = x1 + x2;
	result = (long)((Calib_param.b5+8)>>4);
	return result;
}

long bmp_calc_press(long data)
{
	long result = 0;
	long x1,x2,x3,b3,b6 = 0;
	unsigned long b4,b7 = 0;
	b6 = Calib_param.b5 - 4000;
	
	x1 = (b6 * b6)>>12;
	x1 *= Calib_param.b2;
	x1 >>= 11;
	x2 = (Calib_param.ac2 * b6);
	x2 >>= 11;
	
	x3 = x1 + x2;
	
	b3 = (((((long)Calib_param.ac1)*4 + x3) << CtlReg.bit.OSS) + 2) >> 2; 
	
	x1 = (Calib_param.ac3 * b6)>>13;
	x2 = (Calib_param.b1 * ((b6 * b6)>>12))>>16;
	x3 = (x1 +x2 +2)>>2;
	b4 = (Calib_param.ac4*(unsigned long)(x3+32768))>>15;
	b7 = ((unsigned long)(data - b3) * (50000 >> CtlReg.bit.OSS));
	
	if(b7 < 0x80000000)
	{
		if (b4 != 0)
			result = (b7 << 1)/b4;
		else
			return 0; //Invalid data
	}
	else 
	{
		if(b4 != 0)
			result = (b7 / b4)<<1;
		else 
			return 0; //Invalid data
	}
	x1 = result >>8;
	x1 *= x1;
	x1 = (x1 * 3038)>>16;
	x2 = (result * (-7357))>>16;
	result += (x1 + x2 + 3791)>>4;
	return result;
}