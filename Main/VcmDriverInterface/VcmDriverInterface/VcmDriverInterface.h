// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VCMDRIVERINTERFACE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VCMDRIVERINTERFACE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once


#ifdef VCMDRIVERINTERFACE_EXPORTS
#define VCMDRIVERINTERFACE_API __declspec(dllexport)
#else
#define VCMDRIVERINTERFACE_API __declspec(dllimport)
#endif


class ccmBaseInterface;

// �����Ǵ� VcmDriverInterface.dll ������
class VCMDRIVERINTERFACE_API CVcmDriverInterface {
public:
	CVcmDriverInterface(void);
	~CVcmDriverInterface(void);
	// TODO: �ڴ�������ķ�����

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
