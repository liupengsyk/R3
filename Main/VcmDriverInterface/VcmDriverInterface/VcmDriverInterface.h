// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VCMDRIVERINTERFACE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VCMDRIVERINTERFACE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once


#ifdef VCMDRIVERINTERFACE_EXPORTS
#define VCMDRIVERINTERFACE_API __declspec(dllexport)
#else
#define VCMDRIVERINTERFACE_API __declspec(dllimport)
#endif


class ccmBaseInterface;

// 此类是从 VcmDriverInterface.dll 导出的
class VCMDRIVERINTERFACE_API CVcmDriverInterface {
public:
	CVcmDriverInterface(void);
	~CVcmDriverInterface(void);
	// TODO: 在此添加您的方法。

protected:
	ccmBaseInterface* pCcmCtr;
public:
	virtual int Init(); 

	virtual int ReadCode(USHORT *code) =0;
	virtual int WriteCode(USHORT code) =0;
	virtual int AutoFocus() ;
	virtual int Standby();
	 
	int I2cRead(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode);
	int I2cWrite(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode);

	int I2cReadBlock(UCHAR uAddr, USHORT uReg,BYTE regSize, BYTE *pValue,USHORT uSize);
	int I2cWriteBlock(UCHAR uAddr, USHORT uReg,BYTE regSize, BYTE *pValue,USHORT uSize);
	 
	int Config_ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int CamID);
	CString Config_ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID);
	void Config_WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int  datatype,int CamID);
	void Config_WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID);

	void Addlog(CString strInfo,COLORREF color,int Size);
	
	int SlaveID;
	int CamID;
	void SetInterface(ccmBaseInterface* m_pInterface,int SlaveID,int CamID);

};

extern VCMDRIVERINTERFACE_API int nVcmDriverInterface;

VCMDRIVERINTERFACE_API int fnVcmDriverInterface(void);
VCMDRIVERINTERFACE_API CVcmDriverInterface* GetVcmDriver(ccmBaseInterface* pCcmCtr,CString VcmDriveName,int SlaveID,int CamID);
VCMDRIVERINTERFACE_API CString GetVcmDriverList();
