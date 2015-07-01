/*BMP180 driver*/
#ifndef BMP180_H
#define BMP180_H

#include <linux/types.h>

// slave address
#define BMP_DEVICE_ADDR (0x77)
// default device ID
#define BMP_DEVICE_ID	(0x55)
// reset chip cmd
#define BMP_RESET		(0xB6)
// calibration data size 
#define BMP_CALIB_DATA_SIZE			(22)

// fixed time required for conversion 
#define BMP_TEMP_CONVERSION_TIME	(5)
#define BMP_2MS_DELAY_U8X			(2)
#define BMP_3MS_DELAY_U8X			(3)

#define Uint8 unsigned char

//Register Address
typedef enum{
	BMP_REG_XSB = 0xF8,
	BMP_REG_LSB = 0xF7,
	BMP_REG_MSB = 0xF6,
	BMP_REG_CTL = 0xF4,
	BMP_REG_RST = 0xE0,
	BMP_REG_DID = 0xD0,
	BMP_REG_VER = 0xD1,
	BMP_REG_AC1_MSB = 0xAA,
	BMP_REG_AC1_LSB = 0XAB,
	BMP_REG_AC2_MSB = 0xAC,
	BMP_REG_AC2_LSB = 0XAD,
	BMP_REG_AC3_MSB = 0xAE,
	BMP_REG_AC3_LSB = 0XAF,
	BMP_REG_AC4_MSB = 0xB0,
	BMP_REG_AC4_LSB = 0XB1,
	BMP_REG_AC5_MSB = 0xB2,
	BMP_REG_AC5_LSB = 0XB3,
	BMP_REG_AC6_MSB = 0xB4,
	BMP_REG_AC6_LSB = 0XB5,
	BMP_REG_B1_MSB = 0xB6,
	BMP_REG_B1_LSB = 0XB7,
	BMP_REG_B2_MSB = 0xB8,
	BMP_REG_B2_LSB = 0XB9,
	BMP_REG_MB_MSB = 0xBA,
	BMP_REG_MB_LSB = 0XBB,
	BMP_REG_MC_MSB = 0xBC,
	BMP_REG_MC_LSB = 0XBD,
	BMP_REG_MD_MSB = 0xBE,
	BMP_REG_MD_LSB = 0XBF
} BMPRegister;

//Measurement options
typedef enum{
	BMP_CTL_TEMP = 0x2E,
	BMP_CTL_PRE0 = 0x34,
	BMP_CTL_PRE1 = 0x74,
	BMP_CTL_PRE2 = 0xB4,
	BMP_CTL_PRE3 = 0xF4
} BMPCtlRegVal;

//Register structure
typedef struct{
	unsigned MCL:5;
	unsigned SCO:1;
	unsigned OSS:2;	
}BMPCtlRegs;

// BMP control register 
typedef union{
	BMPCtlRegs bit;
	unsigned short all;
}BMPCtl;

// calibration data structure
typedef struct{
	short 			ac1;
	short 			ac2;
	short 			ac3;
	unsigned short 	ac4;
	unsigned short 	ac5;
	unsigned short 	ac6;
	short 			b1;
	short 			b2;
	short 			mb;
	short 			mc;
	short 			md;
	int				b5;
}BMPCalib;

// get device id
unsigned char bmp_get_id();
// get device version
unsigned char bmp_get_ver();
// get control register value
BMPCtl bmp_get_ctl_reg();
// reset chip
void bmp_reset();
// get calibration data
BMPCalib bmp_get_calib();
// initialize chip
int bmp_init();
// read raw temperature
long bmp_read_temp();
// read raw pressure
long bmp_read_press();
// calculate engineer unit temperature
long bmp_calc_temp(long data);
// calculate engineer unit pressure
long bmp_calc_press(long data);
#endif /*BMP180_H*/