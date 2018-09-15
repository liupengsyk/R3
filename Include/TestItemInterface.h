#pragma once
#define WM_UPDATE_INSPECITON_OPTION     WM_USER + 0x1000
#define WM_INSPCETIONDLL_KEY_CTRLNUM WM_USER + 0x1001
#define	WM_CAMERA_STOP WM_USER + 0x1002
#include "OFccmDef.h"

class ccmBaseInterface;
   
class TestItemInterface
{
public:
	TestItemInterface(void);
	~TestItemInterface(void);

	virtual int InitItem() = 0;

	virtual int Testing() = 0;
	virtual int Initialize();
	virtual int Finalize();

	virtual int LoadOption();
	virtual int SaveOption();

	virtual LPCTSTR GetReportHeads() ;
	virtual LPCTSTR GetReportHightLimit();
	virtual LPCTSTR GetReportLowLimit();
	virtual LPCTSTR GetReportContents();

public:
	enum TESTTYPE
	{
		TYPE_NULL = 0,
		TYPE_INFINITY, //远景
		TYPE_IMAGE,    //画质
		TYPE_MACRO,    //近景
		TYPE_INIT,
		//其他位置的定义 
		TYPE_FAR1,    
		TYPE_IMAGE_BLACK,   
		TYPE_NEAR1,
	};
	void MIPIP10ToRaw8(USHORT * pIn, BYTE * pOut, long number);
	int  GetTimeTag();//获得自2000年1月1日经过的小时数
	BOOL RegReadKey(wchar_t *SubReg,wchar_t *SubKeyName,wchar_t *keyValue);//读注册表
	BOOL RegSetKey(wchar_t *SubReg,wchar_t *SubKeyName,wchar_t *keyValue);//写注册表
	void SetName(LPCTSTR name);
	LPCTSTR GetName();

	void SetOptionDlgHandle(HWND OptionHwnd);
	HWND GetOptionDialogHandle();

	void SetResult(int Result);	
	int GetResult();

	void SetType(int itype);
	int GetType();

	void SetSave(BOOL bSave);
	BOOL isSaved();
	
	int GetCamID();
	void SetCamID(int CamID);
	 
	 

	BOOL GetbStopTest();
	void SetbStopTest(BOOL bStop);

	void SetErrorCode(CString error_Code);
	LPCTSTR GetErrorCode();
	 
	void SetOtpDataFormat(otpDataFormat m_otpDataFormat);
	void GetOtpDataFormat(otpDataFormat &m_otpDataFormat);

	//适用于n+1 BYTE 模式,转换为otp数据格式  
	//dataInput  长度必须等于 格式 len-1
	void ConvertToOtpData(USHORT* dataInput,vector<BYTE> *otpDataOut);

	void ConvertToItemData(BYTE* dataInput,vector<USHORT> *ItemDataOut);

	void GetOtp_CheckSum(vector<BYTE> otpData,BYTE Sumtype,USHORT &sum);


	enum RESULT
	{
		RESULT_NULL = -1,
		RESULT_PASS =1,
		RESULT_OK2 =2,
		RESULT_FAIL =0,
		RESULT_NORMAL=3,
	};

	enum RETURN_STATE
	{
		RET_CONTINUE =-1,
		RET_END = 0,
	};

	void SetConfigPath(LPCTSTR ConfigPathName);
	int ReadConfigInt(LPCTSTR KeyName,int int_default);
	CString ReadConfigString(LPCTSTR keyName,LPCTSTR strDefault,int length = 512);
	void WriteConfigInt(LPCTSTR KeyName,int int_default,int datatype = DATA_DEC);
	void WriteConfigString(LPCTSTR KeyName,LPCTSTR strDefault);
	void Config_ClearConfig(LPCTSTR AppName);
	

	ccmBaseInterface  *m_pInterface;

private:

	int     m_Result;
	CString m_Name;
	CString m_configPathName;
	CString error_Code;
	int     m_Type;
	HWND    m_hOptionDlg;
	BOOL    m_bSave;   //是否存储的开关
	int CamID; 
	 
	BOOL bStopTest;

	int InitPrivateMen();

private:
	otpDataFormat m_otpDataFormat; 



};

