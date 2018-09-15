#pragma once
#include "vcmdriverinterface.h"
class Driver_WV511A :
	public CVcmDriverInterface
{
public:
	Driver_WV511A(void);
	~Driver_WV511A(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;
};

