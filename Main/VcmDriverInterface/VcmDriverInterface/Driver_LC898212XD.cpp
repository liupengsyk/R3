#include "StdAfx.h"
#include "Driver_LC898212XD.h"


DriverIC_LC898212XD::DriverIC_LC898212XD(void)
{

}


DriverIC_LC898212XD::~DriverIC_LC898212XD(void)
{

}

int DriverIC_LC898212XD::Load()
{
	StorageSlaveID = 0xa0;
	StorageI2CMode = 0;
	Reg_Max_H    = 0x05; 
	Reg_Max_L    = 0x06; 
	Reg_Min_H    = 0x03; 
	Reg_Min_L    = 0x04; 
	Reg_Hall_OFF = 0x02; 
	Reg_Hall_BIA = 0x01; 

	return 0;
}

int DriverIC_LC898212XD::Init()
{
	Load();

	USHORT Max_H = 0xff, Max_L = 0xff, Min_H = 0xff, Min_L = 0xff;

	CL898212_wait=FALSE;
	UcSmvEnb =0x3c;



	
	I2cRead(StorageSlaveID, Reg_Max_H, &Max_H, 3); //读取OTP内部HALL MAX、MIN数据数据	
	I2cRead(StorageSlaveID, Reg_Max_L, &Max_L, 3); //读取OTP内部HALL MAX、MIN数据数据
	I2cRead(StorageSlaveID, Reg_Min_H, &Min_H, 3); //读取OTP内部HALL MAX、MIN数据数据  Reg_Min_H
	I2cRead(StorageSlaveID, Reg_Min_L, &Min_L, 3); //读取OTP内部HALL MAX、MIN数据数据

	
 
	USHORT Max = (USHORT)((Max_H << 8) | Max_L);
	USHORT Min = (USHORT)((Min_H << 8) | Min_L);

	Hall_Max = Max;
	Hall_Min = (short)Min; 

	BOOL bRet;

	USHORT Hall_OFF,Hall_BIA;
	I2cRead(StorageSlaveID, Reg_Hall_OFF, &Hall_OFF, 3);
	I2cRead(StorageSlaveID, Reg_Hall_BIA, &Hall_BIA, 3);

	USHORT HALL_Calibration = ((USHORT)Hall_OFF<<8) + (USHORT)Hall_BIA;


	Hall_Max = 0x6F4A;
	Hall_Min = 0x91AA; 
	Hall_BIA =0x7A;
	Hall_OFF= 0xA2;

	if ((Hall_OFF==0xff)||(Hall_BIA==0xff)||(Max_H==0xff)||(Max_L==0xff))
	{	
		CString info = _T("马达OTP未校正，请重新校正!");
		//AddLog(info, COLOR_RED, 330);
		return -3;
	}

	if ((Max==0x00)||(Min==0x00))
	{	
		CString info = _T("马达OTP数据错误，请重新校正!");
		//AddLog(info, COLOR_RED, 330);
		return -3;
	}

	

	bRet = I2cWrite(0xe4,0x80,0x34,0); 	// CLKSEL 1/1, CLKON
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x81,0x20,0); 	// AD 4Time
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x84,0xE0,0); 	// STBY   AD ON,DA ON,OP ON
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x87,0x05,0); 	// PIDSW OFF,AF ON,MS2 ON
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0xA4,0x24,0); 	// Internal OSC Setup (No01=24.18MHz)
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x3A,0x0000,2); // OFFSET Clear
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x04,Max,2); // RZ Clear(Target Value)//2013.11.22 利于调焦
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x02,0x0000,2); // PIDZO Clear
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x18,0x0000,2); // MS1Z22 Clear(STMV Target Value)
	Return_Check(bRet);
	Sleep(10);  		// Wait 5 ms

	// Filter Setting: ST1130114-1.h
	bRet = I2cWrite(0xe4,0x86, 0x0040,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x40, 0x7ff0,2);// Inverter=0  //0x8010  0x7ff0
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x42, 0x7150,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x44, 0x8F90,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x46, 0x61B0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x48, 0x65B0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x4A, 0x2870,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x4C, 0x4030,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x4E, 0x7FF0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x50, 0x04F0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x52, 0x7610,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x54, 0x16C0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x56, 0x0000,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x58, 0x7FF0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x5A, 0x0680,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x5C, 0x72F0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x5E, 0x7F70,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x60, 0x7ED0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x62, 0x7FF0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x64, 0x0000,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x66, 0x0000,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x68, 0x5130,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x6A, 0x72F0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x6C, 0x8010,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x6E, 0x0000,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x70, 0x0000,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x72, 0x18E0,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x74, 0x4E30,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x30, 0x0000,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x76, 0x0C50,2);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x78, 0x4000,2);
	Return_Check(bRet);
	Sleep(10);  		// Wait 5 ms

	bRet = I2cWrite(0xe4,0x86, 0x60,0);	// DSSEL 1/16 INTON
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x88, 0x70,0);	// ANA1   Hall Bias:2mA Amp Gain: x100(Spring Type)
	Return_Check(bRet);
	//{ 0x0028, 0x8080 },	// Hall Offset/Bias	
	bRet = I2cWrite(0xe4,0x28,HALL_Calibration,2);//
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x4C, 0x4000,2);	// Loop Gain
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x83, 0x2C,0);	// RZ OFF,STSW=ms2x2,MS1IN OFF,MS2IN=RZ,FFIN AFTER,DSW ag
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x85, 0xC0,0);	// AF filter,MS1 Clr
	Return_Check(bRet);
	Sleep(10);   		// Wait 1 ms

	bRet = I2cWrite(0xe4,0x84, 0xE3,0);	// STBY   AD ON,DA ON,OP ON,DRMODE H,LNSTBB H
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x97, 0x00,0);	// DRVSEL
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x98, 0x42,0);	
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x99, 0x00,0);
	Return_Check(bRet);
	bRet = I2cWrite(0xe4,0x9A, 0x00,0);
	Return_Check(bRet);

#ifdef EN_STEP_MOVE
	//Step move setting	2013.11.26
	USHORT UcSetEnb,UcSetSwt,SsParStt;//CurHall,
	I2cWrite(0xe4,0x8A, 0x00,0);// StepMove Enable Bit Clear
	I2cRead(0xe4,0x87, &UcSetEnb,0);
	UcSetEnb &=0xFD;
	I2cWrite(0xe4,0x87, UcSetEnb,0);// Measuremenet Circuit1 Off
	I2cRead(0xe4,0x83, &UcSetSwt,0);
	UcSetSwt &=0x7F;
	I2cWrite(0xe4,0x83, UcSetSwt,0);// RZ1 Switch Cut Off
	I2cRead(0xe4,0x04,&SsParStt,2);//Get Start Position     158c
	I2cWrite(0xe4,0x5a, 0x0800,2);// Set Coefficient Value For StepMove
	I2cWrite(0xe4,0x18, SsParStt,2);// Set Start Positon
	I2cWrite(0xe4,0x16, STMV_SIZE,2);// Set StepSize
	I2cWrite(0xe4,0xA0, STMV_INTERVAL,0);// Set StepInterval
	UcSetSwt	|= 0x80;//(unsigned char)0x80;
	I2cWrite(0xe4,0x83, UcSetSwt,0);// RZ1 Switch ON	客户端程序驱动部分
#endif
	Sleep(10);  		// Wait 5 ms
	bRet = I2cWrite(0xe4,0x87, 0x85,0);      
	Return_Check(bRet);


	return 0;
}

int DriverIC_LC898212XD::ReadCode(USHORT* code)
{
	int	Min_Pos = 0, Max_Pos = 1024;

	USHORT Temp=0;;

	//BOOL bRet = I2cRead(0xE4, 0x03 ,&Temp, 2);
	BOOL bRet = I2cRead(0xE4, 0x04 ,&Temp, 2);

	int k = ((Hall_Max - Hall_Min) / (Max_Pos - Min_Pos));

	if (k==0) //除数为0
	{
		*code = 0;
		return -2;
	}

	*code = int ((Hall_Max - (short)Temp ) / k );

	return bRet;
}

int DriverIC_LC898212XD::WriteCode(USHORT code)
{
	USHORT	Min_Pos = 0, Max_Pos = 1024;


#ifndef EN_STEP_MOVE    //Direct Move

	USHORT Temp;
	int k = (Hall_Max - Hall_Min) / (Max_Pos - Min_Pos);
	Temp = (USHORT)(Hall_Max-(code * k));
	return I2cWrite(0xE4, 0x04 ,Temp, 2);


#else    //Step Move
	if(CL898212_wait)
	{
		USHORT ivalue2,ivalue3;
		//signed short	Hall_Max = 0x6900; // Please read INF position from EEPROM or OTP
		//signed short  	Hall_Min = 0x9300; // Please read MACRO position from EEPROM or OTP
		// 		#if 0  	// 1: Move from 0x8001 -> 0x7FFF 
		//    			ivalue2 = (((ivalue - Min_Pos) * (Hall_Max - Hall_Min) / (Max_Pos - Min_Pos)) + Hall_Min) & 0xFFFF;
		// 		#else  // 0: Move Move from  0x7FFF -> 0x8001
		ivalue3 = (((Max_Pos - code) * (Hall_Max - Hall_Min) / (Max_Pos - Min_Pos)) + Hall_Min) & 0xFFFF;
		//		#endif
		ivalue2 = ivalue3;

		unsigned short	UsSmvTim;
		unsigned short	UsSmvDpl;
		USHORT 			SsParStt;	// StepMove Start Position

		//PIOA_SetOutput(_PIO_PA29);													// Monitor I/O Port
		I2cRead(0xE4,0x04,&SsParStt,2);							// Get Start Position	0x04
		UsSmvDpl = abs( SsParStt - ivalue2 );
		if( ( UsSmvDpl <= STMV_SIZE ) && (( UcSmvEnb & STMSV_ON ) == STMSV_ON ) ){
			if( UcSmvEnb & STMCHTG_ON ){
				 I2cWrite(0xE4,0x8F,0x00|0x01,0);
			}
			I2cWrite(0xE4,0x18,ivalue2,2);// Handling Single Step For ES1		0x18
			UcSmvEnb	|= STMVEN_ON;				  // Combine StepMove Enable Bit & StepMove Mode Bit
		} else {
			if( SsParStt < ivalue2 ){												// Check StepMove Direction
				I2cWrite(0xE4,0x16,STMV_SIZE,2);
			} else if( SsParStt > ivalue2 ){
				USHORT temp = -STMV_SIZE;
				I2cWrite(0xE4,0x16,temp,2);//(-STMV_SIZE)
			}
			// Set StepMove Target Positon		0xA1
			I2cWrite(0xE4,0xA1,ivalue2,2);
			UcSmvEnb	|= STMVEN_ON;							// Combine StepMove Enable Bit & StepMove Mode Bit
			// Start StepMove					0x8A
			I2cWrite(0xE4,0x8A,UcSmvEnb,0);//3C
		}
		UsSmvTim=(UsSmvDpl/STMV_SIZE)*((STMV_INTERVAL+1)*10000 / LC898211_fs);			// Stepmove Operation time
		//WaitTime( UsSmvTim );
		Sleep(UsSmvTim);
	}
	CL898212_wait = TRUE;	
#endif   

	return 1;

}

int DriverIC_LC898212XD::AutoFocus() 
{
	return 0;
}