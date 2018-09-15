#pragma once
#include "OFccmDef.h"
using namespace std;
  

class ccmBaseInterface
{
public:
 
	//IIC д�뺯��
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_WriteI2c(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int CamID)=0;

	//IIC ��ȡ����
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_ReadI2c(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int CamID)=0;
	 

	/************** ������ʾ  *******************/
	virtual void Ctrl_Addlog(int CamID,CString info ,COLORREF color,int yHeight) =0;
	virtual void Ctrl_Addlog(int CamID,int info ,COLORREF color,int yHeight) =0;
	virtual void Ctrl_ClearLog(int CamID) = 0;

/****AF ����*********/
public:
	virtual int VcmDr_InitAF(int CamID) = 0;
	virtual int VcmDr_ReadAF_Code(USHORT* code,int CamID) = 0;
	virtual int VcmDr_WriteAF_Code(USHORT code,int CamID) = 0;
	virtual int VcmDr_AutoFocus(int CamID)=0;

	/***********���ù�������(ini�ļ���д) ��װ**************/

public:
	//д��ָ���ķ�����ĳ�����ֵ��STRING��
	virtual void Config_WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string,int CamID) = 0;
	//д��ָ���ķ�����ĳ�����ֵ(INT)
	virtual void Config_WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC,int CamID=0) = 0;
	//��ȡָ���ķ�����ĳ�����ֵ(INT)
	virtual int Config_ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int  CamID) = 0;
	//��ȡָ���ķ�����ĳ�����ֵ(STRING)
	virtual LPCTSTR Config_ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID) = 0;
	//ɾ������(���ƺ�ֵ)
	virtual void Config_ClearConfig(LPCTSTR AppNament ,int CamID) = 0; 
	//ɾ������(�������ƺ�������������)
	virtual void Config_ClearConfig(LPCTSTR AppName, LPCTSTR KeyName,int CamID) = 0; 
	//�õ�TCCF�ļ���·��
	virtual LPCTSTR Config_GetConfigPathName(int CamID) = 0;
	virtual LPCTSTR Config_GetExeDir() = 0;    //EXE

	/************** �õ���ǰͼ��/֡��Ϣ *******************/
public:
	 virtual ImageInfo* Ctrl_GetImgInfo(int CamID) =0;
	 virtual int Ctrl_GetOutPutFormat(int CamID) = 0;

//
//	/************** DC����ʾ  *******************/
//public:
	 //	//DC
	 virtual CDC* Ctrl_GetDisplayWndMemDC(int CamID) = 0;             //�õ���������DC
	 virtual void Ctrl_GetDisplayWndRect(CRect* rect,int CamID) = 0;   //�õ�����ʾ���ڵĴ�С
	 virtual int  Ctrl_ShowMemDC(int CamID) =0;     //��ʾDC
	 virtual int  Ctrl_CopyBackGroundToMemDC(int CamID) = 0;//�ظ�����ɫ

	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF strColor,CString strLog,UINT nSize = 15,UINT cx = 10,UINT cy = 10)= 0; //д����

	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF bkColor)= 0; //������

	 //������
	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF recColor,CRect drawRect,ImageInfo img)= 0; //������ʵ��pixel����

	 //���߶�
	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,CPoint sPoint,CPoint ePoint,ImageInfo img)= 0; //������ʵ��pixel����

	 //����Բ
	 virtual int Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,int cx0,int cy0,int cx1,int cy1,ImageInfo img)= 0; //������ʵ��pixel����

	 
	//��ָ��buf ����ʾ����
	virtual int Ctrl_DrawingToMenDC(int CamID,int Width,int Height,BYTE* SrcImg,int ImgType)=0;

 
	virtual HWND Ctrl_GetMainWndHwnd() = 0;//��ȡ�����ھ��
	virtual  CRect Ctrl_CalcRect_DisplayScreen(double cx, double cy, double x_length, double y_length,int CamID) = 0;
 
	/************** OTP op  *******************/
public:   
	virtual int Otp_OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page = 0)=0;
	virtual int Otp_OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page = 0)=0;

	virtual int Otp_WriteToOtpBoard(int CamID,CString name,vector<BYTE> byteData,BOOL bIn = TRUE) = 0;//otp����д�뵽otpboard   
	virtual int Otp_ReadOtpBoard(int CamID,CString name,vector<BYTE> *byteData,BOOL bIn = TRUE) = 0;//��ȡotpboard����

	virtual int Otp_WriteToOtpBoard(int CamID,CString name,vector<USHORT> byteData,BOOL bIn = TRUE) = 0;//otp����д�뵽otpboard   
	virtual int Otp_ReadOtpBoard(int CamID,CString name,vector<USHORT> *byteData,BOOL bIn = TRUE) = 0;//��ȡotpboard����
	  
//
//	/************** ͼ��ɼ��� *******************/
//public: 
	virtual int Ctrl_PauseGrabFrame(int CamID) = 0;     //��ͼ��ɼ���ֹͣץȡ֡
	virtual int Ctrl_ResumeGrabFrame(int CamID) = 0;    //�ָ�ץ֡

	 
	virtual int pCam_Opencamera(int CamID)=0;  

	virtual int  InitiaSensor(int CamID,CString SettingPath)=0;

	virtual CString Ctrl_GetCamCode()=0;

	//��ȡģ��fuseid 
	virtual int Ctrl_GetCamType() = 0;

	virtual void Ctrl_SetTestItemID(int CamID,int TestItemID)=0;
	virtual int Ctrl_GetTestItemID(int CamID)=0;

	virtual void Ctrl_SetDualCamData(int CamID,DualCamData m_DualCamData)=0;
	virtual DualCamData* Ctrl_GetDualCamData(int CamID)=0;
	virtual void Ctrl_AutoAlignMoveTo(int CamID,int motor,double pos)=0;
	virtual float Ctrl_AutoAlignGetPos(int CamID,int motor) = 0;

	//��ȡ��ǰʱ��
	virtual CString  Ctrl_GetCurrentLocalTime() =0;
 

	virtual void GetDirAllFile(CString dirPath,vector<CString> &fileList) = 0;
	virtual void GetDirAllDirs(CString dirPath,vector<CString> &List) = 0;
	virtual BOOL isFileExist(CString filePath) = 0;//������ڣ�����TRUE;

	//ImgType
	// 1: //����JPG��ʽͼƬ
	//2://����BMP��ʽͼƬ			 
	//3://����raw����������ʽͼƬ
	// 4://����RAW8��ͼƬ		
	// 5:// ����ָ����buf��Ϊjpg��ʽ
	//imgBuf   ����ָ����buf	
	virtual void Ctrl_SaveImage(int CamID,BOOL bSetPath = FALSE, CString PathName = NULL,CString FileName = NULL,int ImgType=1,BYTE* imgBuffer = NULL) = 0;

	//��ȡ��ǰ·��
	virtual CString Ctrl_GetCurrentPath() = 0;


	//��ȡ��ǰ��ʾ���ھ��
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


	//IIC д�뺯��
	//UCHAR uAddr: iic slave ID;
	//USHORT uReg :reg addr
	//USHORT uValue: data
	////I2C Mode    :0:Normal 8Addr,8Data;  1:Samsung 8 Addr,8Data; 
    //I2C Mode    :2:Micron 8 Addr,16Data
    //I2C Mode    :3:Stmicro 16Addr,8Data;4:Micron2 16 Addr,16Data
	virtual int Device_WriteI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0)=0;

	//IIC ��ȡ����
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
	//	/************** ��ȡ��ܻ�����Ϣ *******************/
	//public: 
	virtual int  GetSiteNo() = 0;
        virtual BOOL UnlocktheFile(int CamID)=0;
	//
	//	/************** ��Դ�л� *******************/
	//public:
	virtual BOOL  SetLEDChannel(int CamID,int channel,int type,int WaitFlag)=0;//ͨ�����ͺ�
	virtual void SetFocusStart(int CamID) = 0;
};

