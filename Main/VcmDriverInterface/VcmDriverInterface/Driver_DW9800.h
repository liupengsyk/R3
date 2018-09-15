#pragma once
#include "vcmdriverinterface.h"
class Driver_DW9800 :
	public CVcmDriverInterface
{
public:
	Driver_DW9800(void);
	~Driver_DW9800(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);

	int AutoFocus();
	USHORT sensorId;
};

