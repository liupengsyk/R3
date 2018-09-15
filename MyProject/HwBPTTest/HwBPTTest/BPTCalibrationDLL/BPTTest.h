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
	USHORT xIndex;	//����Ӱ���е�λ��
	USHORT yIndex;
	USHORT Y_Value;
	USHORT Average;
	USHORT NeighborNum;
	USHORT NeighborFlag[20];
	USHORT x;	//Channel��λ��
	USHORT y;
};

struct BadPixelTable
{
	int DefectStackCnt;
	PixelInfo* BadPixelStack; //���л�����Ϣ������DfcStk�С�
};

struct OFPixInfoStack
{
	PixelInfo* BadStk;
	int nBadCnt;
};

//input: pRawImage, a raw10 image;
//RetValue TRUE: OK FALSE :��NG
BPT_API BOOL GetBPTTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, BadPixelTable *Table,int SaveData, int SaveImage);