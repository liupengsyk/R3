#include "StdAfx.h"
#include "SRNTest_TestItem.h"
#include "ImageProc.h"
#include "oSRNtest.h"
#include "Lib_Halcon\include\Halcon.h"

#pragma comment(lib,"oSRNtest.lib")


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SRNTest_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
SRNTest_TestItem::SRNTest_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

BOOL SRNTest_TestItem::SetProperGain(void)
{
	CString strLog = _T("");

	int defExpoTime = 0;
	if (!swscanf(pDlg->m_srnPara.exposureTime, _T("0x%x"), &defExpoTime))
		swscanf(pDlg->m_srnPara.exposureTime, _T("%d"), &defExpoTime);
	USHORT defGain = 0;
	if (!swscanf(pDlg->m_srnPara.analogGain, _T("0x%x"), &defGain))
		swscanf(pDlg->m_srnPara.analogGain, _T("%d"), &defGain);
	m_pInterface->Sensor_WriteExp(CamID, defExpoTime);
	m_pInterface->Sensor_WriteGain(CamID, defGain);

	m_pInterface->Sensor_ReadExp(CamID, defExpoTime);
	m_pInterface->Sensor_ReadGain(CamID, defGain);
	strLog.Format(_T("Write Exp = 0x%X, Gain = 0x%X"), defExpoTime, defGain);
	m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

	return 0;
}

SRNTest_TestItem::~SRNTest_TestItem(void)
{

}


int SRNTest_TestItem::InitItem()
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
int SRNTest_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
	if (!swscanf(pDlg->m_srnPara.errCode, _T("0x%x"), &m_errCode))
		swscanf(pDlg->m_srnPara.errCode, _T("%d"), &m_errCode);

	if (!swscanf(pDlg->m_srnPara.frmDelayCnt, _T("0x%x"), &m_frmDelayNum))
		swscanf(pDlg->m_srnPara.frmDelayCnt, _T("%d"), &m_frmDelayNum);

	m_pInterface->Sensor_ReadExp(CamID, m_initExpoTime);
	m_pInterface->Sensor_ReadGain(CamID, m_initGain);
	CString strLog = _T("");
	strLog.Format(_T("Init Exp = 0x%X, Gain = 0x%X"), m_initExpoTime, m_initGain);
	m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

	m_pRawBuff0 = NULL;
	m_pRawBuff1 = NULL;
	m_pRawBuff2 = NULL;
	m_pRawBuff3 = NULL;

	m_frmDelayCnt = 0;
	m_tstMode = _T("None");
	m_captureCnt = 0;

	return 0;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SRNTest_TestItem::Testing()
{
	//TODO:在此添加测试项代码
	CString strLog = _T("");

	if (m_tstMode == _T("None"))
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("Set SRN Test Start."), COLOR_BLUE, 200);
		SetProperGain();

		m_frmDelayCnt = 0;
		m_captureCnt = 0;
		m_tstMode = _T("SRN_CAPTURE_RAW_IMAGE");
	}

	if (m_tstMode == _T("SRN_CAPTURE_RAW_IMAGE"))
	{
		if (m_frmDelayCnt < m_frmDelayNum)
		{
			//strLog.Format(_T("Delay Frame = %d"), m_frmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
			Sleep(300);
			m_frmDelayCnt++;
			return 1;
		}
		else
		{
#if 0
			m_img = *m_pInterface->Ctrl_GetImgInfo(CamID);
			int imgWidth = m_img.imgwidth;
			int imgHeight = m_img.imgheight;
			DWORD size = imgWidth * imgHeight * 2;
			if (m_captureCnt == 0)
			{
				m_pInterface->Ctrl_Addlog(CamID, _T("Caputre Image 0"), COLOR_BLUE, 200);
				m_pRawBuff0 = new BYTE[size];
				memset(m_pRawBuff0, 0, size);
				Raw10toP10(m_img.Raw_buffer, m_pRawBuff0, imgHeight, imgWidth);
			}
			else if (m_captureCnt == 1)
			{
				m_pInterface->Ctrl_Addlog(CamID, _T("Caputre Image 1"), COLOR_BLUE, 200);
				m_pRawBuff1 = new BYTE[size];
				memset(m_pRawBuff1, 0, size);
				Raw10toP10(m_img.Raw_buffer, m_pRawBuff1, imgHeight, imgWidth);
			}
			else if (m_captureCnt == 2)
			{
				m_pInterface->Ctrl_Addlog(CamID, _T("Caputre Image 2"), COLOR_BLUE, 200);
				m_pRawBuff2 = new BYTE[size];
				memset(m_pRawBuff2, 0, size);
				Raw10toP10(m_img.Raw_buffer, m_pRawBuff2, imgHeight, imgWidth);
			}
			else if (m_captureCnt == 3)
			{
				m_pInterface->Ctrl_Addlog(CamID, _T("Caputre Image 3"), COLOR_BLUE, 200);
				m_pRawBuff3 = new BYTE[size];
				memset(m_pRawBuff3, 0, size);
				Raw10toP10(m_img.Raw_buffer, m_pRawBuff3, imgHeight, imgWidth);

				m_frmDelayCnt = 0;
				m_captureCnt = 0;
				m_tstMode = _T("SRN_TEST");
			}

			m_captureCnt++;

			m_frmDelayCnt = 0;
			return 1;
#else
			m_frmDelayCnt = 0;
			m_captureCnt = 0;
			m_tstMode = _T("SRN_TEST");
#endif
		}
	}

	if (m_tstMode == _T("SRN_TEST"))
	{
		m_img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		int imgWidth = m_img.imgwidth;
		int imgHeight = m_img.imgheight;
#if 0
		m_PPmax = RowNoiseTest(m_pRawBuff0, m_pRawBuff1, m_pRawBuff2, m_pRawBuff3, imgWidth, imgHeight, 64) * 10.0f;
		float specPPMax = (float)_wtof(pDlg->m_srnPara.specPPmax);
		strLog.Format(_T("PPmax = %f(x10), Limit = %f(x10)"), m_PPmax, specPPMax);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
		if (m_PPmax > specPPMax)
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("PPmax Over Range!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_srnPara.errInfo);
			m_pInterface->Ctrl_SetCamErrorCode(CamID, m_errCode);
			SetResult(RESULT_FAIL);

			m_pInterface->Sensor_WriteExp(CamID, m_initExpoTime);
			m_pInterface->Sensor_WriteGain(CamID, m_initGain);

			int exp = 0;
			m_pInterface->Sensor_ReadExp(CamID, exp);
			USHORT gain = 0;
			m_pInterface->Sensor_ReadGain(CamID, gain);
			if ((exp == m_initExpoTime) && (gain == m_initGain))
				m_pInterface->Ctrl_Addlog(CamID, _T("Gain reinit ！"), COLOR_BLUE, 200);

			return 0;
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("SRN Test End PASS"), COLOR_GREEN, 200);
			SetResult(RESULT_PASS);

			m_pInterface->Sensor_WriteExp(CamID, m_initExpoTime);
			m_pInterface->Sensor_WriteGain(CamID, m_initGain);

			int exp = 0;
			m_pInterface->Sensor_ReadExp(CamID, exp);
			USHORT gain = 0;
			m_pInterface->Sensor_ReadGain(CamID, gain);
			if ((exp == m_initExpoTime) && (gain == m_initGain))
				m_pInterface->Ctrl_Addlog(CamID, _T("Gain reinit ！"), COLOR_BLUE, 200);

			return 0;
		}
#else
		using namespace Halcon;

		Hobject  Image, ImageEmphasize;
		BYTE* TestYbuf = new BYTE[m_img.imgwidth * m_img.imgheight];
		ImageProc::RGB24toY(m_img.RGB24_buffer, TestYbuf, m_img.imgwidth, m_img.imgheight);
		gen_image1(&Image, "byte", imgWidth, imgHeight, (long)TestYbuf);
		HTuple Mean, Deviation;
		emphasize(Image, &ImageEmphasize, HTuple(m_img.imgwidth), HTuple(m_img.imgheight), HTuple(50));
		intensity(ImageEmphasize, ImageEmphasize, &Mean, &Deviation);
		write_image(ImageEmphasize, "bmp", 0, "D:/SRNTest.bmp");
		delete[] TestYbuf;
		Image.~Hobject();
		ImageEmphasize.~Hobject();

		m_PPmax = Deviation[0].D();
		float specPPMax = (float)_wtof(pDlg->m_srnPara.specPPmax);
		strLog.Format(_T("PPmax = %.4f(x10), Limit = %f(x10)"), m_PPmax, specPPMax);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
		if (m_PPmax > specPPMax)
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("PPmax Over Range!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_srnPara.errInfo);
			m_pInterface->Ctrl_SetCamErrorCode(CamID, m_errCode);
			SetResult(RESULT_FAIL);

			m_pInterface->Sensor_WriteExp(CamID, m_initExpoTime);
			m_pInterface->Sensor_WriteGain(CamID, m_initGain);

			int exp = 0;
			m_pInterface->Sensor_ReadExp(CamID, exp);
			USHORT gain = 0;
			m_pInterface->Sensor_ReadGain(CamID, gain);
			if ((exp == m_initExpoTime) && (gain == m_initGain))
				m_pInterface->Ctrl_Addlog(CamID, _T("Gain reinit ！"), COLOR_BLUE, 200);

			return 0;
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("SRN Test End PASS"), COLOR_GREEN, 200);
			SetResult(RESULT_PASS);

			m_pInterface->Sensor_WriteExp(CamID, m_initExpoTime);
			m_pInterface->Sensor_WriteGain(CamID, m_initGain);

			int exp = 0;
			m_pInterface->Sensor_ReadExp(CamID, exp);
			USHORT gain = 0;
			m_pInterface->Sensor_ReadGain(CamID, gain);
			if ((exp == m_initExpoTime) && (gain == m_initGain))
				m_pInterface->Ctrl_Addlog(CamID, _T("Gain reinit ！"), COLOR_BLUE, 200);

			return 0;
		}
#endif
	}

	return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SRNTest_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	if (m_pRawBuff0!=NULL)
	{
		delete []m_pRawBuff0;
		m_pRawBuff0 = NULL;
	}

	if (m_pRawBuff1 != NULL)
	{
		delete []m_pRawBuff1;
		m_pRawBuff1 = NULL;
	}

	if (m_pRawBuff2 != NULL)
	{
		delete []m_pRawBuff2;
		m_pRawBuff2 = NULL;
	}

	if (m_pRawBuff3 != NULL)
	{
		delete [] m_pRawBuff3;
		m_pRawBuff3 = NULL;
	}
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SRNTest_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_srnPara.errCode = ReadConfigString(_T("SRN_ERROR_CODE"), _T("16"));
	pDlg->m_srnPara.errInfo = ReadConfigString(_T("SRN_ERROR_INFO"), _T("Test Failed"));
	pDlg->m_srnPara.specPPmax = ReadConfigString(_T("SRN_ERROR_SPEC_PPMAX"), _T("30"));
	pDlg->m_srnPara.frmDelayCnt = ReadConfigString(_T("SRN_ERROR_FRAME_DELAY_COUNT"), _T("5"));
	pDlg->m_srnPara.analogGain = ReadConfigString(_T("SRN_ERROR_ANALOG_GAIN"), _T("0x00"));
	pDlg->m_srnPara.exposureTime = ReadConfigString(_T("SRN_ERROR_EXPOSURE_TIME"), _T("0x00"));
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SRNTest_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigString(_T("SRN_ERROR_CODE"), pDlg->m_srnPara.errCode);
	WriteConfigString(_T("SRN_ERROR_INFO"), pDlg->m_srnPara.errInfo);
	WriteConfigString(_T("SRN_ERROR_SPEC_PPMAX"), pDlg->m_srnPara.specPPmax);
	WriteConfigString(_T("SRN_ERROR_FRAME_DELAY_COUNT"), pDlg->m_srnPara.frmDelayCnt);
	WriteConfigString(_T("SRN_ERROR_ANALOG_GAIN"), pDlg->m_srnPara.analogGain);
	WriteConfigString(_T("SRN_ERROR_EXPOSURE_TIME"), pDlg->m_srnPara.exposureTime);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR SRNTest_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = L"PPmax";
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR SRNTest_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	LowerLimit.Format(_T("%d"), 0);
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR SRNTest_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	UpperLimit.Format(_T("%s"), pDlg->m_srnPara.specPPmax);
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR SRNTest_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%f"), m_PPmax);
	return Content;
}

void SRNTest_TestItem::Raw10toP10(USHORT* pIn, BYTE* pOut, int Height, int Width)
{
	for (int i=0; i<Width * Height; i++)
	{
		pOut[2*i] = pIn[i] & 0xff;
		pOut[2*i+1] = (pIn[i] >> 8);
	}
}

