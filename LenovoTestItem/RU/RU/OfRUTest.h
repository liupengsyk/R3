
#ifdef  OFILMRU_EXPORTS
#define OFILMRU_API __declspec(dllexport)
#else
#define OFILMRU_API __declspec(dllimport)
#endif

struct OFPixInfo
{
	int xIndex;
	int yIndex;
	int Y_Value;
	int Average;
};

struct DefectStack
{
	int DefectStackCnt;
    int Podtype;//1为Bright_Dead 2为Bright_Wound 3为Dark_Dead 4为Dark_Wound
	int DeadNum;
    int WoundNum;
	OFPixInfo* DfcStk; //所有坏点信息存在在DfcStk中。
};

enum CamSelectForRU
{
	RU_Cam0 = 0,
	RU_Cam1,
};


//input: pRawImage, a raw8 image;
//OFILMPOD_API void ShowPODSpecDlg(HWND hwnd,BOOL ShowFlag);
//OFILMPOD_API int OFPODTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,DefectStack* DfcStk,CamSelectForPOD Cam=POD_Cam0);
