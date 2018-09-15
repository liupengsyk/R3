#include "StdAfx.h"
#include "RX_OTPCheck_TestItem.h"
#include "ImageProc.h"
#include "OtpCheck_API.h"
#include "OtpCheck.h"
#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


#define TESTSTATION _T("OJLJP15")
#define STATIONCHECKITEM _T("OJLOJP15TPCheck")

extern "C" Tccm_EXP RX_OTPCheck_TestItem*  CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new RX_OTPCheck_TestItem(pInterface, lpszName);
}

extern "C"  Tccm_EXP int OtpCheck
(	
	char *pszOutputErrorMsg,
	int iMaxOutputErrorMsgLen,
	BYTE *InputOTPData, 
	int iInputOtpSize, 
	int Flag, 
	char *pszFuseID,
	char *pszStationName
)
{

	OtpCheckAPI stApi;
	if (pszFuseID != NULL)
	{
#define MAX_OTPCHECK_FUSEID_LENGTH 100
#define MAX_OTPCHECK_STATTION_NAME_LENGTH 100
		wchar_t szWideCharFuseID[MAX_OTPCHECK_FUSEID_LENGTH] = {0};
		MultiByteToWideChar(CP_ACP, 0, pszFuseID, strlen(pszFuseID), szWideCharFuseID, MAX_OTPCHECK_FUSEID_LENGTH-1);
		stApi.m_FuseID = szWideCharFuseID;
	}
	else
	{
		stApi.m_FuseID = _T("UNKOWN_FUSEID");
	}
	if(pszStationName)
	{
		wchar_t szWideCharStationName[MAX_OTPCHECK_STATTION_NAME_LENGTH] = {0};
		MultiByteToWideChar(CP_ACP, 0, pszStationName, strlen(pszStationName), szWideCharStationName, MAX_OTPCHECK_STATTION_NAME_LENGTH-1);
		stApi.m_StationName = szWideCharStationName;
	}
	else
	{
		stApi.m_StationName = _T("UNKOWN_STASTION");
	}

	#define MAX_ERRMSG_BUF_LEN 10240

	wchar_t *pErrMsg = new wchar_t[MAX_ERRMSG_BUF_LEN];
	memset(pErrMsg,0, sizeof(wchar_t)*MAX_ERRMSG_BUF_LEN);
	int iRet  = stApi.Testing(pErrMsg, InputOTPData,iInputOtpSize, Flag,stApi.m_StationName);

	CString strErrMsg = pErrMsg;
	delete []pErrMsg;
	pErrMsg = NULL;

	char *pMultiMsg = new char [MAX_ERRMSG_BUF_LEN];
	memset(pMultiMsg, 0, sizeof(char) *MAX_ERRMSG_BUF_LEN);
	WideCharToMultiByte(CP_ACP,0,strErrMsg,strErrMsg.GetLength(),pMultiMsg,MAX_ERRMSG_BUF_LEN,NULL,NULL);//T2A,W2A()
	pMultiMsg[MAX_ERRMSG_BUF_LEN - 1] = '\0';

	int iCopyLen = strlen(pMultiMsg) <=(iMaxOutputErrorMsgLen-1)? strlen(pMultiMsg): (iMaxOutputErrorMsgLen-1);
	strncpy(pszOutputErrorMsg, pMultiMsg, iCopyLen);
	pszOutputErrorMsg[iCopyLen] = '\0';
	
	delete []pMultiMsg;
	pMultiMsg = NULL;
	return iRet;
}

/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
RX_OTPCheck_TestItem::RX_OTPCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	m_cfgName = CString(lpszName);
	SetName(lpszName);	                             //���ò�����Ŀ������
	pDlg = new OptionDlg(NULL,this);                 //���ɶԻ������
	                                   //��ȡ����
	pDlg->Create(OptionDlg::IDD,NULL);               //�����Ի��򴰿�
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //�õ��Ի�����

	//TODO: ���ò�����Ŀ������ 
	SetType(TYPE_IMAGE);

	//TODO: �������Ա�����ʼ��

	//.....
}

RX_OTPCheck_TestItem::~RX_OTPCheck_TestItem(void)
{

}


int RX_OTPCheck_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��

	//***
	SetName(m_cfgName);
	pDlg->m_uErrorCode			= ReadConfigInt(_T("ErrorCode"),0);
	pDlg->m_bModuleSelection	= ReadConfigInt(_T("ModuleSelection"),0);
	pDlg->m_BufferSize			= ReadConfigString(_T("BufferSize"),_T("2000"));
	//***

	SetConfigPath(m_pInterface->Ctrl_GetCurrentPath()+_T("SensorTab\\")+_T("init.ini"));
	SetName(_T("Global"));

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��
	SetName(m_cfgName);
    LoadOption(); 
	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RX_OTPCheck_TestItem::Initialize()
{
	TestItemInterface::Initialize();

	SetbStopTest(FALSE);
	//TODO:�ڴ���ӳ�ʼ������


	OtpCheckResult = 0;
	return 0;
}

/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RX_OTPCheck_TestItem::Testing()
{
	CString temMsg = _T("");

	//���ַ���ת��Ϊ����
	USHORT usTemp;
	swscanf_s(pDlg->m_BufferSize,_T("%hX"),&usTemp);
	m_OTPSize = usTemp;

	if(m_OTPSize > 0x8000)
	{
		OtpCheckResult = 0;
		SetResult(RESULT_FAIL);
		temMsg.Format(_T("��д�Ķ�ȡ���ݳ���0x%04X ��< 0x8000��"), m_OTPSize);
	
		m_pInterface->Ctrl_Addlog(CamID,temMsg,COLOR_RED,200);
		return FALSE;
	}

	USHORT *uReadBuffer = new USHORT[m_OTPSize];
	if (NULL == uReadBuffer)
	{
		SAFE_DELETE_ARRAY(uReadBuffer);
		m_pInterface->Ctrl_Addlog(CamID,_T("����uReadBufferʧ��!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OTP Check Fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	BYTE *tempOTPBuff   = new BYTE[m_OTPSize];
	if (NULL == tempOTPBuff)
	{
		SAFE_DELETE_ARRAY(uReadBuffer);
		SAFE_DELETE_ARRAY(tempOTPBuff);
		m_pInterface->Ctrl_Addlog(CamID,_T("����tempOTPBuffʧ��!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OTP Check Fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

    wchar_t szLog[128] = {0};

	m_pInterface->Otp_OtpRead(0x0000,m_OTPSize-1,uReadBuffer,CamID);

    if(pDlg->SaveEEPROMPDAF_En==1)
    {
        CString EEPROMfilepath=m_pInterface->Ctrl_GetCurrentPath()+"PDAFTool\\readbackvalue.bin";
        BYTE tempotpdata[8192]={0};
        for(int i=0;i<8192;i++)
            tempotpdata[i]=uReadBuffer[i];
        SaveEEPROM(tempotpdata,EEPROMfilepath);
    }
	for (int i=0;i<m_OTPSize;i++)
	{
		tempOTPBuff[i] = uReadBuffer[i]&0xFF; 
	}
    wchar_t ErrorMsg[5120]=_T("");

	OtpCheckAPI stAPI;
	stAPI.m_StationName = pDlg->m_TestStation;
	stAPI.m_FuseID = m_pInterface->Sensor_GetCamFuseID(CamID);

	int retValOtpCheck = 0;
	if (pDlg->m_bModuleSelection == FALSE)	//����
	{
		retValOtpCheck = stAPI.Testing(ErrorMsg, tempOTPBuff, m_OTPSize, 1,stAPI.m_StationName);
	} 
	else   //����
	{
		retValOtpCheck = stAPI.Testing(ErrorMsg, tempOTPBuff, m_OTPSize, 2,stAPI.m_StationName);
	}	
	SAFE_DELETE_ARRAY(uReadBuffer);
	SAFE_DELETE_ARRAY(tempOTPBuff);

	CString myCheckMsg(ErrorMsg);
	int nLocation = myCheckMsg.ReverseFind(_T('\n'));
	CString sErrorInfo;
	sErrorInfo = myCheckMsg.Right(myCheckMsg.GetLength()-nLocation-1);
	if (sErrorInfo.GetLength()<3)
	{
		myCheckMsg = myCheckMsg.Left(nLocation);
		nLocation	= myCheckMsg.ReverseFind(_T('\n'));
		sErrorInfo = myCheckMsg.Right(myCheckMsg.GetLength()-nLocation-1);
	}
	else if (sErrorInfo==_T("   ")||sErrorInfo==_T("			"))
	{
		sErrorInfo= _T("OTPCheckʧ��");
	}
	else
	{
		;
	}

	FILE *file;

	if (0 == CamID)
	{
		file = fopen("OTPCheckMsg0.txt","w");
	} 
	else
	{
		file = fopen("OTPCheckMsg1.txt","w");
	}
	
	if(!file)
	{
		perror("open file failed!");				
	}
	else 
	{
		fwrite(ErrorMsg,1,sizeof(ErrorMsg),file);
		fclose(file);	
	}

	

	if(retValOtpCheck != 1)
	{
		SetResult(RESULT_FAIL);
		temMsg=ErrorMsg;
		m_pInterface->Ctrl_Addlog(CamID,temMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,sErrorInfo,COLOR_RED,220);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,sErrorInfo);
		m_pInterface->Ctrl_SetCamErrorCode(CamID,pDlg->m_uErrorCode);
		OtpCheckResult = 0;
	}
	else
	{
		OtpCheckResult = 1;
		SetResult(RESULT_PASS);
		temMsg =_T("OTP CHECK PASS!");
		m_pInterface->Ctrl_Addlog(CamID,temMsg,COLOR_GREEN,200);
	}

    return 0;
}


bool RX_OTPCheck_TestItem::SaveEEPROM(BYTE *EEPROMData,CString filename)
{
    CString TempMsg;	
    HANDLE hFile;
    if (PathFileExists(filename))
    {
        if (!DeleteFile(filename))
        {
            return FALSE;
        }
    }
    hFile = CreateFile(filename,GENERIC_WRITE, FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
    if( hFile != INVALID_HANDLE_VALUE )
    {
        DWORD nBytesWriten;	 
        WriteFile(hFile, EEPROMData,8192,&nBytesWriten,NULL );
        CloseHandle( hFile );		
        if (nBytesWriten != 8192)
        {		 
            return FALSE;
        }	
    }

    return true;
}
bool RX_OTPCheck_TestItem::GT24C32A_Read(USHORT address,byte *Data,int size)
{	
	int i=0;
	int group=0;
	int num=0;
	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	if(Count>0)
		m_pInterface->Device_ReadI2cBlock(0xA0,address,2,Data,Count,0,0,CamID);


	Sleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->Device_ReadI2cBlock(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32,0,0,CamID);

		Sleep(10);
	}
	if(num>0)
		m_pInterface->Device_ReadI2cBlock(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num,0,0,CamID);

	return TRUE;
}
/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RX_OTPCheck_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������
	m_pInterface->Ctrl_ResumeGrabFrame(CamID);

	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int RX_OTPCheck_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ����
	SetConfigPath(GetModulePath() + "\\SetOTPCheckSpec.ini");
	int nPos = 0;
// 	pDlg->m_OTPStation  = FALSE;
// 	pDlg->m_PDAFStation = FALSE;
// 	pDlg->m_CalStation  = FALSE;
	
// 	pDlg->m_FinalTestStation = FALSE;
// 	pDlg->m_AECStation = FALSE;

	if (pDlg->m_bModuleSelection == FALSE)
	{
		SetName(_T("Project1_OTPCheck"));
		pDlg->m_ProjectName	= ReadConfigString(_T("ProjectName1"), _T("XXXXXX"));	
	}
	else
	{
		SetName(_T("Project2_OTPCheck"));
		pDlg->m_ProjectName	= ReadConfigString(_T("ProjectName2"), _T("XXXXXX"));
	}
	nPos = (pDlg->m_ProjectName).Find('/');
	pDlg->m_ProjectName = (nPos < 0) ? pDlg->m_ProjectName : pDlg->m_ProjectName.Left(nPos-1);
	
    //........................
    SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
    SetName(m_cfgName);
 	pDlg->m_TestStation	= ReadConfigString(_T("TestStation"),_T("XXX"),15);

	nPos = (pDlg->m_TestStation).Find('/');
	pDlg->m_TestStation = (nPos < 0) ? pDlg->m_TestStation : pDlg->m_TestStation.Left(nPos-1);

	m_pInterface->Ctrl_Addlog(CamID,pDlg->m_TestStation,COLOR_RED,200);
    pDlg->SaveEEPROMPDAF_En=ReadConfigInt(_T("SaveEEPROMPDAF_En"), 0);

//	pDlg->m_AFStation   = 5;

	if((pDlg->m_TestStation).Find(_T("AF")) != -1)
	{
		pDlg->m_AFStation = 0;
	}
	else if((pDlg->m_TestStation).Find(_T("OTP")) != -1)
	{
		pDlg->m_AFStation = 1;
	//	pDlg->m_OTPStation = 1;
	}
	else if((pDlg->m_TestStation).Find(_T("PDAF")) != -1)
	{
		pDlg->m_AFStation = 2;
	//	pDlg->m_PDAFStation = TRUE;
	}
	else if ((pDlg->m_TestStation).Find(_T("AEC")) != -1)
	{
		pDlg->m_AFStation = 3;
		//   pDlg->m_AECStation = TRUE;
	}
	
	else if((pDlg->m_TestStation).Find(_T("DualCal")) != -1)
	{
		pDlg->m_AFStation = 4;
  //      pDlg->m_CalStation = TRUE;
	}
	else if((pDlg->m_TestStation).Find(_T("Image")) != -1)
	{
		pDlg->m_AFStation = 5;
    //    pDlg->m_FinalTestStation = TRUE;
	}
	
	else
	{
        pDlg->m_AFStation = 5;   //δ������Ĭ��ΪImageվ
	}

	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int RX_OTPCheck_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

	SetConfigPath(GetModulePath() + "\\SetOTPCheckSpec.ini");
	if (pDlg->m_bModuleSelection == FALSE)
	{
		SetName(_T("Project1_OTPCheck"));
		WriteConfigString(_T("ProjectName1"), pDlg->m_ProjectName);
	}
	else
	{
		SetName(_T("Project2_OTPCheck"));
		WriteConfigString(_T("ProjectName2"), pDlg->m_ProjectName);
	}



	//........................
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	SetName(m_cfgName);

	WriteConfigInt(_T("ErrorCode"),pDlg->m_uErrorCode);
	WriteConfigInt(_T("ModuleSelection"),pDlg->m_bModuleSelection);
	WriteConfigString(_T("BufferSize"),pDlg->m_BufferSize);
    pDlg->m_TestStation = "Image";

	CString tempStr;
	tempStr.Format(_T("m_AFStation = %d"),pDlg->m_AFStation);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_RED,200);

	if (pDlg->m_AFStation == 0)
	{
		pDlg->m_TestStation = "AF		// ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}
	else if (pDlg->m_AFStation == 1)
	{
		pDlg->m_TestStation = "OTP		// ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}
	else if (pDlg->m_AFStation == 2)
	{
		pDlg->m_TestStation = "PDAF	    // ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}
	else if (pDlg->m_AFStation == 3)
	{
		pDlg->m_TestStation = "AEC     // ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}
	else if (pDlg->m_AFStation == 4)
	{
		pDlg->m_TestStation = "DualCal	// ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}
	else if (pDlg->m_AFStation == 5)
	{
		pDlg->m_TestStation = "Image	// ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}
	
	else
	{
		pDlg->m_TestStation = "Image	// ����վλ��Ŀǰ��֧�֣�OTP��AF��PDAF��AEC��DualCal��Image��";
	}

    m_pInterface->Ctrl_Addlog(CamID,pDlg->m_TestStation,COLOR_RED,200);

    WriteConfigString(_T("TestStation"), pDlg->m_TestStation);
    WriteConfigInt(_T("SaveEEPROMPDAF_En"), pDlg->SaveEEPROMPDAF_En);
	//LoadOption();
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RX_OTPCheck_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("OtpCheck Result");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RX_OTPCheck_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
������:    GetReportUpperLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RX_OTPCheck_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
������:    GetReportContents
��������:  �����Ӧ������������Ĳ��Ե�����
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RX_OTPCheck_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%d"),OtpCheckResult);
	return Content;
}

CString RX_OTPCheck_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}

