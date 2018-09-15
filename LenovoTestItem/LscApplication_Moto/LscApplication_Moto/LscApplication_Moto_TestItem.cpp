#include "StdAfx.h"
#include "LscApplication_Moto_TestItem.h"
#include "ImageProc.h"

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new LscApplication_Moto_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
LscApplication_Moto_TestItem::LscApplication_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

LscApplication_Moto_TestItem::~LscApplication_Moto_TestItem(void)
{

}


int LscApplication_Moto_TestItem::InitItem()
{
	CamID = GetCamID();
	CurrentName=GetName();
	GlobalSetting=_T("LSCSetting");
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption();

	CString CurrentName = GetName();
	SetName(_T("WBSetting"));

	m_strTestStation = ReadConfigString(_T("TestStation"), _T("XXXXX"));

	SetName(CurrentName);

	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int LscApplication_Moto_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
	vtYDecay.clear();

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int LscApplication_Moto_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	if (pDlg->m_QLSC)
	{
		if (!Qualcomm_LSCCal())
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("LSC Test Fail!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Fail!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("LSC Test Pass!"),COLOR_GREEN,200);
			//m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Pass!"));
			SetResult(RESULT_PASS);
		}

		if (TRUE == pDlg->m_bSavShadingDataPerBlock)
		{
			USHORT OTPData[2000] = {0};
			ReadBinData(OTPData);//读出高通工具生成的1768个字节的LSC数据
			SaveLSCData(OTPData);

			float R_Channel[221] = {0};
			float Gr_Channel[221] = {0};
			float Gb_channel[221] = {0};
			float B_Channel[221] = {0};

			ReadDataFromTXT(R_Channel, Gr_Channel, Gb_channel, B_Channel);
			SaveColorShadingData(R_Channel, Gr_Channel, Gb_channel, B_Channel);
		}
	}
	if (pDlg->m_MLSC)
	{
		if (!MTK_LSCCal())
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Fail!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Pass!"));
			SetResult(RESULT_PASS);
		}
	}
	if (pDlg->m_MLSC_M)
	{
		if (!MTKMONO_LSCCal())
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Fail!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Pass!"));
			SetResult(RESULT_PASS);
		}
	}
	if (pDlg->m_SLSC)
	{
		if (!Sensor_LSCCal())
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Fail!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC Test Pass!"));
			SetResult(RESULT_PASS);
		}
	}
	
    return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int LscApplication_Moto_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    MTKLSC_Paramter
函数功能:  用于配置MTKLSC Paramter
返回值：   无返回值
*******************************************************************/
void LscApplication_Moto_TestItem::MTKLSC_Paramter(CString Filepath)
{
	CString slim_param_capture=Filepath+"\\"+"slim_param_capture.txt";	
	CString lsv_param_capture=Filepath+"\\"+"lsv_param_capture.txt";	

	ImageInfo img=*m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧5失败!"),COLOR_RED,200);
		return;
	}

	fstream file(slim_param_capture);
	string line;
	int count=0;

	ofstream outfile("temp.txt",ios::out|ios::trunc);

	while(!file.eof())
	{
		count++;
		getline(file,line);
		if(count==2)
		{
			outfile<<"Capture_"<<img.imgwidth<<"x"<<img.imgheight<<".raw  //raw_image_data_in_capture_mode_lo"<<endl;
		}
		else if (count==3)
		{
			outfile<<"Capture_"<<img.imgwidth<<"x"<<img.imgheight<<".raw  //raw_image_data_in_capture_mode_md"<<endl;
		}
		else if(count==4)
		{
			outfile<<"Capture_"<<img.imgwidth<<"x"<<img.imgheight<<".raw  //raw_image_data_in_capture_mode_hi"<<endl;
		}
		else if (count==5)
		{
			outfile<<"Capture_"<<img.imgwidth<<"x"<<img.imgheight<<"_coef.txt  //shading_coef_table_for_capture_raw_image_data"<<endl;
		}
		else if(count==7)
		{
			outfile<<img.imgwidth<<"//src_width"<<endl;
		}
		else if (count==8)
		{
			outfile<<img.imgheight<<"//src_height"<<endl;
		}
		else if (count==16)
		{
			outfile<<img.imgwidth<<"//src_offset_x1"<<endl;
		}
		else if (count==17)
		{
			outfile<<img.imgheight<<"//src_offset_y1"<<endl;
		}
		else if (count==11)
		{
			outfile<<pDlg->m_xblock<<"//m_x_grid_num"<<endl;
		}
		else if (count==12)
		{
			outfile<<pDlg->m_yblock<<"//n_y_grid_num"<<endl;
		}
		else if (count==31)
		{
			outfile<<pDlg->m_OBvalue<<"//OB Value"<<endl;
		}
		else
			outfile<<line<<endl;	
	}
	outfile.close();
	file.close();
	ofstream outfile1(slim_param_capture,ios::out|ios::trunc);
	fstream file1("temp.txt");
	while(!file1.eof())
	{
		getline(file1,line);
		outfile1<<line<<endl;
	}
	outfile1.close();
	file1.close();
	system("del temp.txt");

	fstream file2(lsv_param_capture);
	ofstream outfile2("temp.txt",ios::out|ios::trunc);
	string line2;
	int count2=0;
	while(!file2.eof())
	{
		count2++;
		getline(file2,line2);
		if(count2==8)
		{
			outfile2<<img.imgwidth/2<<"//src_width"<<endl;
		}
		else if (count2==9)
		{
			outfile2<<img.imgheight/2<<"//src_height"<<endl;
		}

		else
			outfile2<<line2<<endl;	
	}
	outfile2.close();
	file2.close();
	ofstream outfile3(lsv_param_capture,ios::out|ios::trunc);
	fstream file3("temp.txt");
	while(!file3.eof())
	{
		getline(file3,line2);
		outfile3<<line2<<endl;
	}
	outfile3.close();
	file3.close();
	system("del temp.txt");
}
/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int LscApplication_Moto_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	//SetName(GlobalSetting);
	pDlg->m_MLSC=ReadConfigInt(_T("MTKLSC"),0);
	pDlg->m_MLSC_M=ReadConfigInt(_T("MTKLSCM"),0);
	pDlg->m_bSavShadingDataPerBlock = ReadConfigInt(_T("SavShadingDataPerBlock"), 0);
	pDlg->m_QLSC=ReadConfigInt(_T("QualcommLSC"),0);
	pDlg->m_SLSC=ReadConfigInt(_T("SensorLSC"),0);
	pDlg->m_SaveFlag=ReadConfigInt(_T("SaveFlag"),0);
	pDlg->m_LSCMode=ReadConfigInt(_T("LSCMode"),0);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_MTKSize=ReadConfigInt(_T("MTKSize"),1868);
	pDlg->m_xblock=ReadConfigInt(_T("Xblock"),0);
	pDlg->m_yblock=ReadConfigInt(_T("Yblock"),0);
	pDlg->m_OBvalue=ReadConfigInt(_T("OBValue"),0);

	pDlg->m_YDecayDiff = ReadConfigInt(_T("YDecayDiff"),5);
	pDlg->m_MaxYDecay  = ReadConfigInt(_T("MaxYDecay"),75);
	pDlg->m_MinYDecay  = ReadConfigInt(_T("MinYDecay"),65);

	pDlg->m_GrGbDiff = ReadConfigInt(_T("QLSC_GrGbDiff"),0);
	pDlg->m_RMax=ReadConfigInt(_T("QLSC_Rmax"),0);
	pDlg->m_RMin=ReadConfigInt(_T("QLSC_RMin"),0);
	pDlg->m_GrMax=ReadConfigInt(_T("QLSC_GrMax"),0);
	pDlg->m_GrMin=ReadConfigInt(_T("QLSC_GrMin"),0);
	pDlg->m_GbMax=ReadConfigInt(_T("QLSC_GbMax"),0);
	pDlg->m_GbMin=ReadConfigInt(_T("QLSC_GbMin"),0);
	pDlg->m_BMax=ReadConfigInt(_T("QLSC_BMax"),0);
	pDlg->m_BMin=ReadConfigInt(_T("QLSC_BMin"),0);
	pDlg->m_LSCDistance=ReadConfigInt(_T("LSC_Distance"),0);
	pDlg->Sensor_LSCTarget=ReadConfigInt(_T("Sensor_LSCTarget"),50);
	pDlg->Sensor_LSCGroup=ReadConfigInt(_T("Sensor_LSCGroup"),1);
	pDlg->Sensor_OBvalue=ReadConfigInt(_T("Sensor_OBvalue"),16);
	pDlg->nLenCRegCount=ReadConfigInt(_T("nLenCRegCount"),1);

	pDlg->RGMAX=ReadConfigInt(_T("QLSC_RGmax"),110);
	pDlg->RGMIN=ReadConfigInt(_T("QLSC_RGmin"),90);
	pDlg->BGMAX=ReadConfigInt(_T("QLSC_BGmax"),110);
	pDlg->BGMIN=ReadConfigInt(_T("QLSC_BGmin"),90);

	pDlg->LSCerror=ReadConfigInt(_T("LSCerror"),9);

	pDlg->m_GetDATForS5K4H7YX = (_T("GetDATForS5K4H7YX"),0);

	SetName(CurrentName);
	LSC_R_Max=pDlg->m_RMax;
	LSC_R_Min= pDlg->m_RMin;
	LSC_Gr_Max=pDlg->m_GrMax;
	LSC_Gr_Min=pDlg->m_GrMin;
	LSC_Gb_Max=pDlg->m_GbMax;
	LSC_Gb_Min=pDlg->m_GbMin;
	LSC_B_Max=pDlg->m_BMax;
	LSC_B_Min=pDlg->m_BMin;
	LSC_Distance=pDlg->m_LSCDistance;

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int LscApplication_Moto_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	
	//SetName(GlobalSetting);
	WriteConfigInt(_T("MTKLSC"),pDlg->m_MLSC);
	WriteConfigInt(_T("MTKLSCM"),pDlg->m_MLSC_M);
	WriteConfigInt(_T("SavShadingDataPerBlock"), pDlg->m_bSavShadingDataPerBlock);
	WriteConfigInt(_T("QualcommLSC"),pDlg->m_QLSC);
	WriteConfigInt(_T("SensorLSC"),pDlg->m_SLSC);
	WriteConfigInt(_T("SaveFlag"),pDlg->m_SaveFlag);
	WriteConfigInt(_T("LSCMode"),pDlg->m_LSCMode);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("MTKSize"),pDlg->m_MTKSize);
	WriteConfigInt(_T("Xblock"),pDlg->m_xblock);
	WriteConfigInt(_T("Yblock"),pDlg->m_yblock);
	WriteConfigInt(_T("OBValue"),pDlg->m_OBvalue);

	WriteConfigInt(_T("YDecayDiff"),pDlg->m_YDecayDiff);
	WriteConfigInt(_T("MaxYDecay"),pDlg->m_MaxYDecay);
	WriteConfigInt(_T("MinYDecay"),pDlg->m_MinYDecay);
	pDlg->m_xblock=ReadConfigInt(_T("Xblock"),0);
	pDlg->m_yblock=ReadConfigInt(_T("Yblock"),0);
	pDlg->m_OBvalue=ReadConfigInt(_T("OBValue"),0);

	WriteConfigInt(_T("QLSC_GrGbDiff"),pDlg->m_GrGbDiff);
	WriteConfigInt(_T("QLSC_Rmax"),pDlg->m_RMax);
	WriteConfigInt(_T("QLSC_RMin"),pDlg->m_RMin);
	WriteConfigInt(_T("QLSC_GrMax"),pDlg->m_GrMax);
	WriteConfigInt(_T("QLSC_GrMin"),pDlg->m_GrMin);
	WriteConfigInt(_T("QLSC_GbMax"),pDlg->m_GbMax);
	WriteConfigInt(_T("QLSC_GbMin"),pDlg->m_GbMin);
	WriteConfigInt(_T("QLSC_BMax"),pDlg->m_BMax);
	WriteConfigInt(_T("QLSC_BMin"),pDlg->m_BMin);
	WriteConfigInt(_T("LSC_Distance"),pDlg->m_LSCDistance);
	WriteConfigInt(_T("Sensor_LSCTarget"),pDlg->Sensor_LSCTarget);
	WriteConfigInt(_T("Sensor_LSCGroup"),pDlg->Sensor_LSCGroup);
	WriteConfigInt(_T("Sensor_OBvalue"),pDlg->Sensor_OBvalue);
	WriteConfigInt(_T("nLenCRegCount"),pDlg->nLenCRegCount);

	WriteConfigInt(_T("QLSC_RGmax"),pDlg->RGMAX);
	WriteConfigInt(_T("QLSC_RGMin"),pDlg->RGMIN);
	WriteConfigInt(_T("QLSC_BGMax"),pDlg->BGMAX);
	WriteConfigInt(_T("QLSC_BGMin"),pDlg->BGMIN);
	WriteConfigInt(_T("LSCerror"),pDlg->LSCerror);
	WriteConfigInt(_T("GetDATForS5K4H7YX"),pDlg->m_GetDATForS5K4H7YX);
	
	SetName(CurrentName);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR LscApplication_Moto_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	if (pDlg->m_QLSC)
	{
	 header+=_T("LSC_R_Max,LSC_Gr_Max,LSC_Gb_Max,LSC_B_Max,LSC_R_Min,LSC_Gr_Min,LSC_Gb_Min,LSC_B_Min,LSC_Distance,");
	}
	else if (pDlg->m_MLSC || pDlg->m_MLSC_M)
	{
		for (int i=0; i<13; i++)
		{
			TempMsg.Format(_T("Y_Decay_Block_%d,"),i+1);
			header += TempMsg;
		}
		
	}

	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR LscApplication_Moto_TestItem::GetReportLowLimit()
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
LPCTSTR LscApplication_Moto_TestItem::GetReportHightLimit()
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
LPCTSTR LscApplication_Moto_TestItem::GetReportContents()
{
	
	static CString Content;
	Content = _T("");
	if (pDlg->m_QLSC)
	{
        CString temp;
	    temp.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d,%d"),LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max,LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min,tempMax);
	    Content+=temp;
	}
	else if(pDlg->m_MLSC || pDlg->m_MLSC_M)
	{
		for (int i=0; i<13; i++)
		{
			TempMsg.Format(_T("%f,"),vtYDecay[i]);
			Content += TempMsg;
		}
	}
	
	return Content;
}

/******************************************************************
函数名:    Raw10toRaw8
函数功能:  用于将USHORT 类型Raw10 Buffer转换成BYTE 类型Raw8 Buffer
返回值：   无返回值
*******************************************************************/
void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
/******************************************************************
函数名:    MTK_LSCCal
函数功能:  MTK Color Sensor LSC Calibration
返回值：   返回 1 成功        返回0 失败
*******************************************************************/
BOOL LscApplication_Moto_TestItem::MTK_LSCCal()
{
	USES_CONVERSION;
	FilePath = m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Ref\\");
	//参数配置
	
	ImageInfo img=*m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧2失败!"),COLOR_RED,200);
		return FALSE;
	}

	FileName.Format(_T("Capture_%dx%d.raw"),img.imgwidth,img.imgheight);
	m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,FileName,4,NULL);   //BYTE *imgBuffer

	memset(m_LSCData,0,sizeof(m_LSCData));
	memset(LSCBurningData,0,sizeof(LSCBurningData));

	CString capture_Image=FilePath+"\\"+FileName;
	CString slim_param_capture=FilePath+"\\"+"slim_param_capture.txt";
	CString ShadingCalibration_Slim=FilePath+"\\"+"1_ShadingCalibrationSample.exe";
	CString TaskKill=_T("Taskkill /IM ") + ShadingCalibration_Slim;
	CString ShadingCalibrate=FilePath+"\\"+"ShadingCalibrate.dll";
	CString eeprom_slim=FilePath+"\\"+"eeprom_slim.txt";

	int Timeout=100;
	HANDLE hFile;
	if(!PathFileExists(ShadingCalibrate))
	{
		TempMsg.Format(_T("ShadingCalibrate.dll文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	if(!PathFileExists(ShadingCalibration_Slim))
	{
		TempMsg.Format(_T("1_ShadingCalibrationSample.exe文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	if(!PathFileExists(slim_param_capture))
	{
		TempMsg.Format(_T("slim_param_capture.txt文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	if (PathFileExists(eeprom_slim))
	{
		if (!DeleteFile(eeprom_slim))
		{
			TempMsg.Format(_T("无法删除旧eeprom_slim文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			return FALSE;
		}
	}
	SetCurrentDirectory(FilePath);
	ShellExecute(NULL,_T("open"),ShadingCalibration_Slim,_T(""),_T(""),SW_HIDE);
	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());

	unsigned char EEPROMData[2000] = {0};
	while (Timeout!=0)
	{
		hFile = CreateFile(eeprom_slim,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;

			ReadFile(hFile, EEPROMData, 2000, &nBytesRead, NULL );
			CloseHandle( hFile );

			if (nBytesRead != pDlg->m_MTKSize)
			{
				TempMsg.Format(_T("eeprom_slim大小不为%d Byte!"),pDlg->m_MTKSize);
				m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
				return FALSE;
			}
			break;
		}
		Sleep(100);
		Timeout--;
	}

	TempMsg.Format(_T(">Get LSC Successful!\n"));
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	
 	SetCurrentDirectory(FilePath);
	WinExec(T2A(TaskKill),SW_HIDE);
 	ShellExecute(NULL,_T("open"),TaskKill,_T(""),_T(""),SW_HIDE);
 	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());


	//Apply LSC

	CString after_ob=FilePath+"\\"+"after_ob.raw";
	CString ShadingCorrectionSample2=FilePath+"\\"+"2_ShadingCorrectionSample.exe";
	CString TaskKill2=_T("Taskkill /IM")+ShadingCorrectionSample2;
	CString resultFile=FilePath+"\\"+"result.txt";
	CString lsv_param_capture=FilePath+"\\"+"lsv_param_capture.txt";

	Timeout=100;
	if(!PathFileExists(ShadingCorrectionSample2))
	{
		TempMsg.Format(_T("2_ShadingCorrectionSample.exe文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	if(PathFileExists(resultFile))
	{
		if(!DeleteFile(resultFile))
		{
			TempMsg.Format(_T("无法删除旧result.txt文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			return FALSE;
		}
	}
	if(!PathFileExists(lsv_param_capture))
	{
		TempMsg.Format(_T("lsv_param_capture.txt文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	if(!PathFileExists(eeprom_slim))
	{
		TempMsg.Format(_T("eeprom_slim.txt文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	if(!PathFileExists(after_ob))
	{
		TempMsg.Format(_T("after_ob.raw文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}
	SetCurrentDirectory(FilePath);
	ShellExecute(NULL,_T("open"),ShadingCorrectionSample2,_T(""),_T(""),SW_HIDE);
	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());

	CString FileContent=_T("");
    BOOL bFind = FALSE;
	while (Timeout!=0)
	{
		hFile = CreateFile(resultFile,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			char BIN[1024] = {0};
			ReadFile(hFile, BIN, 1024, &nBytesRead, NULL );
			CloseHandle(hFile);	
			FileContent.Format(_T("%c%c%c%c"),BIN[1],BIN[2],BIN[3],BIN[4]);
			char* pToken = strtok(BIN, "\t");
            while (NULL != pToken)
            {
                if (0 == strcmp("Y_Decay", pToken))
                {
                    bFind = TRUE;
                }
                else
                if (TRUE == bFind)
                {
                    vtYDecay.push_back(atof(pToken)+100.0);
                    bFind = FALSE;
                }

                pToken = strtok(NULL, "\t");
            }
		}		
		Sleep(50);
		Timeout--;
	}
	
 	SetCurrentDirectory(FilePath);
	WinExec(T2A(TaskKill2),SW_HIDE);
 	ShellExecute(NULL,_T("open"),TaskKill2,_T(""),_T(""),SW_HIDE);
 	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());

	if (FileContent !=_T("pass"))
	{
		TempMsg.Format(_T("运行结果为:%s"),FileContent);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return FALSE;
	}

	float yDecay[4] = {0.0};
	yDecay[0] = vtYDecay[0];
	yDecay[1] = vtYDecay[2];
	yDecay[2] = vtYDecay[vtYDecay.size()-3];
	yDecay[3] = vtYDecay[vtYDecay.size()-1];

	float YDecayMax = yDecay[0];
	float YDecayMin = yDecay[0];

	for (int i=1; i<4; i++)
	{
		if(YDecayMax > yDecay[i]) YDecayMax = yDecay[i];
		if(YDecayMin < yDecay[i]) YDecayMin = yDecay[i];
	}

    TempMsg.Format(_T("YDecay1:%f, YDecay2:%f, YDecay3:%f, YDecay4:%f"), vtYDecay[0], vtYDecay[2], vtYDecay[vtYDecay.size()-3], vtYDecay[vtYDecay.size()-1]);
    m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("YDecayMax:%f, YDecayMin:%f"),YDecayMax,YDecayMin);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	if ((YDecayMax-YDecayMin) > pDlg->m_YDecayDiff)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("YDecay Diff超范围!"), COLOR_RED, 200);
		return FALSE;
	}
	
	BOOL bExpVal = (vtYDecay[0] < pDlg->m_MinYDecay) || (vtYDecay[0] > pDlg->m_MaxYDecay)
                    || (vtYDecay[2] < pDlg->m_MinYDecay) || (vtYDecay[2] > pDlg->m_MaxYDecay)
                    || (vtYDecay[vtYDecay.size()-1] < pDlg->m_MinYDecay) || (vtYDecay[vtYDecay.size()-1] > pDlg->m_MaxYDecay)
                    || (vtYDecay[vtYDecay.size()-3] < pDlg->m_MinYDecay) || (vtYDecay[vtYDecay.size()-3] > pDlg->m_MaxYDecay);
    if (TRUE == bExpVal)
    {
        m_pInterface->Ctrl_Addlog(CamID, _T("YDecay超范围!"), COLOR_RED, 200);
        return FALSE;
    }
    else
    {
        m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
    }

	BurnData.clear();
	for (int i=0;i<pDlg->m_MTKSize;i++)
	{
		BurnData.push_back(EEPROMData[i]);
	}
	
	int k=pDlg->LSCerror;
	
	for(int i=0;i<(pDlg->m_MTKSize-k);i++)
	{  
	   int count=0;
	   for(int j=0;j<k;j++)
	   {
	    if((EEPROMData[i+j]==0xff)||(EEPROMData[i+j]==0))
		{
		   count++;
		}
	   }
	   if(count>=k)
	   {
	    m_pInterface->Ctrl_Addlog(CamID,_T("LSC数据生成错误，多个连续数据为0!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return 0;
	   }
	   
	}
	
	int siteNo=m_pInterface->GetSiteNo();
	FileName.Format(_T("LSC_%d.bin"), siteNo);
	if(!m_pInterface->Otp_WriteToOtpBoard(CamID,FileName,BurnData,1))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Bin文件生成失败!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return 0;
	}
	return TRUE;
}
/******************************************************************
函数名:    MTKMONO_LSCCal
函数功能:  MTK MONO Sensor LSC Calibration
返回值：   返回 1 成功        返回0 失败
*******************************************************************/
BOOL LscApplication_Moto_TestItem::MTKMONO_LSCCal()
{
	//TODO:在此添加测试项代码
	USES_CONVERSION;

	ImageInfo img=*m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧3失败!"),COLOR_RED,200);
		return FALSE;
	}
	FilePath = m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\MONORef\\");
	MTKLSC_Paramter(FilePath);
	FileName.Format(_T("MONO__%dx%d_10_0.raw"),img.imgwidth,img.imgheight);
	m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,FileName,3,NULL); 
	memset(m_LSCData,0,sizeof(m_LSCData));
	memset(LSCBurningData,0,sizeof(LSCBurningData));

	CString capture_Image=FilePath+"\\"+FileName;

	CString Raw10to8;
	Raw10to8.Format(_T("10to8.exe %d %d MONO__%dx%d_10_0.raw"),img.imgwidth,img.imgheight,img.imgwidth,img.imgheight);
	CString Raw10toraw8=FilePath+Raw10to8;
	CString slim_param_capture=FilePath+"\\"+"slim_param_capture.txt";
	CString ShadingCalibration_Slim=FilePath+"\\"+"1.ShadingCalibrationSample.exe";
	CString TaskKill=_T("Taskkill /IM ") + ShadingCalibration_Slim;
	CString ShadingCalibrate=FilePath+"\\"+"ShadingCalibrate.dll";
	CString eeprom_slim=FilePath+"\\"+"eeprom_slim.txt";

	int Timeout=100;
	HANDLE hFile;
	if(!PathFileExists(capture_Image))
	{
		TempMsg.Format(_T("Capture Image不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if(!PathFileExists(ShadingCalibrate))
	{
		TempMsg.Format(_T("ShadingCalibrate.dll文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if(!PathFileExists(ShadingCalibration_Slim))
	{
		TempMsg.Format(_T("1.ShadingCalibrationSample.exe文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if(!PathFileExists(slim_param_capture))
	{
		TempMsg.Format(_T("slim_param_capture.txt文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if (PathFileExists(eeprom_slim))
	{
		if (!DeleteFile(eeprom_slim))
		{
			TempMsg.Format(_T("无法删除旧eeprom_slim文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
			return FALSE;
		}
	}

	SetCurrentDirectory(FilePath);
	WinExec(T2A(Raw10toraw8),SW_SHOW);
	Sleep(500);
	ShellExecute(NULL,_T("open"),ShadingCalibration_Slim,_T(""),_T(""),SW_SHOW);
	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());
	
	unsigned char EEPROMData[2000] = {0};
	while (Timeout!=0)
	{
		hFile = CreateFile(eeprom_slim,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;

			ReadFile(hFile, EEPROMData, 2000, &nBytesRead, NULL );
			CloseHandle( hFile );

			if (nBytesRead != pDlg->m_MTKSize)
			{
				TempMsg.Format(_T("eeprom_slim大小不为%d Byte!"),pDlg->m_MTKSize);
				m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
				return FALSE;
			}
			break;
		}
		Sleep(100);
		Timeout--;
	}

	TempMsg.Format(_T(">Get LSC Successful!\n"));
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	
	SetCurrentDirectory(FilePath);
	WinExec(T2A(TaskKill),SW_HIDE);
 	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());


	//Apply LSC

	CString after_ob=FilePath+"\\"+"after_ob.raw";
	CString ShadingCorrectionSample2=FilePath+"\\"+"2.ShadingCorrectionSample.exe";
	CString TaskKill2=_T("Taskkill /IM")+ShadingCorrectionSample2;
	CString resultFile=FilePath+"\\"+"result.txt";
	CString lsv_param_capture=FilePath+"\\"+"lsv_param_capture.txt";

	Timeout=100;
	if(!PathFileExists(ShadingCorrectionSample2))
	{
		TempMsg.Format(_T("2.ShadingCorrectionSample.exe文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if(PathFileExists(resultFile))
	{
		if(!DeleteFile(resultFile))
		{
			TempMsg.Format(_T("无法删除旧result.txt文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
			return FALSE;
		}
	}
	if(!PathFileExists(lsv_param_capture))
	{
		TempMsg.Format(_T("lsv_param_capture.txt文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if(!PathFileExists(eeprom_slim))
	{
		TempMsg.Format(_T("eeprom_slim.txt文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	if(!PathFileExists(after_ob))
	{
		TempMsg.Format(_T("after_ob.raw文件不存在!"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}
	SetCurrentDirectory(FilePath);
	ShellExecute(NULL,_T("open"),ShadingCorrectionSample2,_T(""),_T(""),SW_HIDE);
	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());
	Sleep(500);

	CString FileContent=_T("");
	BOOL bFind = FALSE;

	while (Timeout!=0)
	{
		hFile = CreateFile(resultFile,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			char BIN[1024] = {0};
			ReadFile(hFile, BIN, 1024, &nBytesRead, NULL );
			CloseHandle(hFile);	
			FileContent.Format(_T("%c%c%c%c"),BIN[1],BIN[2],BIN[3],BIN[4]);

			char* pToken = strtok(BIN, "\t");
			while (NULL != pToken)
			{
				if (0 == strcmp("Y_Decay", pToken))
				{
					bFind = TRUE;
				}
				else
					if (TRUE == bFind)
					{
						vtYDecay.push_back(atof(pToken)+100.0);
						bFind = FALSE;
					}

					pToken = strtok(NULL, "\t");
			}
		}		
		Sleep(50);
		Timeout--;
	}

	
 	SetCurrentDirectory(FilePath);
	WinExec(T2A(TaskKill2),SW_HIDE);
// ShellExecute(NULL,_T("open"),TaskKill2,_T(""),_T(""),SW_HIDE);
 	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());

	if (FileContent !="pass")
	{
		TempMsg.Format(_T("运行结果为:%s"),FileContent);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		return FALSE;
	}

	float yDecay[4] = {0.0};
	yDecay[0] = vtYDecay[0];
	yDecay[1] = vtYDecay[2];
	yDecay[2] = vtYDecay[vtYDecay.size()-3];
	yDecay[3] = vtYDecay[vtYDecay.size()-1];

	float YDecayMax = yDecay[0];
	float YDecayMin = yDecay[0];

	for (int i=1; i<4; i++)
	{
		if(YDecayMax > yDecay[i]) YDecayMax = yDecay[i];
		if(YDecayMin < yDecay[i]) YDecayMin = yDecay[i];
	}

	TempMsg.Format(_T("YDecay1:%f, YDecay2:%f, YDecay3:%f, YDecay4:%f"), vtYDecay[0], vtYDecay[2], vtYDecay[vtYDecay.size()-3], vtYDecay[vtYDecay.size()-1]);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("YDecayMax:%f, YDecayMin:%f"),YDecayMax,YDecayMin);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	if ((YDecayMax-YDecayMin) > pDlg->m_YDecayDiff)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("YDecay Diff超范围!"), COLOR_RED, 200);
		return FALSE;
	}

	BOOL bExpVal = (vtYDecay[0] < pDlg->m_MinYDecay) || (vtYDecay[0] > pDlg->m_MaxYDecay)
		|| (vtYDecay[2] < pDlg->m_MinYDecay) || (vtYDecay[2] > pDlg->m_MaxYDecay)
		|| (vtYDecay[vtYDecay.size()-1] < pDlg->m_MinYDecay) || (vtYDecay[vtYDecay.size()-1] > pDlg->m_MaxYDecay)
		|| (vtYDecay[vtYDecay.size()-3] < pDlg->m_MinYDecay) || (vtYDecay[vtYDecay.size()-3] > pDlg->m_MaxYDecay);
	if (TRUE == bExpVal)
	{
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID, _T("YDecay超范围!"), COLOR_RED, 200);
		return FALSE;
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}

	BurnData.clear();
	for (int i=0;i<pDlg->m_MTKSize;i++)
	{
		BurnData.push_back(EEPROMData[i]);
	}
	int siteNo=m_pInterface->GetSiteNo();
	FileName.Format(_T("LSCMono_%d.bin"), siteNo);
	m_pInterface->Otp_WriteToOtpBoard(CamID,FileName,BurnData,1);

	return TRUE;
}

void LscApplication_Moto_TestItem::SaveColorShadingData(float * Color_RG , float * Color_BG , BOOL Result)
{
    USES_CONVERSION;

    int i = 0;
    FILE  *fp  = NULL;
    CTime time = CTime::GetCurrentTime();

    CString RecordTime = _T("");
	RecordTime.Format(_T("%02d:%02d:%02d"), time.GetHour(), time.GetMinute(), time.GetSecond());
    CFile MyFile;
    long fileSizeTemp=0;

    CString FileName;
    SetName(_T("GENERAL_SETTING"));
    TCHAR szReportPath[MAX_PATH] = {0};
    CString strReportPath = ReadConfigString(_T("sReportPath"), szReportPath, MAX_PATH);
    FileName.Format(_T("%s\\LSC13x17_Test_Site_A%04d%02d%02d.xls"), strReportPath.GetBuffer(), time.GetYear(), time.GetMonth(), time.GetDay());

    m_pInterface->Ctrl_Addlog(CamID,FileName,COLOR_BLUE,200);
    CString strLogInfo = _T("");
    fp=freopen(CT2A(FileName),"at",stdout);
    if(fp==NULL)  
    {
        strLogInfo.Format(_T("%s文件打开失败，数据保存失败!"), FileName.GetBuffer(FileName.GetLength()));
        m_pInterface->Ctrl_Addlog(CamID, strLogInfo, COLOR_RED, 200);
        return;
    }

    fileSizeTemp=ftell(fp);
    fseek(fp,0,SEEK_END );
    fileSizeTemp=ftell(fp);
    if(fileSizeTemp==0)
    {
        printf("SN\tFUSE ID\tTest Time\t");//增加SN的保存 LYC 20180528
        for (i = 0; i < 221; i++)
        {
            printf( "%s%d\t", "RG", i);
        }	
        for (i = 0; i < 221; i++)
        {
            printf( "%s%d\t", "BG", i);
        }
    }

	printf("\n");
	printf(T2A(m_pInterface->Ctrl_GetCamCode()));//增加SN的保存 LYC 20180528
	printf("\t");
    printf(CT2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
    printf("#\t");

    printf(CT2A(RecordTime));
    printf("\t");

    for (i = 0; i < 221; i++)
    {
        printf( "%.4f\t",Color_RG[i]);
    }	
    for (i = 0; i < 221; i++)
    {
        printf( "%.4f\t",Color_BG[i]);
    }	

    fclose(fp);
    SetName(CurrentName);
}

/******************************************************************
函数名:    LSCDataCheck
函数功能:  Qualcomm LSC Data Check
返回值：   返回 1 成功        返回0 失败
*******************************************************************/
BOOL LscApplication_Moto_TestItem::LSCDataCheck(USHORT *m_LSCData)
{
	USES_CONVERSION;
	FILE *fp = NULL;

	CString sfilename;
	CString sTmp;
	CString sfileext;

	CFileFind ff;

	CString   SensorINI =m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Qualcomm\\Standard.ini");  // 设定参数文档路径--里面是884个数据

	sfilename.Format(SensorINI);

	fp = fopen(T2A(sfilename), "a");
	if(fp==NULL)
	{
		TempMsg.Format(_T("没有Standard.ini文件"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return 0;
	}
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeRead | CFile::typeText);  

	int StandardBuf[17*13][4] = {0};

	for( int i = 0; i < 221; i++ )
	{
		MyFile.ReadString( sTmp);
		StandardBuf[i][0]=atoi( T2A(sTmp));

		MyFile.ReadString( sTmp);
		StandardBuf[i][1]=atoi( T2A(sTmp));

		MyFile.ReadString( sTmp);
		StandardBuf[i][2]=atoi( T2A(sTmp));

		MyFile.ReadString( sTmp);
		StandardBuf[i][3]=atoi( T2A(sTmp));
	}

	MyFile.Close();


	int tempBuf[17*13][4] = {0};

	int cnt= 0;

	
	LSCData_R_Max=LSCData_Gr_Max=LSCData_Gb_Max=LSCData_B_Max=0;
	
	LSCData_R_Min=LSCData_Gr_Min=LSCData_Gb_Min=LSCData_B_Min=1023;
	tempMax = 0;

	int GrGbDiff = 0;
	int tempGrGbDiff = 0;

	for( int i = 0; i < 221; i++ )
	{
		tempBuf[i][0] = m_LSCData[i*2]+(m_LSCData[i*2+1]<<8) ;
		tempBuf[i][1] = m_LSCData[i*2+442]+(m_LSCData[i*2+1+442]<<8) ;
		tempBuf[i][2] = m_LSCData[i*2+884]+(m_LSCData[i*2+1+884]<<8) ;
		tempBuf[i][3] = m_LSCData[i*2+1326]+(m_LSCData[i*2+1+1326]<<8) ;

		if (tempBuf[i][0]<LSCData_R_Min)
		{
			LSCData_R_Min=tempBuf[i][0];
		}
		if (tempBuf[i][0]>LSCData_R_Max)
		{
			LSCData_R_Max=tempBuf[i][0];
		}
		if (tempBuf[i][1]<LSCData_Gr_Min)
		{
			LSCData_Gr_Min=tempBuf[i][1];
		}
		if (tempBuf[i][1]>LSCData_Gr_Max)
		{
			LSCData_Gr_Max=tempBuf[i][1];
		}
		if (tempBuf[i][2]<LSCData_Gb_Min)
		{
			LSCData_Gb_Min=tempBuf[i][2];
		}
		if (tempBuf[i][2]>LSCData_Gb_Max)
		{
			LSCData_Gb_Max=tempBuf[i][2];
		}
		if(tempBuf[i][3]<LSCData_B_Min)
		{
			LSCData_B_Min=tempBuf[i][3];
		}
		if (tempBuf[i][3]>LSCData_B_Max)
		{
			LSCData_B_Max=tempBuf[i][3];
		}

		tempGrGbDiff = abs(tempBuf[i][1]-tempBuf[i][2]);//增加Gr与Gb值的Diff卡控 LYC 20180528

		if (tempGrGbDiff > GrGbDiff)
		{
			GrGbDiff = tempGrGbDiff;
		}
	}

	if (GrGbDiff > pDlg->m_GrGbDiff)
	{
		TempMsg.Format(_T("Gr Gb Diff = %d > %d !"),GrGbDiff,pDlg->m_GrGbDiff);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return 0;
	}
	
	int tempDistance = 0;
	for( int i = 0; i < 17*13; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			tempDistance = abs(tempBuf[i][j] - StandardBuf[i][j]);

			if( tempDistance > tempMax )
			{
				tempMax = tempDistance;
			}		 
		}
	}


	if (LSCData_R_Max>LSC_R_Max||LSCData_R_Min<LSC_R_Min
		||LSCData_Gr_Max>LSC_Gr_Max||LSCData_Gr_Min<LSC_Gr_Min
		||LSCData_Gb_Max>LSC_Gb_Max||LSCData_Gb_Min<LSC_Gb_Min
		||LSCData_B_Max>LSC_B_Max||LSCData_B_Min<LSC_B_Min
		)
	{	
		TempMsg.Format(_T("R_Max=%d  Gr_Max=%d Gb_Max=%d B_Max=%d "),LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max);
    	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);

		TempMsg.Format(_T("R_Min=%d  Gr_Min=%d Gb_Min=%d B_Min=%d "),LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min);
    	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return 0;
	}
	TempMsg.Format(_T("R_Max=%d  Gr_Max=%d Gb_Max=%d B_Max=%d "),LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	TempMsg.Format(_T("R_Min=%d  Gr_Min=%d Gb_Min=%d B_Min=%d "),LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	TempMsg.Format(_T("LSC Distance为%d !"),tempMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	///////////////Save the Data///////////
	CString FileName = _T("");
	FileName =m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Qualcomm\\LSCData.txt");

	if(PathFileExists(FileName))//增加删除之前按CamID分文件夹保存的LSC数据文本后再新建一个 LYC 20180527
	{
		if(!DeleteFile(FileName))
		{
			TempMsg.Format(_T("无法删除旧LSCData.txt文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
			return FALSE;
		}
	}
	HANDLE hFile = CreateFile(FileName,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	CloseHandle( hFile );
	FILE  *fpn  = NULL;
	   
	fpn=freopen(T2A(FileName),"at",stdout);
	if(fpn==NULL)  
	{
		TempMsg.Format(_T("没有LSCData.txt"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return 0;
	}

	float CenterRG=(float)tempBuf[111][0]*2/(tempBuf[111][1]+tempBuf[111][2]);
	float CenterBG=(float)tempBuf[111][3]*2/(tempBuf[111][1]+tempBuf[111][2]);
	TempMsg.Format(_T("CenterRG=%0.3f"),CenterRG);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("CenterBG=%0.3f"),CenterBG);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		
		
	float tempRG[221]={0};
	float tempBG[221]={0};
	for(int i = 0; i < 17*13; i++ )
	{
		tempRG[i]=(float)tempBuf[i][0]*2/(float)(tempBuf[i][1]+tempBuf[i][2])/CenterRG;
		tempBG[i]=(float)tempBuf[i][3]*2/(float)(tempBuf[i][1]+tempBuf[i][2])/CenterBG;
		printf("RG=%0.3f  BG=%0.3f\n",tempRG[i],tempBG[i]);
	}
	fclose(fpn);//增加
    SaveColorShadingData(tempRG, tempBG, TRUE);
	int RGRatio_H=pDlg->RGMAX;
	int RGRatio_L=pDlg->RGMIN;
	int BGRatio_H=pDlg->BGMAX;
	int BGRatio_L=pDlg->BGMIN;
	for(int  i = 0; i < 221; i++ )
	{
        int tempG=tempRG[i]*100;
		if (tempG>RGRatio_H||tempG<RGRatio_L)
		{
			TempMsg.Format(_T("第%d Block RG超规格!"),i);
		    m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TempMsg.Format(_T("RGref为%0.3f "),tempRG[i]);
	        m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			return 0;
		}
		tempG=tempBG[i]*100;
		if (tempG>BGRatio_H||tempG<BGRatio_L)
		{
			TempMsg.Format(_T("第%d Block BG超规格!"),i);
		    m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TempMsg.Format(_T("BGref为%0.3f "),tempBG[i]);
	        m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			return 0;
		}
	}
	fpn=freopen(T2A(FileName),"at",stdout);//增加文件关闭后重新打开 解决后续数据没有保存的问题 LYC 20180528
	if(fpn==NULL)  
	{
		TempMsg.Format(_T("没有LSCData.txt"));
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		return 0;
	}
	for( int i = 0; i < 221; i++ )
	{
        printf("%d\n",tempBuf[i][0]);
		printf("%d\n",tempBuf[i][1]);
		printf("%d\n",tempBuf[i][2]);
		printf("%d\n",tempBuf[i][3]);
	}
	printf("R 最小为:%d R 最大为:%d",LSCData_R_Min,LSCData_R_Max);
	printf("\n");
    printf("Gr最小为:%d Gr最大为:%d",LSCData_Gr_Min,LSCData_Gr_Max);
	printf("\n");
    printf("Gb最小为:%d Gb最大为:%d",LSCData_Gb_Min,LSCData_Gb_Max);
	printf("\n");
    printf("B 最小为:%d B 最大为:%d",LSCData_B_Min,LSCData_B_Max);
	printf("\n");
	printf("LSCDistanceSpec为：%d",tempMax);
	printf("\n\n");
	fclose(fpn); 

	if( tempMax > LSC_Distance )
	{	
		TempMsg.Format(_T("LSC Distance为%d !"),tempMax);
	    m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
	
		return 0;
	} 

	return 1;
}
/******************************************************************
函数名:    Qualcomm_LSCCal
函数功能:  Qualcomm LSC Calibration
返回值：   返回 1 成功        返回0 失败
*******************************************************************/
BOOL LscApplication_Moto_TestItem::Qualcomm_LSCCal()
{
	ImageInfo *MyImageInfo;
	MyImageInfo=m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == MyImageInfo->Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧1失败!"));
		return FALSE;
	}
	int width=MyImageInfo->imgwidth;
	int height=MyImageInfo->imgheight;

	LPBYTE MyRaw8Buff=new BYTE[width*height];
	if (NULL == MyRaw8Buff)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("MyRaw8Buff 申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("MyRaw8Buff 申请失败失败!"));
		return FALSE;
	}
	memset(MyRaw8Buff,0,width*height);
	Raw10toRaw8(MyImageInfo->Raw_buffer,MyRaw8Buff,width*height);
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7);

	HINSTANCE hDll;   //DLL句柄 
	CString m_szDirectory;
	m_szDirectory=m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\");
	SetCurrentDirectory(m_szDirectory);
	m_szDirectory=m_szDirectory+_T("Qualcomm\\");
	SetCurrentDirectory(m_szDirectory);
	CString DllPath;
	DllPath=m_szDirectory+_T("LSCCalibrationDll.dll");
	hDll = LoadLibrary(DllPath);
	if (NULL==hDll)
	{
		SAFE_DELETE_ARRAY(MyRaw8Buff);
		FreeLibrary(hDll);
		m_pInterface->Ctrl_Addlog(CamID,_T("LSCCalibrationDll.dll丢失,请将文件放回程式目录!"),COLOR_RED,200);
		return FALSE;
	}

	lpFun LensCorrection = (lpFun)GetProcAddress(hDll,"LensCorrectionLib"); 
	if (NULL==LensCorrection)
	{
		SAFE_DELETE_ARRAY(MyRaw8Buff);
		FreeLibrary(hDll);
		m_pInterface->Ctrl_Addlog(CamID,_T("DLL中函数寻找失败!"),COLOR_RED,200);
		return FALSE;
	}
	int CFA_Pattern ;
	
	if ((MyImageInfo->RawFormat == 0) ||
		(MyImageInfo->RawFormat == 1))
	{
		CFA_Pattern =MyImageInfo->RawFormat;
	}
	if (MyImageInfo->RawFormat == 2)
	{
		CFA_Pattern = 3;
	}
	if (MyImageInfo->RawFormat == 3)
	{
		CFA_Pattern = 2;
	}
	int flag = LensCorrection(MyRaw8Buff,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_LSCMode);
	FreeLibrary(hDll);
	SAFE_DELETE_ARRAY(MyRaw8Buff);

	USHORT OTPData[2000];
	memset(OTPData,0,2000*sizeof(USHORT));
	BurnData.clear();
	if (pDlg->m_SaveFlag==0)
	{
       ReadBinData(OTPData);//读出高通工具生成的1768个字节的LSC数据
	}
	if(pDlg->m_SaveFlag==1)
	{
      float R_Channel[221] = {0};
	  float Gr_Channel[221] = {0};
	  float Gb_channel[221] = {0};
	  float B_Channel[221] = {0};
	  int R_Hex,Gr_Hex,Gb_Hex,B_Hex;
	  
	  ReadDataFromTXT(R_Channel, Gr_Channel, Gb_channel, B_Channel);
	  int j = 0;
	  for (int i = 0; i < 221; i++)
	  {
		  R_Hex = (int)(R_Channel[i] * 4 + 0.5);
		  Gr_Hex = (int)(Gr_Channel[i] * 4 + 0.5);
		  Gb_Hex = (int)(Gb_channel[i] * 4 + 0.5);
		  B_Hex = (int)(B_Channel[i] * 4 + 0.5);
		  OTPData[j]=R_Hex&0xff;
		  OTPData[j+1]=(R_Hex>>8)&0xff;
		  OTPData[j+442]=Gr_Hex&0xff;
		  OTPData[j+442+1]=(Gr_Hex>>8)&0xff;
		  OTPData[j+442*2]=Gb_Hex&0xff;
		  OTPData[j+442*2+1]=(Gb_Hex>>8)&0xff;
		  OTPData[j+442*3]=B_Hex&0xff;
		  OTPData[j+442*3+1]=(B_Hex>>8)&0xff;
		  j=j+2;
	  }
	}

	if (!LSCDataCheck(OTPData))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("LSC烧录环境不良!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC烧录环境不良!"));
		return FALSE;
	}
	int k=pDlg->LSCerror;
	
	for(int i=0;i<(1768-k);i++)
	{  
	   int count=0;
	   for(int j=0;j<k;j++)
	   {
	    if((OTPData[i+j]==0xff)||(OTPData[i+j]==0))
		{
		   count++;
		}
	   }
	   if(count>=k)
	   {
			m_pInterface->Ctrl_Addlog(CamID,_T("LSC数据生成错误，多个连续数据为0!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("LSC数据生成错误，多个连续数据为0!"));
			SetResult(RESULT_FAIL);
			return 0;
	   }
	   
	}//lx
	{
		for (int i=0;i<1768;i++)
		{
			BurnData.push_back(OTPData[i]);
		}
		int siteNo=m_pInterface->GetSiteNo();
		FileName.Format(_T("QualcommLSC_%d.bin"), siteNo);
		m_pInterface->Otp_WriteToOtpBoard(CamID,FileName,BurnData,1);//写入的路径为【F:\OHP0792\Execute\Debug\otpdataManager】
	}
	return TRUE;
}
BOOL LscApplication_Moto_TestItem::Sensor_LSCCal()
{
	ImageInfo *MyImageInfo;
	MyImageInfo=m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == MyImageInfo->Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧4失败!"),COLOR_RED,200);
		return FALSE;
	}
	int width=MyImageInfo->imgwidth;
	int height=MyImageInfo->imgheight;

	LPBYTE MyRaw10Buff=new BYTE[width*height*2];
	if (NULL == MyRaw10Buff)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Sensor_LSCCal MyRaw10Buff buffer 申请失败!"),COLOR_RED,200);
		return FALSE;
	}
	memset(MyRaw10Buff,0,width*height*2);
	for (int i=0;i<width*height;i++)
	{
		MyRaw10Buff[i*2]=(MyImageInfo->Raw_buffer[i]&0xff);
		MyRaw10Buff[i*2+1]=(MyImageInfo->Raw_buffer[i]>>8);
	}
	unsigned char LenReg[512]={0}; 

	LenReg[0] = pDlg->m_GetDATForS5K4H7YX;
	
	int iresult=m_pInterface->Sensor_GetLsc(CamID,MyRaw10Buff,width,height,pDlg->Sensor_LSCTarget,pDlg->Sensor_OBvalue,LenReg,pDlg->nLenCRegCount,pDlg->Sensor_LSCGroup);
	SAFE_DELETE_ARRAY(MyRaw10Buff);
	return iresult;
}
/******************************************************************
函数名:    ReadBinData
函数功能:  Read Qualcomm LSC Data From txt
返回值：   返回 1 成功        返回0 失败
*******************************************************************/
BOOL LscApplication_Moto_TestItem::ReadDataFromTXT(float *R_Channel, float *Gr_Channel, float *Gb_channel, float *B_Channel)
{
	USES_CONVERSION;
	int mycharlength = 0;
	int strlength = 0;
	char mycharbuff[512] = {0};
	CStdioFile file;
	CString Path;
	Path =m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Qualcomm\\AWB_LSC_CALIBRATION_DATA.txt");
	int r_count =0;
	int gr_count = 0;
	int gb_count = 0;
	int b_count = 0;
	int channel_count = 0;
	int end_count =0;
	float tempnum;
	int i;
	int q = 0;
	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("}"),0) != -1)
			{
				end_count++;
				continue;
			}
			if (end_count >= 4)
			{
				break;
			}
			if ( strTemp.Find( _T("{"), 0 ) != -1 )
			{	
				channel_count++;
				if ( channel_count == 1 )
				{
					strTemp = strTemp.Mid(38,strTemp.GetLength()-38);
				}
				if ( (channel_count == 2) || ( channel_count == 3 ) )
				{
					strTemp = strTemp.Mid(14,strTemp.GetLength()-14);
				}
				if ( channel_count == 4 )
				{
					strTemp = strTemp.Mid(13,strTemp.GetLength()-13);
				}
				
				for (i = 0;i < 17; i++)
				{
					switch (channel_count)
					{
					case 1:
//						a = strTemp.Mid(38+i*11,9);	
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(T2A(a));
						mycharlength = 0;
						R_Channel[r_count++] = tempnum;
						break;
					case 2:
//						a = strTemp.Mid(14+i*11,9);
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(T2A(a));
						mycharlength = 0;
						Gr_Channel[gr_count++] = tempnum;
						break;
					case 3:
//						a = strTemp.Mid(14+i*11,9);
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(T2A(a));
						mycharlength = 0;
						Gb_channel[gb_count++] = tempnum;
						break;
					case 4:
//						a = strTemp.Mid(13+i*11,9);
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(T2A(a));
						mycharlength = 0;
						B_Channel[b_count++] = tempnum;
						break;
					}
				}
			}
			else
			{
				strTemp = strTemp.Mid(1,strTemp.GetLength()-1);
				for (i = 0;i < 17; i++)
				{
//					strTemp.TrimLeft(1);
					int k = 0;
//					a = strTemp.Mid(1+i*11,9);
					while ( (strTemp[k] != ' ')&&(k<13) )
					{
						mycharlength++;
						k++;
					}
					a = strTemp.Mid(0,mycharlength);
					strlength = strTemp.GetLength();
					if ( (strlength-mycharlength) != 1 )
					{
						strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
					}
					else
					{
						a = strTemp.Mid(0,strlength-1);
					}
					
					float tempnum = (float)atof(T2A(a));
					mycharlength = 0;
					switch (channel_count)
					{
					case 1:
						R_Channel[r_count++] = tempnum;
						break;
					case 2:
						Gr_Channel[gr_count++] = tempnum;
						break;
					case 3:
						Gb_channel[gb_count++] = tempnum;
						break;
					case 4:
						B_Channel[b_count++] = tempnum;
						break;
					}
				}
			}
		}
		file.Close();
	}
	return true;
}

BOOL LscApplication_Moto_TestItem::ReadBinData(USHORT *OTPData)
{
	HANDLE hFile;
	int Timeout=100;
	CString eeprom_slim;
	
    eeprom_slim=m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Qualcomm\\ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");
	
	while (Timeout!=0)
	{
		hFile = CreateFile(eeprom_slim,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			unsigned char BIN[2000] = {0};
			ReadFile(hFile, BIN, 2000, &nBytesRead, NULL );
			CloseHandle( hFile );

			if (nBytesRead != 1768)
			{
				CString ErrorMsg;
				ErrorMsg.Format(_T("eeprom文件大小不为%d Byte!"),1768);
				m_pInterface->Ctrl_Addlog(CamID,ErrorMsg,COLOR_RED,200);
				return FALSE;
			}
			for (int i = 0;i < 1768; i++)
			{
				OTPData[i] = BIN[i];
			}
			break;
		}
		Timeout--;
	}
	return TRUE;
}


void LscApplication_Moto_TestItem::SaveLSCData(USHORT *OTPData)
{
	USES_CONVERSION;

	if (NULL == OTPData)
	{
		return;
	}

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp = 0;

	/*_mkdir(T2A(path));
	path = path+_T("Data\\");
	_mkdir(T2A(path));*/
	CString strCurrentPath = _T("D:\\Data\\");
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += m_strTestStation;
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += _T("\\");
	CString FileName = _T("");
	FileName.Format(_T("LSC%d_4ChannelData_%04d%02d%02d.xls"), BLK_NUM_H * BLK_NUM_V, m_pInterface->GetSiteNo(), time.GetYear(), time.GetMonth(), time.GetDay());
	FileName = /*ReportPath*/strCurrentPath + FileName;

	m_pInterface->Ctrl_Addlog(CamID, FileName, COLOR_BLUE, 200);

	fp = freopen(CT2A(FileName),"at",stdout);
	if (fp == NULL)  
	{
		CString strLog;

		strLog.Format(_T("%s Error---%s open fail\r\n"), __FUNCTION__, FileName.GetBuffer(FileName.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		return;
	}

	RecordTime.Format(_T("%02d:%02d:%02d"),time.GetHour(), time.GetMinute(), time.GetSecond());

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("SN\tFUSE ID\tTest Time\t");
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("Rch_%d\t", i + 1);
		}
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("Grch_%d\t", i + 1);
		}
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("Gbch_%d\t", i + 1);
		}
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("Bch_%d\t", i + 1);
		}
		printf("Result\t");
	}

	printf("\n");
	printf(T2A(m_pInterface->Ctrl_GetCamCode()));//增加SN的保存 LYC 20180528
	printf("\t");
	printf(CT2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	for (i = 0; i < BLK_NUM_H * BLK_NUM_V * 4 * 2; i += 2)
	{
		/*
		*	刘月琴要求存入EXCEL里的数据的顺序:R1Gr1Gb1B1.....R221Gr221Gb221B221，而高通工具生成的文件ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt中的顺序
		*	本来就是R1Gr1Gb1B1.....R221Gr221Gb221B221. 只是每个数据的低字节在前，高字节在后。
		*	--Added by LiHai--20180503
		*/
		USHORT lsc = (OTPData[i + 1] << 8) + (OTPData[i]);
		printf("%d\t", lsc);
	}

	if (RESULT_PASS == GetResult())
	{
		printf("%s", "PASS");
	}
	else
	{
		printf("%s", "NG");
	}

	printf("\t");

	fclose(fp); 
}

void LscApplication_Moto_TestItem::SaveColorShadingData(float *R_Channel, float *Gr_Channel, float *Gb_channel, float *B_Channel)
{
	USES_CONVERSION;

	if ((NULL == R_Channel) || (NULL == Gr_Channel) || (NULL == Gb_channel) || (NULL == B_Channel))
	{
		return;
	}

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp = 0;

	/*_mkdir(T2A(path));
	path = path+_T("Data\\");
	_mkdir(T2A(path));*/
	CString strCurrentPath = _T("D:\\Data\\");
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += m_strTestStation;
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += _T("\\");
	CString FileName = _T("");
	FileName.Format(_T("Block%d_ColorShading_Data_Site_%d_%04d%02d%02d.xls"), BLK_NUM_H * BLK_NUM_V, m_pInterface->GetSiteNo(), time.GetYear(), time.GetMonth(), time.GetDay());
	FileName = /*ReportPath*/strCurrentPath + FileName;

	m_pInterface->Ctrl_Addlog(CamID, FileName, COLOR_BLUE, 200);

	fp = freopen(CT2A(FileName),"at",stdout);
	if (fp == NULL)  
	{
		CString strLog;

		strLog.Format(_T("SaveColorShadingData() Error---%s open fail\r\n"), FileName.GetBuffer(FileName.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		return;
	}

	RecordTime.Format(_T("%02d:%02d:%02d"),time.GetHour(), time.GetMinute(), time.GetSecond());

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("SN\tFUSE ID\tTest Time\t");
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("R/G_Blk%d\t", i + 1);
		}
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("B/G_Blk%d\t", i + 1);
		}
		printf("Result\t");
	}

	printf("\n\t");
	printf(CT2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	/** 高通工具里面每个通道值已经减去了BLC了--Added by LiHai--20180503 **/
	float RG[BLK_NUM_H * BLK_NUM_V] = {0};
	float BG[BLK_NUM_H * BLK_NUM_V] = {0};
	for (i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
	{
		RG[i] = R_Channel[i] / ((Gr_Channel[i] + Gb_channel[i]) / 2.0 + 0.5 );	// Rave/Gave
		BG[i] = B_Channel[i] / ((Gr_Channel[i] + Gb_channel[i]) / 2.0 + 0.5 );	// Bave/Gave 
	}

	float ColorShadingVal_RG[BLK_NUM_H * BLK_NUM_V] = {0};
	float ColorShadingVal_BG[BLK_NUM_H * BLK_NUM_V] = {0};
	float RGMax = RG[BLK_CENTER_INX];	//刘月琴发来的参考文档中，计算的就是除以中心Block的值，而不是除以最大值--Added by LiHai--20180503
	float BGMax = BG[BLK_CENTER_INX];
	for (i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
	{
		ColorShadingVal_RG[i] = RG[i] / RGMax;
		ColorShadingVal_BG[i] = BG[i] / BGMax;
	}

	for (i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
	{
		printf( "%f\t", ColorShadingVal_RG[i]);
	}
	for (i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
	{
		printf( "%f\t", ColorShadingVal_BG[i]);
	}

	if (RESULT_PASS == GetResult())
	{
		printf("%s", "PASS");
	}
	else
	{
		printf("%s", "NG");
	}

	printf("\t");

	fclose(fp); 
}