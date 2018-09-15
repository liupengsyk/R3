#pragma once
#include "vcmdriverinterface.h"
class Driver_DW9714 :
	public CVcmDriverInterface
{
public:
	Driver_DW9714(void);
	~Driver_DW9714(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	int Standby();
	USHORT sensorId;
};

