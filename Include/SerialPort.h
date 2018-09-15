#pragma once


#ifdef CSERIALPORTLIBRARY_EXPORTS
   #define CSerialPort_API  _declspec(dllexport)
#else   
   #define CSerialPort_API    _declspec(dllimport)
#endif

#include <windows.h>

class CSerialPort_API  CSerialPort
{
protected:
private:
	BOOL BaudRateCheck(int ibaudrate);
	BOOL ComPortCheck(int iport);
	BOOL ParityCheck(int iParity);
	BOOL StopBitCheck(int iStopBit);		
public:
	HANDLE		m_hComm;
	OVERLAPPED	m_Read, m_Write;	
	int			m_iBaudRate;
	int			m_iParity;
	int			m_iStopBit;
	char		m_strCOM[20];
	CSerialPort();
	~CSerialPort();
	int    ComOpen(int icomport=1, int ibaudrate=CBR_115200, int iparity=NOPARITY, int istopbit=ONESTOPBIT);
	int    ComClose();	
	DWORD  ReadData(HANDLE hcomm, char *pBuff, DWORD nToRead, OVERLAPPED *p_read);
	DWORD  SendData(HANDLE hcomm, char *pBuff, DWORD size, OVERLAPPED *p_write);	
};

