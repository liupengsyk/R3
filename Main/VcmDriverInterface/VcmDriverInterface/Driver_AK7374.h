#pragma once
#include "vcmdriverinterface.h"
class Driver_AK7374 :
	public CVcmDriverInterface
{
public:
	Driver_AK7374(void);
	~Driver_AK7374(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;

};

