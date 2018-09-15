#pragma once
#include "vcmdriverinterface.h"
class Driver_DW9719 :
	public CVcmDriverInterface
{
public:
	Driver_DW9719(void);
	~Driver_DW9719(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;
};

