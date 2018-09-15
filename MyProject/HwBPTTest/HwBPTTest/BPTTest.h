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
	USHORT xIndex;
	USHORT yIndex;
	USHORT Y_Value;
	USHORT Average;
	USHORT NeighborNum;
	USHORT NeighborFlag[20];
	USHORT x;
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
BPT_API void GetBPTTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, BadPixelTable *Table,int SaveData, int SaveImage);
