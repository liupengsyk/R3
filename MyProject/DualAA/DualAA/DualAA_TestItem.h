#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "mems_dualccm_twolayer\mems_dualccm_twolayer.h"
#include "ImageProc.h"
#include "LIB_Halcon/include/Halcon.h"
#include "LIB_Halcon/include/cpp/HalconCpp.h"
using namespace std;
using namespace Halcon;
#include <map>


struct SpecData
{
	//与控件交换数据
	CString strRotationSpc;
	CString strTiltXSpc;
	CString strTiltYSpc; 

	float RotationSpc;
	float TiltXSpc;
	float TiltYSpc;

};


struct PointChartInfo
{  
	int ObjectLength; 
	int PointPitch; 
};


struct CamBaseInfo
{
	float FocusLength;
	float PixelSize;
	CString strFocusLength;
	CString strPixelSize;
};

struct CamInputInfo
{

	//setting Input
	CamBaseInfo baseInfo; 
	IN_PARAMS m_IN_PARAMS;
	OUT_PARAMS m_OUT_PARAMS;  
	float pitchNearImg;
	float pitchFarImg;
	//data out
	DualCamData m_DualCamData;
};

struct CamDualData
{
	CamInputInfo CamInfo[2];
	PointChartInfo Infinity_PointChartInfo;
	PointChartInfo Macro_PointChartInfo; 
	SpecData m_SpecData; 
	DualCamData DualData_dif;
};

class DualAA_TestItem :public TestItemInterface
{
public:
	DualAA_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~DualAA_TestItem(void);

	int InitItem();

	int Testing();       //子类重载测试代码放放在此函数
	int Initialize();      //子类重载 初始化代码
	int Finalize();      //子类重载 初始化

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

private:
	OptionDlg* pDlg;
	 int CamID;

public:
	ImageInfo* pImgInfo[2];

	CamDualData CamCal;
	BOOL bOnAA;
	BOOL bContinue;

	int GetCamTilt(int CamID);
	int GetTilt_dif();
	int ProcTiltData(BOOL bShowInfo = TRUE);
	void ShowInfo(int CamID);

	void SetDualCamData();
	 

	unsigned char *pBmpRed ;
	unsigned char *pBmpGreen ;
	unsigned char *pBmpBlue ; 
	void GetImgPoint(int CamID, vector<fCPoint> &NearImgPoint, vector<fCPoint> &FarImgPoint);	 
	void union_Array(vector<fCPoint> srcImgPoint,vector<fCPoint> &ImgPoint,float pitch_);
	void ImgPointGetRow(int CamID,vector<fCPoint> &ImgPoint,vector<fCPoint> &ImgRowPoint,float avg_y,int range); 
	void GetAvgPoint_y(vector<fCPoint> ImgPoint,float &avg_y);
	BOOL ImgPoint_xyProc(int CamID,vector<fCPoint> &ImgPoint,vector< vector<fCPoint>> &ImgCoordinate,int pitch_y,vector<fCPoint> &invalidPoint,BOOL Disflag); //Disflag=0:近焦 =1：远焦
	  
	//获取mark点坐标
	BOOL GetMark_xy(int CamID,vector<fCPoint> invalidPoint_near,fCPoint &mark_xy_near,vector<fCPoint> invalidPoint_far,fCPoint &mark_xy_far);
	//获取mark点下标
	void GetMark_ij(vector< vector<fCPoint>> ImgCoordinate,fCPoint mark_xy, CPoint &mark_ij,int pitch_);

	//获取像点相对mark的下标
	void GetImgPoint_xy_ij(vector< vector<fCPoint>> ImgCoordinate, CPoint mark_ij,vector<CString> &imgPoint_ij);
	//match near与far的对应点
	void matchNearFar(vector<CString> imgNearPoint_ij,vector<CString> imgFarPoint_ij,vector<CString>& union_ij);
	//找出对应点
	void GetImg_xy(vector< vector<fCPoint>> ImgCoordinate,CPoint mark_ij,vector<CString> union_ij,vector<float> &img_xy);
	//创建物理坐标
	void createObject_xyz(int CamID,vector<CString> union_ij,vector<int> &object_xyz);
  

	void SortVecAscend(vector<float> vecItem,vector<float> &vecAscendOrder,vector<int> &Index);
	void ReOrderfCpointAscend_x(vector<fCPoint> &vecItem);
	void ReOrderfCpointAscend_y(vector<fCPoint> &vecItem);
	BOOL RowPointCheck(vector<fCPoint> &vecItem,int pitch_,vector<fCPoint> & InvalidPoint,vector<fCPoint> &srcPoint,BOOL Disflag); //// 返回true 不用重排， 返回false,需要重排src point
	void AlignArrayRow(vector<  vector<fCPoint>> &arrayRow,int pitch_);
	 
	BOOL findShapeModel(int CamID,vector< vector< CString>> shapeModelPath,vector< vector<vector<fCPoint>>>* shapeCoordinate);
	void findShapeModel(CString imgPath,CString shapeModelPath,vector<fCPoint>* shapeCoordinate);

	void showCrossCoordinate(int CamID,vector<fCPoint> corrdinate);

	void corrdinateOffset(float ratio,float height,float &img_xy,float distance,float efl,int length);

public:
	float offset_tx;
	float offset_ty;
	float offset_tz;

	BOOL blogProc;
private:
	int failGetMarkTimes;
	void EndTest();
	int error_Limit;

	void LogProc(CString functionName);

	int CheckTimes;

	BOOL camGetTiltPass[2];

	void saveProcImage(int CamID,CString name);
	 
};