#include "StdAfx.h"
#include "GC5025.h"


GC5025::GC5025(void)
{
}


GC5025::~GC5025(void)
{
}

int GC5025:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf) return 0;

	BYTE Addr_hig=(startAddr>>8)&0x03;
	BYTE Addr_low=startAddr&0xFF;
	USHORT Value;
	int SensorMode=0;
	if(I2cWrite(0xf7,0x01,SensorMode) != 1) return 0;                                           
	if(I2cWrite(0xf9,0x00,SensorMode) != 1) return 0;                                           
	if(I2cWrite(0xfc,0x2e,SensorMode) != 1) return 0;                                           
	if(I2cWrite(0xfa,0xb0,SensorMode) != 1) return 0; //OTP clk enable                          
                                     
	if(I2cWrite(0xd4,0x84,SensorMode) != 1) return 0; //OTP enable[7] OTP page select[2]        
                                                                                         
	if(I2cWrite(0xd4,0x84+Addr_hig,SensorMode)!= 1) return 0;//OTP address select high bit [9:8]
	if(I2cWrite(0xd5,Addr_low,SensorMode)!= 1) return 0;//OTP address select low bit [7:0]      
                                                                                       
	if(I2cWrite(0xf3,0x20,SensorMode)!= 1) return 0;//OTP read                                  
                                                                                        
	if(I2cRead(0xd7,buf,SensorMode)!= 1) return 0;//OTP value                                   

	return 1;
}
int GC5025::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf) return 0;
	BYTE Addr_hig=(startAddr>>8)&0x03;
	BYTE Addr_low=startAddr&0xFF;
	int ret=1;
	USHORT readback[2]={0};
	int SensorMode=0;
	if(I2cWrite(0xf7,0x01,SensorMode) != 1) return 0;                                                  
	if(I2cWrite(0xf9,0x00,SensorMode) != 1) return 0;                                                 
	if(I2cWrite(0xfc,0x2e,SensorMode) != 1) return 0;                                                   
	if(I2cWrite(0xfa,0xb0,SensorMode) != 1) return 0;  //OTP clk enable                              	   
	if(I2cWrite(0xd4,0x84,SensorMode) != 1) return 0;  //OTP enable[7] OTP page select[2]            	   
	if(I2cWrite(0xd4,0x84+Addr_hig,SensorMode) != 1) return 0; //OTP address select high bit [9:8]  
	if(I2cWrite(0xd5,Addr_low,SensorMode) != 1) return 0;//OTP address select low bit [7:0]      
	if(I2cWrite(0xd6,buf[0],SensorMode) != 1) return 0;//OTP value                               
	Sleep(5);//Delay 1ms                                                       
	if(I2cWrite(0xd8,0x20,SensorMode) != 1) return 0;//Select T1                                 
	for (int i=0;i<5;i++)//Repeat 5 times                                      
	{                                                                          
		if(I2cWrite(0xf3,0x42,SensorMode) != 1) return 0;//OTP write                               
		Sleep(25);//Delay 24ms                                                   
	}                                                                          
	if(I2cWrite(0xd8,0x10,SensorMode)!= 1) return 0;                                            
	Sleep(5);//Delay 3ms (Check if the value is correct or not;)               

	if(I2cWrite(0xf3,0x20,SensorMode) != 1) return 0;//OTP read                                  
	if(I2cRead(0xd7,readback,SensorMode) != 1) return 0;//OTP value	                             
	if (readback[0]!=buf[0])                                                   
	{                                                                          
		ret=0;                                                                   
	}                                                                          

	if(I2cWrite(0xd8,0x40,SensorMode) != 1) return 0; //Select T2 (First)                         

	for (int i=0;i<5;i++)//Repeat 5 times                                      
	{                                                                          
		if(I2cWrite(0xf3,0x42,SensorMode) != 1) return 0;//OTP write                               
		Sleep(25);//Delay 24ms                                                   
	}                                                                          
	if(I2cWrite(0xd8,0x10,SensorMode) != 1) return 0;                                           
	Sleep(5);//Delay 3ms (Check if the value is correct or not;)               
	if(I2cWrite(0xf3,0x20,SensorMode) != 1) return 0;//OTP read                                  
	if(I2cRead(0xd7,readback,SensorMode)  != 1) return 0;//OTP value	                             
	if (readback[0]!=buf[0])
	{
		ret=0;
		return 0;
	}

	return 1;
}

int GC5025::GetTemperature(USHORT &temperature)
{

	return 1;
}
int GC5025::GetFuseID(CString &FuseID) 
{
	char section[512] = {0};
	char temp[512] = {0};
	USHORT tempVal[9];
		
	for (int j=0;j<9;j++)
	{
		if(ReadReg(0x03B0+j*8,0x03B0+j*8,tempVal+j) != 1) return 0;
		sprintf(section, "%02X",tempVal[j]);
		FuseID += section;
	}
	return 1;
}
int GC5025::Init()
{
	return 1;
}
int GC5025::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int GC5025::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int GC5025::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int GC5025::ApplySpc(short *pSPC)
{
	return 1;
}

int GC5025::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x03,&ExpHigh,0) != 1) return 0;   
	if(I2cRead(0x04,&ExpLow, 0) != 1) return 0;   

	exp=(ExpHigh<<8)+(ExpLow&0xFF);
	return 1;
} 
int GC5025::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;

	if(I2cWrite(0x03,ExpHigh,0) != 1) return 0; 
	if(I2cWrite(0x04,ExpLow, 0) != 1) return 0; 

	return 1;
}


int GC5025::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	if(I2cRead(0xB6,&GainHigh,0) != 1) return 0;
	gain=GainHigh;
	return 1;
} 
int GC5025::WriteGain( USHORT gain)
{
	if(I2cWrite(0xB6,gain&0xff,0) != 1) return 0;
	return 1;
} 