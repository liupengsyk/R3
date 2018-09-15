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
		OV13853,			//������OV13855
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
// ErrorMsg: ���������Ϣ����OTPCheck fail ʱ�������Ĵ�����Ϣ������ȫ����ӡ�����鿪�ٵ�Buffer ����1024
// InputOTPData : ����ģ��OTP���ݣ����ϸ���OTPMap/guide�洢�����鴴Buffer��Сͳһ���óɣ�0x2000���磺InputOTPData[0x2000]
// Flag ��InputOTPData�ı�־λ���������0ʱ��ʾ����������
OTPCheckDLLAPI int _stdcall  GetFuseID(char* FuseID);
OTPCheckDLLAPI int _stdcall  DW9761_Read(USHORT address,unsigned char *Data,int size);

OTPCheckDLLAPI int _stdcall  GetDriverICName(int* DriverICNum,char *ICName[]);
OTPCheckDLLAPI int _stdcall  GetSensorName(int* SensorNum,char *SensorName[]);
OTPCheckDLLAPI int _stdcall  DriverICReset(DriverICType CurType);
OTPCheckDLLAPI int _stdcall  DriverICMove(DriverICType CurType,int Code);
