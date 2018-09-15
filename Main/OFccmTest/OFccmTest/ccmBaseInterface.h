#pragma once
#include "OFccmDef.h"
using namespace std;
  

class ccmBaseInterface
{
public:
 
	//IIC 写入函数
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_WriteI2c(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int CamID)=0;

	//IIC 读取函数
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_ReadI2c(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int CamID)=0;
	 

	/************** 文字显示  *******************/
	virtual void Ctrl_Addlog(int CamID,CString info ,COLORREF color,int yHeight) =0;
	virtual void Ctrl_Addlog(int CamID,int info ,COLORREF color,int yHeight) =0;
	virtual void Ctrl_ClearLog(int CamID) = 0;

/****AF 操作*********/
public:
	virtual int VcmDr_InitAF(int CamID) = 0;
	virtual int VcmDr_ReadAF_Code(USHORT* code,int CamID) = 0;
	virtual int VcmDr_WriteAF_Code(USHORT code,int CamID) = 0;
	virtual int VcmDr_AutoFocus(int CamID)=0;

	/***********配置管理器的(ini文件读写) 封装**************/

public:
	//写入指定的分区下某子项的值（STRING）
	virtual void Config_WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string,int CamID) = 0;
	//写入指定的分区下某子项的值(INT)
	virtual void Config_WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC,int CamID=0) = 0;
	//读取指定的分区下某子项的值(INT)
	virtual int Config_ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int  CamID) = 0;
	//读取指定的分区下某子项的值(STRING)
	virtual LPCTSTR Config_ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID) = 0;
	//删除子项(名称和值)
	virtual void Config_ClearConfig(LPCTSTR AppNament ,int CamID) = 0; 
	//删除分区(分区名称和其下所有子项)
	virtual void Config_ClearConfig(LPCTSTR AppName, LPCTSTR KeyName,int CamID) = 0; 
	//得到TCCF文件的路径
	virtual LPCTSTR Config_GetConfigPathName(int CamID) = 0;
	virtual LPCTSTR Config_GetExeDir() = 0;    //EXE

	/************** 得到当前图像/帧信息 *******************/
public:
	 virtual ImageInfo* Ctrl_GetImgInfo(int CamID) =0;
	 virtual int Ctrl_GetOutPutFormat(int CamID) = 0;

//
//	/************** DC与显示  *******************/
//public:
	 //	//DC
	 virtual CDC* Ctrl_GetDisplayWndMemDC(int CamID) = 0;             //得到辅助窗口DC
	 virtual void Ctrl_GetDisplayWndRect(CRect* rect,int CamID) = 0;   //得到主显示窗口的大小
	 virtual int  Ctrl_ShowMemDC(int CamID) =0;     //显示DC
	 virtual int  Ctrl_CopyBackGroundToMemDC(int CamID) = 0;//回复背景色

	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF strColor,CString strLog,UINT nSize = 15,UINT cx = 10,UINT cy = 10)= 0; //写文字

	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF bkColor)= 0; //画背景

	 //画矩形
	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF recColor,CRect drawRect,ImageInfo img)= 0; //坐标以实际pixel计算

	 //画线段
	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,CPoint sPoint,CPoint ePoint,ImageInfo img)= 0; //坐标以实际pixel计算

	 //画椭圆
	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,int cx0,int cy0,int cx1,int cy1,ImageInfo img)= 0; //坐标以实际pixel计算

	 
	//画指定buf 到显示窗口
	virtual int Ctrl_DrawingToMenDC(int CamID,int Width,int Height,BYTE* SrcImg,int ImgType)=0;

 
	virtual HWND Ctrl_GetMainWndHwnd() = 0;//获取主窗口句柄
	virtual  CRect Ctrl_CalcRect_DisplayScreen(double cx, double cy, double x_length, double y_length,int CamID) = 0;
 
	/************** OTP op  *******************/
public:   
	virtual int Otp_OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page = 0)=0;
	virtual int Otp_OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page = 0)=0;

	virtual int Otp_WriteToOtpBoard(int CamID,CString name,vector<BYTE> byteData,BOOL bIn = TRUE) = 0;//otp数据写入到otpboard   
	virtual int Otp_ReadOtpBoard(int CamID,CString name,vector<BYTE> *byteData,BOOL bIn = TRUE) = 0;//获取otpboard数据

	virtual int Otp_WriteToOtpBoard(int CamID,CString name,vector<USHORT> byteData,BOOL bIn = TRUE) = 0;//otp数据写入到otpboard   
	virtual int Otp_ReadOtpBoard(int CamID,CString name,vector<USHORT> *byteData,BOOL bIn = TRUE) = 0;//获取otpboard数据
	  
//
//	/************** 图像采集板 *******************/
//public: 
	virtual int Ctrl_PauseGrabFrame(int CamID) = 0;     //让图像采集板停止抓取帧
	virtual int Ctrl_ResumeGrabFrame(int CamID) = 0;    //恢复抓帧

	 
	virtual int pCam_Opencamera(int CamID)=0;  

	virtual int  InitiaSensor(int CamID,CString SettingPath)=0;

	virtual CString Ctrl_GetCamCode()=0;

	//获取模组fuseid 
	virtual int Ctrl_GetCamType() = 0;

	virtual void Ctrl_SetTestItemID(int CamID,int TestItemID)=0;
	virtual int Ctrl_GetTestItemID(int CamID)=0;

	virtual void Ctrl_SetDualCamData(int CamID,DualCamData m_DualCamData)=0;
	virtual DualCamData* Ctrl_GetDualCamData(int CamID)=0;
	virtual void Ctrl_AutoAlignMoveTo(int CamID,int motor,double pos)=0;
	virtual float Ctrl_AutoAlignGetPos(int CamID,int motor) = 0;

	//获取当前时间
	virtual CString  Ctrl_GetCurrentLocalTime() =0;
 

	virtual void GetDirAllFile(CString dirPath,vector<CString> &fileList) = 0;
	virtual void GetDirAllDirs(CString dirPath,vector<CString> &List) = 0;
	virtual BOOL isFileExist(CString filePath) = 0;//如果存在，返回TRUE;

	//ImgType
	// 1: //保存JPG格式图片
	//2://保存BMP格式图片			 
	//3://保存raw二进制流格式图片
	// 4://保存RAW8的图片		
	// 5:// 保存指定的buf，为jpg格式
	//imgBuf   保存指定的buf	
	virtual void Ctrl_SaveImage(int CamID,BOOL bSetPath = FALSE, CString PathName = NULL,CString FileName = NULL,int ImgType=1,BYTE* imgBuffer = NULL) = 0;

	//获取当前路径
	virtual CString Ctrl_GetCurrentPath() = 0;


	//获取当前显示窗口句柄
	virtual HWND Ctrl_GethDisplayWnd(int CamID) = 0;
	virtual void Ctrl_SetAAResult(BOOL bAaResult) = 0;
	virtual void Ctrl_SetRgain(int CamID,float Rgain ) = 0;
	virtual void Ctrl_SetBgain(int CamID,float Bgain) = 0;
	virtual CPoint Ctrl_GetCurrentRoiPos(int CamID) = 0;
	virtual int VCM_WriteAF_Distance(int Distance, int CamID) = 0;
	virtual BOOL isStandby(int CamID) = 0;
	virtual void Ctrl_SetTestInfo(int CamID,int TestItemID,TestItemInfo mTestItemInfo) = 0;
	virtual  void Ctrl_GetTestInfo(int CamID,int TestItemID,TestItemInfo &mTestItemInfo) = 0;
	virtual void Ctrl_GetCamCurrent(int CamCurrent[],int CamID) = 0;


	//IIC 写入函数
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_WriteI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0)=0;

	//IIC 读取函数
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_ReadI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID= 0,int CamID = 0)=0;

	virtual int Ctrl_bReOpen(int &OpenTimes) = 0; 

	virtual int Sensor_ReadExp(int CamID,int &exp) = 0;
	virtual int Sensor_WriteExp(int CamID,int exp) = 0;

	virtual int Sensor_ReadGain(int CamID,USHORT &gain) = 0;
	virtual int Sensor_WriteGain(int CamID,USHORT gain) = 0; 

	virtual int Sensor_Init(int CamID) = 0;
	virtual CString Sensor_GetCamFuseID(int CamID)=0;	 
	virtual int Sensor_ApplyAWBGain(int CamID,int rg, int bg, int Typical_rg, int Typical_bg) = 0;
	virtual int Sensor_ApplyLsc(int CamID,BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup) = 0;
	virtual int Sensor_GetLsc(int CamID,BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)=0;
	virtual int Sensor_ApplySpc(int CamID,short *pSPC) = 0;
	virtual int Sensor_spcCal(int CamID,USHORT *imgBuf,short *pSPC) = 0;
	virtual int Sensor_GetOB(int CamID) = 0;
	virtual int ReadReg(int CamID,USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0)=0;
	virtual int WriteReg(int CamID,USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0)=0;


	virtual void Ctrl_GetotpMapPath(int CamID,CString &otpMapPath)= 0;
	virtual void Ctrl_GetotpByteDataPath(int CamID,CString &otpByteDataPath)= 0;
	virtual void Ctrl_GetotpCheckDataPath(int CamID,CString &otpCheckDataPath)= 0;
	virtual void Ctrl_GetotpOutBytePath(int CamID,CString &otpOutByteData)= 0;
	virtual void Ctrl_GetotpItemdataPath(int CamID,CString &otpItemdata)= 0; 

	virtual void Ctrl_GetAwbRatio(int CamID,vector<float>& awbRatio) = 0;
	virtual void Ctrl_SetMachineItemID(int CamID,vector<int> MachineItemID) = 0;

	virtual void Ctrl_SetCamErrorInfo(int CamID,CString errorInfo) = 0;

	virtual void Ctrl_SetCamErrorCode(int CamID,int errorCode) = 0;

	virtual int  Ctrl_GetCamErrorCode(int CamID) = 0;

	virtual void SetGpioPinLevel(int gpioNum,BOOL bH,int CamID) = 0;
	virtual void SensorEnable(int pin,BOOL bH,int CamID)=0;

	virtual int UpdateToMes(int CamID) = 0;

	virtual CString Ctrl_GetCamErrorInfo(int CamID) = 0;

	virtual BOOL mesCheck(int CamID) = 0; 

    virtual BOOL mesBanding(int CamID,CString Runcard,CString BarCode)=0;

	virtual void SetMesInfo(int CamID,BOOL bMes,int MesID) = 0;
	


	//
	//	/************** 获取框架基本信息 *******************/
	//public: 
	virtual int  GetSiteNo() = 0;
        virtual BOOL UnlocktheFile(int CamID)=0;
	//
	//	/************** 光源切换 *******************/
	//public:
	virtual BOOL  SetLEDChannel(int CamID,int channel,int type,int WaitFlag)=0;//通道和型号
	virtual void SetFocusStart(int CamID) = 0;
};

