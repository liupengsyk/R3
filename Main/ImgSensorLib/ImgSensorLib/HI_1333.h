  
#pragma once
#include "ImgSensorLib.h"

//I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
//I2C Mode    :2:Micron 8 Addr,16Data
//I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
#define HI1333_I2C_MODE									3

#define HI1333_REG_EXP_TIM_HW							0x0073
#define HI1333_REG_EXP_TIM_H							0x0074
#define HI1333_REG_EXP_TIM_L							0x0075

#define HI1333_REG_AGAIN_H								0x0076
#define HI1333_REG_AGAIN_L								0x0077

#define HI1333_REG_DGAIN_GR_H							0x0078
#define HI1333_REG_DGAIN_GR_L							0x0079
#define HI1333_REG_DGAIN_GB_H							0x007A
#define HI1333_REG_DGAIN_GB_L							0x007B
#define HI1333_REG_DGAIN_R_H							0x007C
#define HI1333_REG_DGAIN_R_L							0x007D
#define HI1333_REG_DGAIN_B_H							0x007E
#define HI1333_REG_DGAIN_B_L							0x007F

#define HI1333_REG_OTP_CMD								0x0702	//OTP读写方向寄存器
#define HI1333_REG_OTP_WDATA							0x0706	//I2C写数据寄存器
#define HI1333_REG_OTP_RDATA							0x0708	//I2C读数据寄存器

#define HI1333_REG_OTP_ADDR_H							0x070A	//OTP地址寄存器高
#define HI1333_REG_OTP_ADDR_L							0x070B	//OTP地址寄存器低

#define HI1333_REG_MODEL_ID_H							0x0F16
#define HI1333_REG_MODEL_ID_L							0x0F17


#define __IN__
#define __OUT__

#define HI1333_OTP_RD_MODE								0x01
#define HI1333_OTP_WT_MODE								0x02


#define MSB(x)											(unsigned char)(((x) >> 8) & 0xFF)
#define LSB(x)											(unsigned char)((x) & 0xFF)


class HI1333 :
	public CImgSensorLib
{
public:
	HI1333(void);
	~HI1333(void);
	BOOL ReadOTPPage(int page, USHORT *ReadData);
	virtual int GetTemperature(USHORT &temperature) ;
	virtual int GetFuseID(CString &FuseID) ;
	virtual int Init() ;
	virtual int ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg);
	virtual int ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);
	virtual int spcCal(USHORT *imgBuf,short *pSPC);
	virtual int ApplySpc(short *pSPC);

	virtual int ReadExp( int &exp);  
	virtual int WriteExp( int exp);  

	virtual int ReadGain( USHORT &gain);  
	virtual int WriteGain( USHORT gain);  
	virtual int ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	virtual int WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
#if 0
private:
	void HI1333_I2C_WtOtpBy(USHORT addr, unsigned char val);
	USHORT HI1333_I2C_RdOtpBy(USHORT addr);
	BOOL HI1333_I2C_WtOtpData(USHORT addr, __IN__ unsigned char *pBuf, unsigned int nLen);
	BOOL HI1333_I2C_RdOtpData(USHORT addr, __OUT__ unsigned char *pBuf, unsigned int nLen);
#endif
	BOOL HI1333_I2C_WtOtpData(USHORT addr, __IN__ unsigned char *pBuf, unsigned int nLen);
	BOOL HI1333_I2C_RdOtpData(USHORT addr, __OUT__ unsigned char *pBuf, unsigned int nLen);
};


