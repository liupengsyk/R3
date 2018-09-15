
#ifdef  OFILMRI_EXPORTS
#define OFILMRI_API __declspec(dllexport)
#else
#define OFILMRI_API __declspec(dllimport)
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
    int Podtype;//1ΪBright_Dead 2ΪBright_Wound 3ΪDark_Dead 4ΪDark_Wound
	int DeadNum;
    int WoundNum;
	OFPixInfo* DfcStk; //���л�����Ϣ������DfcStk�С�
};

enum CamSelectForRI
{
	RI_Cam0 = 0,
	RI_Cam1,
};


//input: pRawImage, a raw8 image;
//OFILMPOD_API void ShowPODSpecDlg(HWND hwnd,BOOL ShowFlag);
//OFILMPOD_API int OFPODTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,DefectStack* DfcStk,CamSelectForPOD Cam=POD_Cam0);