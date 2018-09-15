#include "stdafx.h"
#include <stdio.h>
#define  SumLSCTool_FILE  1
#include "SumLSCTool.h"
#include "LSC.h"
#include <shlwapi.H>
#pragma  comment(lib,"LSCDLL_5E8.lib")
#pragma  comment(lib,"LSCDLL_4H8_v24.lib")
#pragma	 comment(lib,"shlwapi.lib")

CString GetModulePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[512] = {0};
	char path[512]={0};
	GetModuleFileName(NULL, ModulePath, 512);//return  real  lenghth
	len = strlen(ModulePath);
	for (i = len - 1; i >= 0; i--)
	{
		if (ModulePath[i] == '\\')
		{
			ModulePath[i + 1] = 0;
			flag = true;
			strcpy(path, ModulePath);	
			break;
		}
	}
	
	if (!flag)
	{
		strcpy(path, "");
		
	}
	myStr.Format(path);
	return myStr;
}
BOOL bSaveRawFile( CString sfilename, BYTE *pBuffer, UINT width, UINT height, UINT type )
{	
	HFILE rawFile;
	DWORD dwSizeImage = width * height * type;
	
	rawFile = _lcreat( sfilename, 0 );
	if( rawFile < 0 )
		return FALSE;
	
	UINT len;
	len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );
	
	_lclose( rawFile );
	
	return TRUE;	
}

SumLSCToolDLLAPI int _stdcall  S5K5E8_LSC(BYTE *Raw10Buffer,int iMageWidth,int iMageHeight,int LSCTarget,char *Path,char* ErrorMsg)
{
	/************************************************************************/
	/*                          Apply Lsc                                   */
	/************************************************************************/
/*	AddString("******Lsc Test******\n");*/
	
	CString tempStr1 = "";
	tempStr1.Format("%s",Path);
	tempStr1 += "S5K5E8LSCOTP.ini";

	if(PathFileExists(tempStr1))
	{
		if(!DeleteFile(tempStr1))
		{	
			strcpy(ErrorMsg,"É¾³ýS5K5E8LSCOTP.iniÎÄ¼þÊ§°Ü");
			return FALSE;
		}
	}

	CString tempStr2 = "";
	tempStr2.Format("%s",Path);
	tempStr2 += "S5K5E8LSCsram.ini";

	if (PathFileExists(tempStr2))
	{
		if (DeleteFile(tempStr2))
		{
			strcpy(ErrorMsg,"É¾³ýS5K5E8LSCsram.iniÎÄ¼þÊ§°Ü");
			return FALSE;
		}
	}

	CString tempStr3 = "";
	tempStr3.Format("%s",Path);
	tempStr3 += "InputLSCRaw10.raw";
	if (!bSaveRawFile(tempStr3,Raw10Buffer,iMageWidth,iMageHeight,2))
	{
		strcpy(ErrorMsg,"±£´æRaw10Í¼Æ¬Ê§°Ü");
		return FALSE;

	}
	BOOL result = TRUE;
	
	SetParameter_5E8( 64, iMageWidth, iMageHeight, LSCTarget, LSCTarget, LSCTarget, LSCTarget, 0.3, 0.01, 1,0);
	CString temdllpath=GetModulePath();
	if( Run_5E8(Raw10Buffer,Raw10Buffer,0,temdllpath) != TRUE )
	{
		strcpy(ErrorMsg,"Run_5E8 Fail");
		return FALSE;
	}
	
	result = SaveOTPSetfile_5E8( tempStr1.GetBuffer(tempStr1.GetLength() ) );
	if (!result)
	{

		strcpy(ErrorMsg,"SaveOTPSetfile_5E8 ²âÊÔÊ§°Ü");
		return FALSE;
	}
		
	result = SaveSRAMSetfile_5E8( tempStr2.GetBuffer(tempStr2.GetLength() ) );
	if (!result)
	{
		
		strcpy(ErrorMsg,"SaveSRAMSetfile_5E8 ²âÊÔÊ§°Ü");
		return FALSE;
	}

	return TRUE;
}
SumLSCToolDLLAPI int _stdcall  S5K4H8_LSC(BYTE *Raw10Buffer,int iMageWidth,int iMageHeight,int LSCTarget,int Group,char *Path,char* ErrorMsg)
{

	/************************************************************************/
	/*                          Apply Lsc                                   */
	/************************************************************************/
	/*	AddString("******Lsc Test******\n");*/
	
	CString tempStr1 = "";
	tempStr1.Format("%s",Path);
	tempStr1 += "S5K4H8LSCOTP.ini";
	
	if(PathFileExists(tempStr1))
	{
		if(!DeleteFile(tempStr1))
		{	
			strcpy(ErrorMsg,"É¾³ýS5K4H8LSCOTP.iniÎÄ¼þÊ§°Ü");
			return FALSE;
		}
	}
	
	CString tempStr2 = "";
	tempStr2.Format("%s",Path);
	tempStr2 += "S5K4H8LSCsram.ini";
	
	if (PathFileExists(tempStr2))
	{
		if (!DeleteFile(tempStr2))
		{
			strcpy(ErrorMsg,"É¾³ýS5K4H8LSCsram.iniÎÄ¼þÊ§°Ü");
			return FALSE;
		}
	}
	
	CString tempStr3 = "";
	tempStr3.Format("%s",Path);
	tempStr3 +="LensRI.txt";
	if (!PathFileExists(tempStr3))
	{
		strcpy(ErrorMsg,"LensRI.txt²»´æÔÚ");
		return FALSE;
	}

	CString tempStr4 = "";
	tempStr4.Format("%s",Path);
	tempStr4 += "InputLSCRaw10.raw";
	if (!bSaveRawFile(tempStr4,Raw10Buffer,iMageWidth,iMageHeight,2))
	{
		strcpy(ErrorMsg,"±£´æRaw10Í¼Æ¬Ê§°Ü");
		return FALSE;
		
	}


	FILE* fp = fopen(tempStr3, "rt");
	if(fp==NULL)
	{
		strcpy(ErrorMsg,"´ò¿ªLensRI.txtÊ§°Ü");
		return FALSE;
	}
	else
	{
		double* pLensRI = new double[11];
		char strData[256];
		for(int i=0; i<11; ++i)
		{
			fscanf(fp, "%s", strData);
			sscanf(strData, "%lf", pLensRI+i);
		}
		
		SetUserLensRI(pLensRI[0], pLensRI[1], pLensRI[2], pLensRI[3],
			pLensRI[4], pLensRI[5], pLensRI[6], pLensRI[7],
			pLensRI[8], pLensRI[9], pLensRI[10]);
		delete [] pLensRI;
	}
	fclose(fp);

	SetParameter(64,iMageWidth,iMageHeight,LSCTarget,LSCTarget,LSCTarget,LSCTarget,0.3,0.01,TRUE,FALSE);

	
	BOOL result = TRUE;


	BOOL SetGroup = 0;
	if ( Group == 1 )
	{
		SetGroup = FALSE;
	}
	if ( Group == 2 )
	{
		SetGroup = TRUE;
	}
	BOOL	is_SINGULAR ;
	BOOL	bSEED_overflow ;
	CString tempStr5 = "";
	tempStr5.Format("%s",Path);
	if( Run(Raw10Buffer, Raw10Buffer, FALSE, tempStr5 , SetGroup, &is_SINGULAR, &bSEED_overflow) != TRUE )
	{
		strcpy(ErrorMsg,"ÔËÐÐRunº¯ÊýÊ§°Ü");
		return FALSE;
	}
	
	if( is_SINGULAR == TRUE )
	{
		strcpy(ErrorMsg, "It is SINGULAR(not invertible)!!" );
		return FALSE;
	}
	
	if( bSEED_overflow == 0x1 )
	{
		strcpy(ErrorMsg, "Fail for LSC seed overflow!" );
		return FALSE;
	}
	
	result = SaveOTPSetfile( tempStr1.GetBuffer( tempStr1.GetLength() ) );

	if (!result)
	{
		
		strcpy(ErrorMsg,"SaveOTPSetfile_4H8 ²âÊÔÊ§°Ü");
		return FALSE;
	}
	

	result = SaveSRAMSetfile( tempStr2.GetBuffer( tempStr2.GetLength() ) );
	if (!result)
	{
		
		strcpy(ErrorMsg,"SaveSRAMSetfile_4H8 ²âÊÔÊ§°Ü");
		return FALSE;
	}
	


	return TRUE;
}