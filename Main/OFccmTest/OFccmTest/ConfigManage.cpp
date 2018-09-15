#include "StdAfx.h"
#include "ConfigManage.h"
#include "OFccmControl.h"

#ifdef WIN32 
#include <WTypes.h> 
#endif
#include "ImageProc.h" 
#include"imagehlp.h"
using namespace std;

ConfigManage::ConfigManage(void)
{ 
	m_pSensorPara = new USHORT [8192*4];   //(USHORT*) malloc(8192*sizeof(USHORT));
    pConfigFileRw = NULL;
	memset(m_pSensorPara, 0, 8192*4);
	 
	pMySensorTab = new SensorTab();
	VcmDriver = _T("");
	VCM_SlaveID=0x18;
	bIsLoad = FALSE;
	pConfigFileRw = new ConfigFileRw();
}
 
ConfigManage::~ConfigManage(void)
{
	SAFE_DELETE(pConfigFileRw);
	SAFE_DELETE(pMySensorTab);
	SAFE_DELETE(m_pSensorPara);
 
}

void ConfigManage::InitPath(int CamNum)
{
	bIsLoad = FALSE;

	m_ConfigFilePathName = OFccmControl::globalset.GetCamConfigPath(CamNum);
	pConfigFileRw->SetConfigFilePath(m_ConfigFilePathName);
	strCurrentPath = OFccmControl::globalset.GetCurrentPath();
}
void ConfigManage::SetConfigFilePath(CString m_ConfigFilePathName)
{
	this->m_ConfigFilePathName = m_ConfigFilePathName;
} 

CString ConfigManage::GetConfigFilePath()
{ 
	 return m_ConfigFilePathName;
}
 
SensorTab* ConfigManage::GetSensorTab()
{
	return pMySensorTab;
}

OSTab* ConfigManage::GetOSTab()
{
	return &m_OStab;
}

GeneralSettingTab* ConfigManage::GetGeneralTab()
{
	return &m_GeneralTab;
}
ErrorCodeTab* ConfigManage::GetErrorCodeTab()
{
	return &m_ErrorCodeTab;
}
// CurrentTab* ConfigManage::GetWorkingCurrentTab()
// {
// 	return &m_WorkingCurrentTab;
// }
CurrentTab* ConfigManage::GetStandbyCurrentTab()
{
	return &m_StandbyCurrentTab;
}
CString ConfigManage::GetVcmDr()
{
	return VcmDriver;
}
OtpSensorTab* ConfigManage::GetOtpSensorTab()
{
	return &m_OtpSensorTab;
}
 
vector<CString>* ConfigManage::GetDllNamelist()
{
	return &m_DllNamelist;
};

BOOL ConfigManage::LoadConfig(int CamID)
{
	InitPath(CamID);
	LoadSensorGenaralTab(); 

	LoadVcmDriver();
	LoadOtpSensor();
 
	LoadDllName();
	LoadOSTab();
	LoadGeneralSetting();
	LoadErrorCodeSetting();

	LoadSensorParalist();
//	LoadCurrentTab(m_WorkingCurrentTab, _T("WorkingCurrentTab"));
	LoadCurrentTab(m_StandbyCurrentTab, _T("StandbyCurrentTab")); 

	bIsLoad = TRUE;
	return 0;
}
BOOL ConfigManage::SaveConfig()
{
	 SaveSensorGenaralTab();
	 WriteVcmDriver(VcmDriver);
	 WriteOtpSensor();
	 
	 
	 WriteOSTab();
	 WriteDllName();
	 WriteGeneralSetting();
	 WriteErrorCodeSetting();
 //	 WriteCurrentTab( m_WorkingCurrentTab, _T("WorkingCurrentTab"));
	 WriteCurrentTab( m_StandbyCurrentTab, _T("StandbyCurrentTab"));

	
	 //SaveSensorParalist();
	 bIsLoad = FALSE;
	return 0;
}



BOOL ConfigManage::LoadSensorGenaralTab()
{
	pMySensorTab->name = pConfigFileRw->ReadConfigString(_T("Sensor"),_T("SensorName"),0); 
	pMySensorTab->width   = pConfigFileRw->ReadConfigData(_T("Sensor"),_T("width"),0);
	pMySensorTab->height  = pConfigFileRw->ReadConfigData(_T("Sensor"),_T("height"),0);
	pMySensorTab->type  = pConfigFileRw->ReadConfigData(_T("Sensor"),_T("type"),2);

	pMySensorTab->port = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("port"), 0);
	pMySensorTab->sensorpwdn = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("pwdn"), 0);
	pMySensorTab->sensorreset = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("reset"), 0);

	pMySensorTab->SlaveID  = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("SlaveID"), 0);
	pMySensorTab->mode     = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("mode"), 0);
	pMySensorTab->FlagReg  = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("FlagReg"), 0);
	pMySensorTab->FlagMask = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("FlagMask"), 0xff);
	pMySensorTab->FlagData = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("FlagData"), 0);

	pMySensorTab->FlagReg1  = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("FlagReg1"), 0);
	pMySensorTab->FlagMask1 = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("FlagMask1"), 0x0);
	pMySensorTab->FlagData1 = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("FlagData1"), 0);

	pMySensorTab->outformat= pConfigFileRw->ReadConfigData(_T("Sensor"), _T("outformat"), 0x00);
	pMySensorTab->mclk     = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("mclk"), 0x01);

// 	pMySensorTab->avdd     = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("avdd"), 0x00);
// 	pMySensorTab->dovdd   = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("dovdd"), 0x00);
// 	pMySensorTab->dvdd    = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("dvdd"), 0x00);
// 	pMySensorTab->af_vdd    = pConfigFileRw->ReadConfigData(_T("Sensor"), _T("afvdd"), 0x00);
// 	pMySensorTab->vpp = pConfigFileRw->ReadConfigData(_T("Sensor"),_T("vpp"),0x00);

	CString TempStr = L"";
	TempStr = pConfigFileRw->ReadConfigString(_T("Sensor"), _T("avdd"), _T("0.0"));
	pMySensorTab->avdd = atof(CT2A(TempStr));
	TempStr = pConfigFileRw->ReadConfigString(_T("Sensor"), _T("dovdd"), _T("0.0"));
	pMySensorTab->dovdd = atof(CT2A(TempStr));
	TempStr = pConfigFileRw->ReadConfigString(_T("Sensor"), _T("dvdd"), _T("0.0"));
	pMySensorTab->dvdd = atof(CT2A(TempStr));
	TempStr = pConfigFileRw->ReadConfigString(_T("Sensor"), _T("afvdd"), _T("0.0"));
	pMySensorTab->af_vdd = atof(CT2A(TempStr));
	TempStr = pConfigFileRw->ReadConfigString(_T("Sensor"), _T("vpp"), _T("0.0"));
	pMySensorTab->vpp = atof(CT2A(TempStr));
	return TRUE;
}
BOOL ConfigManage::SaveSensorGenaralTab()
{
	  pConfigFileRw->WriteConfigString(_T("Sensor"),_T("SensorName"),pMySensorTab->name); 
	  pConfigFileRw->WriteConfigData(_T("Sensor"),_T("width"),pMySensorTab->width);
	  pConfigFileRw->WriteConfigData(_T("Sensor"),_T("height"),pMySensorTab->height);
	  pConfigFileRw->WriteConfigData(_T("Sensor"),_T("type"),pMySensorTab->type);

	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("port"), pMySensorTab->port);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("pwdn"), pMySensorTab->sensorpwdn);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("reset"), pMySensorTab->sensorreset);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("mode"), pMySensorTab->mode);

	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("SlaveID"), pMySensorTab->SlaveID);
	
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("FlagReg"), pMySensorTab->FlagReg);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("FlagMask"), pMySensorTab->FlagMask);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("FlagData"), pMySensorTab->FlagData);

	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("FlagReg1"), pMySensorTab->FlagReg1);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("FlagMask1"), pMySensorTab->FlagMask1);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("FlagData1"), pMySensorTab->FlagData1);

	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("outformat"), pMySensorTab->outformat);
	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("mclk"), pMySensorTab->mclk);

// 	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("avdd"), pMySensorTab->avdd);
// 	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("dovdd"), pMySensorTab->dovdd );
// 	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("dvdd"), pMySensorTab->dvdd);
// 	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("afvdd"), pMySensorTab->af_vdd);
// 	  pConfigFileRw->WriteConfigData(_T("Sensor"), _T("vpp"),pMySensorTab->vpp);

	  CString TempStr = L"";
	  TempStr.Format(L"%.2f", pMySensorTab->avdd);
	  pConfigFileRw->WriteConfigString(_T("Sensor"), _T("avdd"), TempStr);
	  TempStr.Format(L"%.2f", pMySensorTab->dovdd);
	  pConfigFileRw->WriteConfigString(_T("Sensor"), _T("dovdd"), TempStr);
	  TempStr.Format(L"%.2f", pMySensorTab->dvdd);
	  pConfigFileRw->WriteConfigString(_T("Sensor"), _T("dvdd"), TempStr);
	  TempStr.Format(L"%.2f", pMySensorTab->af_vdd);
	  pConfigFileRw->WriteConfigString(_T("Sensor"), _T("afvdd"), TempStr);
	  TempStr.Format(L"%.2f", pMySensorTab->vpp);
	  pConfigFileRw->WriteConfigString(_T("Sensor"), _T("vpp"), TempStr);
	return 0;
}

 

BOOL ConfigManage::LoadSensorParalist()
{
	CStdioFile file;
	if (!file.Open(m_ConfigFilePathName, CFile::modeRead | CFile::typeText))
	{
		return FALSE;
	}
	CString szLine = _T("");
	int addr =0, reg=0, value=0;
	BYTE i2cmode = 0;
	USHORT *pParaList= m_pSensorPara;
 

	USHORT ParaListSize=0;
	  
	CString sReg, sVal;
	CString strTmp[15];
	int tmp = 0;
	strTmp[0] = "[ParaList]";
	 
	for(int i = 0; i <1; i++)
	{
		strTmp[i].MakeLower();
		strTmp[i].TrimLeft();
		strTmp[i].TrimRight();	
	}
	int state = -1;
	while(file.ReadString(szLine))
	{
		CString Textout;
		//寻找注释符号或者']',如果有这样的，只取前面的，
		tmp = szLine.FindOneOf(_T("//")); 
		if( tmp == 0)
		{
			continue;
		}
		else if(tmp > 0)
		{
			szLine = szLine.Left(tmp);
		}
		tmp = szLine.FindOneOf(_T("]")); 
		if( tmp == 0)
		{
			continue;
		}
		else if(tmp > 0)
		{
			szLine = szLine.Left(tmp+1);
		}
		szLine.MakeLower();
		szLine.TrimLeft();
		szLine.TrimRight();		

		if(szLine == strTmp[0]) 
		{
			state = 0;
			ParaListSize = 0;
			continue;
		}
		 

		if(szLine.IsEmpty())
			continue;
		if(szLine.Left(1) == _T(","))
			continue;
		if(szLine.Left(1) == _T(";"))
			continue;
		if(szLine.Left(1) == _T("/"))
			continue;

		if(szLine.Left(1) == _T("["))
		{
			state = -1;
			continue;
		}


		AfxExtractSubString(sReg, szLine, 0, ',');
		AfxExtractSubString(sVal, szLine, 1, ',');
		sReg.TrimLeft();   
		sReg.TrimRight();
		sVal.TrimRight();  
		sVal.TrimLeft();

		if(!swscanf_s(sReg, _T("0x%hX"), &reg)) //读取键值对数据	
			swscanf_s(sReg, _T("%d"), &reg);

		if(!swscanf_s(sVal, _T("0x%hX"), &value)) //读取键值对数据	
			swscanf_s(sVal, _T("%d"), &value);

		if(state == 0)
		{
			*(pParaList+ParaListSize) = reg;
			*(pParaList+ParaListSize+1) = value;
			ParaListSize += 2;
		}
		 

	}
	file.Close();

	if(ParaListSize)
	{
		pMySensorTab->ParaListSize = ParaListSize ;// + 2; //datasize...
		pMySensorTab->ParaList = m_pSensorPara;
		 
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
//BOOL ConfigManage::SaveSensorParalist(LPCTSTR AppName,USHORT* paralist,USHORT paralistsize)
//{
//	//CString strPataList = _T("");
//	int  LineNum= paralistsize/2;//行数
//	WritePrivateProfileSection(AppName,0, m_ConfigFilePathName);
//	for (int i=LineNum;i>LineNum-5; i-=1)
//	{
//		CString strLinePara = _T("");
//		strLinePara.TrimRight();
//		strLinePara.TrimLeft();
//		strLinePara.Format(_T("0x%x,0x%x,"),paralist[2*i-2],paralist[2*i-1]);
//		  
//		WritePrivateProfileSectionW(AppName,(LPCWSTR)strLinePara, m_ConfigFilePathName);
//		WritePrivateProfileString(AppName,strLinePara,0, m_ConfigFilePathName);
//	}
//	 
//	return 0;
//}


int ConfigManage::LoadDllName()
{
	//加载DLL
	m_DllNamelist.clear();
	CString prefix = _T("Item_");
	CString num, KeyName;
	CString dllname;
	for(int i = 0; i < 40; i++)
	{
		//得到要加载的dll名字
		num.Format(_T("%d"), i);
		KeyName = prefix + num;
		dllname  = pConfigFileRw->ReadConfigString(_T("TestItem_DLL"), KeyName,  _T(""));

		//验证文件后缀是不是dll
		CString ext = dllname.Right(dllname.GetLength() - dllname.ReverseFind('.') - 1);
		if(ext == _T("dll"))
			m_DllNamelist.push_back(dllname);
	}
	return 0;

}
int ConfigManage::WriteDllName()
{
	//清空
	  pConfigFileRw->ClearConfig(_T("TestItem_DLL"));

	//2 将列表的DLL清单写入CONFIG文件
	CString prefix = _T("Item_");
	CString num,keyname;
	for (UINT i=0; i < m_DllNamelist.size(); i++)
	{
		num.Format(_T("%d"), i);
		keyname = prefix + num;
		CString dllname = m_DllNamelist.at(i);
		pConfigFileRw->WriteConfigString(_T("TestItem_DLL"),keyname,dllname);
	}
	bIsLoad = FALSE;
	return 0;
}

BOOL ConfigManage::LoadVcmDriver()
{ 
	VcmDriver  = pConfigFileRw->ReadConfigString(_T("VCM_Driver"), _T("VCM_Driver"),  _T("")); 
	VCM_SlaveID  = pConfigFileRw->ReadConfigData(_T("VCM_Driver"), _T("VCM_SlaveID"),0x18); 
	return 0;
}
int ConfigManage::WriteVcmDriver(CString vcm_driver)
{
	//清空
	  pConfigFileRw->ClearConfig(_T("VCM_Driver"));

	//2 将列表的DLL清单写入CONFIG文件

	CString num,keyname;
	keyname = _T("VCM_Driver");
	  pConfigFileRw->WriteConfigString(_T("VCM_Driver"),keyname,vcm_driver);
	  pConfigFileRw->WriteConfigInt(_T("VCM_Driver"),_T("VCM_SlaveID"),VCM_SlaveID);

	bIsLoad = FALSE;
	return 0;
}

BOOL ConfigManage::LoadOtpSensor()
{
	m_OtpSensorTab.SensorName  = pConfigFileRw->ReadConfigString(_T("OtpSensor"), _T("OtpSensor"),  _T("")); 
	m_OtpSensorTab.SlaveID = pConfigFileRw->ReadConfigData(_T("OtpSensor"), _T("SlaveID"), 0); 
	return 0;
}
int ConfigManage::WriteOtpSensor()
{
	//清空
	  pConfigFileRw->ClearConfig(_T("OtpSensor"));

	//2 将列表的DLL清单写入CONFIG文件

	CString keyname;
	keyname = _T("OtpSensor");
	  pConfigFileRw->WriteConfigString(_T("OtpSensor"),keyname,m_OtpSensorTab.SensorName);
	keyname = _T("SlaveID");
	  pConfigFileRw->WriteConfigDataHex(_T("OtpSensor"),keyname,m_OtpSensorTab.SlaveID);

	bIsLoad = FALSE;
	return 0;
}
 

int ConfigManage::LoadOSTab()
{

	CString szRatio;
	m_OStab.OS_en = pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("OSen"), 0); //开短路负向测试开关
	m_OStab.PositiveOS_en = pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("PositiveOSen"), 0); //开短路正向测试开关
	m_OStab.SensorpinOS_en = pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("SensorpinOSen"), 0); //开短路两两pin测试开关
	//加载每个PIN的名字
	m_OStab.Pin[0].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_0_Name"), _T("DOVDD"));
	m_OStab.Pin[1].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_1_Name"), _T("DVDD"));
	m_OStab.Pin[2].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_2_Name"), _T("AVDD"));
	m_OStab.Pin[3].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_3_Name"), _T("VPP"));
	m_OStab.Pin[4].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_4_Name"), _T("AFVDD"));
	m_OStab.Pin[5].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_5_Name"), _T("GND1"));
	m_OStab.Pin[6].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_6_Name"), _T("GND2"));
	m_OStab.Pin[7].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_7_Name"), _T("GND3"));
	m_OStab.Pin[8].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_8_Name"), _T("GND4"));
	m_OStab.Pin[9].Name   = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_9_Name"), _T("CLK_P"));
	m_OStab.Pin[10].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_10_Name"), _T("CLK_N"));
	m_OStab.Pin[11].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_11_Name"), _T("D0P"));
	m_OStab.Pin[12].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_12_Name"), _T("D0N"));
	m_OStab.Pin[13].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_13_Name"), _T("D1P"));
	m_OStab.Pin[14].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_14_Name"), _T("D1N"));
	m_OStab.Pin[15].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_15_Name"), _T("D2P"));
	m_OStab.Pin[16].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_16_Name"), _T("D2N"));
	m_OStab.Pin[17].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_17_Name"), _T("D3P"));
	m_OStab.Pin[18].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_18_Name"), _T("D3N"));

	//PINMASK 
	if (pMySensorTab->port == 0) //串口
	{
		m_OStab.PinMask_H = (UINT  )pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("PinMask_H"), 0x01d0);
		m_OStab.PinMask_L = (UINT  )pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("PinMask_L"), 0x0007fff7);
		//加载每个PIN的名字
		m_OStab.Pin[19].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_19_Name"), _T("NC1"));
		m_OStab.Pin[20].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_20_Name"), _T("D0"));
		m_OStab.Pin[21].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_21_Name"), _T("D1"));
		m_OStab.Pin[22].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_22_Name"), _T("D2"));
		m_OStab.Pin[23].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_23_Name"), _T("D3"));
		m_OStab.Pin[24].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_24_Name"), _T("D4"));
		m_OStab.Pin[25].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_25_Name"), _T("D5"));
		m_OStab.Pin[26].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_26_Name"), _T("D6"));
		m_OStab.Pin[27].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_27_Name"), _T("D7"));
		m_OStab.Pin[28].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_28_Name"), _T("D8"));
		m_OStab.Pin[29].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_29_Name"), _T("D9"));
		m_OStab.Pin[30].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_30_Name"), _T("NC2"));
		m_OStab.Pin[31].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_31_Name"), _T("PCLK"));
		m_OStab.Pin[32].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_32_Name"), _T("HSYNC"));
		m_OStab.Pin[33].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_33_Name"), _T("VSYNC"));
		m_OStab.Pin[34].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_34_Name"), _T("NC3"));
		m_OStab.Pin[35].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_35_Name"), _T("NC4"));
		m_OStab.Pin[36].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_36_Name"), _T("MCLK"));
		m_OStab.Pin[37].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_37_Name"), _T("PWDN"));
		m_OStab.Pin[38].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_38_Name"), _T("RESET"));
		m_OStab.Pin[39].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_39_Name"), _T("SCL"));
		m_OStab.Pin[40].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_40_Name"), _T("SDA"));
		m_OStab.Pin[41].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_41_Name"), _T("GPIO1"));
		m_OStab.Pin[42].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_42_Name"), _T("GPIO2"));
		m_OStab.Pin[43].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_43_Name"), _T("GPIO3"));
		m_OStab.Pin[44].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_44_Name"), _T("GPIO4"));
	}
	else //并口或其他
	{
		m_OStab.PinMask_H = (UINT  )pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("PinMask_H"), 0x001f);
		m_OStab.PinMask_L = (UINT  )pConfigFileRw->ReadConfigData(_T("OSTAB"), _T("PinMask_L"), 0xffe800f3);

		m_OStab.Pin[19].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_19_Name"), _T("SDA"));
		m_OStab.Pin[20].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_20_Name"), _T("SCL"));
		m_OStab.Pin[21].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_21_Name"), _T("VSYNC"));
		m_OStab.Pin[22].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_22_Name"), _T("RESET"));
		m_OStab.Pin[23].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_23_Name"), _T("PWDN"));
		m_OStab.Pin[24].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_24_Name"), _T("HSYNC"));
		m_OStab.Pin[25].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_25_Name"), _T("MCLK"));
		m_OStab.Pin[26].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_26_Name"), _T("D9"));
		m_OStab.Pin[27].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_27_Name"), _T("D8"));
		m_OStab.Pin[28].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_28_Name"), _T("D7"));
		m_OStab.Pin[29].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_29_Name"), _T("D6"));
		m_OStab.Pin[30].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_30_Name"), _T("PCLK"));
		m_OStab.Pin[31].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_31_Name"), _T("D2"));
		m_OStab.Pin[32].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_32_Name"), _T("D5"));
		m_OStab.Pin[33].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_33_Name"), _T("D4"));
		m_OStab.Pin[34].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_34_Name"), _T("D3"));
		m_OStab.Pin[35].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_35_Name"), _T("D1"));
		m_OStab.Pin[36].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_36_Name"), _T("D0"));
		m_OStab.Pin[37].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_37_Name"), _T("NC1"));
		m_OStab.Pin[38].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_38_Name"), _T("NC2"));
		m_OStab.Pin[39].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_39_Name"), _T("NC3"));
		m_OStab.Pin[40].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_40_Name"), _T("NC4"));
		m_OStab.Pin[41].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_41_Name"), _T("GPIO1"));
		m_OStab.Pin[42].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_42_Name"), _T("GPIO2"));
		m_OStab.Pin[43].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_43_Name"), _T("NC5"));
		m_OStab.Pin[44].Name  = pConfigFileRw->ReadConfigString(_T("OSTAB"), _T("Pin_44_Name"), _T("NC6"));
	}


	CString prefix = _T("Pin_");
	CString num, KeyName;
	CString szOpenLimit, szShortLimit;
	for(int i = 0; i < DTPIN_MAX; i++)
	{
		num.Format(_T("%d"), i);
		KeyName = prefix + num + _T("_OpenLimit");
		szOpenLimit  = pConfigFileRw->ReadConfigString(_T("OSTAB"), KeyName,  _T("1.0"));
		m_OStab.Pin[i].OpenLimit = _ttof(szOpenLimit);

		KeyName = prefix + num + _T("_ShortLimit");
		szShortLimit  = pConfigFileRw->ReadConfigString(_T("OSTAB"), KeyName,  _T("0.2"));
		m_OStab.Pin[i].ShortLimit = _ttof(szShortLimit); 
	}

	return 0;
}
int ConfigManage::WriteOSTab()
{

	pConfigFileRw->WriteConfigInt(_T("OSTAB"), _T("OSen"), m_OStab.OS_en); //开短路测试开关
	pConfigFileRw->WriteConfigInt(_T("OSTAB"), _T("PositiveOSen"), m_OStab.PositiveOS_en); //开短路测试开关
	pConfigFileRw->WriteConfigInt(_T("OSTAB"), _T("SensorpinOSen"), m_OStab.SensorpinOS_en); //开短路测试开关
	pConfigFileRw->WriteConfigInt(_T("OSTAB"), _T("PinMask_L"), m_OStab.PinMask_L, DATA_HEX_DWORD); 
	pConfigFileRw->WriteConfigInt(_T("OSTAB"), _T("PinMask_H"), m_OStab.PinMask_H, DATA_HEX_DWORD); 

	CString prefix = _T("Pin_");
	CString num, KeyName, szLimit;
	for(int i = 0; i < DTPIN_MAX; i++)
	{
		num.Format(_T("%d"), i);
		KeyName = prefix + num + _T("_Name");
		  pConfigFileRw->WriteConfigString(_T("OSTAB"), KeyName, m_OStab.Pin[i].Name);

		KeyName = prefix + num + _T("_OpenLimit");
		szLimit.Format(_T("%.3lf"),m_OStab.Pin[i].OpenLimit);		
		  pConfigFileRw->WriteConfigString(_T("OSTAB"), KeyName, szLimit);

		KeyName = prefix + num + _T("_ShortLimit");
		szLimit.Format(_T("%.3lf"),m_OStab.Pin[i].ShortLimit);
		  pConfigFileRw->WriteConfigString(_T("OSTAB"), KeyName, szLimit);
	}

	return 0;
}


int ConfigManage::LoadGeneralSetting()
{
  	m_GeneralTab.SaveReport_EnLocal = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("SaveReport_EnLocal"), 0);
	 
 
	m_GeneralTab.ForcedShow_En = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("ForcedShow_En"), 0);

	m_GeneralTab.DevName = pConfigFileRw->ReadConfigString(_T("GENERAL_SETTING"), _T("DevName"), _T(""));
	  

	//创建报告路径
	DefaultReportPath = strCurrentPath + _T("Report");
	if (m_GeneralTab.SaveReport_EnLocal)
	{
		m_GeneralTab.sReportPath = DefaultReportPath;
	}
	else
	{
		m_GeneralTab.sReportPath  = pConfigFileRw->ReadConfigString(_T("GENERAL_SETTING"), _T("sReportPath"), DefaultReportPath);
    }
	

	m_GeneralTab.m_TestMode = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("m_TestMode"), 0);

	m_GeneralTab.SocketEn = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("SocketEn"),0);
	m_GeneralTab.RemoteIP  = pConfigFileRw->ReadConfigString(_T("GENERAL_SETTING"), _T("RemoteIP"), _T("127.0.0.1"));
	m_GeneralTab.iPort = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("iPort"), 5001);
	m_GeneralTab.bSaveDebugExp = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("bSaveDebugExp"), 0);
	m_GeneralTab.bUseLocalExp = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("bUseLocalExp"), 0);
	m_GeneralTab.bForceShow = pConfigFileRw->ReadConfigData(_T("GENERAL_SETTING"), _T("bForceShow"), 0);
 	 
	CFileFind FileFind;
	if(!FileFind.FindFile(m_GeneralTab.sReportPath)) //路径不存在就创建默认路径
	{
		//m_GeneralTab.sReportPath = DefaultReportPath;
		//CreateDirectory(m_GeneralTab.sReportPath, NULL);
		char sTmpReportPath[MAX_PATH];
		ImageProc::ConvertWideToChar(m_GeneralTab.sReportPath,sTmpReportPath );
		MakeSureDirectoryPathExists(sTmpReportPath);
		CreateDirectory(m_GeneralTab.sReportPath, NULL);
	}	 
	return 0;
}
int ConfigManage::WriteGeneralSetting(void)
{
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("SaveReport_EnLocal"), m_GeneralTab.SaveReport_EnLocal);
	 
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("ForcedShow_En"), m_GeneralTab.ForcedShow_En);

	pConfigFileRw->WriteConfigString(_T("GENERAL_SETTING"), _T("DevName"),m_GeneralTab.DevName);
	 
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("m_TestMode"), m_GeneralTab.m_TestMode);

	if (!m_GeneralTab.SaveReport_EnLocal)
	{
		pConfigFileRw->WriteConfigString(_T("GENERAL_SETTING"), _T("sReportPath"), m_GeneralTab.sReportPath);
	}
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("SocketEn"),m_GeneralTab.SocketEn);
	pConfigFileRw->WriteConfigString(_T("GENERAL_SETTING"), _T("RemoteIP"), m_GeneralTab.RemoteIP);
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("iPort"), m_GeneralTab.iPort);

	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("bSaveDebugExp"),m_GeneralTab.bSaveDebugExp);
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("bUseLocalExp"), m_GeneralTab.bUseLocalExp);
	pConfigFileRw->WriteConfigInt(_T("GENERAL_SETTING"), _T("bForceShow"), m_GeneralTab.bForceShow);
	 
	return 0;
}
int ConfigManage::WriteErrorCodeSetting()
{
	pConfigFileRw->WriteConfigInt(_T("ErrorCode"), _T("ErrorCode_en"), m_ErrorCodeTab.ErrorCode_en);
	for(int i=0;i<15;i++)
	{
		CString temp;
		temp.Format(_T("_%d"),i);
		pConfigFileRw->WriteConfigString(_T("ErrorCode"), _T("strSocket")+temp, m_ErrorCodeTab.strSocket[i]);
		pConfigFileRw->WriteConfigString(_T("ErrorCode"), _T("strTestID")+temp, m_ErrorCodeTab.strTestID[i]);
	}
	return 0;
}

int ConfigManage::LoadErrorCodeSetting()
{
	m_ErrorCodeTab.ErrorCode_en=pConfigFileRw->ReadConfigData(_T("ErrorCode"), _T("ErrorCode_en"), 0);
	for(int i=0;i<15;i++)
	{
		CString temp;
		temp.Format(_T("_%d"),i);
		m_ErrorCodeTab.strSocket[i]=pConfigFileRw->ReadConfigString(_T("ErrorCode"), _T("strSocket")+temp,_T(""));
		m_ErrorCodeTab.strTestID[i]=pConfigFileRw->ReadConfigString(_T("ErrorCode"), _T("strTestID")+temp,_T(""));
	}
	return 0;
}
 
int ConfigManage::LoadCurrentTab(CurrentTab &m_CurrentTab,CString strCurrentTab)
{
	CString szRatio;
	m_CurrentTab.Test_en = pConfigFileRw->ReadConfigData(strCurrentTab, _T("Test_en"), 0); //开短路测试开关
	m_CurrentTab.CurrentMask_H = (UINT  )pConfigFileRw->ReadConfigData(strCurrentTab, _T("CurrentMask_H"), 0x01d0);
	m_CurrentTab.CurrentMask_L = (UINT  )pConfigFileRw->ReadConfigData(strCurrentTab, _T("CurrentMask_L"), 0x0007fff7);
	//加载每个电源的名字
	m_CurrentTab.Pin[0].Name   = pConfigFileRw->ReadConfigString(strCurrentTab, _T("Current_0_Name"), _T("AVDD"));
	m_CurrentTab.Pin[1].Name   = pConfigFileRw->ReadConfigString(strCurrentTab, _T("Current_1_Name"), _T("DOVDD"));
	m_CurrentTab.Pin[2].Name   = pConfigFileRw->ReadConfigString(strCurrentTab, _T("Current_2_Name"), _T("DVDD"));
	m_CurrentTab.Pin[3].Name   = pConfigFileRw->ReadConfigString(strCurrentTab, _T("Current_3_Name"), _T("AFVDD"));
	m_CurrentTab.Pin[4].Name   = pConfigFileRw->ReadConfigString(strCurrentTab, _T("Current_4_Name"), _T("VPP"));
	
	m_CurrentTab.dbStandByPowerMax = _wtof(pConfigFileRw->ReadConfigString(strCurrentTab, _T("StandByPowerMax"), _T("0.0"))); //待机功率最大值
	m_CurrentTab.dbStandByPowerMin = _wtof(pConfigFileRw->ReadConfigString(strCurrentTab, _T("StandByPowerMin"), _T("0.0"))); //待机功率最小值
  

	CString prefix = _T("Current_");
	CString num, KeyName;
	CString szCurrentUpLimit, szCurrentLowLimit;
	for(int i = 0; i < DTVOL_MAX; i++)
	{
		num.Format(_T("%d"), i);
		KeyName = prefix + num + _T("_CurrentUpLimit");
		szCurrentUpLimit  = pConfigFileRw->ReadConfigString(strCurrentTab, KeyName,  _T("200"));
		m_CurrentTab.Pin[i].CurrentUpLimit = _ttof(szCurrentUpLimit);

		KeyName = prefix + num + _T("_CurrentLowLimit");
		szCurrentLowLimit  = pConfigFileRw->ReadConfigString(strCurrentTab, KeyName,  _T("0.1"));
		m_CurrentTab.Pin[i].CurrentLowLimit = _ttof(szCurrentLowLimit); 
	}

	return 0;
}
int ConfigManage::WriteCurrentTab(CurrentTab m_CurrentTab,CString strCurrentTab)
{
	pConfigFileRw->WriteConfigInt(strCurrentTab, _T("Test_en"), m_CurrentTab.Test_en); //开短路测试开关
	pConfigFileRw->WriteConfigInt(strCurrentTab, _T("CurrentMask_H"), m_CurrentTab.CurrentMask_H, DATA_HEX_DWORD); 
	pConfigFileRw->WriteConfigInt(strCurrentTab, _T("CurrentMask_L"), m_CurrentTab.CurrentMask_L, DATA_HEX_DWORD); 

	CString strPower;
	strPower.Format(_T("%.3lf"), m_CurrentTab.dbStandByPowerMax);
	pConfigFileRw->WriteConfigString(strCurrentTab, _T("StandByPowerMax"), strPower); 
	strPower.Format(_T("%.3lf"), m_CurrentTab.dbStandByPowerMin);
	pConfigFileRw->WriteConfigString(strCurrentTab, _T("StandByPowerMin"), strPower); 




	CString prefix = _T("Current_");
	CString num, KeyName, szLimit;
	for(int i = 0; i < DTVOL_MAX; i++)
	{
		num.Format(_T("%d"), i);
		KeyName = prefix + num + _T("_Name");
		  pConfigFileRw->WriteConfigString(strCurrentTab, KeyName, m_CurrentTab.Pin[i].Name);

		KeyName = prefix + num + _T("_CurrentUpLimit");
		szLimit.Format(_T("%.3lf"),m_CurrentTab.Pin[i].CurrentUpLimit);		
		  pConfigFileRw->WriteConfigString(strCurrentTab, KeyName, szLimit);

		KeyName = prefix + num + _T("_CurrentLowLimit");
		szLimit.Format(_T("%.3lf"),m_CurrentTab.Pin[i].CurrentLowLimit);
		  pConfigFileRw->WriteConfigString(strCurrentTab, KeyName, szLimit);
	}
	return 0;
}

 

int ConfigManage::GetLastExp()
{
	int exp;
	exp = pConfigFileRw->ReadConfigData(_T("LastExp"), _T("LastExp"), 100);
	return exp;
}
void ConfigManage::SaveExp(int exp)
{
	pConfigFileRw->WriteConfigInt(_T("LastExp"), _T("LastExp"), exp);
}

int ConfigManage::GetLastAgain()
{
	int Again;
	Again = pConfigFileRw->ReadConfigData(_T("LastAgain"), _T("LastAgain"), 100);
	return Again;
}
void ConfigManage::SaveAgain(int Again)
{
	pConfigFileRw->WriteConfigInt(_T("LastAgain"), _T("LastAgain"), Again);
}
	
