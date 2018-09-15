// MyPropertyGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyPropertyGridCtrl.h"
#include <vector>
using namespace std;


// MyPropertyGridCtrl

IMPLEMENT_DYNAMIC(MyPropertyGridCtrl, CMFCPropertyGridCtrl)

MyPropertyGridCtrl::MyPropertyGridCtrl(int CamID)
{
	pMySensorTab =  NULL;
	m_OtpSensor =  NULL;
	 
	VCM_SlaveID=0x18;
	this->CamID = CamID;
}

MyPropertyGridCtrl::~MyPropertyGridCtrl()
{
	SAFE_DELETE(pMySensorTab);
	SAFE_DELETE(m_OtpSensor);
	 
}


BEGIN_MESSAGE_MAP(MyPropertyGridCtrl, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()
// MyPropertyGridCtrl 消息处理程序
void MyPropertyGridCtrl::InitHead()
{
	EnableHeaderCtrl(TRUE,_T("参数"),_T("值"));
	EnableDescriptionArea(TRUE);
	SetDescriptionRows(8);
	MarkModifiedProperties(TRUE);
}
 
void MyPropertyGridCtrl::Show()
{
	InitHead();
	AddSensorTab();
	AddVCMDriver();
	AddOtpSensor();	
	AddRolongoSetting();
	ExpandAll(FALSE);

}

void MyPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp)const
{
	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
	CString ParentName = pProp->GetParent()->GetName();
	if (ParentName == _T("SensorTab"))
	{
		OnSensorTabPropertyChanged(pProp);
	} 
	else if(ParentName == _T("VCM_DRIVER"))
	{
		OnVcmDriverPropertyChanged(pProp);
	}
	else if(ParentName == _T("OtpSensor"))
	{
		OnOtpSensorPropertyChanged(pProp);
	}
	else if(ParentName == _T("RolongoSetting"))
	{
		OnRolongoPropertyChanged(pProp);
	}
 
	  
}
 


void MyPropertyGridCtrl::AddSensorTab()
{	 
    pMySensorTab = pCcmCtrl->Cam[CamID].pConfig->GetSensorTab();

	CMFCPropertyGridProperty* pSensorGridProperty = new CMFCPropertyGridProperty(_T("SensorTab"));
	CMFCPropertyGridProperty* pProperty = NULL;
	CString strtemp = pMySensorTab->name;
	//strtemp.Format(_T("%s"),123); 

	pProperty = new CMFCPropertyGridProperty(_T("SensorName"),strtemp,_T("SensorName"),DATA_STRING); 
	vector<CString> sensorName = pCcmCtrl->GetvecImgSensor();;
	for(UINT i = 0;i<sensorName.size();i++)
	{
		pProperty->AddOption(sensorName[i]);
	}
	pSensorGridProperty->AddSubItem(pProperty);
	 
	
	CMFCPropertyGridProperty* pProperty_Width = AddMyProperty(_T("Width"),pMySensorTab->width,_T("set image width"),DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_Width);

	 
	CMFCPropertyGridProperty* pProperty_Height = AddMyProperty(_T("Height"),pMySensorTab->height,_T("set image height"),DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_Height);

	 
	CMFCPropertyGridProperty* pProperty_Type = AddMyProperty(_T("Type"),pMySensorTab->type,_T("Preview Type:\
		 \n 0:Raw 10 bit; \
	     \n 1:Raw 8 bit;\
		 \n 2:YUV422;\
		 \n 3:RGB565 \
		 \n 6:D_MIPI_RAW10"),DATA_DEC);
	pProperty_Type->AddOption(_T("0"));
	pProperty_Type->AddOption(_T("1"));
	pProperty_Type->AddOption(_T("2"));
	pProperty_Type->AddOption(_T("3"));
	pProperty_Type->AddOption(_T("6"));
	pProperty_Type->AllowEdit(FALSE);
	pSensorGridProperty->AddSubItem(pProperty_Type);


	 
	CMFCPropertyGridProperty* pProperty_Port = AddMyProperty(_T("Port"),pMySensorTab->port,_T("sensor port Type:\
															   \n 0:MIPI; \
															   \n 1:PARALLEL;") ,DATA_DEC);
															   
	pProperty_Port->AddOption(_T("0"));
	pProperty_Port->AddOption(_T("1"));
	pProperty_Port->AllowEdit(FALSE);
	pSensorGridProperty->AddSubItem(pProperty_Port);
	

	 
	CMFCPropertyGridProperty* pProperty_Pwdn = AddMyProperty(_T("Pwdn"),pMySensorTab->sensorpwdn,_T("sensor pwdn control:\
															   \n 0:low level; \
															   \n 1:high level;") ,DATA_DEC);

	pProperty_Pwdn->AddOption(_T("0"));
	pProperty_Pwdn->AddOption(_T("1")); 
	pProperty_Pwdn->AllowEdit(FALSE);
	pSensorGridProperty->AddSubItem(pProperty_Pwdn);
	 
	 
	CMFCPropertyGridProperty* pProperty_Reset = AddMyProperty(_T("Reset"),pMySensorTab->sensorreset,_T("sensor reset control :\
																		   \n 0:low level; \
																		   \n 1:high level;") ,DATA_DEC);

	pProperty_Reset->AddOption(_T("0"));
	pProperty_Reset->AddOption(_T("1"));
	pProperty_Reset->AllowEdit(FALSE);
	pSensorGridProperty->AddSubItem(pProperty_Reset);


 
	CMFCPropertyGridProperty* pProperty_SlaveID = AddMyProperty(_T("SlaveID"),pMySensorTab->SlaveID,_T("sensor slave id ") ,DATA_HEX_BYTE);
	pSensorGridProperty->AddSubItem(pProperty_SlaveID);

	 
	CMFCPropertyGridProperty* pProperty_iicMode = AddMyProperty(_T("I2C_mode"),pMySensorTab->mode,_T("sensor I2C Mode  :\
																	  \n 0:Normal 8Addr,8Data;; \
																	  \n 1:Samsung 8 Addr,8Data; \
																	  \n 2:Micron 8 Addr,16Data \
																	  \n 3:Stmicro 16Addr,8Data \
																	  \n 4:Micron2 16 Addr,16Data") ,DATA_DEC);
 	pProperty_iicMode->AddOption(_T("0"));
	pProperty_iicMode->AddOption(_T("1"));
	pProperty_iicMode->AddOption(_T("2"));
	pProperty_iicMode->AddOption(_T("3"));
	pProperty_iicMode->AddOption(_T("4"));
	pProperty_iicMode->AllowEdit(FALSE);
	pSensorGridProperty->AddSubItem(pProperty_iicMode);


	 
	CMFCPropertyGridProperty* pProperty_OutPut_Format = AddMyProperty(_T("OutPut_Format"),pMySensorTab->outformat,_T("sensor output format  :\
																   \n  0:YCbYCr/RG_GB; \
																   \n 1:YCrYCb/GR_BG; \
																   \n 2:CbYCrY/GB_RG;  \
																   \n 3:CrYCbY/BG_GR ") ,DATA_DEC);
	pProperty_OutPut_Format->AddOption(_T("0"));
	pProperty_OutPut_Format->AddOption(_T("1"));
	pProperty_OutPut_Format->AddOption(_T("2"));
	pProperty_OutPut_Format->AddOption(_T("3"));
	pProperty_OutPut_Format->AllowEdit(FALSE);
	pSensorGridProperty->AddSubItem(pProperty_OutPut_Format);
	 
	CMFCPropertyGridProperty* pProperty_Mclk = AddMyProperty(_T("Mclk"),pMySensorTab->mclk,_T("sensor mclk") ,DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_Mclk);
	 
	CMFCPropertyGridProperty* pProperty_AVDD  = AddMyProperty(_T("AVDD"), (pMySensorTab->avdd * 100),_T("sensor AVDD") ,DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_AVDD);
	 
	CMFCPropertyGridProperty* pProperty_DOVDD = AddMyProperty(_T("DOVDD"), (pMySensorTab->dovdd  * 100),_T("sensor DOVDD") ,DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_DOVDD);
	 
	CMFCPropertyGridProperty* pProperty_DVDD = AddMyProperty(_T("DVDD"), (pMySensorTab->dvdd * 100),_T("sensor DVDD") ,DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_DVDD);
	 
	CMFCPropertyGridProperty* pProperty_AFVDD = AddMyProperty(_T("AFVDD"), (pMySensorTab->af_vdd * 100),_T("sensor AFVDD") ,DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_AFVDD);
	 
	CMFCPropertyGridProperty* pProperty_VPP = AddMyProperty(_T("VPP"), (pMySensorTab->vpp * 100),_T("sensor VPP") ,DATA_DEC);
	pSensorGridProperty->AddSubItem(pProperty_VPP);

	 
	CMFCPropertyGridProperty* pProperty_FlagReg  = AddMyProperty(_T("FlagReg"),pMySensorTab->FlagReg,_T("sensor FlagReg") ,DATA_HEX_DWORD);
    pSensorGridProperty->AddSubItem(pProperty_FlagReg);
	 
	CMFCPropertyGridProperty* pProperty_FlagData  = AddMyProperty(_T("FlagData"),pMySensorTab->FlagData,_T("sensor FlagData") ,DATA_HEX_DWORD);
	pSensorGridProperty->AddSubItem(pProperty_FlagData); 
	 
	CMFCPropertyGridProperty* pProperty_FlagMask = AddMyProperty(_T("FlagMask"),pMySensorTab->FlagMask,_T("sensor FlagMask") ,DATA_HEX_DWORD);
	pSensorGridProperty->AddSubItem(pProperty_FlagMask);

	 
	CMFCPropertyGridProperty* pProperty_FlagReg1 = AddMyProperty(_T("FlagReg1"),pMySensorTab->FlagReg1,_T("sensor FlagReg1") ,DATA_HEX_DWORD);
	pSensorGridProperty->AddSubItem(pProperty_FlagReg1);

	 
	CMFCPropertyGridProperty* pProperty_FlagData1 = AddMyProperty(_T("FlagData1"),pMySensorTab->FlagData1,_T("sensor FlagData1") ,DATA_HEX_DWORD);
	pSensorGridProperty->AddSubItem(pProperty_FlagData1);

	 
	CMFCPropertyGridProperty* pProperty_FlagMask1 = AddMyProperty(_T("FlagMask1"),pMySensorTab->FlagMask1,_T("sensor FlagMask1") ,DATA_HEX_DWORD);
	pSensorGridProperty->AddSubItem(pProperty_FlagMask1);
	

	AddProperty(pSensorGridProperty);
 
	pProperty_Width = NULL;
	pProperty_Width = NULL;
	pProperty_Height = NULL;
	 
	
	pProperty_OutPut_Format = NULL;
	pProperty_Mclk = NULL;	 
	pProperty_Type = NULL;
	pProperty_Port = NULL;
	
	pProperty_Pwdn = NULL;
	pProperty_Reset = NULL;
	
	pProperty_SlaveID = NULL;
	pProperty_iicMode = NULL;
	

	 pProperty_AVDD = NULL;
	 pProperty_DOVDD = NULL;
	 pProperty_DVDD = NULL;
	 pProperty_AFVDD = NULL;
	 pProperty_VPP = NULL;

	 pProperty_FlagReg = NULL;
	 pProperty_FlagData = NULL;
	 pProperty_FlagMask = NULL;

	 pProperty_FlagReg1 = NULL;
	 pProperty_FlagData1 = NULL;
	 pProperty_FlagMask1 = NULL;

	 pSensorGridProperty = NULL;
 
}
void MyPropertyGridCtrl::OnSensorTabPropertyChanged(CMFCPropertyGridProperty* pProp)const
{
	COleVariant val,oriVal;
	CString strVal;
	CString strOriVal;
	CString strName;

	strName = pProp->GetName();
	val = pProp->GetValue();
	oriVal = pProp->GetOriginalValue();

	strVal = val.bstrVal;
	strOriVal = oriVal.bstrVal;

	if (strName == _T("SensorName"))
	{
		pMySensorTab->name = strVal;
		//wcscpy_s(pMySensorTab->name,strVal);//wcscpy_s(pMySensorTab->name,val.bstrVal);
	} 
	else if(strName == _T("Width"))
	{
		pMySensorTab->width = _ttoi(strVal);
	}
	else if(strName == _T("Height"))
	{
		pMySensorTab->height = _ttoi(strVal);
	}
	else if(strName == _T("Type"))
	{
		pMySensorTab->type = _ttoi(strVal);
	}
	else if(strName == _T("Port"))
	{
		pMySensorTab->port = _ttoi(strVal);
	}
	else if(strName == _T("Pwdn"))
	{
		pMySensorTab->sensorpwdn = _ttoi(strVal);
	}
	else if(strName == _T("Reset"))
	{
		pMySensorTab->sensorreset = _ttoi(strVal);
	}
	else if (strName == _T("SlaveID"))
	{
		int temp = 0;
		swscanf_s(strVal, _T("0x%X"), &temp);
		pMySensorTab->SlaveID = temp;
	}
	else if(strName == _T("I2C_mode"))
	{
		pMySensorTab->mode = _ttoi(strVal);
	}
	else if(strName == _T("OutPut_Format"))
	{
		pMySensorTab->outformat = _ttoi(strVal);
	}
	else if(strName == _T("Mclk"))
	{
		pMySensorTab->mclk = _ttoi(strVal);
	}
	else if(strName == _T("AVDD"))
	{
	//	pMySensorTab->avdd = _ttoi(strVal);
		pMySensorTab->avdd = atof(CT2A(strVal)) / 100.0;
	}
	else if(strName == _T("DOVDD"))
	{
	//	pMySensorTab->dovdd = _ttoi(strVal);
		pMySensorTab->dovdd = atof(CT2A(strVal)) / 100.0;
	}
	else if(strName == _T("DVDD"))
	{
	//	pMySensorTab->dvdd = _ttoi(strVal);
		pMySensorTab->dvdd = atof(CT2A(strVal)) / 100.0;
	}
	else if(strName == _T("AFVDD"))
	{
	//	pMySensorTab->af_vdd = _ttoi(strVal);
		pMySensorTab->af_vdd = atof(CT2A(strVal)) / 100.0;
	}
	else if(strName == _T("VPP"))
	{
	//	pMySensorTab->vpp = _ttoi(strVal);
		pMySensorTab->vpp = atof(CT2A(strVal)) / 100.0;
	}
	else if(strName == _T("FlagReg"))
	{
		swscanf_s(strVal, _T("0x%hX"), &pMySensorTab->FlagReg);
	}
	else if(strName == _T("FlagData"))
	{
		swscanf_s(strVal, _T("0x%hX"), &pMySensorTab->FlagData);
	}
	else if(strName == _T("FlagMask"))
	{
		swscanf_s(strVal, _T("0x%hX"), &pMySensorTab->FlagMask);
	}
	else if(strName == _T("FlagReg1"))
	{
		swscanf_s(strVal, _T("0x%hX"), &pMySensorTab->FlagReg1);
	}
	else if(strName == _T("FlagData1"))
	{
		swscanf_s(strVal, _T("0x%hX"), &pMySensorTab->FlagData1);
	}
	else if(strName == _T("FlagMask1"))
	{
		swscanf_s(strVal, _T("0x%hX"), &pMySensorTab->FlagMask1);
	}	 

}

void MyPropertyGridCtrl::AddVCMDriver()
{
	CMFCPropertyGridProperty* pDriverGridProperty = new CMFCPropertyGridProperty(_T("VCM_DRIVER"));
	CMFCPropertyGridProperty* pProperty = NULL;

	CString VCM_Driver;
	VCM_Driver = pCcmCtrl->Cam[CamID].pConfig->GetVcmDr();
	VCM_SlaveID=pCcmCtrl->Cam[CamID].pConfig->VCM_SlaveID;
	pProperty = new CMFCPropertyGridProperty(_T("Driver_Name"),VCM_Driver,_T("Driver_Name"),DATA_STRING);
	vector<CString> vecDriver = pCcmCtrl->GetvecDriver();
	for (UINT i=0;i<vecDriver.size();i++)
	{
		pProperty->AddOption(vecDriver[i]);
	}	
	pDriverGridProperty->AddSubItem(pProperty);
	CMFCPropertyGridProperty* pProperty_SlaveID = AddMyProperty(_T("SlaveID"),VCM_SlaveID,_T("SlaveID"),DATA_HEX_BYTE);	 
	pDriverGridProperty->AddSubItem(pProperty_SlaveID);

	AddProperty(pDriverGridProperty);
	 
	pDriverGridProperty = NULL;
	pProperty = NULL;
	pProperty_SlaveID=NULL;
}
void MyPropertyGridCtrl::OnVcmDriverPropertyChanged(CMFCPropertyGridProperty* pProp)const
{
	COleVariant val,oriVal;
	CString strVal,strOriVal,strName;

	strName = pProp->GetName();
	val = pProp->GetValue();
	oriVal = pProp->GetOriginalValue();

	strVal = val.bstrVal;
	strOriVal = oriVal.bstrVal;

	if (strName == _T("Driver_Name"))
	{
		pCcmCtrl->Cam[CamID].pConfig->VcmDriver =  strVal; //为什么不能返回本类中的cstring变量？ 
	}  
	if (strName == _T("SlaveID"))
	{
		swscanf_s(strVal,_T("0x%hx"),&(pCcmCtrl->Cam[CamID].pConfig->VCM_SlaveID));	
	}

}

void MyPropertyGridCtrl::AddOtpSensor()
{
	CMFCPropertyGridProperty* pOtpSensorGridProperty = new CMFCPropertyGridProperty(_T("OtpSensor"));
	CMFCPropertyGridProperty* pProperty = NULL;
	m_OtpSensor = (pCcmCtrl->Cam[CamID].pConfig->GetOtpSensorTab());
	pProperty = new CMFCPropertyGridProperty(_T("OtpSensor"),m_OtpSensor->SensorName,_T("OtpSensor"),DATA_STRING);
	vector<CString> vecOtpSensor = pCcmCtrl->GetvecOtpSensor();
	for (UINT i=0;i<vecOtpSensor.size();i++)
	{
		pProperty->AddOption(vecOtpSensor[i]);
	}	
	pOtpSensorGridProperty->AddSubItem(pProperty);

	 
	CMFCPropertyGridProperty* pProperty_SlaveID = AddMyProperty(_T("SlaveID"),m_OtpSensor->SlaveID,_T("SlaveID"),DATA_HEX_BYTE);	 
	pOtpSensorGridProperty->AddSubItem(pProperty_SlaveID);
	AddProperty(pOtpSensorGridProperty);

	pOtpSensorGridProperty = NULL;
	pProperty = NULL;
	pProperty_SlaveID = NULL;
}
void MyPropertyGridCtrl::OnOtpSensorPropertyChanged(CMFCPropertyGridProperty* pProp)const
{
	COleVariant val,oriVal;
	CString strVal,strOriVal,strName;

	strName = pProp->GetName();
	val = pProp->GetValue();
	oriVal = pProp->GetOriginalValue();

	strVal = val.bstrVal;
	strOriVal = oriVal.bstrVal;

	if (strName == _T("OtpSensor"))
	{
		m_OtpSensor->SensorName =  strVal; //为什么不能返回本类中的cstring变量？
		//wcscpy_s(strVal,_T("%s"),VCM_Driver);//wcscpy_s(pMySensorTab->name,val.bstrVal);
	}else if (strName == _T("SlaveID"))
	{
		swscanf_s(strVal,_T("0x%hx"),&(m_OtpSensor->SlaveID));		  
	} 
}
void MyPropertyGridCtrl::AddRolongoSetting()
{
	int iTemp=pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ReadConfigData(_T("RolongoPlatform_Set"),_T("HW_I2C_Speed"),400);
	CMFCPropertyGridProperty* pRolongoGridProperty = new CMFCPropertyGridProperty(_T("RolongoSetting"));
	CMFCPropertyGridProperty* pProperty1 = AddMyProperty(_T("HW_I2C_Speed"),iTemp,_T("HW_I2C_Speed"),DATA_DEC);
	pRolongoGridProperty->AddSubItem(pProperty1);
	iTemp=pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ReadConfigData(_T("RolongoPlatform_Set"),_T("HW_I2C_IntervalTime"),200);
	CMFCPropertyGridProperty* pProperty2 = AddMyProperty(_T("HW_I2C_IntervalTime"),iTemp,_T("HW_I2C_IntervalTime"),DATA_DEC);;
	pRolongoGridProperty->AddSubItem(pProperty2);
	CString strTemp;
	strTemp=pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ReadConfigString(_T("RolongoPlatform_Set"),_T("HW_I2C_CommProtocal"),_T("0//0:I2C"));
	CMFCPropertyGridProperty* pProperty3 = new CMFCPropertyGridProperty(_T("HW_I2C_CommProtocal"),strTemp,_T("HW_I2C_CommProtocal"),DATA_STRING);
	pProperty3->AddOption(_T("0//0:I2C"));
	pProperty3->AddOption(_T("1//1:ISP"));
	pRolongoGridProperty->AddSubItem(pProperty3);
	iTemp=pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ReadConfigData(_T("RolongoPlatform_Set"),_T("HW_I2C_Count"),1024);
	CMFCPropertyGridProperty* pProperty4 = AddMyProperty(_T("HW_I2C_Count"),iTemp,_T("HW_I2C_Count"),DATA_DEC);
	pRolongoGridProperty->AddSubItem(pProperty4);
	strTemp=pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ReadConfigString(_T("RolongoPlatform_Set"),_T("HW_PowerupSequence"),_T("0x10//0x10: SONY"));
	CMFCPropertyGridProperty *pProperty5 = new CMFCPropertyGridProperty(_T("HW_PowerupSequence"),strTemp,_T("HW_PowerupSequence"),DATA_STRING);
	pProperty5->AddOption(_T("0x00//0x00: OV"));
	pProperty5->AddOption(_T("0x10//0x10: SONY"));
	pProperty5->AddOption(_T("0x20//0x20:Samsung"));
    pProperty5->AddOption(_T("0x30//0x30: Hynix"));
	pProperty5->AddOption(_T("0x40//0x40:Aptina"));
	pProperty5->AddOption(_T("0x50//0x50:ST"));
    pProperty5->AddOption(_T("0x60//0x60: Toshiba"));
	pProperty5->AddOption(_T("0x70//0x70:Gcoreinc"));
	pProperty5->AddOption(_T("0x80//0x80:Superpix"));
	pRolongoGridProperty->AddSubItem(pProperty5);
	strTemp=pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ReadConfigString(_T("RolongoPlatform_Set"),_T("Image_MIPI_Lane"),_T("0x04//4: 4lane"));
	CMFCPropertyGridProperty *pProperty6 = new CMFCPropertyGridProperty(_T("Image_MIPI_Lane"),strTemp,_T("Image_MIPI_Lane"),DATA_STRING);
	pProperty6->AddOption(_T("0x00//0:Parallel"));
	pProperty6->AddOption(_T("0x01//1: 1lane"));
	pProperty6->AddOption(_T("0x02//2: 2lane"));
	pProperty6->AddOption(_T("0x04//4: 4lane"));
	pRolongoGridProperty->AddSubItem(pProperty6);

	AddProperty(pRolongoGridProperty);

	pRolongoGridProperty = NULL;
	pProperty1=NULL;
	pProperty2=NULL;
	pProperty3=NULL;
	pProperty4=NULL;
	pProperty5=NULL;
	pProperty6=NULL;
}
void MyPropertyGridCtrl::OnRolongoPropertyChanged(CMFCPropertyGridProperty* pProp)const
{
	COleVariant val,oriVal;
	CString strVal,strOriVal,strName;

	strName = pProp->GetName();
	val = pProp->GetValue();
	oriVal = pProp->GetOriginalValue();

	strVal = val.bstrVal;
	strOriVal = oriVal.bstrVal;
	pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	/*
	if (strName == _T("HW_I2C_Speed"))
	{
		pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	}else if (strName == _T("HW_I2C_IntervalTime"))
	{
		pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	}
	else if (strName == _T("HW_I2C_CommProtocal"))
	{
		pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	} 
	else if (strName == _T("HW_I2C_Count"))
	{
		pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	} 
	else if (strName == _T("HW_PowerupSequence"))
	{
		pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	} 
	else if (strName == _T("Image_MIPI_Lane"))
	{
		pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(_T("RolongoPlatform_Set"),strName,strVal);
	} */
}

void MyPropertyGridCtrl::Save()
{
	*(pCcmCtrl->Cam[CamID].pConfig->GetSensorTab()) = *pMySensorTab;
	  
	*(pCcmCtrl->Cam[CamID].pConfig->GetOtpSensorTab()) = *m_OtpSensor;	 
}

CMFCPropertyGridProperty* MyPropertyGridCtrl::AddMyProperty(LPCTSTR name, int data, LPCTSTR description, DWORD_PTR dataType)
{
	CString szTemp;
	switch(dataType)
	{
	case DATA_DEC:
		szTemp.Format(_T("%d"),data);
		return new CMFCPropertyGridProperty(name,szTemp,description,dataType,0,0,_T("0123456789"));
	case DATA_HEX_WORD:
		szTemp.Format(_T("0x%04X"),data);
		return new CMFCPropertyGridProperty(name,szTemp,description,dataType,_T("aaaaaa"),_T("0x____"),_T("0123456789ABCDEFabcdef"));
	case DATA_HEX_BYTE:
		szTemp.Format(_T("0x%02X"),data);
		return new CMFCPropertyGridProperty(name,szTemp,description,dataType,_T("aaaa"),_T("0x__"),_T("0123456789ABCDEFabcdef"));
	case DATA_HEX_DWORD:
		szTemp.Format(_T("0x%X"),data);
		return new CMFCPropertyGridProperty(name,szTemp,description,dataType,_T("aaaaaaaaaa"),_T("0x________"),_T("0123456789ABCDEFabcdef"));
	}

	return NULL;
}

CMFCPropertyGridProperty* MyPropertyGridCtrl::AddMyProperty(int name, int data, LPCTSTR description, DWORD_PTR dataType)
{
	CString szTemp,szName;
	switch(dataType)
	{
	case DATA_DEC:
		szName.Format(_T("%d"),name);
		szTemp.Format(_T("%d"),data);
		return new CMFCPropertyGridProperty(szName,szTemp,description,dataType,0,0,_T("0123456789"));
	case DATA_HEX_WORD:
		szTemp.Format(_T("0x%04X"),data);
		szName.Format(_T("0x%04X"),name);
		return new CMFCPropertyGridProperty(szName,szTemp,description,dataType,_T("aaaaaa"),_T("0x____"),_T("0123456789ABCDEFabcdef"));
	case DATA_HEX_BYTE:
		szTemp.Format(_T("0x%02X"),data);
		szName.Format(_T("0x%04X"),name);
		return new CMFCPropertyGridProperty(szName,szTemp,description,dataType,_T("aaaa"),_T("0x__"),_T("0123456789ABCDEFabcdef"));
	case DATA_HEX_DWORD:
		szTemp.Format(_T("0x%X"),data);
		szName.Format(_T("0x%04X"),name);
		return new CMFCPropertyGridProperty(szName,szTemp,description,dataType,_T("aaaaaaaaaa"),_T("0x________"),_T("0123456789ABCDEFabcdef"));
	}

	return NULL;
}