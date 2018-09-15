#include "StdAfx.h"
#include "IMX_258_0AQH5.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214

IMX_258_0AQH5::IMX_258_0AQH5(void)
{
}


IMX_258_0AQH5::~IMX_258_0AQH5(void)
{
}
int IMX_258_0AQH5:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_258_0AQH5::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL IMX_258_0AQH5::IMX258_ReadOTPPage(int Page, USHORT *ReadData)
{	
	if(NULL == ReadData) return 0;

	if(I2cWrite(0x0a02, Page, 3) != 1) return 0;
	if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0;
	USHORT flag = 0;
	int cnt = 0;
	do 
	{
		if(I2cRead(0x0a01, &flag, 3) != 1) return 0;
		cnt++;
	} while ( ( flag != 0x01 ) && ( cnt < 100 ) );
	if ( cnt >=100 )
	{
		return 0;
	}
	for ( int i = 0; i < 64; i++ )
	{
		if(I2cRead(0x0a04+i, ReadData+i, 3) != 1) return 0;
	}
	return 1;

}
int IMX_258_0AQH5::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_258_0AQH5::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp1[64] = {0};
	char section[256];
	int i=0;
	if(IMX258_ReadOTPPage(15,temp1) != 1) return 0;
	for (i=32;i<43;i++)
	{
		sprintf(section,"%02X",temp1[i]);
		FuseID+=section;
	}
	return 1;
}
int IMX_258_0AQH5::Init()
{
	return 1;
}
int IMX_258_0AQH5::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	if (rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	USHORT r_ratio, b_ratio;
	
	r_ratio = 512 * (Typical_rg) /(rg);
	b_ratio = 512 * (Typical_bg) /(bg);
	
	 
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
	
	if(I2cWrite(  GAIN_RED_ADDR, R_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_RED_ADDR+1, R_GAIN&0xff, 3) != 1) return 0;
	
	if(I2cWrite(  GAIN_BLUE_ADDR, B_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_BLUE_ADDR+1, B_GAIN&0xff, 3) != 1) return 0;
	 
	if(I2cWrite(  GAIN_GREEN1_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN1_ADDR+1, G_GAIN&0xff, 3) != 1) return 0;
	 
	if(I2cWrite(  GAIN_GREEN2_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN2_ADDR+1, G_GAIN&0xff, 3) != 1) return 0;

	return 1;
}


int IMX_258_0AQH5::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}
int IMX_258_0AQH5::spcCal(USHORT *imgBuf,short *pSPC)
{  
	if(NULL == imgBuf || NULL == pSPC) return 0;

	CString path;
	GetCurentPath(path);
	path = path + _T("T_SPC\\IMX_258\\");
	USHORT *Calcbuffer = new USHORT[4208*3122+2];
	if(NULL == Calcbuffer) return 0;
	 
	for (int i=0;i<(4208*3122+2);i++)
	{
		if(i<8418) Calcbuffer[i]=0; //前面两行设置为0
		else Calcbuffer[i]=imgBuf[i-8418]; 
	}
    
	Calcbuffer[0]=4208;
	Calcbuffer[1]=3122;
	CString IniSpcGainPath;
	IniSpcGainPath=path+ _T("spc_gain_258_NML.ini"); 
	
	typedef int (*lpFun)(unsigned short *ImageBuffer, char *IniFilePath, short *pSPC);

	HINSTANCE hDll;   //DLL句柄 
	CString toolDllPath = 	path + _T("IMX258_SPC_DLL.dll");
	hDll = LoadLibrary(toolDllPath);

	lpFun GainCorrection = (lpFun)GetProcAddress(hDll,"calcSPCgainmain");
	if (NULL==GainCorrection)
	{
		delete [] Calcbuffer;
		Calcbuffer = NULL;
		Addlog(_T("DLL 加载失败!\n"));
		FreeLibrary(hDll);
		return FALSE;
	} 
	char IniFilePath[1024]; 
	sprintf(IniFilePath,"%s",IniSpcGainPath.GetBuffer(IniSpcGainPath.GetLength()));
	int flag = GainCorrection(Calcbuffer,IniFilePath,pSPC);
	delete [] Calcbuffer;
	Calcbuffer = NULL;
	FreeLibrary(hDll);
	return 1;
}
int IMX_258_0AQH5::ApplySpc(short *pSPC)
{
	if(NULL == pSPC) return 0;

	typedef int (*lpFun2)(unsigned short *ImageBuffer, char *IniFilePath, char *outname);
	
	HINSTANCE hDll;   //DLL句柄 
	CString m_szDirectory;
	GetCurentPath(m_szDirectory);
	m_szDirectory = m_szDirectory + _T("T_SPC\\IMX_258\\IMX258_SPC_DLL.dll");	
	hDll = LoadLibrary(m_szDirectory);
	if (NULL==hDll)
	{
		FreeLibrary(hDll);
		return FALSE;
	}
	
	lpFun2 JudgeSPCgain = (lpFun2)GetProcAddress(hDll,"judgeSPCgainmain");
	if (NULL==JudgeSPCgain)
	{
		FreeLibrary(hDll);
		return FALSE;
	} 
	if(I2cWrite( 0x0101,0,3) != 1) return 0; //V and H are both set by this. 	
	
	for (int i = 0;i<126;i++)
	{
		BYTE SPCData=0;
		SPCData=pSPC[i]&0xff;
		if(i<63)
			if(I2cWrite( 0xD04C+i,SPCData,3) != 1) return 0;
		else
			if(I2cWrite( 0xD08C+(i-63),SPCData,3) != 1) return 0;		
	}

	if(I2cWrite( 0x3051,0,3) != 1) return 0; //Ver2.0
	if(I2cWrite( 0x3052,0,3) != 1) return 0;
	if(I2cWrite( 0x7BCA,0,3) != 1) return 0;
	if(I2cWrite( 0x7BCB,0,3) != 1) return 0;
	if(I2cWrite( 0x7BC8,1,3) != 1) return 0;
	FreeLibrary(hDll);
	return 1; 
}

int IMX_258_0AQH5::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 
int IMX_258_0AQH5::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_258_0AQH5::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_258_0AQH5::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 