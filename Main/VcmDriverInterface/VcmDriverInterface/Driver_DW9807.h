#pragma once
#include "vcmdriverinterface.h"
class Driver_DW9807 :
	public CVcmDriverInterface
{
public:
	Driver_DW9807(void);
	~Driver_DW9807(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);

	int AutoFocus();
	USHORT sensorId;
};

