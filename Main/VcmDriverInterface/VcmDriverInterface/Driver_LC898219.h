#pragma once
#include "vcmdriverinterface.h"
class Driver_LC898219 :
	public CVcmDriverInterface
{
public:
	Driver_LC898219(void);
	~Driver_LC898219(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;
};

