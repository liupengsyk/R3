///////////////////////////////////////////////////////////////////////////////////////
//	DLL import header file for USB3.0 boards
///////////////////////////////////////////////////////////////////////////////////////
#ifndef __USB_FRM11_IMPORT_H__
#define __USB_FRM11_IMPORT_H__

#ifdef _cplusplus
extern "C" {
#endif

#define MAX_DEVICE	4						// Maximum number of board to find

	// Board level API functions
	extern "C" __declspec(dllimport) int  OpenDAQDevice(void);
	extern "C" __declspec(dllimport) BOOL CloseDAQDevice(void);

	//*************************************************************************************
	// Multi Board LVDS API functions
	//*************************************************************************************
	extern "C" __declspec(dllimport) BOOL  LVDS_GetVersion_Mul(int nBoard, int *nFpgaVer, int *nFirmVer);
	extern "C" __declspec(dllimport) BOOL  LVDS_Init_Mul(int nBoard);
	extern "C" __declspec(dllimport) BOOL  LVDS_Start_Mul(int nBoard);
	extern "C" __declspec(dllimport) BOOL  LVDS_GetFrame_Mul(int nBoard, DWORD* nCnt, unsigned char* buf);
	extern "C" __declspec(dllimport) BOOL  LVDS_BufferFlush_Mul(int nBoard);

	extern "C" __declspec(dllimport) BOOL  LVDS_GetResolution_Mul(int nBoard, DWORD *xRes, DWORD *yRes);
	extern "C" __declspec(dllimport) BOOL  LVDS_Stop_Mul(int nBoard);
	extern "C" __declspec(dllimport) BOOL  LVDS_SetDataMode_Mul(int nBoard, int nMode);		//	0: 8bit(M1L),  1: 16bit(M2L),  2,3: 32bit(M4L)

	//****************************************************************************************
	//	LVDS Select Input (default : MIPI)
	//	0 : MIPI, 1 : Parallel
	//****************************************************************************************
	extern "C" __declspec(dllimport) BOOL  LVDS_SelectInput_Mul(int nBoard, int nInput);

	//****************************************************************************************
	//	Set Vsync Polarity for Parallel output  (2013. 03. 06)
	//  bPol  FALSE:bypass, TRUE: inverse
	//****************************************************************************************
	extern "C" __declspec(dllimport) BOOL  LVDS_VsyncPol_Mul(int nBoard, BOOL bPol);

	//****************************************************************************************
	//	Set Pclk Polarity for Parallel output  (2013. 03. 06)
	//  bPol  FALSE:bypass, TRUE: inverse
	//****************************************************************************************
	extern "C" __declspec(dllimport) BOOL  LVDS_PclkPol_Mul(int nBoard, BOOL bPol);

	//****************************************************************************************
	//	Set VSYNC delay for crc caculation		2014. 01. 14
	//  nDelay	: range 0 to 32767  default 128
	//****************************************************************************************
	extern "C" __declspec(dllimport) BOOL  LVDS_SetVsyncDelay_Mul(int nBoard, int nDelay);



	//****************************************************************************************
	// MCLK functions
	//****************************************************************************************
	extern "C" __declspec(dllimport)  BOOL  CLK_Select_Mul(int nBoard, int nSelect); // nSelect == 0 : fixed clock, others : Program clock
	extern "C" __declspec(dllimport)  DWORD CLK_Get_Mul(int nBoard);
	extern "C" __declspec(dllimport)  BOOL CLK_Set_Mul(int nBoard, DWORD val);		// val:  1039Hz to 68000000hz
	extern "C" __declspec(dllimport)  BOOL CLK_Off_Mul(int nBoard, BOOL bOff);

	//*************************************************************************************
	// DIO input/output
	//*************************************************************************************
	extern "C" __declspec(dllimport) DWORD DIO_SetDirection_Mul(int nBoard, DWORD dwVal);	// 0: input, 1: output 
	extern "C" __declspec(dllimport) BOOL  DIO_GetDirection_Mul(int nBoard, DWORD *dwVal);	// 0: input, 1: output 
	extern "C" __declspec(dllimport) DWORD DIO_Read_Mul(int nBoard);
	extern "C" __declspec(dllimport) BOOL  DIO_Write_Mul(int nBoard, DWORD dwVal);
	extern "C" __declspec(dllimport) BOOL  DIO_GetWrite_Mul(int nBoard, DWORD *dwVal);

	//**************************************************************************************
	//  From here it starts to define Sensor I2C function
	//**************************************************************************************
	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Reset_Mul(int nBoard);
	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Set_Clock_Mul(int nBoard, int nClock); //I2C clock speed ex) nClock=100000 is 100Khz
	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Read_Mul(int nBoard, BYTE slAddr, DWORD nAddrLen, DWORD nAddr, DWORD nCnt, unsigned char* buf);
	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Write_Mul(int nBoard, BYTE slAddr, DWORD nAddrLen, DWORD nAddr, DWORD nCnt, unsigned char* buf);
	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Write2_Mul(int nBoard, BYTE slAddr, DWORD nAddrLen, DWORD nAddr, DWORD nCnt, unsigned char* buf); //remove read cycle but cannot check fail or success

	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Read_Ex_Mul(int nBoard, BYTE slAddr, DWORD nAddrLen, unsigned char* AddrBuf, DWORD nCnt, unsigned char* buf);  //for multi address. Max address 15 BYTE
	extern "C" __declspec(dllimport) BOOL  I2C_SYS_Write_Ex_Mul(int nBoard, BYTE slAddr, DWORD nAddrLen, unsigned char* AddrBuf, DWORD nCnt, unsigned char* buf);

	extern "C" __declspec(dllimport) BOOL  SEN_Reset_Mul(int nBoard, BOOL bReset);		// Reset pin control
	extern "C" __declspec(dllimport) BOOL  SEN_Enable_Mul(int nBoard, BOOL bEnble);		// Enable pin cotrol


	//extern "C" __declspec(dllimport) BOOL  LVDS_SetRolling_Mul(int nBoard, BOOL bRolling);

#ifdef _cplusplus
}
#endif

#endif  //__USB_FRM11_IMPORT_H__