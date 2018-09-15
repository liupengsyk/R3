#include "StdAfx.h"
#include "S5K5E9.h"
#include "LSC/Sensor/S5K5E9/LSC.h"
#pragma comment(lib,"LSC/Sensor/S5K5E9/GLSCv10_LIB_v140.lib")

S5K5E9::S5K5E9(void)
{
	flag=0;
}

S5K5E9::~S5K5E9(void)
{
}
int S5K5E9:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K5E9::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K5E9::GetTemperature(USHORT &temperature)
{
	return 1;
}
int S5K5E9::S5K4H7_ReadOTPPage(int page, USHORT *Buff)
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
int S5K5E9::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp1[64] = {0};
	int i=0;
	char section[256];
	if(S5K4H7_ReadOTPPage(0,temp1) != 1) return 0;					
	for ( i=0; i<8; i++)
	{ 
		sprintf( section,"%02X",temp1[i]);
		FuseID += section;
	}
	return 1;
}
int S5K5E9::Init()
{
	return 1;
}
#define GAIN_DEFAULT    0x0100
int S5K5E9::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}

BOOL bSaveRawFile2( CString sfilename, BYTE *pBuffer, UINT width, UINT height, UINT type )
{	
	if(NULL == pBuffer) return 0;
	HFILE rawFile;
	DWORD dwSizeImage = width * height * type;

	USES_CONVERSION;
	rawFile = _lcreat( T2A(sfilename.GetBuffer(sfilename.GetLength())), 0 );
	if( rawFile < 0 )
		return FALSE;

	UINT len;
	len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );

	_lclose( rawFile );

	return TRUE;	
}

int S5K5E9::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(I2cWrite(0x3400, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x0B00, 0x01, 3) != 1) return 0;
	return 1;
}

int S5K5E9::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(NULL == P10Buff || NULL == nLenCReg) return 0;

	USES_CONVERSION;
	CString tempStr;
	CString path;
	GetCurentPath(path); 
	int Midcount=GetTickCount();
	tempStr.Format(_T("%d"),Midcount);
	Addlog(tempStr);

	if (LSCGroup == 1 || LSCGroup == 2)
	{
	}
	else
	{
		tempStr.Format(_T("LSC Group设置错误: %d"),LSCGroup);
		Addlog(tempStr);
		return FALSE;
	}

	char BayerXMLFilePath[1024];
	char BinaryDataFilePath[1024];
	char SaveOutPutPath[1024];
	char LscSramPath[1024];
	char LscOtpPath[1024];

	path=path+_T("\LSC\\Sensor\\S5K5E9\\");
	CString Path_XML = path + _T("bayer_setting.xml");			//保存的数据。
	strcpy(BayerXMLFilePath,T2A(Path_XML.GetBuffer(Path_XML.GetLength())));
	Path_XML.ReleaseBuffer();

	CString Path_DAT  = path + _T("binary.dat");
	strcpy(BinaryDataFilePath,T2A(Path_DAT.GetBuffer(Path_DAT.GetLength())));
	Path_DAT.ReleaseBuffer();

	CString SaveOutPut = path + _T("output\\");
	CreateDirectory(SaveOutPut,NULL);
	strcpy(SaveOutPutPath,T2A(SaveOutPut.GetBuffer(SaveOutPut.GetLength())));
	SaveOutPut.ReleaseBuffer();

	CString Path_Sram= path + _T("sram.txt");
	strcpy(LscSramPath,T2A(Path_Sram.GetBuffer(Path_Sram.GetLength())));
	Path_Sram.ReleaseBuffer();

	CString Path_Otp   = path + _T("otp.txt");
	strcpy(LscOtpPath,T2A(Path_Otp.GetBuffer(Path_Otp.GetLength())));
	Path_Otp.ReleaseBuffer();

	CString Path_Raw  = path + _T("out.raw");

	const char *pResult;

	int m_GetLscGolden = nLenCReg[0];
	if (m_GetLscGolden)
	{
		LPBYTE temp[1];
		temp[0] = P10Buff;
		pResult  = RunMaster(nLSCTarget, 0, 0, 0, 0, 0, 0, 0, 0, (void **)temp, 1, 0, 0, 0, 0, 0, 0, BayerXMLFilePath, NULL, 0, BinaryDataFilePath);

		if (pResult == 0)
		{
			Addlog(_T("MODE0 Finish!"));
			pResult = GetOutCDEFormat(SaveOutPutPath);
		}
		else
		{
			tempStr.Format(_T("LSC校准失败: %s"),pResult);
			Addlog(tempStr);
			return FALSE;
		}
	}

	pResult = Initialize(NULL, BayerXMLFilePath, 0, BinaryDataFilePath);
	if (pResult == 0)
	{
		pResult = Run(P10Buff);
	}
	else
	{
		tempStr.Format(_T("LSC Initialize失败:%s"),pResult);
		Addlog(tempStr);
		return FALSE;
	}

	if (pResult != 0)
	{
		tempStr.Format(_T("LSC Run fail: %s"),pResult);
		Addlog(tempStr);
		return FALSE;
	}

	const char *pText;
	pResult = GetOutSetFileFormat(pText, 1);

	if(pResult == 0)
	{
		FILE *fpOut;
		fpOut = fopen(LscSramPath, "wt");
		if (NULL == fpOut)
		{
			tempStr.Format(_T("LSC SRAM保存失败: %s"));
			Addlog(tempStr);
			return FALSE;
		}

		fprintf(fpOut,"%s",pText);
		fclose(fpOut);
	}
	else
	{
		tempStr.Format(_T("LSC SRAM生成失败: %s"),pResult);
		Addlog(tempStr);
		return FALSE;
	}

	pResult = GetOutNVMWrite(pText, LSCGroup, 1);
	if(pResult == 0)
	{
		FILE *fpOut;
		fpOut = fopen(LscOtpPath, "wt");
		if (NULL == fpOut)
		{
			tempStr.Format(_T("LSC OTP保存失败: %s"));
			Addlog(tempStr);
			return FALSE;
		}

		fprintf(fpOut,"%s",pText);
		fclose(fpOut);
	}
	else
	{
		tempStr.Format(_T("LSC OTP生成失败: %s"),pResult);
		Addlog(tempStr);
		return FALSE;
	}

	LPBYTE outbuff = (LPBYTE)malloc(width * height * (sizeof(USHORT)));
	if (NULL != outbuff)
	{
		if (pResult == 0)
		{
			pResult = GetResult(P10Buff, outbuff);
		}
		bSaveRawFile2( Path_Raw,outbuff, width, height, sizeof(USHORT) );
		free(outbuff);
	}
	else
	{
		tempStr.Format(_T("out.raw保存失败!"));
		Addlog(tempStr);
	}
	

	USHORT Stream_Flag;
	if(I2cRead(0x0100,&Stream_Flag,3) != 1) return 0;
	if (Stream_Flag == 1)
	{
		if(I2cWrite(0x0100,0x00,3) != 1) return 0;
	}

	CStdioFile file;
	if ( !file.Open( (Path_Sram), CFile::modeRead | CFile::typeText ))
	{
		tempStr.Format(_T("Sram file打开失败!"));
		Addlog(tempStr);
		return  FALSE;
	}

	int     i = 0;
	int     j = 0;

	CString strLine = _T("");
	int count = 0;
	int	 intVal;
	USHORT  intLSCAddr;
	USHORT  intLSCValue;

	while ( file.ReadString(strLine) )
	{
		if ( strLine.Find( _T("s"), 0 ) != -1 ) // -1表示没找到；
		{
			int nLength = strLine.GetLength(); 
			nLength--;
			strLine = strLine.Right(nLength); 

			intVal = strtoul( T2A(strLine.GetBuffer(strLine.GetLength())), NULL, 16 );
			strLine.ReleaseBuffer();

			if( nLength == 6 )
			{  
				intLSCAddr  = intVal>>8;
				intLSCValue = intVal%256;			
				if(I2cWrite(intLSCAddr,intLSCValue,3) != 1) return 0;
				count++;
			}
		} 	
	}

	file.Close(); 

	if(I2cWrite(0x3400,0x00,3) != 1) return 0; 
	if(I2cWrite(0x0B00,0x01,3) != 1) return 0; 
	Sleep(500);      
	if(I2cWrite(0x0100,0x01,3) != 1) return 0; 

	Midcount=GetTickCount();
	tempStr.Format(_T("%d"),Midcount);
	Addlog(tempStr);
	return 1;
}


int S5K5E9::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int S5K5E9::ApplySpc(short *pSPC)
{
	return 1;
}


int S5K5E9::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3)  != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int S5K5E9::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3)  != 1) return 0;

	return 1;
} 

int S5K5E9::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3)  != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int S5K5E9::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 
void S5K5E9::WriteSensorSetting()
{

}
