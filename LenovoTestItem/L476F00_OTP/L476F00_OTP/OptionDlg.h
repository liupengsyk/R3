#pragma once


// OptionDlg 对话框
#include "resource.h"

class L476F00_OTP_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, L476F00_OTP_TestItem* pTestItem);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	L476F00_OTP_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_EEPROMRevision;
	int m_HWVersion;
	int m_SWVersion;
	int m_ActuatorID;
	int m_LensID;
	int m_ManufactureID;
	int m_FactoryID;
	int m_ManufactureLine;
	int m_LPN1;
	int m_LPN2;
	int m_LPN3;
	int m_LPN4;
	int m_LPN5;
	int m_LPN6;
	int m_LPN7;
	int m_LPN8;

	int m_ModuleCode;
	int m_SupplierCode;
	int m_ModuleVersion;

	int m_DataStorageLocation;
	int m_SupplierID;
	int m_SensorID;
	int m_VCMID;
	int m_DriverICID;
	int	m_IRBGID;
	int	m_GyroID;
	int	m_ColorTemperatureID;
	int	m_OISDriverICID;
	int	m_FPC_PCBVersion;
	int	m_CalibrationVersion;
	int	m_OISfirmwareversion;
	int	m_Camerabuildingstage;
	int	m_Productionfactory;
	int	m_Productionline;



};
