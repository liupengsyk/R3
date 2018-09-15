#pragma once
#include "ccmbaseinterface.h"
#include "DeviceInterface.h"
#include "ConfigManage.h"
#include "CameraControl.h"
#include "LoggerManage.h"
#include "TestItemControl.h"
#include "OFccmDef.h"
#include "VcmDriverInterface.h"
#include "OtpSensorInterface.h"
#include "imagekit.h"
#include "resource.h"
#include "ImageExplorer.h"
#include "ClientMFCSocket.h"
#include "ServerMFCSocket.h"
#include "GlobalSetting.h"
#include "MachineControl.h"
#include "ConfigFileRw.h"
#include <WinSock.h>
#include <map>
#include "global.h"
#include "ImgSensorLib.h"
#include "SerialPortLED.h"
using namespace std;

enum DUAL_AUTOTEST_STATUS
{
	DUAL_AUTO_TEST_INIT_STATE = 0,
	DUAL_AUTO_TEST_TESTING,
	DUAL_AUTO_TEST_FINISHED,
};

struct MesRes
{
	BOOL Res;
	CString MesErrorInfo;
};

struct CamTab
{ 
	CVcmDriverInterface* pVcmDriver;
	COtpSensorInterface* pOtpSensor; 
	CImgSensorLib* pImgSensor;

	CCameraControl* pCameraCtrl;
	ConfigManage* pConfig;
	TestItemControl* pTestCtrl;
	ImageExplorer* pImgExplorer;
	ImageInfo m_ImageInfo;
	
	HWND hDisplayWnd;	 
	CRect DisplayRect;
	CDC *DisplayMemryDC;

	HWND hPanel;
	HWND hLogger;

	LoggerManage* m_log;

	Cam_Buf pBuf; 
	Cam_State bState;
	Cam_ThreadCtrl hThreadCtrl;
	Cam_FrameInfo m_FrameInfo;

	CRITICAL_SECTION m_csLock; 
	CRITICAL_SECTION m_DisPlayLock;

	BOOL bTripleBufferDirty;

	ClientMFCSocket* camSocket;//网络通信

	ServerMFCSocket* pServerSocket;//网络通信

	float fWorkingCurrent[5];
	BOOL bWorkingCurrentFailed;
	int channel;
	DualCamData m_DualCamData;
	CString logPath; 
	CPoint CamPoint;

	CString otpMapPath;
	CString otpByteInPath; 
	CString otpByteOutPath;
	CString otpItemdataPath;
	CString otpCheckDataPath; 

	CString fuseID;

	int MesID;
	BOOL usedMes;
	CMesSystem m_Mes;
	BOOL bSuccessMes;

	CString errorInfo;
	int errorCode;
	int NGTestItemID;//NG测试项ID
	int CurrentChannel;//当前光源的channel
	int bLEDG4C_OK;

	int MesSelect;
	int TestItemNum;
	int TestItemPass;
	int iDualAutoTestStatus;//双摄的时候用
	BOOL bFocusEnd;
	BOOL bFocusStatusChange;

	BOOL bOnlyOpenCamera;
	MesRes MesResult;
};

class OFccmControl :
	public ccmBaseInterface
{
public:
	OFccmControl(HWND hTestDlgWnd,HWND hDisplayWnd[],HWND hPanel[],HWND hLogger[],int CameraNum);
	~OFccmControl(void);
	
public:
	/************句柄变量定义******/
	HWND hTestDlgWnd;	
 
	CamTab Cam[2];
	CString ReciveData;
	int CameraNum;
	BOOL InitValue();

	int CamInit();
	

public:
	/**************Logger接口实现*********/	 
	virtual void Ctrl_Addlog(int CamID,CString info,COLORREF color = RGB(1,1,1),int yHeight = 200);
	virtual void Ctrl_Addlog(int CamID,int info ,COLORREF color = RGB(1,1,1),int yHeight = 200 );
	virtual void Ctrl_ClearLog(int CamID);
public:
	/**DC处理****/

	//map<HDC,HWND> map_wnd_dc; //句柄与DC的索引
	CSerialPortLED m_LEDPort;
	void ReturnSetLEDG4C(int CamID,int recLed);
	void LEDPortInitia(CWnd *pPort);
	BOOL Unlock(int CamID);
	BOOL IsUSBlock();
	BOOL bCCMCtrlInit();
	
	BOOL bMemDCInit(int CamID);
	
	//	/************** DC与显示  *******************/
	//public:
	void CreateDisplayMemDC(int CamID);
	void ResetMemdcBackGround(int CamID,COLORREF BKcolor = RGB(100,100,100),CString strLog = _T("OFG-CCM"),COLORREF strColor = RGB(40,40,40));
	 
	virtual CDC* Ctrl_GetDisplayWndMemDC(int CamID) ;             //得到辅助窗口DC
	 virtual void Ctrl_GetDisplayWndRect(CRect* rect,int CamID) ;   //得到主显示窗口的大小
	virtual HWND Ctrl_GethDisplayWnd(int CamID);
	 
	virtual int Ctrl_ShowMemDC(int CamID);
	virtual int Ctrl_CopyBackGroundToMemDC(int CamID);
	virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF strColor,CString strLog,UINT nSize =15,UINT cx = 10,UINT cy =10);
	virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF bkColor);
	virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF recColor,CRect drawRect,ImageInfo img);
	virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,CPoint sPoint,CPoint ePoint,ImageInfo img);
	virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,int cx0,int cy0,int cx1,int cy1,ImageInfo img);
	virtual int Ctrl_DrawingToMenDC(int CamID,int Width,int Height,BYTE* SrcImg,int ImgType);
	 
	void ResetBackGround(int CamID);
	 
	void DrawToDisplayMemDc(int CamID);
	int GetDynamicCompressRatio(int CamID);
	int Ctrl_ShowMemDCChart(int CamID);

public:  

	CString	m_strFrameInfo;
	CString strDisfps;
	 
	BOOL LoopFlag;
	BOOL GetLoopFlag(int CamID);
public:
	/**************camera 数据*********/
	
	Cam_Buf m_Cam_Buf;

	BOOL TripleBufferFlip(LPBYTE *pBuf, BOOL bPrimaryBuffer,int CamID);
	
	BOOL GetBuffer(int CamID);
	void KillDataBuffer(int CamID);

	int OpCameraNum;


public:
	/*******大类*******/
	static GlobalSetting globalset;
	//static int DeviceType;


public: 
	int OpenCamera(int CamID);
	int CamStartInit(int CamID);
	void OpenCameraMsg(int CamID);
	int MachineOpenCamBack(int CamID,BOOL bResult);
	 
	int StartDualCamThread();
	int StartNormalCamThread(int CamID);
	
	void ResumeCameraMsg(int CamID);	
	virtual int Ctrl_PauseGrabFrame(int CamID) ;     //让图像采集板停止抓取帧
	virtual int Ctrl_ResumeGrabFrame(int CamID) ; 
	void PauseCameraMsg(int CamID);
	
	int CloseCamera(int CamID,BOOL bResult);
	int CloseCamera_Final(int CamID, BOOL bResult); //最终关闭模组
	void CloseCameraMsg(int CamID); 

	static UINT GrabFramThread_Cam(void* param);

	static UINT GrabFramThread_Cam0(void* param);
	static UINT DisplayThread_Cam0(void* param);
	static UINT DisplayThread_Cam0OnlyOpen(void * param);

	static UINT GrabFramThread_Cam1(void* param);
	static UINT DisplayThread_Cam1(void* param);
	static UINT DisplayThread_Cam1OnlyOpen(void * param);

	 
	int GrabFramThread();

	int GrabFramThread(int CamNum);	
	int DisplayThread(int CamNum);
	int DisplayThreadOnly(int CamNum);


	static UINT ControlThread(void* param);	
	int ControlThread();	
	HANDLE    hControlThread;
	int       ControlThreadID;
	BOOL bControlThreadOn; 
	HANDLE hControlerProc;             //所有线程结束 
	CRITICAL_SECTION proc_cs;
	void PostMsgToControler(int CamID,int wp,int lp);

	ImageInfo* Ctrl_GetImgInfo(int CamID);

public:
	/***AF操作**/
	virtual int VcmDr_InitAF(int CamID) ;
	virtual int VcmDr_Standby(int CamID);
	virtual int VcmDr_ReadAF_Code(USHORT* code,int CamID);
	virtual int VcmDr_WriteAF_Code(USHORT code,int CamID);
	virtual int VcmDr_AutoFocus(int CamID);

	/***********配置管理器的(ini文件读写) 封装**************/

public:
	//写入指定的分区下某子项的值（STRING）
	virtual void Config_WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string,int CamID) ;
	//写入指定的分区下某子项的值(INT)
	virtual void Config_WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC,int CamID=0) ;
	//读取指定的分区下某子项的值(INT)
	virtual int Config_ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int CamID);
	//读取指定的分区下某子项的值(STRING)
	virtual LPCTSTR Config_ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID) ;
	//删除子项(名称和值)
	virtual void Config_ClearConfig(LPCTSTR AppName,int CamID) ; 
	//删除分区(分区名称和其下所有子项)
	virtual void Config_ClearConfig(LPCTSTR AppName, LPCTSTR KeyName,int CamID) ; 
	//得到TCCF文件的路径
	virtual LPCTSTR Config_GetConfigPathName(int CamID) ; 
	virtual LPCTSTR Config_GetExeDir();
	vector<CString> GetvecDriver();

	vector<CString>  GetvecImgSensor();

	virtual int Ctrl_GetOutPutFormat(int CamID);
	virtual USHORT Ctrl_GetCamTemperature(int CamID);

	void ExecuteItemTest(int ID,int CamID);

	int  OSTest(int CamID);
	int  OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID);
	void SaveOSResult(int * osValue, int NGCount,int CamID);
	int SaveReport(LPCTSTR lpszName, 
		LPCTSTR lpszReportHeaders, 
		LPCTSTR lpszLowerLimits, 
		LPCTSTR lpszUpperLimits,
		LPCTSTR lpszContents,
		LPCTSTR lpszResult,int CamID);
 
public:
	BYTE GetRawBits(SensorTab* CurrentSensor);
	 
public:
//	void SaveWorkingCurrent(int CamID);
	int CheckStandbyCurrent(int CamID);
//	int CheckWorkingCurrent(int* Current, int* Voltage,int CamID);
	void ShowWorkingCurrentAndVoltage(CString fpsInfo,int CamID);
	void ShowStandByCurrentAndVoltage(int CamID);
	void ShowStandByCurrentTestingInfo(int CamID);
	void ShowTextInfo(LPCTSTR str,int CamID);

	 
	int QueryCurrentStatus(int CamID);

	void UpdateUI(int type,int CamID);	 
	 

	virtual HWND Ctrl_GetMainWndHwnd();

	ImageInfo img ;


	int Offline_Process_thread(int CamID);
	void StartOfflineProcessThread(int CamID);
	void StopOfflineProcessThread(int CamID);

	static UINT Offline_Process_thread0(LPVOID pParam);
	static UINT Offline_Process_thread1(LPVOID pParam);
	 
	BOOL isOffline(int CamID);
	void ExitOfflineMode(int CamID);
	BOOL EnterOfflineMode(int CamID,BOOL bEnterOffLine);


	int CheckStaticCurrent(int CamID);
	void ShowStaticCurrentAndVoltage(int CamID);
	void ShowStaticCurrentTestingInfo(int CamID);


	void PauseDisplay(int CamID);

	 
	void InitSocket(int CamID,BOOL bCreateSrv);
	void SendMsgMachine(int CamID);
	void DualAutoTestSendMsgMachine(int CamID);

	virtual CRect Ctrl_CalcRect_DisplayScreen(double cx, double cy, double x_length, double y_length,int CamID);
	void UpdateInspectionItemDlg(void);

	//ImgTypeDUAL

	// 1: //保存JPG格式图片
	//2://保存BMP格式图片			 
	//3://保存raw二进制流格式图片
	// 4://保存RAW8的图片		
	// 5:// 保存指定的buf，为jpg格式
	//imgBuf   保存指定的buf
	virtual void Ctrl_SaveImage(int CamID,BOOL bSetPath = FALSE, CString PathName = NULL,CString FileName = NULL,int ImgType=1,BYTE* imgBuf = NULL);
	private:
	void SaveFullRGB(CString PathName,int CamID);
	void SaveRaw(CString PathName,int CamID,int nameType = 0);
	void SaveRaw8(CString PathName,int CamID,int nameType = 0);
	 
	void SaveImgbuftoImage(CString PathName,int CamID,BYTE* imgBuf);
	public:
	virtual int Otp_OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page = 0);
	virtual int Otp_OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page = 0);

	virtual int Otp_WriteToOtpBoard(int CamID,CString name,vector<BYTE> byteData,BOOL bIn=TRUE) ;	 
	virtual int Otp_ReadOtpBoard(int CamID,CString name,vector<BYTE> *byteData,BOOL bIn = TRUE);

	virtual int Otp_WriteToOtpBoard(int CamID,CString name,vector<USHORT> byteData,BOOL bIn=TRUE) ;	 
	virtual int Otp_ReadOtpBoard(int CamID,CString name,vector<USHORT> *byteData,BOOL bIn = TRUE);

	vector<CString> GetvecOtpSensor(); 
	int pCam_Opencamera(int CamID);

	
	/****设备相关*****/
public:
	CDeviceInterface* pDevice;	 

	/**************I2c接口实现*********/
	virtual int Device_WriteI2c(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int CamID);
	virtual int Device_ReadI2c(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int CamID);
	 


	BOOL isStandby(int CamID);
	BOOL isRunning(int CamID);
	BOOL isPause(int CamID);
private:
	int  CamID;
public:
	BOOL isClose[2];
	BOOL isOpen[2]; // 0 表示
	int ShowResult[2];// 0 表示初始状态  1表示pass -1 表示fail
public:
	void SetCamID(int CamID);

public:
	 
	virtual void Ctrl_SetTestInfo(int CamID,int TestItemID,TestItemInfo mTestItemInfo) ;
	virtual void Ctrl_GetTestInfo(int CamID,int TestItemID,TestItemInfo& mTestItemInfo) ;
	void Ctrl_GetTestInfo(int CamID,CString itemName,TestItemInfo& mTestItemInfo) ;
 
private:
	CString strCamCode;
public:
	virtual void SetCamCode(CString strCamCode);
	virtual CString Ctrl_GetCamCode();
	virtual CString Sensor_GetCamFuseID(int CamID);
	virtual void GetDirAllFile(CString dirPath,vector<CString> &fileList);
	virtual void GetDirAllDirs(CString dirPath,vector<CString> &dirList) ;
	virtual BOOL isFileExist(CString filePath);


	BOOL bWriteExp;
public:
	virtual int Ctrl_GetCamType();
	int GetExpCurrentY(int CamID);

	CString deviceName[4];

	CRITICAL_SECTION m_lockChannel;

	int ContinueTestTime;

	virtual void Ctrl_SetTestItemID(int CamID,int TestItemID);
	virtual  int Ctrl_GetTestItemID(int CamID);

	virtual void Ctrl_SetDualCamData(int CamID,DualCamData m_DualCamData);
	virtual DualCamData* Ctrl_GetDualCamData(int CamID);
	virtual void Ctrl_AutoAlignMoveTo(int CamID,int motor,double pos);
	virtual float Ctrl_AutoAlignGetPos(int CamID,int motor);

public:
	CString  Ctrl_GetCurrentLocalTime();
	MachineControl* pMachineCtrl;
	BOOL bTEST;
	 
	virtual CString Ctrl_GetCurrentPath();
	BOOL OpenImg(int CamID,CString srcImgPath);
	 
	virtual void Ctrl_AddToLog(CString LogInfo,int CamID);
	void CreateLogFile();

	void DrawingAndShowResult(int CamID,CString strInfo,BOOL bResult,BOOL Result_ok2 = FALSE,BOOL bReFreshUi = FALSE);
	void InitCamServerSocket(int CamID);

	void Ctrl_SetAAResult(BOOL bAaResult); 

	BOOL bFindSn;
	USHORT GetRgain(int CamID);
	USHORT GetBgain(int CamID);
	float Rgain;
	float Bgain;
	int SetRgain(int CamID);
	int SetBgain(int CamID);
	void Ctrl_SetRgain(int CamID,float Rgain );
	void Ctrl_SetBgain(int CamID,float Bgain);
	void ManuBalance(int CamID);
	virtual CPoint Ctrl_GetCurrentRoiPos(int CamID);
	virtual int VCM_WriteAF_Distance(int Distance, int CamID);
	BOOL SetAF_CodeMap( int CamID);
	 
	int GetAfCode(vector<int> vecCode,vector<int> vecDist,int Dist);
	vector<int> GetAF_DisMap(int CamID);
	vector<int> vecAfCode;
	vector<int> vecDist;
	
 
	
public:
	ConfigFileRw *pConfigFileRw;
	void OnLButtonUpPoint(int CamID,CPoint cp); 
	void SetCamMousePos(int CamID,CPoint cp);
  
	virtual  void Ctrl_GetCamCurrent(int CamCurrent[],int CamID);

	virtual  int Device_WriteI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0); 
	virtual  int Device_ReadI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID= 0,int CamID = 0);

	
	static int OpenTimes;
	virtual int Ctrl_bReOpen(int &OpenTimes);
	
public:	 
	 
	virtual int Sensor_ReadExp(int CamID,int &exp) ;
	virtual int Sensor_WriteExp(int CamID,int exp) ;

	virtual int Sensor_ReadGain(int CamID,USHORT &gain) ;
	virtual int Sensor_WriteGain(int CamID,USHORT gain);
 
	virtual int Sensor_Init(int CamID); 
	virtual int Sensor_ApplyAWBGain(int CamID,int rg, int bg, int Typical_rg, int Typical_bg) ;
	virtual int Sensor_ApplyLsc(int CamID,BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup) ;
	virtual int Sensor_GetLsc(int CamID,BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);
	virtual int Sensor_ApplySpc(int CamID,short *pSPC);
	virtual int Sensor_spcCal(int CamID,USHORT *imgBuf,short *pSPC); 
	virtual int Sensor_GetOB(int CamID);
	virtual int ReadReg(int CamID,USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	virtual int WriteReg(int CamID,USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
public:
	virtual void Ctrl_GetotpMapPath(int CamID,CString &otpMapPath);
	virtual void Ctrl_GetotpByteDataPath(int CamID,CString &otpByteDataPath);
	virtual void Ctrl_GetotpCheckDataPath(int CamID,CString &otpCheckDataPath);
	virtual void Ctrl_GetotpOutBytePath(int CamID,CString &otpOutByteData);
	virtual void Ctrl_GetotpItemdataPath(int CamID,CString &otpItemdata);

	virtual void Ctrl_GetAwbRatio(int CamID,vector<float>& awbRatio);
	 
	BOOL Mes_SetErrorCode( int CamID,CString errorCode);
private: 
	
	BOOL Mes_AddItem(int CamID,CString Name,CString Result);
	BOOL Mes_AddParameter(int CamID,CString Name, CString Value);
	BOOL Mes_SetResult( int CamID,CString Result); 
public:
	void LogMes();
	virtual int UpdateToMes(int CamID);
	virtual	BOOL mesCheck(int CamID); 
	virtual void SetMesInfo(int CamID,BOOL bMes,int MesID);

	virtual int GetSiteNo();
	virtual BOOL mesBanding(int CamID,CString Runcard,CString BarCode);
	virtual int  InitiaSensor(int CamID,CString SettingPath);
	
public:
	int OpenPassProc(int CamID);
	int OpenFailProc(int CamID);
	private:
	int CamBufRelease();

	int AAstate[4];
public:
	void SendMachineTestResult(int CamID,BOOL bResult,int error_type=0);
	virtual void Ctrl_SetMachineItemID(int CamID,vector<int> MachineItemID);
	

//public:
//	void Ctrl_AddBufInfo(int CamID,BufInfo m_bufInfo);
//	void Ctrl_DelBufInfo(int CamID,CString name);

private:
	CRITICAL_SECTION mMes_cs; 
public:
	virtual void Ctrl_SetCamErrorInfo(int CamID,CString errorInfo);
	virtual void Ctrl_SetCamErrorCode(int CamID,int errorCode);
	virtual int  Ctrl_GetCamErrorCode(int CamID);

	virtual void SetGpioPinLevel(int gpioNum,BOOL bH,int CamID);
	virtual void SensorEnable(int pin,BOOL bH,int CamID);

	virtual CString Ctrl_GetCamErrorInfo(int CamID);

	void InitCamDualData(int CamID);

	virtual BOOL  SetLEDChannel(int CamID,int channel,int type,int WaitFlag);
	virtual BOOL UnlocktheFile(int CamID);

	BOOL IncludeChinese(CString str);
	virtual void SetFocusStart(int CamID);

	// ADDED BY WZX
	BOOL bCheckLight;
}; 




