#ifndef BLEMISH
#define BLEMISH extern "C" _declspec(dllexport)

typedef struct _MaxBlemishInfo
{
	unsigned int xStart;
	unsigned int xEnd;
	unsigned int yStart;
	unsigned int yEnd; 
	unsigned int Intensity; 
	unsigned int MaxPixelCount; 

}MaxBlemishInfo; 

typedef struct _BlemishInfo
{
 
	unsigned int xStart[MAX_PATH];
	unsigned int xEnd[MAX_PATH];
	unsigned int yStart[MAX_PATH]; 
	unsigned int yEnd[MAX_PATH]; 
	unsigned int Intensity[MAX_PATH];  
	unsigned int PixelCount[MAX_PATH];
	unsigned int POGCount;
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