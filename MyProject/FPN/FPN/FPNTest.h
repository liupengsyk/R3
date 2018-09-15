#ifdef FPNTEST_EXPORTS
#define FPNTEST_API __declspec(dllexport)
#else
#define FPNTEST_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define GetPrivateProfileInt  GetPrivateProfileIntW
#else
#define GetPrivateProfileInt  GetPrivateProfileIntA
#endif // !UNICODE

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define ABS(a)	   (((a) < 0) ? -(a) : (a))	
#define YVALUE(r,g,b) (0.299*double(r)+0.587*double(g)+0.114*double(b))
#define RELEASE(a) (((a)!=0) ? (delete[] (a),(a) = 0):((a)=0))

FPNTEST_API BOOL FixedPatternNoise(BYTE* _pRGBImage, int RowStart, int ColStart, int RowEnd, int ColEnd,double &RowDifMaxMeam,double &ColDifMaxMean,int &RowPos,int &ColPos);


