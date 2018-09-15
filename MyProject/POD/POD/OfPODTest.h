
#ifdef  OFILMPOD_EXPORTS
#define OFILMPOD_API __declspec(dllexport)
#else
#define OFILMPOD_API __declspec(dllimport)
#endif

struct OFPixInfo
{
	unsigned short xIndex;
	unsigned short yIndex;
	unsigned short Y_Value;
	unsigned short Average;
};

struct DefectStack
{
	unsigned short DefectStackCnt;
    unsigned short Podtype;//1为Bright_Dead 2为Bright_Wound 3为Dark_Dead 4为Dark_Wound
	unsigned short DeadNum;
    unsigned short WoundNum;
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
