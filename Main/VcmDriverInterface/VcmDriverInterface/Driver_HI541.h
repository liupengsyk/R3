#pragma once
#include "vcmdriverinterface.h"
class Driver_HI541 :
	public CVcmDriverInterface
{
public:
	Driver_HI541(void);
	~Driver_HI541(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus(); 
	USHORT sensorId;
};

