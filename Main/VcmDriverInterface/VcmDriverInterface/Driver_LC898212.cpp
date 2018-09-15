#include "StdAfx.h"
#include "Driver_LC898212.h"
 

stSmvPar Driver_LC898212::StSmvPar;
Driver_LC898212::Driver_LC898212(void)
{
}


Driver_LC898212::~Driver_LC898212(void)
{
}

void Driver_LC898212::GetOtpData()
{
	USHORT tempData = 0;
	USHORT tempData_ = 0;
	USHORT checksum = 0;

	I2cRead(RamSlaveID,0x00,&tempData,3);
	checksum +=tempData;
	I2cRead(RamSlaveID,0x01,&tempData,3);
	checksum +=tempData;
	Hall_Basic = tempData;
	I2cRead(RamSlaveID,0x02,&tempData,3);
	checksum +=tempData;
	Hall_Offset = tempData;
	I2cRead(RamSlaveID,0x03,&tempData,3);
	checksum +=tempData;
	I2cRead(RamSlaveID,0x04,&tempData_,3);
	checksum +=tempData_;
	//I2cRead(0xa0,0x04,&tempData,0);
	//HallMin = (tempData[3]<<8)|(tempData[4]);
	HallMin = (tempData<<8)|tempData_;
	HallMin = 0x91AA;
	I2cRead(RamSlaveID,0x05,&tempData,3);
	checksum +=tempData;
	I2cRead(RamSlaveID,0x06,&tempData_,3);
	checksum +=tempData_;
	//I2cRead(0xa0,0x06,&tempData,0);
	//HallMax = (tempData[5]<<8)|(tempData[6]);
	HallMax = (tempData<<8)|tempData_;
	HallMax =0x6F4A;
	 
	I2cRead(RamSlaveID,0x07,&tempData,3); 
	/*if((checksum&0xff)!=tempData)
	{
	HallMax = (tempData<<8)|tempData_;
	}*/
		

}

void Driver_LC898212::LoadCalibrationData()
{
	HallOff = Config_ReadConfigInt( _T("HallCalibration"),_T("HallOff"),0,CamID);
	HallBias = Config_ReadConfigInt( _T("HallCalibration"),_T("HallBias"),0,CamID);
	HallMax = Config_ReadConfigInt( _T("HallCalibration"),_T("Hallmax"),0,CamID);
	HallMin = Config_ReadConfigInt( _T("HallCalibration"),_T("Hallmin"),0,CamID);
	Init0x40 = Config_ReadConfigInt( _T("HallCalibration"),_T("Init0x40"),0,CamID);
	m_vcmType = Config_ReadConfigString(_T("HallCalibration"), _T("m_VCM_Type"), _T(""), CamID);
	Init0x40 = 1;
	if (Init0x40)
	{
		Data0x40 = 0x8010;
	}
	else
	{
		Data0x40 = 0x4030;
	}
}
 
int Driver_LC898212::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = 0xe4;
	RamSlaveID = 0xa0;
	stSmvPar StSmv0Par;
	//GetOtpData();
	LoadCalibrationData();
	//int HallOff = Hall_Offset;	 	// Please Read Offset from EEPROM or OTP
	//int HallBias = Hall_Basic;   // Please Read Bias from EEPROM or OTP
	
	AfInit( HallOff,  HallBias);	// Initialize driver IC

	// Step move parameter set
	StSmv0Par.UsSmvSiz	= STMV_SIZE ; 
	StSmv0Par.UcSmvItv	= STMV_INTERVAL ;  
	StSmv0Par.UcSmvEnb	= STMCHTG_SET | STMSV_SET | STMLFF_SET ;
	StmvSet( StSmv0Par ) ;
	// 			
	ServoOn();	// Close loop ON


	return 0;
}

int Driver_LC898212::ReadCode(USHORT *code)
{
	int	MinPos = 0;
	int MaxPos = 1024;

	USHORT Temp;

	/*if(0){
	RamWriteA( PIDZO_211H, 0x8001 );
	}else{
	RamWriteA( PIDZO_211H, 0x7fff );
	}*/

	//RamReadA( ADOFFSET_211H ,	&UsHigSmp[0] ) ;
	//BOOL bRet = I2cRead(0xE4, 0x04 ,&Temp, 2);
	BOOL bRet=1;/* = I2cRead(0xE4, 0x04,&Temp,2 );*///ADOFFSET_211H
	RamReadA( RZ_211H ,	(unsigned short *)&Temp );	
	float k = (HallMax - HallMin)/(MaxPos - MinPos);

	if (k==0) //除数为0
	{
		*code = 0;
		return -2;
	}

	//*code = int ((HallMax - (short)Temp ) / k );
	*code = int ((HallMax - (unsigned short)Temp ) / k );
	if (*code>1000)
	{
		*code=0;
	}

	return bRet;
}

int Driver_LC898212::WriteCode(USHORT code)
{
	Sleep(30);
	StmvTo( AF_convert(code) ) ;	// Move to Target Position
	return 0;
}

int Driver_LC898212::AutoFocus()
{	 
	return 0;
}

BOOL Driver_LC898212::AfInit( unsigned char hall_off, unsigned char hall_bias )
{
#define		DataLen		sizeof(Init_Table) / sizeof(IniData)
	unsigned short i;
	unsigned short pos;

	if (m_vcmType == _T(""))  
	{

		//	unsigned short temp;
		const struct INIDATA Init_Table[] = {
			//Addr,	  Data
			//	{ 0xfffe, 0x03 },
			//TDK_CL-ACT_ 212_SPtype_Ini_13011
			{ 0x0080, 0x34 }, 	// CLKSEL 1/1, CLKON
			{ 0x0081, 0x20 }, 	// AD 4Time

			{ 0x0084, 0xE0 }, 	// STBY   AD ON,DA ON,OP ON
			{ 0x0087, 0x05 }, 	// PIDSW OFF,AF ON,MS2 ON
			{ 0x00A4, 0x24 }, 	// Internal OSC Setup (No01=24.18MHz)
			//	{ 0xfffe, 0x04 },
			{ 0x003A, 0x0000 }, // OFFSET Clear
			{ 0x0004, 0x0000 }, // RZ Clear(Target Value)
			{ 0x0002, 0x0000 }, // PIDZO Clear
			{ 0x0018, 0x0000 }, // MS1Z22 Clear(STMV Target Value)
			//  { WAIT, 5 },  		// Wait 5 ms
			{ 0x0088, 0x0070 },
			{ 0x0028, 0x8080 },
			{ 0x004C, 0x4000 },
			//Modified from here
			{ 0x0083, 0x002c },
			{ 0x0085, 0x00c0 },//如果为0，继续
			{ 0x0084, 0x00e3 },
			{ 0x0097, 0x0000 },
			{ 0x0098, 0x0042},
			{ 0x0099, 0x0000 },
			{ 0x009a, 0x0000 },
			{ 0x0086, 0x0040 },
			{ 0x0040, 0x7ff0 },// 0x4030 //0x7ff0  0x8010
			{ 0x0042, 0x7150 },
			{ 0x0044, 0x8f90 },
			{ 0x0046, 0x61b0 },
			{ 0x0048, 0x65b0 },
			{ 0x004a, 0x32f0 },
			{ 0x004C, 0x4030 },
			{ 0x004e, 0x7ff0 },
			{ 0x0050, 0x04f0 },
			{ 0x0052, 0x7610 },
			{ 0x0054, 0x2030 },
			{ 0x0056, 0x0000 },
			{ 0x0058, 0x7ff0 },
			{ 0x005a, 0x0680 },
			{ 0x005C, 0x72f0 },

			//
			{ 0x005e, 0x7f70 },
			{ 0x0060, 0x7ed0},
			{ 0x0062, 0x7ff0 },
			{ 0x0064, 0x0000 },
			{ 0x0066, 0x0000 },
			{ 0x0068, 0x5130 },
			{ 0x006a, 0x72f0 },
			{ 0x006C, 0x8010 },
			{ 0x006e, 0x0000 },
			{ 0x0070, 0x0000 },
			{ 0x0072, 0x18e0 },
			{ 0x0074, 0x4e30 },
			{ 0x0030, 0x0000 },
			{ 0x0076, 0x0c50 },
			{ 0x0078, 0x2000 },
			{ 0x0086, 0x0060 },

		};
		for(i = 0; i < DataLen; i++)
		{
			if(Init_Table[i].addr == WAIT)
			{
				Sleep(Init_Table[i].data);
				continue;
			}

			if(Init_Table[i].addr >= 0x80)
			{
				RegWriteA(Init_Table[i].addr, (unsigned char)(Init_Table[i].data & 0x00ff));
				if (Init_Table[i].addr == 0x85)
				{
					CString tempstr;
					//unsigned char mytemp=1;
					mytemp=1;
					int ncounty=0;
					do 
					{
						Sleep(1);
						RegReadA(0x85,&mytemp);
					/*	tempstr.Format(_T("0x85读取直至为0,mytemp=%d"),mytemp);
						m_pInterface->Ctrl_Addlog(CamID,tempstr,COLOR_BLUE,220);*/
						ncounty++;
						if (ncounty>=20)
						{
							return  FALSE;
						}

					} while (mytemp!=0);			
				}

			} 
			else 
			{
				RamWriteA(Init_Table[i].addr, (unsigned short)Init_Table[i].data);
			} 
		}


		RegWriteA( 0x28, hall_off);		// Hall Offset
		RegWriteA( 0x29, hall_bias);	// Hall Bias
		//	temp = ((hall_off << 8) | hall_bias);
		//	RegWriteA( 0x28, temp );
		//	WriteSensorReg(0xe4,0x28,temp,4);

		RamReadA( 0x3C, &pos); 
		RamWriteA( 0x04, pos); // Direct move target position
		RamWriteA( 0x18, pos); // Step move start position	  

		Sleep(10);
		return TRUE;
	}
	else if (m_vcmType == _T("TVC651"))//vcm tvc_651
	{
		//	unsigned short temp;
		const struct INIDATA Init_Table[] = {
			//Addr,	  Data
			//	{ 0xfffe, 0x03 },
			//TDK_CL-ACT_ 212_SPtype_Ini_13011
			{ 0x0080, 0x34 }, 	// CLKSEL 1/1, CLKON
			{ 0x0081, 0x20 }, 	// AD 4Time

			{ 0x0084, 0xE0 }, 	// STBY   AD ON,DA ON,OP ON
			{ 0x0087, 0x05 }, 	// PIDSW OFF,AF ON,MS2 ON
			{ 0x00A4, 0x24 }, 	// Internal OSC Setup (No01=24.18MHz)
			//	{ 0xfffe, 0x04 },
			{ 0x003A, 0x0000 }, // OFFSET Clear
			{ 0x0004, 0x0000 }, // RZ Clear(Target Value)
			{ 0x0002, 0x0000 }, // PIDZO Clear
			{ 0x0018, 0x0000 }, // MS1Z22 Clear(STMV Target Value)
			//  { WAIT, 5 },  		// Wait 5 ms
			{ 0x0088, 0x0070 },
			{ 0x0028, 0x8080 },
			{ 0x004C, 0x4000 },
			//Modified from here
			{ 0x0083, 0x002c },
			{ 0x0085, 0x00c0 },//如果为0，继续
			{ 0x0084, 0x00e3 },
			{ 0x0097, 0x0000 },
			{ 0x0098, 0x0042},
			{ 0x0099, 0x0000 },
			{ 0x009a, 0x0000 },
			{ 0x0086, 0x0040 },
			{ 0x0040, 0x7ff0 },// 0x4030 //0x7ff0  0x8010
			{ 0x0042, 0x7150 },
			{ 0x0044, 0x8f90 },
			{ 0x0046, 0x61b0 },
			{ 0x0048, 0x3930 },//65b0
			{ 0x004a, 0x2410 },//32f0
			{ 0x004C, 0x4030 },
			{ 0x004e, 0x7ff0 },
			{ 0x0050, 0x04f0 },
			{ 0x0052, 0x7610 },
			{ 0x0054, 0x1450 },//2030
			{ 0x0056, 0x0000 },
			{ 0x0058, 0x7ff0 },
			{ 0x005a, 0x0680 },
			{ 0x005C, 0x72f0 },

			//
			{ 0x005e, 0x7f70 },
			{ 0x0060, 0x7ed0},
			{ 0x0062, 0x7ff0 },
			{ 0x0064, 0x0000 },
			{ 0x0066, 0x0000 },
			{ 0x0068, 0x5130 },
			{ 0x006a, 0x72f0 },
			{ 0x006C, 0x8010 },
			{ 0x006e, 0x0000 },
			{ 0x0070, 0x0000 },
			{ 0x0072, 0x18e0 },
			{ 0x0074, 0x4e30 },
			{ 0x0030, 0x0000 },
			{ 0x0076, 0x0c50 },
			{ 0x0078, 0x2000 },
			{ 0x0086, 0x0060 },

		};
		for(i = 0; i < DataLen; i++)
		{
			if(Init_Table[i].addr == WAIT)
			{
				Sleep(Init_Table[i].data);
				continue;
			}

			if(Init_Table[i].addr >= 0x80)
			{
				RegWriteA(Init_Table[i].addr, (unsigned char)(Init_Table[i].data & 0x00ff));
				if (Init_Table[i].addr == 0x85)
				{
					CString tempstr;
					//unsigned char mytemp=1;
					mytemp=1;
					int ncounty=0;
					do 
					{
						Sleep(1);
						RegReadA(0x85,&mytemp);
						/*tempstr.Format(_T("0x85读取直至为0,mytemp=%d"),mytemp);
						m_pInterface->Ctrl_Addlog(CamID,tempstr,COLOR_BLUE,220);*/
						ncounty++;
						if (ncounty>=20)
						{
							return  FALSE;
						}

					} while (mytemp!=0);			
				}

			} 
			else 
			{
				RamWriteA(Init_Table[i].addr, (unsigned short)Init_Table[i].data);
			} 
		}


		RegWriteA( 0x28, hall_off);		// Hall Offset
		RegWriteA( 0x29, hall_bias);	// Hall Bias
		//	temp = ((hall_off << 8) | hall_bias);
		//	RegWriteA( 0x28, temp );
		//	WriteSensorReg(0xe4,0x28,temp,4);

		RamReadA( 0x3C, &pos); 
		RamWriteA( 0x04, pos); // Direct move target position
		RamWriteA( 0x18, pos); // Step move start position	  

		Sleep(10);
		return TRUE;
	}
	else if (m_vcmType == _T("TVC820"))//tvc_820
	{
		//	unsigned short temp;
		const struct INIDATA Init_Table[] = {
			//Addr,	  Data
			//	{ 0xfffe, 0x03 },
			//TDK_CL-ACT_ 212_SPtype_Ini_13011
			{ 0x0080, 0x34 }, 	// CLKSEL 1/1, CLKON
			{ 0x0081, 0x20 }, 	// AD 4Time

			{ 0x0084, 0xE0 }, 	// STBY   AD ON,DA ON,OP ON
			{ 0x0087, 0x05 }, 	// PIDSW OFF,AF ON,MS2 ON
			{ 0x00A4, 0x24 }, 	// Internal OSC Setup (No01=24.18MHz)
			//	{ 0xfffe, 0x04 },
			{ 0x003A, 0x0000 }, // OFFSET Clear
			{ 0x0004, 0x0000 }, // RZ Clear(Target Value)
			{ 0x0002, 0x0000 }, // PIDZO Clear
			{ 0x0018, 0x0000 }, // MS1Z22 Clear(STMV Target Value)
			//  { WAIT, 5 },  		// Wait 5 ms
			{ 0x0088, 0x0070 },
			{ 0x0028, 0x8080 },
			{ 0x004C, 0x4000 },
			//Modified from here
			{ 0x0083, 0x002c },
			{ 0x0085, 0x00c0 },//如果为0，继续
			{ 0x0084, 0x00e3 },
			{ 0x0097, 0x0000 },
			{ 0x0098, 0x0042},
			{ 0x0099, 0x0000 },
			{ 0x009a, 0x0000 },
			{ 0x0086, 0x0040 },
			{ 0x0040, 0x4030 },// 0x4030 //0x7ff0  0x8010
			{ 0x0042, 0x7150 },
			{ 0x0044, 0x8f90 },
			{ 0x0046, 0x61b0 },
			{ 0x0048, 0x65b0 },
			{ 0x004a, 0x7FF0 },
			{ 0x004C, 0x4030 },
			{ 0x004e, 0x8010 },
			{ 0x0050, 0x04f0 },
			{ 0x0052, 0x7610 },
			{ 0x0054, 0x2030 },
			{ 0x0056, 0x0000 },
			{ 0x0058, 0x7ff0 },
			{ 0x005a, 0x0800 },
			{ 0x005C, 0x72f0 },

			//
			{ 0x005e, 0x7f70 },
			{ 0x0060, 0x7ed0},
			{ 0x0062, 0x7ff0 },
			{ 0x0064, 0x0000 },
			{ 0x0066, 0x0000 },
			{ 0x0068, 0x5130 },
			{ 0x006a, 0x72f0 },
			{ 0x006C, 0x8010 },
			{ 0x006e, 0x0000 },
			{ 0x0070, 0x0000 },
			{ 0x0072, 0x18e0 },
			{ 0x0074, 0x4e30 },
			{ 0x0030, 0x0000 },
			{ 0x0076, 0x3050 },
			{ 0x0078, 0x2000 },
			{ 0x0086, 0x0060 },

		};
		for(i = 0; i < DataLen; i++)
		{
			if(Init_Table[i].addr == WAIT)
			{
				Sleep(Init_Table[i].data);
				continue;
			}

			if(Init_Table[i].addr >= 0x80)
			{
				RegWriteA(Init_Table[i].addr, (unsigned char)(Init_Table[i].data & 0x00ff));
				if (Init_Table[i].addr == 0x85)
				{
					CString tempstr;
					//unsigned char mytemp=1;
					mytemp=1;
					int ncounty=0;
					do 
					{
						Sleep(1);
						RegReadA(0x85,&mytemp);
						/*tempstr.Format(_T("0x85读取直至为0,mytemp=%d"),mytemp);
						m_pInterface->Ctrl_Addlog(CamID,tempstr,COLOR_BLUE,220);*/
						ncounty++;
						if (ncounty>=20)
						{
							return  FALSE;
						}

					} while (mytemp!=0);			
				}

			} 
			else 
			{
				RamWriteA(Init_Table[i].addr, (unsigned short)Init_Table[i].data);
			} 
		}


		RegWriteA( 0x28, hall_off);		// Hall Offset
		RegWriteA( 0x29, hall_bias);	// Hall Bias
		//	temp = ((hall_off << 8) | hall_bias);
		//	RegWriteA( 0x28, temp );
		//	WriteSensorReg(0xe4,0x28,temp,4);

		RamReadA( 0x3C, &pos); 
		RamWriteA( 0x04, pos); // Direct move target position
		RamWriteA( 0x18, pos); // Step move start position	  

		Sleep(10);
		return TRUE;
	}
	else if (m_vcmType == _T("TVC650ABB"))//TVC650ABB
	{
		//	unsigned short temp;
		const struct INIDATA Init_Table[] = {
			{0x80,0x34},
			{0x81,0x20},
			{0x84,0xE0},
			{0x87,0x05},
			{0xA4,0x24},
			{0x3A,0x0000},
			{0x04,0x0000},
			{0x02,0x0000},
			{0x18,0x0000},
			{0x86,0x40},
			{0x40,0xBFD0},
			{0x42,0x7150},
			{0x44,0x8F90},
			{0x46,0x61B0},
			{0x48,0x65B0},
			{0x4A,0x32F0},
			{0x4C,0x4000},
			{0x4E,0x7FF0},
			{0x50,0x04F0},
			{0x52,0x7610},
			{0x54,0x2030},
			{0x56,0x0000},
			{0x58,0x7FF0},
			{0x5A,0x0680},
			{0x5C,0x72F0},
			{0x5E,0x7F70},
			{0x60,0x7ED0},
			{0x62,0x7FF0},
			{0x64,0x0000},
			{0x66,0x0000},
			{0x68,0x5130},
			{0x6A,0x72F0},
			{0x6C,0x8010},
			{0x6E,0x0000},
			{0x70,0x0000},
			{0x72,0x18E0},
			{0x74,0x4E30},
			{0x30,0x0000},
			{0x76,0x0C50},
			{0x78,0x2000},
			{0x86,0x60},
			{0x88,0x70},
			{0x28,0x8020},
			{0x83,0x2C},
			{0x85,0xC0},
			{WAIT,0x1},
			{0x84,0xE3},
			{0x97,0x00},
			{0x98,0x42},
			{0x99,0x00},
			{0x9A,0x00}
		};
		for(i = 0; i < DataLen; i++)
		{
			if(Init_Table[i].addr == WAIT)
			{
				Sleep(Init_Table[i].data);
				continue;
			}

			if(Init_Table[i].addr >= 0x80)
			{
				RegWriteA(Init_Table[i].addr, (unsigned char)(Init_Table[i].data & 0x00ff));
				//if (Init_Table[i].addr == 0x85)
				//{
				//	CString tempstr;
				//	//unsigned char mytemp=1;
				//	mytemp=1;
				//	int ncounty=0;
				//	do 
				//	{
				//		Sleep(1);
				//		RegReadA(0x85,&mytemp);
				//		/*tempstr.Format(_T("0x85读取直至为0,mytemp=%d"),mytemp);
				//		m_pInterface->Ctrl_Addlog(CamID,tempstr,COLOR_BLUE,220);*/
				//		ncounty++;
				//		if (ncounty>=20)
				//		{
				//			return  FALSE;
				//		}

				//	} while (mytemp!=0);			
				//}
			}
			else
			{
				RamWriteA(Init_Table[i].addr, (unsigned short)Init_Table[i].data);
			}
		}


		RegWriteA( 0x28, hall_off);		// Hall Offset
		RegWriteA( 0x29, hall_bias);	// Hall Bias
		//	temp = ((hall_off << 8) | hall_bias);
		//	RegWriteA( 0x28, temp );
		//	WriteSensorReg(0xe4,0x28,temp,4);

		RamReadA( 0x3C, &pos); 
		RamWriteA( 0x04, pos); // Direct move target position
		RamWriteA( 0x18, pos); // Step move start position	  

		//WaitTime(10);
		Sleep(10);
		return TRUE;
	}
	return FALSE;
}

void Driver_LC898212::RegWriteA(unsigned short addr, unsigned short data)
{
/*
	BYTE myData[10] = {0};
//	myData[0] = data;

	if ( addr < 0x80 )
	{
		myData[0] = (data>>8)&0xff;
		myData[1] = data&0xff;
		WriteSensorI2c(0xe4,(addr&0xff),1,myData,2);
	}
	else
	{
		myData[0] = (data&0xff);
		WriteSensorI2c(0xe4,(addr&0xff),1,myData,1);
	}
*/
	//BYTE myData[10] = {0};
	////	myData[0] = data;
	//
	//
	//myData[0] = data;
	//WriteSensorI2c(0xe4,(addr&0xff),1,myData,1);   //is this right?
	I2cWrite(0xe4,(addr&0xff),data,0);
}



void Driver_LC898212::RegReadA(unsigned short addr, unsigned char *data)
{
	//ReadSensorI2c(0xE4, (addr&0xff), 1,data,1);
	I2cRead(0xe4,(addr&0xff),(USHORT*)data,0);
//	ReadSensorReg(0xe4,addr,data,0);
}
void Driver_LC898212::RamReadA(unsigned short addr, unsigned short *data)
{
	/*BYTE myData[10] = {0};
	ReadSensorI2c(0xE4, (addr&0xff), 1,myData,2);
	*data = ((myData[0]<<8) | myData[1]);*/
//	*data = myData[0];
	I2cRead(0xe4,(addr&0xff),data,2);
}

void Driver_LC898212::RamWriteA(unsigned short addr, unsigned short data)
{
	//BYTE myData[10] = {0};
	////	myData[0] = data;	
	//myData[0] = (data>>8)&0xff;
	//myData[1] = data&0xff;
	//WriteSensorI2c(0xe4,(addr&0xff),1,myData,2);

	I2cWrite(0xe4,(addr&0xff),data,2);
	
}
 
//********************************************************************************
// Function Name 	: StmvSet
// Retun Value		: NON
// Argment Value	: NON
// Explanation		: Stpmove parameter Setting Function
// History			: First edition 						2012.06.12 YS.Kim
// History			: Changes								2013.07.19 Rex.Tang
//********************************************************************************
void Driver_LC898212:: StmvSet( stSmvPar StSetSmv )
{
	/*unsigned char	UcSetEnb;
	unsigned char	UcSetSwt;*/
	unsigned short	UsParSiz;
	unsigned char	UcParItv;
    short 			SsParStt;	// StepMove Start Position

    StSmvPar.UsSmvSiz = StSetSmv.UsSmvSiz;
    StSmvPar.UcSmvItv = StSetSmv.UcSmvItv;
    StSmvPar.UcSmvEnb = StSetSmv.UcSmvEnb;
    
	RegWriteA( AFSEND_211	, 0x00 );										// StepMove Enable Bit Clear
	
	RegReadA( ENBL_211 ,	&UcSetEnb );
	UcSetEnb 	&= (unsigned char)0xFD ;
	RegWriteA( ENBL_211	,	UcSetEnb );										// Measuremenet Circuit1 Off
	
	RegReadA( SWTCH_211 ,	&UcSetSwt );
	UcSetSwt	&= (unsigned char)0x7F ;
	RegWriteA( SWTCH_211 , UcSetSwt );										// RZ1 Switch Cut Off
	
	RamReadA( RZ_211H ,	(unsigned short *)&SsParStt );						// Get Start Position
	UsParSiz	= StSetSmv.UsSmvSiz ;										// Get StepSize
	UcParItv	= StSetSmv.UcSmvItv ;										// Get StepInterval
	
	RamWriteA( ms11a_211H	, (unsigned short)0x0800 );						// Set Coefficient Value For StepMove
	RamWriteA( MS1Z22_211H	, (unsigned short)SsParStt );					// Set Start Positon
	RamWriteA( MS1Z12_211H	, UsParSiz );									// Set StepSize
	RegWriteA( STMINT_211	, UcParItv );									// Set StepInterval
	
	UcSetSwt	|= (unsigned char)0x80;
	RegWriteA( SWTCH_211, UcSetSwt );										// RZ1 Switch ON
}



//********************************************************************************
// Function Name 	: StmvTo
// Retun Value		: Stepmove Parameter
// Argment Value	: Stepmove Parameter, Target Position
// Explanation		: Stpmove Function
// History			: First edition 						2012.06.12 YS.Kim
// History			: Changes								2013.07.19 Rex.Tang
//********************************************************************************
unsigned char Driver_LC898212:: StmvTo( short SsSmvEnd )
{
#if 0 
	unsigned short	UsSmvTim;
	unsigned short	UsSmvDpl;
    short 			SsParStt;	// StepMove Start Position
	
	//PIOA_SetOutput(_PIO_PA29);													// Monitor I/O Port
	
	RamReadA( RZ_211H ,	(unsigned short *)&SsParStt );								// Get Start Position
	UsSmvDpl = abs( SsParStt - SsSmvEnd );
	
	if( ( UsSmvDpl <= StSmvPar.UsSmvSiz ) && (( StSmvPar.UcSmvEnb & STMSV_ON ) == STMSV_ON ) )
	{
		if( StSmvPar.UcSmvEnb & STMCHTG_ON )
		{
			RegWriteA( MSSET_211	, INI_MSSET_211 | (unsigned char)0x01 );
		}
		RamWriteA( MS1Z22_211H, SsSmvEnd );										// Handling Single Step For ES1
		StSmvPar.UcSmvEnb	|= STMVEN_ON;										// Combine StepMove Enable Bit & StepMove Mode Bit
	} 
	else 
	{
		if( SsParStt < SsSmvEnd )
		{												// Check StepMove Direction
			RamWriteA( MS1Z12_211H	, StSmvPar.UsSmvSiz );
		} 
		else if( SsParStt > SsSmvEnd )
		{
			RamWriteA( MS1Z12_211H	, -StSmvPar.UsSmvSiz );
		}
		
		RamWriteA( STMVENDH_211, SsSmvEnd );									// Set StepMove Target Positon
		StSmvPar.UcSmvEnb	|= STMVEN_ON;										// Combine StepMove Enable Bit & StepMove Mode Bit
		RegWriteA( STMVEN_211	, StSmvPar.UcSmvEnb );							// Start StepMove
	}
	
	UsSmvTim=(UsSmvDpl/STMV_SIZE)*((STMV_INTERVAL+1)*10000 / LC898211_fs);			// Stepmove Operation time
	Sleep( UsSmvTim );
	//TRACE("STMV Operation Time = %d \n", UsSmvTim ) ;
	
	return StmvEnd( StSmvPar.UcSmvEnb );
#endif
	unsigned short	UsSmvTim;
	unsigned short	UsSmvDpl;
	short 			SsParStt;	// StepMove Start Position

	//PIOA_SetOutput(_PIO_PA29);													// Monitor I/O Port

	RamReadA( RZ_211H ,	(unsigned short *)&SsParStt );											// Get Start Position
	UsSmvDpl = abs( SsParStt - SsSmvEnd );

	if( ( UsSmvDpl <= StSmvPar.UsSmvSiz ) && (( StSmvPar.UcSmvEnb & STMSV_ON ) == STMSV_ON ) )
	{
		if( StSmvPar.UcSmvEnb & STMCHTG_ON )
		{
			RegWriteA( MSSET_211	, INI_MSSET_211 | (unsigned char)0x01 );
		}
		RamWriteA( MS1Z22_211H, SsSmvEnd );										// Handling Single Step For ES1
		StSmvPar.UcSmvEnb	|= STMVEN_ON;										// Combine StepMove Enable Bit & StepMove Mode Bit
	} 
	else 
	{
		if( SsParStt < SsSmvEnd )
		{												// Check StepMove Direction
			RamWriteA( MS1Z12_211H	, StSmvPar.UsSmvSiz );
		} 
		else if( SsParStt > SsSmvEnd )
		{
			RamWriteA( MS1Z12_211H	, -StSmvPar.UsSmvSiz );
		}

		RamWriteA( STMVENDH_211, SsSmvEnd );									// Set StepMove Target Positon
		StSmvPar.UcSmvEnb	|= STMVEN_ON;										// Combine StepMove Enable Bit & StepMove Mode Bit
		RegWriteA( STMVEN_211	, StSmvPar.UcSmvEnb );							// Start StepMove
	}

	UsSmvTim=(UsSmvDpl/STMV_SIZE)*((STMV_INTERVAL+1)*10000 / LC898211_fs);			// Stepmove Operation time
	Sleep( UsSmvTim );
	CString str;
	str.Format(_T("UsSmvTim %d"),UsSmvTim);
	Addlog(str,(0,0,150),200);
	//TRACE("STMV Operation Time = %d \n", UsSmvTim ) ;

	return StmvEnd( StSmvPar.UcSmvEnb );
}



//********************************************************************************
// Function Name 	: StmvEnd
// Retun Value		: Stepmove Parameter
// Argment Value	: Stepmove Parameter
// Explanation		: Stpmove Finish Check Function
// History			: First edition 						2012.06.12 YS.Kim
// History			: Changes								2013.07.19 Rex.Tang
//********************************************************************************
unsigned char Driver_LC898212:: StmvEnd( unsigned char UcParMod )
{
	/*unsigned char	UcChtGst;*/
	unsigned short  i = 0;
	
	while( (UcParMod & (unsigned char)STMVEN_ON ) && (i++ < 100))								// Wait StepMove operation end
	{
		RegReadA( STMVEN_211 , &UcParMod );
	}

	if( ( UcParMod & (unsigned char)0x08 ) == (unsigned char)STMCHTG_ON )
	{		// If Convergence Judgement is Enabled
        for(i=0; i<CHTGOKN_WAIT; i++)
		{
        	RegReadA( MSSET_211, &UcChtGst );
        	if(!(UcChtGst & 0x01))	break;
        	Sleep(1);	
		}
	}
	
	if( UcChtGst & 0x01 )
	{
		UcParMod	|= (unsigned char)0x80 ;									// STMV Success But Settling Time Over
		//PIOA_ClearOutput(_PIO_PA29);											// Monitor I/O Port
	}
	else
	{
		UcParMod	&= (unsigned char)0x7F ;									// STMV Success 
	}
	
	return UcParMod;															// Bit0:0 Successful convergence Bit0:1 Time Over
}

void Driver_LC898212:: ServoOn(void)
{
    Sleep(5);
   	RegWriteA( 0x87, 0x85 );		// Servo ON
	//RegWriteA( 0x87, 0x05 ); // Servo off
} 

unsigned short Driver_LC898212::AF_convert(int position)   
{

#if 0     // 1: INF -> Macro =  0x8001 -> 0x7FFF
	//position = position - 512;
	//USHORT hallValue = (((position - Min_Pos) * (unsigned short)(HallMax - HallMin) / (Max_Pos - Min_Pos)) + HallMin) & 0xFFFF;
	USHORT hallValue = (((position - Min_Pos) * (unsigned short)( HallMin- HallMax) / (Max_Pos - Min_Pos)) + HallMax) & 0xFFFF;
	CString strHall;
	strHall.Format(_T("当前hall值 = %04x"),hallValue);
	Addlog(strHall,(0,0,150),200);
	//	unsigned short temp = 0;
	//	temp = (((position - Min_Pos) * (unsigned short)(HallMax - HallMin) / (Max_Pos - Min_Pos)) + HallMin) & 0xFFFF;
	return hallValue;
#else  // 0: INF -> Macro =  0x7FFF -> 0x8001

	//	unsigned short temp = 0;
	//	temp =(((Max_Pos - position) * (unsigned short)(HallMax - HallMin) / (Max_Pos - Min_Pos)) + HallMin) & 0xFFFF;

	//return ((((Max_Pos - position) * (unsigned short)(HallMax - HallMin) / (Max_Pos - Min_Pos)) + HallMin) & 0xFFFF);

	USHORT hallValue = ((((Max_Pos - position) * (unsigned short)(HallMax - HallMin) / (Max_Pos - Min_Pos)) + HallMin) & 0xFFFF);
	CString strHall;
	strHall.Format(_T("当前hall值 = %04x"),hallValue);
	Addlog(strHall,(0,0,150),200);
	return hallValue;
#endif

}

