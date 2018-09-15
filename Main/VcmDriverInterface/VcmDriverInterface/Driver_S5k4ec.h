#pragma once
#include "VcmDriverInterface.h"

class Driver_S5k4ec :public CVcmDriverInterface
{
public:
	Driver_S5k4ec(void);
	
	~Driver_S5k4ec(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus(); 
	USHORT sensorId;

};

