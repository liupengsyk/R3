#include "StdAfx.h"
#include "HwBPTTest_TestItem.h"
#include "ImageProc.h"
//#include <direct.h>

#include "BPTCalibrationDLL/BPTTest.h"
#pragma comment (lib,"BPTCalibrationDLL/BPTCalibration.lib")



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new HwBPTTest_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
HwBPTTest_TestItem::HwBPTTest_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	m_szName = lpszName;
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

HwBPTTest_TestItem::~HwBPTTest_TestItem(void)
{

}


int HwBPTTest_TestItem::InitItem()
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
int HwBPTTest_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
	FuseID = GetSensorFuseID();
 
	if (!swscanf(pDlg->m_bptErrCode, _T("0x%x"), &m_errCode))
		swscanf(pDlg->m_bptErrCode, _T("%d"), &m_errCode);

	if (!swscanf(pDlg->m_bptFrmDelayNum, _T("0x%x"), &m_frmDelayNum))
		swscanf(pDlg->m_bptFrmDelayNum, _T("%d"), &m_frmDelayNum);

	m_pInterface->Sensor_ReadExp(CamID, m_initExpTime);
	m_pInterface->Sensor_ReadGain(CamID, m_initExpGain);
	CString strLog = _T("");
	strLog.Format(_T("Init Exp = 0x%X, Gain = 0x%X"), m_initExpTime, m_initExpGain);
	m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

	m_tstMode = _T("None");
	m_frmDelayCnt = 0;

	InitializeCriticalSection(&m_csLock);

	m_StartTiem = 0;
	m_EndTime = 0;
	m_badPixelCount = 0;
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int HwBPTTest_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	int myCamID = 0;
	CString strLog = _T("");
	if (m_tstMode == _T("None"))
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("Set BPT Test Start."), COLOR_BLUE, 200);

		int defExpoTime = 0;
		if (!swscanf(pDlg->m_ExValue, _T("0x%x"), &defExpoTime))
			swscanf(pDlg->m_ExValue, _T("%d"), &defExpoTime);
		USHORT defGain = 0;
		if (!swscanf(pDlg->m_GainValue, _T("0x%x"), &defGain))
			swscanf(pDlg->m_GainValue, _T("%d"), &defGain);
		m_pInterface->Sensor_WriteGain(CamID, defGain);

		if (pDlg->m_SensorSelection==IMX_SENSOR)
		{
			m_pInterface->Device_WriteI2c(0x34,0x0100,0x00,3,CamID);
			m_pInterface->Device_WriteI2c(0x34,0x0340,0xB5,3,CamID);
			m_pInterface->Device_WriteI2c(0x34,0x0341,0xEA,3,CamID);
			m_pInterface->Device_WriteI2c(0x34,0x0100,0x01,3,CamID);
		} 
		else		//OV12A10
		{
			m_pInterface->Device_WriteI2c(0x6C, 0x3208, 0x03, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3c80, 0x0c, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3c90, 0x01, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3c92, 0x00, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3c93, 0x13, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3c94, 0x12, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3c95, 0xf6, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3500, 0x00, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3501, 0xea, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3502, 0x60, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3508, 0x02, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3509, 0x00, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x350a, 0x04, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x350b, 0x00, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x380e, 0x0e, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x380f, 0xae, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3826, 0x0d, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3827, 0x04, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x401a, 0x40, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3714, 0x20, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x5203, 0x0c, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x5204, 0x11, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x520d, 0xf5, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x520e, 0xf5, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x5208, 0xff, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x5201, 0xa8, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x5200, 0x01, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3208, 0x13, 3, CamID);
			m_pInterface->Device_WriteI2c(0x6C, 0x3208, 0xa3, 3, CamID);
		}

		//m_pInterface->Sensor_WriteExp(CamID, defExpoTime);
		

		Sleep(30);
		int tmpExpT = 0;
		USHORT tmpGain = 0;
		m_pInterface->Sensor_ReadExp(CamID, tmpExpT);
		m_pInterface->Sensor_ReadGain(CamID, tmpGain);
		strLog.Format(_T("Write Exp = 0x%X, Gain = 0x%X"), tmpExpT, tmpGain);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

		m_frmDelayCnt = 0;
		m_tstMode = _T("BPT_TEST");

	}

	if (m_tstMode == _T("BPT_TEST"))
	{
		if (m_frmDelayCnt < m_frmDelayNum)
		{
			Sleep(300);
			m_frmDelayCnt++;
			return 1;
		}
		else
		{

			ImageInfo img = * m_pInterface->Ctrl_GetImgInfo(CamID);

			if (pDlg->m_SensorSelection == IMX_SENSOR)
			{
				m_pInterface->Device_WriteI2c(0x34,0x0100,0x00,3,CamID);
				m_pInterface->Device_WriteI2c(0x34,0x0340,0x0F,3,CamID);
				m_pInterface->Device_WriteI2c(0x34,0x0341,0xA8,3,CamID);
				m_pInterface->Device_WriteI2c(0x34,0x0100,0x01,3,CamID);
			}
			else
			{
				m_pInterface->Device_WriteI2c(0x6C, 0x0100, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3c80, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3c90, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3500, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3501, 0xea, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3502, 0x60, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3508, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3509, 0x80, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x350a, 0x04, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x350b, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x380e, 0x0e, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x380f, 0xae, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3827, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3826, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x401a, 0x58, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x4092, 0x00, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x3714, 0x24, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x5203, 0x24, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x5204, 0x12, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x520d, 0x0f, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x520e, 0xfd, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x5208, 0x40, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x5201, 0x94, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x5200, 0x03, 3, CamID);
				m_pInterface->Device_WriteI2c(0x6C, 0x0100, 0x01, 3, CamID);
			}

			m_pInterface->Sensor_WriteExp(CamID, m_initExpTime);
			m_pInterface->Sensor_WriteGain(CamID, m_initExpGain);
			int exp = 0;
			m_pInterface->Sensor_ReadExp(CamID, exp);
			USHORT gain = 0;
			m_pInterface->Sensor_ReadGain(CamID, gain);
			if (exp == m_initExpTime && gain == m_initExpGain)
				m_pInterface->Ctrl_Addlog(CamID, _T("Gain reinit !"), COLOR_BLUE, 200);

			BadPixelTable m_pBpt;
			m_pBpt.BadPixelStack = (PixelInfo *)new	PixelInfo[4000];
			if (m_pBpt.BadPixelStack == NULL)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("分配内存失败！"),COLOR_RED,200);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("内存分配失败！"));
				m_pInterface->Ctrl_SetCamErrorCode(CamID,m_errCode);
				return 0;
			}
			for (int i = 0; i < 4000; i++)
			{
				m_pBpt.BadPixelStack[i].Average = 0;
				for (int m=0; m<20; m++ )
				{
					m_pBpt.BadPixelStack[i].NeighborFlag[m] = 0;
				}
				m_pBpt.BadPixelStack[i].xIndex = 0;
				m_pBpt.BadPixelStack[i].yIndex = 0;
				m_pBpt.BadPixelStack[i].Y_Value = 0;
				m_pBpt.BadPixelStack[i].x = 0;
				m_pBpt.BadPixelStack[i].y = 0;
			}
			
			BYTE *ImageInput = new BYTE[img.imgwidth*img.imgheight*2];
			RAW10_To_BYTE(img.Raw_buffer,ImageInput,img.imgwidth, img.imgheight);
// 			BYTE *ImageInput = new BYTE[img.imgwidth*img.imgheight];
// 			Raw10toRaw8(img.Raw_buffer,ImageInput,img.imgwidth, img.imgheight);
			if (pDlg->m_SavePic==TRUE)
			{
				CFile cf;
				CString PathName = _T("D:\\BPTTest\\Raw10.raw");
				BOOL ret = cf.Open(PathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				if (!ret)
				{
					return 0;
				} 
				cf.Write((void*)img.Raw_buffer, img.imgwidth * img.imgheight*2);
				cf.Close();
			}
			EnterCriticalSection(&m_csLock);
			{
				SetName(_T("Sensor"));
				int OutFormat = ReadConfigInt(_T("outformat"),0);
				strLog.Format(_T("OutFormat = %d"), OutFormat);
				m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
				SetName(m_szName);
				BOOL ret = 0;
				if (pDlg->m_SaveData)
				{
					ret =GetBPTTest(ImageInput, img.imgwidth, img.imgheight, OutFormat, &m_pBpt, 1, 1);
				} 
				else
				{
					ret = GetBPTTest(ImageInput, img.imgwidth, img.imgheight, OutFormat, &m_pBpt, 0, 0);
				}
				if (ret == 0)
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("BPT环境异常"),COLOR_RED,200);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("BPT环境异常"));
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_errCode);
					SetResult(RESULT_FAIL);
					return 0;
				}
				m_pInterface->Ctrl_Addlog(CamID,_T("BPT测试完成，卡控开始"),COLOR_BLUE,200);
			}
			LeaveCriticalSection(&m_csLock);
			if (pDlg->m_SavePic==TRUE)
			{
				CFile cf;
				CString PathName = _T("D:\\BPTTest_ImageInput.raw");
				BOOL ret = cf.Open(PathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				if (!ret)
				{
					return 0;
				} 
				cf.Write((void*)ImageInput, img.imgwidth * img.imgheight);
				cf.Close();
			}
			
			if (ImageInput!=NULL)
			{
				delete [] ImageInput;
			}
			strLog.Format(_T("DPTCnt = %d"), m_pBpt.DefectStackCnt);
			m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
			m_badPixelCount = m_pBpt.DefectStackCnt;
			for (int i=0;i<m_pBpt.DefectStackCnt-1;i++)
			{
				for (int j=i+1;j < m_pBpt.DefectStackCnt;j++)
				{
					if (m_pBpt.BadPixelStack[i].xIndex == m_pBpt.BadPixelStack[j].xIndex && m_pBpt.BadPixelStack[i].yIndex == m_pBpt.BadPixelStack[j].yIndex)
					{
						SetResult(RESULT_FAIL);
						m_pInterface->Ctrl_Addlog(CamID,_T("BPT数据异常！请重新测试！"),COLOR_RED,200);
						m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("BPT数据异常"));
						m_pInterface->Ctrl_SetCamErrorCode(CamID, m_errCode);
						if (pDlg->m_SavePic==TRUE)
						{
							CFile file;
							CString PathName = _T("D:\\BPTTest_NG_Raw10.raw");
							BOOL ret = file.Open(PathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
							if (!ret)
							{
								return 0;
							} 
							file.Write((void*) img.Raw_buffer, img.imgwidth * img.imgheight * sizeof(USHORT) );
							file.Close();
						}						
						return 0;
					}
				}
			}
			m_pInterface->Ctrl_Addlog(CamID,_T("BPT数据OK，烧录开始"),COLOR_BLUE,200);
			USHORT bptTblBuff[8009] = {0};
			memset(bptTblBuff, 0, sizeof(USHORT)*8009);

			int bptCount = (m_pBpt.DefectStackCnt > 2000) ? 2000 : m_pBpt.DefectStackCnt;
			bptTblBuff[0] = (m_pBpt.DefectStackCnt >> 8) & 0xFF;
			bptTblBuff[1] = m_pBpt.DefectStackCnt & 0xFF;
			CString tempLog;
			for (int i=0; i<bptCount; i++)
			{
				bptTblBuff[2 +  4 * i + 0] = ((m_pBpt.BadPixelStack[i].xIndex >> 8) & 0xFF);
				bptTblBuff[2 +  4 * i + 1] = (m_pBpt.BadPixelStack[i].xIndex & 0xFF);
				bptTblBuff[2 +  4 * i + 2] = ((m_pBpt.BadPixelStack[i].yIndex >> 8) & 0xFF);
				bptTblBuff[2 +  4 * i + 3] = (m_pBpt.BadPixelStack[i].yIndex & 0xFF);
			}

			if (m_pBpt.BadPixelStack != NULL)
			{
				free(m_pBpt.BadPixelStack);
				m_pBpt.BadPixelStack = NULL;
			}

			time_t timep;
			struct tm* t;
			timep = time(NULL);
			t = localtime(&timep);			

			bptTblBuff[8002] = (1900 + t->tm_year - 2000) & 0xFF;
			bptTblBuff[8003] = (t->tm_mon + 1) & 0xFF;
			bptTblBuff[8004] = (t->tm_mday) & 0xFF;
			bptTblBuff[8005] = t->tm_hour & 0xFF;
			bptTblBuff[8006] = pDlg->m_VerInfo;
			bptTblBuff[8007] = pDlg->m_StaInfo;

			DWORD sum = 0;
			for (int i=0; i<8008; i++)
				sum += bptTblBuff[i];
			bptTblBuff[8008] = sum % 255;

			USHORT bptBurnStartAddr = 0;
			if (!swscanf(pDlg->m_bptBurnStartAddr, _T("0x%x"), &bptBurnStartAddr))
				swscanf(pDlg->m_bptBurnStartAddr, _T("%d"), &bptBurnStartAddr);
			USHORT bptBurnEndAddr = 0;
			if (!swscanf(pDlg->m_bptBurnEndAddr, _T("0x%x"), &bptBurnEndAddr))
				swscanf(pDlg->m_bptBurnEndAddr, _T("%d"), &bptBurnEndAddr);
			m_pInterface->Otp_OtpWrite(bptBurnStartAddr, bptBurnEndAddr, bptTblBuff, myCamID);
			Sleep(50);
			USHORT retBuff[8009] = {0};
			memset(retBuff, 0, sizeof(retBuff));
			m_pInterface->Otp_OtpRead(bptBurnStartAddr, bptBurnEndAddr, retBuff, myCamID);

			sum = 0;
			for (int i=0; i<8008; i++)
				sum += retBuff[i];
			int retChksum = sum % 255;

			strLog.Format(_T("Burn Chksum = 0x%X, ReCaleChksum = 0x%X, DumpChksum = 0x%X"), bptTblBuff[8008], retChksum, retBuff[8008]);
			m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
			if ((bptTblBuff[8008] != retChksum) || (bptTblBuff[8008] != retBuff[8008]))
			{
				m_pInterface->Ctrl_Addlog(CamID, _T("DPT Chksum Failed."), COLOR_RED, 200);				
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_bptErrInfo);
				m_pInterface->Ctrl_SetCamErrorCode(CamID, m_errCode);
				return 0;
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID, _T("DPT Chksum Pass ."), COLOR_GREEN, 200);
				SetResult(RESULT_PASS);
				return 0;
			}
			m_pInterface->Ctrl_Addlog(CamID,_T("BPT数据OK，烧录开始"),COLOR_GREEN,200);
			

			m_frmDelayCnt = 0;
			m_tstMode = _T("BPT_TEST_FINISHED");
		}
	}

	return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int HwBPTTest_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	DeleteCriticalSection(&m_csLock);
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int HwBPTTest_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码
	pDlg->m_ExValue = ReadConfigString(_T("BPT_EXPOSURE_TIME"), _T("0x0000"));
	pDlg->m_GainValue = ReadConfigString(_T("BPT_EXPOSURE_GAIN"), _T("0x0000"));

	 pDlg->m_VerInfo = ReadConfigInt(L"VerInfo",0);
	 pDlg->m_StaInfo = ReadConfigInt(L"StaInfo",0);
	 
	 pDlg->m_bptErrCode = ReadConfigString(_T("BPT_ERROR_CODE"), _T("16"));
	 pDlg->m_bptErrInfo = ReadConfigString(_T("BPT_ERROR_INFO"), _T("Test Failed"));
	 pDlg->m_bptFrmDelayNum = ReadConfigString(_T("BPT_FRAME_DELAY_NUMBER"), _T("5"));

	 pDlg->m_bptBurnStartAddr = ReadConfigString(_T("BPT_BURN_START_ADDR"), _T("0x0000"));
	 pDlg->m_bptBurnEndAddr = ReadConfigString(_T("BPT_BURN_END_ADDR"), _T("0x0000"));

	 pDlg->m_SensorSelection = ReadConfigInt(_T("SensorSelection"),0);
	 pDlg->m_SavePic = ReadConfigInt(_T("SavePicture"),0);
	 pDlg->m_SaveData = ReadConfigInt(_T("SaveData"),0);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int HwBPTTest_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	    
	WriteConfigString(_T("BPT_EXPOSURE_TIME"), pDlg->m_ExValue);
	WriteConfigString(_T("BPT_EXPOSURE_GAIN"), pDlg->m_GainValue);

	 WriteConfigInt(L"StaInfo",pDlg->m_StaInfo);
	 WriteConfigInt(L"VerInfo",pDlg->m_VerInfo);

	 WriteConfigString(_T("BPT_ERROR_CODE"), pDlg->m_bptErrCode);
	 WriteConfigString(_T("BPT_ERROR_INFO"), pDlg->m_bptErrInfo);
	 WriteConfigString(_T("BPT_FRAME_DELAY_NUMBER"), pDlg->m_bptFrmDelayNum);

	 WriteConfigString(_T("BPT_BURN_START_ADDR"), pDlg->m_bptBurnStartAddr);
	 WriteConfigString(_T("BPT_BURN_END_ADDR"), pDlg->m_bptBurnEndAddr);

	 WriteConfigInt(_T("SensorSelection"),pDlg->m_SensorSelection);
	 WriteConfigInt(_T("SavePicture"),pDlg->m_SavePic);

	 WriteConfigInt(_T("SaveData"),pDlg->m_SaveData);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR HwBPTTest_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("BadPixel Count");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR HwBPTTest_TestItem::GetReportLowLimit()
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
LPCTSTR HwBPTTest_TestItem::GetReportHightLimit()
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
LPCTSTR HwBPTTest_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%d"),m_badPixelCount);
	return Content;
}

int HwBPTTest_TestItem::Sum()
{ 
	return 0;
}

CString HwBPTTest_TestItem::GetSensorFuseID()
{
	USHORT Value[9];
	m_pInterface->Otp_OtpRead(0x0006,0x000e,Value,CamID,0);
	CString str;
	CString mean;
	for (int i=0; i<9; i++)
	{
		str.Format(L"%02x",Value[i]);
		mean += str;
	}
	mean.MakeUpper();
	return mean;
}

void HwBPTTest_TestItem::WideToChar(CString wide,char* Char)
{
	//////////////////////////////////////////////////////////////////////////
	CStringA tempA;
	tempA=CT2A(wide);
	strcpy(Char,tempA.GetBuffer(tempA.GetLength()));
	/*
	//计算需要多少个字节才能表示对应的多字节字符串  
	DWORD num=WideCharToMultiByte(CP_ACP,0,wide,-1,NULL,0,NULL,0);  

	//开辟空间 
	char *pChar;
	pChar=(char*)malloc(num*sizeof(char));  
	memset(pChar,0,num*sizeof(char));  

	//将宽字节字符串转换为多字节字符串  
	WideCharToMultiByte(CP_ACP,0,wide,-1,pChar,num,NULL,0);  
	memcpy(Char,pChar,sizeof(unsigned char)*num);
	if (pChar!=NULL)
	{
		free(pChar);
	}
	*/
}
 
void HwBPTTest_TestItem::SaveOTPdata(USHORT*m_OTPdata)
{
	// TODO: 在此添加控件通知处理程序代码
	CTime t = CTime::GetCurrentTime();
	CString strtemp = L"";
	CString FuseID = L"";
	char wPath[512] = {0};
	strtemp.Format(L">readOTP start:%02d:%02d\n", t.GetMinute(), t.GetSecond());

	CString  path= L"D:\\";
	path = path + L"\\Data\\";
	WideToChar(path, wPath);
	_mkdir(wPath);
	if (b12m)
		path = path + L"\\IMX386\\";
	else
		path = path + L"\\HI259\\";
	WideToChar(path, wPath);
	_mkdir(wPath);

	FuseID = GetSensorFuseID();
	path += FuseID;
	path += ".ini";
	WideToChar(path, wPath);
	FILE *fp;
	fp = fopen(wPath, "w");
	fclose(fp);

	CStdioFile MyFile(path, CFile::modeWrite | CFile::typeText); 
	USHORT otpdata[10000] = {0};
	USHORT StartAdr, EndAdr;
	if(b12m)
	{
		StartAdr = 0x0000;
		EndAdr = 0x1d63;	
	}
	else
	{
		StartAdr = 0x0000;
		EndAdr = 0x1478;
	}
	for(int i=StartAdr;i<=EndAdr;i++)
	{
		strtemp.Format(L"0x%04x\t0x%02x\n",i,m_OTPdata[i-StartAdr]);
		MyFile.WriteString(strtemp);	   
	}
	t=CTime::GetCurrentTime();
}
 
void HwBPTTest_TestItem::RAW10_To_BYTE(USHORT *Pin,BYTE *Pout,int ImageWidth,int ImageHeight)
{
	for (DWORD i=0; i<ImageWidth*ImageHeight; i++)
	{
		Pout[2*i] = Pin[i] & 0xFF;
		Pout[2*i+1] = (Pin[i]>>8)&0xFF;
	}
}

void HwBPTTest_TestItem::Raw10toRaw8(USHORT *Pin,BYTE *Raw8out,int ImageWidth,int ImageHeight)
{
	
	for (DWORD i=0; i<ImageWidth*ImageHeight; i++)
	{
		if (Pin[i] > 255)
		{
			Pin[i] = 255;
		}

		Raw8out[i] = Pin[i] & 0xFF;
	}
}