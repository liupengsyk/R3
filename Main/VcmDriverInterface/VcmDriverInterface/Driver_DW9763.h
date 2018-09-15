#pragma once
#include "vcmdriverinterface.h"
class Driver_DW9763 :
	public CVcmDriverInterface
{
public:
	Driver_DW9763(void);
	~Driver_DW9763(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);

	int AutoFocus();
	USHORT sensorId;
};

