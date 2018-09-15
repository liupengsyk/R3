#include "StdAfx.h"
#include "AF_TestItem.h"
#include "ImageProc.h"
#include "SFRMatLib.h"

#pragma comment(lib, "ofSFR.lib")
#pragma comment(lib, "SFRMatLib.lib")

#define		ROI_IMAGE_CHECK_LEVEL		200

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */
#define RELEASE(a) if(a!=NULL){delete[] a; a = NULL;}

		bool SaveBmp(char* FileName,unsigned char* Data,int Width,int Height)	//	块Bmp郎
		{
			bool res = true;

			if ((Width == 0)||(Height == 0))
			{
				OutputDebugStringA("SaveImage, w / h = 0");
				return false;
			}
			int ScanLineSize = 0;
			double ModValue = int(fmod(double(Width)*3,4));
			if (ModValue == 0)
			{
				ScanLineSize = (Width)*3;
			}
			else
			{
				ScanLineSize = (Width)*3+int((ModValue == 0)? (0):(4-ModValue));
			}

			unsigned char* pBitmapBits = new unsigned char[ScanLineSize*(Height)];
			memset(pBitmapBits,0,ScanLineSize*(Height));

			for (int i=0;i<Height;i++)
			{
				for (int j=0;j<Width;j++)
				{
					pBitmapBits[(Height-1-i)*ScanLineSize+j*3+2] = Data[(i*Width+j)*3+2];
					pBitmapBits[(Height-1-i)*ScanLineSize+j*3+1] = Data[(i*Width+j)*3+1];
					pBitmapBits[(Height-1-i)*ScanLineSize+j*3+0] = Data[(i*Width+j)*3+0];
				}
			}
			LONG lWidth = Width;
			LONG lHeight = Height;
			WORD wBitsPerPixel = 24;
			BITMAPINFOHEADER bmpInfoHeader = {0};
			// Set the size
			bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
			// Bit count
			bmpInfoHeader.biBitCount = wBitsPerPixel;
			// Use all colors
			bmpInfoHeader.biClrImportant = 0;
			// Use as many colors according to bits per pixel
			bmpInfoHeader.biClrUsed = 0;
			// Store as Uncompressed
			bmpInfoHeader.biCompression = BI_RGB;
			// Set the height in pixels
			bmpInfoHeader.biHeight = lHeight;
			// Width of the Image in pixels
			bmpInfoHeader.biWidth = lWidth;
			// Default number of planes
			bmpInfoHeader.biPlanes = 1;
			// Calculate the image size in bytes
			bmpInfoHeader.biSizeImage = ScanLineSize*(Height); 

			BITMAPFILEHEADER bfh = {0};
			// This value should be values of BM letters i.e 0⊙4D42
			// 0⊙4D = M 0⊙42 = B storing in reverse order to match with endian
			bfh.bfType = 0x4D42;
			/* or
			bfh.bfType = ˉBˇ+(ˇMˇ << 8);
			// <<8 used to shift ˉMˇ to end
			*/
			// Offset to the RGBQUAD
			bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
			// Total size of image including size of headers
			bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
			// Create the file in disk to write
			HANDLE hFile = CreateFileA(FileName,GENERIC_WRITE, 0,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL); 

			if( !hFile ) // return if error opening file
			{
				res = false;
				RELEASE(pBitmapBits);
				//OfMSGBOX(TEXT("Some error, Can't open this file"));
				OutputDebugStringA("save image create file Fail");
				return res;
			} 

			DWORD dwWritten = 0;
			// Write the File header
			WriteFile( hFile, &bfh, sizeof(bfh), &dwWritten , NULL );
			// Write the bitmap info header
			WriteFile( hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL );
			// Write the RGB Data
			WriteFile( hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL );
			// Close the file handle
			CloseHandle( hFile );

			RELEASE(pBitmapBits);

			return res;	
		}
extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new AF_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
AF_TestItem::AF_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	//设置测试项目的名字
	m_cfgName =(CString)lpszName;
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化
	m_nTestSatrTime = 0;
	m_nTestEndTime = 0;
	m_nTestSatrTimeTwo = 0;
	m_nTestEndTimeTwo = 0;

	m_nTestAutoExpoureStartTime = 0;
	m_nTestAutoExpoureEndTime = 0;

	m_nTestAutoFocusStartTime = 0;
	m_nTestAutoFocusEndTime = 0;

	m_nTestAF_ONE_DELAYStartTime = 0;
	m_nTestAF_ONE_DELAYEndTime = 0;

	m_nTest_AF_BIG_STEPStartTime = 0;
	m_nTest_AF_BIG_STEPEndTime = 0;

	m_nTestAF_TWO_DELAYStartTime = 0;
	m_nTestAF_TWO_DELAYEndTime = 0;

	m_nTestAF_MINI_STEPStartTime = 0;
	m_nTestAF_MINI_STEPEndTime = 0;

	m_nTestAF_THREE_DELAYStartTime = 0;
	m_nTestAF_THREE_DELAYEndTime = 0;

	m_nTestFINAL_CHECK_MODEStartTime = 0;
	m_nTestFINAL_CHECK_MODEEndTime = 0;

	m_nTestROIStartTime = 0;
	m_nTestROIEndTime = 0;

    m_nBestCodeStartTime = 0;
	m_nBestCodeEndTime = 0;

	m_nTestFinialROIStartTime = 0;
	m_nTestFinialROIEndTime = 0;

	
	m_nTestBigStepROIStartTime = 0;
	m_nTestBigStepROIEndTime = 0;
	//.....
}


AF_TestItem::~AF_TestItem(void)
{

}


int AF_TestItem::InitItem()
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
int AF_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
     
	max_index = 0;
	min_index = 0;
	memset(sort_sfrvalue, 0, sizeof(float) * 100);

	m_afErrCode = _ttoi(pDlg->m_afPara.errCode);
	m_afFrmDelayNum = _ttoi(pDlg->m_afPara.frmDelay);
	m_afAeExpNum = _ttoi(pDlg->m_afPara.ae.expoNum);
	m_aeBrightness = 0.0f;

	m_afMode = _T("None");
	m_afFrmDelayCnt = 0;
	m_afStepCnt = 0;
	m_afStepCode = 0;
	m_afTargetCode = 0;
	m_afBestCode = 0;
	m_afBestAv = 0.0f;
	m_afFinalOtpCode=0;

	m_errCnt = 0;
	m_finalEndCnt = 0;
	m_finalChkCnt = 0;
	for (int i=0; i<16; i++)
		m_mtfValBuff[i].clear();
	for (int i=0; i<128; i++)
		m_mtfValBuff[i].clear();
	for (int i=0;i<20;i++)
	{
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal=0.0f;
	}

    for (int i=0;i<64;i++)
    {
        pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH=0.0f;
        pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV=0.0f;
    }

	memset(&m_afStepData, 0, sizeof (m_afStepData));
	memset(m_sfrRoiValue, 0, sizeof(SFR_ROI_VALUE) * 21);

	m_pInterface->VcmDr_InitAF(CamID);
	m_pInterface->VcmDr_WriteAF_Code(0, CamID);
	Sleep(50);

	CString filePath = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile.ini");
	CStdioFile f;
	if (f.Open(filePath, CFile::modeRead | CFile::typeText))
	{
		f.Close();
	//	::DeleteFileW(filePath);
	}
	else
	{
		f.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		f.Close();
	}


	return 0;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	CString strTmp = _T("");

	if (m_afMode == _T("None"))////初始状态
	{
		m_nTestSatrTime = GetTickCount();
		if (pDlg->m_afPara.ae.enable == TRUE)//对应界面中Auto Exposure Setings中Enable
		{
			if (m_afFrmDelayCnt == 0)
			{
				m_pInterface->Sensor_ReadExp(CamID, m_afAeOldExpT);
				int initExpT = 0;
				if (!swscanf(pDlg->m_afPara.ae.expoInitVal, _T("0x%x"), &initExpT))
					swscanf(pDlg->m_afPara.ae.expoInitVal, _T("%d"), &initExpT);
				m_pInterface->Sensor_WriteExp(CamID, initExpT);
				strTmp.Format(_T("AE Init ExpT = 0x%04X"), initExpT);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
				m_afFrmDelayCnt++;
			}
			else if (m_afFrmDelayCnt < m_afFrmDelayNum)//m_afFrmDelayNum 对应界面中AF ParaSetings 中 FrameDelay 的编辑框
			{
				Sleep(20);
				m_afFrmDelayCnt++;
				//strTmp.Format(_T("Frame Delay Count : %d"), m_afFrmDelayCnt);
				//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			}
			else
			{
				m_afMode = _T("AUTO_EXPOSURE_MODE");
				m_afFrmDelayCnt = 0;
				m_afAeExpCnt = 0;

				m_afAeTarget = _ttoi(pDlg->m_afPara.ae.target);//对应界面中Auto Exposure setings 中Expo.Target编辑框
				m_afAeRange = _ttoi(pDlg->m_afPara.ae.range);//对应界面中Auto Exposure setings 中Expo.Target的正负多少范围的编辑框
			}

			m_nTestEndTime = GetTickCount();
		    strTmp.Format(_T("None step_Finish_time = %d"), m_nTestEndTime-m_nTestSatrTime);
		    m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		}
		else if (pDlg->m_afPara.enable == TRUE)//对应界面中AF Paramer Seting中Enable
			m_afMode = _T("AUTO_FOCUS_START");
		else if (pDlg->m_afPara.finalChkEnable == TRUE)//对应界面中Final Check Setings 中Enable
		{
			if (pDlg->m_afPara.finalChkAfEn)//对应界面中Final Check Setings 中Check && AF使能
				m_afMode = _T("FINAL_CHECK_MODE_FIX_CODE");
			else
				m_afMode = _T("FINAL_CHECK_MODE");
			m_errCnt = 0;
			m_finalChkCnt = 0;
			for (int i=0; i<16; i++)
				m_mtfValBuff[i].clear();
			for (int i=0; i<128; i++)
				m_finalAvBuff[i].clear();
		}
		else
			m_afMode = _T("AF_END");

		
		return 1;
	}
	else if (m_afMode == _T("AUTO_EXPOSURE_MODE"))//开始自动线性曝光
	{
		m_nTestAutoExpoureStartTime = GetTickCount();
		if ((m_afFrmDelayCnt % m_afFrmDelayNum) == 0)
		{
			ImageInfo* imgInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
			DWORD size = imgInfo->imgwidth * imgInfo->imgheight;
			BYTE* pRaw8Buff = new BYTE[size];
			for (DWORD i=0; i<size; i++)
				pRaw8Buff[i] = (imgInfo->Raw_buffer[i] >> 2) & 0xFF - _ttoi(pDlg->m_afPara.ae.blc);//对应
			CRect aeRoiRc;

			m_nTestSatrTimeTwo = GetTickCount();
			m_aeBrightness = AE_GetCenterRoiBrightness(pRaw8Buff, imgInfo, pDlg->m_afPara.ae, &aeRoiRc);
			m_nTestEndTimeTwo = GetTickCount();
			strTmp.Format(_T("获取单次曝光值完成时间 = %d"), m_nTestEndTimeTwo - m_nTestSatrTimeTwo);
			m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

			delete[] pRaw8Buff;
			pRaw8Buff = NULL;
			strTmp.Format(_T("Brightness = %.04f"), m_aeBrightness);
			m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			ShowAeRoiInfo(&aeRoiRc);//显示曝光区域以及值

			if ((m_aeBrightness < (m_afAeTarget - m_afAeRange)) || (m_aeBrightness > (m_afAeTarget + m_afAeRange)))
			{
				int MinTarget = m_afAeTarget - m_afAeRange;
				int MaxTarget = m_afAeTarget + m_afAeRange;
				int expT = 0;
				m_pInterface->Sensor_ReadExp(CamID, expT);
				//if ((m_aeBrightness < MinTarget) && ((MinTarget - m_aeBrightness) < 20))
				//	expT += 64;
				//else if((m_aeBrightness > MaxTarget) && ((m_aeBrightness - MaxTarget) < 20))
				//	expT -= 64;
				//else
				{
					if (m_aeBrightness >= 250)
						m_aeFactor /= 2.0f;
					else if (m_aeBrightness <= 5)
						m_aeFactor *= 2.0f;
					else
						m_aeFactor = m_afAeTarget / m_aeBrightness;

					expT = (int)(expT * m_aeFactor);
				}
				
				m_pInterface->Sensor_WriteExp(CamID, expT);
				Sleep(10);

				m_afFrmDelayCnt++;
				m_afAeExpCnt++;
				if (m_afAeExpCnt > m_afAeExpNum)//m_afAeExpNum 对应界面中Auto exposure setings 中expor.num曝光次数
				{
					m_afMode = _T("AF_END");
					m_pInterface->Ctrl_Addlog(CamID, _T("Auto Exposure fail !"), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
					return 1;
				}
				else
					return 1;
			}
			else
			{
				int currentExposure = 0;
				m_pInterface->Sensor_ReadExp(CamID, currentExposure);
				strTmp.Format(_T("AE OK ! Brightness = %f,CurrentExposure = %d"), m_aeBrightness,currentExposure);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
				if (pDlg->m_afPara.enable == TRUE)
					m_afMode = _T("AUTO_FOCUS_START");
				else if (pDlg->m_afPara.finalChkEnable)
				{
					if (pDlg->m_afPara.finalChkAfEn)
						m_afMode = _T("FINAL_CHECK_MODE_FIX_CODE");
					else
						m_afMode = _T("FINAL_CHECK_MODE");
					m_errCnt = 0;
					m_finalChkCnt = 0;
					for (int i=0; i<16; i++)
						m_mtfValBuff[i].clear();
					for (int i=0; i<128; i++)
						m_finalAvBuff[i].clear();
				}
				else
				{
					m_afMode = _T("AF_END");
					SetResult(RESULT_PASS);
				}
				m_afFrmDelayCnt = 0;

				m_nTestAutoExpoureEndTime = GetTickCount();
		        strTmp.Format(_T("自动曝光完成时间 = %d"), m_nTestAutoExpoureEndTime - m_nTestAutoExpoureStartTime);
		        m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				return 1;
			}
		}
		else
		{
			Sleep(20);
			//strTmp.Format(_T("Frame Delay = %d"), m_afFrmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			m_afFrmDelayCnt++;
			return 1;
		}

	}
	else if (m_afMode == _T("AUTO_FOCUS_START"))//开始自动对焦前的准备
	{
		m_nTestAutoFocusStartTime = GetTickCount();
		if (pDlg->m_afPara.finalChkEnable && pDlg->m_afPara.finalChkAfEn && (m_finalEndCnt == 1))//设置起始code值和终止code值
		{
			m_afStartCode = m_afFinalOtpCode - _ttoi(pDlg->m_afPara.finalChkAfRange);//check模式下采用调烧录Code正负搜寻模式
			m_afEndCode = m_afFinalOtpCode + _ttoi(pDlg->m_afPara.finalChkAfRange);//m_afFinalOtpCode从Peakread.ini文件中读取
		}
		else
		{
			m_afStartCode = _ttoi(pDlg->m_afPara.startCode);//普通模式下采用界面设置的起始和终止code值
			m_afEndCode = _ttoi(pDlg->m_afPara.endCode);//对应界面中Startcode 和Endcode
		}

		m_afStepCode = (m_afEndCode - m_afStartCode) / 6; //第一轮分6步跑完
		m_afStepCnt = 0;

		m_afTargetCode = m_afStartCode + (m_afStepCnt++) * m_afStepCode;
		m_pInterface->VcmDr_WriteAF_Code(m_afTargetCode, CamID);
		strTmp.Format(_T("DAC : %d"), m_afTargetCode);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

		m_afMode = _T("AF_ONE_DELAY");
		m_afFrmDelayCnt = 0;

		m_afStepData.bigStepCnt = 0;
		m_afStepData.miniStepCnt = 0;
		for (int i=0; i<256; i++)
		{
			m_afStepData.bigStepData[i].code = 0;
			m_afStepData.bigStepData[i].av = 0.0f;

			m_afStepData.miniStepData[i].code = 0;
			m_afStepData.miniStepData[i].av = 0.0f;

			
		}

		/*m_nTestAutoFocusEndTime = GetTickCount();
		strTmp.Format(_T("自动对焦完成时间 = %d"), m_nTestAutoExpoureEndTime-m_nTestAutoExpoureStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);*/

		m_nTestAutoFocusEndTime = GetTickCount();
		strTmp.Format(_T("自动对焦完成时间 = %d"), m_nTestAutoFocusEndTime-m_nTestAutoFocusStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		return 1;
	}
	else if (m_afMode == _T("AF_ONE_DELAY"))//延时几帧
	{
		/*strTmp.Format(_T("自动对焦完成1 = %d"), m_nTestAutoFocusStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		strTmp.Format(_T("自动对焦完成2 = %d"), m_nTestAutoFocusEndTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);*/

		m_nTestAF_ONE_DELAYStartTime = GetTickCount();
		if (m_afFrmDelayCnt < m_afFrmDelayNum)
		{
			m_afFrmDelayCnt++;
			//strTmp.Format(_T("Frame Delay Count : %d"), m_afFrmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		}
		else
		{
			m_afMode = _T("AF_BIG_STEP");
			m_afFrmDelayCnt = 0;
			if (pDlg->m_afPara.throuthFocusEnable == TRUE)//对应界面中AF throughfocus setings 中Enable
			{
				SetName(_T("GENERAL_SETTING"));
				CString SavePath=ReadConfigString(_T("sReportPath"),_T("D:"));
				SetName(m_cfgName);
			
				_wmkdir(SavePath);

				SYSTEMTIME stTime;
				GetLocalTime(&stTime);

				SavePath+=_T("\\");
				SavePath+=GetName();
				_wmkdir(SavePath);
				CString strTimeString ;
				strTimeString.Format(_T("%04d%02d"),stTime.wYear,stTime.wMonth);
				CString strDataFileName = SavePath + _T("\\") + _T("ThroughFocus_Data") + strTimeString + _T(".csv");
				FILE* fp = NULL;
				fp = _wfopen(strDataFileName,_T("a"));
				if (fp == NULL)
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("打开文档失败，无法保存数据"),COLOR_RED,200);
					return FALSE;
				}
				fclose(fp);

				m_fileLogMsg.Open(strDataFileName,CFile::modeWrite | CFile::typeText);
				if (m_fileLogMsg.GetLength() == 0)
				{
					CString strHeader = _T("SensorID,");
					if (pDlg->m_afPara.throuthFocusEnginerModeEnable == TRUE)
					{
						//strHeader += _T("Code,ROI_0,ROI_1,ROI_2,ROI_3,ROI_4,ROI_5,ROI_6,ROI_7,ROI_8,ROI_9,ROI_10,ROI_11,ROI_12,");
						
						strHeader += _T("Code,CEN,TL8,TR8,BL8,BR8,TL6,TR6,BL6,BR6,TL4,TR4,BL4,BR4,");
					}
					else 
					{
						strHeader += _T("Code,ROI_CEN,ROI_LU,ROI_RU,ROI_LD,ROI_RD");
					}

					strHeader += _T("\n");
					m_fileLogMsg.WriteString(strHeader);
				}
				m_fileLogMsg.SeekToEnd();
				m_fileLogMsg.WriteString(_T("BigStep\r"));

				
			}
		}
		m_nTestAF_ONE_DELAYEndTime = GetTickCount();
		strTmp.Format(_T("AF_ONE_DELAY--完成时间 = %d"), m_nTestAF_ONE_DELAYEndTime-m_nTestAF_ONE_DELAYStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		return 1;
	}
	else if (m_afMode == _T("AF_BIG_STEP"))//大步跑
	{
		/*m_nTestAF_ONE_DELAYEndTime = GetTickCount();
		strTmp.Format(_T("AF_ONE_DELAY--完成时间 = %d"), m_nTestAF_ONE_DELAYEndTime-m_nTestAF_ONE_DELAYStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);*/

		m_nTest_AF_BIG_STEPStartTime = GetTickCount();
		if (m_afFrmDelayCnt % m_afFrmDelayNum == 0)
		{
			m_afStepData.bigStepData[m_afStepData.bigStepCnt].code = m_afTargetCode;
			m_nTestBigStepROIStartTime = GetTickCount();
			m_afStepData.bigStepData[m_afStepData.bigStepCnt].av = GetRoiAfValue(pDlg->m_afPara.avAlgorithm);
			m_nTestBigStepROIEndTime = GetTickCount();
			strTmp.Format(_T("AF_BIG_STEP--获取ROI完成时间 = %d"), m_nTestBigStepROIEndTime-m_nTestBigStepROIStartTime);
		    m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			if (m_afStepData.bigStepData[m_afStepData.bigStepCnt].av<0.0001)
			{
				m_afMode = _T("AF_END");
				m_pInterface->Ctrl_Addlog(CamID, _T("AF Test Failed ."), COLOR_RED, 200);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
				m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
				return 1;
			}
			if (pDlg->m_afPara.throuthFocusEnable == TRUE)//开启四周数据保存功能
			{
				if (pDlg->m_afPara.throuthFocusEnginerModeEnable == TRUE)//全视场保存
				{
					if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<16; i++)
						{
							t.Format(_T("%f, "), pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
						{
							t.Format(_T("%f, %f, "), pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
					{
						CString t = _T("");
						t.Format(_T("%s#,%d, "),m_pInterface->Sensor_GetCamFuseID(CamID), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
						{
							t.Format(_T("%f, "), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);
							strTmp += t;
						}
						strTmp += _T("\r");
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
						{
							t.Format(_T("%f, %f, "), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					else
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<16; i++)
						{
							t.Format(_T("%f, "), pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					m_fileLogMsg.SeekToEnd();
					m_fileLogMsg.WriteString(strTmp);
				}
				else//只保存某一个视场数据
				{
					int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
					int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
					int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
					int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
					if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[lu].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ru].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ld].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[rd].mtfVal);
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValRight);
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
					{
						strTmp.Format(_T("%s#,%d,%f,%f,%f,%f,%f\r"), m_pInterface->Sensor_GetCamFuseID(CamID),
							m_afTargetCode,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[lu].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ru].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ld].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[rd].sfrVal);
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[lu].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[lu].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ru].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ru].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ld].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ld].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[rd].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[rd].sfrValV);
					}
					else
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[lu].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ru].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ld].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[rd].mtfVal);
					}
					m_fileLogMsg.SeekToEnd();
					m_fileLogMsg.WriteString(strTmp);
				}
			}

			strTmp.Format(_T("DAC : %d, %f"), m_afStepData.bigStepData[m_afStepData.bigStepCnt].code, m_afStepData.bigStepData[m_afStepData.bigStepCnt].av);
			m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

			m_afStepData.bigStepCnt++;

			m_afTargetCode = m_afStartCode + (m_afStepCnt++) * m_afStepCode;
			if (m_afTargetCode > m_afEndCode)//跑完一轮
			{
				m_afBestAv = 0.0f;
				m_afBestCode = GetBestCode(_T("BIG_STEP_MODE"), m_afStepData, m_afStepCode, &m_afStartCode, &m_afEndCode, &m_afBestAv);
				m_afStartCode=m_afBestCode-35;		//77;
				m_afEndCode=m_afBestCode+35;		//77;
				m_afMiniStartCode = m_afStartCode;
				m_afMiniEndCode = m_afEndCode;

				strTmp.Format(_T("First Best Code : %d, %f"), m_afBestCode, m_afBestAv);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				strTmp.Format(_T("startCode : %d, endCode : %d"), m_afStartCode, m_afEndCode);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				m_afStepCode = (m_afEndCode - m_afStartCode) /4;
				strTmp.Format(_T("stepCode : %d"), m_afStepCode);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				m_afStepCnt = 0;
				m_afTargetCode = m_afStartCode + (m_afStepCnt++) * m_afStepCode;
				m_pInterface->VcmDr_WriteAF_Code(m_afTargetCode, CamID);//写入目标code值
				strTmp.Format(_T("DAC : %d"), m_afTargetCode);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				m_afMode = _T("AF_TWO_DELAY");
				m_afFrmDelayCnt = 0;
				//if (pDlg->m_afPara.throuthFocusEnable == TRUE)
					//m_fileLogMsg.Close();

				m_nTest_AF_BIG_STEPEndTime = GetTickCount();
				strTmp.Format(_T("AF_BIG_STEP大步搜索完成时间 : %d"), m_nTest_AF_BIG_STEPEndTime - m_nTest_AF_BIG_STEPStartTime);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			}
			else
			{
				m_pInterface->VcmDr_WriteAF_Code(m_afTargetCode, CamID);
				m_afFrmDelayCnt++;
			}
			return 1;
		}
		else
		{
			m_afFrmDelayCnt++;
			//strTmp.Format(_T("Frame Delay Count : %d"), m_afFrmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			return 1;
		}
	}
	else if (m_afMode == _T("AF_TWO_DELAY"))
	{
		/*m_nTest_AF_BIG_STEPEndTime = GetTickCount();
		strTmp.Format(_T("AF_BIG_STEP大步搜索完成时间 : %d"), m_nTest_AF_BIG_STEPEndTime - m_nTest_AF_BIG_STEPStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);*/

		m_nTestAF_TWO_DELAYStartTime = GetTickCount();
		if (m_afFrmDelayCnt < m_afFrmDelayNum)
		{
			m_afFrmDelayCnt++;
			//strTmp.Format(_T("Frame Delay Count : %d"), m_afFrmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		}
		else
		{
			m_afMode = _T("AF_MINI_STEP");
			m_afFrmDelayCnt = 0;
			m_afPosiCnt = 0;
			if (pDlg->m_afPara.throuthFocusEnable == TRUE)
			{	
				m_fileLogMsg.WriteString(_T("MinStep\r"));
			}
		}
		m_nTestAF_TWO_DELAYEndTime = GetTickCount();
		strTmp.Format(_T("AF_TWO_DELAY完成时间 : %d"), m_nTestAF_TWO_DELAYEndTime - m_nTestAF_TWO_DELAYStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		return 1;
	}
	else if (m_afMode == _T("AF_MINI_STEP"))//小步搜寻
	{
	/*	m_nTestAF_TWO_DELAYEndTime = GetTickCount();
		strTmp.Format(_T("AF_TWO_DELAY完成时间 : %d"), m_nTestAF_TWO_DELAYEndTime - m_nTestAF_TWO_DELAYStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);*/

		m_nTestAF_MINI_STEPStartTime = GetTickCount();
		if (m_afFrmDelayCnt % m_afFrmDelayNum == 0)
		{
			m_afStepData.miniStepData[m_afStepData.miniStepCnt].code = m_afTargetCode;
			m_nTestROIStartTime = GetTickCount();
			m_afStepData.miniStepData[m_afStepData.miniStepCnt].av = GetRoiAfValue(pDlg->m_afPara.avAlgorithm);
			m_nTestROIEndTime = GetTickCount();
			strTmp.Format(_T("AF_MINI_STEP获取ROI完成时间 : %d"), m_nTestROIEndTime - m_nTestROIStartTime);
		    m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

			if (m_afStepData.miniStepData[m_afStepData.miniStepCnt].av<0.0001)
			{
				m_afMode = _T("AF_END");
				m_pInterface->Ctrl_Addlog(CamID, _T("AF Test Failed ."), COLOR_RED, 200);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
				m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
				return 1;
			}
			if (pDlg->m_afPara.throuthFocusEnable == TRUE)
			{
				if (pDlg->m_afPara.throuthFocusEnginerModeEnable == TRUE)
				{
					if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<16; i++)
						{
							t.Format(_T("%f, "), pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
						{
							t.Format(_T("%f, %f, "), pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
					{
						CString t = _T("");
						t.Format(_T("%s#,%d, "),m_pInterface->Sensor_GetCamFuseID(CamID), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
						{
							t.Format(_T("%f, "), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);
							strTmp += t;
						}
						strTmp += _T("\r");
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
						{
							t.Format(_T("%f, %f, "), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					else
					{
						CString t = _T("");
						t.Format(_T("%d, "), m_afTargetCode);
						strTmp = t;
						for (int i = 0; i<16; i++)
						{
							t.Format(_T("%f, "), pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
							strTmp += t;
						}
						strTmp += _T("\r\n");
					}
					m_fileLogMsg.SeekToEnd();
					m_fileLogMsg.WriteString(strTmp);
				}
				else
				{
					int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
					int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
					int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
					int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
					if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[lu].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ru].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ld].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[rd].mtfVal);
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[lu].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ru].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[ld].sfrValRight,
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValBottom, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[rd].sfrValRight);
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
					{
						strTmp.Format(_T("%s#,%d,%f,%f,%f,%f,%f\r"),m_pInterface->Sensor_GetCamFuseID(CamID), m_afTargetCode,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[lu].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ru].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ld].sfrVal,
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[rd].sfrVal);
					}
					else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[lu].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[lu].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ru].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ru].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ld].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ld].sfrValV,
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[rd].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[rd].sfrValV);
					}
					else
					{
						strTmp.Format(_T("%d,%f,%f,%f,%f,%f\r\n"), m_afTargetCode,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[lu].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ru].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[ld].mtfVal,
							pDlg->m_dlgMtf.m_mtfPara.roiPara[rd].mtfVal);
					}
					m_fileLogMsg.SeekToEnd();
					m_fileLogMsg.WriteString(strTmp);
				}
			}

			strTmp.Format(_T("DAC : %d, %f"), m_afStepData.miniStepData[m_afStepData.miniStepCnt].code, m_afStepData.miniStepData[m_afStepData.miniStepCnt].av);
			m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
				
			if (pDlg->m_afPara.throuthFocusEnable == TRUE)//保存四周数据
				AppendAfBurnFile(pDlg->m_afPara.avAlgorithm, ++m_afPosiCnt, m_afTargetCode, m_sfrRoiValue);

			m_afStepData.miniStepCnt++;

			m_afTargetCode = m_afStartCode + (m_afStepCnt++) * m_afStepCode;
			if (m_afTargetCode > m_afEndCode)
			{
				float afBestAv = 0.0f;
				m_nBestCodeStartTime = GetTickCount();
				m_afBestCode = GetBestCode(_T("MINI_STEP_MODE"), m_afStepData, m_afStepCode, &m_afStartCode, &m_afEndCode, &afBestAv);
				m_nBestCodeEndTime = GetTickCount();
				strTmp.Format(_T("MINI_STEP_MODE获取BESTCODE完成时间 : %d"), m_nBestCodeEndTime - m_nBestCodeStartTime);
		        m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				strTmp.Format(_T("Best Code : %d, %f"), m_afBestCode, afBestAv);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				if (m_afBestCode>_ttoi(pDlg->m_afPara.endCode) || m_afBestCode<_ttoi(pDlg->m_afPara.startCode))
				{
					m_afMode = _T("AF_END");
					m_pInterface->Ctrl_Addlog(CamID, _T("AF PeakCode not in range ."), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
					return 1;
				}

				float bestAvSpec = (float)_ttof(pDlg->m_afPara.afAvSpec);//对应界面中AFpara setings  中 AV Spec编辑框，用于对焦后，得到的Bescode是否超出规格
				if ((m_afBestCode < m_afMiniStartCode) || (m_afBestCode > m_afMiniEndCode) || (afBestAv < bestAvSpec))
				{
					m_afMode = _T("AF_END");
					m_pInterface->Ctrl_Addlog(CamID, _T("AF Test Failed ."), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
					return 1;
				}
				else
					SetResult(RESULT_PASS);

				strTmp.Format(_T("Code1 : %d, Code2 : %d"), m_afStartCode, m_afEndCode);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

				m_pInterface->VcmDr_WriteAF_Code(m_afBestCode, CamID);//写入bestcode值
				WriteConfigInt(_T("BestCode"), m_afBestCode);

				m_afMode = _T("AF_THREE_DELAY");
				m_afFrmDelayCnt = 0;
				if (pDlg->m_afPara.throuthFocusEnable == TRUE)
					m_fileLogMsg.Close();

				m_nTestAF_MINI_STEPEndTime = GetTickCount();
				strTmp.Format(_T("AF_MINI_STEP完成时间 : %d"), m_nTestAF_MINI_STEPEndTime - m_nTestAF_MINI_STEPStartTime);
		        m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			}
			else
			{
				m_pInterface->VcmDr_WriteAF_Code(m_afTargetCode, CamID);
				m_afFrmDelayCnt++;
			}
			return 1;
		}
		else
		{
			m_afFrmDelayCnt++;
			//strTmp.Format(_T("Frame Delay Count : %d"), m_afFrmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
			return 1;
		}
	}
	else if (m_afMode == _T("AF_THREE_DELAY"))
	{
		/*m_nTestAF_MINI_STEPEndTime = GetTickCount();
		strTmp.Format(_T("AF_MINI_STEP完成时间 : %d"), m_nTestAF_MINI_STEPEndTime - m_nTestAF_MINI_STEPStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
*/
		m_nTestAF_THREE_DELAYStartTime = GetTickCount();
		if (m_afFrmDelayCnt < m_afFrmDelayNum)
		{
			m_afFrmDelayCnt++;
			//strTmp.Format(_T("Frame Delay Count : %d"), m_afFrmDelayCnt);
			//m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		}
		else
		{
			if (pDlg->m_afPara.finalChkEnable)
			{
				m_afMode = _T("FINAL_CHECK_MODE");
				m_errCnt = 0;
				m_finalChkCnt = 0;
				for (int i=0; i<16; i++)
					m_mtfValBuff[i].clear();
				for (int i=0; i<128; i++)
					m_finalAvBuff[i].clear();
			}
			else
			{
				m_afMode = _T("AF_END");
				SetResult(RESULT_PASS);
			}
			m_afFrmDelayCnt = 0;
		}
		m_nTestAF_THREE_DELAYEndTime = GetTickCount();
		strTmp.Format(_T("AF_THREE_DELAY完成时间 : %d"), m_nTestAF_THREE_DELAYEndTime - m_nTestAF_THREE_DELAYStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		return 1;
	}
	else if (m_afMode == _T("FINAL_CHECK_MODE_FIX_CODE"))
	{
		if (m_afFrmDelayCnt == 0)
		{
			CString fileName = m_pInterface->Ctrl_GetCurrentPath() + _T("PeakRead.ini");
			CString keyName = _T("PeakRead");
			m_afFinalOtpCode = GetPrivateProfileIntW(keyName, pDlg->m_afPara.finalChkCodeName, 0, fileName);//获取已经有的CODE值
			m_pInterface->VcmDr_WriteAF_Code(m_afFinalOtpCode, CamID);//写入最后烧入的code值
			strTmp.Format(_T("FinalChk OTP Code = %d"), m_afFinalOtpCode);
			m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

			m_afFrmDelayCnt++;
		}
		else if (m_afFrmDelayCnt < m_afFrmDelayNum)
		{
			m_afFrmDelayCnt++;
			Sleep(20);
		}
		else
		{
			m_afMode = _T("FINAL_CHECK_MODE");
			m_errCnt = 0;
			m_finalChkCnt = 0;
			for (int i=0; i<16; i++)
				m_mtfValBuff[i].clear();
			for (int i=0; i<128; i++)
				m_finalAvBuff[i].clear();
		}
		return 1;
	}
	else if (m_afMode == _T("FINAL_CHECK_MODE"))
	{
		/*m_nTestAF_THREE_DELAYEndTime = GetTickCount();
		strTmp.Format(_T("AF_THREE_DELAY完成时间 : %d"), m_nTestAF_THREE_DELAYEndTime - m_nTestAF_THREE_DELAYStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);*/

		m_nTestFINAL_CHECK_MODEStartTime = GetTickCount();
		if (m_finalEndCnt < 2)
		{
			if (m_finalChkCnt < 2)
			{
				m_nTestFinialROIStartTime = GetTickCount();
				float ret = GetRoiAfValueForFinalCheck(pDlg->m_afPara.avAlgorithm);
				m_nTestFinialROIEndTime = GetTickCount();
				strTmp.Format(_T("最后ROI完成时间 : %d"), m_nTestFinialROIEndTime - m_nTestFinialROIStartTime);
		        m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
#if 0
				if (ret < 0.0001)
				{
					m_afMode = _T("AF_END");
					m_pInterface->Ctrl_Addlog(CamID, _T("AF Test Failed ."), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
					return 1;
				}
#endif
				m_finalChkCnt++;
				return 1;
			}
			else
			{
				m_finalEndCnt++;
				if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
				{
					float diffMaxVal = 0.0f, diffMinVal = 100.0f;
					float diffMaxValH = 0.0f, diffMinValH = 100.0f;
					float diffMaxValV = 0.0f, diffMinValV = 100.0f;
					int roiNumLU = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLU);
					int roiNumRU = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumRU);
					int roiNumLD = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLD);
					int roiNumRD = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumRD);
					for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
					{
						if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) && 
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
							continue;

						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop = 0.0f;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom = 0.0f;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft = 0.0f;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight = 0.0f;
						for (unsigned int j=0; j<m_finalAvBuff[i].size() / 4; j++)
						{
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop += m_finalAvBuff[i][4 * j + 0];
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom += m_finalAvBuff[i][4 * j + 1];
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft += m_finalAvBuff[i][4 * j + 2];
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight += m_finalAvBuff[i][4 * j + 3];
						}
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop /= (float)(m_finalAvBuff[i].size() / 4);
						if (!((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop >= 0) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop <= 1)))
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop = 0.0f;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom /= (float)(m_finalAvBuff[i].size() / 4);
						if (!((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom >= 0) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom <= 1)))
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom = 0.0f;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft /= (float)(m_finalAvBuff[i].size() / 4);
						if (!((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft >= 0) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft <= 1)))
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft = 0.0f;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight /= (float)(m_finalAvBuff[i].size() / 4);
						if (!((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight >= 0) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight <= 1)))
							pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight = 0.0f;
						float offsetAfterH = 0.0f;
						if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
							offsetAfterH = (float)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop);
						else
							offsetAfterH = (float)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom);
						offsetAfterH += (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrOffsetH);

						float offsetAfterV = 0.0f;
						if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
							offsetAfterV = (float)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft);
						else
							offsetAfterV = (float)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
						offsetAfterV += (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrOffsetV);

						float limitH = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitH);
						float limitV = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitV);
						if (offsetAfterH > limitH)
						{
							if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
								pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop = offsetAfterH;
							else
								pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom = offsetAfterH;
						}
						if (offsetAfterV > limitV)
						{
							if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
								pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft = offsetAfterV;
							else
								pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight = offsetAfterV;
						}

						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultTop = (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop > limitH) ? TRUE : FALSE;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultBottom = (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom > limitH) ? TRUE : FALSE;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultLeft = (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft > limitV) ? TRUE : FALSE;
						pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultRight = (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight > limitV) ? TRUE : FALSE;
						strTmp.Format(_T("ROI%02d : sfrValH=%.02f[>%.02f], sfrValV=%.02f[>%.02f]"), 
							i, (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop * 100.0f : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom * 100.0f, limitH*100, 
							(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft * 100.0f : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight * 100.0f, limitV*100);
						
						BOOL bH = (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultBottom;
						BOOL bV = (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].resultRight;
						if ((bH == FALSE) || (bV == FALSE))
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.enable == TRUE)
						{
							if ((i == roiNumLU) || (i == roiNumRU) || (i == roiNumLD) || (i == roiNumRD))
							{
								float tmpH = 0.0f, tmpV = 0.0f;
								if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
								{
									diffMaxValH = max(diffMaxValH, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop);
									diffMinValH = min(diffMinValH, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop);
									tmpH = pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop;
								}
								else
								{
									diffMaxValH = max(diffMaxValH, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom);
									diffMinValH = min(diffMinValH, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom);
									tmpH = pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom;
								}

								if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
								{
									diffMaxValV = max(diffMaxValV, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft);
									diffMinValV = min(diffMinValV, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft);
									tmpV = pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft;
								}
								else
								{
									diffMaxValV = max(diffMaxValV, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
									diffMinValV = min(diffMinValV, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
									tmpV = pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight;
								}

								diffMaxVal = max(diffMaxVal, (tmpH + tmpV) / 2.0f);
								diffMinVal = min(diffMinVal, (tmpH + tmpV) / 2.0f);
							}
						}
					}

					if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.enable == TRUE)
					{
						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffVal = diffMaxVal - diffMinVal;
						else
							pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffVal = (diffMaxVal - diffMinVal) / diffMaxVal;
						float diffSpec = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.spec);
						strTmp.Format(_T("SFR Corner Diff=%f[<%f]"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffVal, diffSpec);
						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffVal > diffSpec)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValH = diffMaxValH - diffMinValH;
						else
							pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValH = (diffMaxValH - diffMinValH) / diffMaxValH;
						float diffSpecH = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specH);
						strTmp.Format(_T("SFR Corner DiffH=%f[<%f]"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValH, diffSpecH);
						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValH > diffSpecH)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValV = diffMaxValV - diffMinValV;
						else
							pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValV = (diffMaxValV - diffMinValV) / diffMaxValV;
						float diffSpecV = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specV);
						strTmp.Format(_T("SFR Corner DiffV=%f[<%f]"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValV, diffSpecV);
						if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValV > diffSpecV)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
					}

					AppendAfBurnFileForFinalCheck(_T("X_CHART_SFR"));
				}
				else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
				{
					float diffMaxVal = 0.0f, diffMinVal = 100.0f;
					int roiNumLU = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLU);
					int roiNumRU = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumRU);
					int roiNumLD = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLD);
					int roiNumRD = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumRD);
					for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
					{
						if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
							continue;

						pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = 0.0f;
						for (unsigned int j=0; j<m_finalAvBuff[i].size(); j++)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal += m_finalAvBuff[i][j];

						pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal /= (float)(m_finalAvBuff[i].size());//三帧图像取均值
						if (!((pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal >= 0) && (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal <= 1)))
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = 0.0f;
						float offsetAfter = 0.0f;
						if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == TRUE)
							offsetAfter = (float)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);
						offsetAfter += (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrOffset);

						float limit = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].limit);
						if (offsetAfter > limit)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = offsetAfter;

						pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].result = (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal > limit) ? TRUE : FALSE;
						strTmp.Format(_T("ROI%02d : sfrVal=%.02f[>%.02f]"), i, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal * 100.0f, limit * 100);
						
						if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].result == FALSE)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.enable == TRUE)
						{
							if ((i == roiNumLU) || (i == roiNumRU) || (i == roiNumLD) || (i == roiNumRD))
							{
								float tmp = 0.0f;
								diffMaxVal = max(diffMaxVal, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);//取得最大的SFR值
								diffMinVal = min(diffMinVal, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);//取得最小的SFR值
								tmp = pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal;

								//max_index = i;
								float tempsfrvalue[4] = {0};
								for(int ik = 0; ik < 4; ik ++)
								{
									tempsfrvalue[ik] = pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal;
								}
								SortSfrValue(4,tempsfrvalue);//进行排序
							}
						}
					}

					float diffSpectmep = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.spec);
					//pDlg->m_dlgMotoChartSfr.m_ndiff_value/100;
					if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.enable == TRUE)
					{
						if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal = diffMaxVal - diffMinVal;
						else
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal = (diffMaxVal - diffMinVal) / diffMaxVal;
						//判断是否大于30，如果大于30的话就进行offset,最大的减去最小的除以最大的是否大于30，如果大于的话，取第二个最大的减去最小的
						for(int i = 0; i < 4; i ++)
						{
							if(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal > diffSpectmep)
							{
								pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal = (sort_sfrvalue[1] - diffMinVal) / sort_sfrvalue[1];
								if(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal > diffSpectmep)
								{
									pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal = (sort_sfrvalue[2] - diffMinVal) / sort_sfrvalue[2];
								}
								else
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
						float tempfsetvalue_offset = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffOffset);
						float diffSpec = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.spec);
						strTmp.Format(_T("SFR Corner Diff=%f[<%f]"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal, diffSpec);
						if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal > diffSpec)//判断Differ是否大于界面上所设定的规格
						{
							if((pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal + tempfsetvalue_offset) > diffSpec)
							{
								pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal = pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal + tempfsetvalue_offset;
							}
							else
							{
								m_errCnt++;
								m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
							}
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
					}

					AppendAfBurnFileForFinalCheck(_T("MOTO_CHART_SFR"));
				}
				else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
				{
					float diffMaxVal = 0.0f, diffMinVal = 100.0f;
					float diffMaxValH = 0.0f, diffMinValH = 100.0f;
					float diffMaxValV = 0.0f, diffMinValV = 100.0f;
					int roiNumLU = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLU);
					int roiNumRU = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumRU);
					int roiNumLD = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLD);
					int roiNumRD = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumRD);
					for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
					{
						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
							continue;

						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = 0.0f;
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = 0.0f;
#if 1
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = MedielFilterFor3(m_finalAvBuff[i][2 * 0 + 0], m_finalAvBuff[i][2 * 1 + 0], m_finalAvBuff[i][2 * 2 + 0]);
						if (!((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH >= 0) && (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH <= 1)))
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = 0.0f;
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = MedielFilterFor3(m_finalAvBuff[i][2 * 0 + 1], m_finalAvBuff[i][2 * 1 + 1], m_finalAvBuff[i][2 * 2 + 1]);
						if (!((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV >= 0) && (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV <= 1)))
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = 0.0f;
#else
						for (unsigned int j=0; j<m_finalAvBuff[i].size() / 2; j++)
						{
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH += m_finalAvBuff[i][2 * j + 0];
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV += m_finalAvBuff[i][2 * j + 1];
						}
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH /= (float)(m_finalAvBuff[i].size() / 2);
						if (!((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH >= 0) && (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH <= 1)))
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = 0.0f;
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV /= (float)(m_finalAvBuff[i].size() / 2);
						if (!((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV >= 0) && (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV <= 1)))
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = 0.0f;
#endif
						float offsetAfterH = (float)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH + _ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrOffsetH));
						float offsetAfterV = (float)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV + _ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrOffsetV));
						float limitH = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitH);
						float limitV = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitV);
						if (offsetAfterH > limitH)
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = offsetAfterH;
						if (offsetAfterV > limitV)
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = offsetAfterV;
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].resultH = (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH > limitH) ? TRUE : FALSE;
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].resultV = (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV > limitV) ? TRUE : FALSE;
						strTmp.Format(_T("ROI%02d : sfrValH=%.02f[>%.02f], sfrValV=%.02f[>%.02f]"), 
							i, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100.0f, limitH*100, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100.0f, limitV*100);
						if ((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].resultH == FALSE) || (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].resultV == FALSE))
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable == TRUE)
						{
							if ((i == roiNumLU) || (i == roiNumRU) || (i == roiNumLD) || (i == roiNumRD))
							{
								diffMaxVal = max(diffMaxVal, (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV) / 2.0f);
								diffMinVal = min(diffMinVal, (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV) / 2.0f);
								diffMaxValH = max(diffMaxValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH);
								diffMinValH = min(diffMinValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH);
								diffMaxValV = max(diffMaxValV, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV);
								diffMinValV = min(diffMinValV, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV);
							}
						}
					}

					if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable == TRUE)
					{
						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffVal = diffMaxVal - diffMinVal;
						else
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffVal = (diffMaxVal - diffMinVal) / diffMaxVal;
						float diffSpec = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.spec);
						strTmp.Format(_T("SFR Corner Diff=%f[<%f]"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffVal, diffSpec);
						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffVal > diffSpec)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValH = diffMaxValH - diffMinValH;
						else
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValH = (diffMaxValH - diffMinValH) / diffMaxValH;
						float diffSpecH = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specH);
						strTmp.Format(_T("SFR Corner DiffH=%f[<%f]"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValH, diffSpecH);
						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValH > diffSpecH)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffAbsolute == TRUE)
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValV = diffMaxValV - diffMinValV;
						else
							pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValV = (diffMaxValV - diffMinValV) / diffMaxValV;
						float diffSpecV = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specV);
						strTmp.Format(_T("SFR Corner DiffV=%f[<%f]"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValV, diffSpecV);
						if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValV > diffSpecV)
						{
							m_errCnt++;
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
						}
						else
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
					}

					AppendAfBurnFileForFinalCheck(_T("SQUARE_CHART_SFR"));
				}
				else
				{
					float maxVal = 0.0f, minVal = 100.0f;
					for (int i=0; i<16; i++)
					{
						if (pDlg->m_dlgMtf.m_mtfPara.roiPara[i].enable == TRUE)
						{
							pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal = 0.0f;
							for (unsigned int j=0; j<m_mtfValBuff[i].size(); j++)
								pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal += m_mtfValBuff[i][j];
							pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal /= m_mtfValBuff[i].size();
							float limit1 = (float)_ttof(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].limit1);
							pDlg->m_dlgMtf.m_mtfPara.roiPara[i].result = ((pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal * 100.0f + 0.5) > limit1) ? TRUE : FALSE;
							strTmp.Format(_T("ROI%02d : mtfVal=%.02f[>%.02f]"), i, pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal * 100.0f + 0.5, limit1);
							if (pDlg->m_dlgMtf.m_mtfPara.roiPara[i].result == FALSE)
							{
								m_errCnt++;
								m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
							}
							else
								m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
							if (i >= 1)
							{
								maxVal = max(maxVal, pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
								minVal = min(minVal, pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
							}
						}
					}
					pDlg->m_dlgMtf.m_mtfPara.cornerDiff = maxVal - minVal;
					float diffSpec = (float)_ttof(pDlg->m_dlgMtf.m_mtfPara.cornerDiffSpec);
					strTmp.Format(_T("MTF Corner Diff=%.02f[<%.02f]"), pDlg->m_dlgMtf.m_mtfPara.cornerDiff * 100.0f + 0.5, diffSpec);
					if ((pDlg->m_dlgMtf.m_mtfPara.cornerDiff * 100.0f + 0.5) > diffSpec)
					{
						m_errCnt++;
						m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
					}
					else
						m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);

					ShowMtfRoiInfo(pDlg->m_dlgMtf.m_mtfPara);
				}

				if (pDlg->m_afPara.finalChkAfEn == TRUE)
				{
					if (m_errCnt == 0)
					{
						m_afMode = _T("AF_END");
						if (m_finalEndCnt == 2)
						{
							int diffCode = m_afBestCode - m_afFinalOtpCode;
							strTmp.Format(_T("Diff Code = %d[%d]"), diffCode, _ttoi(pDlg->m_afPara.finalChkCodeDiffSpec));
							m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
							if (abs(diffCode) > _ttoi(pDlg->m_afPara.finalChkCodeDiffSpec))
							{
								SetResult(RESULT_FAIL);
								m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
								m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
								return 1;
							}
							else
								SetResult(RESULT_PASS);
						}
						else
							SetResult(RESULT_PASS);
					}
					else
					{
						m_afMode = _T("AUTO_FOCUS_START");
						if (m_finalEndCnt == 2)
						{
							m_afMode = _T("AF_END");
							SetResult(RESULT_FAIL);
							m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
							m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
							return 1;
						}
					}
				}
				else
				{
					m_afMode = _T("AF_END");
					if (m_errCnt == 0)
						SetResult(RESULT_PASS);
					else
					{
						SetResult(RESULT_FAIL);
						CStdioFile ngRawFile;
						CString filename;
						filename.Format(_T("D:\\Cam%d_Af_PeakSFR_NG_Raw_Image.raw"),CamID);
						BOOL ret = ngRawFile.Open(filename,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
						if (ret==0)
						{
							m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
							m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
							return 1;
						}
						ImageInfo* myimgInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
						ngRawFile.Write(myimgInfo->Raw_buffer,myimgInfo->imgwidth*myimgInfo->imgheight*2);
						ngRawFile.Close();
						m_pInterface->Ctrl_SetCamErrorCode(CamID, m_afErrCode);
						m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_afPara.errInfo);
						return 1;
					}
				}
				return 1;
			}
		}
		else
			m_afMode = _T("AF_END");
	}
	else if (m_afMode == _T("AF_END"))
	{
		//m_pInterface->Sensor_WriteExp(CamID, m_afAeOldExpT);
		m_nTestFINAL_CHECK_MODEEndTime = GetTickCount();//
		strTmp.Format(_T("FINAL_CHECK_MODE完成时间 : %d"), m_nTestFINAL_CHECK_MODEEndTime - m_nTestFINAL_CHECK_MODEStartTime);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		return 0;
	}

   return 0;
}

void AF_TestItem::SortSfrValue(int inum,float m_dSFRValueTemp[])
{
	//从大到小进行排序
	int k = 0;
	int j =0;
	float tempdata = 0;
	for(int i = 0; i < (inum-1);i ++)
	{
		k = i;
		for(j = i +1; j < inum; j ++)
		{
			if(m_dSFRValueTemp[k] < m_dSFRValueTemp[j])
			{
				k = j;
			}
			if(k != i)
			{
				tempdata = m_dSFRValueTemp[i];
				m_dSFRValueTemp[i] = m_dSFRValueTemp[k];
				m_dSFRValueTemp[k] = tempdata;
			}
		}
	}
	for(int jkm = 0; jkm < inum; jkm ++)
	{
		sort_sfrvalue[jkm] = m_dSFRValueTemp[jkm];
	} 
}
/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	m_fileLogMsg.Close();

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AF_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	pDlg->m_afPara.errCode = ReadConfigString(_T("AF_ERROR_CODE"), _T("16"));
	pDlg->m_afPara.errInfo = ReadConfigString(_T("AF_ERROR_INFO"), _T("Test Failed"));

	pDlg->m_afPara.enable = ReadConfigInt(_T("AF_ENABLE"), 1);
	pDlg->m_afPara.avAlgorithm = ReadConfigString(_T("AF_VALUE_ALGORITHM"), _T("MTF"));
	pDlg->m_afPara.frmDelay = ReadConfigString(_T("AF_FRAME_DELAY"), _T("3"));
	pDlg->m_afPara.startCode = ReadConfigString(_T("AF_START_CODE"), _T("100"));
	pDlg->m_afPara.endCode = ReadConfigString(_T("AF_END_CODE"), _T("400"));
	pDlg->m_afPara.afAvSpec = ReadConfigString(_T("AF_PARA_AV_SPEC"), _T("0.5"));

	pDlg->m_afPara.sfrFullMode = ReadConfigInt(_T("AF_SFR_FULL_MODE"), 1);
	pDlg->m_afPara.sfrLastMode = ReadConfigInt(_T("AF_SFR_LAST_MODE"), 0);
	pDlg->m_afPara.sfrBmpMode = ReadConfigInt(_T("AF_SFR_BMP_MODE"), 0);
	pDlg->m_afPara.sfrDemosaic = ReadConfigInt(_T("AF_SFR_DEMOSAIC_MODE"), 0);
	pDlg->m_afPara.sfrRawBalance = ReadConfigInt(_T("AF_SFR_RAW_BALANCE_MODE"), 1);

	pDlg->m_afPara.stationName = ReadConfigString(_T("AF_PARA_STATION_NAME"), _T("Station1"));
	pDlg->AF_UI_Update();

	pDlg->m_afPara.throuthFocusLU = ReadConfigString(_T("AF_THROUTH_FOCUS_LEFT_UP"), _T("1"));
	pDlg->m_afPara.throuthFocusRU = ReadConfigString(_T("AF_THROUTH_FOCUS_RIGHT_UP"), _T("2"));
	pDlg->m_afPara.throuthFocusLD = ReadConfigString(_T("AF_THROUTH_FOCUS_LEFT_DOWN"), _T("3"));
	pDlg->m_afPara.throuthFocusRD = ReadConfigString(_T("AF_THROUTH_FOCUS_RIGHT_DOWN"), _T("4"));
	pDlg->m_afPara.throuthFocusEnable = ReadConfigInt(_T("AF_THROUTH_FOCUS_ENABLE"), 0);
	pDlg->m_afPara.throuthFocusEnginerModeEnable = ReadConfigInt(_T("AF_THROUTH_FOCUS_ENGINER_MODE_ENABLE"), 0);
	pDlg->ThroughFocusUI_Update();

	pDlg->m_afPara.ae.enable = ReadConfigInt(_T("AF_AE_ENABLE"), 1);
	pDlg->m_afPara.ae.overAverage = ReadConfigInt(_T("AF_AE_OVER_AVERAGE"), 0);
	pDlg->m_afPara.ae.over128 = ReadConfigInt(_T("AF_AE_OVER_128"), 0);
	pDlg->m_afPara.ae.channel = ReadConfigString(_T("AF_AE_CHANNEL"), _T("G"));
	pDlg->m_afPara.ae.roi = ReadConfigString(_T("AF_AE_ROI"), _T("5X5"));
	pDlg->m_afPara.ae.blc = ReadConfigString(_T("AF_AE_BLC"), _T("0"));
	pDlg->m_afPara.ae.expoNum = ReadConfigString(_T("AF_AE_EXPOSURE_NUMBER"), _T("20"));
	pDlg->m_afPara.ae.expoInitVal = ReadConfigString(_T("AF_AE_EXPOSURE_INIT_VALUE"), _T("0x00F0"));
	pDlg->m_afPara.ae.target = ReadConfigString(_T("AF_AE_TARGET"), _T("128"));
	pDlg->m_afPara.ae.range = ReadConfigString(_T("AF_AE_RANGE"), _T("5"));
	pDlg->AE_UI_Update();

	pDlg->m_afPara.finalChkEnable = ReadConfigInt(_T("AF_FINAL_CHECK_ENABLE"), 0);
	pDlg->m_afPara.finalChkAfEn = ReadConfigInt(_T("AF_FINAL_CHECK_AF_ENABLE"), 0);
	pDlg->m_afPara.finalChkCodeName = ReadConfigString(_T("AF_FINAL_CHECK_AF_CODE_NAME"), _T("None"));
	pDlg->m_afPara.finalChkAfRange = ReadConfigString(_T("AF_FINAL_CHECK_AF_RANGE"), _T("30"));
	pDlg->m_afPara.finalChkCodeDiffSpec = ReadConfigString(_T("AF_FINAL_CHECK_AF_CODE_DIFF_SPEC"), _T("15"));
	pDlg->FinalCheck_UI_Update();

	GetMtfPara();
	GetSquareChartSfrPara();
	GetXChartSfrPara();
	GetMotoChartSfrPara();
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AF_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码

	WriteConfigString(_T("AF_ERROR_CODE"), pDlg->m_afPara.errCode);
	WriteConfigString(_T("AF_ERROR_INFO"), pDlg->m_afPara.errInfo);

	WriteConfigInt(_T("AF_ENABLE"), pDlg->m_afPara.enable);
	WriteConfigString(_T("AF_VALUE_ALGORITHM"), pDlg->m_afPara.avAlgorithm);
	WriteConfigString(_T("AF_FRAME_DELAY"), pDlg->m_afPara.frmDelay);
	WriteConfigString(_T("AF_START_CODE"), pDlg->m_afPara.startCode);
	WriteConfigString(_T("AF_END_CODE"), pDlg->m_afPara.endCode);
	WriteConfigString(_T("AF_PARA_AV_SPEC"), pDlg->m_afPara.afAvSpec);

	WriteConfigInt(_T("AF_SFR_FULL_MODE"), pDlg->m_afPara.sfrFullMode);
	WriteConfigInt(_T("AF_SFR_LAST_MODE"), pDlg->m_afPara.sfrLastMode);
	WriteConfigInt(_T("AF_SFR_BMP_MODE"), pDlg->m_afPara.sfrBmpMode);
	WriteConfigInt(_T("AF_SFR_DEMOSAIC_MODE"), pDlg->m_afPara.sfrDemosaic);
	WriteConfigInt(_T("AF_SFR_RAW_BALANCE_MODE"), pDlg->m_afPara.sfrRawBalance);
	//WriteConfigInt(_T("m_ndiff_value"), pDlg->m_ndiff_value);
	
	WriteConfigString(_T("AF_PARA_STATION_NAME"), pDlg->m_afPara.stationName);

	WriteConfigString(_T("AF_THROUTH_FOCUS_LEFT_UP"), pDlg->m_afPara.throuthFocusLU);
	WriteConfigString(_T("AF_THROUTH_FOCUS_RIGHT_UP"), pDlg->m_afPara.throuthFocusRU);
	WriteConfigString(_T("AF_THROUTH_FOCUS_LEFT_DOWN"), pDlg->m_afPara.throuthFocusLD);
	WriteConfigString(_T("AF_THROUTH_FOCUS_RIGHT_DOWN"), pDlg->m_afPara.throuthFocusRD);
	WriteConfigInt(_T("AF_THROUTH_FOCUS_ENABLE"), pDlg->m_afPara.throuthFocusEnable);
	WriteConfigInt(_T("AF_THROUTH_FOCUS_ENGINER_MODE_ENABLE"), pDlg->m_afPara.throuthFocusEnginerModeEnable);

	WriteConfigInt(_T("AF_AE_ENABLE"), pDlg->m_afPara.ae.enable);
	WriteConfigInt(_T("AF_AE_OVER_AVERAGE"), pDlg->m_afPara.ae.overAverage);
	WriteConfigInt(_T("AF_AE_OVER_128"), pDlg->m_afPara.ae.over128);
	WriteConfigString(_T("AF_AE_CHANNEL"), pDlg->m_afPara.ae.channel);
	WriteConfigString(_T("AF_AE_ROI"), pDlg->m_afPara.ae.roi);
	WriteConfigString(_T("AF_AE_BLC"), pDlg->m_afPara.ae.blc);
	WriteConfigString(_T("AF_AE_EXPOSURE_NUMBER"), pDlg->m_afPara.ae.expoNum);
	WriteConfigString(_T("AF_AE_EXPOSURE_INIT_VALUE"), pDlg->m_afPara.ae.expoInitVal);
	WriteConfigString(_T("AF_AE_TARGET"), pDlg->m_afPara.ae.target);
	WriteConfigString(_T("AF_AE_RANGE"), pDlg->m_afPara.ae.range);

	WriteConfigInt(_T("AF_FINAL_CHECK_ENABLE"), pDlg->m_afPara.finalChkEnable);
	WriteConfigInt(_T("AF_FINAL_CHECK_AF_ENABLE"), pDlg->m_afPara.finalChkAfEn);
	WriteConfigString(_T("AF_FINAL_CHECK_AF_CODE_NAME"), pDlg->m_afPara.finalChkCodeName);
	WriteConfigString(_T("AF_FINAL_CHECK_AF_RANGE"), pDlg->m_afPara.finalChkAfRange);
	WriteConfigString(_T("AF_FINAL_CHECK_AF_CODE_DIFF_SPEC"), pDlg->m_afPara.finalChkCodeDiffSpec);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_TestItem::GetReportHeads()
{
	static CString header = _T("");
	header = _T("");
	CString strT = _T("");

	if (pDlg->m_afPara.ae.enable == TRUE)
		header += _T("AE_Bright,");

	if (pDlg->m_afPara.enable == TRUE)
	{
		header += _T("AF_BestCode,");
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
			header += _T("sfrVal,sfrValH,sfrValV,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
			header += _T("sfrVal,");
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
			header += _T("sfrVal,sfrValH,sfrValV,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
			header += _T("mtfVal,");
		else
			header += _T("diffVal,");
	}

	if (pDlg->m_afPara.finalChkEnable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
			{
				if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) && 
					(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
					continue;
				
				strT.Format(_T("ROI%02d_H,ROI%02d_V,"), i, i);
				header += strT;
			}
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
		{
			//for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
			//{
			//	if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
			//		continue;

			//	strT.Format(_T("ROI%02d,"), i);
			//	header += strT;
			//}
			//strT.Format(_T("CEN,TL8,TR8,BL8,BR8,TL6,TR6,BL6,BR6,TL4,TR4,BL4,BR4,L6,R6,B4,L4,R4,T4,"));
			strT.Format(_T("CEN,TL4,TR4,BL4,BR4,TL6,TR6,BL6,BR6,TL8,TR8,BL8,BR8,L6,R6,B4,L4,R4,T4,CTC,PeakCode,"));
			header+=strT;
		}
		else if(pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
			{
				if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
					continue;
				
				strT.Format(_T("ROI%02d_H,ROI%02d_V,"), i, i);
				header += strT;
			}
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
		{
			for (int i=0; i<5; i++)
			{
				strT.Format(_T("ROI%02d,"), i);
				header += strT;
			}
			header += _T("C2C_Val,");
		}
		else
		{
			for (int i=0; i<5; i++)
			{
				strT.Format(_T("ROI%02d,"), i);
				header += strT;
			}
			header += _T("C2C_Val,");
		}
	}

	if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
			header += _T("DiffVal,DiffValH,DiffValV,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
			header += _T("DiffVal,");
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
			header += _T("DiffVal,DiffValH,DiffValV,");
	}
	
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_TestItem::GetReportLowLimit()
{
	static CString LowerLimit = _T("");
	LowerLimit = _T("");
	CString strT = _T("");

	if (pDlg->m_afPara.ae.enable == TRUE)
	{
		int target = _ttoi(pDlg->m_afPara.ae.target);
		int range = _ttoi(pDlg->m_afPara.ae.range);
		strT.Format(_T("%d,"), target - range);
		LowerLimit += strT;
	}

	if (pDlg->m_afPara.enable == TRUE)
	{
		int startCode = _ttoi(pDlg->m_afPara.startCode);
		strT.Format(_T("%d,"), startCode);
		LowerLimit += strT;
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
			LowerLimit += _T("0.00,0.00,0.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
			LowerLimit += _T("0.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
			LowerLimit += _T("0.00,0.00,0.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
			LowerLimit += _T("0.00,");
		else
			LowerLimit += _T("0,");
	}

	if (pDlg->m_afPara.finalChkEnable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
			{
				if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) &&
					(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
					continue;

				LowerLimit += pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitH + _T(",") + pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitV + _T(",");
			}
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
		{
			//for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
			//{
			//	//if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
			//	//	continue;

			//	LowerLimit += pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].limit + _T(",");
			//}
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].limit + _T(",");

			//0.4
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[9].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[10].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[11].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[12].limit + _T(",");
			//0.6
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[5].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[6].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[7].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[8].limit + _T(",");
			//0.8
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[1].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[2].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[3].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[4].limit + _T(",");

			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[13].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[14].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[15].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[16].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[17].limit + _T(",");
			LowerLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[18].limit + _T(",");
			//CTC
			LowerLimit+=_T("0,");
			//PeakCode
			LowerLimit+=pDlg->m_afPara.startCode+_T(",");
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
			{
				if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
					continue;

				LowerLimit += pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitH + _T(",") + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitV + _T(",");
			}
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
			LowerLimit += _T("0.00,0.00,0.00,0.00,0.00,0.00,");
		else
			LowerLimit += _T("0.00,0.00,0.00,0.00,0.00,0.00,");
	}

	if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			LowerLimit += pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.spec + _T(",");
			LowerLimit += pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specH + _T(",");
			LowerLimit += pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specV + _T(",");
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
			LowerLimit += pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.spec + _T(",");
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			LowerLimit += pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.spec + _T(",");
			LowerLimit += pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specH + _T(",");
			LowerLimit += pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specV + _T(",");
		}
	}
	
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_TestItem::GetReportHightLimit()
{
	static CString UpperLimit = _T("");
	UpperLimit = _T("");
	CString strT = _T("");

	if (pDlg->m_afPara.ae.enable == TRUE)
	{
		int target = _ttoi(pDlg->m_afPara.ae.target);
		int range = _ttoi(pDlg->m_afPara.ae.range);
		strT.Format(_T("%d,"), target + range);
		UpperLimit += strT;
	}
	
	if (pDlg->m_afPara.enable == TRUE)
	{
		int endCode = _ttoi(pDlg->m_afPara.endCode);
		strT.Format(_T("%d,"), endCode);
		UpperLimit += strT;
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
			UpperLimit += _T("1.00,1.00,1.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
			UpperLimit += _T("1.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
			UpperLimit += _T("1.00,1.00,1.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
			UpperLimit += _T("1.00,");
		else
			UpperLimit += _T("99999,");
	}

	if (pDlg->m_afPara.finalChkEnable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
			{
				if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) &&
					(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
					continue;
				
				UpperLimit += _T("1.00,1.00,");
			}
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
			{
				//if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
				//	continue;
				
				UpperLimit += _T("1.00,");
			}
			//CTC
			UpperLimit+=pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.spec + _T(",");
			//PeakCode
			UpperLimit+=pDlg->m_afPara.endCode+_T(",");
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
			{
				if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
					continue;
				
				UpperLimit += _T("1.00,1.00,");
			}
		}
		else
		{
			strT.Format(_T("1.00,1.00,1.00,1.00,1.00,%s,"), pDlg->m_dlgMtf.m_mtfPara.cornerDiffSpec);
			UpperLimit += strT;
		}
	}

	if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
			UpperLimit += _T("1.00,1.00,1.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
			UpperLimit += _T("1.00,");
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
			UpperLimit += _T("1.00,1.00,1.00,");
	}

	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_TestItem::GetReportContents()
{
	static CString Content = _T("");
	Content = _T("");
	CString strT = _T("");

	if (pDlg->m_afPara.ae.enable == TRUE)
	{
		strT.Format(_T("%f,"), m_aeBrightness);
		Content += strT;
	}

	if (pDlg->m_afPara.enable == TRUE)
	{
		strT.Format(_T("%d,"), m_afBestCode);
		Content += strT;
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			strT.Format(_T("%f,%f,%f,"), 
				((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValBottom + 
				(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValRight) / 2,
				(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValBottom, 
				(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[0].sfrValRight);
			Content += strT;
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
		{
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal);
			Content += strT;
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			strT.Format(_T("%f,%f,%f,"), 
				(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValV) / 2,
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValV);
			Content += strT;
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MTF"))
		{
			strT.Format(_T("%f,"), pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal);
			Content += strT;
		}
		else
		{
			strT.Format(_T("%f,"), m_afBestAv);
			Content += strT;
		}
	}

	if (pDlg->m_afPara.finalChkEnable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
			{
				if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) &&
					(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
					continue;
				
				strT.Format(_T("%f,%f,"), (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom, 
					(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE) ? pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft : pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
				Content += strT;
			}
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
		{
			//for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
			//{
			//	//if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
			//	//	continue;
			//	
			//	strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);
			//	Content += strT;
			//}

			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal);
			Content += strT;
			//0.4
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[9].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[10].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[11].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[12].sfrVal);
			Content += strT;
			//0.6
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[5].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[6].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[7].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[8].sfrVal);
			Content += strT;
			//0.8
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[1].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[2].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[3].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[4].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[13].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[14].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[15].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[16].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[17].sfrVal);
			Content += strT;
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[18].sfrVal);
			Content += strT;
			//CTC
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal);
			Content += strT;
			//peak
			strT.Format(_T("%d,"), m_afBestCode);
			Content += strT;

		}
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
			{
				if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
					continue;
				
				strT.Format(_T("%f,%f,"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV);
				Content += strT;
			}
		}
		else
		{
			for (int i=0; i<5; i++)
			{
				strT.Format(_T("%f,"), pDlg->m_dlgMtf.m_mtfPara.roiPara[i].mtfVal);
				Content += strT;
			}
			strT.Format(_T("%f,"), pDlg->m_dlgMtf.m_mtfPara.cornerDiff);
			Content += strT;
		}
	}

	if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable == TRUE)
	{
		if (pDlg->m_afPara.avAlgorithm == _T("X_CHART_SFR"))
		{
			strT.Format(_T("%f,%f,%f,"),
				pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffVal,
				pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValH,
				pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValV);
			Content += strT;
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("MOTO_CHART_SFR"))
		{
			strT.Format(_T("%f,"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal);
			Content += strT;
		}
		else if (pDlg->m_afPara.avAlgorithm == _T("SQUARE_CHART_SFR"))
		{
			strT.Format(_T("%f,%f,%f,"),
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffVal,
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValH,
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValV);
			Content += strT;
		}
	}

	return Content;
}


float AF_TestItem::GetRoiAfValue(CString algorithm)
{
	float ret = 0.0f;

	ImageInfo imgInfo = *(m_pInterface->Ctrl_GetImgInfo(CamID));
	int imgWidth = imgInfo.imgwidth, imgHeight = imgInfo.imgheight;

	CString strAlgo = pDlg->m_afPara.avAlgorithm;
	if ((m_afMode == _T("AF_MINI_STEP")) && (pDlg->m_afPara.sfrLastMode))
		strAlgo = _T("X_CHART_SFR");

	if (strAlgo == _T("MTF"))
	{
		int w = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiWidth);
		int h = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiHeight);
		for (int i=0; i<5; i++)
		{
			int cx = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].cx);//界面给定区域中心坐标
			int cy = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].cy);
			pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.left = imgWidth * (cx - w / 2) / 100;//左点
			pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.right = imgWidth * (cx + w / 2) / 100;//右点
			pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.top = imgHeight * (cy - h / 2) / 100;//顶端
			pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.bottom = imgHeight * (cy + h / 2) / 100;//下端
		}

		BYTE* yBuff = new BYTE[imgWidth * imgHeight];//申请一块区域
		if (pDlg->m_dlgMtf.m_mtfPara.bUseRaw8Test)
		{
			BYTE *pRaw8Buf = new BYTE[imgWidth*imgHeight];
			BYTE *pRGB24Buf = new BYTE[imgWidth*imgHeight*3+1024];
			if (NULL == pRaw8Buf || NULL == pRGB24Buf)
			{
				SAFE_DELETE_ARRAY(pRaw8Buf);
				SAFE_DELETE_ARRAY(pRGB24Buf);
				SAFE_DELETE_ARRAY(yBuff);
				CString Msg;
				Msg.Format(_T("%s malloc memory error!"),GetName());
				m_pInterface->Ctrl_Addlog(CamID,Msg, COLOR_RED,500);	
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,Msg);
				return 0.0;
			}
			ImageProc::RawToRaw8(imgInfo.Raw_buffer,pRaw8Buf, imgWidth, imgHeight, imgInfo.RawBits);
			ImageProc::Raw8toRGB24(pRaw8Buf, pRGB24Buf, imgWidth, imgHeight, imgInfo.RawFormat);
			ImageProc::RGB24toY(pRGB24Buf,yBuff,imgWidth,imgHeight);
			SAFE_DELETE_ARRAY(pRaw8Buf);
			SAFE_DELETE_ARRAY(pRGB24Buf);
		}
		else
		{
			ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		}
		//根据图像的宽度，高度，ROI的区域，求出mtf值，也即图像解析力值
		pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal = ret = (float)ImageProc::GetMTF(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[0].rc) * 100 + 0.5f;
		if (pDlg->m_afPara.throuthFocusEnable != 0)
		{
			pDlg->m_dlgMtf.m_mtfPara.roiPara[1].mtfVal = (float)ImageProc::GetMTF(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[1].rc) * 100 + 0.5f;
			pDlg->m_dlgMtf.m_mtfPara.roiPara[2].mtfVal = (float)ImageProc::GetMTF(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[2].rc) * 100 + 0.5f;
			pDlg->m_dlgMtf.m_mtfPara.roiPara[3].mtfVal = (float)ImageProc::GetMTF(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[3].rc) * 100 + 0.5f;
			pDlg->m_dlgMtf.m_mtfPara.roiPara[4].mtfVal = (float)ImageProc::GetMTF(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[4].rc) * 100 + 0.5f;
		}
		delete[] yBuff;
		yBuff = NULL;
	}
	else if (strAlgo == _T("X_CHART_SFR"))
	{
#if 1
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		memset(yBuff, 0, imgWidth * imgHeight);
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CSize imgSize;
		imgSize.cx = imgWidth;
		imgSize.cy = imgHeight;
		CSize roiSize;
		roiSize.cx = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiWidth);//ROI区域宽度
		roiSize.cy = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiHeight);//ROI区域高度
		CSize maskSize;
		maskSize.cx = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskWidth);
		maskSize.cy = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskHeight);
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
		for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
		{
			if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) &&
				(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
				continue;

			CPoint roiCnPosi[4];
			roiCnPosi[0].x = 0;
			roiCnPosi[0].y = 0;
			roiCnPosi[1].x = 0;
			roiCnPosi[1].y = 0;
			roiCnPosi[2].x = 0;
			roiCnPosi[2].y = 0;
			roiCnPosi[3].x = 0;
			roiCnPosi[3].y = 0;
			float sfrVal[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			CString logMsg = _T("");
			if (i == 0)
			{
				CPoint tagPosi;
				tagPosi.x = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cX);
				tagPosi.y = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cY);
				GetXChartTargetCenter(yBuff, imgSize, tagPosi, maskSize, roiCnPosi, &logMsg);//获取中心CHAR
				if (logMsg != _T(""))
					m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
				float freq = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].freq);
                freq /= 2; //input Nyquist freq(half of sampling frequency)
				GetXChartSfrValue(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, roiCnPosi, roiSize, freq, sfrVal, &logMsg);//获取XCHAR图的SFR值
				ShowXChartSfrInfoROI(i, tagPosi, roiCnPosi, roiSize, sfrVal, pDlg->m_dlgSfr.m_xChartSfrPara);//显示
				float tmpRetVal = 0.0f;
				int tmpRetCnt = 0;
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
				{
					tmpRetVal += sfrVal[0];
					tmpRetCnt++;
				}
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == TRUE)
				{
					tmpRetVal += sfrVal[1];
					tmpRetCnt++;
				}
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
				{
					tmpRetVal += sfrVal[2];
					tmpRetCnt++;
				}
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == TRUE)
				{
					tmpRetVal += sfrVal[3];
					tmpRetCnt++;
				}
				ret = (float)tmpRetVal / tmpRetCnt;//取均值
			}
			else if (pDlg->m_afPara.throuthFocusEnable != 0)
			{
				if ((lu == i) || (ru == i) || (ld == i) || (rd == i))
				{
					CPoint tagPosi;
					tagPosi.x = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cX);
					tagPosi.y = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cY);
					GetXChartTargetCenter(yBuff, imgSize, tagPosi, maskSize, roiCnPosi, &logMsg);//获取CHAR图中心
					if (logMsg != _T(""))
						m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
					float freq = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].freq);
                    freq /= 2; //input Nyquist freq(half of sampling frequency)
					GetXChartSfrValue(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, roiCnPosi, roiSize, freq, sfrVal, &logMsg);//获取charSFR值
					ShowXChartSfrInfoROI(i, tagPosi, roiCnPosi, roiSize, sfrVal, pDlg->m_dlgSfr.m_xChartSfrPara);//显示
				}
			}
		}
		if (yBuff != NULL)
			delete[] yBuff;
		yBuff = NULL;
#else
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CPoint fov85Center[4], fov55Center[4], fov35Center[4], fovCn;
		GetAllTestTargetCenter(yBuff, imgWidth, imgHeight, fov85Center, fov55Center, fov35Center, &fovCn);//获取所有的中心点坐标
		AllROI_Sort(fov85Center, fov55Center, fov35Center, &fovCn, m_sortRoi);//排序
		ShowAllTargetNumber(m_sortRoi);//显示所有排完序的ROI

		if (pDlg->m_afPara.sfrDemosaic)
		{
			BYTE* pDemoscRGB24 = (BYTE*)malloc(imgWidth * imgHeight * 3);
			ImageProc::RawtoRGB24MotoDemosaic(imgInfo.Raw_buffer, pDemoscRGB24, imgWidth, imgHeight, imgInfo.RawFormat, imgInfo.RawBits);
			BYTE* pDemoscY = (BYTE*)malloc(imgWidth * imgHeight);
			ImageProc::RGB24toY(pDemoscRGB24, pDemoscY, imgWidth, imgHeight);
			for (int j=0; j<imgHeight; j++)
				memcpy(yBuff + j * imgWidth, pDemoscY + (imgHeight - 1 - j) * imgWidth, imgWidth);//????
			SAFE_DELETE(pDemoscY);
			SAFE_DELETE(pDemoscRGB24);
		}

		int freq = _ttoi(pDlg->m_afPara.sfrFreq);//界面设定的SFR频率
		int sfrRoiWidth = _ttoi(pDlg->m_afPara.sfrRoiWidth);//界面设定的ROI的宽度
		int sfrRoiHeight = _ttoi(pDlg->m_afPara.sfrRoiHeight);//界面设定的ROI的高度
		int sfrRoiOffsetX = _ttoi(pDlg->m_afPara.sfrRoiOffsetX);//界面设定的offsetx
		int sfrRoiOffsetY = _ttoi(pDlg->m_afPara.sfrRoiOffsetY);//界面设定的offsety

		SFR_PARA sfrPara;
		memset(&sfrPara, 0, sizeof(SFR_PARA));
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
		sfrPara.AllRoiPara[0].RightRoiEn = sfrPara.AllRoiPara[0].BottomRoiEn = TRUE;
		sfrPara.AllRoiPara[0].Freq = freq;
		if (pDlg->m_afPara.throuthFocusEnable != 0)
		{
			sfrPara.AllRoiPara[lu].RightRoiEn = sfrPara.AllRoiPara[lu].BottomRoiEn = TRUE;
			sfrPara.AllRoiPara[ru].LeftRoiEn = sfrPara.AllRoiPara[ru].BottomRoiEn = TRUE;
			sfrPara.AllRoiPara[ld].RightRoiEn = sfrPara.AllRoiPara[ld].TopRoiEn = TRUE;
			sfrPara.AllRoiPara[rd].LeftRoiEn = sfrPara.AllRoiPara[rd].TopRoiEn = TRUE;
			sfrPara.AllRoiPara[lu].Freq = freq;
			sfrPara.AllRoiPara[ru].Freq = freq;
			sfrPara.AllRoiPara[ld].Freq = freq;
			sfrPara.AllRoiPara[rd].Freq = freq;
		}
		sfrPara.RoiWidth = sfrRoiWidth;//传给SFR——PARA结构体
		sfrPara.RoiHeight = sfrRoiHeight;
		sfrPara.RoiOffsetX = sfrRoiOffsetX;
		sfrPara.RoiOffsetY = sfrRoiOffsetY;

		double sfrValueSum = 0.0f;
		int sfrValueCnt = 0;
		for (int i=0; i<20; i++)
		{
			if ((m_sortRoi[i].x == 0) || (m_sortRoi[i].y == 0))//等于0的话进行下一个
				continue;

			int roiOffsetX = sfrPara.RoiOffsetX + sfrPara.AllRoiPara[i].OffsetX;
			int roiOffsetY = sfrPara.RoiOffsetY + sfrPara.AllRoiPara[i].OffsetY;
			if (sfrPara.AllRoiPara[i].TopRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].topVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("TOP"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].topVal = GetSfrValueForROI(yBuff, i, _T("TOP"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);//获取顶端SFR值
				m_sfrRoiValue[i].topResult = (m_sfrRoiValue[i].topVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("TOP"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);//显示
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].topVal;
					sfrValueCnt++;
				}
			}

			if (sfrPara.AllRoiPara[i].BottomRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].bottomVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("BOTTOM"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].bottomVal = GetSfrValueForROI(yBuff, i, _T("BOTTOM"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);//获取底部SFR值
				m_sfrRoiValue[i].bottomResult = (m_sfrRoiValue[i].bottomVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("BOTTOM"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);//显示
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].bottomVal;
					sfrValueCnt++;
				}
			}

			if (sfrPara.AllRoiPara[i].LeftRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].leftVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("LEFT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].leftVal = GetSfrValueForROI(yBuff, i, _T("LEFT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);//获取左边SFR值
				m_sfrRoiValue[i].leftResult = (m_sfrRoiValue[i].leftVal > sfrPara.AllRoiPara[i].SpecV / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("LEFT"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);//显示SFR值
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].leftVal;
					sfrValueCnt++;
				}
			}

			if (sfrPara.AllRoiPara[i].RightRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].rightVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("RIGHT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].rightVal = GetSfrValueForROI(yBuff, i, _T("RIGHT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);//获取右边SFR值
				m_sfrRoiValue[i].rightResult = (m_sfrRoiValue[i].rightVal > sfrPara.AllRoiPara[i].SpecV / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("RIGHT"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);//显示SFR值
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].rightVal;
					sfrValueCnt++;
				}
			}
		}

		delete[] yBuff;
		yBuff = NULL;

		ret = (float)(sfrValueSum / sfrValueCnt);//取均值
#endif
	}
	else if (strAlgo == _T("MOTO_CHART_SFR"))
	{
#if 1
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];//申请区域
		memset(yBuff, 0, imgWidth * imgHeight);
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);//坐标转换
		CSize imgSize;
		imgSize.cx = imgWidth;//图片宽度
		imgSize.cy = imgHeight;//图片高度
		CSize roiSize;
		roiSize.cx = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiWidth);//界面设定的ROI宽度
		roiSize.cy = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiHeight);//界面设定的ROI高度
		CSize maskSize;
		maskSize.cx = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskWidth);
		maskSize.cy = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskHeight);
		int maskOffsetX = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskOffsetX);
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);//界面设定的左上
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);//界面设定的右上
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);//界面设定的左下
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);//界面设定的右下

		CPoint roiCnPosi[20];
		for (int i=0; i<20; i++)
		{
			roiCnPosi[i].x = 0;
			roiCnPosi[i].y = 0;
		}
		for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
		{
			pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal=0.0f;//new add 2017/5/6
		}
		float sfrVal = 0.0f;
		CString logMsg = _T("");
		int AutoROIRet = GetMotoChartTargetCenter(yBuff, imgSize, maskSize, maskOffsetX, roiCnPosi, &logMsg);//获取中心MOTOChar
		if (logMsg != _T(""))
		{
			CString temp = _T("");
			temp.Format(_T("Auto ROI Ret = %d"),AutoROIRet);
			logMsg += temp;
			m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
			bool ret = SaveBmp("D:\\FullNg.bmp",imgInfo.RGB24_buffer,imgWidth,imgHeight);//存储图片
			temp.Format(_T("Save bmp %s"),ret?_T("OK"):_T("NG"));
			m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		}

		for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
		{
			if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
				continue;

			if ((roiCnPosi[i].x == 0) || (roiCnPosi[i].y == 0))
				continue;
	
			if (pDlg->m_afPara.throuthFocusEnginerModeEnable == TRUE)
			{
				float freq = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].freq);//获取界面中所设定的频率


				int roiStartX = roiCnPosi[i].x - roiSize.cx / 2;//取得起始点坐标
				if (roiStartX % 2 != 0)
					roiStartX += 1;
				int roiStartY = roiCnPosi[i].y - roiSize.cy / 2;
				if (roiStartY % 2 != 0)
					roiStartY += 1;
				if (roiSize.cx % 2 != 0)
					roiSize.cx += 1;
				if (roiSize.cy % 2 != 0)
					roiSize.cy += 1;
				BYTE* ROIP10 = new BYTE[roiSize.cx * roiSize.cy * 2];//申请区域
				memset(ROIP10, 0, roiSize.cx * roiSize.cy * 2);//初始化
				//剪切图
				PCutImageBufferDll((BYTE*)imgInfo.Raw_buffer, imgInfo.imgwidth, imgInfo.imgheight, 2, roiStartX, roiStartY, roiSize.cx, roiSize.cy, ROIP10);

				BYTE* BlockBuffer = new BYTE[roiSize.cx * roiSize.cy * 2];
				memset(BlockBuffer, 0, roiSize.cx * roiSize.cy * 2);
				int outformat = m_pInterface->Ctrl_GetOutPutFormat(CamID);
				RawToGrayscaleImage_Raw10Dll(ROIP10, outformat, roiSize.cx, roiSize.cy, BlockBuffer);
				int grayLT = BlockBuffer[0] + BlockBuffer[1] * 256;
				int grayRT = BlockBuffer[roiSize.cx * 2 - 4] + BlockBuffer[roiSize.cx * 2 - 3] * 256;
				int grayLB = BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2] + BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2 + 1] * 256;
				int grayRB = BlockBuffer[roiSize.cx * roiSize.cy * 2 - 4] + BlockBuffer[roiSize.cx * roiSize.cy * 2 - 3] * 256;
				if ((abs(grayLT - grayLB) > ROI_IMAGE_CHECK_LEVEL) || (abs(grayRT - grayRB) > ROI_IMAGE_CHECK_LEVEL) || ((grayLT > grayRT) && (grayLB < grayRB)) || ((grayLT < grayRT) && (grayLB > grayRB)))
				{
					
					pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = ret = 0.0f;
					ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);//显示
					CString str=_T("");
					str.Format(_T("ROI %d 计算失败, (%d, %d, %d, %d)"),i, grayLT, grayRT, grayLB, grayRB);
					m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
					if (1)
					{
						CString path = _T("");
						path.Format(_T("D:\\ROI%.2dGrayScale.raw"), i);
						CStdioFile file;
						file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
						file.Write(BlockBuffer,  roiSize.cx * roiSize.cy * 2);
						file.Close();
					}

					SAFE_DELETE(BlockBuffer);
					SAFE_DELETE(ROIP10);
					if (yBuff != NULL)
						delete[] yBuff;
					yBuff = NULL;
					return ret;
				}

				CSFRData *SFRData = new CSFRData[roiSize.cx + 10];
				double SFRValue  = SFRMatTest(BlockBuffer, SFRData, roiSize.cx, roiSize.cy);
				double x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
				//double x = 1.0f / (2.0f * pDlg->m_sfrPara.AllRoiPara[i].Freq) / 0.0014f;
				double x = freq / 0.0014f;
				for (int k = 0; k < roiSize.cx -1; k++)
				{
					if (SFRData[k].freq < x)
					{
						x1 = SFRData[k].freq;
						y1 = SFRData[k].SFR;
						x2 = SFRData[k+1].freq;
						y2 = SFRData[k+1].SFR;
					}
				}
				SFRValue = (x2*y1 - x*y1 + x*y2 - x1*y2) / (x2 - x1);

				SAFE_DELETE(BlockBuffer);
				SAFE_DELETE(ROIP10);

				pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = ret = (float)SFRValue;
				ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);//显示ROI
			}//计算所有ROI
			else
				{//不计算所有ROI
				if (i == 0)
				{
					float freq = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].freq);//界面设定频率
					
					int roiStartX = roiCnPosi[i].x - roiSize.cx / 2;
					if (roiStartX % 2 != 0)
						roiStartX += 1;
					int roiStartY = roiCnPosi[i].y - roiSize.cy / 2;
					if (roiStartY % 2 != 0)
						roiStartY += 1;
					if (roiSize.cx % 2 != 0)
						roiSize.cx += 1;
					if (roiSize.cy % 2 != 0)
						roiSize.cy += 1;
					BYTE* ROIP10 = new BYTE[roiSize.cx * roiSize.cy * 2];
					memset(ROIP10, 0, roiSize.cx * roiSize.cy * 2);
					PCutImageBufferDll((BYTE*)imgInfo.Raw_buffer, imgInfo.imgwidth, imgInfo.imgheight, 2, roiStartX, roiStartY, roiSize.cx, roiSize.cy, ROIP10);

					BYTE* BlockBuffer = new BYTE[roiSize.cx * roiSize.cy * 2];
					memset(BlockBuffer, 0, roiSize.cx * roiSize.cy * 2);
					//int outformat = OUTFORMAT_BGGR;
					int outformat = m_pInterface->Ctrl_GetOutPutFormat(CamID);
					RawToGrayscaleImage_Raw10Dll(ROIP10, outformat, roiSize.cx, roiSize.cy, BlockBuffer);
					int grayLT = BlockBuffer[0] + BlockBuffer[1] * 256;
					int grayRT = BlockBuffer[roiSize.cx * 2 - 4] + BlockBuffer[roiSize.cx * 2 - 3] * 256;
					int grayLB = BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2] + BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2 + 1] * 256;
					int grayRB = BlockBuffer[roiSize.cx * roiSize.cy * 2 - 4] + BlockBuffer[roiSize.cx * roiSize.cy * 2 - 3] * 256;
					if ((abs(grayLT - grayLB) > ROI_IMAGE_CHECK_LEVEL) || (abs(grayRT - grayRB) > ROI_IMAGE_CHECK_LEVEL) || ((grayLT > grayRT) && (grayLB < grayRB)) || ((grayLT < grayRT) && (grayLB > grayRB)))
					{
						pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal = ret = 0.0f;
						ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);
						CString str=_T("");
						str.Format(_T("ROI %d 计算失败, (%d, %d, %d, %d)"),i, grayLT, grayRT, grayLB, grayRB);
						m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);

						if (1)
						{
							CString path = _T("");
							path.Format(_T("D:\\ROI%.2dGrayScale.raw"), i);
							CStdioFile file;
							file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
							file.Write(BlockBuffer,  roiSize.cx * roiSize.cy * 2);
							file.Close();
						}

						SAFE_DELETE(BlockBuffer);
						SAFE_DELETE(ROIP10);
						if (yBuff != NULL)
							delete[] yBuff;
						yBuff = NULL;
						return ret;
					}

					//////////////////////////////////////////////////////////////////////////
					//if (1)
					//{
					//	if (i==0)
					//	{
					//		char temp[512] = {0};
					//		memset(temp, 0, 512);
					//		printf(temp, "D:/ROI%.2dGrayScale.raw", i);
					//		CString path = _T("");
					//		path.Format(_T("D:\\ROI%.2dGrayScale.raw"), i);
					//		CStdioFile file;
					//		file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
					//		file.Write(BlockBuffer,  roiSize.cx * roiSize.cy * 2);
					//		file.Close();
					//	}
					//	//bSaveRawFile(temp, BlockBuffer, pDlg->m_sfrPara.RoiWidth, pDlg->m_sfrPara.RoiHeight, 2);
					//}
					//////////////////////////////////////////////////////////////////////////

					CSFRData *SFRData = new CSFRData[roiSize.cx + 10];
					double SFRValue  = SFRMatTest(BlockBuffer, SFRData, roiSize.cx, roiSize.cy);
					double x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
					//double x = 1.0f / (2.0f * pDlg->m_sfrPara.AllRoiPara[i].Freq) / 0.0014f;
					double x = freq / 0.0014f;
					for (int k = 0; k < roiSize.cx -1; k++)
					{
						if (SFRData[k].freq < x)
						{
							x1 = SFRData[k].freq;
							y1 = SFRData[k].SFR;
							x2 = SFRData[k+1].freq;
							y2 = SFRData[k+1].SFR;
						}
					}
					SFRValue = (x2*y1 - x*y1 + x*y2 - x1*y2) / (x2 - x1);

					SAFE_DELETE(BlockBuffer);
					SAFE_DELETE(ROIP10);

					pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal = ret = (float)SFRValue;
					ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);
				}
				else if ((pDlg->m_afPara.throuthFocusEnable != 0) && (m_afMode == _T("AF_MINI_STEP")))
				{
					if ((lu == i) || (ru == i) || (ld == i) || (rd == i))
					{
						float freq = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].freq);
					
						int roiStartX = roiCnPosi[i].x - roiSize.cx / 2;
						if (roiStartX % 2 != 0)
							roiStartX += 1;
						int roiStartY = roiCnPosi[i].y - roiSize.cy / 2;
						if (roiStartY % 2 != 0)
							roiStartY += 1;
						if (roiSize.cx % 2 != 0)
							roiSize.cx += 1;
						if (roiSize.cy % 2 != 0)
							roiSize.cy += 1;
						
						BYTE* ROIP10 = new BYTE[roiSize.cx * roiSize.cy * 2];
						memset(ROIP10, 0, roiSize.cx * roiSize.cy * 2);
						PCutImageBufferDll((BYTE*)imgInfo.Raw_buffer, imgInfo.imgwidth, imgInfo.imgheight, 2, roiStartX, roiStartY, roiSize.cx, roiSize.cy, ROIP10);

						BYTE* BlockBuffer = new BYTE[roiSize.cx * roiSize.cy * 2];
						memset(BlockBuffer, 0, roiSize.cx * roiSize.cy * 2);
						int outformat = m_pInterface->Ctrl_GetOutPutFormat(CamID);
						RawToGrayscaleImage_Raw10Dll(ROIP10, outformat, roiSize.cx, roiSize.cy, BlockBuffer);
						int grayLT = BlockBuffer[0] + BlockBuffer[1] * 256;
						int grayRT = BlockBuffer[roiSize.cx * 2 - 4] + BlockBuffer[roiSize.cx * 2 - 3] * 256;
						int grayLB = BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2] + BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2 + 1] * 256;
						int grayRB = BlockBuffer[roiSize.cx * roiSize.cy * 2 - 4] + BlockBuffer[roiSize.cx * roiSize.cy * 2 - 3] * 256;
						if ((abs(grayLT - grayLB) > ROI_IMAGE_CHECK_LEVEL) || (abs(grayRT - grayRB) > ROI_IMAGE_CHECK_LEVEL) || ((grayLT > grayRT) && (grayLB < grayRB)) || ((grayLT < grayRT) && (grayLB > grayRB)))
						{
							ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);
							CString str=_T("");
							str.Format(_T("ROI %d 计算失败, (%d, %d, %d, %d)"),i, grayLT, grayRT, grayLB, grayRB);
							m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
							if (1)
							{
								CString path = _T("");
								path.Format(_T("D:\\ROI%.2dGrayScale.raw"), i);
								CStdioFile file;
								file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
								file.Write(BlockBuffer, roiSize.cx * roiSize.cy * 2);
								file.Close();
							}
							if (i == lu)
								pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[lu].sfrVal = ret = 0.0f;
							else if (i == ru)
								pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ru].sfrVal = ret = 0.0f;
							else if (i == ld)
								pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ld].sfrVal = ret = 0.0f;
							else if (i == rd)
								pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[rd].sfrVal = ret = 0.0f;

							SAFE_DELETE(BlockBuffer);
							SAFE_DELETE(ROIP10);
							if (yBuff != NULL)
								delete[] yBuff;
							yBuff = NULL;
							return ret;
						}

						CSFRData *SFRData = new CSFRData[roiSize.cx + 10];
						double SFRValue  = SFRMatTest(BlockBuffer, SFRData, roiSize.cx, roiSize.cy);
						double x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
						//double x = 1.0f / (2.0f * pDlg->m_sfrPara.AllRoiPara[i].Freq) / 0.0014f;
						double x = freq / 0.0014f;
						for (int k = 0; k < roiSize.cx -1; k++)
						{
							if (SFRData[k].freq < x)
							{
								x1 = SFRData[k].freq;
								y1 = SFRData[k].SFR;
								x2 = SFRData[k+1].freq;
								y2 = SFRData[k+1].SFR;
							}
						}
						SFRValue = (x2*y1 - x*y1 + x*y2 - x1*y2) / (x2 - x1);

						SAFE_DELETE(BlockBuffer);
						SAFE_DELETE(ROIP10);

						ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], (float)SFRValue);

						if (i == lu)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[lu].sfrVal = (float)SFRValue;
						else if (i == ru)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ru].sfrVal = (float)SFRValue;
						else if (i == ld)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ld].sfrVal = (float)SFRValue;
						else if (i == rd)
							pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[rd].sfrVal = (float)SFRValue;
					}
				}
			}
		}
		if (yBuff != NULL)
			delete[] yBuff;
		yBuff = NULL;
#else
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		GetAllTestTargetCenterForMotoChart(yBuff, imgWidth, imgHeight, m_sortRoi);
		ShowAllTargetNumber(m_sortRoi);

		int freq = _ttoi(pDlg->m_afPara.sfrFreq);
		int sfrRoiWidth = _ttoi(pDlg->m_afPara.sfrRoiWidth);
		int sfrRoiHeight = _ttoi(pDlg->m_afPara.sfrRoiHeight);
		int sfrRoiOffsetX = _ttoi(pDlg->m_afPara.sfrRoiOffsetX);
		int sfrRoiOffsetY = _ttoi(pDlg->m_afPara.sfrRoiOffsetY);

		SFR_PARA sfrPara;
		memset(&sfrPara, 0, sizeof(SFR_PARA));
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
		sfrPara.AllRoiPara[0].TopRoiEn = sfrPara.AllRoiPara[0].BottomRoiEn = sfrPara.AllRoiPara[0].LeftRoiEn = sfrPara.AllRoiPara[0].RightRoiEn = TRUE;
		sfrPara.AllRoiPara[0].Freq = freq;
		if (pDlg->m_afPara.throuthFocusEnable != 0)
		{
			sfrPara.AllRoiPara[lu].TopRoiEn = sfrPara.AllRoiPara[lu].BottomRoiEn = sfrPara.AllRoiPara[lu].LeftRoiEn = sfrPara.AllRoiPara[lu].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[ru].TopRoiEn = sfrPara.AllRoiPara[ru].BottomRoiEn = sfrPara.AllRoiPara[ru].LeftRoiEn = sfrPara.AllRoiPara[ru].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[ld].TopRoiEn = sfrPara.AllRoiPara[ld].BottomRoiEn = sfrPara.AllRoiPara[ld].LeftRoiEn = sfrPara.AllRoiPara[ld].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[rd].TopRoiEn = sfrPara.AllRoiPara[rd].BottomRoiEn = sfrPara.AllRoiPara[rd].LeftRoiEn = sfrPara.AllRoiPara[rd].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[lu].Freq = freq;
			sfrPara.AllRoiPara[ru].Freq = freq;
			sfrPara.AllRoiPara[ld].Freq = freq;
			sfrPara.AllRoiPara[rd].Freq = freq;
		}
		sfrPara.RoiWidth = sfrRoiWidth;
		sfrPara.RoiHeight = sfrRoiHeight;
		sfrPara.RoiOffsetX = sfrRoiOffsetX;
		sfrPara.RoiOffsetY = sfrRoiOffsetY;

		for (int i=0; i<20; i++)
		{
			if ((m_sortRoi[i].x == 0) || (m_sortRoi[i].y == 0))
				continue;

			if ((sfrPara.AllRoiPara[i].TopRoiEn && sfrPara.AllRoiPara[i].BottomRoiEn && sfrPara.AllRoiPara[i].LeftRoiEn && sfrPara.AllRoiPara[i].RightRoiEn))
			{
				BYTE* ROIP10 = new BYTE[sfrPara.RoiWidth * sfrPara.RoiHeight * 2];
				memset(ROIP10, 0, sfrPara.RoiWidth * sfrPara.RoiHeight * 2);
				int roiStartX = m_sortRoi[i].x - sfrPara.RoiWidth/2 + sfrPara.RoiOffsetX;
				if (roiStartX % 2 != 0)
					roiStartX += 1;
				int roiStartY = m_sortRoi[i].y - sfrPara.RoiHeight/2 + sfrPara.RoiOffsetY;
				if (roiStartY % 2 != 0)
					roiStartY += 1;
				if (sfrPara.RoiWidth % 2 != 0)
					sfrPara.RoiWidth += 1;
				if (sfrPara.RoiHeight % 2 != 0)
					sfrPara.RoiHeight += 1;

				PCutImageBufferDll((BYTE*)imgInfo.Raw_buffer, imgInfo.imgwidth, imgInfo.imgheight, 2, roiStartX, roiStartY, sfrPara.RoiWidth, sfrPara.RoiHeight, ROIP10);		

				BYTE* BlockBuffer = new BYTE[sfrPara.RoiWidth * sfrPara.RoiHeight * 2];
				memset(BlockBuffer, 0, sfrPara.RoiWidth * sfrPara.RoiHeight * 2);
				int outformat = OUTFORMAT_BGGR;
				RawToGrayscaleImage_Raw10Dll(ROIP10, outformat, sfrPara.RoiWidth, sfrPara.RoiHeight, BlockBuffer);

				CSFRData *SFRData=new CSFRData[sfrPara.RoiWidth];
				double SFRValue  = SFRMatTest(BlockBuffer, SFRData, sfrPara.RoiWidth, sfrPara.RoiHeight);

				SAFE_DELETE(BlockBuffer);
				SAFE_DELETE(ROIP10);

				//SFRValue = GetSfrValueForROI_MotoChart(yBuff, i, imgWidth, imgHeight, sfrRoiOffsetX, sfrRoiOffsetY, sfrPara, m_sortRoi);
				m_sfrRoiValue[i].topVal = m_sfrRoiValue[i].bottomVal = m_sfrRoiValue[i].leftVal = m_sfrRoiValue[i].rightVal = SFRValue;

				m_sfrRoiValue[i].topResult = m_sfrRoiValue[i].bottomResult = m_sfrRoiValue[i].leftResult = m_sfrRoiValue[i].rightResult = (m_sfrRoiValue[i].topVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestMotoChartROI(i, sfrPara.RoiOffsetX, sfrPara.RoiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi[i], (float)m_sfrRoiValue[i].topVal);
			}
		}

		delete[] yBuff;
		yBuff = NULL;

		ret = (float)(m_sfrRoiValue[0].topVal);
#endif
	}
	else if (strAlgo == _T("SQUARE_CHART_SFR"))
	{
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		memset(yBuff, 0, imgWidth * imgHeight);
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CSize imgSize;
		imgSize.cx = imgWidth;
		imgSize.cy = imgHeight;
		CSize roiSize;
		roiSize.cx = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiWidth);
		roiSize.cy = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiHeight);
		CSize maskSize;
		maskSize.cx = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskWidth);
		maskSize.cy = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskHeight);
		int maskPhi = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskPhi);
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
		for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
		{
			if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
				continue;

			int type[2] = {0, 0};
			CPoint roiCnPosi[2];
			roiCnPosi[0].x = 0;
			roiCnPosi[0].y = 0;
			roiCnPosi[1].x = 0;
			roiCnPosi[1].y = 0;
			float sfrVal[2] = {0.0f, 0.0f};
			CString logMsg = _T("");
			if (i == 0)
			{
				CPoint tagPosi;
				tagPosi.x = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cX);
				tagPosi.y = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cY);
				GetSquareChartTargetCenter(yBuff, imgSize, tagPosi, maskSize, maskPhi, type, roiCnPosi, &logMsg);//获取中心
				if (logMsg != _T(""))
					m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
				float freq = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].freq);
                freq /= 2; //input Nyquist freq(half of sampling frequency)
				GetSquareChartSfrValueForOPPO(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, type, roiCnPosi, roiSize, freq, sfrVal, &logMsg);//获取SFR值
				//GetSquareChartSfrValueForWhiteBalance(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, type, roiCnPosi, roiSize, freq, sfrVal, &logMsg);
				if (type[0] == 0)
					pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = sfrVal[0];
				else
					pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = sfrVal[0];
				if (type[1] == 0)
					pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = sfrVal[1];
				else
					pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = sfrVal[1];
				ShowSquareChartSfrInfoROI(i, tagPosi, type, roiCnPosi, roiSize, sfrVal, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara);
				ret = (float)(sfrVal[0] + sfrVal[1]) / 2.0f;
			}
			else if (pDlg->m_afPara.throuthFocusEnable != 0)
			{
				if ((lu == i) || (ru == i) || (ld == i) || (rd == i))
				{
					CPoint tagPosi;
					tagPosi.x = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cX);
					tagPosi.y = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cY);
					GetSquareChartTargetCenter(yBuff, imgSize, tagPosi, maskSize, maskPhi, type, roiCnPosi, &logMsg);
					if (logMsg != _T(""))
						m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
					float freq = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].freq);
                    freq /= 2; //input Nyquist freq(half of sampling frequency)
					GetSquareChartSfrValueForOPPO(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, type, roiCnPosi, roiSize, freq, sfrVal, &logMsg);
					//GetSquareChartSfrValueForWhiteBalance(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, type, roiCnPosi, roiSize, freq, sfrVal, &logMsg);
					if (type[0] == 0)
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = sfrVal[0];
					else
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = sfrVal[0];
					if (type[1] == 0)
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = sfrVal[1];
					else
						pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = sfrVal[1];
					ShowSquareChartSfrInfoROI(i, tagPosi, type, roiCnPosi, roiSize, sfrVal, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara);
				}
			}
		}
		if (yBuff != NULL)
			delete[] yBuff;
		yBuff = NULL;
	}
	else
	{
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		pDlg->m_dlgMtf.m_mtfPara.roiPara[0].mtfVal = ret = (float)ImageProc::GetResolution_FV(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[0].rc);
		if (pDlg->m_afPara.throuthFocusEnable != 0)
		{
			pDlg->m_dlgMtf.m_mtfPara.roiPara[1].mtfVal = (float)ImageProc::GetResolution_FV(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[1].rc);
			pDlg->m_dlgMtf.m_mtfPara.roiPara[2].mtfVal = (float)ImageProc::GetResolution_FV(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[2].rc);
			pDlg->m_dlgMtf.m_mtfPara.roiPara[3].mtfVal = (float)ImageProc::GetResolution_FV(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[3].rc);
			pDlg->m_dlgMtf.m_mtfPara.roiPara[4].mtfVal = (float)ImageProc::GetResolution_FV(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[4].rc);
		}
		delete[] yBuff;
		yBuff = NULL;
	}

	if (algorithm.Find(_T("SFR")) == -1)
		ShowRoiInformation(pDlg->m_dlgMtf.m_mtfPara);

	return ret;
}

float AF_TestItem::GetRoiAfValueForFinalCheck(CString algorithm)
{
	float ret = 0.0f;

	ImageInfo imgInfo = *(m_pInterface->Ctrl_GetImgInfo(CamID));
	int imgWidth = imgInfo.imgwidth, imgHeight = imgInfo.imgheight;
    
	int w = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiWidth);
	int h = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiHeight);
	for (int i=0; i<5; i++)
	{
		int cx = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].cx);
		int cy = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].cy);
		pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.left = imgWidth * (cx - w / 2) / 100;
		pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.right = imgWidth * (cx + w / 2) / 100;
		pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.top = imgHeight * (cy - h / 2) / 100;
		pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.bottom = imgHeight * (cy + h / 2) / 100;
	}

	CString strAlgo = pDlg->m_afPara.avAlgorithm;
	if (strAlgo == _T("X_CHART_SFR"))
	{
#if 1
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		memset(yBuff, 0, imgWidth * imgHeight);
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CSize imgSize;
		imgSize.cx = imgWidth;
		imgSize.cy = imgHeight;
		CSize maskSize;
		maskSize.cx = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskWidth);
		maskSize.cy = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskHeight);
		for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
		{
			CSize roiSize;
			roiSize.cx = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiWidth);
			roiSize.cy = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiHeight);

			if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) &&
				(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
				continue;

			CPoint roiCnPosi[4];
			roiCnPosi[0].x = 0;
			roiCnPosi[0].y = 0;
			roiCnPosi[1].x = 0;
			roiCnPosi[1].y = 0;
			roiCnPosi[2].x = 0;
			roiCnPosi[2].y = 0;
			roiCnPosi[3].x = 0;
			roiCnPosi[3].y = 0;
			float sfrVal[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			CString logMsg = _T("");

			CPoint tagPosi;
			tagPosi.x = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cX);
			tagPosi.y = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cY);
			GetXChartTargetCenter(yBuff, imgSize, tagPosi, maskSize, roiCnPosi, &logMsg);
			if (logMsg != _T(""))
				m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
			float freq = (float)_ttof(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].freq);
            freq /= 2; //input Nyquist freq(half of sampling frequency)
#if 0	// Vovi Chart // maskSize : 600
			if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov == _T("80"))
			{
				roiSize.cx *= 2;
				roiSize.cy *= 2;
			}
#endif
			GetXChartSfrValue(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, roiCnPosi, roiSize, freq, sfrVal, &logMsg);

			pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop = sfrVal[0];
			pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom = sfrVal[1];
			pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft = sfrVal[2];
			pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight = sfrVal[3];
			ShowXChartSfrInfoROI(i, tagPosi, roiCnPosi, roiSize, sfrVal, pDlg->m_dlgSfr.m_xChartSfrPara);
			m_finalAvBuff[i].push_back(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop);
			m_finalAvBuff[i].push_back(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom);
			m_finalAvBuff[i].push_back(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft);
			m_finalAvBuff[i].push_back(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight);
		}
		if (yBuff != NULL)
			delete[] yBuff;
		yBuff = NULL;
#else
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CPoint fov85Center[4], fov55Center[4], fov35Center[4], fovCn;
		GetAllTestTargetCenter(yBuff, imgWidth, imgHeight, fov85Center, fov55Center, fov35Center, &fovCn);
		AllROI_Sort(fov85Center, fov55Center, fov35Center, &fovCn, m_sortRoi);
		ShowAllTargetNumber(m_sortRoi);

		if (pDlg->m_afPara.sfrDemosaic)
		{
			BYTE* pDemoscRGB24 = (BYTE*)malloc(imgWidth * imgHeight * 3);
			ImageProc::RawtoRGB24MotoDemosaic(imgInfo.Raw_buffer, pDemoscRGB24, imgWidth, imgHeight, imgInfo.RawFormat, imgInfo.RawBits);
			BYTE* pDemoscY = (BYTE*)malloc(imgWidth * imgHeight);
			ImageProc::RGB24toY(pDemoscRGB24, pDemoscY, imgWidth, imgHeight);
			for (int j=0; j<imgHeight; j++)
				memcpy(yBuff + j * imgWidth, pDemoscY + (imgHeight - 1 - j) * imgWidth, imgWidth);
			SAFE_DELETE(pDemoscY);
			SAFE_DELETE(pDemoscRGB24);
		}

		int freq = _ttoi(pDlg->m_afPara.sfrFreq);
		int sfrRoiWidth = _ttoi(pDlg->m_afPara.sfrRoiWidth);
		int sfrRoiHeight = _ttoi(pDlg->m_afPara.sfrRoiHeight);
		int sfrRoiOffsetX = _ttoi(pDlg->m_afPara.sfrRoiOffsetX);
		int sfrRoiOffsetY = _ttoi(pDlg->m_afPara.sfrRoiOffsetY);

		SFR_PARA sfrPara;
		memset(&sfrPara, 0, sizeof(SFR_PARA));
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
		sfrPara.AllRoiPara[0].RightRoiEn = sfrPara.AllRoiPara[0].BottomRoiEn = TRUE;
		sfrPara.AllRoiPara[0].Freq = freq;
		if (pDlg->m_afPara.throuthFocusEnable != 0)
		{
			sfrPara.AllRoiPara[lu].RightRoiEn = sfrPara.AllRoiPara[lu].BottomRoiEn = TRUE;
			sfrPara.AllRoiPara[ru].LeftRoiEn = sfrPara.AllRoiPara[ru].BottomRoiEn = TRUE;
			sfrPara.AllRoiPara[ld].RightRoiEn = sfrPara.AllRoiPara[ld].TopRoiEn = TRUE;
			sfrPara.AllRoiPara[rd].LeftRoiEn = sfrPara.AllRoiPara[rd].TopRoiEn = TRUE;
			sfrPara.AllRoiPara[lu].Freq = freq;
			sfrPara.AllRoiPara[ru].Freq = freq;
			sfrPara.AllRoiPara[ld].Freq = freq;
			sfrPara.AllRoiPara[rd].Freq = freq;
		}
		sfrPara.RoiWidth = sfrRoiWidth;
		sfrPara.RoiHeight = sfrRoiHeight;
		sfrPara.RoiOffsetX = sfrRoiOffsetX;
		sfrPara.RoiOffsetY = sfrRoiOffsetY;

		double sfrValueSum = 0.0f;
		int sfrValueCnt = 0;
		for (int i=0; i<20; i++)
		{
			if ((m_sortRoi[i].x == 0) || (m_sortRoi[i].y == 0))
				continue;

			int roiOffsetX = sfrPara.RoiOffsetX + sfrPara.AllRoiPara[i].OffsetX;
			int roiOffsetY = sfrPara.RoiOffsetY + sfrPara.AllRoiPara[i].OffsetY;
			if (sfrPara.AllRoiPara[i].TopRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].topVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("TOP"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].topVal = GetSfrValueForROI(yBuff, i, _T("TOP"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				m_sfrRoiValue[i].topResult = (m_sfrRoiValue[i].topVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("TOP"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].topVal;
					sfrValueCnt++;
				}
			}

			if (sfrPara.AllRoiPara[i].BottomRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].bottomVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("BOTTOM"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].bottomVal = GetSfrValueForROI(yBuff, i, _T("BOTTOM"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				m_sfrRoiValue[i].bottomResult = (m_sfrRoiValue[i].bottomVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("BOTTOM"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].bottomVal;
					sfrValueCnt++;
				}
			}

			if (sfrPara.AllRoiPara[i].LeftRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].leftVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("LEFT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].leftVal = GetSfrValueForROI(yBuff, i, _T("LEFT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				m_sfrRoiValue[i].leftResult = (m_sfrRoiValue[i].leftVal > sfrPara.AllRoiPara[i].SpecV / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("LEFT"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].leftVal;
					sfrValueCnt++;
				}
			}

			if (sfrPara.AllRoiPara[i].RightRoiEn)
			{
				if (pDlg->m_afPara.sfrRawBalance)
					m_sfrRoiValue[i].rightVal = GetSfrValueForRawBalanceROI(imgInfo.Raw_buffer, i, _T("RIGHT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				else
					m_sfrRoiValue[i].rightVal = GetSfrValueForROI(yBuff, i, _T("RIGHT"), imgWidth, imgHeight, roiOffsetX, roiOffsetY, sfrPara, m_sortRoi);
				m_sfrRoiValue[i].rightResult = (m_sfrRoiValue[i].rightVal > sfrPara.AllRoiPara[i].SpecV / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestROI(i, _T("RIGHT"), roiOffsetX, roiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi, m_sfrRoiValue);
				if (i == 0)
				{
					sfrValueSum += m_sfrRoiValue[i].rightVal;
					sfrValueCnt++;
				}
			}
		}

		delete[] yBuff;
		yBuff = NULL;

		ret = (float)(sfrValueSum / sfrValueCnt);
#endif
	}
	else if (strAlgo == _T("MOTO_CHART_SFR"))
	{
#if 1
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		memset(yBuff, 0, imgWidth * imgHeight);
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CSize imgSize;
		imgSize.cx = imgWidth;
		imgSize.cy = imgHeight;
		CSize roiSize;
		roiSize.cx = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiWidth);
		roiSize.cy = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiHeight);
		CSize maskSize;
		maskSize.cx = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskWidth);
		maskSize.cy = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskHeight);
		int maskOffsetX = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskOffsetX);
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);

		CPoint roiCnPosi[20];
		for (int i=0; i<20; i++)
		{
			roiCnPosi[i].x = 0;
			roiCnPosi[i].y = 0;		
		}
		for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
		{
			pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal=0.0f;//new add 2017/5/6
		}
		float sfrVal = 0.0f;
		CString logMsg = _T("");
		int AutoROIRet = GetMotoChartTargetCenter(yBuff, imgSize, maskSize, maskOffsetX, roiCnPosi, &logMsg);//获取所有ROI
		if (logMsg != _T(""))
		{
			CString temp = _T("");
			temp.Format(_T("AutoROI Ret = %d"),AutoROIRet);
			logMsg += temp;
			m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
		}

		for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
		{
			if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable == FALSE)
				continue;

			if ((roiCnPosi[i].x == 0) || (roiCnPosi[i].y == 0))
				continue;

			float freq = (float)_ttof(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].freq);


			int roiStartX = roiCnPosi[i].x - roiSize.cx / 2;
			if (roiStartX % 2 != 0)
				roiStartX += 1;
			int roiStartY = roiCnPosi[i].y - roiSize.cy / 2;
			if (roiStartY % 2 != 0)
				roiStartY += 1;
			if (roiSize.cx % 2 != 0)
				roiSize.cx += 1;
			if (roiSize.cy % 2 != 0)
				roiSize.cy += 1;

			BYTE* ROIP10 = new BYTE[roiSize.cx * roiSize.cy * 2];
			memset(ROIP10, 0, roiSize.cx * roiSize.cy * 2);
			PCutImageBufferDll((BYTE*)imgInfo.Raw_buffer, imgInfo.imgwidth, imgInfo.imgheight, 2, roiStartX, roiStartY, roiSize.cx, roiSize.cy, ROIP10);

			BYTE* BlockBuffer = new BYTE[roiSize.cx * roiSize.cy * 2];
			memset(BlockBuffer, 0, roiSize.cx * roiSize.cy * 2);
			int outformat = m_pInterface->Ctrl_GetOutPutFormat(CamID);
			RawToGrayscaleImage_Raw10Dll(ROIP10, outformat, roiSize.cx, roiSize.cy, BlockBuffer);
			int grayLT = BlockBuffer[0] + BlockBuffer[1] * 256;
			int grayRT = BlockBuffer[roiSize.cx * 2 - 4] + BlockBuffer[roiSize.cx * 2 - 3] * 256;
			int grayLB = BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2] + BlockBuffer[roiSize.cx * (roiSize.cy - 2) * 2 + 1] * 256;
			int grayRB = BlockBuffer[roiSize.cx * roiSize.cy * 2 - 4] + BlockBuffer[roiSize.cx * roiSize.cy * 2 - 3] * 256;
			if ((abs(grayLT - grayLB) > ROI_IMAGE_CHECK_LEVEL) || (abs(grayRT - grayRB) > ROI_IMAGE_CHECK_LEVEL) || ((grayLT > grayRT) && (grayLB < grayRB)) || ((grayLT < grayRT) && (grayLB > grayRB)))
			{
				SaveBmp("D:\\MotoNG.bmp",imgInfo.RGB24_buffer,imgInfo.imgwidth, imgInfo.imgheight);
				pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = ret = 0.0f;
				ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);
				CString str=_T("");
				str.Format(_T("ROI %d 计算失败, (%d, %d, %d, %d)"),i, grayLT, grayRT, grayLB, grayRB);
				m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
				if (1)
				{
					CString path = _T("");
					path.Format(_T("D:\\ROI%.2dGrayScale.raw"), i);
					CStdioFile file;
					file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
					file.Write(BlockBuffer,  roiSize.cx * roiSize.cy * 2);
					file.Close();
				}
				SAFE_DELETE(BlockBuffer);
				SAFE_DELETE(ROIP10);
				if (yBuff != NULL)
					delete[] yBuff;
				yBuff = NULL;
				return ret;
			}
			CSFRData *SFRData = new CSFRData[roiSize.cx];
			double SFRValue  = SFRMatTest(BlockBuffer, SFRData, roiSize.cx, roiSize.cy);
			double x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
			//double x = 1.0f / (2.0f * pDlg->m_sfrPara.AllRoiPara[i].Freq) / 0.0014f;
			double x = freq / 0.0014f;
			for (int k = 0; k < roiSize.cx -1; k++)
			{
				if (SFRData[k].freq < x)
				{
					x1 = SFRData[k].freq;
					y1 = SFRData[k].SFR;
					x2 = SFRData[k+1].freq;
					y2 = SFRData[k+1].SFR;
				}
			}
			SFRValue = (x2*y1 - x*y1 + x*y2 - x1*y2) / (x2 - x1);

			SAFE_DELETE(BlockBuffer);
			SAFE_DELETE(ROIP10);

			pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal = ret = (float)SFRValue;
			ShowSfrTestMotoChartROI(i, 0, 0, roiSize.cx, roiSize.cy, roiCnPosi[i], ret);
			m_finalAvBuff[i].push_back(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal);
		}

		delete[] yBuff;
		yBuff = NULL;
#else
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		GetAllTestTargetCenterForMotoChart(yBuff, imgWidth, imgHeight, m_sortRoi);
		ShowAllTargetNumber(m_sortRoi);

		int freq = _ttoi(pDlg->m_afPara.sfrFreq);
		int sfrRoiWidth = _ttoi(pDlg->m_afPara.sfrRoiWidth);
		int sfrRoiHeight = _ttoi(pDlg->m_afPara.sfrRoiHeight);
		int sfrRoiOffsetX = _ttoi(pDlg->m_afPara.sfrRoiOffsetX);
		int sfrRoiOffsetY = _ttoi(pDlg->m_afPara.sfrRoiOffsetY);

		SFR_PARA sfrPara;
		memset(&sfrPara, 0, sizeof(SFR_PARA));
		int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
		int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
		int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
		int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);
		sfrPara.AllRoiPara[0].TopRoiEn = sfrPara.AllRoiPara[0].BottomRoiEn = sfrPara.AllRoiPara[0].LeftRoiEn = sfrPara.AllRoiPara[0].RightRoiEn = TRUE;
		sfrPara.AllRoiPara[0].Freq = freq;
		if (pDlg->m_afPara.throuthFocusEnable != 0)
		{
			sfrPara.AllRoiPara[lu].TopRoiEn = sfrPara.AllRoiPara[lu].BottomRoiEn = sfrPara.AllRoiPara[lu].LeftRoiEn = sfrPara.AllRoiPara[lu].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[ru].TopRoiEn = sfrPara.AllRoiPara[ru].BottomRoiEn = sfrPara.AllRoiPara[ru].LeftRoiEn = sfrPara.AllRoiPara[ru].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[ld].TopRoiEn = sfrPara.AllRoiPara[ld].BottomRoiEn = sfrPara.AllRoiPara[ld].LeftRoiEn = sfrPara.AllRoiPara[ld].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[rd].TopRoiEn = sfrPara.AllRoiPara[rd].BottomRoiEn = sfrPara.AllRoiPara[rd].LeftRoiEn = sfrPara.AllRoiPara[rd].RightRoiEn = TRUE;
			sfrPara.AllRoiPara[lu].Freq = freq;
			sfrPara.AllRoiPara[ru].Freq = freq;
			sfrPara.AllRoiPara[ld].Freq = freq;
			sfrPara.AllRoiPara[rd].Freq = freq;
		}
		sfrPara.RoiWidth = sfrRoiWidth;
		sfrPara.RoiHeight = sfrRoiHeight;
		sfrPara.RoiOffsetX = sfrRoiOffsetX;
		sfrPara.RoiOffsetY = sfrRoiOffsetY;

		for (int i=0; i<20; i++)
		{
			if ((m_sortRoi[i].x == 0) || (m_sortRoi[i].y == 0))
				continue;

			if ((sfrPara.AllRoiPara[i].TopRoiEn && sfrPara.AllRoiPara[i].BottomRoiEn && sfrPara.AllRoiPara[i].LeftRoiEn && sfrPara.AllRoiPara[i].RightRoiEn))
			{
				BYTE* ROIP10 = new BYTE[sfrPara.RoiWidth * sfrPara.RoiHeight * 2];
				memset(ROIP10, 0, sfrPara.RoiWidth * sfrPara.RoiHeight * 2);
				int roiStartX = m_sortRoi[i].x - sfrPara.RoiWidth/2 + sfrPara.RoiOffsetX;
				if (roiStartX % 2 != 0)
					roiStartX += 1;
				int roiStartY = m_sortRoi[i].y - sfrPara.RoiHeight/2 + sfrPara.RoiOffsetY;
				if (roiStartY % 2 != 0)
					roiStartY += 1;
				if (sfrPara.RoiWidth % 2 != 0)
					sfrPara.RoiWidth += 1;
				if (sfrPara.RoiHeight % 2 != 0)
					sfrPara.RoiHeight += 1;

				PCutImageBufferDll((BYTE*)imgInfo.Raw_buffer, imgInfo.imgwidth, imgInfo.imgheight, 2, roiStartX, roiStartY, sfrPara.RoiWidth, sfrPara.RoiHeight, ROIP10);		

				BYTE* BlockBuffer = new BYTE[sfrPara.RoiWidth * sfrPara.RoiHeight * 2];
				memset(BlockBuffer, 0, sfrPara.RoiWidth * sfrPara.RoiHeight * 2);
				int outformat = OUTFORMAT_BGGR;
				RawToGrayscaleImage_Raw10Dll(ROIP10, outformat, sfrPara.RoiWidth, sfrPara.RoiHeight, BlockBuffer);

				CSFRData *SFRData=new CSFRData[sfrPara.RoiWidth];
				double SFRValue  = SFRMatTest(BlockBuffer, SFRData, sfrPara.RoiWidth, sfrPara.RoiHeight);

				SAFE_DELETE(BlockBuffer);
				SAFE_DELETE(ROIP10);

				//SFRValue = GetSfrValueForROI_MotoChart(yBuff, i, imgWidth, imgHeight, sfrRoiOffsetX, sfrRoiOffsetY, sfrPara, m_sortRoi);
				m_sfrRoiValue[i].topVal = m_sfrRoiValue[i].bottomVal = m_sfrRoiValue[i].leftVal = m_sfrRoiValue[i].rightVal = SFRValue;

				m_sfrRoiValue[i].topResult = m_sfrRoiValue[i].bottomResult = m_sfrRoiValue[i].leftResult = m_sfrRoiValue[i].rightResult = (m_sfrRoiValue[i].topVal > sfrPara.AllRoiPara[i].SpecH / 1000.0f) ? TRUE : FALSE;
				ShowSfrTestMotoChartROI(i, sfrPara.RoiOffsetX, sfrPara.RoiOffsetY, sfrPara.RoiWidth, sfrPara.RoiHeight, m_sortRoi[i], (float)m_sfrRoiValue[i].topVal);
			}
		}

		delete[] yBuff;
		yBuff = NULL;

		ret = (float)(m_sfrRoiValue[0].topVal);
#endif
	}
	else if (strAlgo == _T("SQUARE_CHART_SFR"))
	{
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		memset(yBuff, 0, imgWidth * imgHeight);
		ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		CSize imgSize;
		imgSize.cx = imgWidth;
		imgSize.cy = imgHeight;
		CSize roiSize;
		roiSize.cx = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiWidth);
		roiSize.cy = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiHeight);
		CSize maskSize;
		maskSize.cx = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskWidth);
		maskSize.cy = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskHeight);
		int maskPhi = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskPhi);
		for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
		{
			if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
				continue;

			int type[2] = {0, 0};
			CPoint roiCnPosi[2];
			roiCnPosi[0].x = 0;
			roiCnPosi[0].y = 0;
			roiCnPosi[1].x = 0;
			roiCnPosi[1].y = 0;
			float sfrVal[2] = {0.0f, 0.0f};
			CString logMsg = _T("");
			CPoint tagPosi;
			tagPosi.x = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cX);
			tagPosi.y = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cY);
			GetSquareChartTargetCenter(yBuff, imgSize, tagPosi, maskSize, maskPhi, type, roiCnPosi, &logMsg);
			if (logMsg != _T(""))
					m_pInterface->Ctrl_Addlog(CamID, logMsg, COLOR_BLUE, 200);
			float freq = (float)_ttof(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].freq);
            freq /= 2; //input Nyquist freq(half of sampling frequency)
			GetSquareChartSfrValueForOPPO(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, type, roiCnPosi, roiSize, freq, sfrVal, &logMsg);
			//GetSquareChartSfrValueForWhiteBalance(i, imgInfo.Raw_buffer, imgInfo.RawFormat, imgSize, type, roiCnPosi, roiSize, freq, sfrVal, &logMsg);
			if (type[0] == 0)
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = sfrVal[0];
			else
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = sfrVal[0];
			if (type[1] == 0)
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH = sfrVal[1];
			else
				pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV = sfrVal[1];
			ShowSquareChartSfrInfoROI(i, tagPosi, type, roiCnPosi, roiSize, sfrVal, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara);
			m_finalAvBuff[i].push_back(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH);
			m_finalAvBuff[i].push_back(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV);
		}
		if (yBuff != NULL)
			delete[] yBuff;
		yBuff = NULL;
	}
	else
	{
		BYTE* yBuff = new BYTE[imgWidth * imgHeight];
		

		if (pDlg->m_dlgMtf.m_mtfPara.bUseRaw8Test)
		{
			BYTE *pRaw8Buf = new BYTE[imgWidth*imgHeight];
			BYTE *pRGB24Buf = new BYTE[imgWidth*imgHeight*3+1024];
			if (NULL == pRaw8Buf || NULL == pRGB24Buf)
			{
				SAFE_DELETE_ARRAY(pRaw8Buf);
				SAFE_DELETE_ARRAY(pRGB24Buf);
				SAFE_DELETE_ARRAY(yBuff);
				CString Msg;
				Msg.Format(_T("%s malloc memory error!"),GetName());
				m_pInterface->Ctrl_Addlog(CamID,Msg, COLOR_RED,500);	
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,Msg);
				return 0.0;
			}
			ImageProc::RawToRaw8(imgInfo.Raw_buffer,pRaw8Buf, imgWidth, imgHeight, imgInfo.RawBits);
			ImageProc::Raw8toRGB24(pRaw8Buf, pRGB24Buf, imgWidth, imgHeight, imgInfo.RawFormat);
			ImageProc::RGB24toY(pRGB24Buf,yBuff,imgWidth,imgHeight);
			SAFE_DELETE_ARRAY(pRaw8Buf);
			SAFE_DELETE_ARRAY(pRGB24Buf);
		}
		else
		{
			ImageProc::RGB24toY(imgInfo.RGB24_buffer, yBuff, imgWidth, imgHeight);
		}
		for (int i=0; i<16; i++)
		{
			if (pDlg->m_dlgMtf.m_mtfPara.roiPara[i].enable == TRUE)
			{
				int cx = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].cx);
				int cy = _ttoi(pDlg->m_dlgMtf.m_mtfPara.roiPara[i].cy);
				pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.left = imgWidth * (cx - w / 2) / 100;
				pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.right = imgWidth * (cx + w / 2) / 100;
				pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.top = imgHeight * (cy - h / 2) / 100;
				pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc.bottom = imgHeight * (cy + h / 2) / 100;
				m_mtfValBuff[i].push_back((float)ImageProc::GetMTF(yBuff, imgWidth, imgHeight, pDlg->m_dlgMtf.m_mtfPara.roiPara[i].rc));
			}
		}
		delete[] yBuff;
		yBuff = NULL;
	}

	if (algorithm.Find(_T("SFR")) == -1)
		ShowRoiInformation(pDlg->m_dlgMtf.m_mtfPara);

	return ret;
}

void AF_TestItem::ShowRoiInformation(MTF_PARA mtfPara)
{
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);
	
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150, 255, 255)); 

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect, CamID);
	double Xratio = 1.0f * DisplayWndRect.Width() / img.imgwidth;
	double Yratio = 1.0f * DisplayWndRect.Height() / img.imgheight;

	int cx1 = 0, cy1 = 0, cx2 = 0, cy2 = 0;
	int px1 = 0, py1 = 0, px2 = 0, py2 = 0;
	COLORREF CurrentColor = COLOR_YELLOW;

	px1 = (int)(mtfPara.roiPara[0].rc.left * Xratio);
	py1 = (int)(mtfPara.roiPara[0].rc.top * Yratio);
	px2 = (int)(mtfPara.roiPara[0].rc.right * Xratio);
	py2 = (int)(mtfPara.roiPara[0].rc.bottom * Yratio);
	
	CPen pen(PS_SOLID,2,CurrentColor);
	pDisplayWndMemDC->SelectObject(&pen);
	pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

	pDisplayWndMemDC->SetTextColor(CurrentColor);
	CString strTmp = CString(_T(""));
	strTmp.Format(_T("%.2f"), mtfPara.roiPara[0].mtfVal);
	pDisplayWndMemDC->TextOut(px1 + 25, py1 + 10, strTmp);
	if (pDlg->m_afPara.throuthFocusEnable == TRUE)
	{
		for (int i=1; i<5; i++)
		{
			px1 = (int)(mtfPara.roiPara[i].rc.left * Xratio);
			py1 = (int)(mtfPara.roiPara[i].rc.top * Yratio);
			px2 = (int)(mtfPara.roiPara[i].rc.right * Xratio);
			py2 = (int)(mtfPara.roiPara[i].rc.bottom * Yratio);
	
			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

			pDisplayWndMemDC->SetTextColor(CurrentColor);
			CString strTmp = CString(_T(""));
			strTmp.Format(_T("%.2f"), mtfPara.roiPara[i].mtfVal);
			pDisplayWndMemDC->TextOut(px1 + 25, py1 + 10, strTmp);
		}
	}

	cx1 = 0;
	cy1 = img.imgheight / 2;
	px1 = (int)(cx1 * Xratio);
	py1 = (int)(cy1 * Yratio);

	cx2 = img.imgwidth - 1;
	cy2 = img.imgheight / 2;
	px2 = (int)(cx2 * Xratio);
	py2 = (int)(cy2 * Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	cx1 = img.imgwidth / 2;
	cy1 = 0;
	px1 = (int)(cx1 * Xratio);
	py1 = (int)(cy1 * Yratio);

	cx2 = img.imgwidth / 2;
	cy2 = img.imgheight - 1;
	px2 = (int)(cx2 * Xratio);
	py2 = (int)(cy2 * Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void AF_TestItem::ShowSfrTestROI(int roiNum, CString type, int offsetX, int offsetY, int roiWidth, int roiHeight, CPoint* sortRoi, SFR_ROI_VALUE* sfrRoiValue)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect, CamID);

	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150, 255, 255)); 

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0f * DisplayWndRect.Width() / ImgWidth;
	double Yratio = 1.0f * DisplayWndRect.Height() / ImgHeight;
	
	if ((sortRoi[roiNum].x == 0) || (sortRoi[roiNum].y == 0))
		return;

	COLORREF CurrentColor = COLOR_YELLOW;
	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	switch (roiNum)
	{
	case 2:
	case 3:
	case 6:
	case 7:
	case 10:
	case 11:
	case 14:
	case 15:
		if (type == CString(_T("TOP")))
		{
			cx1 = sortRoi[roiNum].x + offsetX - roiWidth / 2;
			cy1 = sortRoi[roiNum].y - offsetY - roiHeight / 2;
			cx2 = sortRoi[roiNum].x + offsetX + roiWidth / 2;
			cy2 = sortRoi[roiNum].y - offsetY + roiHeight / 2;

			CurrentColor = (sfrRoiValue[roiNum].topResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		if (type == CString(_T("BOTTOM")))
		{
			cx1 = sortRoi[roiNum].x - offsetX - roiWidth / 2;
			cy1 = sortRoi[roiNum].y + offsetY - roiHeight / 2;
			cx2 = sortRoi[roiNum].x - offsetX + roiWidth / 2;
			cy2 = sortRoi[roiNum].y + offsetY + roiHeight / 2;

			CurrentColor = (sfrRoiValue[roiNum].bottomResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		if (type == CString(_T("LEFT")))
		{
			cx1 = sortRoi[roiNum].x - offsetY - roiHeight / 2;
			cy1 = sortRoi[roiNum].y - offsetX - roiWidth / 2;
			cx2 = sortRoi[roiNum].x - offsetY + roiHeight / 2;
			cy2 = sortRoi[roiNum].y - offsetX + roiWidth / 2;

			CurrentColor = (sfrRoiValue[roiNum].leftResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		if (type == CString(_T("RIGHT")))
		{
			cx1 = sortRoi[roiNum].x + offsetY - roiHeight / 2;
			cy1 = sortRoi[roiNum].y + offsetX - roiWidth / 2;
			cx2 = sortRoi[roiNum].x + offsetY + roiHeight / 2;
			cy2 = sortRoi[roiNum].y + offsetX + roiWidth / 2;

			CurrentColor = (sfrRoiValue[roiNum].rightResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		break;
	default:
		if (type == CString(_T("TOP")))
		{
			cx1 = sortRoi[roiNum].x - offsetX - roiWidth / 2;
			cy1 = sortRoi[roiNum].y - offsetY - roiHeight / 2;
			cx2 = sortRoi[roiNum].x - offsetX + roiWidth / 2;
			cy2 = sortRoi[roiNum].y - offsetY + roiHeight / 2;

			CurrentColor = (sfrRoiValue[roiNum].topResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		if (type == CString(_T("BOTTOM")))
		{
			cx1 = sortRoi[roiNum].x + offsetX - roiWidth / 2;
			cy1 = sortRoi[roiNum].y + offsetY - roiHeight / 2;
			cx2 = sortRoi[roiNum].x + offsetX + roiWidth / 2;
			cy2 = sortRoi[roiNum].y + offsetY + roiHeight / 2;

			CurrentColor = (sfrRoiValue[roiNum].bottomResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		if (type == CString(_T("LEFT")))
		{
			cx1 = sortRoi[roiNum].x - offsetY - roiHeight / 2;
			cy1 = sortRoi[roiNum].y + offsetX - roiWidth / 2;
			cx2 = sortRoi[roiNum].x - offsetY + roiHeight / 2;
			cy2 = sortRoi[roiNum].y + offsetX + roiWidth / 2;

			CurrentColor = (sfrRoiValue[roiNum].leftResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		if (type == CString(_T("RIGHT")))
		{
			cx1 = sortRoi[roiNum].x + offsetY - roiHeight / 2;
			cy1 = sortRoi[roiNum].y - offsetX - roiWidth / 2;
			cx2 = sortRoi[roiNum].x + offsetY + roiHeight / 2;
			cy2 = sortRoi[roiNum].y - offsetX + roiWidth / 2;

			CurrentColor = (sfrRoiValue[roiNum].rightResult == TRUE) ? COLOR_GREEN : COLOR_RED;
		}
		break;
	}

	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	
	CPen pen(PS_SOLID,2,CurrentColor);
	pDisplayWndMemDC->SelectObject(&pen);
	pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

	pDisplayWndMemDC->SetTextColor(CurrentColor);
	CString strTmp = CString(_T(""));
	if (type == CString(_T("TOP")))
	{
		strTmp.Format(_T("%.3f"), sfrRoiValue[roiNum].topVal);
		pDisplayWndMemDC->TextOut(px1 - 5, py1 - 20, strTmp); 
	}
	if (type == CString(_T("BOTTOM")))
	{
		strTmp.Format(_T("%.3f"), sfrRoiValue[roiNum].bottomVal);
		pDisplayWndMemDC->TextOut(px1 - 5, py1 + 25, strTmp); 
	}
	if (type == CString(_T("LEFT")))
	{
		strTmp.Format(_T("%.3f"), sfrRoiValue[roiNum].leftVal);
		pDisplayWndMemDC->TextOut(px1 - 45, py1 + 10, strTmp); 
	}
	if (type == CString(_T("RIGHT")))
	{
		strTmp.Format(_T("%.3f"), sfrRoiValue[roiNum].rightVal);
		pDisplayWndMemDC->TextOut(px1 + 25, py1 + 10, strTmp); 
	}

	m_pInterface->Ctrl_ShowMemDC(CamID);

	cx1 = 0;
	cy1 = ImgHeight/2;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth-1;
	cy2 = ImgHeight/2;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	cx1 = ImgWidth/2;
	cy1 = 0;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth/2;
	cy2 = ImgHeight-1;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);
}

void AF_TestItem::ShowSfrTestMotoChartROI(int roiNum, int offsetX, int offsetY, int roiWidth, int roiHeight, CPoint roiCnPosi, float sfrVal)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	BOOL bShowCen = FALSE;
	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	
	if ((roiCnPosi.x == 0) || (roiCnPosi.y == 0))
		return;

	CurrentColor = COLOR_YELLOW;

	cx1 = roiCnPosi.x + offsetX - roiWidth / 2;
	cy1 = roiCnPosi.y - offsetY - roiHeight / 2;
	cx2 = roiCnPosi.x + offsetX + roiWidth / 2;
	cy2 = roiCnPosi.y - offsetY + roiHeight / 2;

	CurrentColor = (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[roiNum].enable == TRUE) ? COLOR_GREEN : COLOR_RED;

	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	
	CPen pen(PS_SOLID,2,CurrentColor);
	pDisplayWndMemDC->SelectObject(&pen);

	bShowCen = TRUE;
	if(bShowCen)
		pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

	pDisplayWndMemDC->SetTextColor(CurrentColor);
	CString strTmp = CString(_T(""));
	strTmp.Format(_T("%.4f"), sfrVal);
	pDisplayWndMemDC->TextOut(px1 - 5, py1 - 20, strTmp); 

	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void AF_TestItem::ShowAllTargetNumber(CPoint* sortROI)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	for (UINT i=0; i<21; i++)
	{
		if ((sortROI[i].x == 0) || (sortROI[i].y == 0))
			continue;

		CurrentColor = COLOR_YELLOW;
		CPen pen(PS_SOLID,2,CurrentColor);
		pDisplayWndMemDC->SelectObject(&pen);

		cx1 = sortROI[i].x - 100;
		cy1 = sortROI[i].y - 100;
		px1 = (int)(cx1* Xratio);
		py1 = (int)(cy1* Yratio);

		cx2 = sortROI[i].x + 100;
		cy2 = sortROI[i].y + 100;
		px2 = (int)(cx2* Xratio);
		py2 = (int)(cy2* Yratio);

		CurrentColor = COLOR_YELLOW;
		pDisplayWndMemDC->SetTextColor(CurrentColor);
		CString strTmp = CString(_T(""));
		strTmp.Format(_T("%d"), i);
		pDisplayWndMemDC->TextOut(px1 + 25,py1 + 25, strTmp);
	}

	m_pInterface->Ctrl_ShowMemDC(CamID);

	CPen pen(PS_SOLID, 2, COLOR_YELLOW);
	pDisplayWndMemDC->SelectObject(&pen);

	cx1 = 0;
	cy1 = ImgHeight/2;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth-1;
	cy2 = ImgHeight/2;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	cx1 = ImgWidth/2;
	cy1 = 0;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth/2;
	cy2 = ImgHeight-1;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);
}

void AF_TestItem::ShowAeRoiInfo(CRect* rc)
{
	CRect DisplayWndRect;//区域对象
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect, CamID);//得到显示区域

	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);//
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect, CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0f * DisplayWndRect.Width() / ImgWidth;
	double Yratio = 1.0f * DisplayWndRect.Height() / ImgHeight;

	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	
	CurrentColor = COLOR_YELLOW;
	CPen pen(PS_SOLID,2,CurrentColor);
	pDisplayWndMemDC->SelectObject(&pen);

	cx1 = rc->left;
	cy1 = rc->top;
	px1 = (int)(cx1 * Xratio);
	py1 = (int)(cy1 * Yratio);

	cx2 = rc->right;
	cy2 = rc->bottom;
	px2 = (int)(cx2 * Xratio);
	py2 = (int)(cy2 * Yratio);
	
	pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
	if ((m_aeBrightness > (m_afAeTarget + m_afAeRange)) || (m_aeBrightness < (m_afAeTarget - m_afAeRange)))
		pDisplayWndMemDC->SetTextColor(COLOR_RED);
	else
		pDisplayWndMemDC->SetTextColor(COLOR_BLUE);

	CString str = _T("");
	if (pDlg->m_afPara.ae.channel == _T("R"))
		str.Format(_T("R = %.04f"), m_aeBrightness);
	else if (pDlg->m_afPara.ae.channel == _T("Gr"))
		str.Format(_T("Gr = %.04f"), m_aeBrightness);
	else if (pDlg->m_afPara.ae.channel == _T("Gb"))
		str.Format(_T("Gb = %.04f"), m_aeBrightness);
	else if (pDlg->m_afPara.ae.channel == _T("B"))
		str.Format(_T("B = %.04f"), m_aeBrightness);
	else if (pDlg->m_afPara.ae.channel == _T("G"))
		str.Format(_T("G = %.04f"), m_aeBrightness);
	else
		str.Format(_T("Y = %.04f"), m_aeBrightness);
	pDisplayWndMemDC->TextOut((int)(0.4 * DisplayWndRect.Width()), (int)(0.5 * DisplayWndRect.Height()), str);

	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void AF_TestItem::ShowMtfRoiInfo(MTF_PARA mtfPara)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect, CamID);

	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect, CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0f * DisplayWndRect.Width() / ImgWidth;
	double Yratio = 1.0f * DisplayWndRect.Height() / ImgHeight;

	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	
	CurrentColor = COLOR_YELLOW;
	CPen pen(PS_SOLID,2,CurrentColor);
	pDisplayWndMemDC->SelectObject(&pen);

	for (int i=0; i<16; i++)
	{
		if (mtfPara.roiPara[i].enable == FALSE)
			continue;

		cx1 = mtfPara.roiPara[i].rc.left;
		cy1 = mtfPara.roiPara[i].rc.top;
		px1 = (int)(cx1 * Xratio);
		py1 = (int)(cy1 * Yratio);

		cx2 = mtfPara.roiPara[i].rc.right;
		cy2 = mtfPara.roiPara[i].rc.bottom;
		px2 = (int)(cx2 * Xratio);
		py2 = (int)(cy2 * Yratio);

		pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		if (mtfPara.roiPara[i].result == FALSE)
			pDisplayWndMemDC->SetTextColor(COLOR_RED);
		else
			pDisplayWndMemDC->SetTextColor(COLOR_BLUE);

		CString str = _T("");
		str.Format(_T("%.02f"), mtfPara.roiPara[i].mtfVal * 100.0f + 0.5);
		pDisplayWndMemDC->TextOut((int)(px1+10), (int)(py1+10), str);

		m_pInterface->Ctrl_ShowMemDC(CamID);
	}
}

void AF_TestItem::AppendAfBurnFile(CString strAlgo, int cnt, int code, PSFR_ROI_VALUE roiVal)
{
	int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
	int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
	int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
	int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);

	CString filePath = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile.ini");
	if (strAlgo == _T("X_CHART_SFR"))
	{
		CString key = _T(""), val = _T("");
		key.Format(_T("Position_%d"), cnt);
		val.Format(_T("%d"), code);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		
		key.Format(_T("CEN_%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[0].rightVal + roiVal[0].bottomVal) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("TL6_%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[lu].rightVal + roiVal[lu].bottomVal) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("TR6_%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[ru].leftVal + roiVal[ru].bottomVal) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("BR6_%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[rd].leftVal + roiVal[rd].topVal) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("BL6_%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[ld].rightVal + roiVal[ld].topVal) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
	}
	else if (strAlgo == _T("MOTO_CHART_SFR"))
	{
		CString key = _T(""), val = _T("");
		key.Format(_T("Position_%d"), cnt);
		val.Format(_T("%d"), code);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		
		key.Format(_T("CEN_%d"), cnt);
		val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[0].sfrVal * 100+0.5));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("TL6_%d"), cnt);
		val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[lu].sfrVal * 100+0.5));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("TR6_%d"), cnt);
		val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ru].sfrVal * 100+0.5));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("BR6_%d"), cnt);
		val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[rd].sfrVal * 100+0.5));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("BL6_%d"), cnt);
		val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[ld].sfrVal * 100+0.5));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
	}
	else if (strAlgo == _T("SQUARE_CHART_SFR"))
	{
		CString key = _T(""), val = _T("");
		key.Format(_T("Position_%d"), cnt);
		val.Format(_T("%d"), code);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		
		key.Format(_T("CEN_%d"), cnt);
		val.Format(_T("%d"), (int)((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[0].sfrValV) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("LU_%d"), cnt);
		val.Format(_T("%d"), (int)((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[lu].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[lu].sfrValV) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("RU_%d"), cnt);
		val.Format(_T("%d"), (int)((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ru].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ru].sfrValV) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("LD_%d"), cnt);
		val.Format(_T("%d"), (int)((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ld].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[ld].sfrValV) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("RD_%d"), cnt);
		val.Format(_T("%d"), (int)((pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[rd].sfrValH + pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[rd].sfrValV) * 50));
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
	}
	else
	{
		;
	}
}

void AF_TestItem::AppendAfBurnFileForFinalCheck(CString strAlgo)
{
	CString filePath = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile.ini");
	if (strAlgo == _T("X_CHART_SFR"))
	{
		CString key = _T(""), val = _T("");
		key.Format(_T("%s"), _T("Posture"));
		val.Format(_T("%d"), 1);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("%s"), _T("Peak"));
		val.Format(_T("%d"), m_afBestCode);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		
		int ctcFov = 0;
		for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
		{
			if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn == FALSE) &&
				(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == FALSE) && (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn == FALSE))
				continue;

			if (i == _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLU))
				ctcFov = _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10;
#if 1
			if (i == 0)
			{
				key = _T("CEN_H");
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
					val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop * 100));
				else
					val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom * 100));
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key = _T("CEN_V");
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
					val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft * 100));
				else
					val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight * 100));
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
			}
			else
			{
				if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov == _T(""))
					continue;

				if (i % 4 == 1)
				{
					key.Format(_T("TL%01d_H"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("TL%01d_V"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 2)
				{
					key.Format(_T("TR%01d_H"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("TR%01d_V"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 3)
				{
					key.Format(_T("BL%01d_H"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("BL%01d_V"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 0)
				{
					key.Format(_T("BR%01d_H"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValTop * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValBottom * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("BR%01d_V"), _ttoi(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov) / 10);
					if (pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn == TRUE)
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValLeft * 100));
					else
						val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrValRight * 100));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
			}
#else
			key.Format(_T("ROI_%02d_H"), i);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

			key.Format(_T("ROI_%02d_V"), i);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
#endif
		}

		if (pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.enable)
		{
			key.Format(_T("CTC%01d"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffVal * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

			key.Format(_T("CTC%01d_H"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValH * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

			key.Format(_T("CTC%01d_V"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffValV * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		}
	}
	else if (strAlgo == _T("MOTO_CHART_SFR"))
	{
		if (pDlg->m_afPara.throuthFocusEnable == 0)//不烧throuthFocus的话 数据置为0
		{
			for (int cnt=1;cnt<=15;cnt++)
			{
				CString key = _T(""), val = _T("");
				key.Format(_T("Position_%d"), cnt);
				val.Format(_T("%d"), 0);
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key.Format(_T("CEN_%d"), cnt);
				val.Format(_T("%d"), 0);
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key.Format(_T("TL6_%d"), cnt);
				val.Format(_T("%d"), 0);
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key.Format(_T("TR6_%d"), cnt);
				val.Format(_T("%d"), 0);
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key.Format(_T("BR6_%d"), cnt);
				val.Format(_T("%d"), 0);
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key.Format(_T("BL6_%d"), cnt);
				val.Format(_T("%d"), 0);
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
			}
		}
		CString key = _T(""), val = _T("");
		key.Format(_T("%s"), _T("Posture"));
		val.Format(_T("%d"), 1);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("%s"), _T("Peak"));
		val.Format(_T("%d"), m_afBestCode);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		
		int ctcFov = 0;
		for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
		{
			if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.enable == FALSE)
				continue;

			if (i == _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLU))
				ctcFov = _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov) / 10;
#if 1
			if (i == 0)
			{
				key = _T("CEN");
				val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal * 100+0.5));
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
			}
			else
			{
				if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov == _T(""))
					continue;

				if (i % 4 == 1)
				{
					key.Format(_T("TL%01d"), _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 2)
				{
					key.Format(_T("TR%01d"), _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 3)
				{
					key.Format(_T("BL%01d"), _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 0)
				{
					key.Format(_T("BR%01d"), _ttoi(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrVal * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
			}
#else
			key.Format(_T("ROI_%02d"), i);
			val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrValH * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
#endif
		}

		if (pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.enable)
		{
			key.Format(_T("CTC%01d"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffVal * 100+0.5));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		}
	}
	else if (strAlgo == _T("SQUARE_CHART_SFR"))
	{
		CString key = _T(""), val = _T("");
		key.Format(_T("%s"), _T("Posture"));
		val.Format(_T("%d"), 1);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

		key.Format(_T("%s"), _T("Peak"));
		val.Format(_T("%d"), m_afBestCode);
		WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		
		int ctcFov = 0;
		for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
		{
			if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable == FALSE)
				continue;

			if (i == _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLU))
				ctcFov = _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10;
#if 1
			if (i == 0)
			{
				key = _T("CEN_H");
				val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100 + 0.5));
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

				key = _T("CEN_V");
				val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100 + 0.5));
				WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
			}
			else
			{
				if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov == _T(""))
					continue;

				if (i % 4 == 1)
				{
					key.Format(_T("TL%01d_H"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100 + 0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("TL%01d_V"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100 + 0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 2)
				{
					key.Format(_T("TR%01d_H"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("TR%01d_V"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 3)
				{
					key.Format(_T("BL%01d_H"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("BL%01d_V"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
				if (i % 4 == 0)
				{
					key.Format(_T("BR%01d_H"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

					key.Format(_T("BR%01d_V"), _ttoi(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov) / 10);
					val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100+0.5));
					WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
				}
			}
#else
			key.Format(_T("ROI_%02d_H"), i);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValH * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

			key.Format(_T("ROI_%02d_V"), i);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrValV * 100));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
#endif
		}

		if (pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable)
		{
			key.Format(_T("CTC%01d"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffVal * 100+0.5));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

			key.Format(_T("CTC%01d_H"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValH * 100+0.5));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);

			key.Format(_T("CTC%01d_V"), ctcFov);
			val.Format(_T("%d"), (int)(pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffValV * 100+0.5));
			WritePrivateProfileString(pDlg->m_afPara.stationName, key, val, filePath);
		}
	}
	else
	{
		;
	}
}

float AF_TestItem::AE_GetCenterRoiBrightness(BYTE* pRaw8Buff, ImageInfo* pImgInfo, AE_PARA aePara, CRect* rc)
{
	float ret = 0;
	int imgWidth = pImgInfo->imgwidth, imgHeight = pImgInfo->imgheight;
	int startX = 0, endX = 0, startY = 0, endY = 0;
	if (aePara.roi == _T("3X3"))
	{
		startX = imgWidth / 3;
		endX = imgWidth * 2 / 3;
		startY = imgHeight / 3;
		endY = imgHeight * 2 / 3;
	}
	else if (aePara.roi == _T("5X5"))
	{
		startX = imgWidth * 2 / 5;
		endX = imgWidth * 3 / 5;
		startY = imgHeight * 2 / 5;
		endY = imgHeight * 3 / 5;
	}
	else if (aePara.roi == _T("7X7"))
	{
		startX = imgWidth * 3 / 7;
		endX = imgWidth * 4 / 7;
		startY = imgHeight * 3 / 7;
		endY = imgHeight * 4 / 7;
	}
	else if (aePara.roi == _T("8X8"))
	{
// 		startX = imgWidth  * 3 / 8;
// 		endX = imgWidth * 5 / 8;
// 		startY = imgHeight * 3 / 8;
// 		endY = imgHeight * 5 / 8;
		startX = imgWidth  * 7 / 16;
		endX = imgWidth * 9 / 16;
		startY = imgHeight * 7 / 16;
		endY = imgHeight * 9 / 16;
	}
	else if (aePara.roi == _T("9X9"))
	{
		startX = imgWidth * 4 / 9;
		endX = imgWidth * 5 / 9;
		startY = imgHeight * 4 / 9;
		endY = imgHeight * 5 / 9;
	}
	else
	{
		startX = 0;
		endX = imgWidth;
		startY = 0;
		endY = imgHeight;
	}

	if ((startX % 2) != 0)
		startX++;
	if ((endX % 2) == 0)
		endX--;
	if ((startY % 2) != 0)
		startY++;
	if ((endY % 2) == 0)
		endY--;

	rc->left = startX;
	rc->right = endX;
	rc->top = startY;
	rc->bottom = endY;

	float sumR = 0.0f, sumGr = 0.0f, sumGb = 0.0f, sumB = 0.0f;
	DWORD cnt = 0;
	if (pDlg->m_afPara.ae.over128)
	{
		switch (pImgInfo->RawFormat)
		{
		case OUTFORMAT_RGGB:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					int Gr = pRaw8Buff[j * imgWidth + (i + 1)];
					int Gb = pRaw8Buff[(j + 1) * imgWidth + i];
					if (((Gr + Gb) / 2.0f) > 128)
					{
						sumGr += Gr;
						sumGb += Gb;
						cnt++;
					}
				}
			}
			break;
		case OUTFORMAT_GRBG:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					int Gr = pRaw8Buff[j * imgWidth + i];
					int Gb = pRaw8Buff[(j + 1) * imgWidth + (i + 1)];
					if (((Gr + Gb) / 2.0f) > 128)
					{
						sumGr += Gr;
						sumGb += Gb;
						cnt++;
					}
				}
			}
			break;
		case OUTFORMAT_GBRG:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					int Gb = pRaw8Buff[j * imgWidth + i];
					int Gr = pRaw8Buff[(j + 1) * imgWidth + (i + 1)];
					if (((Gr + Gb) / 2.0f) > 128)
					{
						sumGr += Gr;
						sumGb += Gb;
						cnt++;
					}
				}
			}
			break;
		case OUTFORMAT_BGGR:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					int Gb = pRaw8Buff[j * imgWidth + (i + 1)];
					int Gr = pRaw8Buff[(j + 1) * imgWidth + i];
					if (((Gr + Gb) / 2.0f) > 128)
					{
						sumGr += Gr;
						sumGb += Gb;
						cnt++;
					}
				}
			}
			break;
		default:
			break;
		}

		if (cnt == 0)
			cnt = 1;

		float gR = sumGr / cnt;
		float gB = sumGb / cnt;
		ret = (gR + gB) / 2.0f;
	}
	else
	{
		switch (pImgInfo->RawFormat)
		{
		case OUTFORMAT_RGGB:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					sumR += pRaw8Buff[j * imgWidth + i];
					sumGr += pRaw8Buff[j * imgWidth + (i + 1)];
					sumGb += pRaw8Buff[(j + 1) * imgWidth + i];
					sumB += pRaw8Buff[j * imgWidth + i];
					cnt++;
				}
			}
			break;
		case OUTFORMAT_GRBG:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					sumR += pRaw8Buff[j * imgWidth + (i + 1)];
					sumGr += pRaw8Buff[j * imgWidth + i];
					sumGb += pRaw8Buff[(j + 1) * imgWidth + (i + 1)];
					sumB += pRaw8Buff[(j+1) * imgWidth + i];
					cnt++;
				}
			}
			break;
		case OUTFORMAT_GBRG:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					sumR += pRaw8Buff[(j + 1) * imgWidth + i];
					sumGr += pRaw8Buff[(j + 1) * imgWidth + (i + 1)];
					sumGb += pRaw8Buff[j * imgWidth + i];
					sumB += pRaw8Buff[j * imgWidth + (i + 1)];
					cnt++;
				}
			}
			break;
		case OUTFORMAT_BGGR:
			for (int j=startY; j< endY; j+=2)
			{
				for (int i=startX; i< endX; i+=2)
				{
					sumR += pRaw8Buff[(j + 1) * imgWidth + (i + 1)];
					sumGb += pRaw8Buff[j * imgWidth + (i + 1)];
					sumGr += pRaw8Buff[(j + 1) * imgWidth + i];
					sumB += pRaw8Buff[j * imgWidth + i];
					cnt++;
				}
			}
			break;
		default:
			break;
		}

		if (cnt == 0)
			cnt = 1;

		if (pDlg->m_afPara.ae.channel == _T("R"))
			ret = sumR / cnt;
		else if (pDlg->m_afPara.ae.channel == _T("Gr"))
			ret = sumGr / cnt;
		else if (pDlg->m_afPara.ae.channel == _T("Gb"))
			ret = sumGb / cnt;
		else if (pDlg->m_afPara.ae.channel == _T("B"))
			ret = sumB / cnt;
		else if (pDlg->m_afPara.ae.channel == _T("G"))
			ret = (sumGr + sumGb) / (2.0f * cnt);
		else if (pDlg->m_afPara.ae.channel == _T("Y"))
		{
			float R = sumR / cnt;
			float G = (sumGr + sumGb) / (2.0f * cnt);
			float B = sumB / cnt;
			ret = (R * 299.0f + G * 587.0f + B * 114.0f) / 1000.0f;
		}
		else
			ret = 0.0f;
	}

	return ret;
}

void AF_TestItem::SaveMtfPara()
{
	WriteConfigString(_T("AF_MTF_ROI_WIDTH"), pDlg->m_dlgMtf.m_mtfPara.roiWidth);
	WriteConfigString(_T("AF_MTF_ROI_HEIGHT"), pDlg->m_dlgMtf.m_mtfPara.roiHeight);
	WriteConfigString(_T("AF_MTF_CORNER_DIFF"), pDlg->m_dlgMtf.m_mtfPara.cornerDiffSpec);

	WriteConfigInt(_T("AF_MTF_ROI00_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[0].enable);
	WriteConfigString(_T("AF_MTF_ROI00_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[0].cx);
	WriteConfigString(_T("AF_MTF_ROI00_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[0].cy);
	WriteConfigString(_T("AF_MTF_ROI00_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[0].limit1);
	WriteConfigString(_T("AF_MTF_ROI00_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[0].limit2);

	WriteConfigInt(_T("AF_MTF_ROI01_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[1].enable);
	WriteConfigString(_T("AF_MTF_ROI01_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[1].cx);
	WriteConfigString(_T("AF_MTF_ROI01_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[1].cy);
	WriteConfigString(_T("AF_MTF_ROI01_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[1].limit1);
	WriteConfigString(_T("AF_MTF_ROI01_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[1].limit2);

	WriteConfigInt(_T("AF_MTF_ROI02_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[2].enable);
	WriteConfigString(_T("AF_MTF_ROI02_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[2].cx);
	WriteConfigString(_T("AF_MTF_ROI02_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[2].cy);
	WriteConfigString(_T("AF_MTF_ROI02_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[2].limit1);
	WriteConfigString(_T("AF_MTF_ROI02_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[2].limit2);

	WriteConfigInt(_T("AF_MTF_ROI03_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[3].enable);
	WriteConfigString(_T("AF_MTF_ROI03_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[3].cx);
	WriteConfigString(_T("AF_MTF_ROI03_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[3].cy);
	WriteConfigString(_T("AF_MTF_ROI03_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[3].limit1);
	WriteConfigString(_T("AF_MTF_ROI03_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[3].limit2);

	WriteConfigInt(_T("AF_MTF_ROI04_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[4].enable);
	WriteConfigString(_T("AF_MTF_ROI04_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[4].cx);
	WriteConfigString(_T("AF_MTF_ROI04_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[4].cy);
	WriteConfigString(_T("AF_MTF_ROI04_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[4].limit1);
	WriteConfigString(_T("AF_MTF_ROI04_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[4].limit2);

	WriteConfigInt(_T("AF_MTF_ROI05_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[5].enable);
	WriteConfigString(_T("AF_MTF_ROI05_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[5].cx);
	WriteConfigString(_T("AF_MTF_ROI05_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[5].cy);
	WriteConfigString(_T("AF_MTF_ROI05_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[5].limit1);
	WriteConfigString(_T("AF_MTF_ROI05_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[5].limit2);

	WriteConfigInt(_T("AF_MTF_ROI06_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[6].enable);
	WriteConfigString(_T("AF_MTF_ROI06_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[6].cx);
	WriteConfigString(_T("AF_MTF_ROI06_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[6].cy);
	WriteConfigString(_T("AF_MTF_ROI06_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[6].limit1);
	WriteConfigString(_T("AF_MTF_ROI06_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[6].limit2);

	WriteConfigInt(_T("AF_MTF_ROI07_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[7].enable);
	WriteConfigString(_T("AF_MTF_ROI07_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[7].cx);
	WriteConfigString(_T("AF_MTF_ROI07_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[7].cy);
	WriteConfigString(_T("AF_MTF_ROI07_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[7].limit1);
	WriteConfigString(_T("AF_MTF_ROI07_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[7].limit2);

	WriteConfigInt(_T("AF_MTF_ROI08_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[8].enable);
	WriteConfigString(_T("AF_MTF_ROI08_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[8].cx);
	WriteConfigString(_T("AF_MTF_ROI08_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[8].cy);
	WriteConfigString(_T("AF_MTF_ROI08_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[8].limit1);
	WriteConfigString(_T("AF_MTF_ROI08_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[8].limit2);

	WriteConfigInt(_T("AF_MTF_ROI09_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[9].enable);
	WriteConfigString(_T("AF_MTF_ROI09_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[9].cx);
	WriteConfigString(_T("AF_MTF_ROI09_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[9].cy);
	WriteConfigString(_T("AF_MTF_ROI09_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[9].limit1);
	WriteConfigString(_T("AF_MTF_ROI09_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[9].limit2);

	WriteConfigInt(_T("AF_MTF_ROI10_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[10].enable);
	WriteConfigString(_T("AF_MTF_ROI10_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[10].cx);
	WriteConfigString(_T("AF_MTF_ROI10_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[10].cy);
	WriteConfigString(_T("AF_MTF_ROI10_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[10].limit1);
	WriteConfigString(_T("AF_MTF_ROI10_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[10].limit2);

	WriteConfigInt(_T("AF_MTF_ROI11_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[11].enable);
	WriteConfigString(_T("AF_MTF_ROI11_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[11].cx);
	WriteConfigString(_T("AF_MTF_ROI11_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[11].cy);
	WriteConfigString(_T("AF_MTF_ROI11_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[11].limit1);
	WriteConfigString(_T("AF_MTF_ROI11_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[11].limit2);

	WriteConfigInt(_T("AF_MTF_ROI12_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[12].enable);
	WriteConfigString(_T("AF_MTF_ROI12_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[12].cx);
	WriteConfigString(_T("AF_MTF_ROI12_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[12].cy);
	WriteConfigString(_T("AF_MTF_ROI12_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[12].limit1);
	WriteConfigString(_T("AF_MTF_ROI12_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[12].limit2);

	WriteConfigInt(_T("AF_MTF_ROI13_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[13].enable);
	WriteConfigString(_T("AF_MTF_ROI13_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[13].cx);
	WriteConfigString(_T("AF_MTF_ROI13_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[13].cy);
	WriteConfigString(_T("AF_MTF_ROI13_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[13].limit1);
	WriteConfigString(_T("AF_MTF_ROI13_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[13].limit2);

	WriteConfigInt(_T("AF_MTF_ROI14_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[14].enable);
	WriteConfigString(_T("AF_MTF_ROI14_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[14].cx);
	WriteConfigString(_T("AF_MTF_ROI14_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[14].cy);
	WriteConfigString(_T("AF_MTF_ROI14_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[14].limit1);
	WriteConfigString(_T("AF_MTF_ROI14_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[14].limit2);

	WriteConfigInt(_T("AF_MTF_ROI15_ENABLE"), pDlg->m_dlgMtf.m_mtfPara.roiPara[15].enable);
	WriteConfigString(_T("AF_MTF_ROI15_X"), pDlg->m_dlgMtf.m_mtfPara.roiPara[15].cx);
	WriteConfigString(_T("AF_MTF_ROI15_Y"), pDlg->m_dlgMtf.m_mtfPara.roiPara[15].cy);
	WriteConfigString(_T("AF_MTF_ROI15_LIMIT1"), pDlg->m_dlgMtf.m_mtfPara.roiPara[15].limit1);
	WriteConfigString(_T("AF_MTF_ROI15_LIMIT2"), pDlg->m_dlgMtf.m_mtfPara.roiPara[15].limit2);
}

void AF_TestItem::GetMtfPara()
{
	pDlg->m_dlgMtf.m_mtfPara.roiWidth = ReadConfigString(_T("AF_MTF_ROI_WIDTH"), _T("10"));
	pDlg->m_dlgMtf.m_mtfPara.roiHeight = ReadConfigString(_T("AF_MTF_ROI_HEIGHT"), _T("10"));
	pDlg->m_dlgMtf.m_mtfPara.cornerDiffSpec = ReadConfigString(_T("AF_MTF_CORNER_DIFF"), _T("100"));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[0].enable = ReadConfigInt(_T("AF_MTF_ROI00_ENABLE"), 1);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[0].cx = ReadConfigString(_T("AF_MTF_ROI00_X"), _T("50"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[0].cy = ReadConfigString(_T("AF_MTF_ROI00_Y"), _T("50"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[0].limit1 = ReadConfigString(_T("AF_MTF_ROI00_LIMIT1"), _T("40"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[0].limit2 = ReadConfigString(_T("AF_MTF_ROI00_LIMIT2"), _T("30"));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[1].enable = ReadConfigInt(_T("AF_MTF_ROI01_ENABLE"), 1);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[1].cx = ReadConfigString(_T("AF_MTF_ROI01_X"), _T("20"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[1].cy = ReadConfigString(_T("AF_MTF_ROI01_Y"), _T("20"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[1].limit1 = ReadConfigString(_T("AF_MTF_ROI01_LIMIT1"), _T("40"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[1].limit2 = ReadConfigString(_T("AF_MTF_ROI01_LIMIT2"), _T("30"));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[2].enable = ReadConfigInt(_T("AF_MTF_ROI02_ENABLE"), 1);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[2].cx = ReadConfigString(_T("AF_MTF_ROI02_X"), _T("80"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[2].cy = ReadConfigString(_T("AF_MTF_ROI02_Y"), _T("20"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[2].limit1 = ReadConfigString(_T("AF_MTF_ROI02_LIMIT1"), _T("40"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[2].limit2 = ReadConfigString(_T("AF_MTF_ROI02_LIMIT2"), _T("30"));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[3].enable = ReadConfigInt(_T("AF_MTF_ROI03_ENABLE"), 1);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[3].cx = ReadConfigString(_T("AF_MTF_ROI03_X"), _T("20"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[3].cy = ReadConfigString(_T("AF_MTF_ROI03_Y"), _T("80"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[3].limit1 = ReadConfigString(_T("AF_MTF_ROI03_LIMIT1"), _T("40"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[3].limit2 = ReadConfigString(_T("AF_MTF_ROI03_LIMIT2"), _T("30"));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[4].enable = ReadConfigInt(_T("AF_MTF_ROI04_ENABLE"), 1);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[4].cx = ReadConfigString(_T("AF_MTF_ROI04_X"), _T("80"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[4].cy = ReadConfigString(_T("AF_MTF_ROI04_Y"), _T("80"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[4].limit1 = ReadConfigString(_T("AF_MTF_ROI04_LIMIT1"), _T("40"));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[4].limit2 = ReadConfigString(_T("AF_MTF_ROI04_LIMIT2"), _T("30"));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[5].enable = ReadConfigInt(_T("AF_MTF_ROI05_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[5].cx = ReadConfigString(_T("AF_MTF_ROI05_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[5].cy = ReadConfigString(_T("AF_MTF_ROI05_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[5].limit1 = ReadConfigString(_T("AF_MTF_ROI05_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[5].limit2 = ReadConfigString(_T("AF_MTF_ROI05_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[6].enable = ReadConfigInt(_T("AF_MTF_ROI06_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[6].cx = ReadConfigString(_T("AF_MTF_ROI06_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[6].cy = ReadConfigString(_T("AF_MTF_ROI06_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[6].limit1 = ReadConfigString(_T("AF_MTF_ROI06_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[6].limit2 = ReadConfigString(_T("AF_MTF_ROI06_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[7].enable = ReadConfigInt(_T("AF_MTF_ROI07_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[7].cx = ReadConfigString(_T("AF_MTF_ROI07_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[7].cy = ReadConfigString(_T("AF_MTF_ROI07_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[7].limit1 = ReadConfigString(_T("AF_MTF_ROI07_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[7].limit2 = ReadConfigString(_T("AF_MTF_ROI07_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[8].enable = ReadConfigInt(_T("AF_MTF_ROI08_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[8].cx = ReadConfigString(_T("AF_MTF_ROI08_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[8].cy = ReadConfigString(_T("AF_MTF_ROI08_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[8].limit1 = ReadConfigString(_T("AF_MTF_ROI08_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[8].limit2 = ReadConfigString(_T("AF_MTF_ROI08_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[9].enable = ReadConfigInt(_T("AF_MTF_ROI09_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[9].cx = ReadConfigString(_T("AF_MTF_ROI09_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[9].cy = ReadConfigString(_T("AF_MTF_ROI09_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[9].limit1 = ReadConfigString(_T("AF_MTF_ROI09_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[9].limit2 = ReadConfigString(_T("AF_MTF_ROI09_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[10].enable = ReadConfigInt(_T("AF_MTF_ROI10_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[10].cx = ReadConfigString(_T("AF_MTF_ROI10_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[10].cy = ReadConfigString(_T("AF_MTF_ROI10_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[10].limit1 = ReadConfigString(_T("AF_MTF_ROI10_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[10].limit2 = ReadConfigString(_T("AF_MTF_ROI10_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[11].enable = ReadConfigInt(_T("AF_MTF_ROI11_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[11].cx = ReadConfigString(_T("AF_MTF_ROI11_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[11].cy = ReadConfigString(_T("AF_MTF_ROI11_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[11].limit1 = ReadConfigString(_T("AF_MTF_ROI11_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[11].limit2 = ReadConfigString(_T("AF_MTF_ROI11_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[12].enable = ReadConfigInt(_T("AF_MTF_ROI12_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[12].cx = ReadConfigString(_T("AF_MTF_ROI12_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[12].cy = ReadConfigString(_T("AF_MTF_ROI12_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[12].limit1 = ReadConfigString(_T("AF_MTF_ROI12_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[12].limit2 = ReadConfigString(_T("AF_MTF_ROI12_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[13].enable = ReadConfigInt(_T("AF_MTF_ROI13_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[13].cx = ReadConfigString(_T("AF_MTF_ROI13_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[13].cy = ReadConfigString(_T("AF_MTF_ROI13_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[13].limit1 = ReadConfigString(_T("AF_MTF_ROI13_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[13].limit2 = ReadConfigString(_T("AF_MTF_ROI13_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[14].enable = ReadConfigInt(_T("AF_MTF_ROI14_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[14].cx = ReadConfigString(_T("AF_MTF_ROI14_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[14].cy = ReadConfigString(_T("AF_MTF_ROI14_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[14].limit1 = ReadConfigString(_T("AF_MTF_ROI14_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[14].limit2 = ReadConfigString(_T("AF_MTF_ROI14_LIMIT2"), _T(""));

	pDlg->m_dlgMtf.m_mtfPara.roiPara[15].enable = ReadConfigInt(_T("AF_MTF_ROI15_ENABLE"), 0);
	pDlg->m_dlgMtf.m_mtfPara.roiPara[15].cx = ReadConfigString(_T("AF_MTF_ROI15_X"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[15].cy = ReadConfigString(_T("AF_MTF_ROI15_Y"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[15].limit1 = ReadConfigString(_T("AF_MTF_ROI15_LIMIT1"), _T(""));
	pDlg->m_dlgMtf.m_mtfPara.roiPara[15].limit2 = ReadConfigString(_T("AF_MTF_ROI15_LIMIT2"), _T(""));
}

void AF_TestItem::SaveSquareChartSfrPara()
{
	WriteConfigInt(_T("AF_SQUARE_CHART_SFR_ENABLE"), pDlg->m_dlgSquareChartSfr.m_roiEnable);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_IMG_WIDTH"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.imgWidth);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_IMG_HEIGHT"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.imgHeight);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_FOV"), pDlg->m_dlgSquareChartSfr.m_addParaByFov);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_PX"), pDlg->m_dlgSquareChartSfr.m_pX);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_PY"), pDlg->m_dlgSquareChartSfr.m_pY);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_OFFSET_H"), pDlg->m_dlgSquareChartSfr.m_sfrOffsetH);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_OFFSET_V"), pDlg->m_dlgSquareChartSfr.m_sfrOffsetV);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_SPEC_H"), pDlg->m_dlgSquareChartSfr.m_specH);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_SPEC_V"), pDlg->m_dlgSquareChartSfr.m_specV);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_MEMO"), pDlg->m_dlgSquareChartSfr.m_memo);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_FREQ"), pDlg->m_dlgSquareChartSfr.m_freq);
	WriteConfigInt(_T("AF_SQUARE_CHART_SFR_ROI_COUNT"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt);

	WriteConfigString(_T("AF_SQUARE_CHART_SFR_ROI_WIDTH"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiWidth);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_ROI_HEIGHT"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiHeight);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_MASK_WIDTH"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskWidth);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_MASK_HEIGHT"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskHeight);
	WriteConfigString(_T("AF_SQUARE_CHART_SFR_MASK_PHI"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskPhi);

	WriteConfigInt(_T("AF_SQUARE_CHART_CORNER_DIFF_ENABLE"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable);
	WriteConfigInt(_T("AF_SQUARE_CHART_CORNER_DIFF_ABSOLUTE"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffAbsolute);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_SPEC"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.spec);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_SPEC_H"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specH);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_SPEC_V"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specV);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_OFFSET"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffOffset);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_OFFSET_H"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffOffsetH);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_OFFSET_V"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffOffsetV);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_LU"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLU);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_RU"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumRU);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_LD"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLD);
	WriteConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_RD"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumRD);

	for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
	{
		CString tmpKey = _T(""), tmpVal = _T("");
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_ENABLE"), i);
		tmpVal.Format(_T("%d"), pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable);
		WriteConfigString(tmpKey, tmpVal);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_PX"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cX);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_PY"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cY);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_FREQ"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].freq);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_OFFSET_H"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrOffsetH);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_OFFSET_V"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrOffsetV);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_SPEC_H"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitH);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_SPEC_V"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitV);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_FOV"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_MEMO"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].memo);
	}
}

void AF_TestItem::GetSquareChartSfrPara()
{
	pDlg->m_dlgSquareChartSfr.m_roiEnable = ReadConfigInt(_T("AF_SQUARE_CHART_SFR_ENABLE"), 1);
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.imgWidth = ReadConfigString(_T("AF_SQUARE_CHART_SFR_IMG_WIDTH"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.imgHeight = ReadConfigString(_T("AF_SQUARE_CHART_SFR_IMG_HEIGHT"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_addParaByFov = ReadConfigString(_T("AF_SQUARE_CHART_SFR_FOV"), _T("None"));
	pDlg->m_dlgSquareChartSfr.m_pX = ReadConfigString(_T("AF_SQUARE_CHART_SFR_PX"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_pY = ReadConfigString(_T("AF_SQUARE_CHART_SFR_PY"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_sfrOffsetH = ReadConfigString(_T("AF_SQUARE_CHART_SFR_OFFSET_H"), _T("0.0000"));
	pDlg->m_dlgSquareChartSfr.m_sfrOffsetV = ReadConfigString(_T("AF_SQUARE_CHART_SFR_OFFSET_V"), _T("0.0000"));
	pDlg->m_dlgSquareChartSfr.m_specH = ReadConfigString(_T("AF_SQUARE_CHART_SFR_SPEC_H"), _T("0.0000"));
	pDlg->m_dlgSquareChartSfr.m_specV = ReadConfigString(_T("AF_SQUARE_CHART_SFR_SPEC_V"), _T("0.0000"));
	pDlg->m_dlgSquareChartSfr.m_memo = ReadConfigString(_T("AF_SQUARE_CHART_SFR_MEMO"), _T(""));
	pDlg->m_dlgSquareChartSfr.m_freq = ReadConfigString(_T("AF_SQUARE_CHART_SFR_FREQ"), _T("0.125"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt = ReadConfigInt(_T("AF_SQUARE_CHART_SFR_ROI_COUNT"), 0);

	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiWidth = ReadConfigString(_T("AF_SQUARE_CHART_SFR_ROI_WIDTH"), _T("64"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiHeight = ReadConfigString(_T("AF_SQUARE_CHART_SFR_ROI_HEIGHT"), _T("64"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskWidth = ReadConfigString(_T("AF_SQUARE_CHART_SFR_MASK_WIDTH"), _T("250"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskHeight = ReadConfigString(_T("AF_SQUARE_CHART_SFR_MASK_HEIGHT"), _T("250"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiMaskPhi = ReadConfigString(_T("AF_SQUARE_CHART_SFR_MASK_PHI"), _T("-6"));

	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.enable = ReadConfigInt(_T("AF_SQUARE_CHART_CORNER_DIFF_ENABLE"), 0);
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffAbsolute = ReadConfigInt(_T("AF_SQUARE_CHART_CORNER_DIFF_ABSOLUTE"), 1);
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.spec = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_SPEC"), _T("1.00"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specH = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_SPEC_H"), _T("1.00"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.specV = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_SPEC_V"), _T("1.00"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffOffset = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_OFFSET"), _T("0.00"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffOffsetH = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_OFFSET_H"), _T("0.00"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.diffOffsetV = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_OFFSET_V"), _T("0.00"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLU = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_LU"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumRU = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_RU"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumLD = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_LD"), _T("0"));
	pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.cornerDiffPara.roiNumRD = ReadConfigString(_T("AF_SQUARE_CHART_CORNER_DIFF_ROI_NUM_RD"), _T("0"));

	for (int i=0; i<pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiCnt; i++)
	{
		CString tmpKey = _T("");
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_ENABLE"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].enable = ReadConfigInt(tmpKey, 0);
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_PX"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cX = ReadConfigString(tmpKey, _T("0"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_PY"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].cY = ReadConfigString(tmpKey, _T("0"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_FREQ"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].freq = ReadConfigString(tmpKey, _T("0.125"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_OFFSET_H"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrOffsetH = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_OFFSET_V"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].sfrOffsetV = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_SPEC_H"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitH = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_SPEC_V"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].limitV = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_FOV"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].fov = ReadConfigString(tmpKey, _T(""));
		tmpKey.Format(_T("AF_SQUARE_CHART_SFR_ROI%03d_MEMO"), i);
		pDlg->m_dlgSquareChartSfr.m_squareChartSfrPara.roiPara[i].memo = ReadConfigString(tmpKey, _T(""));
	}
}

void AF_TestItem::ShowSquareChartSfrInfoROI(int roiNum, CPoint tagPosi, int* roiType, CPoint* roiCnPosi, CSize roiSize, float* sfrValue, SQUARE_CHART_SFR_PARA sfrPara)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect, CamID);

	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150, 255, 255)); 

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0f * DisplayWndRect.Width() / ImgWidth;
	double Yratio = 1.0f * DisplayWndRect.Height() / ImgHeight;

	COLORREF CurrentColor = COLOR_BLUE;
	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;

	for (int i=0; i<2; i++)
	{
		if ((roiCnPosi[i].x == 0) && (roiCnPosi[i].y == 0))
			continue;

		int roiWidth = ((roiSize.cx % 2 == 0) ? roiSize.cx : roiSize.cx - 1);
		int roiHeight = ((roiSize.cy % 2 == 0) ? roiSize.cy : roiSize.cy - 1);
		int roiStartX = roiCnPosi[i].x - roiWidth / 2;
		roiStartX = (roiStartX % 2 == 0) ? roiStartX : roiStartX + 1;
		int roiStartY = roiCnPosi[i].y - roiHeight / 2;
		roiStartY = (roiStartY % 2 == 0) ? roiStartY : roiStartY + 1;

		//CStdioFile fRoiPosi;
		//fRoiPosi.Open(_T("D:\\SquareChartRoiPosi.txt"), CFile::modeCreate|CFile::modeWrite|CFile::typeText);
		//CString strT = _T("");
		//strT.Format(_T("%02d,%d/%d,%d/%d,%d/%d"), i, tagPosi.x, tagPosi.y, roiStartX, roiStartY, roiStartX + roiWidth, roiStartY + roiHeight);
		//fRoiPosi.WriteString(strT);
		//fRoiPosi.Close();

		cx1 = roiStartX;
		cy1 = roiStartY;
		cx2 = roiStartX + roiWidth;
		cy2 = roiStartY + roiHeight;
		if (roiType[i] == 0)
			CurrentColor = (sfrPara.roiPara[0].sfrValH > _ttof(sfrPara.roiPara[roiNum].limitH)) ? COLOR_BLUE : COLOR_RED;
		else
			CurrentColor = (sfrPara.roiPara[0].sfrValV > _ttof(sfrPara.roiPara[roiNum].limitV)) ? COLOR_BLUE : COLOR_RED;
		//CurrentColor = COLOR_BLUE;

		px1 = (int)(cx1* Xratio);
		py1 = (int)(cy1* Yratio);
		px2 = (int)(cx2* Xratio);
		py2 = (int)(cy2* Yratio);
	
		CPen pen(PS_SOLID, 2, CurrentColor);
		pDisplayWndMemDC->SelectObject(&pen);
		pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

		pDisplayWndMemDC->SetTextColor(CurrentColor);
		CString strTmp = CString(_T(""));
		strTmp.Format(_T("%02d: %.2f/%.2f"), roiNum, sfrPara.roiPara[roiNum].sfrValH * 100.0f, sfrPara.roiPara[roiNum].sfrValV * 100.0f);
		cx1 = tagPosi.x;
		cy1 = tagPosi.y;
		px1 = (int)(cx1* Xratio);
		py1 = (int)(cy1* Yratio);
		//pDisplayWndMemDC->TextOut(px1 - 50, py1 + 10, strTmp);
		pDisplayWndMemDC->TextOut(px1 - 50, py1 + 25, strTmp);
	}

	CurrentColor = COLOR_YELLOW;
	CPen penLine(PS_SOLID, 2, CurrentColor);
	pDisplayWndMemDC->SelectObject(&penLine);

	cx1 = tagPosi.x - 6;
	cy1 = tagPosi.y - 6;
	cx2 = tagPosi.x + 6;
	cy2 = tagPosi.y + 6;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

	cx1 = 0;
	cy1 = ImgHeight/2;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth-1;
	cy2 = ImgHeight/2;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	cx1 = ImgWidth/2;
	cy1 = 0;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth/2;
	cy2 = ImgHeight-1;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void AF_TestItem::ShowXChartSfrInfoROI(int roiNum, CPoint tagPosi, CPoint* roiCnPosi, CSize roiSize, float* sfrValue, X_CHART_SFR_PARA sfrPara)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect, CamID);

	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150, 255, 255)); 

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0f * DisplayWndRect.Width() / ImgWidth;
	double Yratio = 1.0f * DisplayWndRect.Height() / ImgHeight;

	COLORREF CurrentColor = COLOR_BLUE;
	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;

	CString strTmp = CString(_T(""));
	strTmp.Format(_T("%02d: "), roiNum);
	for (int i=0; i<4; i++)
	{
		if ((roiCnPosi[i].x == 0) && (roiCnPosi[i].y == 0))
			continue;

		int roiWidth = ((roiSize.cx % 2 == 0) ? roiSize.cx : roiSize.cx - 1);
		int roiHeight = ((roiSize.cy % 2 == 0) ? roiSize.cy : roiSize.cy - 1);
		int roiStartX = roiCnPosi[i].x - roiWidth / 2;
		roiStartX = (roiStartX % 2 == 0) ? roiStartX : roiStartX + 1;
		int roiStartY = roiCnPosi[i].y - roiHeight / 2;
		roiStartY = (roiStartY % 2 == 0) ? roiStartY : roiStartY + 1;

		cx1 = roiStartX;
		cy1 = roiStartY;
		cx2 = roiStartX + roiWidth;
		cy2 = roiStartY + roiHeight;
		if ((i == 0) || (i == 1))
			CurrentColor = (sfrValue[i] > _ttof(sfrPara.roiPara[roiNum].limitH)) ? COLOR_BLUE : COLOR_RED;
		else
			CurrentColor = (sfrValue[i] > _ttof(sfrPara.roiPara[roiNum].limitV)) ? COLOR_BLUE : COLOR_RED;
		//CurrentColor = COLOR_BLUE;

		px1 = (int)(cx1* Xratio);
		py1 = (int)(cy1* Yratio);
		px2 = (int)(cx2* Xratio);
		py2 = (int)(cy2* Yratio);
	
		CPen pen(PS_SOLID, 2, CurrentColor);
		pDisplayWndMemDC->SelectObject(&pen);
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].topEn == TRUE) && (i == 0))
			pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].bottomEn == TRUE) && (i == 1))
			pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].leftEn == TRUE) && (i == 2))
			pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].rightEn == TRUE) && (i == 3))
			pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

		pDisplayWndMemDC->SetTextColor(CurrentColor);
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].topEn == TRUE) && (i == 0))
		{
			CString t = _T("");
			t.Format(_T("%.2f/"), sfrValue[i] * 100.0f);
			strTmp += t;
		}
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].bottomEn == TRUE) && (i == 1))
		{
			CString t = _T("");
			t.Format(_T("%.2f/"), sfrValue[i] * 100.0f);
			strTmp += t;
		}
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].leftEn == TRUE) && (i == 2))
		{
			CString t = _T("");
			t.Format(_T("%.2f/"), sfrValue[i] * 100.0f);
			strTmp += t;
		}
		if ((pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[roiNum].rightEn == TRUE) && (i == 3))
		{
			CString t = _T("");
			t.Format(_T("%.2f/"), sfrValue[i] * 100.0f);
			strTmp += t;
		}
	}
	
	cx1 = tagPosi.x;
	cy1 = tagPosi.y;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);
	if (roiNum == 0)
		pDisplayWndMemDC->TextOut(px1 - 50, py1 + 35, strTmp);
	else if (((roiNum % 4) == 1) || ((roiNum % 4) == 2))
		pDisplayWndMemDC->TextOut(px1 - 50, py1 + 35, strTmp);
	else
		pDisplayWndMemDC->TextOut(px1 - 50, py1 + 15, strTmp);

	CurrentColor = COLOR_YELLOW;
	CPen penLine(PS_SOLID, 2, CurrentColor);
	pDisplayWndMemDC->SelectObject(&penLine);

	cx1 = tagPosi.x - 6;
	cy1 = tagPosi.y - 6;
	cx2 = tagPosi.x + 6;
	cy2 = tagPosi.y + 6;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

	cx1 = 0;
	cy1 = ImgHeight/2;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth-1;
	cy2 = ImgHeight/2;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	cx1 = ImgWidth/2;
	cy1 = 0;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = ImgWidth/2;
	cy2 = ImgHeight-1;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	pDisplayWndMemDC->MoveTo(px1, py1);
	pDisplayWndMemDC->LineTo(px2, py2);

	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void AF_TestItem::SaveXChartSfrPara()
{
	WriteConfigInt(_T("AF_XCHART_SFR_TOP_ENABLE"), pDlg->m_dlgSfr.m_roiTopEn);
	WriteConfigInt(_T("AF_XCHART_SFR_BOTTOM_ENABLE"), pDlg->m_dlgSfr.m_roiBottomEn);
	WriteConfigInt(_T("AF_XCHART_SFR_LEFT_ENABLE"), pDlg->m_dlgSfr.m_roiLeftEn);
	WriteConfigInt(_T("AF_XCHART_SFR_RIGHT_ENABLE"), pDlg->m_dlgSfr.m_roiRightEn);
	WriteConfigString(_T("AF_XCHART_SFR_IMG_WIDTH"), pDlg->m_dlgSfr.m_xChartSfrPara.imgWidth);
	WriteConfigString(_T("AF_XCHART_SFR_IMG_HEIGHT"), pDlg->m_dlgSfr.m_xChartSfrPara.imgHeight);
	WriteConfigString(_T("AF_XCHART_SFR_FOV"), pDlg->m_dlgSfr.m_addParaByFov);
	WriteConfigString(_T("AF_XCHART_SFR_PX"), pDlg->m_dlgSfr.m_pX);
	WriteConfigString(_T("AF_XCHART_SFR_PY"), pDlg->m_dlgSfr.m_pY);
	WriteConfigString(_T("AF_XCHART_SFR_OFFSET_H"), pDlg->m_dlgSfr.m_sfrOffsetH);
	WriteConfigString(_T("AF_XCHART_SFR_OFFSET_V"), pDlg->m_dlgSfr.m_sfrOffsetV);
	WriteConfigString(_T("AF_XCHART_SFR_SPEC_H"), pDlg->m_dlgSfr.m_specH);
	WriteConfigString(_T("AF_XCHART_SFR_SPEC_V"), pDlg->m_dlgSfr.m_specV);
	WriteConfigString(_T("AF_XCHART_SFR_MEMO"), pDlg->m_dlgSfr.m_memo);
	WriteConfigString(_T("AF_XCHART_SFR_FREQ"), pDlg->m_dlgSfr.m_freq);
	WriteConfigInt(_T("AF_XCHART_SFR_ROI_COUNT"), pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt);

	WriteConfigString(_T("AF_XCHART_SFR_ROI_WIDTH"), pDlg->m_dlgSfr.m_xChartSfrPara.roiWidth);
	WriteConfigString(_T("AF_XCHART_SFR_ROI_HEIGHT"), pDlg->m_dlgSfr.m_xChartSfrPara.roiHeight);
	WriteConfigString(_T("AF_XCHART_SFR_MASK_WIDTH"), pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskWidth);
	WriteConfigString(_T("AF_XCHART_SFR_MASK_HEIGHT"), pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskHeight);

	WriteConfigInt(_T("AF_XCHART_CORNER_DIFF_ENABLE"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.enable);
	WriteConfigInt(_T("AF_XCHART_CORNER_DIFF_ABSOLUTE"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffAbsolute);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_SPEC"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.spec);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_SPEC_H"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specH);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_SPEC_V"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specV);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_OFFSET"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffOffset);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_OFFSET_H"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffOffsetH);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_OFFSET_V"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffOffsetV);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_LU"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLU);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_RU"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumRU);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_LD"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLD);
	WriteConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_RD"), pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumRD);

	for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
	{
		CString tmpKey = _T(""), tmpVal = _T("");
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_TOP_ENABLE"), i);
		tmpVal.Format(_T("%d"), pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn);
		WriteConfigString(tmpKey, tmpVal);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_BOTTOM_ENABLE"), i);
		tmpVal.Format(_T("%d"), pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn);
		WriteConfigString(tmpKey, tmpVal);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_LEFT_ENABLE"), i);
		tmpVal.Format(_T("%d"), pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn);
		WriteConfigString(tmpKey, tmpVal);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_RIGHT_ENABLE"), i);
		tmpVal.Format(_T("%d"), pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn);
		WriteConfigString(tmpKey, tmpVal);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_PX"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cX);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_PY"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cY);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_FREQ"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].freq);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_OFFSET_H"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrOffsetH);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_OFFSET_V"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrOffsetV);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_SPEC_H"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitH);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_SPEC_V"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitV);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_FOV"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_MEMO"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].memo);
	}
}

void AF_TestItem::GetXChartSfrPara()
{
	pDlg->m_dlgSfr.m_roiTopEn = ReadConfigInt(_T("AF_XCHART_SFR_TOP_ENABLE"), 1);
	pDlg->m_dlgSfr.m_roiBottomEn = ReadConfigInt(_T("AF_XCHART_SFR_BOTTOM_ENABLE"), 1);
	pDlg->m_dlgSfr.m_roiLeftEn = ReadConfigInt(_T("AF_XCHART_SFR_LEFT_ENABLE"), 1);
	pDlg->m_dlgSfr.m_roiRightEn = ReadConfigInt(_T("AF_XCHART_SFR_RIGHT_ENABLE"), 1);
	pDlg->m_dlgSfr.m_xChartSfrPara.imgWidth = ReadConfigString(_T("AF_XCHART_SFR_IMG_WIDTH"), _T("0"));
	pDlg->m_dlgSfr.m_xChartSfrPara.imgHeight = ReadConfigString(_T("AF_XCHART_SFR_IMG_HEIGHT"), _T("0"));
	pDlg->m_dlgSfr.m_addParaByFov = ReadConfigString(_T("AF_XCHART_SFR_FOV"), _T("None"));
	pDlg->m_dlgSfr.m_pX = ReadConfigString(_T("AF_XCHART_SFR_PX"), _T("0"));
	pDlg->m_dlgSfr.m_pY = ReadConfigString(_T("AF_XCHART_SFR_PY"), _T("0"));
	pDlg->m_dlgSfr.m_sfrOffsetH = ReadConfigString(_T("AF_XCHART_SFR_OFFSET_H"), _T("0.0000"));
	pDlg->m_dlgSfr.m_sfrOffsetV = ReadConfigString(_T("AF_XCHART_SFR_OFFSET_V"), _T("0.0000"));
	pDlg->m_dlgSfr.m_specH = ReadConfigString(_T("AF_XCHART_SFR_SPEC_H"), _T("0.0000"));
	pDlg->m_dlgSfr.m_specV = ReadConfigString(_T("AF_XCHART_SFR_SPEC_V"), _T("0.0000"));
	pDlg->m_dlgSfr.m_memo = ReadConfigString(_T("AF_XCHART_SFR_MEMO"), _T(""));
	pDlg->m_dlgSfr.m_freq = ReadConfigString(_T("AF_XCHART_SFR_FREQ"), _T("0.125"));
	pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt = ReadConfigInt(_T("AF_XCHART_SFR_ROI_COUNT"), 0);

	pDlg->m_dlgSfr.m_xChartSfrPara.roiWidth = ReadConfigString(_T("AF_XCHART_SFR_ROI_WIDTH"), _T("64"));
	pDlg->m_dlgSfr.m_xChartSfrPara.roiHeight = ReadConfigString(_T("AF_XCHART_SFR_ROI_HEIGHT"), _T("64"));
	pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskWidth = ReadConfigString(_T("AF_XCHART_SFR_MASK_WIDTH"), _T("250"));
	pDlg->m_dlgSfr.m_xChartSfrPara.roiMaskHeight = ReadConfigString(_T("AF_XCHART_SFR_MASK_HEIGHT"), _T("250"));

	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.enable = ReadConfigInt(_T("AF_XCHART_CORNER_DIFF_ENABLE"), 0);
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffAbsolute = ReadConfigInt(_T("AF_XCHART_CORNER_DIFF_ABSOLUTE"), 1);
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.spec = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_SPEC"), _T("1.00"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specH = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_SPEC_H"), _T("1.00"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.specV = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_SPEC_V"), _T("1.00"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffOffset = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_OFFSET"), _T("0.00"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffOffsetH = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_OFFSET_H"), _T("0.00"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.diffOffsetV = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_OFFSET_V"), _T("0.00"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLU = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_LU"), _T("0"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumRU = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_RU"), _T("0"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumLD = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_LD"), _T("0"));
	pDlg->m_dlgSfr.m_xChartSfrPara.cornerDiffPara.roiNumRD = ReadConfigString(_T("AF_XCHART_CORNER_DIFF_ROI_NUM_RD"), _T("0"));

	for (int i=0; i<pDlg->m_dlgSfr.m_xChartSfrPara.roiCnt; i++)
	{
		CString tmpKey = _T("");
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_TOP_ENABLE"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].topEn = ReadConfigInt(tmpKey, 0);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_BOTTOM_ENABLE"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].bottomEn = ReadConfigInt(tmpKey, 0);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_LEFT_ENABLE"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].leftEn = ReadConfigInt(tmpKey, 0);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_RIGHT_ENABLE"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].rightEn = ReadConfigInt(tmpKey, 0);
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_PX"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cX = ReadConfigString(tmpKey, _T("0"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_PY"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].cY = ReadConfigString(tmpKey, _T("0"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_FREQ"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].freq = ReadConfigString(tmpKey, _T("0.125"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_OFFSET_H"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrOffsetH = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_OFFSET_V"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].sfrOffsetV = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_SPEC_H"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitH = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_SPEC_V"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].limitV = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_FOV"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].fov = ReadConfigString(tmpKey, _T(""));
		tmpKey.Format(_T("AF_XCHART_SFR_ROI%03d_MEMO"), i);
		pDlg->m_dlgSfr.m_xChartSfrPara.roiPara[i].memo = ReadConfigString(tmpKey, _T(""));
	}
}

float AF_TestItem::MedielFilterFor3(float a, float b, float c)
{
	float tmpBuff[3] = {0.0f};
	tmpBuff[0] = a;
	tmpBuff[1] = b;
	tmpBuff[2] = c;
	float tmpT = 0.0f;
	for (int i=0; i<2; i++)
	{
		for (int j=i+1; j<3; j++)
		{
			if (tmpBuff[i] > tmpBuff[j])
			{
				tmpT = tmpBuff[i];
				tmpBuff[i] = tmpBuff[j];
				tmpBuff[j] = tmpT;
			}
		}
	}
	return tmpBuff[1];
}

void AF_TestItem::SaveMotoChartSfrPara()
{
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt = 19;

	WriteConfigInt(_T("AF_MOTO_CHART_SFR_ENABLE"), pDlg->m_dlgMotoChartSfr.m_roiEnable);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_OFFSET"), pDlg->m_dlgMotoChartSfr.m_sfrOffset);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_SPEC"), pDlg->m_dlgMotoChartSfr.m_spec);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_MEMO"), pDlg->m_dlgMotoChartSfr.m_memo);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_FREQ"), pDlg->m_dlgMotoChartSfr.m_freq);
	WriteConfigInt(_T("AF_MOTO_CHART_SFR_ROI_COUNT"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt);

	WriteConfigString(_T("AF_MOTO_CHART_SFR_ROI_WIDTH"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiWidth);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_ROI_HEIGHT"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiHeight);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_MASK_WIDTH"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskWidth);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_MASK_HEIGHT"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskHeight);
	WriteConfigString(_T("AF_MOTO_CHART_SFR_MASK_OFFSETX"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskOffsetX);

	WriteConfigInt(_T("AF_MOTO_CHART_CORNER_m_ndiff_value"), pDlg->m_dlgMotoChartSfr.m_ndiff_value);

	WriteConfigInt(_T("AF_MOTO_CHART_CORNER_DIFF_ENABLE"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.enable);
	WriteConfigInt(_T("AF_MOTO_CHART_CORNER_DIFF_ABSOLUTE"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffAbsolute);
	WriteConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_SPEC"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.spec);
	WriteConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_OFFSET"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffOffset);
	WriteConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_LU"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLU);
	WriteConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_RU"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumRU);
	WriteConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_LD"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLD);
	WriteConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_RD"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumRD);

	for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
	{
		CString tmpKey = _T(""), tmpVal = _T("");
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_ENABLE"), i);
		tmpVal.Format(_T("%d"), pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable);
		WriteConfigString(tmpKey, tmpVal);
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_FREQ"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].freq);
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_OFFSET"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrOffset);
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_SPEC"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].limit);
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_FOV"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov);
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_MEMO"), i);
		WriteConfigString(tmpKey, pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].memo);
	}
}

void AF_TestItem::GetMotoChartSfrPara()
{
	pDlg->m_dlgMotoChartSfr.m_roiEnable = ReadConfigInt(_T("AF_MOTO_CHART_SFR_ENABLE"), 1);
	pDlg->m_dlgMotoChartSfr.m_ndiff_value = ReadConfigInt(_T("AF_MOTO_CHART_CORNER_m_ndiff_value"), 30);

	pDlg->m_dlgMotoChartSfr.m_sfrOffset = ReadConfigString(_T("AF_MOTO_CHART_SFR_OFFSET"), _T("0.0000"));
	pDlg->m_dlgMotoChartSfr.m_spec = ReadConfigString(_T("AF_MOTO_CHART_SFR_SPEC"), _T("0.0000"));
	pDlg->m_dlgMotoChartSfr.m_memo = ReadConfigString(_T("AF_MOTO_CHART_SFR_MEMO"), _T(""));
	pDlg->m_dlgMotoChartSfr.m_freq = ReadConfigString(_T("AF_MOTO_CHART_SFR_FREQ"), _T("0.125"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt = 19;//ReadConfigInt(_T("AF_MOTO_CHART_SFR_ROI_COUNT"), 0);

	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiWidth = ReadConfigString(_T("AF_MOTO_CHART_SFR_ROI_WIDTH"), _T("64"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiHeight = ReadConfigString(_T("AF_MOTO_CHART_SFR_ROI_HEIGHT"), _T("64"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskWidth = ReadConfigString(_T("AF_MOTO_CHART_SFR_MASK_WIDTH"), _T("250"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskHeight = ReadConfigString(_T("AF_MOTO_CHART_SFR_MASK_HEIGHT"), _T("250"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiMaskOffsetX = ReadConfigString(_T("AF_MOTO_CHART_SFR_MASK_OFFSETX"), _T("40"));

	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.enable = ReadConfigInt(_T("AF_MOTO_CHART_CORNER_DIFF_ENABLE"), 0);
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffAbsolute = ReadConfigInt(_T("AF_MOTO_CHART_CORNER_DIFF_ABSOLUTE"), 1);
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.spec = ReadConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_SPEC"), _T("1.00"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.diffOffset = ReadConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_OFFSET"), _T("0.00"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLU = ReadConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_LU"), _T("0"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumRU = ReadConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_RU"), _T("0"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumLD = ReadConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_LD"), _T("0"));
	pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.cornerDiffPara.roiNumRD = ReadConfigString(_T("AF_MOTO_CHART_CORNER_DIFF_ROI_NUM_RD"), _T("0"));

	for (int i=0; i<pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiCnt; i++)
	{
		CString tmpKey = _T("");
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_ENABLE"), i);
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].enable = ReadConfigInt(tmpKey, 0);
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_FREQ"), i);
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].freq = ReadConfigString(tmpKey, _T("0.125"));
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_OFFSET"), i);
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].sfrOffset = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_SPEC"), i);
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].limit = ReadConfigString(tmpKey, _T("0.0000"));
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_FOV"), i);
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].fov = ReadConfigString(tmpKey, _T(""));
		tmpKey.Format(_T("AF_MOTO_CHART_SFR_ROI%03d_MEMO"), i);
		pDlg->m_dlgMotoChartSfr.m_motoChartSfrPara.roiPara[i].memo = ReadConfigString(tmpKey, _T(""));
	}
}