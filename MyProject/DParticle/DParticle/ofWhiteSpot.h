#ifdef  WhiteSpot_EXPORTS
#define WhiteSpot_API __declspec(dllexport)
#else
#define WhiteSpot_API __declspec(dllimport)
#endif

#define TRUE 1
#define  FALSE 0

typedef unsigned char BYTE;
typedef int BOOL;
typedef unsigned int UINT;
typedef long LONG;
typedef int INT;

#include <Windows.h>


struct WPixInfo
{
	USHORT xIndex;
	USHORT yIndex;
	USHORT Y_Value;
	USHORT Average;
};

struct WDefectStack
{
	USHORT DefectStackCnt;
    USHORT Podtype;//1ΪBright_Dead 2ΪBright_Wound 3ΪDark_Dead 4ΪDark_Wound
	USHORT DeadNum;
	WPixInfo* DfcStk; //���л�����Ϣ������DfcStk�С�
};


//input: pRawImage, a raw8 image;
//WhiteSpot_API void ShowWhiteSpotSpecDlg(HWND hwnd,BOOL ShowFlag);
WhiteSpot_API int  WhiteSpotTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,WDefectStack* DfcStk);