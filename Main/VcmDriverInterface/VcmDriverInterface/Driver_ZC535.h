#pragma once
#include "vcmdriverinterface.h"
class Driver_ZC535 :
	public CVcmDriverInterface
{
public:
	Driver_ZC535(void);
	~Driver_ZC535(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;
};

