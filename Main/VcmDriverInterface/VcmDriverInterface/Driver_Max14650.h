#pragma once
#include "vcmdriverinterface.h"

class Driver_Max14650:public CVcmDriverInterface
{
public:
	Driver_Max14650(void);
	~Driver_Max14650(void);

	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);

	int AutoFocus();
	USHORT sensorId;

};

