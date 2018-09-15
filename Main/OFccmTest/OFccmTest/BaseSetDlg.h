#pragma once
#include <vector>
using namespace std;
// BaseSetDlg 对话框

class BaseSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BaseSetDlg)

public:
	BaseSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BaseSetDlg();

// 对话框数据
	enum { IDD = IDD_BaseSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	vector<CString> vecSensorlist;
	void GetvecSensorlist();
	vector<CString> vecMachinelist;
	void GetvecMachinelist();

	vector<CString> vecCam;
	void GetvecCam();
	

	afx_msg void OnBnClickedbofflinetest();
	RawOptionDlg m_RawOptionDlg;
	afx_msg void OnCbnKillfocusStationname();

private:
	CString SensorTabPath;
public:
	afx_msg void OnBnClickedSaveto();
	afx_msg void OnBnClickedDeleteto();

private:
	void InitImgDevSelect();
	void InitCamTypeSelect();
	void InitCamNumSelect();
	void InitSensorSelect();
	void InitMachineSelect();
	void InitCamSelect();
public:
	BOOL showLog;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
