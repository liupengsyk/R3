#pragma once
#include "vcmdriverinterface.h"

#define EN_STEP_MOVE   //Step Moveģʽ

#define	LC898211_fs		234375
#define STMV_SIZE			(unsigned short)0x0180					// StepSize(MS1Z12)
#define STMV_INTERVAL		(unsigned char)0x01						// Step Interval(STMVINT)
//#define UcSmvEnb		(unsigned char)0x0C    //((unsigned char)0x08 | (unsigned char)0x04 | (unsigned char)0x00)
#define STMSV_ON			(unsigned char)0x04
#define	STMCHTG_ON			(unsigned char)0x08						// STMVEN Register Set
#define STMVEN_ON			(unsigned char)0x01 
#define Return_Check(x) { if (!x) return -1;}
class DriverIC_LC898212XD :	public CVcmDriverInterface
{
public:
	DriverIC_LC898212XD(void);
	~DriverIC_LC898212XD(void);
	int ReadCode(USHORT* code);
	int WriteCode(USHORT code);
	int Init();
	int Load();

	USHORT StorageSlaveID;
	BYTE StorageI2CMode;

	USHORT Reg_Max_H;
	USHORT Reg_Max_L;
	USHORT Reg_Min_H;
	USHORT Reg_Min_L;
	USHORT Reg_Hall_OFF;
	USHORT Reg_Hall_BIA;

	signed short Hall_Max;
	signed short Hall_Min,temp;

	/*unsigned short Hall_Max;
	unsigned short Hall_Min,temp;*/

	unsigned char UcSmvEnb;
	BOOL CL898212_wait;
	int AutoFocus() ;
};

