#pragma once
#include "vcmdriverinterface.h"
class DriverIC_DW9718 :
	public CVcmDriverInterface
{
public:
	DriverIC_DW9718(void);
	~DriverIC_DW9718(void);
	int Init();
	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);

	int AutoFocus();
	USHORT sensorId;
};

