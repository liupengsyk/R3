#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "MtkLib\mtk_stereo_verify.h"
#include "mems_dualccm_twolayer\mems_dualccm_twolayer.h"
#include "HwCalLib\hwdepthcalibapi.h"
#include <map>

struct PointInfo
{
   CPoint PointIndex;
   fCPoint PointValue; 
};

struct CamInfo
{
	CString CamPixelSize;
	CString CamEFL;
};
struct ChartInfo
{
	int objectPitch_x;
	int FarChartDist;
	int NearChartDist;
};
 
using namespace std;

//struct MtkStereoInput
//{
//	int *gVerify_Items ;//	Store the output information. The array length is equal to 180.  
//	USHORT* main_raw_near;// unsigned short * The raw image (bit depth: 16) of main camera captured from 20 cm. 
//	USHORT* main_raw_far;// unsigned short * The raw image (bit depth: 16) of main camera captured from 90 cm. 
//	int main_w ;//int The width of image in main camera. 
//	int main_h ;//int The height of image in main camera. 
//	USHORT* sub_raw_near;// unsigned short * The raw image (bit depth: 16) of sub camera captured from 20 cm. 
//	USHORT* sub_raw_far;// unsigned short * The raw image (bit depth: 16) of sub camera captured from 90 cm. 
//	int sub_w ;//int The width of image in sub camera. 
//	int sub_h ;// int The height of image in sub camera. 
//	CString* configname;// char  [] 
//};
//vecOutFlag = [18](2,2,-34,-338,-31,183,28,-15,1,38,2,-40,-119,-40,-46,31,-17,1)
//vecOutFlag = [19](2,2,-68,-291,-68,3,120,0,1,67,2,-70,-143,-70,-143,126,9,0,13)
//vecOutFlag = [19](2,2,-68,-291,-68,3,118,8,1,71,2,-70,-143,-70,-143,122,-22,1,11)
//vecOutFlag = [19](2,2,57,-237,35,354,113,5,1,14,1,55,-13,33,125,122,4,1,11)
//output:
//return 0: Error  1: Success     1
//gVerify_Items[0] Show the status of this stereo camera according to the following information (gVerify_Items[118]). 0: PASS  ≧1: FAIL  2
//gVerify_Items[1] Geometric quality level. (20 cm)   //0: PASS  ≧1: FAIL   2
//gVerify_Items[2] # pixels from an image top boundary to the FAIL region. (20 cm)    -34
//gVerify_Items[3] # pixels from an image right boundary to the FAIL region. (20 cm)   -338
//gVerify_Items[4] # pixels from an image bottom boundary to the FAIL region. (20 cm)   -31
//gVerify_Items[5] # pixels from an image left boundary to the FAIL region. (20 cm)      183
//gVerify_Items[6] The average intensity of main camera. (≧40) (20 cm)   28
//gVerify_Items[7] The difference of average intensity between main camera and sub camera. (20 cm)  -15
//gVerify_Items[8] The captured pattern if the MTK designed chart. (20 cm) //0: MTK chart  1: Not MTK chart   1
//gVerify_Items[9] For judging a scene is a MTK chart nor not. (20 cm)   38
//gVerify_Items 
//[10-18] 
//gVerify_Items[10-18] are the results of data at 90 cm and the meaning of gVerify_Items[10-18] are the same to the gVerify_Items[1-9]. 

class StereoCal_TestItem :public TestItemInterface
{
public:
	StereoCal_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~StereoCal_TestItem(void);

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
	
	BOOL GetImgRawbuf(CString srcImgPath,ImageInfo* img);

	int *gVerify_Items; 
	ImageInfo leftFarImg;
	ImageInfo leftNearImg;
	ImageInfo rightFarImg;
	ImageInfo rightNearImg; 

	CString ConfigName;
	CString leftFarImgPath;
	CString leftNearImgPath;
	CString rightFarImgPath;
	CString rightNearImgPath;
	
	CString cfg_dir;
	CString image_dir;

	CString Platform;

	int Mtk_StereoCal();
	int MainCamID;

	typedef int (*qcaldc_calibrate)(const char * qcaldc_config_filename);
	int Qualcomm_StereoCal();

	vector<int> vecOutFlag;
	
	//DXO
	BOOL GetImg_Info(CString srcImgPath,ImageInfo* img);
	BOOL LoadImages();
	
	vector< vector<fCPoint>> LeftFarImagePoint_xy;
	vector< vector<fCPoint>> LeftNearImagePoint_xy;
	vector< vector<fCPoint>> RightFarImagePoint_xy;
	vector< vector<fCPoint>> RightNearImagePoint_xy;
	

	//所有像点的坐标
	//vector< vector<fCPoint>> ImagePoint_xy;
	BOOL SetImagePoint_xy(ImageInfo img,vector< vector<fCPoint>> &ImagePoint_xy,CPoint &MarkPoint_ij); 

	//创建的世界坐标
	vector< vector<CPoint>> LeftObjectArray;
	vector< vector<CPoint>> RightObjectArray;
	void CreateObjectArray(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,vector< vector<CPoint>> &ObjectArray);

	//创建影像坐标
	void CreateImageArray(vector< vector<fCPoint>> NearImagePoint_xy,vector< vector<CPoint>> &ObjectArray);
	
	//根据mark点坐标得到下标
	 
	int SetPoint_ij_xy(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,map< CString,fCPoint> &ImagePoint_ij_xy);
	int GetPoint_ij_xyKey(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,vector< CString> &strKey);

	//获取相邻的值的差值 最小和最大
	int GetPitchInfoOfArray(vector<float> srcData,float &minPitch,float &maxPitch);

	//取货出现次数最多的项
	int GetMutiItemInVec(vector<int> vecItem,int &mutiItem);

	void GetMaxInVec(vector<int> vecItem,int &maxItem,int &Index);
	void GetMinInVec(vector<int> vecItem,int &minItem,int &Index);
	void GetMaxInVec(vector<float> vecItem,float &maxItem,int &Index);
	void GetMinInVec(vector<float> vecItem,float &minItem,int &Index);
	
	void GetFirstDestItemIndexInVec(vector<int> vecItem,int destItem,int &Index);

	float Left_imgPoint_xy [512*2] ;
	float Left_Oblect_xyz[512*3];

	float Right_imgPoint_xy[512*2];
	float Right_Oblect_xyz[512*3];
	

	int Cam3DCalibration();
	int CalibrationSingleCam(IN_PARAMS m_IN_PARAMS,DualCamData &m_DualCamData); 
	int NearChartDist_mm;
	int FarChartDist_mm;
	int ChartPointPitch;

	//二维排序  从左到右，从上到下，依次排列
	int PointSort(vector<fCPoint> srcPointArray,vector< vector<fCPoint>> &destPointArray);
	//从左往右    会从原数据中去掉找到的row
	void GetRowPoint(vector<fCPoint> &srcPointArray,int pitch_xy,vector<fCPoint> &destPointRow);

	int GetMaxMin_xyInPointArray(vector<fCPoint> srcPointArray,fCPoint &min_xPoint,fCPoint &max_xPoint,fCPoint &min_yPoint,fCPoint &max_yPoint);
	//获取点的距离并且得到对应的下标
	int GetPointDist(vector<fCPoint> srcPointArray,fCPoint originPoint,vector<int> &vecDist,int maxLimit,vector<int> &Index);
	//排序并且获取其原本的下标
	void SortVecAscend(vector<int> vecItem,vector<int> &vecAscendOrder,vector<int> &Index);
	void SortVecDescend(vector<int> vecItem,vector<int> &vecDescendOrder,vector<int> &Index);
	void SortVecAscend(vector<float> vecItem,vector<float> &vecAscendOrder,vector<int> &Index);
	void SortVecDescend(vector<float> vecItem,vector<float> &vecDescendOrder,vector<int> &Index);
		
	void GetDxoImgPoint(ImageInfo NearImg,ImageInfo FarImg,vector< vector<fCPoint>> &NearImgPoint,vector< vector<fCPoint>> &FarImgPoint);
	int CamDxoCalibration();
    void SetMarkPoint_ij(vector< vector<fCPoint>> &ImagePoint_xy,CPoint &MarkPoint_ij);
	void CreateImgPoint_xy(vector< vector<fCPoint>> NearImgPoint_xy,map<CString, fCPoint> farImgPointMap,vector<CString>  farImgPointMapKey,float* imgPoint_xy);
    void CreateObject_xyz(vector< vector<CPoint>> ObjectArray,int ObjectPitch,int ChartDistDiff,float* Object_xyz);

public:
	CamInfo m_CamInfo[2];
	ChartInfo m_ChartInfo;

	int Cam3DCalibrationSys();
	void CreateImgPoint_xy(vector< vector<fCPoint>> NearImgPoint_xy,vector< vector<fCPoint>> FarImgPoint_xy,float* imgPoint_xy);
	void CreateObject_xyz(vector< vector<CPoint>> NearObjectArray,vector< vector<CPoint>> FarObjectArray,int ObjectPitch,int ChartDistDiff,float* Object_xyz);
	void CreateObjectArraySys(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,vector< vector<CPoint>> &ObjectArray);
 
	int HwHdcCal(); 
	int HwMmiCheck();
	int ReleaseBuffer();
	int fileCopy();

	vector<float> depth30;
	vector<float> depth100;
	vector<CString> tempStr;
	vector<CString> tempStr30;
};