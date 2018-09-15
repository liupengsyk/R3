#include "StdAfx.h"
#include "Sample.h"


Sample::Sample(void)
{
}


Sample::~Sample(void)
{
}

int Sample:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int Sample::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int Sample::GetTemperature(USHORT &temperature)
{
	return 1;
}
int Sample::GetFuseID(CString &FuseID) 
{
	return 1;
}
int Sample::Init()
{
	return 1;
}
int Sample::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int Sample::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int Sample::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int Sample::ApplySpc(short *pSPC)
{
	return 1;
}

int Sample::ReadExp( int &exp){return 1;} 
int Sample::WriteExp( int exp){return 1;} 

int Sample::ReadGain( USHORT &gain){return 1;} 
int Sample::WriteGain( USHORT gain){return 1;} 