#pragma once
#include "OFccmDef.h"
 
#include "RawOptionDlg.h"

class OFccmControl;

class ImageExplorer
{
public:

	//���캯��
	ImageExplorer(HWND ScrWnd, OFccmControl* Tccm,int CamID);
	~ImageExplorer(void);

	//��ʼ��ͼ����ʾ��Ϣ
	void Init(void);

    /***ͼ��Ĵ��봴��***/
	BOOL Open(BOOL bRaw = FALSE);      //ͨ���Ի������ͼ���ļ�
	BOOL Open(ImageInfo* CurImage);     //ͨ��RAWbuffer��RGB24buffer����ͼ��
	BOOL Open(CString srcImgPath);//��ָ��·��ͼƬ
	BOOL CreateRawImage(LPCTSTR PathName,BOOL OffLineTest = FALSE);
	BOOL CreateImage(BYTE* RGB24,int width, int height);
	 

	//�õ�������ָ��
	BYTE*   GetRGB24Buffer() ;

	USHORT* GetRawBuffer();
	 

	//�õ�ͼ�񳤿�
	int GetImageWidth();
	int GetImageHeight();
	BYTE GetRawBits();
	int GetRawFormat();


	//�ͷ�����
	void Free(void);

	//ͼ����ʾ
	BOOL ShowNaviIamge();
	BOOL ShowScrImage();

	//ͼ�������Ϣ��ʾ
	void ShowPointInfo(CPoint point);
	void ShowScrImageInfo(void);


	struct RAWINFO
	{
		int width;
		int height;
		ULONGLONG length;
		int bits;
		int outformat;
	}sRawInfo;

	struct NAVISTATUS
	{
		BOOL Moveable;
		BOOL LBDown;
		CPoint CurPoint;
		CPoint PrevPoint;
	}NaviStatus;

	struct SCRSTATUS
	{
		BOOL LBDown;
		CPoint CurPoint;
		CPoint PrevPoint;
	}ScrStatus;

	//����ͼ���ڵ������Ӧ
	void OnLButtonDown_NaviWnd(CPoint Point);
	void OnLButtonUp_NaviWnd(CPoint Point);
	void OnMouseMove_NaviWnd(CPoint Point);

	//Դͼ���ڵ������Ӧ
	void OnMouseWheel(short zDelta, CPoint pt);
	void OnLButtonDown(CPoint Point);
	void OnLButtonUp(CPoint Point);
	void OnMouseMove(CPoint Point);

private:

	OFccmControl* pTccm;      	//����ָ��

	enum TYPE
	{
		NULLTYPE,
		RAW, 
		NON_RAW,		
	};
	 
	CImage ScrImage;           //ԭͼ
	HWND   hScrWnd;            //��ʾԴͼ�Ĵ��ھ��
	CPoint ScrImageStartPoint; //ԭͼ����ʾ��㣨������
	double dStartPoint_x;      //ԭͼ����ʾX С�����ģ����㣩
	double dStartPoint_y;      //ԭͼ����ʾY С�����ģ����㣩
	float  ZoomRatio;          //���ű���
 
	//����������
	void CreateBuffer();
 
	//������ָ��
	BYTE*   RGB24_buffer;  
	void CreateRGB24buffer(void);

	USHORT* raw_buffer;
	void CreateRaw_buffer(void);
	 
	void   DistanceMapping(CPoint CurPoint,CPoint PrevPoint); //ӳ�����
	BOOL   CheckPointInNaviRect(CPoint Point); //�ж������Ƿ��ڵ�������

	RawOptionDlg m_RawOptionDlg;
	void CreateRawGrayPixel(BYTE* RGB24, USHORT* Raw, int width, int height, int Bits);
	void CreateRawColorPixel(BYTE* RGB24, USHORT* Raw, int width, int height, int Bits, int outformat);

	void CreateRawGrayPixel(BYTE* RGB24, BYTE* Raw, int width, int height);
	void CreateRawColorPixel(BYTE* RGB24, BYTE* Raw, int width, int height, int outformat);
public:
	int CamID;
	CString GetImgPath();

	CString GetImgName();
private:
	CString ImgName;
	 
};

