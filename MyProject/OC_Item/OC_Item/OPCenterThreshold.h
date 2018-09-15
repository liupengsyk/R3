#ifdef Optical_FILE
#define Optical_API extern "C" _declspec(dllexport)
#else
#define Optical_API extern "C" _declspec(dllimport)
#endif

#include <afxcmn.h>
#include <math.h>

struct OFOPTCenterPixInfo
{
	int xIndex;
	int yIndex;
	int Y_Value;
};

struct OFOPTCenterIn
{
	BYTE *ImageBufferIn;
	int  Width;
	int  Height;
	BYTE Outformat;
};


Optical_API  void OpticalCenterTest(OFOPTCenterIn *_OFOPTCenterIn,BYTE *ImageBufferOut,int &CenterPixelX,int &CenterPixelY);
