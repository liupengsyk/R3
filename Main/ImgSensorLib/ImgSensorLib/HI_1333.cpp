  

#include "StdAfx.h"
#include "HI_1333.h"


HI1333::HI1333(void)
{

}


HI1333::~HI1333(void)
{
}
int HI1333:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int HI1333::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
BOOL HI1333::ReadOTPPage(int page, USHORT *ReadData)
{
	return 1;
}
int HI1333::GetTemperature(USHORT &temperature)
{
	return 1;
}
int HI1333::GetFuseID(CString &FuseID) 
{
#if 1
	unsigned char temp[9] = {0};
	int i = 0;
	FuseID = _T("");
	char section[256] = {0};
	
	if(HI1333_I2C_RdOtpData(0x0001, temp, sizeof(temp) / sizeof(temp[0])) != 1) return 0;
	for (i = 0; i < 9; i++)
	{
		sprintf( section, "%02X", temp[i]);
		CString str(section);
		FuseID += str;
	}
#else
	USHORT temp  = 0;
	int i = 0;
	FuseID = _T("");
	char section[256] = {0};
	
	//1).首先进行I2C写的操作:
	I2cWrite(0x0A02, 0x01, HI1333_I2C_MODE);
	I2cWrite(0x0A00, 0x00, HI1333_I2C_MODE);
	Sleep(10); 
	I2cWrite(0x0F02, 0x00, HI1333_I2C_MODE);
	I2cWrite(0x071A, 0x01, HI1333_I2C_MODE);
	I2cWrite(0x071B, 0x09, HI1333_I2C_MODE);
	I2cWrite(0x0d04, 0x01, HI1333_I2C_MODE);
	I2cWrite(0x0d00, 0x07, HI1333_I2C_MODE);
	I2cWrite(0x003E, 0x10, HI1333_I2C_MODE);
	I2cWrite(0x0A00, 0x01, HI1333_I2C_MODE);
	 
	I2cWrite(HI1333_REG_OTP_ADDR_H, 0x00, HI1333_I2C_MODE);   // start address H 
	I2cWrite(HI1333_REG_OTP_ADDR_L, 0x01, HI1333_I2C_MODE);   //start address L 
	I2cWrite(HI1333_REG_OTP_CMD, 0x01, HI1333_I2C_MODE);   //Read Enable
	 
	//2).然后进行I2C读的操作(举例说明):
	for ( i = 0; i < 9; i++)
	{ 
		int flag = I2cRead(HI1333_REG_OTP_RDATA, &temp, HI1333_I2C_MODE);
		sprintf( section, "%02X", temp);
		CString str(section);
		FuseID += str;
	}

	//3).最后进行I2C写的操作（使图像恢复预览）:
	I2cWrite(0x0A00, 0x00, HI1333_I2C_MODE);     // stand by on 
	Sleep(10); 
	I2cWrite(0x003E, 0x00, HI1333_I2C_MODE);     // display mode 
	I2cWrite(0x0A00, 0x01, HI1333_I2C_MODE);     // stand by off 
#endif

	return 1;
}
int HI1333::Init()
{
	return 1;
}
int HI1333::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	int R_gain = 0x200;
	int G_gain = 0x200;
	int B_gain = 0x200;
	
	R_gain = 0x200 * Typical_rg / rg;
	B_gain = 0x200 * Typical_bg / bg;
	if (R_gain < B_gain)
	{
		if (R_gain < 0x200)
		{
			B_gain = 0x200 * B_gain / R_gain;
			G_gain = 0x200 * G_gain / R_gain;
			R_gain = 0x200;
		}
	}
	else
	{
		if (B_gain < 0x200)
		{
			R_gain = 0x200 * R_gain / B_gain;
			G_gain = 0x200 * G_gain / B_gain;
			B_gain = 0x200;
		}
	}
	
	if(I2cWrite(HI1333_REG_DGAIN_GR_H, MSB(G_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_GR_L, LSB(G_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_GB_H, MSB(G_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_GB_L, LSB(G_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_R_H, MSB(R_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_R_L, LSB(R_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_B_H, MSB(B_gain), HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_DGAIN_B_L, LSB(B_gain), HI1333_I2C_MODE) != 1) return 0;

	return 1;
}
int HI1333::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	/*
	*	LSC的打开和关闭功能，见<Hi-1333_OTP_guide_ver0.4_170707.pdf>文件中描述.
	*	LSC使能功能是在0x075C寄存器中设置, 点亮设定里面0x075C寄存器的值是0x01, 也就是打开了DPC功能.
	*	咨询过原厂, DPC功能打开不会影响PDAF功能. 所以应该打开DPC功能.
	*/
	return 1;
}

int HI1333::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int HI1333::ApplySpc(short *pSPC)
{
	return 1;
}
 

int HI1333::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	
	if(I2cRead(HI1333_REG_EXP_TIM_H, &ExpHigh, HI1333_I2C_MODE) != 1) return 0;
	if(I2cRead(HI1333_REG_EXP_TIM_L, &ExpLow, HI1333_I2C_MODE) != 1) return 0;
	exp = (ExpHigh << 8) + (ExpLow & 0xFF);

	return 1;
} 

int HI1333::WriteExp( int exp)
{
	USHORT ExpHigh = exp >> 8;
	USHORT ExpLow = exp & 0xFF;
	 
	if(I2cWrite(HI1333_REG_EXP_TIM_H, ExpHigh, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_EXP_TIM_L, ExpLow, HI1333_I2C_MODE) != 1) return 0;

	return 1;
} 

int HI1333::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;

	//Analog Gain, not Digital Gain
	if(I2cRead(HI1333_REG_AGAIN_H, &GainHigh, HI1333_I2C_MODE) != 1) return 0;
	if(I2cRead(HI1333_REG_AGAIN_L, &GainLow, HI1333_I2C_MODE) != 1) return 0;
	 
	gain = (GainHigh << 8) + (GainLow & 0xFF);

	return 1;
} 
int HI1333::WriteGain( USHORT gain)
{
	USHORT GainHigh = gain >> 8;
	USHORT GainLow = gain & 0xFF;

	//Analog Gain, not Digital Gain
	if(I2cWrite(HI1333_REG_AGAIN_H, GainHigh, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(HI1333_REG_AGAIN_L, GainLow, HI1333_I2C_MODE) != 1) return 0;
	
	return 1;
} 

#if 0
/******************************* 以下四个函数主要用来向Sensor里面写入AWB和LSC校准数据 *******************************/
void HI1333::HI1333_I2C_WtOtpBy(USHORT addr, unsigned char val)
{
	I2cWrite(HI1333_REG_OTP_ADDR_H, MSB(addr), HI1333_I2C_MODE); // start address H
	I2cWrite(HI1333_REG_OTP_ADDR_L, LSB(addr), HI1333_I2C_MODE); // start address L
	I2cWrite(HI1333_REG_OTP_CMD, HI1333_OTP_WT_MODE, HI1333_I2C_MODE); // OTP write mode
	I2cWrite(HI1333_REG_OTP_WDATA, val, HI1333_I2C_MODE); // otp data write
	Sleep(5); // delay 5ms
	/*If(data[i] != IIC_Read(0x0718)) // verify
	{
		Fail_address[k]=i;
		k++;
	}*/
}

unsigned char HI1333::HI1333_I2C_RdOtpBy(USHORT addr)
{
	unsigned char RdData;
	
	I2cWrite(HI1333_REG_OTP_ADDR_H, MSB(addr), HI1333_I2C_MODE); // start address H
	I2cWrite(HI1333_REG_OTP_ADDR_L, LSB(addr), HI1333_I2C_MODE); // start address L
	I2cWrite(HI1333_REG_OTP_CMD, HI1333_OTP_RD_MODE, HI1333_I2C_MODE); // OTP read mode
	RdData = I2cRead(HI1333_REG_OTP_RDATA, HI1333_I2C_MODE); // OTP data read

	return RdData;
}

BOOL HI1333::HI1333_I2C_WtOtpData(USHORT addr, __IN__ unsigned char *pBuf, unsigned int nLen)
{
	unsigned int i;

	if (NULL == pBuf)
	{
		return FALSE;
	}
	
	I2cWrite(HI1333_REG_OTP_ADDR_H, MSB(addr), HI1333_I2C_MODE); // start address H
	I2cWrite(HI1333_REG_OTP_ADDR_L, LSB(addr), HI1333_I2C_MODE); // start address L
	I2cWrite(HI1333_REG_OTP_CMD, HI1333_OTP_WT_MODE, HI1333_I2C_MODE); // OTP write mode
	for (i = 0; i < nLen; i++)
	{
		I2cWrite(HI1333_REG_OTP_WDATA, pBuf[i], HI1333_I2C_MODE); // otp data write
		Sleep(5); // delay 5ms
		/*If(data[i] != IIC_Read(0x0718)) // verify
		{
			Fail_address[k]=i;
			k++;
		}*/
	}

	return TRUE;
}

BOOL HI1333::HI1333_I2C_RdOtpData(USHORT addr, __OUT__ unsigned char *pBuf, unsigned int nLen)
{
	unsigned int i;

	if (NULL == pBuf)
	{
		return FALSE;
	}
	
	I2cWrite(HI1333_REG_OTP_ADDR_H, MSB(addr), HI1333_I2C_MODE); // start address H
	I2cWrite(HI1333_REG_OTP_ADDR_L, LSB(addr), HI1333_I2C_MODE); // start address L
	I2cWrite(HI1333_REG_OTP_CMD, HI1333_OTP_RD_MODE, HI1333_I2C_MODE); // OTP read mode
	for (i = 0; i < nLen; i++)
	{
		pBuf[i] = I2cRead(HI1333_REG_OTP_RDATA, HI1333_I2C_MODE); // OTP data read
	}

	return TRUE;
}
#endif

BOOL HI1333::HI1333_I2C_WtOtpData(USHORT addr, __IN__ unsigned char *pBuf, unsigned int nLen)
{
	unsigned int i;

	if (NULL == pBuf)
	{
		return FALSE;
	}

	//1).首先进行I2C写的操作：
	if(I2cWrite(0x0A02, 0x01, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0A00, 0x00, HI1333_I2C_MODE) != 1) return 0;
	Sleep(10); 
	if(I2cWrite(0x0F02, 0x00, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x071A, 0x01, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x071B, 0x09, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0d04, 0x01, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0d00, 0x07, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x003E, 0x10, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0A00, 0x01, HI1333_I2C_MODE) != 1) return 0;

	if(I2cWrite(HI1333_REG_OTP_ADDR_H, MSB(addr), HI1333_I2C_MODE) != 1) return 0;   // start address H 
	if(I2cWrite(HI1333_REG_OTP_ADDR_L, LSB(addr), HI1333_I2C_MODE) != 1) return 0;   //start address L 
	if(I2cWrite(HI1333_REG_OTP_CMD, HI1333_OTP_WT_MODE, HI1333_I2C_MODE) != 1) return 0;	 //OTP write mode
	 
	//2).然后进行I2C读的操作(举例说明)：
	for ( i = 0; i < nLen; i++)
	{ 
		if(I2cWrite(HI1333_REG_OTP_WDATA, pBuf[i], HI1333_I2C_MODE) != 1) return 0; // otp data write
		Sleep(5); // delay 5ms
	}

	//3).最后进行I2C写的操作（使图像恢复预览）：
	if(I2cWrite(0x0A00, 0x00, HI1333_I2C_MODE) != 1) return 0;  // stand by on 
	Sleep(10); 
	if(I2cWrite(0x003E, 0x00, HI1333_I2C_MODE) != 1) return 0;	 // display mode 
	if(I2cWrite(0x0A00, 0x01, HI1333_I2C_MODE) != 1) return 0;	 // stand by off 

	return TRUE;
}

BOOL HI1333::HI1333_I2C_RdOtpData(USHORT addr, __OUT__ unsigned char *pBuf, unsigned int nLen)
{
	USHORT u16Tmp;
	unsigned int i;

	if (NULL == pBuf)
	{
		return FALSE;
	}

	//1).首先进行I2C写的操作：

	if(I2cWrite(0x0A02, 0x01, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0A00, 0x00, HI1333_I2C_MODE) != 1) return 0;
	Sleep(10);
	if(I2cWrite(0x0F02, 0x00, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x071A, 0x01, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x071B, 0x09, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0d04, 0x01, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0d00, 0x07, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x003E, 0x10, HI1333_I2C_MODE) != 1) return 0;
	if(I2cWrite(0x0A00, 0x01, HI1333_I2C_MODE) != 1) return 0;
	 
	if(I2cWrite(HI1333_REG_OTP_ADDR_H, MSB(addr), HI1333_I2C_MODE) != 1) return 0;   // start address H 
	if(I2cWrite(HI1333_REG_OTP_ADDR_L, LSB(addr), HI1333_I2C_MODE) != 1) return 0;   //start address L 
	if(I2cWrite(HI1333_REG_OTP_CMD, HI1333_OTP_RD_MODE, HI1333_I2C_MODE) != 1) return 0;   //OTP Read Enable
	 
	//2).然后进行I2C读的操作(举例说明)：
	for ( i = 0; i < nLen; i++)
	{
		if(I2cRead(HI1333_REG_OTP_RDATA, &u16Tmp, HI1333_I2C_MODE) != 1) return 0;
		pBuf[i] = LSB(u16Tmp);
	}

	//3).最后进行I2C写的操作（使图像恢复预览）：
	if(I2cWrite(0x0A00, 0x00, HI1333_I2C_MODE) != 1) return 0;	 // stand by on 
	Sleep(10); 
	if(I2cWrite(0x003E, 0x00, HI1333_I2C_MODE) != 1) return 0;   // display mode 
	if(I2cWrite(0x0A00, 0x01, HI1333_I2C_MODE) != 1) return 0;	 // stand by off 

	return TRUE;
}


