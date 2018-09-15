#include "StdAfx.h"
#include "OF_RI_RU_test.h"
#include "RU_TestItem.h"
#include "ImageProc.h"

#pragma comment(lib, "Moto_RI_RU_Test.lib")

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

static RUValue pRUValue;

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new RU_TestItem(pInterface, lpszName);
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
RU_TestItem::RU_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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
	for (i = 0; i < RU_TST_PIC_NUM; i++)
	{
		m_pBuf[i] = NULL;
	}
	m_CurPicInx = 0;
	//.....
}

RU_TestItem::~RU_TestItem(void)
{
	int i;
	for (i = 0; i < RU_TST_PIC_NUM; i++)
	{
		if (NULL != m_pBuf[i])
		{
			delete []m_pBuf[i];
			m_pBuf[i] = NULL;
		}
	}
	m_CurPicInx = 0;
}


int RU_TestItem::InitItem()
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
int RU_TestItem::Initialize()
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
int RU_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������

	/** Step 1:��ȡͼ���С���Ա�����׼���洢���� **/
	img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("RU_TestItem::Testing()---ץ֡1ʧ��!"), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RU TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	int i;
	long ImgLen = img.imgwidth * img.imgheight;
	if (m_CurPicInx < RU_TST_PIC_NUM)
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

			m_pInterface->Ctrl_Addlog(CamID,_T("RU_TestItem::Testing()---m_pBuf����ʧ��!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RU TEST FAILED!"));
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

			m_pInterface->Ctrl_Addlog(CamID, _T("RU_TestItem::Testing()---Ctrl_GetImgInfo() Fail"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RU TEST FAILED!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		memcpy(m_pBuf[m_CurPicInx], img.Raw_buffer, ImgLen * 2);	//ע�⣺Raw_buffer��USHORT�������ݣ����Կ���ʱ��Ҫ����2

	#if RU_TEST_OUTPUT_INFO == 1
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

	#if RU_TEST_OUTPUT_INFO == 1
		OutputDebugInfo(1, i);
	#endif

		Tmp = 0;
		for (j = 0; j < RU_TST_PIC_NUM; j++)
		{
			Tmp += *(m_pBuf[j] + i);
		}
		
#if 0
		*(m_pBuf[0] + i) = (USHORT)((Tmp + 0.5) / RU_TST_PIC_NUM);
#else
		Tmp = (USHORT)((Tmp + 0.5) / RU_TST_PIC_NUM);
		if (Tmp < BLC_RAW10)
		{
			Tmp = 0;
		}
		else//ԭʼ�ϼܹ������У�RU���Դ�������˴���ش���û�������else����䣬�о��Ǵ���ģ�Ҫ��Ȼ��ԭʼ����С��BLC_RAW10�����ͳɸ����ˣ�--Added by LiHai--20171226
		{
			Tmp -= BLC_RAW10;
		}
		
		pBy = (LPBYTE)(m_pBuf[0] + i);	//ƽ��ֵ����m_pBuf[0]�洢�������Ǵ洢Ϊ����RU���㺯������Ҫ�����ݸ�ʽ--Added by LiHai--20171226
		pBy[0] = LOBYTE(Tmp);
		pBy[1] = HIBYTE(Tmp);

	#if RU_TEST_OUTPUT_INFO == 1
		OutputDebugInfo(2, i);
	#endif
#endif
	}

#if RU_TEST_OUTPUT_INFO == 1
	OutputDebugInfo(3, i);
#endif

	/**
	*	RU���Ժ��������ݽṹ:
	*		R[7��0]R[9:8]Gr[7��0]Gr[9:8]Gb[7��0]Gb[9:8]B[7��0]B[9:8]...
	*	--Added by LiHai--20171225
	**/
#if RU_TEST_BY_READ_RUPIC_BIN == 1
	RuTstByReadRuPicBin(pRUValue);
#else
	RelativeUniformityTest((LPBYTE)(m_pBuf[0]), img.imgwidth, img.imgheight, img.RawFormat, &pRUValue);
#endif

	//���Relative Uniformityֵ
	CString strLog;
	m_RUResult = TRUE;
	if (pRUValue.RU_RChannel > pDlg->m_RuMaxSpec.RU_Max_R)
	{
		strLog.Format(_T("R_RU = %.2f > %.2f Percent\r\n"), pRUValue.RU_RChannel, pDlg->m_RuMaxSpec.RU_Max_R);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RUResult = FALSE;
	}
	else
	{
		strLog.Format(_T("R_RU = %.2f < %.2f Percent\r\n"), pRUValue.RU_RChannel, pDlg->m_RuMaxSpec.RU_Max_R);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRUValue.RU_GrChannel > pDlg->m_RuMaxSpec.RU_Max_Gr)
	{
		strLog.Format(_T("Gr_RU= %.2f > %.2f Percent\r\n"), pRUValue.RU_GrChannel, pDlg->m_RuMaxSpec.RU_Max_Gr);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RUResult = FALSE;
	}
	else
	{
		strLog.Format(_T("Gr_RU= %.2f < %.2f Percent\r\n"), pRUValue.RU_GrChannel, pDlg->m_RuMaxSpec.RU_Max_Gr);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRUValue.RU_GbChannel > pDlg->m_RuMaxSpec.RU_Max_Gb)
	{
		strLog.Format(_T("Gb_RU= %.2f > %.2f Percent\r\n"), pRUValue.RU_GbChannel, pDlg->m_RuMaxSpec.RU_Max_Gb);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RUResult = FALSE;
	}
	else
	{
		strLog.Format(_T("Gb_RU= %.2f < %.2f Percent\r\n"), pRUValue.RU_GbChannel, pDlg->m_RuMaxSpec.RU_Max_Gb);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRUValue.RU_BChannel > pDlg->m_RuMaxSpec.RU_Max_B)
	{
		strLog.Format(_T("B_RU = %.2f > %.2f Percent\r\n"), pRUValue.RU_BChannel, pDlg->m_RuMaxSpec.RU_Max_B);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_RUResult = FALSE;
	}
	else
	{
		strLog.Format(_T("B_RU = %.2f < %.2f Percent\r\n"), pRUValue.RU_BChannel, pDlg->m_RuMaxSpec.RU_Max_B);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	m_ShadingResult = TRUE;
	if (pRUValue.Shding_RChannel > pDlg->m_RuMaxSpec.RU_Max_R)
	{
		strLog.Format(_T("R_Shding = %.2f > %.2f Percent\r\n"), pRUValue.Shding_RChannel, pDlg->m_RuMaxSpec.RU_Max_R);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_ShadingResult = FALSE;
	}
	else
	{
		strLog.Format(_T("R_Shding = %.2f < %.2f Percent\r\n"), pRUValue.Shding_RChannel, pDlg->m_RuMaxSpec.RU_Max_R);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRUValue.Shding_GrChannel > pDlg->m_RuMaxSpec.RU_Max_Gr)
	{
		strLog.Format(_T("Gr_Shding= %.2f > %.2f Percent\r\n"), pRUValue.Shding_GrChannel, pDlg->m_RuMaxSpec.RU_Max_Gr);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_ShadingResult = FALSE;
	}
	else
	{
		strLog.Format(_T("Gr_Shding= %.2f < %.2f Percent\r\n"), pRUValue.Shding_GrChannel, pDlg->m_RuMaxSpec.RU_Max_Gr);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRUValue.Shding_GbChannel > pDlg->m_RuMaxSpec.RU_Max_Gb)
	{
		strLog.Format(_T("Gb_Shding= %.2f > %.2f Percent\r\n"), pRUValue.Shding_GbChannel, pDlg->m_RuMaxSpec.RU_Max_Gb);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_ShadingResult = FALSE;
	}
	else
	{
		strLog.Format(_T("Gb_Shding= %.2f < %.2f Percent\r\n"), pRUValue.Shding_GbChannel, pDlg->m_RuMaxSpec.RU_Max_Gb);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	if (pRUValue.Shding_BChannel > pDlg->m_RuMaxSpec.RU_Max_B)
	{
		strLog.Format(_T("B_Shding = %.2f > %.2f Percent\r\n"), pRUValue.Shding_BChannel, pDlg->m_RuMaxSpec.RU_Max_B);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_ShadingResult = FALSE;
	}
	else
	{
		strLog.Format(_T("B_Shding = %.2f < %.2f Percent\r\n"), pRUValue.Shding_BChannel, pDlg->m_RuMaxSpec.RU_Max_B);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_BLUE, 200);
	}

	/** �ϼܹ��ڴ˴��������ݵ�excel�ļ��У��˴����Կ����Ƿ񱣴�--Added by LiHai--20171226 **/
	//SaveRUData(pRUValue, m_RUResult, m_ShadingResult);

	for (i = 0; i < RU_TST_PIC_NUM; i++)
	{
		if (NULL != m_pBuf[i])
		{
			delete []m_pBuf[i];
			m_pBuf[i] = NULL;
		}
	}

	if ((m_RUResult != TRUE) || (m_ShadingResult != TRUE))
	{
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("RU TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	SetResult(RESULT_PASS);
	m_pInterface->Ctrl_Addlog(CamID, _T("RU test PASS!\r\n"), COLOR_BLUE, 200);
	return 0;
}

/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int RU_TestItem::Finalize()
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
int RU_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	CString str;
	str = ReadConfigString(_T("RU_Max_R"), _T("0"));
	pDlg->m_RuMaxSpec.RU_Max_R = _tstof(str);//tempnum =atof(T2A(a));
	str = ReadConfigString(_T("RU_Max_Gr"), _T("0"));
	pDlg->m_RuMaxSpec.RU_Max_Gr = _tstof(str);
	str = ReadConfigString(_T("RU_Max_Gb"), _T("0"));
	pDlg->m_RuMaxSpec.RU_Max_Gb = _tstof(str);
	str = ReadConfigString(_T("RU_Max_B"), _T("0"));
	pDlg->m_RuMaxSpec.RU_Max_B = _tstof(str);

	//........................
	return TestItemInterface::LoadOption();
}

/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int RU_TestItem::SaveOption()
{
	if (!TestItemInterface::SaveOption())
	{
		return -1;
	}

	//TODO: �ڴ���Ӵ洢����
	CString str;
	str.Format(_T("%f"), pDlg->m_RuMaxSpec.RU_Max_R);
	WriteConfigString(_T("RU_Max_R"), str.GetBuffer(str.GetLength()));
	str.Format(_T("%f"), pDlg->m_RuMaxSpec.RU_Max_Gr);
	WriteConfigString(_T("RU_Max_Gr"), str.GetBuffer(str.GetLength()));
	str.Format(_T("%f"), pDlg->m_RuMaxSpec.RU_Max_Gb);
	WriteConfigString(_T("RU_Max_Gb"), str.GetBuffer(str.GetLength()));
	str.Format(_T("%f"), pDlg->m_RuMaxSpec.RU_Max_B);
	WriteConfigString(_T("RU_Max_B"), str.GetBuffer(str.GetLength()));
	//........................
	return 0;
}

CString RU_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}

#if RU_TEST_OUTPUT_INFO == 1
void RU_TestItem::OutputDebugInfo(BYTE InfoType, int i)
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
				strLog.Format(_T("GetImg--j:%2d--0x%02X 0x%02X    0x%02X 0x%02X    0x%02X 0x%02X\r\n"), j, *(m_pBuf[i] + j), img.Raw_buffer[j], *(m_pBuf[i] + j + 0X10000), img.Raw_buffer[j + 0X10000], *(m_pBuf[i] + j + ImgLen - 32), img.Raw_buffer[j + ImgLen - 32]);
				m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
			}
		}
		break;

	case 1:
		{
			if (i < 32)
			{
				CString strLog;
				strLog.Format(_T("GetAve--0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"), *(m_pBuf[0] + i), *(m_pBuf[1] + i), *(m_pBuf[2] + i), *(m_pBuf[3] + i), *(m_pBuf[4] + i));
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

#if RU_TEST_BY_READ_RUPIC_BIN == 1
void RU_TestItem::RuTstByReadRuPicBin(struct RUValue &pRUValue)
{
	int i;
	LPBYTE pRuPicData = NULL;
	FILE  *fp  = NULL;
	CString path;

	path = _T("F:\\L8856F80_OTP\\L8856F80_OTP\\Debug\\RU_PIC.bin");

	fp = _wfopen(path,_T("rb"));
	if(fp == NULL)  
	{
		//AddString(RED,"open file fail!");
		return;
	}

	img.imgwidth = 3264;
	img.imgheight = 2448;
	int ImgLen = 2 * img.imgwidth * img.imgheight;
	//pRuPicData = new unsigned char[ImgLen];
	pRuPicData = (LPBYTE)malloc(ImgLen);
	if (NULL == pRuPicData)
	{
		fclose(fp);
		//AddString(RED,"new pRuPicData error\r\n");
		return;
	}

	fseek( fp, 0, SEEK_SET);
	int readnum = fread((void*)pRuPicData, ImgLen, 1, fp);
	for (i = 0; i < 32; i++)
	{
		if (i % 16 == 15)
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRuPicData[i]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
		else
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRuPicData[i]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
	}

	for (i = 0; i < 32; i++)
	{
		if (i % 16 == 15)
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRuPicData[i + 0XFFFF + 1]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
		else
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRuPicData[i + 0XFFFF + 1]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
	}

	for (i = 0; i < 32; i++)
	{
		if (i % 16 == 15)
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRuPicData[i + ImgLen - 32]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
		else
		{
			CString strLog;
			strLog.Format(_T(" 0x%02X\r\n"), pRuPicData[i + ImgLen - 32]);
			m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		}
	}

	fclose(fp);

	RelativeUniformityTest(pRuPicData, img.imgwidth, img.imgheight, img.RawFormat, &pRUValue);
	//delete []pRuPicData;
	free(pRuPicData);
}
#endif

/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RU_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("RU_R,RU_Gr,RU_Gb,RU_B,Result,R_Shading,Gr_Shading,Gb_Shading,B_Shading,Result");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR RU_TestItem::GetReportLowLimit()
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
LPCTSTR RU_TestItem::GetReportHightLimit()
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
LPCTSTR RU_TestItem::GetReportContents()
{
	static CString Content;
	Content.Format(_T("%.2f%%,%.2f%%,%.2f%%,%.2f%%,"), pRUValue.RU_RChannel, pRUValue.RU_GrChannel, pRUValue.RU_GbChannel, pRUValue.RU_BChannel);
	if (RESULT_PASS == m_RUResult)
	{
		Content += _T("PASS,");
	}
	else
	{
		Content += _T("NG,");
	}

	CString Content_RUShading;
	Content_RUShading.Format(_T("%.2f%%,%.2f%%,%.2f%%,%.2f%%,"), pRUValue.Shding_RChannel, pRUValue.Shding_GrChannel, pRUValue.Shding_GbChannel, pRUValue.Shding_BChannel);
	if (RESULT_PASS == m_ShadingResult)
	{
		Content_RUShading += _T("PASS");
	}
	else
	{
		Content_RUShading += _T("NG");
	}

	Content += Content_RUShading;

	return Content;
}