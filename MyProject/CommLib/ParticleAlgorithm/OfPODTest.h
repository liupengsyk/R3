
#ifdef  OFILMPOD_EXPORTS
#define OFILMPOD_API __declspec(dllexport)
#else
#define OFILMPOD_API __declspec(dllimport)
#endif

struct OFPixInfo
{
	USHORT xIndex;
	USHORT yIndex;
	USHORT Y_Value;
	USHORT Average;
};

struct DefectStack
{
	USHORT DefectStackCnt;
    USHORT Podtype;//1为Bright_Dead 2为Bright_Wound 3为Dark_Dead 4为Dark_Wound
	USHORT DeadNum;
    USHORT WoundNum;
	OFPixInfo* DfcStk; //所有坏点信息存在在DfcStk中。
};

enum CamSelectForPOD
{
	POD_Cam0 = 0,
	POD_Cam1,
};


//input: pRawImage, a raw8 image;
OFILMPOD_API void ShowPODSpecDlg(HWND hwnd,BOOL ShowFlag);
OFILMPOD_API int OFPODTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,DefectStack* DfcStk,CamSelectForPOD Cam=POD_Cam0);
