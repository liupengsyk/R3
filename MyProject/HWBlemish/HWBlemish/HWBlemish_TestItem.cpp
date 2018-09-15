#include "StdAfx.h"
#include "HWBlemish_TestItem.h"
#include "ImageProc.h"
#include "tlhelp32.h" 
//#include "IPcomLicense.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new HWBlemish_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
HWBlemish_TestItem::HWBlemish_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	m_cfgName=CString(lpszName);
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

HWBlemish_TestItem::~HWBlemish_TestItem(void)
{

}


int HWBlemish_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	LoadOption(); 
	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int HWBlemish_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:�ڴ���ӳ�ʼ������
	CString path = m_pInterface->Ctrl_GetCurrentPath()+L"hwBlemishTestDemo\\"+L"HwBlemishTest.ini";
	SetConfigPath(path);
	SetName(L"Spec");
	WriteConfigString(L"RangeThreshold",pDlg->m_P1);
	WriteConfigString(L"medianThreshold",pDlg->m_P2);
	WriteConfigString(L"minBetaGray",pDlg->m_P3);
	WriteConfigString(L"hv_minBetaFFT",pDlg->m_P4);

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(m_cfgName);

	FrameCount = 0;
	BlemishRes = FALSE;
 
	return 0;
}



/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int HWBlemish_TestItem::Testing()
{
	CString strLog = L"";
	if (FrameCount<pDlg->m_FrameCount)
	{
		FrameCount++;
		strLog.Format(L"FrameCount:%d",FrameCount);
		m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
		return 1;
	}

	BOOL Res = testHwBlemish();
	if (Res == TRUE)
	{
		m_pInterface->Ctrl_Addlog(CamID,L"TestPass",COLOR_GREEN,200);
		SetResult(RESULT_PASS);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"HwBlemish Test PASS");
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,L"TestFail",COLOR_RED,200);
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"HwBlemish Test Fail");
	}
	return 0;
	
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int HWBlemish_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������
	m_pInterface->Ctrl_SetCamErrorCode(CamID, pDlg->m_ErrorCode);

	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int HWBlemish_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(m_cfgName);

	pDlg->m_P1 = ReadConfigString(L"RangeThreshold",L"2.3");
	pDlg->m_P2 = ReadConfigString(L"medianThreshold",L"0.4");
	pDlg->m_FrameCount = ReadConfigInt(L"FrameCount",3);

	pDlg->m_P3 = ReadConfigString(L"minBetaGray",L"4.0");
	pDlg->m_P4 = ReadConfigString(L"hv_minBetaFFT",L"14.0");

	pDlg->m_ShowTime = ReadConfigInt(L"ShowTime",1000);
	pDlg->m_DelayTimes = ReadConfigInt(L"DelayTime",20);

	pDlg->m_LoadImgEn = ReadConfigInt(L"LoadImgEn",0);
	pDlg->m_Width = ReadConfigInt(L"Width",0);
	pDlg->m_Height = ReadConfigInt(L"Height",0);
	pDlg->m_ImgPath = ReadConfigString(L"ImgPath",L"");
	pDlg->m_ImgName = ReadConfigString(L"ImgName",L"");

	pDlg->m_ErrorCode = ReadConfigInt(L"ErrorCode", 0);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int HWBlemish_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	  
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(m_cfgName);
	WriteConfigString(L"RangeThreshold",pDlg->m_P1);
	WriteConfigString(L"medianThreshold",pDlg->m_P2);
	WriteConfigInt(L"FrameCount",3);
	WriteConfigString(L"minBetaGray",pDlg->m_P3);
	WriteConfigString(L"hv_minBetaFFT",pDlg->m_P4);

	WriteConfigInt(L"ShowTime",pDlg->m_ShowTime);
    WriteConfigInt(L"DelayTime",pDlg->m_DelayTimes );

	WriteConfigInt(L"LoadImgEn",pDlg->m_LoadImgEn);
	WriteConfigInt(L"Width",pDlg->m_Width);
	WriteConfigInt(L"Height",pDlg->m_Height);
	WriteConfigString(L"ImgPath",pDlg->m_ImgPath);
	WriteConfigString(L"ImgName",pDlg->m_ImgName);

	WriteConfigInt(L"ErrorCode", pDlg->m_ErrorCode);
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR HWBlemish_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR HWBlemish_TestItem::GetReportLowLimit()
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
LPCTSTR HWBlemish_TestItem::GetReportHightLimit()
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
LPCTSTR HWBlemish_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

void HWBlemish_TestItem::ShowJPEGFromFile(CString Filename)
{
	LPPICTURE gpPicture = NULL;
	if(!PathFileExists(Filename))
	{
		//AddString(RED,"file %s don't exist",Filename);
		CString tempStr=L"";
		tempStr.Format(L"file %s don't exist");
		m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_RED,200);
		return;
	}

	HANDLE hFile = CreateFile(Filename,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("open file fail");
		CString tempStr=L"";
		tempStr.Format(L"Open file fail");
		m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_RED,200);
		return;
	}

	DWORD dwFileSize = GetFileSize(hFile,NULL);
	_ASSERT(-1 != dwFileSize);

	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	_ASSERT(NULL!= hGlobal);
	pvData = GlobalLock(hGlobal);
	_ASSERT(NULL != pvData);
	DWORD dwBytesRead =0;

	BOOL bRead = ReadFile(hFile,pvData,dwFileSize,&dwBytesRead,NULL);
	_ASSERT(FALSE != bRead);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);
	LPSTREAM pstm = NULL;
	HRESULT hr = CreateStreamOnHGlobal(hGlobal,TRUE,&pstm);
	_ASSERT(SUCCEEDED(hr)&&pstm);

	if(gpPicture)
	{
		gpPicture->Release();
	}
	hr = OleLoadPicture(pstm,dwFileSize,FALSE,IID_IPicture,(LPVOID *)&gpPicture);
	_ASSERT(SUCCEEDED(hr)&&gpPicture);
	pstm->Release();
	HDC hdc;
	CDC * pDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	hdc = pDC->GetSafeHdc();
	if(gpPicture)
	{
		OLE_XSIZE_HIMETRIC hmWidth = 0;
		OLE_XSIZE_HIMETRIC hmHeight = 0;
		gpPicture->get_Width(&hmWidth);
		gpPicture->get_Height(&hmHeight);
#ifndef HIMETRIC_PER_INCH  
#define HIMETRIC_PER_INCH 2540  
#endif 

		int nWidth = MulDiv(hmWidth,GetDeviceCaps(hdc,LOGPIXELSX),HIMETRIC_PER_INCH);
		int nHeight = MulDiv(hmHeight,GetDeviceCaps(hdc,LOGPIXELSX),HIMETRIC_PER_INCH);
		CRect rect;
	//	pWnd->GetClientRect(&rect);
		m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
		gpPicture->Render(hdc,0,0,rect.Width(),rect.Height(),0,hmHeight,hmWidth,-hmHeight,NULL);
	//	pWnd->ReleaseDC(pDC);
		m_pInterface->Ctrl_ShowMemDC(CamID);
	}
}

int GetProcessCount(const TCHAR* szExeName)
{
	TCHAR sztarget[MAX_PATH];
	lstrcpy(sztarget, szExeName);
	CharLowerBuff(sztarget, MAX_PATH);

	int count = 0;
	PROCESSENTRY32 my;
	HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (((int)l) != -1)
	{
		my.dwSize = sizeof(my);
		if (Process32First(l, &my))
		{
			do
			{
				CharLowerBuff(my.szExeFile, MAX_PATH);
				if (lstrcmp(sztarget, my.szExeFile) == 0)
				{
					count++;
				}
			}while (Process32Next(l, &my));
		}
		CloseHandle(l);
	}
	return count;
}

BOOL HWBlemish_TestItem::testHwBlemish()
{

	CString strLog = L"";
	//TODO:�ڴ���Ӳ��������

	//�������·
	//CString toolPath = m_pInterface->Ctrl_GetCurrentPath() + L"hwBlemishTestDemo\\";
	CString ResPath =  m_pInterface->Ctrl_GetCurrentPath() + L"hwBlemishTestDemo\\" + L"hwBlemishDemoRes.txt";
	CString strSrcPath = m_pInterface->Ctrl_GetCurrentPath()+L"hwBlemishTestDemo\\" + L"TestImage.bmp";
	CString strOutDir = m_pInterface->Ctrl_GetCurrentPath()+ L"hwBlemishTestDemo\\"+L"result_img.jpeg";

	if (m_pInterface->isFileExist(ResPath))
	{
		if (::DeleteFile(ResPath))
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Delete hwBlemishDemoRes.txt ok",COLOR_BLUE,200);
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Delete hwBlemishDemoRes.txt ng",COLOR_RED,200);
		}

	}
	if (m_pInterface->isFileExist(strSrcPath))
	{
		if (::DeleteFile(strSrcPath))
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Delete int img ok",COLOR_BLUE,200);
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Delete int img  ng",COLOR_RED,200);
		}
	}

	if (m_pInterface->isFileExist(strOutDir))
	{
		if (::DeleteFile(strOutDir))
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Delete res img ok",COLOR_BLUE,200);
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Delete res img  ng",COLOR_RED,200);
		}
	}


	img = *(m_pInterface->Ctrl_GetImgInfo(CamID));

	if (pDlg->m_LoadImgEn)
	{
		CString tmpStr;
		m_pInterface->Ctrl_Addlog(CamID,L"Use Load Img test pog",COLOR_BLUE,200);
		CString FileName = m_pInterface->Ctrl_GetCurrentPath() + pDlg->m_ImgPath + L"\\" + pDlg->m_ImgName;
		tmpStr.Format(L"ImgPath:%s",FileName);
		m_pInterface->Ctrl_Addlog(CamID,tmpStr,COLOR_BLUE,200);

		FILE *fp = fopen(CT2A(FileName),"rb");
		if (fp!=NULL)
		{
			fread(img.Raw_buffer,1,pDlg->m_Width*pDlg->m_Height*2,fp);
			fclose(fp);
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,L"Load Img Fail",COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"HwBlemish Test Fail");
			SetResult(RESULT_FAIL);
			return 0;
		}

		ImageProc::RawtoRGB24(img.Raw_buffer,img.RGB24_buffer,pDlg->m_Width,pDlg->m_Height,img.RawFormat,img.RawBits);
		img.imgwidth = pDlg->m_Width;
		img.imgheight = pDlg->m_Height;
	}

	m_pInterface->Ctrl_Addlog(CamID,L"StartTestBlemish...",COLOR_BLUE,200);
//	m_pInterface->Ctrl_SaveImage(CamID,1,m_pInterface->Ctrl_GetCurrentPath()+L"hwBlemishTestDemo\\",L"TestImage",2,img.RGB24_buffer);

	CString tempFilePath = m_pInterface->Ctrl_GetCurrentPath() + L"hwBlemishTestDemo\\TestImage.bmp";
	int width = img.imgwidth;//5120;
	int height = img.imgheight;//3840;
	CImage tempImage;
	tempImage.Create(width, height, 24);
	//ȡ��ͼ���׵�ַָ��
	BYTE* Image_Data = (BYTE*)tempImage.GetPixelAddress(0, height-1);

	//�������
	BYTE* pSrc = img.RGB24_buffer; 
	for(int i=0;i<height;i++)
	{//BMP��ת
		int y=height-1-i;
		for(int j=0;j<width;j++)
		{
			Image_Data[(y*width+j)*3]   = pSrc[(i*width+j)*3];
			Image_Data[(y*width+j)*3+1] = pSrc[(i*width+j)*3+1];
			Image_Data[(y*width+j)*3+2] = pSrc[(i*width+j)*3+2];
		}
	}
	//�洢
	tempImage.Save(tempFilePath);
	pSrc = NULL;


	Sleep(100);
	CString toolPath = m_pInterface->Ctrl_GetCurrentPath() + L"hwBlemishTestDemo\\";
	::ShellExecute(NULL, _T("open"), _T("hwBlemishDemo.exe"), _T(""), toolPath, SW_HIDE);

	CStdioFile cvFile;
	int iOpenCount = 0;
	while(1)
	{
		BOOL bRes = m_pInterface->isFileExist(ResPath);
		if (bRes==TRUE)
		{
			break;
		}
		else
		{
			if (iOpenCount<pDlg->m_DelayTimes)
			{
				Sleep(500);
				iOpenCount++;
			}
			else
			{
				break;
			}
		}

	}

	if (iOpenCount>=pDlg->m_DelayTimes)
	{
		m_pInterface->Ctrl_Addlog(CamID,L"hwPOG TEST Fail!",COLOR_RED,300);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"HwBlemish TEST Fail!");
		SetResult(RESULT_FAIL);
		return 0;
	}

	Sleep(1000);
	char tempChar[128] = {0};
	FILE *fp = NULL;
	fp = fopen(CT2A(ResPath),"r");
	
	if (fp!=NULL)
	{
		fread(tempChar,1,128,fp);
		fclose(fp);
		fp = NULL;
	}
	CString strRes(tempChar);

// 	Sleep(1000);
// 	CString strRes = L"";
// 	CStdioFile tempFile;
// 	if (!tempFile.Open(ResPath, CFile::modeRead | CFile::typeText))
// 	{
// 	//	return 0;
// 	}
// 	else
// 	{
// 		tempFile.ReadString(strRes);
// 	}
// 	tempFile.Close();



	bool Result=false;
	if (strRes.Find(L"PASS")==-1)
	{
		m_pInterface->Ctrl_Addlog(CamID,L"POG Test Result IS NG",COLOR_RED,200);
		Result=false;
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,L"POG Test Result IS PASS",COLOR_GREEN,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"HwBlemish TEST Pass!");
		SetResult(RESULT_PASS);
		Result=true;
	}
	if (Result==false)
	{
		if (m_pInterface->isFileExist(strOutDir))
		{
			ShowJPEGFromFile(strOutDir);
			Sleep(pDlg->m_ShowTime);
		}
		else
		{
			strLog.Format(L"Cant Find JPEG:%s",strOutDir);
			m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_RED,200);
		}

		m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"HwBlemish TEST Fail!");
		SetResult(RESULT_FAIL);
	}
	
	return Result;
}