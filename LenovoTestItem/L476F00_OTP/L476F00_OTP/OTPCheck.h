#ifdef   OTPCheck_FILE
#define  OTPCheckDLLAPI extern "C"  __declspec(dllexport)
#else
#define  OTPCheckDLLAPI  extern "C"  __declspec(dllimport)
#endif


enum DriverICType
{
		DW9714=0,
		DW9718,
		DW9761,	
		DW9800,	
		BU64295296297GWZ,
		BU64244,
		BU64291,
		AD5820,
		DW9804,
		DW9719,
		DW9763,
		BU64297GWZ,
		DW9714V,
		LC898214XC,
		AK7371,
		AK7374,
};
enum SensorType{
		S5K2P7=0,
		HI1332,
		IMX135,
		IMX179,
		IMX214,
		IMX219,
		IMX258,
		OV12A10,
		OV5645,
		OV5648,
		OV8858,
		OV8865,
		OV13853,			//适用于OV13855
		OV13880,
		S5K3H7,
		S5K3L2,
		S5K4H8,
		S5K5E3,
		SR544,
		S5K3P3,
		S5K3M2,
		S5K3H2,
		S5K3L8,
		OV13855,
		HI846,
		OV16885,
		S5K3P8,
		IMX350,
		IMX386,
		OV13358,
		IMX376,
};

OTPCheckDLLAPI int _stdcall  OTPcheck(char* ErrorMsg,BYTE *InputOTPData=NULL, int Flag=0);
// ErrorMsg: 输出错误信息，当OTPCheck fail 时输出具体的错误信息，建议全部打印，建议开辟的Buffer 大于1024
// InputOTPData : 输入模组OTP数据，需严格按照OTPMap/guide存储，建议创Buffer大小统一设置成：0x2000，如：InputOTPData[0x2000]
// Flag ：InputOTPData的标志位，当其大于0时表示有数据输入
OTPCheckDLLAPI int _stdcall  GetFuseID(char* FuseID);
OTPCheckDLLAPI int _stdcall  DW9761_Read(USHORT address,unsigned char *Data,int size);

OTPCheckDLLAPI int _stdcall  GetDriverICName(int* DriverICNum,char *ICName[]);
OTPCheckDLLAPI int _stdcall  GetSensorName(int* SensorNum,char *SensorName[]);
OTPCheckDLLAPI int _stdcall  DriverICReset(DriverICType CurType);
OTPCheckDLLAPI int _stdcall  DriverICMove(DriverICType CurType,int Code);
