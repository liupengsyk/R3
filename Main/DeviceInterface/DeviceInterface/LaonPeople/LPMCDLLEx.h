/******************************************************************************
*	Project : MIU Project (MIPI USB30)
*
*
*	
*	File Name : LPMC500DLLEx.h 
******************************************************************************/
#ifndef __V5U_DLLEX_H__
#define __V5U_DLLEX_H__


#ifdef _USRDLL
#define DLLFunction  __declspec(dllexport)
#else
#define DLLFunction  __declspec(dllimport)
#endif


#ifdef _cplusplus
extern "C" {
#endif


/******************************************************************************
*   MIU ERROR CODE DEFINE
*******************************************************************************/
#define MIU_OK						0x00000000
#define MIU_CREATE_ERR				0x00000001	
#define MIU_NODEVICE				0x00000002
#define MIU_TIMEOUT					0x00000003
#define MIU_REGWRITEERROR           0x00000004
#define MIU_BOARDNUMBERDUPLICATION  0x00000005
#define MIU_ALREADY_OPENED			0x00000006
#define MIU_ALREADY_CLOSED			0x00000007
#define MIU_INDEX_ERR				0x00000008

#define MIU_ENDPOINTOPENFAIL		0x00000052

#define MIU_NEW_FRAME				0x0100
#define MIU_RECEIVING				0x0101

#define AF_SUCCESS					0x0300
#define AF_TRY_AGAIN				0x0301
#define AF_LIMIT_FAIL				0x0302
#define AF_PEAK						0x0303

#define AE_SUCCESS					0x0200
#define AE_TRY_AGAIN				0x0201
#define AE_RETRY_COUNT				500

#define MIU_VOLTAGECTL_ERROR		0x00001001
#define MIU_PINCONTROL_ERROR		0x00001002
#define MIU_ADCSET_ERROR			0x00001003
#define	MIU_ADCREAD_ERROR			0x00001004
#define MIU_CALIBRATIONINIT_ERROR   0x00001005

/******************************************************************************
*   MIU EVENT CODE DEFINE
*******************************************************************************/
#define MIU_EVENT_TIMEOUT			0x0000F001
#define	MIU_EVENT_ATTACHED			0x0000F002
#define MIU_EVNET_DETACHED			0x0000F003

#define OS_TESTPIN_COUNT			55

// SensorUSBMode
#define	SENSOR1_USB1 0
#define	SENSOR1_USB2 1
#define	SENSOR2_USB2 3

/******************************************************************************
*   MIU Register TYPE
*******************************************************************************/
typedef enum {
    REG_MIU = 0,
    REG_IIC1,
	REG_OS
} MIU_REG_TYPE;


/******************************************************************************
*   VCM CONTROL
*******************************************************************************/
typedef enum {
	DW9718S = 0,	//13M
	AK7348			//16M
} VCM_CONTROL;


/******************************************************************************
* Structure of MIU InitialValue
*******************************************************************************/
typedef struct _MIUINITIALVALUE
{
	unsigned char	SensorMode;
	unsigned short	nWidth;
	unsigned short	nHeight;
	unsigned char	MCLKOnOff;
	unsigned char   MCLKSelection;
	float			MCLK;
	unsigned char	PCLKInversion;
	unsigned char	IICDeviceID;	
	unsigned char	IICMode;
	unsigned char	IICSpeed;
	unsigned char	IICReadRestart;
	unsigned char	IICReadRestartInterval;
	unsigned char	IICSCKPinCheck;
	unsigned char	IICAddressLength;
	unsigned char	IICDataLength;
	unsigned char	MIPILaneEnable;
	unsigned char	MIPIDataType;
	float			MIUIOVoltage;
	unsigned char	FirstPowerChannel;
	float			FirstPowerVoltage;
	unsigned char	SecondPowerChannel;
	float			SecondPowerVoltage;
	unsigned char	ThirdPowerChannel;
	float			ThirdPowerVoltage;
	unsigned char	FourthPowerChannel;
	float			FourthPowerVoltage;
	float			OTPVoltage;
	unsigned char   MIPI8bitMode;
}MIU_INITIALValue;

typedef struct _AF_ROI
{
	int nXStart;
	int nYStart;
}AF_ROI;


//20140903 추가
typedef struct _AFCONTROL
{
	unsigned char	nActuatorI2CID;
	unsigned char	nActuatorI2CMode; // (0 : 8bit address, 8bit data, 1 : 8bit address, 16bit data, 2 : 16bit address, 8bit data, 3 : 16bit address, 16bit data)
	unsigned char	nSensorI2CID;
	unsigned char	nSensorI2CMode;   // (0 : 8bit address, 8bit data, 1 : 8bit address, 16bit data, 2 : 16bit address, 8bit data, 3 : 16bit address, 16bit data)
	unsigned short	nMaxValue;
	unsigned short	nMinValue;
	unsigned short	nStepValue;
	unsigned short	nActuatorType; // Default : 0

	int				nRoiWidth;
	int				nRoiHeight;
	BOOL			bRoiOn;
	AF_ROI			AFRoi;
}AFCONTROL;

typedef struct _AECONTROL
{
	unsigned short nMinValue;
	unsigned short nMaxValue;
	unsigned short nUpperAddr;
	unsigned short nLowerAddr;
	unsigned char nRegisterAddrCount;
	unsigned char nRegisterDataLength;
	unsigned char nAERegisterType;
	unsigned char nAE_TargetValue;
	unsigned int nWindowWidth;
	unsigned int nWindowHeight;
	unsigned int nExposureValueY;
}AECONTROL;

/******************************************************************************
*   MIUGetDeviceList : MIU Get Device List 
*
*	@param pDeviceCount[out], Device count
*   @param pDeviceList[out], Device List
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUGetDeviceList(char* pDeviceCount, char* pDeviceList);


/******************************************************************************
*   MIUOpenDevice : MIU Open 
*
*	@param		      
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUOpenDevice(char iDeviceIndex);


/******************************************************************************
*   MIUCloseDevice : MIU Close 
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUCloseDevice(char iDeviceIndex);

/******************************************************************************
*   MIUCloseAll : MIU Close All
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUCloseAll();


/******************************************************************************
*   MIUInitialize : MIU Initialize 
*	
*	@param		iDeviceIndex, [in] DeviceIndex
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUInitialize(char iDeviceIndex, MIU_INITIALValue InitialValue );


/******************************************************************************
*   MIUWriteRegister : MIU Register Write 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      iRegisterType, [in] 어떤 Type의 Register를 읽을지 나타낸다. 
*               MIU_REG_TYPE 참고 
*   @param      nAddress, [in] 쓰고자 하는 Register의 address  
*   @param      nData, [in] Data Value
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUWriteRegister(char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned short nAddress, unsigned short nData);


/******************************************************************************
*   MIUReadRegister : MIU Register Read 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      iRegisterType, [in] 어떤 Type의 Register를 읽을지 나타낸다. 
*               MIU_REG_TYPE 참고 
*   @param      nAddress, [in] 쓰고자 하는 Register의 address  
*   @param      pData, [in, Out] Read Register Value
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUReadRegister(char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned short nAddress, unsigned short* pData);



/******************************************************************************
*   MIUInitializeImageValue : MIU 에 Capture하게될 Image의 특성을 설정 한다.
*                             Image Receive Start 를 하기 전에 설정 한다.
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      PixelFormat, [in] 전송 Image의 PixelFormat 을 Write
*   @param      nWidth, [in] Image의 Width
*   @param      nHeight, [in] Image의 Height
*   @param      nMaxWidth, [in] Image의 Width
*   @param      nMaxHeight, [in] Image의 Height
*   @param      nBufferCount, [in] ImageBuffer Count
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUInitializeImageValue(char iDeviceIndex, unsigned int PixelFormat, unsigned int nWidth, 
												  unsigned int nHeight, unsigned int nMaxWidth, 
												  unsigned int nMaxHeight,unsigned char nBufferCount);



/******************************************************************************
*   MIUStart : MIU Capture Start
*
*	@param		iDeviceIndex, [in] DeviceIndex
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUStart(char iDeviceIndex);




/******************************************************************************
*   MIUStop : MIU Capture Stop
*
*	@param		iDeviceIndex, [in] DeviceIndex
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUStop(char iDeviceIndex);



/******************************************************************************
*   MIUPause : MIU Capture Pause for resolution change
*
*	@param		iDeviceIndex, [in] DeviceIndex
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUPause(char iDeviceIndex);


/******************************************************************************
*   MIUResume : MIU Capture Resume for resolution change
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      nWidth, [in] Width
*   @param      nHeight, [in] Height
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUResume(char iDeviceIndex, unsigned int nWidth, unsigned int nHeight);



/******************************************************************************
*   MIUResumeChageFormat : MIU Capture Resume for resolution change
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      PixelFormat, [in] Pixel Format
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUResumeChageFormat(char iDeviceIndex, unsigned int PixelFormat );


/******************************************************************************
*   MIUPause : MIU Capture Preview Pause
*
*	@param		iDeviceIndex, [in] DeviceIndex
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUPreviewPause(char iDeviceIndex);


/******************************************************************************
*   MIUResume : MIU Capture Preview Resume
*
*	@param		iDeviceIndex, [in] DeviceIndex
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUPreviewResume(char iDeviceIndex);




/******************************************************************************
*   MIUGetImageData : MIU Get Image Data
*
*	@param		iDeviceIndex, [in] DeviceIndex
*	@param      ppImagePoint, [in, Out] Display 할 Image Data의 Pointer를 가져올 Pointer
*   @param      pTimeStamp, [in, Out] TimeStamp 값을 가져갈 변수
*
*   @return     Receive Status
******************************************************************************/
DLLFunction int __stdcall MIUGetImageData(char iDeviceIndex, unsigned char** ppImagePoint, INT64* pTimeStamp);




/******************************************************************************
*   MIUGetDiagnostics : MIU Get Diagnostics
*
*	@param		iDeviceIndex, [in] DeviceIndex
*	@param      pImageCount [in, Out] 수신한 Image Count 값을 가져온다.
*   @param      pFramerate  [in, Out] Frame Rate (fps)을 가져온다.
*   @param      pBitrate    [in, Out] Bitrate를 가져온다. (Mbps 단위)
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUGetDiagnostics(char iDeviceIndex, unsigned int* pImageCount, float* pFramerate, float* pBitrate);




/******************************************************************************
*   MIUGetSensorFrameRate : MIU Get Sensor Frame Rate
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      pSensorFramerate  [in, Out] Sensor Frame Rate (fps)을 가져온다.
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUGetSensorFrameRate(char iDeviceIndex, float* pSensorFramerate);



/******************************************************************************
*   IICWrite : IIC Write 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      iRegisterType [in], SENSOR1 or SENSOR2
*   @param      SlaveID [in], IIC Slave ID
*	@param      ProtocolType [in] IIC 전송 Type을 설정 한다.
*   @param      DataSize  [in] pData의 총 Byte 수.
*   @param      pData   [in] Write 할 Data
*   @param      bWriteMode [in] Write Mode Option
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall IICWrite(char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned char SlaveID, int ProtocolType, int DataSize, BYTE *pdata, BOOL bWriteMode);



/******************************************************************************
*   IICWriteCX : IIC Write for renesas isp
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      iRegisterType [in], SENSOR1 or SENSOR2
*   @param      SlaveID [in], IIC Slave ID
*	@param      ProtocolType [in] IIC 전송 Type을 설정 한다.
*   @param      DataSize  [in] pData의 총 Byte 수.
*   @param      pData   [in] Write 할 Data
*   @param      bWriteMode [in] Write Mode Option
*
*   @return     Error Value
//******************************************************************************/
DLLFunction int __stdcall IICWriteCX(char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned char SlaveID, int ProtocolType, int DataSize, BYTE *pdata, BOOL bWriteMode);


/******************************************************************************
*   IICWriteCX : IIC Write for Normal 
*
*   @param      iDeviceIndex, [in] Device Index
*   @param      iRegisterType [in], SENSOR1 or SENSOR2
*   @param      DataSize  [in] pData의 총 Byte 수.
*   @param      pData   [in] Write 할 Data
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall IICWriteN(char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned char SlaveID, int DataSize, BYTE *pdata);




/******************************************************************************
*   IICRead : IIC Read 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      iRegisterType [in], SENSOR1 or SENSOR2
*   @param      SlaveID [in], IIC Slave ID
*	@param      ProtocolType [in] IIC 전송 Type을 설정 한다.
*   @param      DataSize  [in] pData의 총 Byte 수.
*   @param      pData   [in] Write 할 Data
*   @param      ReadSize [in] Read 할 Data Size (Count, Address 포함)
*   @param      pReadData   [in, Out] Read 할 Data Buffer
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall IICRead (char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned char SlaveID, int ProtocolType, int DataSize, BYTE *pdata, int ReadSize, BYTE* pReadData);

/******************************************************************************
*   IICReadCX : IIC Read 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      iRegisterType [in], SENSOR1 or SENSOR2
*   @param      SlaveID [in], IIC Slave ID
*	@param      ProtocolType [in] IIC 전송 Type을 설정 한다.
*   @param      DataSize  [in] pData의 총 Byte 수.
*   @param      pData   [in] Write 할 Data
*   @param      ReadSize [in] Read 할 Data Size (Count, Address 포함)
*   @param      pReadData   [in, Out] Read 할 Data Buffer
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall IICReadCX (char iDeviceIndex, MIU_REG_TYPE iRegisterType, unsigned char SlaveID, int ProtocolType, int DataSize, BYTE *pdata, int ReadSize, BYTE* pReadData);



/******************************************************************************
*   CurrentMeasureAllPowerOn : Current Measure AllPowerOn  (Power UP Sequence 설정)
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      n1stPowerPin, [in] 1st Power up pin
*   @param      n1stVoltage, [in] 1st Voltage 
*   @param      n2ndPowerPin, [in] 2nd Power up pin
*   @param      n2ndVoltage, [in] 2nd Voltage 
*   @param      n3rdPowerPin, [in] 3rd Power up pin
*   @param      n3rdVoltage, [in] 3rd Voltage 
*   @param      n4thPowerPin, [in] 4th Power up pin
*   @param      n4thVoltage, [in] 4th Voltage 
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction unsigned int  __stdcall CurrentMeasureAllPowerOn(char iDeviceIndex, unsigned char n1stPowerPin, float n1stVoltage,
													unsigned char n2ndPowerPin, float n2ndVoltage,
													unsigned char n3rdPowerPin, float n3rdVoltage,
													unsigned char n4thPowerPin, float n4thVoltage);



/******************************************************************************
*   CurrentMeasureMode : Current Measure Mode Select  
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      Mode, [in] 측정 하고자 하는 Mode 0 : Dynamic Mode, 1 : Static Mode
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction unsigned int __stdcall CurrentMeasureMode(char iDeviceIndex,unsigned char Mode);



/******************************************************************************
*   CurrentMeasureResult : Current Measure result  
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   @param      nChannelNumber, [in] 측정 하고자 하는 channel number (0~4)
*   @param      CurrentMeasureResult, [out] Current measure result
*   @param      Mode, [in] Current measure Mode, 0 : Dynamic Mode, 1 : standby Mode
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction unsigned int __stdcall CurrentMeasureResult(char iDeviceIndex, unsigned char nChannelNumber, double* CurrentMeasureResult, unsigned char Mode);



/******************************************************************************
*   CurrentMeasureDone : Current Measure all power off  
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction unsigned int __stdcall CurrentMeasurePowerOff(char iDeviceIndex);



/******************************************************************************
*   AutoCalibration : Auto Calibration 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction void __stdcall AutoCalibration(char iDeviceIndex );


/******************************************************************************
*   InitializeCalibration : Initialize Calibration 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction void __stdcall InitializeCalibration(char iDeviceIndex);



/******************************************************************************
*   FirmwareDownload : Firmware Download  
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nSize, [in] Firmware image size
*   @param      pData, [in] Firmware image
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall FirmwareDownload(char iDeviceIndex, unsigned int nSize, unsigned char* pData);


/******************************************************************************
*   MIUMainClockControl : Main clock control
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      bOnOff, [in] Clock On/Off setting 0 : Off, 1 : On
*   @param      ClockSelection, [in] 1 : Clock Generator, 2 : External Clock
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUMainClockControl(char iDeviceIndex, char bOnOff, char ClockSelection);


/******************************************************************************
*   MIUSetMCLK : MIU MCLK Setting 
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      fMCLK, [in] M Clock Value
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetMCLK(char iDeviceIndex, float fMCLK);



/******************************************************************************
*   MIUSetPowerChannel0 : Power channel 0 setting
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetPowerChannel0(char iDeviceIndex, float fVoltage, char OnOff);


/******************************************************************************
*   MIUSetPowerChannel1 : Power channel 1 setting
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetPowerChannel1(char iDeviceIndex, float fVoltage, char OnOff);


/******************************************************************************
*   MIUSetPowerChannel2 : Power channel 2 setting
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetPowerChannel2(char iDeviceIndex, float fVoltage, char OnOff);

/******************************************************************************
*   MIUSetPowerChannel3 : Power channel 3 setting
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetPowerChannel3(char iDeviceIndex, float fVoltage, char OnOff);


/******************************************************************************
*   MIUSetPowerOTP : OTP Power OnOff
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetPowerOTP(char iDeviceIndex, float fVoltage, char OnOff);



/******************************************************************************
*   MIUSetIOPowerChannel : Power channel IO setting
*
*	@param		iDeviceIndex, [in] DeviceIndex
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetIOPowerChannel(char iDeviceIndex, float fVoltage, char OnOff);



/******************************************************************************
*   MIUGetFirmwareInformation : Get Firmware Information
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      pInformation, [in, Out] pointer for getting firmware information  
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction void __stdcall MIUGetFirmwareInformation(char iDeviceIndex, char* pInformation);


/******************************************************************************
*   MIUWriteSPISensor : Write SPI Sensor
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nData, [in] Write Data  
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUWriteSPISensor(char iDeviceIndex, unsigned short nData);


/******************************************************************************
*   MIUReadSPISensor : Read SPI Sensor
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      pData, [in, out] Read Data  
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUReadSPISensor(char iDeviceIndex, unsigned short* pData);


/******************************************************************************
*   MIUGetGPIOMode : Get GPIO Mode
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nGPIONumber, [in] GPIO Number (0~3)  // 800CK support only 0
*   @param      pMode, [in,out] GPIO Mode (0:Output, 1:Input)
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUGetGPIOMode(char iDeviceIndex, char nGPIONumber, char* pMode);


/******************************************************************************
*   MIUSetGPOStatus : Set GPIO Mode
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nGPIONumber, [in] GPIO Number (0~3) // 800CK support only 0
*   @param      nMode, [in] GPO Status
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetGPIOMode(char iDeviceIndex, char nGPIONumber, char nMode);


/******************************************************************************
*   MIUGetGPIOStatus : Get GPIO Status
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nNumber, [in] GPIO Number (0~3) // 800CK support only 0
*   @param      pStatus, [in,out] GPIO Status
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUGetGPIOStatus(char iDeviceIndex, char nGPIONumber, char* pStatus);


/******************************************************************************
*   MIUSetGPOStatus : Set GPIO Status
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nNumber, [in] GPIO Number (0~3) // 800CK support only 0
*   @param      nStatus, [in] GPO Status
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUSetGPIOStatus(char iDeviceIndex, char nGPIONumber, char nStatus);


// 20121025 
/******************************************************************************
*   OpenShortTest : Open Short Test  
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      UpperTestResult, [out] Upper Diode Test Result (OS_TESTPIN_COUNT (43) channel)
*   @param      LowerTestResult, [out] Lower Diode Test Result (OS_TESTPIN_COUNT (43) channel)
*   @param      nInputCurrent, [in] Source Current value (Range 0~ 200)
*   @param      nSensorMode, [in] SensorMode	//20121031 
*   @param      nDelay, [in] Power, Gnd 관련 측정 delay				//20121112  (파라메터 제거)
*   @param      UpperEn, [in] Enable Test Power and Ground Pin	//20121112
*   @param      LowerEn, [in] Enable Test SPI Pin					//20121112
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction unsigned int __stdcall OpenShortTest(char iDeviceIndex, float* UpperTestResult, float* LowerTestResult, unsigned char nInputCurrent, unsigned char nSensorMode,int* UpperEn, int* LowerEn);



/******************************************************************************
*   LeakageTest : Leakage Current Test  
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      LeakageTestResult, [out] Leakage Test Result (OS_TESTPIN_COUNT (43) channel)
*   @param      IOVoltage, [in] IO Channel Voltage
*   @param      nDelay, [in] Power, Gnd 관련 측정 delay
*   @param      LeakageEn, [in] Leakage test pin Enable
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction unsigned int __stdcall LeakageTest(char iDeviceIndex, float* LeakageTestResult, float IOVoltage  , unsigned char nSensorMode,int* LeakageEn);



/******************************************************************************
*   MIUSetI2CMode : MIU Set I2C Mode
*
*	@param		iDeviceIndex, [in] DeviceIndex
*               
*   @param      nMode (0 : 8bit address, 8bit data, 1 : 8bit address, 16bit data
*                      2 : 16bit address, 8bit data, 3 : 16bit address, 16bit data)
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUSetI2CMode(char iDeviceIndex, unsigned short nMode);


/******************************************************************************
*   MIUSetI2CID : MIU Set I2C ID
*
*	@param		iDeviceIndex, [in] DeviceIndex
*               
*   @param      nID (바꾸고자 하는 I2C ID)
*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUSetI2CID(char iDeviceIndex, unsigned char nI2CID);




/******************************************************************************
*   MIUSetOTPSequence : MIUSetOTPSequence
*
*	@param		iDeviceIndex, [in] DeviceIndex
*               
*   @param      nSequence, Opt Power On 순서 
                0 : before 1st Power , 
				1 : before 2nd Power ,
				2 : before 3rd Power ,
				3 : before 4th Power ,
				4 : after  4th Power 	

*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUSetOTPSequence(char iDeviceIndex, int nSequence);




/******************************************************************************
*   MIUSetPowerChSleep : MIUSetPowerChSleep
*
*	@param		iDeviceIndex, [in] DeviceIndex
*               
*   @param      PowerChSleep, power Ch별  Sleep // int PowerChSleep[6]

			    PowerChSleep[0] = 0; //1stPowerChannel Sleep
				PowerChSleep[1] = 0; //2ndPowerChannel Sleep
				PowerChSleep[2] = 0; //3rdPowerChannel Sleep
				PowerChSleep[3] = 0; //4thPowerChannel Sleep
				PowerChSleep[4] = 0; //OTP Power Sleep


*
*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUSetPowerChSleep(char iDeviceIndex, int* PowerChSleep);



/******************************************************************************
*   ProgressAF : AF 
*
*   @param      nAfConfigValue, [in] AFContorl Config Value
*   @param      nOffsetValue, [in] AFContorl Offset Value 
*
*   @return     AF Status
******************************************************************************/
DLLFunction int __stdcall SetAFSettingValue(unsigned int nAfConfigValue, unsigned int nOffsetValue);

/******************************************************************************
*   ProgressAF : AF 
*
*	@param		iDeviceIndex, [in] DeviceIndex
*	@param		pImage, [in] AF 대상의 Image
*	@param		nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height
*	@param		pActTarget, [in,out] VCM Target Value
*   @param      afControlValue, [in,out] AFContorl Value
*   @param      nOffsetValue, [in] AFContorl Offset Value 
*   @param      pAFScore, [out] Auto Focuse Score
*
*   @return     AF Status
******************************************************************************/
DLLFunction int __stdcall ProgressAF(char iDeviceIndex,unsigned char* pImage,unsigned int nWidth, unsigned int nHeight, unsigned int* pActTarget ,AFCONTROL* afControlValue, int* pAFScore, BOOL bAFScoreMode, int nPixelFormat);



/******************************************************************************
*   MIUMoveVCM : VCM Control
*
*	@param		iDeviceIndex, [in] AF 대상의 Image
*	@param		code, [in] VCM Target Value
*   @param      afControlValue, [in,out] AF Control Value 
*
*   @return     AF Status
******************************************************************************/
DLLFunction int __stdcall MIUMoveVCM(char iDeviceIndex, int code, AFCONTROL afControlValue);



/********************************************************************************
*   MIUSetHallValue : Get value for LC898212XD VCM Control
*
*	@param		Stepmove Parameter
*	@param		Stepmove Parameter
*	@param		Stepmove Parameter
*	@param		Stepmove Parameter
*
*   @return     Error Value
/********************************************************************************/
DLLFunction int __stdcall MIUSetHallValue(int tHall_Bias,int tHall_Offset,int tHallMin,int tHallMax);


/********************************************************************************
*   MIUSetSetSmvParValue : Get value for LC898212XD VCM Control
*
*	@param		Stepmove Parameter
*	@param		Stepmove Parameter
*	@param		Stepmove Parameter
*
*   @return     Error Value
/********************************************************************************/
DLLFunction int __stdcall MIUSetSetSmvParValue(unsigned short tUsSmvSiz, unsigned short tUcSmvItv, unsigned short tUcSmvEnb);


/********************************************************************************
*   StmvTo : LC898212XD VCM Control
*
*	@param		iDeviceIndex, [in] AF 대상의 iDeviceIndex
*	@param		code, [in] VCM Target Value
*
*   @return     Error Value
/********************************************************************************/
DLLFunction unsigned short __stdcall StmvTo(char iDeviceIndex, int nCode);


/********************************************************************************
*   StmvEnd : LC898212XD VCM Control
*
*	@param		iDeviceIndex, [in] AF 대상의 iDeviceIndex
*	@param		Stepmove Parameter
*
*   @return     Stepmove Parameter
/********************************************************************************/
DLLFunction unsigned short __stdcall StmvEnd(char iDeviceIndex, unsigned short UcParMod);


static void AeAdjust(char iDeviceIndex, short nAdjust, AECONTROL AeConrol);

DLLFunction int __stdcall ProcessAE(char iDeviceIndex,unsigned char* pImage,unsigned int nWidth, unsigned int nHeight, unsigned char nTarget, AECONTROL AeConrol, unsigned int* ExposureValue);

/******************************************************************************
*   MIULSTtoFPGA : Setup Lens shading table in FPGA
*	@param		iDeviceIndex
*	@param		iLSCRomIndex

*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIULSTtoFPGA(char iDeviceIndex , unsigned char iLSCRomIndex);


/******************************************************************************
*   MIUReadLsVersion : Read Lens Shading version.
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nSize, [in] Data size
*   @param      pData, [out] Data pointer
*   @param      nMemoryIndex, [in] EEPROM INDEX to convert to physical memory address
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUReadLsVersion(char iDeviceIndex, unsigned int nSize, unsigned char* pData, unsigned char nMemoryIndex);


/******************************************************************************
*   MIUGetDeviceCount : MIUGetDeviceCount
*
*
*   @return     Device Count
******************************************************************************/
DLLFunction int __stdcall MIUGetDeviceCount();


/******************************************************************************
*   MIUSetIICMUXControl : MIUSetIICMUXControl
*
*
*   @return     MIU status
******************************************************************************/
DLLFunction int __stdcall MIUSetIICMUXControl(char iDeviceIndex, UINT mode);

/******************************************************************************
*   LSCGetTableSize : Get LSC Table Size;
*
*	@param		nWidth,		[in] image  width
*	@param		nHeight,	[in] image  Height
*	@param		pSize,		[out] Table size
*
*   @return    return Status
******************************************************************************/
DLLFunction int __stdcall LSCGetTableSize(unsigned int nWidth, unsigned int nHeight, unsigned int* pSize);


/******************************************************************************
*   LSCTableMake : LSC Table 생성
*
*	@param		pAverageImage, [in] Average  Image
*
*   @return    return Status
******************************************************************************/
DLLFunction int __stdcall LSCTableMake(double* pAverageImage,unsigned int nWidth, unsigned int nHeight, unsigned short* pEvenTable, unsigned short* pOddTable, int* pEvenSize, int* pOddSize);

/******************************************************************************
*   MIURomDataDownload : CalData   Download to EEPROM(BOOTROM) 
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      nSize, [in] Data size
*   @param      pData, [in] Data pointer
*   @param      nMemoryIndex, [in] EEPROM INDEX to convert to physical memory address
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIURomDataDownload(char iDeviceIndex, unsigned int nSize, unsigned char* pData, unsigned char nMemoryIndex);

/******************************************************************************
*   MIUGetFPGAFirmwareVersion : Get FPGA Firmware Version
*
*	@param      iDeviceIndex, [in] DeviceIndex
*   @param      pInformation, [in, Out] pointer for getting firmware version
*   
*   @return     Error Value 
******************************************************************************/
DLLFunction int __stdcall MIUGetFPGAFirmwareVersion(char iDeviceIndex, unsigned short* pVersion);


/******************************************************************************
*   MIUGetDllVersion : Get DLL Version
*	@param		nVersion, [out] DLL Version

*   @return     Error Value
******************************************************************************/
DLLFunction int __stdcall MIUGetDllVersion(unsigned char* pVersion);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Event Callback
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef   void   (CALLBACK   *EventCallback)(char, int);  

/******************************************************************************
*   USBRestartCallback : Set Evnet Callback function
*
*   @return     Error Value
******************************************************************************/
DLLFunction void  WINAPI USBRestartCallback(EventCallback func, unsigned int WaitTime); 

#ifdef _cplusplus
}
#endif

#endif  //__V5U_DLLEX_H__
