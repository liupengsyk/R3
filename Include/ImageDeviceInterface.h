#pragma once

#include "OFccmDef.h"

class ccmBaseInterface;

class ImageDeviceInterface
{
public:
	ImageDeviceInterface(void);
	~ImageDeviceInterface(void);

	virtual int ImageDeviceOpen() = 0;
	virtual int ImageDeviceClose() = 0;
	virtual int ImageDeviceGrabImageBuf() = 0;

	virtual int ImageDeviceGetVoltage();
	virtual int ImageDeviceGetCurrent();

	virtual int ImageDeviceOpenShortTest();
	virtual int ImageDeviceOTP_Check();


	/***********Interface from  base*********/ 
public: 
	void AddLog(int CamID,CString Info,COLORREF CurrentColor,int InfoSize = 200);
	CString ReadConfigString(LPCTSTR keyName,LPCTSTR strDefault); 
	int ReadConfigInt(LPCTSTR KeyName,int int_default); 
	void WriteConfigString(LPCTSTR KeyName,LPCTSTR strDefault);
	void WriteConfigInt(LPCTSTR KeyName,int data,int datatype);
	HWND GetCamDisplayHwnd(int CamID);

private:
	int     m_Result;
	CString m_Name;
	CString m_configPathName; 
	int CamID;


public:
	void SetResult(int Result);	
	int GetResult();

	void SetName(LPCTSTR name);
	LPCTSTR GetName();

	void SetConfigPath(LPCTSTR ConfigPathName); 

	int GetCamID();
	void SetCamID(int CamID);

	enum RESULT
	{
		RESULT_NULL = -1,
		RESULT_PASS =1, 
		RESULT_FAIL =0,
	};
};

