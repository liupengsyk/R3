#include "StdAfx.h"
#include "HwDarkPartical_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

enum
{
	CHANNEL_R = 0,
	CHANNEL_GR = 1,
	CHANNEL_GB = 2,
	CHANNEL_B = 3,
	CHANNELCOUNT = 4
};

enum
{
	OUTFORMAT_RGGB = 0,
	OUTFORMAT_GRBG = 1,
	OUTFORMAT_GBRG = 2,
	OUTFORMAT_BGGR = 3
};



extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new HwDarkPartical_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
HwDarkPartical_TestItem::HwDarkPartical_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

HwDarkPartical_TestItem::~HwDarkPartical_TestItem(void)
{

}


int HwDarkPartical_TestItem::InitItem()
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
int HwDarkPartical_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
	m_fQuantitySpec = pDlg->m_QuantitySpec/100.0f;	//界面设置的是百分比
	m_nDnDiff = pDlg->m_DnDiff;

	m_pInterface->Sensor_ReadExp(CamID, m_PrimaryExposure);
	m_pInterface->Sensor_ReadGain(CamID, m_PrimaryGain);
	CString strLog = _T("");
	strLog.Format(_T("Primary Exposure = 0x%04X, Primary Gain = 0x%04X"), m_PrimaryExposure, m_PrimaryGain);
	m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

	if (!swscanf(pDlg->m_AnalogGain, _T("0x%x"), &m_AnalogGain))
		swscanf(pDlg->m_AnalogGain, _T("%d"), &m_AnalogGain);

	if (!swscanf(pDlg->m_InitExposure, _T("0x%x"), &m_InitExposure))
		swscanf(pDlg->m_InitExposure, _T("%d"), &m_InitExposure);
	m_FrameCount = 0;
	m_GainSettingTime = 0;
	m_DParticalResult = 0;

	for (int i=0; i<4; i++)
	{
		m_PrimaryReg[i] = 0;
	}
	return 0;
}

/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int HwDarkPartical_TestItem::Testing()
{
   
	if (m_FrameCount == 0)	//设置Gain值与Exposure值
	{
		if (pDlg->m_SensorType == UNIVERSAL_SENSOR)
		{
			m_pInterface->Sensor_WriteGain(CamID,m_AnalogGain);	
			m_pInterface->Sensor_WriteExp(CamID,m_InitExposure);
		} 
		else if(pDlg->m_SensorType == OV12A10)
		{
			m_pInterface->Sensor_WriteGain(CamID,m_AnalogGain);	//15.5x Gain

			for (int i=0; i<4;i++)
			{
				m_pInterface->Device_ReadI2c(OV12A10_SLAVED_ID,0x380C + i,&m_PrimaryReg[i],3,CamID);
			}
			m_pInterface->Device_WriteI2c(OV12A10_SLAVED_ID,0x380C,0x08,3,CamID);	// HTS 必须是6的倍数
			m_pInterface->Device_WriteI2c(OV12A10_SLAVED_ID,0x380D,0x50,3,CamID);
			m_pInterface->Device_WriteI2c(OV12A10_SLAVED_ID,0x380E,0x18,3,CamID);
			m_pInterface->Device_WriteI2c(OV12A10_SLAVED_ID,0x380F,0xD2,3,CamID);

			m_pInterface->Sensor_WriteExp(CamID,m_InitExposure);	//曝光时间125ms
		}
	}

	if (m_FrameCount < pDlg->m_FrameDelay)
	{
		Sleep(300);
		m_FrameCount++;
		return 1;
	}
	m_FrameCount = 0;
	int tempChkExposure = 0;
	USHORT tempChkGain = 0;
	m_pInterface->Sensor_ReadExp(CamID,tempChkExposure);
	m_pInterface->Sensor_ReadGain(CamID,tempChkGain);

#pragma region Check Gain Setting Code If You Need
	if (tempChkGain == m_AnalogGain && tempChkExposure == m_InitExposure)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Gain & Exposure Set OK"),COLOR_BLUE,200);
	}
	else if (pDlg->m_CheckGain)
	{
		m_GainSettingTime++;
		if (m_GainSettingTime == 2)
		{
			m_bResult = FALSE;
			m_pInterface->Ctrl_Addlog(CamID,_T("Gain & Exposure Set Fail Again"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("亮点测试后NG"));
			m_pInterface->Ctrl_Addlog(CamID,_T("华为亮点测试Gain值设置失败"),COLOR_RED,200);
			m_DParticalResult = 1;
			SetResult(RESULT_FAIL);
			return 0;
		} 
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Gain & Exposure Set Fail,Set Again"),COLOR_RED,200);
			return 1;
		}

	}
#pragma endregion Check Gain Setting Code If You Need
		
	ImageInfo* img = m_pInterface->Ctrl_GetImgInfo(CamID);
	int iImgW = img->imgwidth;
	int iImgH = img->imgheight;
#pragma region Reset Register And Save Raw10 If You Need After Ctrl_GetImgInfo
	if (pDlg->m_SensorType == UNIVERSAL_SENSOR)
	{
		m_pInterface->Sensor_WriteGain(CamID, m_PrimaryGain);
		m_pInterface->Sensor_WriteExp(CamID, m_PrimaryExposure);		//主要是恢复Gain值，Exposure不必要	
	} 
	else if(pDlg->m_SensorType == OV12A10)
	{

		m_pInterface->Sensor_WriteGain(CamID, m_PrimaryGain);

		for (int i=0; i<4;i++)
		{
			m_pInterface->Device_WriteI2c(OV12A10_SLAVED_ID,0x380C+i,m_PrimaryReg[i],3,CamID);
		}

		m_pInterface->Sensor_WriteExp(CamID, m_PrimaryExposure);		//主要是恢复Gain值，Exposure不必要
	}
	m_pInterface->Ctrl_Addlog(CamID,_T("Gain值恢复"),COLOR_BLUE,200);
	if (pDlg->m_SaveImgRaw10)
	{
		CString PathName = m_pInterface->Ctrl_GetCurrentPath() + _T("ImgSave\\");
		_wmkdir(PathName);
		CString tempStr;
		tempStr.Format(_T("_%d_%d"),iImgW,iImgH);
		CString FileName = _T("HwDarkPartical_")+m_pInterface->Sensor_GetCamFuseID(CamID)+tempStr;
		if (img->RawFormat == OUTFORMAT_RGGB)
		{
			FileName += _T("_RGGB_RAW10.raw");
		}
		else if (img->RawFormat == OUTFORMAT_GRBG)
		{
			FileName += _T("_GRBG_RAW10.raw");
		}
		else if (img->RawFormat == OUTFORMAT_GBRG)
		{
			FileName += _T("_GBRG_RAW10.raw");
		}
		else	//(img->RawFormat == OUTFORMAT_BGGR)
		{
			FileName += _T("_BGGR_RAW10.raw");
		}
		//m_pInterface->Ctrl_SaveImage(CamID,TRUE,PathName,FileName,3);		//使用此语句保存的图像实际上不是参与测试的图像

		CStdioFile Raw10File;
		if (Raw10File.Open(PathName+FileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			Raw10File.Write(img->Raw_buffer,iImgH*iImgW*2);
			Raw10File.Close();
		}
	}

#pragma endregion Reset Register And Save Raw10 If You Need After Ctrl_GetImgInfo
	
	m_bResult = DParticalTest(img->Raw_buffer,m_pInterface->Ctrl_GetOutPutFormat(CamID),iImgW,iImgH,m_fQuantitySpec,m_nDnDiff);
	if (m_bResult == FALSE)
	{
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("华为亮点测试NG"));
		m_pInterface->Ctrl_Addlog(CamID,_T("DPartical Test NG"),COLOR_RED,200);
		m_DParticalResult = 1;	//12+8数据回传约定，非零表示有亮点
		SetResult(RESULT_FAIL);
	}else
	{
		
		m_pInterface->Ctrl_Addlog(CamID,_T("DPartical Test OK"),COLOR_BLUE,200);
		m_DParticalResult = 0;
		SetResult(RESULT_PASS);
	}
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int HwDarkPartical_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int HwDarkPartical_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_CheckGain	 = ReadConfigInt(_T("CheckGain"),1);
	pDlg->m_QuantitySpec = _ttof(ReadConfigString(_T("QuantitySpec"),_T("0.1")));
	pDlg->m_SaveImgRaw10 = ReadConfigInt(_T("SaveImgRaw10"),1);
	pDlg->m_DnDiff		 = ReadConfigInt(_T("DnDiff"),50);
	pDlg->m_SaveDarkParticalInfo = ReadConfigInt(_T("SaveDarkParticalInfo"),1);
	pDlg->m_FrameDelay	 = ReadConfigInt(_T("FrameDelayNumber"),3);
	pDlg->m_SensorType	 = ReadConfigInt(_T("SensorType"),UNIVERSAL_SENSOR);;
	pDlg->m_AnalogGain			= ReadConfigString(_T("AnalogGain"),_T("0x01C0"));
	pDlg->m_InitExposure		= ReadConfigString(_T("InitExposure"),_T("0x01C0"));
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int HwDarkPartical_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	  
	WriteConfigInt(_T("CheckGain"),pDlg->m_CheckGain);
	WriteConfigInt(_T("SensorType"),pDlg->m_SensorType);
	WriteConfigInt(_T("FrameDelayNumber"),pDlg->m_FrameDelay);
	WriteConfigInt(_T("DnDiff"),pDlg->m_DnDiff);
	WriteConfigInt(_T("SaveImgRaw10"),pDlg->m_SaveImgRaw10);
	WriteConfigInt(_T("SaveDarkParticalInfo"),pDlg->m_SaveDarkParticalInfo);
	CString tempStr;
	tempStr.Format(_T("%f"),pDlg->m_QuantitySpec);
	WriteConfigString(_T("QuantitySpec"),tempStr);
	WriteConfigString(_T("InitExposure"),pDlg->m_InitExposure);
	WriteConfigString(_T("AnalogGain"),pDlg->m_AnalogGain);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR HwDarkPartical_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("<Partical Result");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR HwDarkPartical_TestItem::GetReportLowLimit()
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
LPCTSTR HwDarkPartical_TestItem::GetReportHightLimit()
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
LPCTSTR HwDarkPartical_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%d>"),m_DParticalResult);
	return Content;
}
BOOL HwDarkPartical_TestItem::GetChannelBuffer(USHORT* pRawImage,USHORT*pChannelImage,USHORT nImgW,USHORT nImgH,int nPixOrder,int iChannel,long long &sum)
{
	sum = 0;
	switch(iChannel)
	{
	case CHANNEL_R:
		{			
			switch(nPixOrder)
			{	
			case OUTFORMAT_RGGB:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2 * nImgW + x*2];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GRBG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2 * nImgW + x*2 + 1];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GBRG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_BGGR:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2  + 1];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			}

		}
		break;
	case  CHANNEL_GR:
		{

			switch(nPixOrder)
			{	
			case OUTFORMAT_RGGB:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2 * nImgW + x*2 + 1];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GRBG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2 * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GBRG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 + 1];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_BGGR:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			}
		}
		break;
	case CHANNEL_GB:
		{			
			switch(nPixOrder)
			{	
			case OUTFORMAT_RGGB:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GRBG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 + 1];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GBRG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2 * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_BGGR:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2  * nImgW + x*2 + 1 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			}
		}
		break;
	case  CHANNEL_B:
		{

			switch(nPixOrder)
			{	
			case OUTFORMAT_RGGB:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 + 1 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GRBG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[(y*2 + 1) * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_GBRG:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2  * nImgW + x*2 + 1];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			case OUTFORMAT_BGGR:
				{
					for (int y = 0; y < nImgH/2; y++)
					{
						for (int x = 0; x < nImgW/2; x++)
						{
							pChannelImage[y*nImgW/2 + x] = pRawImage[y*2  * nImgW + x*2 ];
							sum += pChannelImage[y*nImgW/2 + x];
						}
					}
				}
				break;
			}
		}
		break;
	}



	return TRUE;
}

void HwDarkPartical_TestItem::GetBadPixelPosition(DParticalInfo* pStack, int iStart,int iCount, int nPixOrder, int iChannel)
{
	switch(nPixOrder)
	{
	case  OUTFORMAT_RGGB:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex ;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_BGGR:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_GBRG:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			}
		}
		break;
	case OUTFORMAT_GRBG:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex + 1;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = iStart; iIndex < iCount; iIndex++)
					{
						pStack[iIndex].iXIndex = 2* pStack[iIndex].iChannelXIndex;
						pStack[iIndex].iYIndex = 2* pStack[iIndex].iChannelYIndex + 1;
					}
				}
				break;
			}
		}
		break;
	}
}
/****************************************************************************************************
返回值：		FALSE表示NG，TRUE表示测试PASS
pImg:			USHORT型Raw10 Buffer
iOutformat：	Sensor的Outformat
iImgW,iImgH:	图像宽高
fSpec:			数量规格
iDNDiff:		亮度规格
*****************************************************************************************************/
BOOL HwDarkPartical_TestItem::DParticalTest(USHORT* pImg,int iOutformat,int iImgW,int iImgH,float fSpec,int iDNDiff)
{
	BOOL bResult = TRUE;
	CString logStr;
	USHORT* pChannel = new USHORT[iImgW*iImgH/4];
	int particalCount = 0;
	DParticalInfo* dparticals = (DParticalInfo* )malloc(sizeof(DParticalInfo)*iImgW*iImgH*fSpec);		//DPartical数量不能超过ImgWidth*ImgHeight*QuantitySpec
	memset(dparticals,0,sizeof(DParticalInfo)*iImgW*iImgH*fSpec);
	for (int iChannel = CHANNEL_R; iChannel < CHANNELCOUNT; iChannel++)
	{	
		memset(pChannel,0,iImgH*iImgW/4);
		long long sum = 0;
		GetChannelBuffer(pImg,pChannel,iImgW,iImgH,iOutformat,iChannel,sum);	//获取单通道Channel Buffer以及单通道的亮度总和

		int avg = sum / (iImgH*iImgW/4);			
		int lastPaticalCount = particalCount;
		bResult = GetDPartical(pChannel,iImgW,iImgH,avg,particalCount,fSpec,iDNDiff,dparticals);
		GetBadPixelPosition(dparticals,lastPaticalCount,particalCount,iOutformat,iChannel);
		logStr.Format(_T("当前Channel亮点数量为：%d 总和已达到：%d"),particalCount-lastPaticalCount,particalCount);
		m_pInterface->Ctrl_Addlog(CamID,logStr,COLOR_BLUE,200);
		if (bResult == FALSE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Dpartical数量已超规格"),COLOR_RED,200);
			break;
		}
		int iNeighbors = 0;
		//获取相邻的亮点
		for (int idx = lastPaticalCount; idx < particalCount; idx++)
		{
			DParticalInfo tmpDPI ;
			tmpDPI.iChannelXIndex = dparticals[idx].iChannelXIndex;
			tmpDPI.iChannelYIndex = dparticals[idx].iChannelYIndex;
			tmpDPI.iDNValue = dparticals[idx].iDNValue;
			for (int idx = lastPaticalCount + 1; idx < particalCount; idx++)
			{
				USHORT xDiff = abs(dparticals[idx].iChannelXIndex -  tmpDPI.iChannelXIndex);
				USHORT yDiff = abs(dparticals[idx].iChannelYIndex - tmpDPI.iChannelYIndex);

				if (xDiff >= 2 && yDiff >= 2)
				{
					break;
				}

				if (xDiff <= 1 && yDiff <= 1 && !(xDiff == 0 && yDiff == 0) )
				{
					logStr.Format(_T("单通道相邻坐标：【%d,%d】与【%d,%d】"),dparticals[idx].iChannelXIndex,dparticals[idx].iChannelYIndex,tmpDPI.iChannelXIndex,tmpDPI.iChannelYIndex);
					m_pInterface->Ctrl_Addlog(CamID,logStr,COLOR_BLUE,200);
					iNeighbors++;
				}
			}
		}
		if (iNeighbors > 0)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("单通道内存在相邻的亮点"),COLOR_RED,200);
			bResult = FALSE;
			break;
		}

	}

	if (pDlg->m_SaveDarkParticalInfo)//Save DPartical Info
	{
		SaveData(dparticals,particalCount,bResult);
	}
	delete[] dparticals;
	delete[] pChannel;
	return bResult;
}

BOOL HwDarkPartical_TestItem::GetDPartical(USHORT* pChannel, int iImgW, int iImgH, int avg, int& particalnum, float fSpec, int iDNDiff,DParticalInfo* pInfos)
{
	BOOL bResult = TRUE;
	int iChannelW = iImgW/2;
	int iChannelH = iImgH/2;

	for (int y = 0; y < iChannelH; y++)
	{
		for (int x = 0; x < iChannelW; x++)
		{
			if (pChannel[y*iChannelW + x] > (avg + iDNDiff))
			{		
			
				if (particalnum  + 1 >= iImgH*iImgW*fSpec)
				{
					bResult = FALSE;
					break;
				}
				pInfos[particalnum].iChannelXIndex = x;
				pInfos[particalnum].iChannelYIndex = y;
				pInfos[particalnum].iDNValue = pChannel[y*iChannelW + x];
				particalnum++;
			}
		}
	}
	return bResult;
}

void HwDarkPartical_TestItem::SaveData(DParticalInfo* pInfo,int iCount,BOOL bResult)
{
	CString cfgName = GetName();
	SetName(L"GENERAL_SETTING");
	CString strReportPath = ReadConfigString(_T("sReportPath"),_T("0"));	
	SetName(cfgName);

	//////////////////////////////////////////////////////////////////////////
	FILE* fp = NULL;
	SYSTEMTIME st;
	GetLocalTime(&st);
	_wmkdir(strReportPath);

	//strReportPath += _T("\\SFR_Course_Data");
	strReportPath+=_T("\\");
	strReportPath+=GetName();
	_wmkdir(strReportPath);

	CString strTimeString ;
	strTimeString.Format(_T("%04d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	CString strDataFileName = strReportPath + _T("\\") + _T("HWDPartical") + strTimeString + _T(".csv");
	m_pInterface->Ctrl_Addlog(CamID,strDataFileName,COLOR_BLUE,200);
	fp = _wfopen(strDataFileName,_T("a"));
	if (fp == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("打开文档失败，无法保存数据"),COLOR_RED,200);
		return ;
	}
	fclose(fp);
	CStdioFile DataFile(strDataFileName, CFile::modeWrite | CFile::typeText);
	if (DataFile.GetLength() == 0)
	{
		CString strHeader = _T("SensorID,Time,Result,XIndex,YIndex,ChannelXIndex,ChannelYIndex,DNValue\n");	
		DataFile.WriteString(strHeader);
	}
	DataFile.SeekToEnd();
	CString strContent = _T("");
	strContent.Format(_T("%s,%02d:%02d:%02d,%s\n"),m_pInterface->Sensor_GetCamFuseID(CamID),
		st.wHour,st.wMinute,st.wSecond,bResult?_T("PASS"):_T("FAIL"));
	DataFile.WriteString(strContent);
	for (int i = 0; i < iCount; i++)
	{
		strContent.Format(_T(",,,%d,%d,%d,%d,%d\n"),pInfo[i].iXIndex,pInfo[i].iYIndex,
			pInfo[i].iChannelXIndex,pInfo[i].iChannelYIndex,pInfo[i].iDNValue);
		DataFile.WriteString(strContent);
	}
	DataFile.Close();
	return ;
}