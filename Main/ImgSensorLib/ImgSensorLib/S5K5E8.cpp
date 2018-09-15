#include "StdAfx.h"
#include "S5K5E8.h"
#include "LSC/Sensor/S5K4H8_5E8/SumLSCTool.h"
#include "atlstr.h"
#pragma comment(lib,"LSC/Sensor/S5K4H8_5E8/SumLSCTool.lib")
// #include "LSC/Sensor/S5K5E8/SumLSCTool.h"
// #pragma comment(lib,"LSC/Sensor/S5K5E8/SumS5K5E8LSCTool.lib")


S5K5E8::S5K5E8(void)
{
}


S5K5E8::~S5K5E8(void)
{
}

int S5K5E8:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K5E8::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K5E8::S5K5E8_ReadOTPPage(int page, USHORT *Buff)
{		
	if(NULL == Buff) return 0;

	USHORT stram_flag = 0;
	if(I2cRead(0x0100,&stram_flag,3) != 1) return 0;
	if (stram_flag == 0x01)
	{
		if(I2cWrite(0x0100,0x00,3) != 1) return 0;
	}	

	if(I2cWrite(0x0a00,0x04, 3 ) != 1) return 0;                              
	if(I2cWrite(0x0a02,page, 3 ) != 1) return 0;                              
	if(I2cWrite(0x0a00,0x01,  3) != 1) return 0; //otp enable and read start  

	//read otp data to buff
	Sleep(100);
	for( int i = 0; i < 64; i++ )
	{
		if(I2cRead(0x0a04+i, Buff+i, 3 ) != 1) return 0;
	}
	Sleep(100);

	if(I2cWrite(0x0a00,0x04, 3) != 1) return 0;                              
	if(I2cWrite(0x0a00,0x00, 3) != 1) return 0; //otp enable and read end	   
	if(I2cWrite(0x0100,0x01, 3) != 1) return 0;                              

	return 1;
}
int S5K5E8::GetTemperature(USHORT &temperature)
{
	return 1;
}
int S5K5E8::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp1[64] = {0};
	int i=0;
	char section[256];
	if(S5K5E8_ReadOTPPage(0,temp1) != 1) return 0;					
	for ( i=0; i<8; i++)
	{ 
		sprintf( section,"%02X",temp1[i]);
		FuseID += section;
	}
	return 1;
}
int S5K5E8::Init()
{
	return 1;
}
#define GAIN_DEFAULT    0x0100
int S5K5E8::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	USHORT r_ratio, b_ratio;

	r_ratio = 512.0 * (Typical_rg) /(rg) + 0.5;
	b_ratio = 512.0 * (Typical_bg) /(bg) + 0.5;

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

	if(I2cWrite(0x020e, G_GAIN, 4) != 1) return 0;	
	if(I2cWrite(0x0210, R_GAIN, 4) != 1) return 0; 
	if(I2cWrite(0x0212, B_GAIN, 4) != 1) return 0; 
	if(I2cWrite(0x0214, G_GAIN, 4) != 1) return 0; 

	return 1;
}
int S5K5E8::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(I2cWrite(0x0100,0x00, 3) != 1) return 0; 
	if(I2cWrite(0x3400,0x00, 3) != 1) return 0; 
	if(I2cWrite(0x0100,0x01, 3) != 1) return 0; 

	return 1;
}

int S5K5E8::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int S5K5E8::ApplySpc(short *pSPC)
{
	return 1;
}

 
int S5K5E8::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3)  != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int S5K5E8::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	/*I2cWrite(0x0100,0x00,3);*/
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3)  != 1) return 0;
	/*I2cWrite(0x0100,0x01,3);*/

	return 1;
} 

int S5K5E8::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3)  != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int S5K5E8::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3)  != 1) return 0;

	return 1;
}
int S5K5E8::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(NULL == P10Buff) return 0;

	USES_CONVERSION;
	CString tempMsg = _T("");
	CString path = _T("");
	GetCurentPath(path);
	path=path+_T("\LSC\\Sensor\\S5K5E8\\");

	//在该两个文件夹下会生成两个文件
	//"S5K5E8LSCOTP.ini";
	//"S5K5E8LSCsram.ini";
	char OutPath[1024]={0};
	strcpy(OutPath,T2A(path.GetBuffer(path.GetLength())));
	char ErrorMsg[1024] = {0};
	Addlog(_T("开始测试LSC"));
	wchar_t wErrorMsg[1024] = {0};
	int result = S5K5E8_LSC(P10Buff,width,height,nLSCTarget,OutPath,ErrorMsg);
	MultiByteToWideChar(CP_ACP, 0, ErrorMsg, strlen(ErrorMsg), wErrorMsg, sizeof(wErrorMsg)-1);
	if (result !=1)
	{
		tempMsg.Format(_T("LSC校准失败: %s"),ErrorMsg);
		Addlog(tempMsg);
		return FALSE;
	}
	Addlog(_T("测试LSC 成功"));
	//biao ji 1
	CString LSCSramPath = path + _T("S5K5E8LSCsram.ini");

	CStdioFile file;
	if ( !file.Open( (LSCSramPath), CFile::modeRead | CFile::typeText ))
	{
		tempMsg.Format(_T("S5K5E8LSCsram file打开失败!"));
		Addlog(tempMsg);
		return  FALSE;
	}

	int     i = 0;
	int     j = 0;

	CString strLine = _T("");

	int	 intVal;
	int  intLSCAddr;
	int  intLSCValue;

	USHORT value1=0;
	USHORT value2=0;
	if(I2cRead(0x0202,&value1,3) != 1) return 0;                 
	if(I2cRead(0x0203,&value2,3) != 1) return 0;                 
	if(I2cWrite(0x0100,0x00,3  ) != 1) return 0;                 
	Sleep(10);                              		                 

	if(I2cWrite(0x0101,0x00,3) != 1) return 0;                  	
	if(I2cWrite(0x0136,0x18,3) != 1) return 0;                   
	if(I2cWrite(0x3303,0x02,3) != 1) return 0;                   
	if(I2cWrite(0x3400,0x01,3) != 1) return 0;                   
	if(I2cWrite(0x3906,0xBE,3) != 1) return 0;                   
	if(I2cWrite(0x3C01,0x0F,3) != 1) return 0;                   
	if(I2cWrite(0x3C14,0x00,3) != 1) return 0;                   
	if(I2cWrite(0x3235,0x08,3) != 1) return 0;                   
	if(I2cWrite(0x3063,0x2E,3) != 1) return 0;                   
	if(I2cWrite(0x307A,0x40,3) != 1) return 0;                   
	if(I2cWrite(0x307B,0x11,3) != 1) return 0;                   
	if(I2cWrite(0x3079,0x20,3) != 1) return 0;                   
	if(I2cWrite(0x3070,0x05,3) != 1) return 0;                   
	if(I2cWrite(0x3067,0x06,3) != 1) return 0;                   
	if(I2cWrite(0x3071,0x62,3) != 1) return 0;                   
	if(I2cWrite(0x3203,0x43,3) != 1) return 0;                   
	if(I2cWrite(0x3205,0x43,3) != 1) return 0;                   
	if(I2cWrite(0x320b,0x42,3) != 1) return 0;                   
	if(I2cWrite(0x3007,0x00,3) != 1) return 0;                   
	if(I2cWrite(0x3008,0x14,3) != 1) return 0;                   
	if(I2cWrite(0x3020,0x58,3) != 1) return 0;                   
	if(I2cWrite(0x300D,0x34,3) != 1) return 0;                   
	if(I2cWrite(0x300E,0x17,3) != 1) return 0;                   
	if(I2cWrite(0x3021,0x02,3) != 1) return 0;                   
	if(I2cWrite(0x3010,0x59,3) != 1) return 0;                   
	if(I2cWrite(0x3002,0x01,3) != 1) return 0;                   
	if(I2cWrite(0x3005,0x01,3) != 1) return 0;                   
	if(I2cWrite(0x3008,0x04,3) != 1) return 0;                   
	if(I2cWrite(0x300F,0x70,3) != 1) return 0;                   
	if(I2cWrite(0x3010,0x69,3) != 1) return 0;                   
	if(I2cWrite(0x3017,0x10,3) != 1) return 0;                   
	if(I2cWrite(0x3019,0x19,3) != 1) return 0;                   
	if(I2cWrite(0x300C,0x62,3) != 1) return 0;                   
	if(I2cWrite(0x3064,0x10,3) != 1) return 0;                   
	if(I2cWrite(0x323D,0x01,3) != 1) return 0;                   
	if(I2cWrite(0x323E,0x3E,3) != 1) return 0;                   
	if(I2cWrite(0x0305,0x04,3) != 1) return 0;                   
	if(I2cWrite(0x0307,0xa6,3) != 1) return 0;                   
	if(I2cWrite(0x0340,0x0f,3) != 1) return 0;                   
	if(I2cWrite(0x0341,0x82,3) != 1) return 0;                   

	if(I2cWrite(0x0202,value1,3) != 1) return 0;                 
	if(I2cWrite(0x0203,value2,3) != 1) return 0;                 
	               
	if(I2cWrite(0x3400,0x00,  3) != 1) return 0;  // lsc on                   
	if(I2cWrite(0x0100,0x00,  3) != 1) return 0;  //stream off   


	int k=0;
	while ( file.ReadString(strLine) )
	{		
		k++;
		strLine = strLine.Left(9);
		strLine.TrimRight();
		if ( strLine.Find( _T("s"), 0 ) != -1 )
		{ 
			int nLength = strLine.GetLength(); 
			nLength--;
			strLine = strLine.Right(nLength); 
			intVal = strtoul( T2A(strLine.GetBuffer(strLine.GetLength())), NULL, 16 );
			if( nLength == 6 )
			{  
				intLSCAddr = intVal>>8;
				intLSCValue = intVal%256;
				if ((k>2) && (intLSCAddr==0x3457))
				{		
					if(I2cWrite(intLSCAddr, intLSCValue, 3 ) != 1) return 0;				
					Sleep(10);
				} 
				else
				{
					if(I2cWrite(intLSCAddr, intLSCValue, 3 ) != 1) return 0;
				}
			}
			else if( nLength == 8 )
			{
				intLSCAddr = intVal>>16;
				intLSCValue = ( intVal&0xffff );			
				if(I2cWrite(intLSCAddr, intLSCValue, 4 ) != 1) return 0;
			}
		}
	}
	file.Close(); 
	Sleep(100);
	if(I2cWrite(0x3400,0x00,3) != 1) return 0;
	if(I2cWrite(0x0100,0x01,3) != 1) return 0;
	Sleep(200);

	return 1;
}