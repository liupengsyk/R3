#include "StdAfx.h"
#include "IMX_268.h"
#include <vector>
using std::vector;


IMX_268::IMX_268(void)
{

}


IMX_268::~IMX_268(void)
{
}

int IMX_268::ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

    if(I2cWrite(0x0A02, page, 3) != 1) return 0;
    if(I2cWrite(0x0A00, 0x01, 3) != 1) return 0;
    int cnt=0;
    USHORT uValue = 0;
    if(I2cRead(0x0A01, &uValue, 3) != 1) return 0;
    while(0x0d == uValue)
    {
        cnt++;
        Sleep(1);
        if(cnt>20) return FALSE;
        if(I2cRead(0x0A01, &uValue, 3) != 1) return 0;
    }

    if (0x5d == uValue)
    {
        return 0;
    }

    for (USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {
        if(I2cRead(uAddr, &buf[uAddr-startAddr], 3) != 1) return 0;
    }

	return 1;
}
int IMX_268::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

    BYTE Status;
    //stream OFF
    if(I2cWrite(0x0100, 0x00, 3) != 1) return 0;
    //Set HZ
    if(I2cWrite(0x0136, 0x0C, 3) != 1) return 0;
    if(I2cWrite(0x0137, 0x00, 3) != 1) return 0;

    //Set Wrcnt
    if(I2cWrite(0x4902, 0x00, 3) != 1) return 0;
    if(I2cWrite(0x4903, 0x78, 3) != 1) return 0;
    //Write Status
    if(I2cWrite(0x0A00, 0x04, 3) != 1) return 0;
    //Set Page
    if(I2cWrite(0x0A02, page, 3) != 1) return 0;
    //Transfer Mode
    if(I2cWrite(0x0A00, 0x03, 3) != 1) return 0;

    for (USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {
        if(I2cWrite(uAddr, buf[uAddr-startAddr], 3) != 1) return 0;
    }

  //  Sleep(30);
    USHORT uValue = 0;
    int cnt=0;
    if(I2cRead(0x0A01, &uValue, 3) != 1) return 0;
    while(0x0d == uValue)
    {
        cnt++;
        Sleep(1);
        if(cnt>20) return 0;
        if(I2cRead(0x0A01, &uValue, 3) != 1) return 0;
    }

    if (0x6d == 0x0A01)
    {
        return 0;
    }

    return 1;
}

int IMX_268::GetTemperature(USHORT &temperature)
{
	return 1;
}

int IMX_268::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
    USHORT TmpBuf[64] = {0};
    if (TRUE != ReadReg(0x0A04, 0x0A06+63, TmpBuf, 15))
    {
        return 0;
    }

    for (USHORT uIdx=32; uIdx<43; ++uIdx)
    {
        FuseID.Format(_T("%s%02x"), FuseID.GetBuffer(), TmpBuf[uIdx]);
    }
    
    FuseID.MakeUpper();

    return 1;
}

int IMX_268::Init()
{
	return 1;
}

int IMX_268::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
    if(I2cWrite(0x0210, rg>>8, 3) != 1) return 0;
	if(I2cWrite(0x0211, rg&0xff, 3) != 1) return 0;
	
	if(I2cWrite(0x0212, bg>>8, 3) != 1) return 0;
	if(I2cWrite(0x0213, bg&0xff, 3) != 1) return 0;
	//	
    if(I2cWrite(0x020E, Typical_rg>>8, 3) != 1) return 0;
	if(I2cWrite(0x020F, Typical_rg&0xff, 3) != 1) return 0;
	//
	if(I2cWrite(0x0214, Typical_bg>>8, 3) != 1) return 0;
	if(I2cWrite(0x0215, Typical_bg&0xff, 3) != 1) return 0;
	return 1;
}

int IMX_268::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(NULL == raw8) return 0;

    typedef bool(*pGetLSCData)(unsigned char *Raw10Buffer, int ImageWidth, int ImageHeight, int BayerType,  int percent,  unsigned char offset,  unsigned char *pLenCReg, int *nLenCRegCount);
    CString strDllFile = _T("");
    strDllFile.Format(_T("%sLSC\\Sensor\\IMX_268\\IMX_CalLSCPara.dll"), pCcmCtr->Ctrl_GetCurrentPath());
    HMODULE hDllMod = ::LoadLibrary(strDllFile);
    if (NULL == hDllMod)
    {
		FreeLibrary(hDllMod);
        pCcmCtr->Ctrl_Addlog(CamID, _T("IMX_CalLSCPara.dll No Find."), COLOR_RED, 200);
        return FALSE;
    }

    pGetLSCData GetLSCData = (pGetLSCData)GetProcAddress(hDllMod, "LenC_Cal_IMX268_Raw10");
    if (NULL == GetLSCData)
    {
		FreeLibrary(hDllMod);
        return FALSE;
    }
    
#define LSCDATASIZE 320

    int LSCDataCount = 0;
    BYTE LSCData[LSCDATASIZE] = {0};
    vector<BYTE> vtLscData;
    BOOL bRetFlag = GetLSCData(raw8, width, height, 0, nLSCTarget, 64, LSCData, &LSCDataCount);
	FreeLibrary(hDllMod);

    if ((FALSE == bRetFlag) || (320 != LSCDataCount))
    {
        return FALSE;
    }

    for (USHORT uAddr=0xA100; uAddr<=0xA23F; ++uAddr)
    {
        if(I2cWrite(uAddr, LSCData[uAddr-0xA100], 3) != 1)	return 0;
    }
    
    if(I2cWrite(0x0220, 0x00, 3 ) != 1) return 0;	//Hdr Disable
    if(I2cWrite(0x0b00, 0x01, 3 ) != 1) return 0;	//LSC Enable
    if(I2cWrite(0x3021, 0x00, 3 ) != 1) return 0;	//LSC Table1
    USHORT uValue = 0;
    if(I2cRead(SlaveID, 0x3025, &uValue, 3) != 1) return 0;
    if (0x00 != uValue)
    {
        return FALSE;
    }

    int i=0;
    int j=0;
    for (i=0;i<320;i=i+8)
    {
        vtLscData[j++] = LSCData[i+1];
        vtLscData[j++] = LSCData[i+3];
        vtLscData[j++] = LSCData[i+5];
        vtLscData[j++] = LSCData[i+7];
        vtLscData[j++] = (LSCData[i]<<6)+(LSCData[i+2]<<4)+(LSCData[i+4]<<2)+(LSCData[i+6]);
    }

    CString strLSCPath;
    int siteNo = pCcmCtr->GetSiteNo();
    strLSCPath.Format(_T("SensorLSC_%d.bin"), siteNo);
    pCcmCtr->Otp_WriteToOtpBoard(CamID, strLSCPath, vtLscData);

	return 1;
}

int IMX_268::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_268::ApplySpc(short *pSPC)
{
	return 1;
}

int IMX_268::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202, &ExpHigh, 3) != 1) return 0;
	if(I2cRead(0x0203, &ExpLow, 3) != 1) return 0;
	exp = (ExpHigh<<8) + (ExpLow&0xFF);

	return 1;
} 

int IMX_268::WriteExp( int exp)
{
	USHORT ExpHigh = exp>>8;
	USHORT ExpLow = exp&0xFF;
	if(I2cWrite(0x0202, ExpHigh, 3) != 1) return 0;
	if(I2cWrite(0x0203, ExpLow, 3) != 1) return 0;

	return 1;
} 

int IMX_268::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_268::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 