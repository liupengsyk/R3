#ifdef  BPT_EXPORTS
#define BPT_API __declspec(dllexport)
#else
#define BPT_API __declspec(dllimport)
#endif
struct AllPixelInfo
{
	USHORT xIndex;
	USHORT yIndex;
	USHORT Y_Value;
};
struct PixelInfo
{
	USHORT xIndex;	//整张影像中的位置
	USHORT yIndex;
	USHORT Y_Value;
	USHORT Average;
	USHORT NeighborNum;
	USHORT NeighborFlag[20];
	USHORT x;	//Channel中位置
	USHORT y;
};

struct BadPixelTable
{
	int DefectStackCnt;
	PixelInfo* BadPixelStack; //所有坏点信息存在在DfcStk中。
};

struct OFPixInfoStack
{
	PixelInfo* BadStk;
	int nBadCnt;
};

//input: pRawImage, a raw10 image;
//RetValue TRUE: OK FALSE :　NG
BPT_API BOOL GetBPTTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, BadPixelTable *Table,int SaveData, int SaveImage);
