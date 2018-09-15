#include "StdAfx.h"
#include "OF_RI_RU_test.h"
#include "RI_TestItem.h"
#include "ImageProc.h"

#pragma comment(lib, "Moto_RI_RU_Test.lib")

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

static RIValue pRIValue;

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new RI_TestItem(pInterface, lpszName);
}
BOOL bSaveRawFile( char* sfilename, BYTE *pBuffer, DWORD BufSize)
{	
	if (BufSize <= 0)
	{
		return FALSE;
	}

	HFILE rawFile;
	DWORD dwSizeImage = BufSize;

	rawFile = _lcreat( sfilename, 0 );
	if ( rawFile < 0 )
	{
		return FALSE;
	}

	UINT len;
	len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );

	_lclose( rawFile );

	return TRUE;	
}

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
RI_TestItem::RI_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	m_cfgName = CString(lpszName);
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化
	int i;
	for (i = 0; i < RI_TST_PIC_NUM; i++)
	{
		m_pBuf[i] = NULL;
	}
	m_CurPicInx = 0;
	//.....
}

RI_TestItem::~RI_TestItem(void)
{
	int i;
	for (i = 0; i < RI_TST_PIC_NUM; i++)
	{
		if (NULL != m_pBuf[i])
		{
			delete []m_pBuf[i];
			m_pBuf[i] = NULL;
		}
	}
	m_CurPicInx = 0;
}


int RI_TestItem::InitItem()
{
	CamID = GetCamID();
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径
	LoadOption();

	m_CurPicInx = 0;

	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int RI_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
 
	m_CurPicInx = 0;

	return 0;
}

/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int RI_TestItem::Testing()
{
   //TODO:在此添加测试项代码

	/** Step 1:获取图像大小，以便下面准备存储缓存 **/
	img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("RI_TestItem::Testing()---抓帧1失败!"), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RI TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	int i;
	long ImgLen = img.imgwidth * img.imgheight;
	if (m_CurPicInx < RI_TST_PIC_NUM)
	{
		m_pBuf[m_CurPicInx] = new USHORT[ImgLen];
		if (NULL == m_pBuf[m_CurPicInx])
		{
			int j;
			for (j = 0; j < m_CurPicInx; j++)
			{
				if (NULL != m_pBuf[j])
				{
					delete []m_pBuf[j];
					m_pBuf[j] = NULL;
				}
			}

			m_pInterface->Ctrl_Addlog(CamID,_T("RI_TestItem::Testing()---m_pBuf申请失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RI TEST FAILED!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		if (NULL == img.Raw_buffer)
		{
			int j;
			for (j = 0; j < m_CurPicInx; j++)
			{
				if (NULL != m_pBuf[j])
				{
					delete []m_pBuf[j];
					m_pBuf[j] = NULL;
				}
			}

			m_pInterface->Ctrl_Addlog(CamID, _T("RI_TestItem::Testing()---Ctrl_GetImgInfo() Fail"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RI TEST FAILED!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		memcpy(m_pBuf[m_CurPicInx], img.Raw_buffer, ImgLen * 2);	//注意：Raw_buffer是USHORT类型数据，所以拷贝时需要乘以2

	#if RI_TEST_OUTPUT_INFO == 1
		OutputDebugInfo(0, m_CurPicInx);
	#endif

		m_CurPicInx++;
		return 1;//Sleep(200);	//留时间让抓图线程抓一张新图
	}

	LPBYTE pBy = NULL;//注意：指针定义为far类型，处理X86指针默认情况下只能处理段内偏移地址而不能处理段地址导致一个指针只能寻址64KB地址空间问题--Added by LiHai--20171227
	//5张图片获取成功，计算5张图片各通道的平均值
	for (i = 0; i < ImgLen; i++)
	{
		int j;
		long Tmp;

	#if RI_TEST_OUTPUT_INFO == 1
		OutputDebugInfo(1, i);
	#endif

		Tmp = 0;
		for (j = 0; j < RI_TST_PIC_NUM; j++)
		{
			Tmp += *(m_pBuf[j] + i);
		}
		
#if 0
		*(m_pBuf[0] + i) = (USHORT)((Tmp + 0.5) / RI_TST_PIC_NUM);
#else
		Tmp = (USHORT)((Tmp + 0.5) / RI_TST_PIC_NUM);
		if (Tmp < BLC_RAW10)
		{
			Tmp = 0;
		}
		else//原始老架构代码中，RU测试代码里面此处相关代码没有这个【else】语句，感觉是错误的，要不然当原始数据小于BLC_RAW10，不就成负数了？--Added by LiHai--20171226
		{
			Tmp -= BLC_RAW10;
		}

		pBy = (LPBYTE)(m_pBuf[0] + i);	//平均值借用m_pBuf[0]存储，并且是存储为下面RI计算函数所需要的数据格式--Added by LiHai--20171226
		pBy[0] = LOBYTE(Tmp);
		pBy[1] = HIBYTE(Tmp);

	#if RI_TEST_OUTPUT_INFO == 1
		OutputDebugInfo(2, i);
	#endif
#endif
	}

#if RI_TEST_OUTPUT_INFO == 1
	OutputDebugInfo(3, i);
#endif

	/**
	*	RI测试函数的数据结构:
	*		R[7：0]R[9:8]Gr[7：0]Gr[9:8]Gb[7：0]Gb[9:8]B[7：0]B[9:8]...
	*	--Added by LiHai--20171225
	**/
#if RI_TEST_BY_READ_RIPIC_BIN == 1
	RiTstByReadRiPicBin(pRIValue);
#else
	RelativeIlluminationTest((unsigned char *)(m_pBuf[0]), img.imgwidth, img.imgheight, img.RawFormat, &pRIValue);
#endif

	//输出Relative Illumination值值
	CString strLog;
	BOOL m_RIResult = TRUE;
	if (pRIValue.RI_RChannel < pDlg->m_RiMinSpec.RI_Min_R)
	{
		strLog.Format(_T("R_RI = %.2f%% < %.2f%% Percent\r\n"), pRIValue.RI_RChannel, pDlg->m_RiMinSpec.RI_Min_R);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RIResult = FALSE;
	}
	else
	{
		strLog.Format(_T("R_RI = %.2f%% >= %.2f%% Percent\r\n"), pRIValue.RI_RChannel, pDlg->m_RiMinSpec.RI_Min_R);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRIValue.RI_GrChannel < pDlg->m_RiMinSpec.RI_Min_Gr)
	{
		strLog.Format(_T("Gr_RI= %.2f%% < %.2f%% Percent\r\n"), pRIValue.RI_GrChannel, pDlg->m_RiMinSpec.RI_Min_Gr);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RIResult = FALSE;
	}
	else
	{
		strLog.Format(_T("Gr_RI= %.2f >= %.2f Percent\r\n"), pRIValue.RI_GrChannel, pDlg->m_RiMinSpec.RI_Min_Gr);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRIValue.RI_GbChannel < pDlg->m_RiMinSpec.RI_Min_Gb)
	{
		strLog.Format(_T("Gb_RI= %.2f%% < %.2f%% Percent\r\n"), pRIValue.RI_GbChannel, pDlg->m_RiMinSpec.RI_Min_Gb);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RIResult = FALSE;
	}
	else
	{
		strLog.Format(_T("Gb_RI= %.2f%% >= %.2f%% Percent\r\n"), pRIValue.RI_GbChannel, pDlg->m_RiMinSpec.RI_Min_Gb);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRIValue.RI_BChannel < pDlg->m_RiMinSpec.RI_Min_B)
	{
		strLog.Format(_T("B_RI = %.2f%% < %.2f%% Percent\r\n"), pRIValue.RI_BChannel, pDlg->m_RiMinSpec.RI_Min_B);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RIResult = FALSE;
	}
	else
	{
		strLog.Format(_T("B_RI = %.2f%% >= %.2f%% Percent\r\n"), pRIValue.RI_BChannel, pDlg->m_RiMinSpec.RI_Min_B);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	/** 老架构在此处保存数据到excel文件中，此处可以考虑是否保存--Added by LiHai--20171226 **/
	//SaveRIData(pRIValue, m_RIResult);

	for (i = 0; i < RI_TST_PIC_NUM; i++)
	{
		if (NULL != m_pBuf[i])
		{
			delete []m_pBuf[i];
			m_pBuf[i] = NULL;
		}
	}
	
	if (m_RIResult != TRUE)
	{
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RI TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	SetResult(RESULT_PASS);
	m_pInterface->Ctrl_Addlog(CamID, _T("RI test PASS!\r\n"), COLOR_BLUE, 200);
	return 0;
}

/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int RI_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码

	int j;
	for (j = 0; j < m_CurPicInx; j++)
	{
		if (NULL != m_pBuf[j])
		{
			delete []m_pBuf[j];
			m_pBuf[j] = NULL;
		}
	}
	m_CurPicInx = 0;

	return 0;
}

/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int RI_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	CString str;
	str = ReadConfigString(_T("RI_Min_R"), _T("0"));
	pDlg->m_RiMinSpec.RI_Min_R = _tstof(str);//tempnum =atof(T2A(a));
	str = ReadConfigString(_T("RI_Min_Gr"), _T("0"));
	pDlg->m_RiMinSpec.RI_Min_Gr = _tstof(str);
	str = ReadConfigString(_T("RI_Min_Gb"), _T("0"));
	pDlg->m_RiMinSpec.RI_Min_Gb = _tstof(str);
	str = ReadConfigString(_T("RI_Min_B"), _T("0"));
	pDlg->m_RiMinSpec.RI_Min_B = _tstof(str);

	//........................
	return TestItemInterface::LoadOption();
}

/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int RI_TestItem::SaveOption()
{
	if (!TestItemInterface::SaveOption())
	{
		return -1;
	}

	//TODO: 在此添加存储代码
	CString str;
	str.Format(_T("%f"), pDlg->m_RiMinSpec.RI_Min_R);
	WriteConfigString(_T("RI_Min_R"), str.GetBuffer(str.GetLength()));
	str.Format(_T("%f"), pDlg->m_RiMinSpec.RI_Min_Gr);
	WriteConfigString(_T("RI_Min_Gr"), str.GetBuffer(str.GetLength()));
	str.Format(_T("%f"), pDlg->m_RiMinSpec.RI_Min_Gb);
	WriteConfigString(_T("RI_Min_Gb"), str.GetBuffer(str.GetLength()));
	str.Format(_T("%f"), pDlg->m_RiMinSpec.RI_Min_B);
	WriteConfigString(_T("RI_Min_B"), str.GetBuffer(str.GetLength()));
	//........................
	return 0;
}

CString RI_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}

#if RI_TEST_OUTPUT_INFO == 1
void RI_TestItem::OutputDebugInfo(BYTE InfoType, int i)
{
	long ImgLen = img.imgwidth * img.imgheight;
	switch (InfoType)
	{
	case 0:
		{
			/**
			*	注意指针，当定义普通[char *]类指针时，X86架构导致只能访问段内偏移地址，而不能
			*	访问段地址，所以最大是64K地址范围，超过部分需要加[far]关键字。
			*	以下打印就是输出[首地址]、[64K地址]、[图片最后地址]三个地方的数据，每个地方32个像素。
			*	用于调试时对比数据使用。
			*	--Added by LiHai--20171227
			**/
			int j;
			for (j = 0; j < 32; j++)
			{
				CString strLog;
				strLog.Format(_T("GetImg--j:%2d--0x%02X 0x%02X    0x%02X 0x%02X    0x%02X 0x%02X\r\n"), j, *(m_pBuf[0] + j), img.Raw_buffer[j], *(m_pBuf[0] + j + 0X10000), img.Raw_buffer[j + 0X10000], *(m_pBuf[0] + j + ImgLen - 32), img.Raw_buffer[j + ImgLen - 32]);
				m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
			}
		}
		break;

	case 1:
		{
			if (i < 32)
			{
				CString strLog;
				strLog.Format(_T("GetAve--0x%02X"), *(m_pBuf[0] + i));
				m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
			}
		}
		break;

	case 2:
		{
			if (i < 32)
			{
				CString strLog;
				strLog.Format(_T("Ave--0x%02X\r\n"), *(m_pBuf[0] + i));
				m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
			}
		}
		break;

	case 3:
		{
			LPBYTE pBy = (LPBYTE)(m_pBuf[0]);
			for (i = 0; i < 64; i++)
			{
				if (i % 16 == 15)
				{
					CString strLog;
					strLog.Format(_T("XX 0x%04X\r\n"), pBy[i]);
					m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
				}
				else
				{
					CString strLog;
					strLog.Format(_T("XX 0x%04X\r\n"), pBy[i]);
					m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
				}
			}

			for (i = 0; i < 64; i++)
			{
				if (i % 16 == 15)
				{
					CString strLog;
					strLog.Format(_T("YY 0x%04X\r\n"), pBy[i + 2 * 0X10000]);
					m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
				}
				else
				{
					CString strLog;
					strLog.Format(_T("YY 0x%04X\r\n"), pBy[i + 2 * 0X10000]);
					m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
				}
			}

			for (i = 0; i < 64; i++)
			{
				if (i % 16 == 15)
				{
					CString strLog;
					strLog.Format(_T("ZZ 0x%04X\r\n"), pBy[i + 2 * ImgLen - 64]);
					m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
				}
				else
				{
					CString strLog;
					strLog.Format(_T("ZZ 0x%04X\r\n"), pBy[i + 2 * ImgLen - 64]);
					m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
				}
			}
		}
		break;

	default:
		break;
	}
}
#endif

#if RI_TEST_BY_READ_RIPIC_BIN == 1
void RI_TestItem::RiTstByReadRiPicBin(struct RIValue &pRIValue)
{
	int i;
	LPBYTE pRiPicData = NULL;
	FILE  *fp  = NULL;
	CString path;

	path = _T("F:\\L8856F80_OTP\\L8856F80_OTP\\Debug\\RI_PIC.bin");

	fp = _wfopen(path,_T("rb"));
	if(fp == NULL)  
	{
		//AddString(RED,"open file fail!");
		return;
	}

	int ImgLen = 2 * img.imgwidth * img.imgheight;
	//pRiPicData = new unsigned char[ImgLen];
	pRiPicData = (LPBYTE)malloc(ImgLen);
	if (NULL == pRiPicData)
	{
		fclose(fp);
		//AddString(RED,"new pRuPicData error\r\n");
		return;
	}

	fseek( fp, 0, SEEK_SET);
	int readnum = fread((void*)pRiPicData, ImgLen, 1, fp);
	for (i = 0; i < 32; i++)
	{
		if (i % 16 == 15)
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRiPicData[i]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
		else
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRiPicData[i]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
	}

	for (i = 0; i < 32; i++)
	{
		if (i % 16 == 15)
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRiPicData[i + 0XFFFF + 1]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
		else
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRiPicData[i + 0XFFFF + 1]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
	}

	for (i = 0; i < 32; i++)
	{
		if (i % 16 == 15)
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRiPicData[i + ImgLen - 32]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
		else
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRiPicData[i + ImgLen - 32]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
	}

	fclose(fp);

	RelativeIlluminationTest(pRiPicData, img.imgwidth, img.imgheight, img.RawFormat, &pRIValue);
	//delete []pRiPicData;
	free(pRiPicData);
}
#endif

/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR RI_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("RI_R,RI_Gr,RI_Gb,RI_B,Result");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR RI_TestItem::GetReportLowLimit()
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
LPCTSTR RI_TestItem::GetReportHightLimit()
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
LPCTSTR RI_TestItem::GetReportContents()
{
	static CString Content;
	Content.Format(_T("%.2f%%,%.2f%%,%.2f%%,%.2f%%,"), pRIValue.RI_RChannel, pRIValue.RI_GrChannel, pRIValue.RI_GbChannel, pRIValue.RI_BChannel);
	if (RESULT_PASS == GetResult())
	{
		Content += _T("PASS");
	}
	else
	{
		Content += _T("NG");
	}

	return Content;
}