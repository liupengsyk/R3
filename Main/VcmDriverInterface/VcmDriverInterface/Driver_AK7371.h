#pragma once
#include "vcmdriverinterface.h"
class Driver_AK7371 :
	public CVcmDriverInterface
{
public:
	Driver_AK7371(void);
	~Driver_AK7371(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;

};

