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
	unsigned int Max_Differ;
	unsigned int Range;
	int RemainPixelCount;
	MaxBlemishInfo _MaxBlemish;
	
}BlemishInfo;


BLEMISH void ShowBlemishSpecDlg(HWND hwnd);
BLEMISH int fnBlemishAlgorithm(BYTE *SrcBMPBuffer,int iWidth, int iHeight,BlemishInfo *BlemishRes); 

#endif