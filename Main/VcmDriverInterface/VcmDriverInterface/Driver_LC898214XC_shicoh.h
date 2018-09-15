#pragma once
#include "vcmdriverinterface.h"
class Driver_LC898214XC_shicoh :
	public CVcmDriverInterface
{
public:
	Driver_LC898214XC_shicoh(void);
	~Driver_LC898214XC_shicoh(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	 
	int Init();
	USHORT sensorId;

private:
	short OpHome;
	short OpTop ;
	float CodeIndex;
};

