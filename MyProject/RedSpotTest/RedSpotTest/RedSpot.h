#ifdef   RedSpot_FILE
#define  RedSpotDLLAPI extern "C"  __declspec(dllexport)
#else
#define  RedSpotDLLAPI  extern "C"  __declspec(dllimport)
#endif

typedef struct _STRYrgb
{
	double Y;
	double R;
	double G;
	double B;
}STRYrgb, *pSTRYrgbr;

typedef struct _STRYCbCr
{
	double Y;
	double Cb;
	double Cr;
}STRYCbCr, *pSTRYCbCr;

typedef struct _RedSpotY
{
	STRYCbCr OutputYCbCr;
	STRYrgb OutputYrgb;
	CPoint YrgbPoint[4];
}RedSpotY;

RedSpotDLLAPI int _stdcall  RedSpotTestYUV(LPBYTE pIn,int ImgWidth,int ImgHeight ,int RatioX,int RatioY,int OutFormat,RedSpotY *pRedSpotYCbCr);
RedSpotDLLAPI int _stdcall  RedSpotTest(LPBYTE pTestBmpBuffer,int ImgWidth,int ImgHeight ,int RatioX,int RatioY,RedSpotY *pRedSpotYrgb);