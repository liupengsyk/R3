
#pragma once
#include "stdafx.h"
#include <vector> 
//#include "imagekit.h"
using namespace std;

#define WM_SOCKET_CLOSE	WM_USER+1000
#define WM_SOCKET_UPDATE	WM_USER+1001
#define WM_SOCKET_OPENCAMERA WM_USER+1002
#define WM_SOCKET_CLOSECAMERA WM_USER+1003
#define WM_SOCKET_STARTTEST	WM_USER+1004
#define WM_SOCKET_STARTCHECK WM_USER+1005
#define WM_SOCKET_CLOSECAMERAFINAL WM_USER+1006
#define WM_SOCKET_TESTPASS WM_USER+2000
#define WM_SOCKET_TESTFAIL WM_USER+2001
#define WM_SOCKET_SHOW_RESULT  WM_USER+2002

#define WM_SOCKET_CCT_A_OK  WM_USER+2003
#define WM_SOCKET_CCT_TL84_OK  WM_USER+2004
#define WM_SOCKET_CCT_D65_OK  WM_USER+2005
#define WM_SOCKET_CCT_D50_OK  WM_USER+2006
#define WM_SOCKET_BINDING  WM_USER+2007

#define WM_UPDATEMSG WM_USER+100
#define WM_ADDLOG WM_USER+101
#define WM_CLEARLOG WM_USER+102

#define WM_UPDATEUI  WM_USER+103

#define WM_INSPECTIONBUTTON_SHOWSTATUS  WM_USER+104
#define WM_INSPECTIONBUTTON_ALIGNWNDPOS WM_USER+105
 
#define WM_SHOWTEXTINFO       WM_USER + 106
#define WM_CREATESETTINGDLG       WM_USER + 107
#define WM_CREATEMACHINESETTINGDLG WM_USER + 108

#define WM_AUTOCALIBRATION WM_USER + 109
#define WM_AUTOGETAVE WM_USER + 110

#define WM_DESTROYSETTINGDLG       WM_USER + 111
enum AUTOLIGHTSTATUS    //���ļ���Ӧ״̬һ�£������Զ�У��
{
	AUTOLIGHT_INIT = 0,  //��ʼֵ,0
	AUTOLIGHT_GOLDEN_SUCESS,     //��ȡGOLDENֵ�ɹ� T0100
	AUTOLIGHT_GOLDEN_FAIL_CAPTURE,    //��ȡɫ����ʧ�� T0101
	AUTOLIGHT_GOLDEN_FAIL,    //��ȡGOLDENֵʧ�� T0102

	AUTOLIGHT_SAVECOEFFEE_SUCESS,    //����ɹ�  T0200
	AUTOLIGHT_SAVECOEFFEE_FAIL_AVAGEGODEN,    //��ȡGoldenƽ��ֵʧ�� T0201
	AUTOLIGHT_SAVECOEFFEE_FAIL,    //����ʧ�� T0202
};

//�عⷵ��ֵ
#define EXPOSURE_FRAME_PITCHING             1
#define EXPOSURE_ERROR_OVERFLOW            -1
#define EXPOSURE_ERROR_ZERO                -2
#define EXPOSURE_RUNNING                    2
#define EXPOSURE_OK                         0
#define EXP_Y 0  //Yͨ���ع�
#define EXP_G 1  //Gͨ���ع�


#define COLOR_RED			(RGB(255,0,0))
#define COLOR_GREEN			(RGB(0,255,0))
#define	COLOR_BLUE			(RGB(0,0,255))
#define COLOR_YELLOW		(RGB(255,255,0))
#define COLOR_MAGENTA		(RGB(255,0,255))
#define COLOR_CYAN			(RGB(0,255,255))
#define COLOR_GREEN_WHITE	(RGB(100,240,100))
#define COLOR_WHITE			(RGB(255,255,255))
#define COLOR_BLACK			(RGB(0, 0, 0))
#define COLOR_PINK			(RGB(255,50,255))
#define COLOR_SKY			(RGB(66,162,255))
#define COLOR_SYS			(RGB(236,233,215))
#define COLOR_LIMIT			(RGB(0,128,128))
#define COLOR_BROWN			(RGB(128, 0, 0))
#define COLOR_ORANGE		(RGB(255,128,64))
#define COLOR_GREENRED		(RGB(128,128,0))
#define COLOR_GREENBLUE		(RGB(0,128,128))
#define COLOR_DARKGRAY		(RGB(192,192,192))


#define DATA_DEC        0 //ʮ����
#define	DATA_HEX_WORD   1 //ʮ������
#define	DATA_HEX_BYTE   2 //0xAA
#define	DATA_HEX_DWORD  3 //0xAAAA
#define	DATA_STRING     4

//#define WORKSTATION_NULL   0 
//#define WORKSTATION_TJ     1 
//#define WORKSTATION_OTP    2
//#define WORKSTATION_JC     3 
//#define WORKSTATION_EX1    4 
//#define WORKSTATION_EX2    5 

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{	if(p) { delete (p);						(p)=NULL; }	}
#define SAFE_DELETE_ARRAY(p)	{	if(p) { delete [] (p);					(p)=NULL; }	}
#define SAFE_CLOSE_HANDLE(p)	{	if(p) { ::CloseHandle(p);				(p)=NULL; }	}
#define SAFE_DELETE_OBJECT(p)	{	if(p) { ::DeleteObject((HGDIOBJ) p);	(p)=NULL; }	}
#endif	/* SAFE_DELETE */

//enum WindowMode
//{
//	SetWindowInit=0,
//	SetWindowStop,
//	SetWindowPlay,
//	SetWindowStandby,
//	SetWindowResume,
//	SetWindowOffLine,
//};
 
//UI 
#define  UI_DLGOPEN              0   //��������
#define  UI_LOADCONFIG_SUCESS    1   //���ز����ɹ�
#define  UI_PLAY_SUCESS          2   //���ųɹ�
#define  UI_PLAY_FAILED         -2   //����ʧ��
#define  UI_STOP_SUCESS          3   //ֹͣ�ɹ�
#define  UI_AUTOTEST_START       4   //�Զ����Կ�ʼ
#define  UI_MANUALTEST_START     5   //�Զ����Կ�ʼ
#define  UI_ENTER_OFFLINE        6   //
#define  UI_EXIT_OFFLINE         7
#define  UI_START_STOP           8   //����ֹͣ��
 
//status 
#define STATUS_OFFLINE  1
#define STATUS_RUNNING  2
#define STATUS_STANDBY  3
#define STATUS_OPENNING 4
#define STATUS_PAUSE    5 


struct ImageInfo
{
	int imgwidth;
	int imgheight;	
	BYTE* RGB24_buffer; 
	USHORT* Raw_buffer; 
	BYTE RawFormat;
	BYTE  RawBits;
};

struct logTab
{ 
	CString logstr;	
	COLORREF color;
	int height;
	logTab()
	{
		logstr = ("");	
		color = RGB(1,1,1);
		height = 15;
	}
};

struct OtpSensorTab
{
	CString SensorName;
	int SlaveID;
};
 
#define SET_BIT(x,y) x|=(1<<y)  //��X�ĵ�Y��λ����1
#define CLR_BIT(x,y) x&= ~(1<<y) //��X�ĵ�Y��λ����0

#define DTPIN_MAX   45
struct OSTab
{	
	BOOL    OS_en;//����
	BOOL    PositiveOS_en;//����
	BOOL    SensorpinOS_en;//����pin����
	DWORD   PinMask_L;
	DWORD   PinMask_H;
	struct OSPin
	{
		double  OpenLimit;
		double  ShortLimit;
		CString Name;
	}Pin[DTPIN_MAX];
};
 

#define DTVOL_MAX   5
struct CurrentTab
{	
	BOOL    Test_en;
	DWORD   CurrentMask_L;
	DWORD   CurrentMask_H;
	struct OSPin
	{
		double  CurrentUpLimit;
		double  CurrentLowLimit;
		CString Name;
	}Pin[DTVOL_MAX];
	double dbStandByPowerMax;
	double dbStandByPowerMin;
};

 
struct ErrorCodeTab
{	
	BOOL    ErrorCode_en;
	CString strSocket[15];
	CString strTestID[15];
};

struct GeneralSettingTab
{
  	BOOL SaveReport_EnLocal; //�洢���Լ�¼

	BOOL bSaveDebugExp;
	BOOL bUseLocalExp;	 
	BOOL bForceShow;
	//BOOL bUseMes;
	//int MesID;
	CString DevName;

	//ģ���ŵķ�ʽ
	enum
	{
		IDMODE_RANDOM,
		IDMODE_MANUAL_INPUT,
		IDMODE_READ_FROM_OTP_EEPROM,
		IDMODE_AUTO_INPUT,
	};
	 
	BOOL ForcedShow_En;
 
	int m_TestMode;

	CString sReportPath;
	 
	CString RemoteIP;
	int iPort;
	BOOL SocketEn;
};

struct Cam_Buf
{
	//ͼ��bufferָ��
	BYTE*   grab_buffer;     //ԭʼ����ָ��
	BYTE*   display_buffer;   //�������ԭʼ����ָ��	 
	BYTE*   transfer_buffer; //���������е���תbuffer;

	BYTE*   RGB24_buffer;    //RGB24����ָ�� 
	USHORT* raw_buffer;      //RAW
	 
};

struct Cam_State
{
	BOOL bCameraStandby;
	BOOL bCameraStarted;
	BOOL bCameraOffLine;	 
	BOOL bAEmutex;
	BOOL bLoopFlag;
	BOOL bOffline_LoopFlag; //�����߳�ѭ����־
	BOOL bCameraRunSate;//0:��ʼ״̬ 1���ξ��ж�״̬ 2����������״̬
	BOOL bDisplayThreadRunSate;//-1:��ʼ״̬,δ�����߳� ��0�������̣߳�1:�̷߳���״̬
	BOOL bGrabThreadRunSate; //0��ʼ״̬ 1:�̷߳���״̬��0�������̣߳�1:�̷߳���״̬
};

struct Cam_FrameInfo
{
	UINT FrameCnt ;
	UINT PlayCnt ; 
	ULONG m_GrabSize;
	CString	m_strFrameInfo;//frame

};

struct Cam_ThreadCtrl
{
	HANDLE    m_hCameraThreadEventDead; //thread for video 
	HANDLE    m_hDisplayThreadEventDead;//thread for display
	HANDLE    m_hGrabPause;            //��ͣ�߳�	
	HANDLE    m_hDisPlayPause;           //��ͣ�߳�
	HANDLE    m_hOfflineThreadDead;
};


enum RUNMODE
{
	RUNMODE_PLAY = 0,
	RUNMODE_PAUSE,
	RUNMODE_STOP,
	RUNMODE_TVLine,
};

enum WindowUpdate
{
	WindowStop = 0,	
	WindowPlay ,
	WindowOffLine, 
};

enum OP_CAMERA
{
	PLAY_CAMERA = 0xFF00,
	STOP_CAMERA,
	PAUSE_CAMERA,
	RESUME_CAMERA,
	TM_SINGLETEST,
}; 

enum COLOR_GAIN
{
	COLOR_GAIN_R,
	COLOR_GAIN_B,
	COLOR_GAIN_GR,
	COLOR_GAIN_GB,
};

struct fCPoint
{
	float x;
	float y;
	int PointSize;
	fCPoint()
	{
		x=0;
		y=0;
		PointSize = 0;
	};

};

struct DualCamData
{
	BOOL bResult;
	float Rotation;
	float TiltX;
	float TiltY;
	float dif_Rotation;
	float dif_TiltX;
	float dif_TiltY;
	float shift_x;
	float shift_y;
	float shift_z;
	float dif_shift_x;
	float dif_shift_y;
	float dif_shift_z;
	float BaseLine;

	vector<fCPoint> PointPos;
	CPoint OpticalCenter;
};

struct TestItemData
{
	CString paraName;
	CString paraValue;
};

struct TestItemInfo
{
	CString strName;
	CString baseDllName;
	BOOL bResult;
	BOOL bCompelete;
	int Test_ID;
	vector<TestItemData>  vec_Data;
};

enum TEST_MODE
{
	TESTMODE_MANUAL = 0,
	TESTMODE_AUTO,
	TESTMODE_MACHINETEST,
	TESTMODE_AATEST,
};
 
struct otpDataFormat
{ 
	int  len; //Ŀ�� byte  ���ȡ�
	BYTE bits;//BYTE bits �������ݵ���Чλ
	BOOL bHigh8;//ȡ�߰�λ TRUE,�Ͱ�λFALSE
	BOOL bHightFirst;//��λ��ǰTRUE,��λ��ǰ FLASE, 
	BOOL unit_byteH_L;//BOOL unit_byteH_L ,�����ֽڰ��Ӹߵ����ŷ� TRUE��(8bit�дӵ�λ��ʼ)��
	CString page;
	USHORT upage;
};
 
struct otpReg
{ 
	CString sReg;
	CString eReg;   
	
	USHORT usReg;
	USHORT ueReg;  

	BOOL bUsed;	
	BOOL bUShort;
	int ByteLen;
	int IndexID;
};

struct otpFlagReg
{
	otpReg flagReg;
	CString FlagValue; 
	USHORT uValue;
	BOOL bUSHORT;	
	int ByteLen;
	int IndexID;
};

struct otpCheckSumReg
{
	otpReg sumRangeReg;
	otpReg sumReg;
	int Sumtype; 
	BOOL bUSHORT;	 
	int ByteLen;
	int IndexID;
};

struct BlockOtpReg
{
	CString Name;
	int ItemLen;
	otpDataFormat otpFormat;
	otpReg m_otpReg;
	otpFlagReg m_otpFlagReg;
	otpFlagReg m_otpVerReg;
	otpCheckSumReg m_otpCheckSumReg;

};
  
struct OtpConver
{ 
	CString Name;
	BlockOtpReg mapOtpReg; 
	int byteLen;
	int blockLen;
	vector<USHORT> otpItemData_In;
	CString otpItemData_In_name;
	vector<BYTE> otpInputByte; 
	vector<BYTE> otpOutByte; 
	vector<USHORT> otpItemData_Out;  
};

struct BufInfo
{
	CString name;
	int BufSize;
	int Num;
};

typedef struct _OIS_SR_Result
{
	double Static_V_LeftDist;
	double Static_V_RightDist;
	double Static_H_UpDist;
	double Static_H_BtDist;

	double On_V_LeftDist;
	double On_V_RightDist;
	double On_H_UpDist;
	double On_H_BtDist;

	double Off_V_LeftDist;
	double Off_V_RightDist;
	double Off_H_UpDist;
	double Off_H_BtDist;

	double SR_V_Left;
	double SR_V_Right;
	double SR_H_Up;
	double SR_H_Bt;

}OIS_SR_Result;

typedef union
{
	BYTE cDATA[29];
	struct
	{
		BYTE	cHallBiasXH;
		BYTE	cHallBiasXL;
		BYTE	cHallBiasYH;
		BYTE	cHallBiasYL;
		BYTE	cHallOffsetXH;
		BYTE	cHallOffsetXL;
		BYTE	cHallOffsetYH;
		BYTE	cHallOffsetYL;
		BYTE	cLoopGainXH;
		BYTE	cLoopGainXL;
		BYTE	cLoopGainYH;
		BYTE	cLoopGainYL;
		BYTE	cGyroOffsetXH;
		BYTE	cGyroOffsetXL;
		BYTE	cGyroOffsetYH;
		BYTE	cGyroOffsetYL;
		BYTE	cOSCValue;
		BYTE	cHallADOffsetXH;
		BYTE	cHallADOffsetXL;
		BYTE	cHallADOffsetYH;
		BYTE	cHallADOffsetYL;
		BYTE	cGyroGainX1;
		BYTE	cGyroGainX2;
		BYTE	cGyroGainX3;
		BYTE	cGyroGainX4;
		BYTE	cGyroGainY1;
		BYTE	cGyroGainY2;
		BYTE	cGyroGainY3;
		BYTE	cGyroGainY4;
	};
}
OfONOISData;

typedef union
{
	BYTE cDATA[33];
	struct
	{
		BYTE	cHallOffsetXL;
		BYTE	cHallOffsetXH;
		BYTE	cHallOffsetYL;
		BYTE	cHallOffsetYH;
		BYTE	cHallBiasXL;
		BYTE	cHallBiasXH;
		BYTE	cHallBiasYL;
		BYTE	cHallBiasYH;
		BYTE	cHallADOffsetXL;
		BYTE	cHallADOffsetXH;
		BYTE	cHallADOffsetYL;	
		BYTE	cHallADOffsetYH;
		BYTE	cLoopGainXL;
		BYTE	cLoopGainXH;
		BYTE	cLoopGainYL;
		BYTE	cLoopGainYH;
		BYTE	cOpticaloffsetXL;
		BYTE	cOpticaloffsetXH;
		BYTE	cOpticaloffsetYL;
		BYTE	cOpticaloffsetYH;
		BYTE	cGyroOffsetXL;
		BYTE	cGyroOffsetXH;
		BYTE	cGyroOffsetYL;
		BYTE	cGyroOffsetYH;
		BYTE	cOSCValue;
		BYTE	cGyroGainX4;
		BYTE	cGyroGainX3;
		BYTE	cGyroGainX2;
		BYTE	cGyroGainX1;	
		BYTE	cGyroGainY4;
		BYTE	cGyroGainY3;
		BYTE	cGyroGainY2;
		BYTE	cGyroGainY1;
	};
}
OfONOISData_GT;