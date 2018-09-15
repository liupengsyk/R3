#pragma once
#include "vcmdriverinterface.h"
class Driver_BU64295296297GWZ :
	public CVcmDriverInterface
{
public:
	Driver_BU64295296297GWZ(void);
	~Driver_BU64295296297GWZ(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;
};

