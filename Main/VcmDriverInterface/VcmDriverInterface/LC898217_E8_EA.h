 
#pragma once
#include "vcmdriverinterface.h"
class LC898217_E8_EA :
	public CVcmDriverInterface
{
public:
	LC898217_E8_EA(void);
	~LC898217_E8_EA(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;
private:
	int AF_convert(int position);
	void RamWriteA(BYTE SlaveID,USHORT addr,USHORT data);
	void RamReadA(BYTE SlaveID,USHORT addr,USHORT *data);

	void RegReadA(BYTE SlaveID,USHORT addr,BYTE *data);
	void RegWriteA(BYTE SlaveID,USHORT addr,BYTE data);

};