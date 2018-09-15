#pragma once
#include "OFccmDef.h"
 
#include "RawOptionDlg.h"

class OFccmControl;

class ImageExplorer
{
public:

	//构造函数
	ImageExplorer(HWND ScrWnd, OFccmControl* Tccm,int CamID);
	~ImageExplorer(void);

	//初始化图像显示信息
	void Init(void);

    /***图像的打开与创建***/
	BOOL Open(BOOL bRaw = FALSE);      //通过对话框加载图像文件
	BOOL Open(ImageInfo* CurImage);     //通过RAWbuffer和RGB24buffer创建图像
	BOOL Open(CString srcImgPath);//打开指定路径图片
	BOOL CreateRawImage(LPCTSTR PathName,BOOL OffLineTest = FALSE);
	BOOL CreateImage(BYTE* RGB24,int width, int height);
	 

	//得到数据区指针
	BYTE*   GetRGB24Buffer() ;

	USHORT* GetRawBuffer();
	 

	//得到图像长宽
	int GetImageWidth();
	int GetImageHeight();
	BYTE GetRawBits();
	int GetRawFormat();


	//释放数据
	void Free(void);

	//图像显示
	BOOL ShowNaviIamge();
	BOOL ShowScrImage();

	//图像相关信息显示
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

	//导航图窗口的鼠标响应
	void OnLButtonDown_NaviWnd(CPoint Point);
	void OnLButtonUp_NaviWnd(CPoint Point);
	void OnMouseMove_NaviWnd(CPoint Point);

	//源图窗口的鼠标响应
	void OnMouseWheel(short zDelta, CPoint pt);
	void OnLButtonDown(CPoint Point);
	void OnLButtonUp(CPoint Point);
	void OnMouseMove(CPoint Point);

private:

	OFccmControl* pTccm;      	//主控指针

	enum TYPE
	{
		NULLTYPE,
		RAW, 
		NON_RAW,		
	};
	 
	CImage ScrImage;           //原图
	HWND   hScrWnd;            //显示源图的窗口句柄
	CPoint ScrImageStartPoint; //原图的显示起点（整数）
	double dStartPoint_x;      //原图的显示X 小数点后的（浮点）
	double dStartPoint_y;      //原图的显示Y 小数点后的（浮点）
	float  ZoomRatio;          //缩放倍率
 
	//创建数据区
	void CreateBuffer();
 
	//数据区指针
	BYTE*   RGB24_buffer;  
	void CreateRGB24buffer(void);

	USHORT* raw_buffer;
	void CreateRaw_buffer(void);
	 
	void   DistanceMapping(CPoint CurPoint,CPoint PrevPoint); //映射距离
	BOOL   CheckPointInNaviRect(CPoint Point); //判断鼠标点是否在导航框内

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

