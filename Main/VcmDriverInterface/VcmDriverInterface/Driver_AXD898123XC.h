 
#pragma once
#include "vcmdriverinterface.h"
class Driver_AXD898123XC :
	public CVcmDriverInterface
{
public:
	Driver_AXD898123XC(void);
	~Driver_AXD898123XC(void);

	int ReadCode(USHORT *code);
	int WriteCode(USHORT code);
	int AutoFocus();
	int Init();
	USHORT sensorId;

private:
  
	UINT8	RdStatus( UINT8 UcStBitChk );
	void	SetTregAf( UINT16 UsTregAf);
	void RamRead32A(unsigned short addr, unsigned long* data);
	void RamWrite32A(unsigned short addr, unsigned long data);
};


