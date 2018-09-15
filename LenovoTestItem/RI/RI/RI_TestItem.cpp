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
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
RI_TestItem::RI_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��
	LoadOption();

	m_CurPicInx = 0;

	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RI_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:�ڴ���ӳ�ʼ������
 
	m_CurPicInx = 0;

	return 0;
}

/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RI_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������

	/** Step 1:��ȡͼ���С���Ա�����׼���洢���� **/
	img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("RI_TestItem::Testing()---ץ֡1ʧ��!"), COLOR_RED, 200);
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

			m_pInterface->Ctrl_Addlog(CamID,_T("RI_TestItem::Testing()---m_pBuf����ʧ��!"), COLOR_RED, 200);
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

		memcpy(m_pBuf[m_CurPicInx], img.Raw_buffer, ImgLen * 2);	//ע�⣺Raw_buffer��USHORT�������ݣ����Կ���ʱ��Ҫ����2

	#if RI_TEST_OUTPUT_INFO == 1
		OutputDebugInfo(0, m_CurPicInx);
	#endif

		m_CurPicInx++;
		return 1;//Sleep(200);	//��ʱ����ץͼ�߳�ץһ����ͼ
	}

	LPBYTE pBy = NULL;//ע�⣺ָ�붨��Ϊfar���ͣ�����X86ָ��Ĭ�������ֻ�ܴ������ƫ�Ƶ�ַ�����ܴ���ε�ַ����һ��ָ��ֻ��Ѱַ64KB��ַ�ռ�����--Added by LiHai--20171227
	//5��ͼƬ��ȡ�ɹ�������5��ͼƬ��ͨ����ƽ��ֵ
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
		else//ԭʼ�ϼܹ������У�RU���Դ�������˴���ش���û�������else����䣬�о��Ǵ���ģ�Ҫ��Ȼ��ԭʼ����С��BLC_RAW10�����ͳɸ����ˣ�--Added by LiHai--20171226
		{
			Tmp -= BLC_RAW10;
		}

		pBy = (LPBYTE)(m_pBuf[0] + i);	//ƽ��ֵ����m_pBuf[0]�洢�������Ǵ洢Ϊ����RI���㺯������Ҫ�����ݸ�ʽ--Added by LiHai--20171226
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
	*	RI���Ժ��������ݽṹ:
	*		R[7��0]R[9:8]Gr[7��0]Gr[9:8]Gb[7��0]Gb[9:8]B[7��0]B[9:8]...
	*	--Added by LiHai--20171225
	**/
#if RI_TEST_BY_READ_RIPIC_BIN == 1
	RiTstByReadRiPicBin(pRIValue);
#else
	RelativeIlluminationTest((unsigned char *)(m_pBuf[0]), img.imgwidth, img.imgheight, img.RawFormat, &pRIValue);
#endif

	//���Relative Illuminationֵֵ
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

	/** �ϼܹ��ڴ˴��������ݵ�excel�ļ��У��˴����Կ����Ƿ񱣴�--Added by LiHai--20171226 **/
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
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RI_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������

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
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int RI_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
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
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int RI_TestItem::SaveOption()
{
	if (!TestItemInterface::SaveOption())
	{
		return -1;
	}

	//TODO: �ڴ���Ӵ洢����
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
			*	ע��ָ�룬��������ͨ[char *]��ָ��ʱ��X86�ܹ�����ֻ�ܷ��ʶ���ƫ�Ƶ�ַ��������
			*	���ʶε�ַ�����������64K��ַ��Χ������������Ҫ��[far]�ؼ��֡�
			*	���´�ӡ�������[�׵�ַ]��[64K��ַ]��[ͼƬ����ַ]�����ط������ݣ�ÿ���ط�32�����ء�
			*	���ڵ���ʱ�Ա�����ʹ�á�
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
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RI_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("RI_R,RI_Gr,RI_Gb,RI_B,Result");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RI_TestItem::GetReportLowLimit()
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
LPCTSTR RI_TestItem::GetReportHightLimit()
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