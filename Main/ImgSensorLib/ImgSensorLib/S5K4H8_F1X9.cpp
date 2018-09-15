#include "StdAfx.h"
#include "S5K4H8_F1X9.h"
#include "LSC/Sensor/S5K4H8_5E8/SumLSCTool.h"
#include "atlstr.h"
#pragma comment(lib,"LSC/Sensor/S5K4H8_5E8/SumLSCTool.lib")
S5K4H8_F1X9::S5K4H8_F1X9(void)
{
	flag=0;
}





S5K4H8_F1X9::~S5K4H8_F1X9(void)
{
}
int S5K4H8_F1X9:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K4H8_F1X9::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K4H8_F1X9::GetTemperature(USHORT &temperature)
{
	return 1;
}
int S5K4H8_F1X9::S5K4H8_ReadOTPPage(int page, USHORT *Buff)
{	
	if(NULL == Buff) return 0;

	USHORT stram_flag = 0;
	if(I2cRead(0x0100,&stram_flag,3) != 1) return 0;
	if (stram_flag == 0)
	{
		if(I2cWrite(0x0100,0x01,3) != 1) return 0;
	}	

	if(I2cWrite(0x0a02,page, 3 ) != 1) return 0;
	if(I2cWrite(0x0a00,0x0100, 4) != 1) return 0; //otp enable and read start
	//read otp data to buff
	Sleep(100);
	for( int i = 0; i < 64; i++ )
	{
		if(I2cRead(0x0a04+i, Buff+i, 3 ) != 1) return 0;
	}
	Sleep(100);
	if(I2cWrite(0x0a00,0x0000, 4) != 1) return 0; //otp enable and read end	
	return 1;
}
int S5K4H8_F1X9::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp1[64] = {0};
	int i=0;
	char section[256];
	if(S5K4H8_ReadOTPPage(0,temp1) != 1) return 0;
	for ( i=32; i<38; i++)
	{ 
		sprintf( section,"%02X",temp1[i]);
		FuseID += section;
	}
	return 1;
}
int S5K4H8_F1X9::Init()
{
	return 1;
}
#define GAIN_DEFAULT    0x0100
int S5K4H8_F1X9::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (Typical_rg) /(rg);
	b_ratio = 512 * (Typical_bg) /(bg);

	if( !r_ratio || !b_ratio)
	{
		return FALSE;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT;	
			B_GAIN = (USHORT)(GAIN_DEFAULT * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT;	
			G_GAIN =(USHORT)(GAIN_DEFAULT * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT *  b_ratio / r_ratio);
		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT;
				G_GAIN = (USHORT)(GAIN_DEFAULT * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT;
			}
		}	
	}	
	if(I2cWrite(0x6028, 0x4000, 4) != 1) return 0;  
	if(I2cWrite(0x602a, 0x3058, 4) != 1) return 0;  
	if(I2cWrite(0x6f12, 0x01,   3) != 1) return 0;  
	if(I2cWrite(0x020e, G_GAIN, 4) != 1) return 0;	
	if(I2cWrite(0x0210, R_GAIN, 4) != 1) return 0;  
	if(I2cWrite(0x0212, B_GAIN, 4) != 1) return 0;  
	if(I2cWrite(0x0214, G_GAIN, 4) != 1) return 0;  

	return 1;
}
int S5K4H8_F1X9::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	SensorEnable(3, 0 );  
	Sleep(10);
	SensorEnable(3, 1 );
	Sleep(10);
	if(WriteSensorSetting() != 1) return 0;  //重下setting
	Sleep(50);

	if(I2cWrite(0x6028, 0x4000, 4 ) != 1) return 0;	
	if(I2cWrite(0x602A, 0x0B00, 4 ) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x01,   3 ) != 1) return 0; 
	Sleep(100);   
 
	if(I2cWrite(0x6028, 0x4000, 4 ) != 1) return 0; 
	if(I2cWrite(0x602A, 0x0100, 4 ) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x01,   3 ) != 1) return 0; 

	return 1;
}

int S5K4H8_F1X9::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(NULL == P10Buff) return 0;
	USES_CONVERSION;
	CString tempMsg = _T("");
	CString path = _T("");
	GetCurentPath(path);
	path=path+_T("\LSC\\Sensor\\S5K4H8_F1X9\\");

	//在该两个文件夹下会生成两个文件
	//"S5K4H8LSCOTP.ini";
	//"S5K4H8LSCsram.ini";
	char OutPath[1024]={0};
	strcpy(OutPath,T2A(path.GetBuffer(path.GetLength())));
	char ErrorMsg[1024] = {0};
	Addlog(_T("开始测试LSC"));
	wchar_t wErrorMsg[1024] = {0};

	int result = S5K4H8_LSC(P10Buff,width,height,nLSCTarget,LSCGroup,OutPath,ErrorMsg);
	MultiByteToWideChar(CP_ACP, 0, ErrorMsg, strlen(ErrorMsg), wErrorMsg, sizeof(wErrorMsg)-1);
	if (result !=1)
	{
		tempMsg.Format(_T("LSC校准失败: %s"),wErrorMsg);
		Addlog(tempMsg);
		return FALSE;
	}
	Addlog(_T("测试LSC 成功"));
		//APPLY LSC//
		CString tempStr2 = _T("");
		tempStr2 = path + _T("\\S5K4H8LSCsram.ini");
		CStdioFile file;

		if ( !file.Open( (tempStr2), CFile::modeRead | CFile::typeText ))
		{
			Addlog(_T("打开文件失败!"));
			return  FALSE;
		}

		int     i = 0;
		int     j = 0;
		CStringA strLine = "";
		CString tempLine = _T("");
		CString strtemp = _T("");
		int count = 0;
		int	 intVal;
		USHORT  intLSCAddr;
		USHORT  intLSCValue;
		DWORD Midcount=GetTickCount();
		strtemp.Format(_T("%d"),Midcount);
		Addlog(strtemp);
		SensorEnable(3, 0 );  
		Sleep(10);
		SensorEnable(3, 1 );
		Sleep(10);
		Midcount=GetTickCount();
		strtemp.Format(_T("%d"),Midcount);
		Addlog(strtemp);
		if(WriteSensorSetting() != 1) return 0;//重下setting
		Midcount=GetTickCount();
		strtemp.Format(_T("%d"),Midcount);
		Addlog(strtemp);
		Sleep(50);

		if(I2cWrite(0x6028, 0x4000, 4 ) != 1) return 0;                                      
		if(I2cWrite(0x602A, 0x0B00, 4 ) != 1) return 0;  //0B00 01,Auto lsc on               
		if(I2cWrite(0x6F12, 0x01,   3 ) != 1) return 0;                                      
		Sleep(50);                                        
		if(I2cWrite(0x6028, 0x2000, 4 ) != 1) return 0;                                      
		if(I2cWrite(0x602A, 0x0C40, 4 ) != 1) return 0;                                      
		if(I2cWrite(0x6F12, 0x00,   3 ) != 1) return 0;                                      

		Midcount=GetTickCount();
		while ( file.ReadString(tempLine) )
		{
			strLine=W2A(tempLine);
			if ( strLine.Find( "s", 0 ) != -1 ) // -1表示没找到；
			{
				int nLength = strLine.GetLength(); 
				nLength--;
				strLine = strLine.Right(nLength); 

				intVal = strtoul( strLine, NULL, 16 );	//16进制 ，strLine字符串转成无符号长整型存入intVal

				if( nLength == 6 )
				{  
					if(I2cWrite(0xfffe, 0x0003, 4 ) != 1) return 0;
					intLSCAddr  = intVal>>8;
					intLSCValue = intVal%256;			
					if(I2cWrite(intLSCAddr, intLSCValue, 3 ) != 1) return 0;
					count++;
				}
				else if( nLength == 8 )
				{
					if(I2cWrite(0xfffe, 0x0004, 4 ) != 1) return 0;
					intLSCAddr  = intVal>>16;
					intLSCValue = ( intVal&0xffff );			
					if(I2cWrite(intLSCAddr, intLSCValue, 4 ) != 1) return 0; 
					count++;
				}
			} 	
		}

		file.Close(); 

		if(I2cWrite(0x6028, 0x4000, 4 ) != 1) return 0; 	
		if(I2cWrite(0x602A, 0x0100, 4 ) != 1) return 0; 	
		if(I2cWrite(0x6F12, 0x01,   3 ) != 1) return 0;  

		Midcount=GetTickCount();
		strtemp.Format(_T("%d"),Midcount);
		Addlog(strtemp);
	return 1;
}

int S5K4H8_F1X9::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int S5K4H8_F1X9::ApplySpc(short *pSPC)
{
	return 1;
}


int S5K4H8_F1X9::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int S5K4H8_F1X9::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int S5K4H8_F1X9::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int S5K4H8_F1X9::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 


int S5K4H8_F1X9::WriteSensorSetting()
{
	if(I2cWrite(0x602A, 0x6214,4) != 1) return 0;                                             
	if(I2cWrite(0x6F12, 0x7971,4) != 1) return 0;  
	if(I2cWrite(0x602A, 0x6218,4) != 1) return 0;                                                      
	if(I2cWrite(0x6F12, 0x7150,4) != 1) return 0;  
	if(I2cWrite(0x6028, 0x2000,4) != 1) return 0;  
	if(I2cWrite(0x602A, 0x1FD0,4) != 1) return 0;  
	if(I2cWrite(0x6F12, 0x0448,4) != 1) return 0;  
	if(I2cWrite(0x6F12, 0x0349,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0160,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xC26A,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x511A,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x8180,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x60B8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x20E8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x13A0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x38B5,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0021,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0446,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x8DF8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0010,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F5,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xB470,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0122,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6946,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x59F8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x9DF8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xFF28,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x05D0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0020,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x08B1,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x04F2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6914,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2046,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x38BD,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0120,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xF8E7,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x10B5,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x92B0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0C46,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x4822,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6946,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x46F8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0020,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6946,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x04EB,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x4003,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0A5C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x02F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0F02,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x04F8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x1020,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0A5C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x401C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x1209,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x5A70,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x4828,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xF2D3,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x12B0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x10BD,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2DE9,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xF041,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x164E,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0F46,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x06F1,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x1105,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xA236,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xB0B1,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x1449,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x1248,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0968,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0078,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xB1F8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6A10,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xC007,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0ED0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0846,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xFFF7,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xBEFF,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x84B2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2946,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2046,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xFFF7,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xD0FF,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x4FF4,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x9072,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x3146,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x04F1,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x4800,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x16F8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x002F,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x05D0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x3146,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2846,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xBDE8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xF041,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x13B8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xBDE8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xF081,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0022,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xAFF2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x5501,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0348,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x00F0,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x10B8,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0C40,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x2000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0560,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x152D,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x48F6,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x296C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xC0F2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x000C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6047,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x41F2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x950C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xC0F2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x000C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6047,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x49F2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x514C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0xC0F2,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x000C,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x6047,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x4088,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0166,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0002,4) != 1) return 0; 
	if(I2cWrite(0x5360, 0x0004,4) != 1) return 0; 
	if(I2cWrite(0xF490, 0x0030,4) != 1) return 0; 
	if(I2cWrite(0xF47A, 0x0012,4) != 1) return 0; 
	if(I2cWrite(0xF428, 0x0200,4) != 1) return 0; 
	if(I2cWrite(0xF48E, 0x0010,4) != 1) return 0; 
	if(I2cWrite(0xF45C, 0x0004,4) != 1) return 0; 
	if(I2cWrite(0x31AA, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x1004, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x1006, 0x0006,4) != 1) return 0; 
	if(I2cWrite(0x6028, 0x2000,4) != 1) return 0; 
	if(I2cWrite(0x602A, 0x0EC6,4) != 1) return 0; 
	if(I2cWrite(0x6F12, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x31FB, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x0B04, 0x0101,4) != 1) return 0; 
	if(I2cWrite(0x3078, 0x0059,4) != 1) return 0; 
	if(I2cWrite(0x307C, 0x0025,4) != 1) return 0; 
	if(I2cWrite(0x36D0, 0x00DD,4) != 1) return 0; 
	if(I2cWrite(0x36D2, 0x0100,4) != 1) return 0; 
	if(I2cWrite(0x0B00, 0x0080,4) != 1) return 0; 
	if(I2cWrite(0x306A, 0x00EF,4) != 1) return 0; 
	if(I2cWrite(0x0340, 0x09C2,4) != 1) return 0; 
	if(I2cWrite(0x0342, 0x0EA0,4) != 1) return 0; 
	if(I2cWrite(0x0344, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x0346, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x0348, 0x0CCF,4) != 1) return 0; 
	if(I2cWrite(0x034A, 0x099F,4) != 1) return 0; 
	if(I2cWrite(0x034C, 0x0CD0,4) != 1) return 0; 
	if(I2cWrite(0x034E, 0x09A0,4) != 1) return 0; 
	if(I2cWrite(0x0900, 0x0111,4) != 1) return 0; 
	if(I2cWrite(0x0380, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x0382, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x0384, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x0386, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x0400, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x0404, 0x0010,4) != 1) return 0; 
	if(I2cWrite(0x0136, 0x1800,4) != 1) return 0; 
	if(I2cWrite(0x0300, 0x0005,4) != 1) return 0; 
	if(I2cWrite(0x0302, 0x0001,4) != 1) return 0; 
	if(I2cWrite(0x0304, 0x0006,4) != 1) return 0; 
	if(I2cWrite(0x0306, 0x00AF,4) != 1) return 0; 
	if(I2cWrite(0x030C, 0x0006,4) != 1) return 0; 
	if(I2cWrite(0x030E, 0x00AF,4) != 1) return 0; 
	if(I2cWrite(0x3008, 0x0000,4) != 1) return 0; 
	if(I2cWrite(0x0200, 0x0618,4) != 1) return 0; 
	if(I2cWrite(0x0202, 0x0904,4) != 1) return 0; 
	if(I2cWrite(0x0204, 0x0020,4) != 1) return 0; 
	if(I2cWrite(0x0114, 0x0330,4) != 1) return 0; 
	if(I2cWrite(0x317A, 0x0101,4) != 1) return 0; 
	if(I2cWrite(0x317C, 0x0096,4) != 1) return 0; 
	if(I2cWrite(0x0100, 0x0000,4) != 1) return 0; 

	return 1;
}
