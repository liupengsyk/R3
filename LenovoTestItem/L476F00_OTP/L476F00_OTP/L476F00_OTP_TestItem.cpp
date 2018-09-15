#include "StdAfx.h"
#include "L476F00_OTP_TestItem.h"
#include "ImageProc.h"

#include "DTCCM2_SDK/dtccm2.h"
#include "DTCCM2_SDK/imagekit.h"
#pragma  comment(lib,"DTCCM2_SDK\\dtccm2.lib")

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new L476F00_OTP_TestItem(pInterface, lpszName);
}

#define OTP_CHK_MODULE_SERIAL_NUMBER								0x001A
#define OTP_CHK_MODULE_ID_REVERSED_ADDR_END							0x002A

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
L476F00_OTP_TestItem::L476F00_OTP_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化


	//.....
}

L476F00_OTP_TestItem::~L476F00_OTP_TestItem(void)
{

}


int L476F00_OTP_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int L476F00_OTP_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
 
	return 0;
}

unsigned char L476F00_OTP_TestItem::GetBCD(unsigned char ascii)
{
	unsigned char Tmp;

	Tmp = 0;
	if ((ascii >= '0') && (ascii <= '9'))
	{
		Tmp = ascii - '0';
	}
	else if ((ascii >= 'a') && (ascii <= 'f'))
	{
		Tmp = ascii - 'a' + 10;
	}
	else if ((ascii >= 'A') && (ascii <= 'F'))
	{
		Tmp = ascii - 'A' + 10;
	}
	else
	{
		CString str;
		str.Format(_T("Error_OTP_TestItem::GetBCD()---ascii:%0x%X"), ascii);
		m_pInterface->Ctrl_Addlog(CamID, str, COLOR_RED, 200);
	}

	return Tmp;
}

/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int L476F00_OTP_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	m_pInterface->Ctrl_PauseGrabFrame(CamID);
	Sleep(300);

	USHORT buf[2] = {0x00, 0x10};
	USHORT pCheckData[2];
	int ret = 0;
	if(TRUE != m_pInterface->Otp_OtpRead(E2STARTADDR, E2STARTADDR, pCheckData, CamID)) //
	{
		if(TRUE != m_pInterface->Otp_OtpRead(E2STARTADDR, E2STARTADDR, pCheckData, CamID))
		{
			ret = 1;
		}
	}

	if (1 == ret)
	{
		if (m_pInterface->Device_WriteI2c(0xa0, 0x8000, buf[0],3,  CamID)!=1)
		{
			if (m_pInterface->Device_WriteI2c(0xa0, 0x8000, buf[0],3,  CamID)!=1)
			{
				if (m_pInterface->Device_WriteI2c(0xa0, 0x8000, buf[0],3,  CamID)!=1)
				{
					m_pInterface->Ctrl_Addlog(CamID, _T("去除写保护 失败1！"), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_ResumeGrabFrame(CamID);
					Sleep(300);
					return 0;
				}
			}
		}

		Sleep(100);
		if (m_pInterface->Device_WriteI2c(0xa0, 0x8000, buf[1],3, CamID)!=1)
		{
			Sleep(500);
			if (m_pInterface->Device_WriteI2c(0xa0, 0x8000, buf[1],3, CamID)!=1)
			{
				Sleep(100);
				if (m_pInterface->Device_WriteI2c(0xa0, 0x8000, buf[1],3, CamID)!=1)
				{
					m_pInterface->Ctrl_Addlog(CamID, _T("更改寄存器 失败！"), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_ResumeGrabFrame(CamID);
					Sleep(300);
					return 0;
				}
			}
		}
	}

	int i;
	int checksum;
	CString str;
	CTime t=CTime::GetCurrentTime();
	USHORT BasicData[OTP_MODULE_LEN] = {0};
	//USHORT AFData[26]={0};
	USHORT SFRData[455]={0};
	USHORT WBData[OTP_AWB_LEN] = {0};
	USHORT OCData[OTP_OC_LEN] = {0};
	USHORT LSCData[OTP_LSC_LEN] = {0};
	USHORT CTalkData[OTP_CTalk_LEN] = {0};
	//USHORT GainmapData[895]={0};

	/*for (i = 0; i < 8; i++)
	{
		USHORT dddd[1024] = {0xFFFF};

		int j = 0;
		for (j = 0; j < sizeof(dddd) / sizeof(dddd[0]); j++)
		{
			dddd[j] = 0xFFFF;
		}

		if (!m_pInterface->Otp_OtpWrite(i * 1024, (i + 1) * 1024 - 1, dddd, CamID))
		{m_pInterface->Ctrl_Addlog(CamID,_T("DDDDDDDDDDDDD"),COLOR_RED,200);
			if (!m_pInterface->Otp_OtpWrite(i * 1024, (i + 1) * 1024 - 1, dddd, CamID))
			{m_pInterface->Ctrl_Addlog(CamID,_T("EEEEEEEEEEE"),COLOR_RED,200);
				if (!m_pInterface->Otp_OtpWrite(i * 1024, (i + 1) * 1024 - 1, dddd, CamID))
				{m_pInterface->Ctrl_Addlog(CamID,_T("FFFFFFFFFFFFFFFF"),COLOR_RED,200);
					continue;
				}
			}
		}
	}*/

	//烧录 BasicData
	BasicData[0] = pDlg->m_EEPROMRevision;
	BasicData[1] = pDlg->m_HWVersion;
	BasicData[2] = pDlg->m_SWVersion;

	BasicData[3] = pDlg->m_LPN1;
	BasicData[4] = pDlg->m_LPN2;
	BasicData[5] = pDlg->m_LPN3;
	BasicData[6] = pDlg->m_LPN4;
	BasicData[7] = pDlg->m_LPN5;
	BasicData[8] = pDlg->m_LPN6;
	BasicData[9] = pDlg->m_LPN7;
	BasicData[10] = pDlg->m_LPN8;

	BasicData[11] = pDlg->m_ActuatorID;
	BasicData[12] = pDlg->m_LensID;

	BasicData[13] = (pDlg->m_ManufactureID >> 8) & 0xFF;
	BasicData[14] = pDlg->m_ManufactureID & 0xFF;
	BasicData[15] = (pDlg->m_FactoryID >> 8) & 0xFF;
	BasicData[16] = pDlg->m_FactoryID & 0xFF;
	//BasicData[13] = 'O';
	//BasicData[14] = 'F';
	//BasicData[15] = 'N';
	//BasicData[16] = 'C';
	BasicData[17] = pDlg->m_ManufactureLine;

	BasicData[18] = t.GetYear()-2000;
	BasicData[19] = t.GetMonth();
	BasicData[20] = t.GetDay();

	if (BasicData[19]>12||BasicData[19]<1||BasicData[20]>31||BasicData[20]<1||BasicData[18]>22||BasicData[18]<17)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("获取的年月日异常，请检查电脑设置!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("获取的年月日异常，请检查电脑设置!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
	USHORT FuseID_Char[16];
	memset(FuseID_Char,0,16*sizeof(USHORT));
	CString FuseID = m_pInterface->Sensor_GetCamFuseID(CamID);//GetSensorFuseID(FuseID_Char);
	m_pInterface->Ctrl_Addlog(CamID, _T("Fuse Id:") + FuseID, COLOR_BLUE, 200);
	//memcpy(FuseID_Char, FuseID, FuseID.GetLength());
	int nFuseIdLen = MIN(FuseID.GetLength()/2, sizeof(FuseID_Char) / sizeof(FuseID_Char[0]));
	for (i = 0; i < /*sizeof(FuseID_Char) / sizeof(FuseID_Char[0])*/nFuseIdLen; i++)
	{
		unsigned char ByH;
		unsigned char ByL;

		ByH = (unsigned char)(FuseID.GetAt(2 * i));
		ByL = (unsigned char)(FuseID.GetAt(2 * i + 1));

		ByH = GetBCD( ByH );
		ByL = GetBCD( ByL );

		FuseID_Char[i] = ByH;
		FuseID_Char[i] <<= 4;
		FuseID_Char[i] |= ByL;

		BasicData[21 + i] = FuseID_Char[i];
	}

	/*BasicData[21]= FuseID_Char[0];
	BasicData[22]= FuseID_Char[1];
	BasicData[23]= FuseID_Char[2];
	BasicData[24]= FuseID_Char[3];
	BasicData[25]= FuseID_Char[4];
	BasicData[26]= FuseID_Char[5];
	BasicData[27]= FuseID_Char[6];
	BasicData[28]= FuseID_Char[7];
	BasicData[29]= FuseID_Char[8];
	BasicData[30]= FuseID_Char[9];
	BasicData[31]= FuseID_Char[10];
	BasicData[32]= FuseID_Char[11];
	BasicData[33]= FuseID_Char[12];
	BasicData[34]= FuseID_Char[13];
	BasicData[35]= FuseID_Char[14];
	BasicData[36]= FuseID_Char[15];*/

	checksum = 0;
	checksum = GetCRC(BasicData, OTP_CHK_MODULE_ID_ADDR_END - OTP_CHK_MODULE_ID_ADDR_START + 1);
	BasicData[OTP_CHK_MODULE_ID_CHKSUM_ADDR_START] = (checksum >> 8) & 0xFF;
	BasicData[OTP_CHK_MODULE_ID_CHKSUM_ADDR_END] = checksum & 0xFF;

	//str.Format(_T("CheckSum=0x%04X  Addr[32]:0x%02X Addr[20]:0x%02X"), checksum, BasicData[32], BasicData[20]);
	//m_pInterface->Ctrl_Addlog(CamID, str, COLOR_BLUE, 200);

	if (!m_pInterface->Otp_OtpWrite(OTP_CHK_MODULE_ID_ADDR_START, OTP_CHK_MODULE_ID_CHKSUM_ADDR_END, BasicData, CamID))
	{
		if (!m_pInterface->Otp_OtpWrite(OTP_CHK_MODULE_ID_ADDR_START, OTP_CHK_MODULE_ID_CHKSUM_ADDR_END, BasicData, CamID))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Basic data烧录失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Basic data烧录失败!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}

	/**************************************************/
	/*               烧录AF SFR                      */
	/************************************************/
#if 0
	memset(AFData,0,26);
	m_pInterface->Otp_OtpRead(OTP_CHK_AF_ADDR_START, OTP_CHK_AF_CHKSUM_ADDR_END, AFData, CamID);

	int AFCheck_CRC = GetCRC(AFData,24);
	str.Format(_T("AFCRC=0x%04X  3F=0x%02X 40=0x%02X"),AFCheck_CRC,AFData[24],AFData[25]);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	if (((AFData[24] << 8) + AFData[25]) != AFCheck_CRC)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("AF CRC错误!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("AF CRC错误!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
#endif

#if 0
	memset(SFRData,0,455);
	m_pInterface->Otp_OtpRead(OTP_CHK_SFR_ADDR_START, OTP_CHK_SFR_CHKSUM_ADDR_END, SFRData, CamID);

	int SFRCheck_CRC = GetCRC(SFRData,453);
	str.Format(_T("SFRCRC=0x%04X  245=0x%02X 246=0x%02X"),SFRCheck_CRC,SFRData[453],SFRData[454]);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	if (((SFRData[453] << 8) + SFRData[454]) != SFRCheck_CRC)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("SFR CRC错误!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("SFR CRC错误!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
#endif

	//烧录AWB
	int siteNo=m_pInterface->GetSiteNo();
	CString WBDataPath;
	WBDataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\AWBData_%d.ini"),CamID,siteNo);
	WBDataPath=m_pInterface->Ctrl_GetCurrentPath() + WBDataPath;
	FILE *pWBFile = _tfopen(WBDataPath.GetBuffer(WBDataPath.GetLength()), _T("r"));
	if (NULL == pWBFile)
	{
		CString strLog;

		strLog.Format(_T("open file %s Error\r\n"), WBDataPath.GetBuffer(WBDataPath.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_RED, 200);
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
	fclose(pWBFile);
	int Golden_R_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_R"),  0x15,   WBDataPath );
	int Golden_B_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_B"),  0x15,   WBDataPath );
	int Golden_Gr_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_Gr"),  0x15,   WBDataPath );
	int Golden_Gb_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_Gb"),  0x15,   WBDataPath );
	int Golden_RG_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_RG"),  0x15,   WBDataPath );
	int Golden_BG_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_BG"),  0x15,   WBDataPath );
	int Golden_GG_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("Golden_GG"),  0x15,   WBDataPath );

	str.Format(_T("Golden_R_HighCCT=%d  Golden_B_HighCCT=%d"),Golden_R_HighCCT,Golden_B_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("Golden_Gr_HighCCT=%d  Golden_Gb_HighCCT=%d"),Golden_Gr_HighCCT,Golden_Gb_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("Golden_RG_HighCCT=%d  Golden_BG_HighCCT=%d  Golden_GG_HighCCT=%d"),Golden_RG_HighCCT,Golden_BG_HighCCT,Golden_GG_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

	memset(WBData, 0, sizeof(WBData) / sizeof(WBData[0]));
	int Ev_HighCCT = GetPrivateProfileInt( _T("AWBData"), _T("Ev_5000k"), 0, WBDataPath);
	int u_HighCCT  = GetPrivateProfileInt( _T("AWBData"), _T("u_5000k"), 0, WBDataPath);
	int v_HighCCT  = GetPrivateProfileInt( _T("AWBData"), _T("v_5000k"), 0, WBDataPath);
	str.Format(_T("Ev_5000k=%d  u_5000k=%d  v_5000k=%d"), Ev_HighCCT, u_HighCCT, v_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID, str, COLOR_BLUE, 200);

	WBData[0] = (Ev_HighCCT>>8);
	WBData[1] = Ev_HighCCT & 0xff;
	WBData[2] = (u_HighCCT>>8);
	WBData[3] = u_HighCCT & 0xff;
	WBData[4] = (v_HighCCT>>8);
	WBData[5] = v_HighCCT & 0xff;

	WBData[6] =  (Golden_R_HighCCT >> 8) & 0x03;	
	WBData[7] =  Golden_R_HighCCT & 0xff; 
	WBData[8] =  (Golden_Gr_HighCCT >> 8) & 0x03;
	WBData[9] =  Golden_Gr_HighCCT & 0xff;      
	WBData[10] =  (Golden_Gb_HighCCT >> 8) & 0x03;
	WBData[11] =  Golden_Gb_HighCCT & 0xff; 
	WBData[12] =  (Golden_B_HighCCT >> 8) & 0x03;
	WBData[13] =  Golden_B_HighCCT & 0xff;      
	WBData[14] = (Golden_RG_HighCCT>>8);
	WBData[15] = Golden_RG_HighCCT & 0xff;
	WBData[16] = (Golden_BG_HighCCT>>8);
	WBData[17] = Golden_BG_HighCCT & 0xff;
	WBData[18] = (Golden_GG_HighCCT>>8);
	WBData[19] = Golden_GG_HighCCT & 0xff;

	int R_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_R"),  0x15,   WBDataPath );
	int B_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_B"),  0x15,   WBDataPath );
	int Gr_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_Gr"),  0x15,   WBDataPath );
	int Gb_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_Gb"),  0x15,   WBDataPath );
	int RG_fin_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_RGain"),  0x15,   WBDataPath );
	int BG_fin_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_BGain"),  0x15,   WBDataPath );
	int GG_fin_HighCCT = GetPrivateProfileInt(_T("AWBData"),_T("OTP_GGain"),  0x15,   WBDataPath );

	str.Format(_T("OTP_R=%d  OTP_B=%d"),R_HighCCT,B_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("OTP_Gr=%d  OTP_Gb=%d"),Gr_HighCCT,Gb_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("OTP_RGain=%d  OTP_BGain=%d  OTP_GGain=%d"),RG_fin_HighCCT,BG_fin_HighCCT,GG_fin_HighCCT);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

	WBData[20] = (R_HighCCT >> 8) & 0x03;
	WBData[21] = R_HighCCT & 0xff; 
	WBData[22] = (Gr_HighCCT >> 8) & 0x03;
	WBData[23] = Gr_HighCCT & 0xff;      
	WBData[24] = (Gb_HighCCT >> 8) & 0x03;
	WBData[25] = Gb_HighCCT & 0xff; 
	WBData[26] = (B_HighCCT >> 8) & 0x03;	
	WBData[27] = B_HighCCT & 0xff;      
	WBData[28] = (RG_fin_HighCCT>>8);
	WBData[29] = RG_fin_HighCCT & 0xff;
	WBData[30] = (BG_fin_HighCCT>>8);
	WBData[31] = BG_fin_HighCCT & 0xff;
	WBData[32] = (GG_fin_HighCCT>>8);
	WBData[33] = GG_fin_HighCCT & 0xff;

	int WB_CRC = GetCRC(WBData, 34);
	WBData[34] = ((WB_CRC>>8) & 0xFF);
	WBData[35] = (WB_CRC & 0xFF);

	//str.Format(_T("WB_CRC=0x%04X  6C=0x%02X 6D=0x%02X"), WB_CRC, WBData[34], WBData[35]);
	//m_pInterface->Ctrl_Addlog(CamID, str, COLOR_BLUE, 200);

	if (!m_pInterface->Otp_OtpWrite(OTP_CHK_AWB_ADDR_START, OTP_CHK_AWB_CHKSUM_ADDR_END, WBData, CamID))
	{
		if (!m_pInterface->Otp_OtpWrite(OTP_CHK_AWB_ADDR_START, OTP_CHK_AWB_CHKSUM_ADDR_END, WBData, CamID))
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("AWB data烧录失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("AWB data烧录失败!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}

	/**************************************************/
	/*               烧录OC                          */
	/************************************************/
	CString OCDataPath;
	OCDataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\OCData_%d.ini"),CamID,siteNo);
	OCDataPath=m_pInterface->Ctrl_GetCurrentPath()+OCDataPath;
	FILE *pOCFile = _tfopen(OCDataPath.GetBuffer(OCDataPath.GetLength()), _T("r"));
	if (NULL == pOCFile)
	{
		CString strLog;

		strLog.Format(_T("open file %s Error\r\n"), OCDataPath.GetBuffer(OCDataPath.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_RED, 200);
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
	fclose(pOCFile);
	int R_OpticalPixel_x = GetPrivateProfileInt(_T("OCData"),_T("OC_R_X"),0,OCDataPath);
	int R_OpticalPixel_y = GetPrivateProfileInt(_T("OCData"),_T("OC_R_Y"),0,OCDataPath);
	int Gr_OpticalPixel_x = GetPrivateProfileInt(_T("OCData"),_T("OC_Gr_X"),0,OCDataPath);
	int Gr_OpticalPixel_y = GetPrivateProfileInt(_T("OCData"),_T("OC_Gr_Y"),0,OCDataPath);
	int Gb_OpticalPixel_x = GetPrivateProfileInt(_T("OCData"),_T("OC_Gb_X"),0,OCDataPath);
	int Gb_OpticalPixel_y = GetPrivateProfileInt(_T("OCData"),_T("OC_Gb_Y"),0,OCDataPath);
	int B_OpticalPixel_x = GetPrivateProfileInt(_T("OCData"),_T("OC_B_X"),0,OCDataPath);
	int B_OpticalPixel_y = GetPrivateProfileInt(_T("OCData"),_T("OC_B_Y"),0,OCDataPath);

	OCData[0] =  (R_OpticalPixel_x)>>8;
	OCData[1] =  (R_OpticalPixel_x)&0xff;
	OCData[2] =  (R_OpticalPixel_y)>>8;
	OCData[3] =  (R_OpticalPixel_y)&0xff;

	OCData[4] =  (Gr_OpticalPixel_x)>>8;
	OCData[5] =  (Gr_OpticalPixel_x)&0xff;
	OCData[6] =  (Gr_OpticalPixel_y)>>8;
	OCData[7] =  (Gr_OpticalPixel_y)&0xff;

	OCData[8]  =  (Gb_OpticalPixel_x)>>8;
	OCData[9] =  (Gb_OpticalPixel_x)&0xff;
	OCData[10] =  (Gb_OpticalPixel_y)>>8;
	OCData[11] =  (Gb_OpticalPixel_y)&0xff;

	OCData[12] =  (B_OpticalPixel_x)>>8;
	OCData[13] =  (B_OpticalPixel_x)&0xff;
	OCData[14] =  (B_OpticalPixel_y)>>8;
	OCData[15] =  (B_OpticalPixel_y)&0xff;

	int OC_CRC = GetCRC(OCData,16);
	OCData[16] = ((OC_CRC>>8)&0xff);
	OCData[17] = (OC_CRC&0xff);

	//str.Format(_T("OC_CRC=0x%04X  7E=0x%02X 7F=0x%02X"),OC_CRC,OCData[16],OCData[17]);
	//m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

	if (!m_pInterface->Otp_OtpWrite(OTP_CHK_OC_ADDR_START, OTP_CHK_OC_CHKSUM_ADDR_END, OCData, CamID))
	{
		if (!m_pInterface->Otp_OtpWrite(OTP_CHK_OC_ADDR_START, OTP_CHK_OC_CHKSUM_ADDR_END, OCData, CamID))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("OC data烧录失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OC data烧录失败!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}

#if 0
	/**************************************************/
	/*               烧录GainMap                     */
	/************************************************/
	vector <BYTE> m_GainmapData;
	m_GainmapData.resize(890);

	CString GainmapPath;
	GainmapPath.Format(_T("Gainmap_%d.bin"),siteNo); 
	if(!m_pInterface->Otp_ReadOtpBoard(CamID,GainmapPath,&m_GainmapData,1))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("读取Gainmap bin文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("读取Gainmap bin文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}

	GainmapData[0] = 0xff;	// Sensor Mode used for DCC cal (1 Byte, char: 0x31 Mode 1, 0x32 Mode 2, 0x33 Mode 3)
	GainmapData[1] = 0xff;	// Sensor Mode used for Gain cal (1 Byte, char: 0x31 Mode 1, 0x32 Mode 2, 0x33 Mode 3)
	GainmapData[2] = 0x30;	//mirror filp normal
	for(i=0;i<890;i++)//0x1103-0x147C=890--注意：Gainmap_%d.bin文件中已经有Version Number、Map Width、Map Height数据，所以直接拷贝即可
	{
		GainmapData[i+3]=m_GainmapData[i];
	}

	int GainMap_CRC = GetCRC(GainmapData,893);
	GainmapData[893] = ((GainMap_CRC>>8)&0xff);
	GainmapData[894] = (GainMap_CRC&0xff);

	str.Format(_T("GainMap_CRC=0x%04X  147D=0x%02X 147E=0x%02X"),GainMap_CRC,GainmapData[893],GainmapData[894]);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	if (!m_pInterface->Otp_OtpWrite(OTP_CHK_LRC_ADDR_START, OTP_CHK_LRC_CHKSUM_ADDR_END, GainmapData, CamID))
	{
		if (!m_pInterface->Otp_OtpWrite(OTP_CHK_LRC_ADDR_START, OTP_CHK_LRC_CHKSUM_ADDR_END, GainmapData, CamID))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Gainmap data烧录失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Gainmap data烧录失败!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}
#endif

	//lsc
	vector <BYTE> m_LSCData;
	m_LSCData.resize(1768);

	CString LSCPath;
	LSCPath.Format(_T("QualcommLSC_%d.bin"),siteNo); //R,GR,GB,B,L ,H
	if(!m_pInterface->Otp_ReadOtpBoard(CamID,LSCPath,&m_LSCData,1))
	{
		m_LSCData.clear();
		m_pInterface->Ctrl_Addlog(CamID,_T("读取QualcommLSC bin文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("读取QualcommLSC bin文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}

	for (i = 0; i < 884; i++)//RGrGbB H,L
	{
		LSCData[2 * i] = m_LSCData[2 * i + 1];
		LSCData[2 * i + 1] = m_LSCData[2 * i];
	}
	// 	int j = 0;
	// 	for (i=0;i<221;i++) //R1Gr1Gb1B1...R221Gr221Gb221B221 H,L
	// 	{
	// 		LSCData[j] = m_LSCData[2*i+1];
	// 		LSCData[j+1] = m_LSCData[2*i];
	// 		LSCData[j+2] = m_LSCData[442+2*i+1];
	// 		LSCData[j+3] = m_LSCData[442+2*i];
	// 		LSCData[j+4] = m_LSCData[884+2*i+1];
	// 		LSCData[j+5] = m_LSCData[884+2*i];
	// 		LSCData[j+6] = m_LSCData[1326+2*i+1];
	// 		LSCData[j+7] = m_LSCData[1326+2*i];
	// 		j = j+8;
	// 	}
	checksum = GetCRC(LSCData, 1768);
	LSCData[1768] = (checksum >> 8) & 0xFF;
	LSCData[1769] = checksum & 0xFF;

	if (!m_pInterface->Otp_OtpWrite(OTP_CHK_LSC_ADDR_START, OTP_CHK_LSC_CHKSUM_ADDR_END, LSCData, CamID))
	{
		if (!m_pInterface->Otp_OtpWrite(OTP_CHK_LSC_ADDR_START, OTP_CHK_LSC_CHKSUM_ADDR_END, LSCData, CamID))
		{
			m_LSCData.clear();
			m_pInterface->Ctrl_Addlog(CamID, _T("LSC data烧录失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("LSC data烧录失败!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}
	m_LSCData.clear();

	vector <BYTE> m_CTalkData;
	m_CTalkData.resize(1056);

	CString CTalkPath;
	CTalkPath.Format(_T("Xtalk_%d.bin"),siteNo); //DataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\Xtalk_%d.bin"), CamID, siteNo);
	if(!m_pInterface->Otp_ReadOtpBoard(CamID,CTalkPath,&m_CTalkData,1))
	{
		m_CTalkData.clear();
		m_pInterface->Ctrl_Addlog(CamID,_T("读取CrossTalk bin文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("读取CrossTalk bin文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
	
	for (i = 0; i < 1056; i++)
	{
		CTalkData[i] = m_CTalkData[i];
	}
	checksum = GetCRC(CTalkData, 1056);
	CTalkData[1056] = (checksum >> 8) & 0xFF;
	CTalkData[1057] = checksum & 0xFF;

	if (!m_pInterface->Otp_OtpWrite(OTP_CHK_CTalk_ADDR_START, OTP_CHK_CTalk_CHKSUM_ADDR_END, CTalkData, CamID))
	{
		if (!m_pInterface->Otp_OtpWrite(OTP_CHK_CTalk_ADDR_START, OTP_CHK_CTalk_CHKSUM_ADDR_END, CTalkData, CamID))
		{
			m_CTalkData.clear();
			m_pInterface->Ctrl_Addlog(CamID, _T("CrossTalk data烧录失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("CrossTalk data烧录失败!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}
	m_CTalkData.clear();

	Sleep(300);	//烧录完成后在读取之前需要等待一下，让芯片烧录成功--Added by LiHai--20180508

	/** 数据读取出来，进行对比，看看是否烧录成功 **/
	USHORT TempReadData[0x1FFF]= {0};
	BYTE ReadData[0x1FFF]= {0};
	m_pInterface->Otp_OtpRead(0x0000, sizeof(TempReadData)/sizeof(TempReadData[0]) - 1, TempReadData, CamID);
	//BasicData Check
	for (i = OTP_CHK_MODULE_ID_ADDR_START; i <= OTP_CHK_MODULE_ID_CHKSUM_ADDR_END; i++)
	{
		if (TempReadData[i] != BasicData[i])
		{
			CString str;
			str.Format(_T("BasicData[%d]---0x%X != 0x%X\r\n"), i, TempReadData[i], BasicData[i]);
			m_pInterface->Ctrl_Addlog(CamID, str, COLOR_RED, 200);

			m_pInterface->Ctrl_Addlog(CamID,_T("BasicData烧录的值与读取的值不一致!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("BasicData烧录的值与读取的值不一致!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);

			//for (i = OTP_CHK_MODULE_ID_ADDR_START; i <= OTP_CHK_MODULE_ID_CHKSUM_ADDR_END; i++)
			//{
			//	CString str;
			//	str.Format(_T("BasicData[%d]---0x%X 0x%X\r\n"), i, TempReadData[i], BasicData[i]);
			//	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
			//}

			return 0;
		}
	}
	//WBData Check
	for (i = 0; i < OTP_AWB_LEN; i++)
	{
		if (TempReadData[i + OTP_CHK_AWB_ADDR_START] != WBData[i])
		{
			CString str;
			str.Format(_T("WBData[%d]---0x%X != 0x%X\r\n"), i, TempReadData[i + OTP_CHK_AWB_ADDR_START], WBData[i]);
			m_pInterface->Ctrl_Addlog(CamID, str, COLOR_RED, 200);

			m_pInterface->Ctrl_Addlog(CamID,_T("AWBData烧录的值与读取的值不一致!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("AWBData烧录的值与读取的值不一致!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);

			//for (i = 0; i <= OTP_AWB_LEN; i++)
			//{
			//	CString str;
			//	str.Format(_T("WBData[%d]---0x%X 0x%X\r\n"), i, TempReadData[i + OTP_CHK_AWB_ADDR_START], WBData[i]);
			//	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
			//}

			return 0;
		}
	}
	//OCData Check
	for (i = 0; i < OTP_OC_LEN; i++)
	{
		if (TempReadData[i + OTP_CHK_OC_ADDR_START] != OCData[i])
		{

			CString str;
			str.Format(_T("OCData[%d]---0x%X != 0x%X\r\n"), i, TempReadData[i + OTP_CHK_OC_ADDR_START], OCData[i]);
			m_pInterface->Ctrl_Addlog(CamID, str, COLOR_RED, 200);

			m_pInterface->Ctrl_Addlog(CamID,_T("OCData烧录的值与读取的值不一致!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OCData烧录的值与读取的值不一致!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);

			//for (i = 0; i <= OTP_OC_LEN; i++)
			//{
			//	CString str;
			//	str.Format(_T("OCData[%d]---0x%X 0x%X\r\n"), i, TempReadData[i + OTP_CHK_OC_ADDR_START], OCData[i]);
			//	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
			//}

			return 0;
		}
	}
	//LSC Check
	for (i = 0; i < OTP_LSC_LEN; i++)
	{
		if (TempReadData[i + OTP_CHK_LSC_ADDR_START] != LSCData[i])
		{
			CString str;
			str.Format(_T("LSCData[%d]---0x%X != 0x%X\r\n"), i, TempReadData[i + OTP_CHK_LSC_ADDR_START], LSCData[i]);
			m_pInterface->Ctrl_Addlog(CamID, str, COLOR_RED, 200);

			m_pInterface->Ctrl_Addlog(CamID,_T("LSCData烧录的值与读取的值不一致!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSCData烧录的值与读取的值不一致!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);

			//for (i = 0; i <= OTP_LSC_LEN; i++)
			//{
			//	CString str;
			//	str.Format(_T("LSCData[%d]---0x%X 0x%X\r\n"), i, TempReadData[i + OTP_CHK_LSC_ADDR_START], LSCData[i]);
			//	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
			//}

			return 0;
		}
	}
#if 0
	//GainMapData Check
	for (i=0;i<895;i++)
	{
		if (TempReadData[i + OTP_CHK_LRC_ADDR_START]!=GainmapData[i])
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("GainmapData烧录的值与读取的值不一致!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("GainmapData烧录的值与读取的值不一致!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300);
			return 0;
		}
	}
#endif

	//删除bin 文件
	CString DataPath;
	DataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\AWBData_%d.ini"),CamID,siteNo);
	DataPath=m_pInterface->Ctrl_GetCurrentPath()+DataPath;
	if(!DeleteFile(DataPath))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("删除AWB文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("删除AWB文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}

	//CString OCDataPath1;
	DataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\OCData_%d.ini"),CamID,siteNo);
	DataPath=m_pInterface->Ctrl_GetCurrentPath()+DataPath;
	if(!DeleteFile(DataPath))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("删除OC文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("删除OC文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
	//CString LSCDataPath;
	DataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\QualcommLSC_%d.bin"),CamID,siteNo);
	DataPath=m_pInterface->Ctrl_GetCurrentPath()+DataPath;
	if(!DeleteFile(DataPath))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("删除LSC文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("删除LSC文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
	//CString CrossTalkDataPath;
	DataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\Xtalk_%d.bin"), CamID, siteNo);
	DataPath = m_pInterface->Ctrl_GetCurrentPath() + DataPath;
	if(!DeleteFile(DataPath))
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("删除CrossTalk文件失败!"), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("删除CrossTalk文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return FALSE;
	}
#if 0
	CString GainmapDataPath;
	GainmapDataPath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\Gainmap_%d.bin"),CamID,siteNo);
	GainmapDataPath=m_pInterface->Ctrl_GetCurrentPath()+GainmapDataPath;
	if(!DeleteFile(GainmapDataPath))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("删除Gainmap文件失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("删除Gainmap文件失败!"));
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
		return 0;
	}
#endif
	m_pInterface->Ctrl_ResumeGrabFrame(CamID);
	Sleep(300);
	return 0;
}


/********** ********************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int L476F00_OTP_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int L476F00_OTP_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_EEPROMRevision=ReadConfigInt(_T("EEPROMTableRev"),0x33);
	pDlg->m_HWVersion=ReadConfigInt(_T("CalHWVer"),0x30);
	pDlg->m_SWVersion=ReadConfigInt(_T("CalSWVer"),0x30);

	pDlg->m_ActuatorID=ReadConfigInt(_T("ActuatorID"),0x30);
	pDlg->m_LensID=ReadConfigInt(_T("LensID"),0x29);
	pDlg->m_ManufactureID=ReadConfigInt(_T("ManufactuterID"),0x4F46);
	pDlg->m_FactoryID=ReadConfigInt(_T("FactoryID"),0x4E43);
	pDlg->m_ManufactureLine=ReadConfigInt(_T("ManufactuterLine"),0x31);

	pDlg->m_LPN1=ReadConfigInt(_T("LPN1"),0x32);
	pDlg->m_LPN2=ReadConfigInt(_T("LPN2"),0x38);
	pDlg->m_LPN3=ReadConfigInt(_T("LPN3"),0x43);
	pDlg->m_LPN4=ReadConfigInt(_T("LPN4"),0x32);
	pDlg->m_LPN5=ReadConfigInt(_T("LPN5"),0x36);
	pDlg->m_LPN6=ReadConfigInt(_T("LPN6"),0x36);
	pDlg->m_LPN7=ReadConfigInt(_T("LPN7"),0x38);
	pDlg->m_LPN8=ReadConfigInt(_T("LPN8"),0x35);


//	pDlg->m_DriverICID=ReadConfigInt(_T("DriverICID"),1);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int L476F00_OTP_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("EEPROMTableRev"),pDlg->m_EEPROMRevision);
	WriteConfigInt(_T("CalHWVer"),pDlg->m_HWVersion);
	WriteConfigInt(_T("CalSWVer"),pDlg->m_SWVersion);


	WriteConfigInt(_T("ActuatorID"),pDlg->m_ActuatorID);
	WriteConfigInt(_T("LensID"),pDlg->m_LensID);
	WriteConfigInt(_T("ManufactuterID"),pDlg->m_ManufactureID);
	WriteConfigInt(_T("FactoryID"),pDlg->m_FactoryID);
	WriteConfigInt(_T("ManufactuterLine"),pDlg->m_ManufactureLine);

	WriteConfigInt(_T("LPN1"),pDlg->m_LPN1);
	WriteConfigInt(_T("LPN2"),pDlg->m_LPN2);
	WriteConfigInt(_T("LPN3"),pDlg->m_LPN3);
	WriteConfigInt(_T("LPN4"),pDlg->m_LPN4);
	WriteConfigInt(_T("LPN5"),pDlg->m_LPN5);
	WriteConfigInt(_T("LPN6"),pDlg->m_LPN6);
	WriteConfigInt(_T("LPN7"),pDlg->m_LPN7);
	WriteConfigInt(_T("LPN8"),pDlg->m_LPN8);
	//WriteConfigInt(_T("LensID"),pDlg->m_LensID);
	//WriteConfigInt(_T("VCMID"),pDlg->m_VCMID);
	//WriteConfigInt(_T("DriverICID"),pDlg->m_DriverICID);
	//WriteConfigInt(_T("IRBGID"),pDlg->m_IRBGID);
	//WriteConfigInt(_T("GyroID"),pDlg->m_GyroID);
	//WriteConfigInt(_T("ColorTemperatureID"),pDlg->m_ColorTemperatureID);
	//WriteConfigInt(_T("OISDriverICID"),pDlg->m_OISDriverICID);
	//WriteConfigInt(_T("FPC_PCBVersion"),pDlg->m_FPC_PCBVersion);
	//WriteConfigInt(_T("CalibrationVersion"),pDlg->m_CalibrationVersion);
	//WriteConfigInt(_T("OISfirmwareversion"),pDlg->m_OISfirmwareversion);
	//WriteConfigInt(_T("Camerabuildingstage"),pDlg->m_Camerabuildingstage);
	//WriteConfigInt(_T("Productionfactor"),pDlg->m_Productionfactory);
	//WriteConfigInt(_T("Productionline"),pDlg->m_Productionline);



//   WriteConfigInt(_T("DriverICID"),pDlg->m_DriverICID);

// 	pDlg->m_ModuleID=ReadConfigInt(_T("ModuleID"),7);
// 	pDlg->m_LensID=ReadConfigInt(_T("LensID"),4);
// 	pDlg->m_VCMID=ReadConfigInt(_T("VCMID"),1);
// 	pDlg->m_DriverICID=ReadConfigInt(_T("DriverICID"),1);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR L476F00_OTP_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("Result");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR L476F00_OTP_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR L476F00_OTP_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR L476F00_OTP_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	if (GetResult() == RESULT_FAIL)
	{
		Content += _T("Fail");
	}
	else
	{
		Content += _T("Pass");
	}
	return Content;
}

void L476F00_OTP_TestItem::Dump()
{
	FILE *fp = NULL;

	CString sfilename = _T("");
	CString sTmp      = _T("");
	CString sfileext  = _T("");
	CString sfilter   = _T("Ini File(*.txt)|*.txt");

	CFileFind ff;
	CString sn = _T("");
	CString number = _T("");
	USES_CONVERSION;
	int siteNo=m_pInterface->GetSiteNo();
	CString strFuseID;
	strFuseID.Format(_T("%d_"),siteNo);
   
	strFuseID=strFuseID+m_pInterface->Sensor_GetCamFuseID(CamID);
	strFuseID=strFuseID+_T("Readback Value.ini");
	CString   SensorINI = m_pInterface->Ctrl_GetCurrentPath() +strFuseID ; // 设定参数文档路径

	sfilename.Format(SensorINI);

	fp = fopen(T2A(sfilename), "a");
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeWrite | CFile::typeText);  
	Sleep(100);

	USHORT ReadData[8192]={0};
	m_pInterface->Otp_OtpRead(0x0000,0x1FFF,ReadData,CamID);
	for (int k=0;k<8192;k++)
	{
		sTmp.Format(_T("0x%04X\t0x%02X\n"),k,ReadData[k]);
		MyFile.WriteString(sTmp);

	}
	MyFile.Close();
	ShellExecute(NULL, _T("open"), sfilename, NULL, NULL, SW_SHOWNORMAL);
}

int L476F00_OTP_TestItem::GetCRC(USHORT *Data, int Len)
{
	int CRC[16] = {0};
	int DoInvert = 0,i=0,j=0;
	int mycrc16x = 0;
	for (i=0; i<Len; i++)
	{
		int hexonly = Data[i];
		for (j=7; j>=0; j--)
		{
			DoInvert = hexonly & ((int)pow(2.0,j));
			DoInvert = DoInvert / (pow(2.0,j));
			DoInvert = DoInvert ^ CRC[15];

			CRC[15] = CRC[14] ^ DoInvert;
			CRC[14] = CRC[13];
			CRC[13] = CRC[12];
			CRC[12] = CRC[11];
			CRC[11] = CRC[10];
			CRC[10] = CRC[9];
			CRC[9] = CRC[8];
			CRC[8] = CRC[7];
			CRC[7] = CRC[6];
			CRC[6] = CRC[5];
			CRC[5] = CRC[4];
			CRC[4] = CRC[3];
			CRC[3] = CRC[2];
			CRC[2] = CRC[1] ^ DoInvert;
			CRC[1] = CRC[0];
			CRC[0] = DoInvert;
		}
	}

	for (i=0; i<16; i++)
	{
		mycrc16x = mycrc16x + (CRC[i]*pow(2.0,i));
	}
	return mycrc16x;
}