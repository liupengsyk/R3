
#include "StdAfx.h"
#include "Driver_AXD898123XC.h"

#define   OIS_ADDR 0x7c
#define  CMD_AF_POSITION  0xF01A
#define		CMD_READ_STATUS					0xF100				// Status Read
#define		READ_STATUS_INI					0x01000000

Driver_AXD898123XC::Driver_AXD898123XC(void)
{

}
 
Driver_AXD898123XC::~Driver_AXD898123XC(void)
{
}

int Driver_AXD898123XC::Init()
{ 
	 
	 
	return 0;
}

int Driver_AXD898123XC::ReadCode(USHORT *code)
{

	return 0;
}

int Driver_AXD898123XC::WriteCode(USHORT code)
{ 
	//code = 1000 + code;
	SetTregAf( code*2); 
	return 0;
}

int Driver_AXD898123XC::AutoFocus()
{	
	return 0;
}

//********************************************************************************
// Function Name 	: RdStatus
// Retun Value		: 0:success 1:FAILURE
// Argment Value	: bit check  0:ALL  1:bit24
// Explanation		: High level status check Function
// History			: First edition 						
//********************************************************************************
UINT8	Driver_AXD898123XC::RdStatus( UINT8 UcStBitChk)
{
	ULONG	UlReadVal ;
	
	RamRead32A( CMD_READ_STATUS , &UlReadVal);
//TRACE(" (Rd St) = %08x\n", (unsigned INT16)UlReadVal ) ;
	if( UcStBitChk ){
		UlReadVal &= READ_STATUS_INI ;
	}
	if( !UlReadVal ){
		return( 0 );
	}else{
		return( 1 );
	}
}

void	Driver_AXD898123XC::SetTregAf( UINT16 UsTregAf)
{
	UINT8	UcStRd = 1;

	RamWrite32A( CMD_AF_POSITION,	UsTregAf| 0x00010000) ;		// bit 16 : FST mode
	//RamWrite32A( CMD_AF_POSITION,	UsTregAf| 0x00000000) ;
	while( UcStRd ) {
		UcStRd = RdStatus(1);
	}
}

void Driver_AXD898123XC::RamRead32A(unsigned short addr, unsigned long* data)
{
	BYTE myData[10] = {0};
	//ReadSensorI2c(OIS_ADDR, addr, 2,myData,4);
	int flag = I2cReadBlock(OIS_ADDR,addr,2,myData,4);
	*data = ((myData[0]<<24) | (myData[1]<<16)| (myData[2]<<8)| myData[3]);
}

void Driver_AXD898123XC::RamWrite32A(unsigned short addr, unsigned long data)
{
	BYTE myData[10] = {0};

	myData[0] = (data>>24)&0xff;
	myData[1] = (data>>16)&0xff;
	myData[2] = (data>>8)&0xff;
	myData[3] = data&0xff;
	//WriteSensorI2c(OIS_ADDR,addr,2,myData,4);
	int flag = I2cWriteBlock(OIS_ADDR,addr,2,myData,4);
}
