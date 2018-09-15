#pragma once
#include "vcmdriverinterface.h"
class Driver_DW9767 :
	public CVcmDriverInterface
{
public:
	Driver_DW9767(void);
	~Driver_DW9767(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);

	int AutoFocus();
	USHORT sensorId;
};

