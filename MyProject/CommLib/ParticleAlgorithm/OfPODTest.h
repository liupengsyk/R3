
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
    USHORT Podtype;//1ΪBright_Dead 2ΪBright_Wound 3ΪDark_Dead 4ΪDark_Wound
	USHORT DeadNum;
    USHORT WoundNum;
	OFPixInfo* DfcStk; //���л�����Ϣ������DfcStk�С�
};

enum CamSelectForPOD
{
	POD_Cam0 = 0,
	POD_Cam1,
};


//input: pRawImage, a raw8 image;
OFILMPOD_API void ShowPODSpecDlg(HWND hwnd,BOOL ShowFlag);
OFILMPOD_API int OFPODTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,DefectStack* DfcStk,CamSelectForPOD Cam=POD_Cam0);
