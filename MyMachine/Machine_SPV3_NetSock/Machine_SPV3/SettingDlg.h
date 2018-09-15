#pragma once
#include "resource.h"
#include <vector>
#include "afxcmn.h"

using namespace std;

// SettingDlg �Ի���
class Machine_NCO_SPV3;
 
class SettingDlg : public CDialog
{
	DECLARE_DYNAMIC(SettingDlg)

public:
	SettingDlg(CWnd* pParent ,Machine_NCO_SPV3 *pMachine);   // ��׼���캯��
	virtual ~SettingDlg();

// �Ի�������
	enum { IDD = IDD_SettingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	Machine_NCO_SPV3 *pMachine;

public:  
	   
	// ����վ���б�
	CListCtrl StationList;
	afx_msg void OnBnClickedButtonAddStation();

	afx_msg void OnBnClickedButtonDeleteStation();

	int AddStationCheck(); 

	static int lastPos; 
	afx_msg void OnLvnItemchangedStationList(NMHDR *pNMHDR, LRESULT *pResult);

	void UI_Update();
	CString ipAddr;
	vector<CString> ipvec;
	// ʹ��error �쳣ָ�handle��Ҫȫ�̼�ز���Ӧ��ָ��
	BOOL error_en;
	// ����falseʱ����ִ�в�������
	
};
