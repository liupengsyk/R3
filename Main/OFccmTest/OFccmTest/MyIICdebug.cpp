// MyIICdebug.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyIICdebug.h"
#include "afxdialogex.h"


// MyIICdebug �Ի���

IMPLEMENT_DYNAMIC(MyIICdebug, CDialogEx)

MyIICdebug::MyIICdebug(int CamID,CWnd* pParent /*=NULL*/)
	: CDialogEx(MyIICdebug::IDD, pParent)
{
	m_Address = _T("");
	m_Value = _T("00");
	m_Register = _T("00");
	m_StdI2C = FALSE;
	Type_Reg = 4;
	Type_Val = 4;
	Format_Val = _T("%04X");
	this->CamID = CamID;
	pDebugSensorTab = pCcmCtrl->Cam[CamID].pConfig->GetSensorTab(); 
 
	 
}

MyIICdebug::~MyIICdebug()
{

}

void MyIICdebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	 
	DDX_Text(pDX, IDC_ADDRESS, m_Address);
	DDV_MaxChars(pDX, m_Address, 2);
	DDX_Text(pDX, IDC_VALUE, m_Value);
	DDV_MaxChars(pDX, m_Value, Type_Val);
	DDX_Text(pDX, IDC_REGISTER, m_Register);
	DDV_MaxChars(pDX, m_Register, Type_Reg);
	DDX_Control(pDX, IDC_MODE_COMBO, m_I2CMode);
	DDX_Control(pDX, IDC_SLIDER_EXP_GAIN, m_ExpGainCtrl);
	DDX_Control(pDX, IDC_SLIDER_EXP_TIME, m_ExpTimeCtrl);
	  
}


BEGIN_MESSAGE_MAP(MyIICdebug, CDialogEx)

	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_EXP_TIME, &MyIICdebug::OnNMReleasedcaptureSliderExpTime)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_EXP_GAIN, &MyIICdebug::OnNMReleasedcaptureSliderExpGain)	 
	ON_CBN_SELCHANGE(IDC_MODE_COMBO, &MyIICdebug::OnCbnSelchangeModeCombo)
	ON_BN_CLICKED(IDOK, &MyIICdebug::OnBnClickedOk)
	ON_BN_CLICKED(IDC_READ, &MyIICdebug::OnBnClickedRead)	 
	ON_BN_CLICKED(IDC_WriteAFCode, &MyIICdebug::OnBnClickedWriteafcode)
	 
	ON_BN_CLICKED(IDC_btnSaveExp, &MyIICdebug::OnBnClickedbtnsaveexp)
	 
END_MESSAGE_MAP()


// MyIICdebug ��Ϣ�������

BOOL MyIICdebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	 
	InitFont();
	
	InitI2C();
	InitExposureDebug();
	InitColorDebug();
	OnCbnSelchangeModeCombo(); //����
	ShowCurrentY();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

 

void MyIICdebug::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( !UpdateData(TRUE) )  return; //��������

	USHORT Address =0,Register =0,Value =0;

	//���ַ���ת��Ϊ����
	swscanf_s(m_Address,_T("%hX"),&Address);
	swscanf_s(m_Register,_T("%hX"),&Register);
	swscanf_s(m_Value,_T("%hX"),&Value);


	//�õ���ǰI2C MODE
	int Mode = m_I2CMode.GetCurSel();


 
	BOOL bRet=-1;

	bRet = pCcmCtrl->Device_WriteI2c((UCHAR)Address,Register,Value,(BYTE)Mode,CamID);


	if( !bRet )
	{
		AfxMessageBox(_T("Write I2C err!"));
		 
	}
	//CDialogEx::OnOK();
}


void MyIICdebug::OnBnClickedRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_VALUE)->SetWindowText(_T("0"));
	if ( !UpdateData(TRUE) )  return; //��������

	USHORT Address =0,Register=0,Value=0;

	//���ַ���ת��Ϊ����
	swscanf_s(m_Address,_T("%hX"),&Address);
	swscanf_s(m_Register,_T("%hX"),&Register);
	//�õ���ǰI2C MODE
	BYTE Mode = m_I2CMode.GetCurSel();

	 
	BOOL bRet =-1;

	bRet =  pCcmCtrl->Device_ReadI2c((UCHAR)Address,Register,&Value,Mode,CamID);


	if( !bRet )
	{
		AfxMessageBox(_T("Read I2C err!"));
		 
	}

	m_Value.Format(Format_Val,Value);
	UpdateData(FALSE); //���½���
}
void MyIICdebug::OnCbnSelchangeModeCombo()
{
	int Mode = m_I2CMode.GetCurSel();
	switch(Mode)
	{
	case 0:
	case 1:
		Type_Reg = 2;
		Type_Val = 2;
		Format_Val = _T("%02X");
		break;
	case 2:
		Type_Reg = 2;
		Type_Val = 4;
		Format_Val = _T("%04X");
		break;
	case 3:		
		Type_Reg = 4;
		Type_Val = 2;
		Format_Val = _T("%02X");
		break;
	case 4:
		Type_Reg = 4;
		Type_Val = 4;
		Format_Val = _T("%04X");
		break;
	case 5:
		Type_Reg = 2;
		Type_Val = 2;
		Format_Val = _T("%02X");
		break;
	case 6:
		Type_Reg = 2;
		Type_Val = 4;
		Format_Val = _T("%04X");
		break;
	}
	UpdateData(FALSE); //��������
}

void MyIICdebug::InitFont(void)
{
	//�ͷ�
	font_Edit.DeleteObject();
	font_Combox.DeleteObject();


	font_Combox.CreateFont(20,0,0,0,FW_BOLD,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("΢���ź�"));
	font_Edit.CreateFont(22,0,0,0,FW_BOLD,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("Arial"));

	GetDlgItem(IDC_ADDRESS)->SetFont(&font_Edit);
	GetDlgItem(IDC_REGISTER)->SetFont(&font_Edit);
	GetDlgItem(IDC_VALUE)->SetFont(&font_Edit);

}


HBRUSH MyIICdebug::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//������̬��ˢ��ˢ��static����
	static HBRUSH brush  = ::CreateSolidBrush(RGB(123,104,238));

	int ID = pWnd->GetDlgCtrlID();
	switch(ID)
	{
	case IDC_TITLE:
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkMode(TRANSPARENT);
			return brush;
		}

	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void MyIICdebug::InitExposureDebug(void)
{
	//time
	pCcmCtrl->Cam[CamID].pCameraCtrl->Init();
	int MaxValue = 0xFFFF;

	m_ExpTimeCtrl.SetRange(0, MaxValue);
	int exp;
	if (pCcmCtrl->Sensor_ReadExp(CamID,exp) == 1)
	{
		m_ExpTimeCtrl.SetPos(exp);
		CString str;
		str.Format(_T("%04X"),exp);
		GetDlgItem(IDC_EXP_TIME)->SetWindowText(str);
	}
	
	//gain
	int GainMaxValue = 0xFFFF;
	m_ExpGainCtrl.SetRange(0, GainMaxValue);
	USHORT gain;
	if (pCcmCtrl->Sensor_ReadGain(CamID,gain) == 1)
	{
		m_ExpGainCtrl.SetPos(gain);
		CString str;
		str.Format(_T("%04X"),gain);
		GetDlgItem(IDC_EXP_GAIN)->SetWindowText(str);
	}

	 
	ShowCurrentY();
}


void MyIICdebug::InitColorDebug(void)
{
	USHORT Value = 0;

	//R
	USHORT Mask = 0XFFFF;

	int effective_bit = 0;
	for(int i=0; i<32; i++)
	{
		if (Mask & (0x1<<i)) 	effective_bit++;
	}
	USHORT Max = (1<<effective_bit) - 1;
 
}


void MyIICdebug::InitI2C(void)
{
	//��ʼ��Combobox�ַ���
	m_I2CMode.AddString(_T("0: Reg_8 value_8"));
	m_I2CMode.AddString(_T("1: Reg_8 value_8 (stopen)"));
	m_I2CMode.AddString(_T("2: Reg_8 value_16"));
	m_I2CMode.AddString(_T("3: Reg_16 value_8"));
	m_I2CMode.AddString(_T("4: Reg_16 value_16"));
	m_I2CMode.AddString(_T("5: Addr_8 value_8(direct)"));
	m_I2CMode.AddString(_T("6: Addr_8 value_16(direct)"));

	//���ݵ�ǰ��sensorTab��ʼ��ѡ��

	m_I2CMode.SetCurSel(pDebugSensorTab->mode);

	//��ʼ��I2C��ַ
	CString str;
	str.Format(_T("%02X"),pDebugSensorTab->SlaveID);
	m_Address = str;
}


void MyIICdebug::OnNMReleasedcaptureSliderExpTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int exp = m_ExpTimeCtrl.GetPos();
	if (pCcmCtrl->Sensor_WriteExp(CamID,exp) == 1)
	{
		CString str;
		str.Format(_T("%04X"),exp);
		GetDlgItem(IDC_EXP_TIME)->SetWindowText(str);
	}

	pCcmCtrl->Cam[CamID].pConfig->SaveExp(exp);

	ShowCurrentY();
	*pResult = 0;
}

void MyIICdebug::ShowCurrentY()
{
	Sleep(500);
	int AveY = pCcmCtrl->GetExpCurrentY(CamID);
	CString str;
	str.Format(_T("%d"),AveY);
	GetDlgItem(IDC_AveY)->SetWindowText(str);
}

void MyIICdebug::OnNMReleasedcaptureSliderExpGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int gain = m_ExpGainCtrl.GetPos();
	if (pCcmCtrl->Sensor_WriteGain(CamID,gain) == 1)
	{
		CString str;
		str.Format(_T("%04X"),gain);
		GetDlgItem(IDC_EXP_GAIN)->SetWindowText(str);
	} 
	pCcmCtrl->Cam[CamID].pConfig->SaveAgain(gain);

	ShowCurrentY();
	*pResult = 0;

}

void MyIICdebug::OnBnClickedWriteafcode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strCode;
	int Code=0;	
	GetDlgItem(IDC_Code)->GetWindowText(strCode);
	Code  = _ttoi(strCode);
	pCcmCtrl->VcmDr_InitAF(CamID);
	Sleep(1000);
	pCcmCtrl->VcmDr_WriteAF_Code(Code,CamID);
}
 
void MyIICdebug::OnBnClickedbtnsaveexp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//time
	UpdateData(FALSE); 
	USHORT exp;
	CString str;
	
	GetDlgItem(IDC_EXP_TIME)->GetWindowText(str);
	str = _T("0x")+str;
	swscanf_s(str, _T("0x%hX"), &exp);	 
	m_ExpTimeCtrl.SetPos(exp);
	pCcmCtrl->Sensor_WriteExp(CamID,exp); 
	pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	pCcmCtrl->Ctrl_Addlog(CamID,exp,COLOR_BLUE,200);
	 
	
    USHORT gain;
	GetDlgItem(IDC_EXP_GAIN)->GetWindowText(str);
	str = _T("0x")+str;
	swscanf_s(str, _T("0x%hX"), &gain);	
	m_ExpGainCtrl.SetPos(gain); 
	pCcmCtrl->Sensor_WriteGain(CamID,gain); 
	pCcmCtrl->Ctrl_Addlog(CamID,gain,COLOR_BLUE,200);

	pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	pCcmCtrl->Cam[CamID].pConfig->SaveExp(exp);
	pCcmCtrl->Cam[CamID].pConfig->SaveAgain(gain);

	ShowCurrentY();
}
 