#ifndef BLEMISH
#define BLEMISH extern "C" _declspec(dllexport)

typedef struct _MaxBlemishInfo
{
	unsigned short xStart;
	unsigned short xEnd;
	unsigned short yStart;
	unsigned short yEnd; 
	unsigned short Intensity; 
	unsigned short MaxPixelCount; 

}MaxBlemishInfo; 

typedef struct _BlemishInfo
{
 
	unsigned short xStart[MAX_PATH];
	unsigned short xEnd[MAX_PATH];
	unsigned short yStart[MAX_PATH]; 
	unsigned short yEnd[MAX_PATH]; 
	unsigned short Intensity[MAX_PATH];  
	unsigned short PixelCount[MAX_PATH];
	unsigned short POGCount;
	MaxBlemishInfo _MaxBlemish;
	
}BlemishInfo;

enum CamSeletForPOG
{
	POG_Cam0 = 0,
	POG_Cam1,
};

BLEMISH void ShowBlemishSpecDlg(HWND hwnd, BOOL bShowFlag );
BLEMISH int fnBlemishAlgorithm(BYTE *SrcBMPBuffer,int iWidth, int iHeight,BlemishInfo *BlemishRes,CamSeletForPOG Cam); 

#endif