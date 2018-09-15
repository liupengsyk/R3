#include "StdAfx.h"
#include "OC_Item_Moto_TestItem.h"
#include "ImageProc.h"
//#include "OPCenterThreshold.h"

#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */
//#pragma comment(lib,"OPCenterThreshold.lib")

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OC_Item_Moto_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
OC_Item_Moto_TestItem::OC_Item_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

OC_Item_Moto_TestItem::~OC_Item_Moto_TestItem(void)
{

}


int OC_Item_Moto_TestItem::InitItem()
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
int OC_Item_Moto_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
 
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OC_Item_Moto_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	
	if (pDlg->m_PixelSize < 0.5)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Pixel size设置过小"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OC Test fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	
	if (!OCTest())
	{
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("OC Test fail!"));
		SetResult(RESULT_FAIL);
	}
	else
	{
		SetResult(RESULT_PASS);
		SaveAWBData();//测试成功，存储数据到文件以便后面OTP烧录可以取出数据
	}

	if (pDlg->m_Continue)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}

BOOL OC_Item_Moto_TestItem::OCTest()
{
	m_pInterface->Ctrl_Addlog(CamID,_T("**Get Optical Center**"),COLOR_BLUE,200);
	CString TempStr = _T("");

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		return FALSE;
	}

#if OC_TEST_SAVE_PIC_DEFINE == 1
	{
		USES_CONVERSION;

		CString FilePath = m_pInterface->Ctrl_GetCurrentPath() + _T("OC_Test_Moto_SavePic\\");
		_mkdir(T2A(FilePath));
		CString FileName = m_pInterface->Ctrl_GetCamCode() + _T("OC_Test_Moto_SavePic.raw");
		CString BmpFileName = m_pInterface->Ctrl_GetCamCode() + _T("OC_Test_Moto_SavePic");
		CString TempMsg;
		TempMsg.Format(_T("%d_%s"), CamID, FileName);
		m_pInterface->Ctrl_SaveImage(CamID, 1, FilePath, TempMsg, 3, NULL);   //BYTE *imgBuffer
		TempMsg.Format(_T("%d_%s"), CamID, BmpFileName);
		m_pInterface->Ctrl_SaveImage(CamID, 1, FilePath, TempMsg, 2, NULL);   //BYTE *imgBuffer



		CString strFile = FilePath + m_pInterface->Ctrl_GetCamCode() + _T("OC_Test_Moto_SavePic-1.raw");
		FILE *pFile = _wfopen(strFile.GetBuffer(strFile.GetLength()), _T("wb"));
		if (NULL == pFile)
		{
			//wprintf(_T("_wfopen NG-1.raw Error\r\n"));
			return FALSE;
		}
		size_t ret = fwrite(img.Raw_buffer, 1, img.imgwidth * img.imgheight * sizeof(USHORT), pFile);
		//wprintf(_T("ret:%d %d %d\r\n"), ret, CenterIn.Width * CenterIn.Height * sizeof(USHORT), GetLastError());
		fclose(pFile);
	}
#endif

	int  width   = img.imgwidth;
	int  height  = img.imgheight;

	float PC_x = float((width - 1) / 2.0);
	float PC_y = float((height - 1) / 2.0);

	CenterX = PC_x;
	CenterY = PC_y;

	OFOPTCenterIn CenterIn;
	CenterIn.ImageBufferIn = img.Raw_buffer;
	CenterIn.Width = img.imgwidth;
	CenterIn.Height = img.imgheight;
	CenterIn.Outformat = img.RawFormat;
	CenterIn.pixelSize = pDlg->m_PixelSize;

#if 0//OC_TEST_READ_PIC_DEFINE == 1
	{
		CString strFile = _T("D:\\test\\OC_Test_Moto\\Debug\\OC_\\NG-1.raw");
		FILE *pFile = _wfopen(strFile.GetBuffer(strFile.GetLength()), _T("rb"));
		if (NULL == pFile)
		{
			//wprintf(_T("_wfopen NG-1.raw Error\r\n"));
			return FALSE;
		}

		fseek(pFile, 0, SEEK_END);
		long pos = ftell(pFile);
		//wprintf(_T("pos:%d  %d\r\n"), pos, g_SensorCfg.m_Width * g_SensorCfg.m_Height * sizeof(USHORT));

		fseek(pFile, 0, SEEK_SET);

		/*BYTE *pRawFile = new BYTE[g_SensorCfg.m_Width * g_SensorCfg.m_Height * 5 / 4];
		if (NULL == pRawFile)
		{
			fclose(pFile);
			return;
		}*/

		/*OFOPTCenterIn CenterIn;
		CenterIn.ImageBufferIn = new USHORT[g_SensorCfg.m_Width * g_SensorCfg.m_Height];
		if (NULL == CenterIn.ImageBufferIn)
		{
			fclose(pFile);
			//delete []pRawFile;
			return FALSE;
		}
		CenterIn.Width = g_SensorCfg.m_Width;
		CenterIn.Height = g_SensorCfg.m_Height;
		CenterIn.Outformat = g_SensorCfg.m_Format;
		CenterIn.pixelSize = 1.12;*/
	
		memset(CenterIn.ImageBufferIn, 0x00, CenterIn.Width * CenterIn.Height * sizeof(USHORT));
		size_t ret = fread(CenterIn.ImageBufferIn, 1, CenterIn.Width * CenterIn.Height * sizeof(USHORT), pFile);
		//wprintf(_T("ret:%d %d %d\r\n"), ret, CenterIn.Width * CenterIn.Height * sizeof(USHORT), GetLastError());
		fclose(pFile);
	}
#endif

	if (!pDlg->m_isMonoSensor)
	{
		switch (pDlg->m_isBayerSingleChannel)
		{
		case OC_BAYER_MODE_SINGLE_CHANNEL:
			{
				OpticalCenterTest(&CenterIn, &CenterOut);

				float channelIndex = 0.0;
				if (CenterOut.R_OpticalOut > pDlg->m_OC_BayerSpec_R)
				{
					if (pDlg->m_OCSpec_Shift && CenterOut.R_OpticalOut < pDlg->m_OCSpec_AfterShift)
					{
						channelIndex = (1-((pDlg->m_OC_BayerSpec_R - 3.0)/CenterOut.R_OpticalOut));

						CenterOut.R_OpticalPixel.xIndex = (int)((PC_x - CenterOut.R_OpticalPixel.xIndex)*channelIndex + CenterOut.R_OpticalPixel.xIndex);
						CenterOut.R_OpticalPixel.yIndex = (int)((PC_y - CenterOut.R_OpticalPixel.yIndex)*channelIndex + CenterOut.R_OpticalPixel.yIndex);
						CenterOut.R_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.R_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.R_OpticalPixel.yIndex-PC_y),2)));
					}
					else
					{
						TempStr.Format(_T("OC_R = %f,xIndex=%d,yIndex=%d"),CenterOut.R_OpticalOut,CenterOut.R_OpticalPixel.xIndex,CenterOut.R_OpticalPixel.yIndex);
						m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
						return FALSE;
					}
				}
				if (CenterOut.Gr_OpticalOut > pDlg->m_OC_BayerSpec_Gr)
				{
					if (pDlg->m_OCSpec_Shift && CenterOut.Gr_OpticalOut < pDlg->m_OCSpec_AfterShift)
					{
						channelIndex = (1-((pDlg->m_OC_BayerSpec_Gr - 3.0)/CenterOut.Gr_OpticalOut));

						CenterOut.Gr_OpticalPixel.xIndex = (int)((PC_x - CenterOut.Gr_OpticalPixel.xIndex)*channelIndex + CenterOut.Gr_OpticalPixel.xIndex);
						CenterOut.Gr_OpticalPixel.yIndex = (int)((PC_y - CenterOut.Gr_OpticalPixel.yIndex)*channelIndex + CenterOut.Gr_OpticalPixel.yIndex);
						CenterOut.Gr_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.Gr_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.Gr_OpticalPixel.yIndex-PC_y),2)));
					}
					else
					{
						TempStr.Format(_T("OC_Gr = %f,xIndex=%d,yIndex=%d"),CenterOut.Gr_OpticalOut,CenterOut.Gr_OpticalPixel.xIndex,CenterOut.Gr_OpticalPixel.yIndex);
						m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
						return FALSE;
					}
				}
				if (CenterOut.Gb_OpticalOut > pDlg->m_OC_BayerSpec_Gb)
				{
					if (pDlg->m_OCSpec_Shift && CenterOut.Gb_OpticalOut < pDlg->m_OCSpec_AfterShift)
					{
						channelIndex = (1-((pDlg->m_OC_BayerSpec_Gb - 3.0)/CenterOut.Gb_OpticalOut));

						CenterOut.Gb_OpticalPixel.xIndex = (int)((PC_x - CenterOut.Gb_OpticalPixel.xIndex)*channelIndex + CenterOut.Gb_OpticalPixel.xIndex);
						CenterOut.Gb_OpticalPixel.yIndex = (int)((PC_y - CenterOut.Gb_OpticalPixel.yIndex)*channelIndex + CenterOut.Gb_OpticalPixel.yIndex);
						CenterOut.Gb_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.Gb_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.Gb_OpticalPixel.yIndex-PC_y),2)));
					}
					else
					{
						TempStr.Format(_T("OC_Gb = %f,xIndex=%d,yIndex=%d"),CenterOut.Gb_OpticalOut,CenterOut.Gb_OpticalPixel.xIndex,CenterOut.Gb_OpticalPixel.yIndex);
						m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
						return FALSE;
					}
				}
				if (CenterOut.B_OpticalOut > pDlg->m_OC_BayerSpec_B)
				{
					if (pDlg->m_OCSpec_Shift && CenterOut.B_OpticalOut < pDlg->m_OCSpec_AfterShift)
					{
						channelIndex = (1-((pDlg->m_OC_BayerSpec_B - 3.0)/CenterOut.B_OpticalOut));

						CenterOut.B_OpticalPixel.xIndex = (int)((PC_x - CenterOut.B_OpticalPixel.xIndex)*channelIndex + CenterOut.B_OpticalPixel.xIndex);
						CenterOut.B_OpticalPixel.yIndex = (int)((PC_y - CenterOut.B_OpticalPixel.yIndex)*channelIndex + CenterOut.B_OpticalPixel.yIndex);
						CenterOut.B_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.B_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.B_OpticalPixel.yIndex-PC_y),2)));
					}
					else
					{
						TempStr.Format(_T("OC_B = %f,xIndex=%d,yIndex=%d"),CenterOut.B_OpticalOut,CenterOut.B_OpticalPixel.xIndex,CenterOut.B_OpticalPixel.yIndex);
						m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
						return FALSE;
					}
				}

				/**
				*	以下是一个例子：
				*	[180226-08:33:36:114]:OC_R = 37.356660,xIndex=2090,yIndex=1529
				*	[180226-08:33:36:129]:OC_Gr = 65.618045,xIndex=2082,yIndex=1505
				*	[180226-08:33:36:160]:OC_Gb = 93.172290,xIndex=2079,yIndex=1480
				*	[180226-08:33:36:176]:OC_B = 91.487616,xIndex=2064,yIndex=1488
				*	可以看出，按R、Gr、Gb、B顺序Distance的值越来越大，因为七色光中从红到紫的折射率越来越大,
				*	所以，通过Lens后按R、Gr、Gb、B顺序离物理光心的距离也越来越远。比如R的
				**/
				TempStr.Format(_T("OC_R = %f,xIndex=%d,yIndex=%d"),CenterOut.R_OpticalOut,CenterOut.R_OpticalPixel.xIndex,CenterOut.R_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

				TempStr.Format(_T("OC_Gr = %f,xIndex=%d,yIndex=%d"),CenterOut.Gr_OpticalOut,CenterOut.Gr_OpticalPixel.xIndex,CenterOut.Gr_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

				TempStr.Format(_T("OC_Gb = %f,xIndex=%d,yIndex=%d"),CenterOut.Gb_OpticalOut,CenterOut.Gb_OpticalPixel.xIndex,CenterOut.Gb_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

				TempStr.Format(_T("OC_B = %f,xIndex=%d,yIndex=%d"),CenterOut.B_OpticalOut,CenterOut.B_OpticalPixel.xIndex,CenterOut.B_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
			}
			break;

		case OC_BAYER_MODE_Y:
			{
				if (FALSE == OpticalCenterTest_Y(&CenterIn, &CenterOut, img.RGB24_buffer))
				{
					m_pInterface->Ctrl_Addlog(CamID, _T("OpticalCenterTest_Y() Error\r\n"), COLOR_RED, 200);
					SetResult(RESULT_FAIL);
					return FALSE;
				}
				if (CenterOut.YVal_OpticalOut > pDlg->m_OC_BayerSpec_Y)
				{
					TempStr.Format(_T("OC_Y = %f, xIndex=%d, yIndex=%d"), CenterOut.YVal_OpticalOut, CenterOut.YVal_OpticalPixel.xIndex, CenterOut.YVal_OpticalPixel.yIndex);
					m_pInterface->Ctrl_Addlog(CamID, TempStr, COLOR_RED, 200);
					return FALSE;
				}

				TempStr.Format(_T("OC_Y = %f, xIndex=%d, yIndex=%d"), CenterOut.YVal_OpticalOut, CenterOut.YVal_OpticalPixel.xIndex, CenterOut.YVal_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID, TempStr, COLOR_BLUE, 200);
			}
			break;

		default:
			break;
		}
	}
	else
	{
		OpticalCenterTest_Mono(&CenterIn,&CenterOut_Mono);

		float channelIndex = 0.0;
		if (CenterOut_Mono.OC_Out > pDlg->m_OC_MonoSpec)
		{
			if (pDlg->m_OCSpec_Shift && CenterOut_Mono.OC_Out < pDlg->m_OCSpec_AfterShift)
			{
				channelIndex = (1-((pDlg->m_OC_MonoSpec - 3.0)/CenterOut_Mono.OC_Out));
				CenterOut_Mono.OpticalPixel.xIndex = (int)((PC_x - CenterOut_Mono.OpticalPixel.xIndex)*channelIndex + CenterOut_Mono.OpticalPixel.xIndex);
				CenterOut_Mono.OpticalPixel.yIndex = (int)((PC_y - CenterOut_Mono.OpticalPixel.yIndex)*channelIndex + CenterOut_Mono.OpticalPixel.yIndex);
				CenterOut_Mono.OC_Out =  pDlg->m_PixelSize * (sqrt(pow((CenterOut_Mono.OpticalPixel.xIndex-PC_x),2)+pow((CenterOut_Mono.OpticalPixel.yIndex-PC_y),2)));
			}
			else
			{
				TempStr.Format(_T("OC_Mono = %f,xIndex=%d,yIndex=%d"),CenterOut_Mono.OC_Out,CenterOut_Mono.OpticalPixel.xIndex,CenterOut_Mono.OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
				return FALSE;
			}
		}

		TempStr.Format(_T("OC_Mono = %f,xIndex=%d,yIndex=%d"),CenterOut_Mono.OC_Out,CenterOut_Mono.OpticalPixel.xIndex,CenterOut_Mono.OpticalPixel.yIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	}

	m_pInterface->Ctrl_Addlog(CamID,_T("OC 测试 PASS !"),COLOR_BLUE,200);
	return TRUE;
}

/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OC_Item_Moto_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OC_Item_Moto_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	USES_CONVERSION;
	CString TempStr = _T("");

	TempStr = ReadConfigString(_T("PixelSize"),_T("1.4"));
	pDlg->m_PixelSize = strtod(T2A(TempStr),NULL);

	/** Mono Sensor OC 配置参数 **/
	TempStr = ReadConfigString(_T("OC_MonoSpec"),_T("70"));
	pDlg->m_OC_MonoSpec = strtod(T2A(TempStr),NULL);

	/** Bayer Sensor OC 配置参数 **/
	TempStr = ReadConfigString(_T("OC_BayerSpec_R"),_T("70"));
	pDlg->m_OC_BayerSpec_R = strtod(T2A(TempStr),NULL);
	TempStr = ReadConfigString(_T("OC_BayerSpec_Gr"),_T("70"));
	pDlg->m_OC_BayerSpec_Gr = strtod(T2A(TempStr),NULL);
	TempStr = ReadConfigString(_T("OC_BayerSpec_Gb"),_T("70"));
	pDlg->m_OC_BayerSpec_Gb = strtod(T2A(TempStr),NULL);
	TempStr = ReadConfigString(_T("OC_BayerSpec_B"),_T("70"));
	pDlg->m_OC_BayerSpec_B = strtod(T2A(TempStr),NULL);

	/** Bayer Sensor OC 配置参数 **/
	TempStr = ReadConfigString(_T("OC_BayerSpec_Y"),_T("70"));
	pDlg->m_OC_BayerSpec_Y = strtod(T2A(TempStr),NULL);

	TempStr = ReadConfigString(_T("OCSpec_AfterShift"),_T("70"));
	pDlg->m_OCSpec_AfterShift = strtod(T2A(TempStr),NULL);
	
	pDlg->m_OCSpec_Shift = ReadConfigInt(_T("OCSpec_Shift_En"),0);
	pDlg->m_Continue = ReadConfigInt(_T("m_Continue"),0);
	pDlg->m_isMonoSensor = ReadConfigInt(_T("isMonoSensor"),1);
	if (OC_MODE_MONO == pDlg->m_isMonoSensor)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO_Mono))->SetCheck(1);
	}
	else
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO_Bayer))->SetCheck(1);
	}

	pDlg->m_isBayerSingleChannel = ReadConfigInt(_T("isBayerSingleChannel"), 1);
	if (OC_BAYER_MODE_Y == pDlg->m_isBayerSingleChannel)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO_Bayer_Y))->SetCheck(1);
	}
	else
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO_Channel))->SetCheck(1);
	}

	pDlg->OCModeSel(pDlg->m_isMonoSensor);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OC_Item_Moto_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	
	USES_CONVERSION;
	CString TempStr = _T("");

	TempStr.Format(_T("%f"),pDlg->m_PixelSize);
	WriteConfigString(_T("PixelSize"),TempStr);

	/** Mono Sensor OC 配置参数 **/
	TempStr.Format(_T("%f"),pDlg->m_OC_MonoSpec);
	WriteConfigString(_T("OC_MonoSpec"),TempStr);

	/** Bayer Sensor OC 配置参数 **/
	TempStr.Format(_T("%f"),pDlg->m_OC_BayerSpec_R);
	WriteConfigString(_T("OC_BayerSpec_R"),TempStr);
	TempStr.Format(_T("%f"),pDlg->m_OC_BayerSpec_Gr);
	WriteConfigString(_T("OC_BayerSpec_Gr"),TempStr);
	TempStr.Format(_T("%f"),pDlg->m_OC_BayerSpec_Gb);
	WriteConfigString(_T("OC_BayerSpec_Gb"),TempStr);
	TempStr.Format(_T("%f"),pDlg->m_OC_BayerSpec_B);
	WriteConfigString(_T("OC_BayerSpec_B"),TempStr);

	/** Bayer Sensor OC 配置参数 **/
	TempStr.Format(_T("%f"),pDlg->m_OC_BayerSpec_Y);
	WriteConfigString(_T("OC_BayerSpec_Y"),TempStr);

	TempStr.Format(_T("%f"),pDlg->m_OCSpec_AfterShift);
	WriteConfigString(_T("OCSpec_AfterShift"),TempStr);

	WriteConfigInt(_T("OCSpec_Shift_En"),pDlg->m_OCSpec_Shift);
	WriteConfigInt(_T("m_Continue"),pDlg->m_Continue);
	WriteConfigInt(_T("isMonoSensor"),pDlg->m_isMonoSensor);

	WriteConfigInt(_T("isBayerSingleChannel"),pDlg->m_isBayerSingleChannel);

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_Moto_TestItem::GetReportHeads()
{
	static CString header;
	if (!pDlg->m_isMonoSensor)
	{
		switch (pDlg->m_isBayerSingleChannel)
		{
		case OC_BAYER_MODE_SINGLE_CHANNEL:
			{
				//header = _T("模组编号,OC_R_X,OC_R_Y,OC_R_Dis,OC_Gr_X,OC_Gr_Y,OC_Gr_Dis,OC_Gb_X,OC_Gb_Y,OC_Gb_Dis,OC_B_X,OC_B_Y,OC_B_Dis,OC_X,OC_Y,PixelSize");
				header = _T("模组编号,OC_R_X,OC_R_Y,OC_Gr_X,OC_Gr_Y,OC_Gb_X,OC_Gb_Y,OC_B_X,OC_B_Y,OC_R_X_Diff,OC_R_Y_Diff,OC_R_Dis,OC_Gr_X_Diff,OC_Gr_Y_Diff,OC_Gr_Dis,OC_Gb_X_Diff,OC_Gb_Y_Diff,OC_Gb_Dis,OC_B_X_Diff,OC_B_Y_Diff,OC_B_Dis,OC_X,OC_Y,PixelSize");
			}
			break;

		case OC_BAYER_MODE_Y:
			{
			#if OC_SAVE_DELTA_X_Y == 1
				header = _T("模组编号,OC_YVal_X,OC_YVal_Y,OC_YVal_Dis,OC_X,OC_Y,PixelSize,DeltaX,DeltaY");
			#else
				header = _T("模组编号,OC_YVal_X,OC_YVal_Y,OC_YVal_Dis,OC_X,OC_Y,PixelSize");
			#endif
			}
			break;

		default:
			break;
		}
	}
	else
	{
		header = _T("模组编号,OptiX,OptiY,OC_X_Diff,OC_Y_Diff,OC_Dis,OC_X,OC_Y,PixelSize");
	}
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_Moto_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
//	LowerLimit.Format(_T("%d,%d"),OCSpecX_low,OCSpecY_low);
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_Moto_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
//	UpperLimit.Format(_T("%d,%d"),OCSpecX_hig,OCSpecY_hig);
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_Moto_TestItem::GetReportContents()
{
	static CString Content;
	CString CamCode=m_pInterface->Ctrl_GetCamCode();
	float PC_x=CenterX;
	float PC_y=CenterY;

	if (!pDlg->m_isMonoSensor)
	{
		switch (pDlg->m_isBayerSingleChannel)
		{
		case OC_BAYER_MODE_SINGLE_CHANNEL:
			{
				Content.Format(_T("%s,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"),
								CamCode,
								CenterOut.R_OpticalPixel.xIndex,
								CenterOut.R_OpticalPixel.yIndex,
								CenterOut.Gr_OpticalPixel.xIndex,
								CenterOut.Gr_OpticalPixel.yIndex,
								CenterOut.Gb_OpticalPixel.xIndex,
								CenterOut.Gb_OpticalPixel.yIndex,
								CenterOut.B_OpticalPixel.xIndex,
								CenterOut.B_OpticalPixel.yIndex,
								CenterOut.R_OpticalPixel.xIndex - PC_x,
								CenterOut.R_OpticalPixel.yIndex - PC_y,
								CenterOut.R_OpticalOut,
								CenterOut.Gr_OpticalPixel.xIndex - PC_x,
								CenterOut.Gr_OpticalPixel.yIndex - PC_y,
								CenterOut.Gr_OpticalOut,
								CenterOut.Gb_OpticalPixel.xIndex - PC_x,
								CenterOut.Gb_OpticalPixel.yIndex - PC_y,
								CenterOut.Gb_OpticalOut,
								CenterOut.B_OpticalPixel.xIndex - PC_x,
								CenterOut.B_OpticalPixel.yIndex - PC_y,
								CenterOut.B_OpticalOut,
								PC_x,
								PC_y,
								pDlg->m_PixelSize
								);
			}
			break;

		case OC_BAYER_MODE_Y:
			{
			#if OC_SAVE_DELTA_X_Y == 1
				Content.Format(_T("%s,%d,%d,%f,%f,%f,%f,%d,%d"),
								CamCode,
								CenterOut.YVal_OpticalPixel.xIndex,
								CenterOut.YVal_OpticalPixel.yIndex,
								CenterOut.YVal_OpticalOut,
								PC_x,
								PC_y,
								pDlg->m_PixelSize,
								abs(CenterOut.YVal_OpticalPixel.xIndex - (int)PC_x),
								abs(CenterOut.YVal_OpticalPixel.yIndex - (int)PC_y)
								);
			#else
				Content.Format(_T("%s,%d,%d,%f,%f,%f,%f"),
								CamCode,
								CenterOut.YVal_OpticalPixel.xIndex,
								CenterOut.YVal_OpticalPixel.yIndex,
								CenterOut.YVal_OpticalOut,
								PC_x,
								PC_y,
								pDlg->m_PixelSize
								);
			#endif
			}
			break;

		default:
			break;
		}
	}
	else 
	{
		Content.Format(_T("%s,%d,%d,%f,%f,%f,%f,%f,%f"),
			CamCode,
			CenterOut_Mono.OpticalPixel.xIndex,
			CenterOut_Mono.OpticalPixel.yIndex,
			CenterOut_Mono.OpticalPixel.xIndex - PC_x,
			CenterOut_Mono.OpticalPixel.yIndex - PC_y,
			CenterOut_Mono.OC_Out,
			PC_x,
			PC_y,
			pDlg->m_PixelSize);
	}
	
	return Content;
}

void Raw2Channels( USHORT* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, OFOPTCenterPixInfo* pGr, OFOPTCenterPixInfo* pR, OFOPTCenterPixInfo* pB,  OFOPTCenterPixInfo* pGb )
{
	int GbCnt = 0;
	int GrCnt = 0;
	int BCnt  = 0;
	int RCnt  = 0;

	switch(nPixOrder)
	{
	case 0: //rggb
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGb[GbCnt].xIndex = j;
						pGb[GbCnt].yIndex = i;
						GbCnt++;

						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pB[BCnt].xIndex = (j+1);
						pB[BCnt].yIndex = i;
						BCnt++; 
					}
					else
					{
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pR[RCnt].xIndex = j;
						pR[RCnt].yIndex = i;
						RCnt++;

						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pGr[GrCnt].xIndex = (j+1);
						pGr[GrCnt].yIndex = i;
						GrCnt++;
					} 
				}
			}

		}
		break;

	case 1: //OUTFORMAT_GRBG
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pB[BCnt].xIndex = j;
						pB[BCnt].yIndex = i;
						BCnt++;

						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pGb[GbCnt].xIndex = (j+1);
						pGb[GbCnt].yIndex = i;
						GbCnt++; 
					}
					else
					{
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGr[GrCnt].xIndex = j;
						pGr[GrCnt].yIndex = i;
						GrCnt++;

						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pR[RCnt].xIndex = (j+1); 
						pR[RCnt].yIndex = i;
						RCnt++;
					} 
				}
			}			
		}
		break;

	case 2: //OUTFORMAT_GBRG
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pR[RCnt].xIndex = j;
						pR[RCnt].yIndex = i;
						RCnt++;

						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pGr[GrCnt].xIndex = (j+1);
						pGr[GrCnt].yIndex = i;
						GrCnt++; 
					}
					else
					{
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGb[GbCnt].xIndex = j;
						pGb[GbCnt].yIndex = i;
						GbCnt++;

						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pB[BCnt].xIndex = (j+1);
						pB[BCnt].yIndex = i;
						BCnt++;
					} 
				}
			}			
		}
		break;

	case 3: //OUTFORMAT_BGGR
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGr[GrCnt].xIndex = j;
						pGr[GrCnt].yIndex = i;
						GrCnt++;

						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pR[RCnt].xIndex = (j+1);
						pR[RCnt].yIndex = i;
						RCnt++; 
					}
					else
					{
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pB[BCnt].xIndex = j;
						pB[BCnt].yIndex = i;
						BCnt++;

						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pGb[GbCnt].xIndex = (j+1);
						pGb[GbCnt].yIndex = i;
						GbCnt++;
					} 
				}
			}			
		}
		break;

	default:
		break;
	} 
}

void getNewPanel(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo* outpPanel,int nImgWidth ,int nImgHeight)
{
	long nCnt= 0;	
	int RawFileWidth = nImgWidth/2 ;
	int RawFileHeight= nImgHeight/2 ;
	int Brightness = 0;
	int i=0,j=0;
	for( j = 0; j < RawFileHeight; j++ )
	{
		for(i = 0; i < RawFileWidth; i++ )
		{
			outpPanel[nCnt].xIndex  = pPanel[j*RawFileWidth+i].xIndex;
			outpPanel[nCnt].yIndex  = pPanel[j*RawFileWidth+i].yIndex;
			outpPanel[nCnt].Y_Value = pPanel[j*RawFileWidth+i].Y_Value;
			nCnt++;

			outpPanel[nCnt].xIndex  = pPanel[j*RawFileWidth+i].xIndex+1;
			outpPanel[nCnt].yIndex  = pPanel[j*RawFileWidth+i].yIndex;
			if ((i+1) < RawFileWidth)
			{
				outpPanel[nCnt].Y_Value = (pPanel[j*RawFileWidth+i].Y_Value+pPanel[j*RawFileWidth+i+1].Y_Value)/2;
			}
			else
			{
				outpPanel[nCnt].Y_Value = pPanel[j*RawFileWidth+i].Y_Value;
			}		
			nCnt++;
		}
		nCnt = nCnt + nImgWidth;
	}

	nCnt = 0;
	for( j = 0; j < nImgHeight; j=j+2 )
	{
		nCnt = nCnt + nImgWidth;
		for( int i = 0; i < nImgWidth; i++ )
		{
			outpPanel[nCnt].xIndex  = outpPanel[j*nImgWidth+i].xIndex;
			outpPanel[nCnt].yIndex  = outpPanel[j*nImgWidth+i].yIndex+1;
			if ((j+2) < nImgHeight)
			{
				outpPanel[nCnt].Y_Value = (outpPanel[j*nImgWidth+i].Y_Value+outpPanel[(j+2)*nImgWidth+i].Y_Value)/2;
			}
			else
			{
				outpPanel[nCnt].Y_Value = outpPanel[j*nImgWidth+i].Y_Value;
			}
			nCnt++;
		}
	}
}

void InsertionSort(OFOPTCenterPixInfo* pPanel,int ImageWidth,int ImageHeight)
{
	//插入法排序
	OFOPTCenterPixInfo pPixel;

	for (long i = 1;i < (ImageWidth*ImageHeight);i++)
	{
		if (pPanel[i].Y_Value > pPanel[i-1].Y_Value)
		{
			pPixel.Y_Value = pPanel[i].Y_Value;
			pPixel.xIndex  = pPanel[i].xIndex ;
			pPixel.yIndex  = pPanel[i].yIndex ;

			long j = (i - 1);
			while((j>=0)&&(pPixel.Y_Value > pPanel[j].Y_Value))
			{
				pPanel[j+1].Y_Value = pPanel[j].Y_Value;
				pPanel[j+1].xIndex  = pPanel[j].xIndex;
				pPanel[j+1].yIndex  = pPanel[j].yIndex;	
				j--;
			}

			pPanel[j+1].Y_Value = pPixel.Y_Value;
			pPanel[j+1].xIndex  = pPixel.xIndex;
			pPanel[j+1].yIndex  = pPixel.yIndex;
		}
	}
}

#if OC_TEST_SAVE_PIC_DEFINE == 1
unsigned int ChNum = 0;
#endif
void OC_Item_Moto_TestItem::getOCCoord(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo *CenterPixInfo,int ImageWidth,int ImageHeight)
{
	signed __int64 x_Checksum = 0;
	signed __int64 y_Checksum = 0;
	signed __int64 Checksum = 0;
	int  nCnt = 0;
	int Index = (int)(ImageWidth*ImageHeight*0.1);
	int MaxBrightness = 0;
	int k=0;
	int n=0;
	int m=0;

	for (k = 0 ; k < ImageHeight;k++)      
	{
		for (int n = 0; n < ImageWidth;n++)
		{
			if (pPanel[n+k*ImageWidth].Y_Value > MaxBrightness)
			{
				MaxBrightness = pPanel[n+k*ImageWidth].Y_Value;
			}
		}
	}

#if OC_TEST_SAVE_PIC_DEFINE == 1
	{
		CString str;

		str.Format(_T("\r\nMaxBrightness:%d\r\n"), MaxBrightness);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);

		USES_CONVERSION;

		CString FilePath = m_pInterface->Ctrl_GetCurrentPath() + _T("\\OC_Test_Moto_SavePic\\");
		_mkdir(T2A(FilePath));

		CString FileName = NULL;
		if (0 == ChNum)
		{
			FileName = FilePath + _T("OC_Test_Moto_SavePic_R.xls");
		}
		else if (1 == ChNum)
		{
			FileName = FilePath + _T("OC_Test_Moto_SavePic_Gr.xls");
		}
		else if (2 == ChNum)
		{
			FileName = FilePath + _T("OC_Test_Moto_SavePic_Gb.xls");
		}
		else if (3 == ChNum)
		{
			FileName = FilePath + _T("OC_Test_Moto_SavePic_B.xls");
		}
		ChNum++;
		
		CStdioFile pFile;
		if (TRUE == pFile.Open(FileName.GetBuffer(FileName.GetLength()), CFile::modeWrite | CFile::modeCreate | CFile::typeText))
		{
			//pFile.WriteString(_T("X\tY\tVal\n"));
			pFile.WriteString(_T("\t"));
			for (n = 0; n < ImageWidth; n++)
			{
				int inx = n;
				str.Format(_T("%d\t"), pPanel[inx].xIndex);
				pFile.WriteString(str);
			}
			for (k = 0; k < ImageHeight; k++)
			{
				str.Format(_T("\n%d\t"), pPanel[k*ImageWidth].yIndex);
				pFile.WriteString(str);
				for (n = 0; n < ImageWidth; n++)
				{
					int inx = n+k*ImageWidth;
					str.Format(_T("%d\t"), pPanel[inx].Y_Value);
					pFile.WriteString(str);
				}
			}
			pFile.Close();
		}
	}
#endif

#if OC_TEST_SAVE_PIC_DEFINE == 1
	int MaxValCnt = 0;
	DWORD dwCurCnt = GetTickCount();
#endif
	/**
	*	打印显示以下三层循环，每次耗时约0.6S~1.2S
	*	--Added by LiHai--20180226
	**/
	for (m = MaxBrightness; m > 0;m--)
	{
	#if OC_TEST_SAVE_PIC_DEFINE == 1
		MaxValCnt++;
	#endif
		for (k = 0 ; k < ImageHeight; k++)
		{
			for (n = 0; n < ImageWidth; n++)
			{
				if (pPanel[n+k*ImageWidth].Y_Value==m)
				{
					x_Checksum += ((pPanel[n+k*ImageWidth].xIndex)*(pPanel[n+k*ImageWidth].Y_Value));
					y_Checksum += ((pPanel[n+k*ImageWidth].yIndex)*(pPanel[n+k*ImageWidth].Y_Value));
					Checksum += pPanel[n+k*ImageWidth].Y_Value;
					nCnt++;
				}
			}
		}

		if (nCnt > Index)//最亮的点且占比超过10%的数目，认为是光心
		{
			x_Checksum = (1.0*x_Checksum/Checksum + 0.5);
			y_Checksum = (1.0*y_Checksum/Checksum + 0.5);
		#if OC_TEST_SAVE_PIC_DEFINE == 1
			{
				CString str;

				str.Format(_T("m:%d  nCnt:%d  Index:%d  MaxValCnt:%d  DeltaCnt:%d\r\n"), m, nCnt, Index, MaxValCnt, GetTickCount() - dwCurCnt);
				m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
			}
		#endif
			break;
		}
	}

	CenterPixInfo->xIndex = x_Checksum;
	CenterPixInfo->yIndex = y_Checksum;
}


void OC_Item_Moto_TestItem::getOptCenter(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo *CenterPixInfo,int ImageWidth,int ImageHeight,double *Distance,SensorFormat ID)
{
	if (ID == BAYER)
	{
		getOCCoord(pPanel,CenterPixInfo,ImageWidth/2,ImageHeight/2);      //获取OC坐标值
	}
	else
	{
		getOCCoord(pPanel,CenterPixInfo,ImageWidth,ImageHeight);      //获取OC坐标值
	}

	//计算对应channel的几何中心
	float xIndex_1 = (ImageWidth-1)/2.0; 
	float yIndex_1 = (ImageHeight-1)/2.0;

	*Distance = sqrt(pow((CenterPixInfo->xIndex - xIndex_1),2)+pow((CenterPixInfo->yIndex - yIndex_1),2));
}

//#if OC_TEST_BY_YVALUE_DEFINE == 0
BOOL OC_Item_Moto_TestItem::OpticalCenterTest(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut * _OFOPTCenterOut)
{
	//存放4channel的信息
	OFOPTCenterPixInfo *pGr = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];
	OFOPTCenterPixInfo *pB  = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];
	OFOPTCenterPixInfo *pR  = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];
	OFOPTCenterPixInfo *pGb = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];

	if (NULL == pGr || NULL == pGb || NULL == pR || NULL == pB)
	{
		SAFE_DELETE_ARRAY(pGr);
		SAFE_DELETE_ARRAY(pGb);
		SAFE_DELETE_ARRAY(pB);
		SAFE_DELETE_ARRAY(pR);
		return FALSE;
	}

	//对Raw图分4个Channel
	Raw2Channels( _OFOPTCenterIn->ImageBufferIn, _OFOPTCenterIn->Width,_OFOPTCenterIn->Height, _OFOPTCenterIn->Outformat, pGr, pR, pB, pGb );
#if OC_TEST_SAVE_PIC_DEFINE == 1
	{
		USES_CONVERSION;

		CString FilePath = m_pInterface->Ctrl_GetCurrentPath() + _T("OC_Test_Moto_SavePic\\");
		_mkdir(T2A(FilePath));

		CString FileName = FilePath + _T("OC_Test_Moto_SavePic_R");
		SaveRaw(FileName, pR, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);

		FileName = FilePath + _T("OC_Test_Moto_SavePic_Gr");
		SaveRaw(FileName, pGr, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);

		FileName = FilePath + _T("OC_Test_Moto_SavePic_Gb");
		SaveRaw(FileName, pGb, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);

		FileName = FilePath + _T("OC_Test_Moto_SavePic_B");
		SaveRaw(FileName, pB, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);
	}
#endif

	//获取Optlcal center的坐标和distance
	OFOPTCenterPixInfo  *OPTCenterPix = new OFOPTCenterPixInfo;
	if (NULL == OPTCenterPix)
	{
		SAFE_DELETE_ARRAY(pGr);
		SAFE_DELETE_ARRAY(pGb);
		SAFE_DELETE_ARRAY(pB);
		SAFE_DELETE_ARRAY(pR);
		return FALSE;
	}
	double m_Distance = 0;

#if OC_TEST_SAVE_PIC_DEFINE == 1
	ChNum = 0;
#endif
	getOptCenter(pR, OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);
	{
		CString str;

		str.Format(_T("pR---xIndex:%04d   yIndex:%04d   %.04f\r\n"), OPTCenterPix->xIndex, OPTCenterPix->yIndex, m_Distance * _OFOPTCenterIn->pixelSize);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
	}

	_OFOPTCenterOut->R_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->R_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->R_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	getOptCenter(pGr,OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);
	{
		CString str;

		str.Format(_T("pGr---xIndex:%04d   yIndex:%04d   %.04f\r\n"), OPTCenterPix->xIndex, OPTCenterPix->yIndex, m_Distance * _OFOPTCenterIn->pixelSize);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
	}

	_OFOPTCenterOut->Gr_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->Gr_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->Gr_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	getOptCenter(pGb,OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);
	{
		CString str;

		str.Format(_T("pGb---xIndex:%04d   yIndex:%04d   %.04f\r\n"), OPTCenterPix->xIndex, OPTCenterPix->yIndex, m_Distance * _OFOPTCenterIn->pixelSize);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
	}

	_OFOPTCenterOut->Gb_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->Gb_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->Gb_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	getOptCenter(pB, OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);
	{
		CString str;

		str.Format(_T("pB---xIndex:%04d   yIndex:%04d   %.04f\r\n"), OPTCenterPix->xIndex, OPTCenterPix->yIndex, m_Distance * _OFOPTCenterIn->pixelSize);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
	}

	_OFOPTCenterOut->B_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->B_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->B_OpticalOut = m_Distance*(_OFOPTCenterIn->pixelSize);

	SAFE_DELETE_ARRAY(pGr);
	SAFE_DELETE_ARRAY(pGb);
	SAFE_DELETE_ARRAY(pB);
	SAFE_DELETE_ARRAY(pR);
	SAFE_DELETE_ARRAY(OPTCenterPix);
	return TRUE;
}
//#else
BOOL OC_Item_Moto_TestItem::OpticalCenterTest_Y(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut * _OFOPTCenterOut, BYTE *pRGB24)
{
	//存放4channel的信息
	OFOPTCenterPixInfo *pY = new OFOPTCenterPixInfo[_OFOPTCenterIn->Width * _OFOPTCenterIn->Height];

	if (NULL == pY)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("OpticalCenterTest_Y()---pY is NULL\r\n"), COLOR_BLUE, 200);
		SAFE_DELETE_ARRAY(pY);
		return FALSE;
	}

	int i = 0;
	int j = 0;
	for(i = 0; i < _OFOPTCenterIn->Height; i++)
	{
		for (j = 0; j < _OFOPTCenterIn->Width; j++)
		{
			int k = i * _OFOPTCenterIn->Width + j;
			//pDest[i]=(114*pSrc[i*3]+587*pSrc[i*3+1]+299*pSrc[i*3+2]+500)/1000;
			pY[k].Y_Value = (114*pRGB24[k*3] + 587*pRGB24[k*3+1] + 299*pRGB24[k*3+2]+500)/1000;	//加500的目的是四舍五入
			pY[k].xIndex = j;
			pY[k].yIndex = i;
		}
	}

#if OC_TEST_SAVE_PIC_DEFINE == 1
	{
		USES_CONVERSION;

		CString FilePath = m_pInterface->Ctrl_GetCurrentPath() + _T("OC_Test_Moto_SavePic\\");
		_mkdir(T2A(FilePath));

		CString FileName = FilePath + _T("OC_Test_Moto_SavePic_R");
		SaveRaw(FileName, pR, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);

		FileName = FilePath + _T("OC_Test_Moto_SavePic_Gr");
		SaveRaw(FileName, pGr, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);

		FileName = FilePath + _T("OC_Test_Moto_SavePic_Gb");
		SaveRaw(FileName, pGb, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);

		FileName = FilePath + _T("OC_Test_Moto_SavePic_B");
		SaveRaw(FileName, pB, (_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4);
	}
#endif

	//获取Optlcal center的坐标和distance
	#if 0	//小容量变量不要new，防止内存碎片--Added by LiHai--20180314
	OFOPTCenterPixInfo  *OPTCenterPix = new OFOPTCenterPixInfo;
	if (NULL == OPTCenterPix)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("OpticalCenterTest_Y()---OPTCenterPix is NULL\r\n"), COLOR_BLUE, 200);
		SAFE_DELETE_ARRAY(pY);
		return FALSE;
	}

	double m_Distance = 0;

	getOptCenter(pY, OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,MONO);	//注意Y通道计算时最后参数使用MONO
	{
		CString str;

		str.Format(_T("pY---xIndex:%d   yIndex:%d   %.04f\r\n"), OPTCenterPix->xIndex, OPTCenterPix->yIndex, m_Distance * _OFOPTCenterIn->pixelSize);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
	}

	_OFOPTCenterOut->YVal_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->YVal_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->YVal_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	SAFE_DELETE_ARRAY(pY);
	SAFE_DELETE_ARRAY(OPTCenterPix);
	#else
	OFOPTCenterPixInfo  OPTCenterPix;
	double m_Distance = 0;

	getOptCenter(pY, &OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,MONO);	//注意Y通道计算时最后参数使用MONO
	{
		CString str;

		str.Format(_T("pY---xIndex:%d   yIndex:%d   %.04f\r\n"), OPTCenterPix.xIndex, OPTCenterPix.yIndex, m_Distance * _OFOPTCenterIn->pixelSize);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);
	}

	_OFOPTCenterOut->YVal_OpticalPixel.xIndex  = OPTCenterPix.xIndex;
	_OFOPTCenterOut->YVal_OpticalPixel.yIndex  = OPTCenterPix.yIndex;

	_OFOPTCenterOut->YVal_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	SAFE_DELETE_ARRAY(pY);
	#endif

	return TRUE;
}
//#endif

BOOL OC_Item_Moto_TestItem::OpticalCenterTest_Mono(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut_Mono * _OFOPTCenterOut)
{
	//存放4channel的信息
	OFOPTCenterPixInfo *pNewBuffer = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)];
	if (NULL == pNewBuffer)
	{
		SAFE_DELETE_ARRAY(pNewBuffer);
		return FALSE;
	}
	for (int i=0; i<_OFOPTCenterIn->Height; i++)
	{
		for (int j=0; j<_OFOPTCenterIn->Width; j++)
		{
			int Pos = i*_OFOPTCenterIn->Width + j;
			pNewBuffer[Pos].Y_Value = _OFOPTCenterIn->ImageBufferIn[Pos];
			pNewBuffer[Pos].xIndex = j;
			pNewBuffer[Pos].yIndex = i;
		}
	}

	//获取Optlcal center的坐标和distance
	OFOPTCenterPixInfo  *OPTCenterPix = new OFOPTCenterPixInfo;
	if (NULL == OPTCenterPix)
	{
		SAFE_DELETE_ARRAY(pNewBuffer);
		return FALSE;
	}
	double m_Distance = 0;

	getOptCenter(pNewBuffer, OPTCenterPix, _OFOPTCenterIn->Width, _OFOPTCenterIn->Height, &m_Distance, MONO);

	_OFOPTCenterOut->OpticalPixel.xIndex = OPTCenterPix->xIndex;
	_OFOPTCenterOut->OpticalPixel.yIndex = OPTCenterPix->yIndex;

	_OFOPTCenterOut->OC_Out = m_Distance * (_OFOPTCenterIn->pixelSize);

	SAFE_DELETE_ARRAY(pNewBuffer);
	SAFE_DELETE_ARRAY(OPTCenterPix);

	return TRUE;
}

BOOL OC_Item_Moto_TestItem::SaveAWBData(void)
{
	CString FilePath;
	CString FileName;
	USES_CONVERSION;

	FilePath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\"), CamID);
	FilePath = m_pInterface->Ctrl_GetCurrentPath() + FilePath;

	int siteNo = m_pInterface->GetSiteNo();
	_mkdir(T2A(FilePath));
	FileName.Format(_T("OCData_%d.ini"), siteNo);//写入的路径为【F:\OHP0792\Execute\Debug\otpdataManager】, 还可以使用m_pInterface->Otp_WriteToOtpBoard函数存储
	FileName = FilePath + FileName;
	//删除旧文件
	if (PathFileExists(FileName))
	{
		if (!DeleteFile(FileName))
		{
			CString TempMsg;

			TempMsg.Format(_T("[%s]:无法删除旧OTP文件!"), __FUNCTION__);
			m_pInterface->Ctrl_Addlog(CamID, TempMsg, COLOR_RED, 200);
			return FALSE;
		}
	}
	ofstream outfile(FileName, ios::out | ios::trunc);

	outfile << "[OCData]" << endl;

	if (!pDlg->m_isMonoSensor)
	{
		switch (pDlg->m_isBayerSingleChannel)
		{
		case OC_BAYER_MODE_SINGLE_CHANNEL:
			{
				outfile << "OC_R_X=" << CenterOut.R_OpticalPixel.xIndex << endl;
				outfile << "OC_R_Y=" << CenterOut.R_OpticalPixel.yIndex << endl;

				outfile << "OC_Gr_X=" << CenterOut.Gr_OpticalPixel.xIndex << endl;
				outfile << "OC_Gr_Y=" << CenterOut.Gr_OpticalPixel.yIndex << endl;

				outfile << "OC_Gb_X=" << CenterOut.Gb_OpticalPixel.xIndex << endl;
				outfile << "OC_Gb_Y=" << CenterOut.Gb_OpticalPixel.yIndex << endl;

				outfile << "OC_B_X=" << CenterOut.B_OpticalPixel.xIndex << endl;
				outfile << "OC_B_Y=" << CenterOut.B_OpticalPixel.yIndex << endl;
			}
			break;

		case OC_BAYER_MODE_Y:
			{
				outfile << "OC_YVal_X=" << CenterOut.YVal_OpticalPixel.xIndex << endl;
				outfile << "OC_YVal_Y=" << CenterOut.YVal_OpticalPixel.yIndex << endl;
			}
			break;

		default:
			break;
		}
	}
	else
	{
		outfile << "OC_Mono_X=" << CenterOut_Mono.OpticalPixel.xIndex << endl;
		outfile << "OC_Mono_Y=" << CenterOut_Mono.OpticalPixel.yIndex << endl;
	}
	
	outfile.close();

	return TRUE;
}

#if OC_TEST_SAVE_PIC_DEFINE == 1
void OC_Item_Moto_TestItem::SaveRaw(CString PathName, OFOPTCenterPixInfo *pCh, int nLen)
{
	USHORT *pCh0 = new USHORT[nLen];
	if (NULL == pCh0)
	{
		return;
	}

	int i;
	for (i = 0; i < nLen; i++)
	{
		pCh0[i] = pCh->Y_Value;
		pCh++;
	}

	PathName = PathName + _T(".bin");

	CFile file;
	BOOL ret = file.Open(PathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (!ret)
	{
		delete []pCh0;
		return;
	}
	file.Write((void*) pCh0, nLen * sizeof(USHORT) );
	file.Close();

	CString str;
	str.Format(_T("已保存到：%s"),PathName);

	delete []pCh0;
}
#endif
