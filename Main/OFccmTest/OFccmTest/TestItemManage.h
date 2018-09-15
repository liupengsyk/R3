#pragma once
#include "afxcmn.h"
#include "MyDestListCtrl.h"
#include "MySrcListCtrl.h"
#include "CreateExTestItem.h"


// TestItemManage 对话框

class TestItemManage : public CDialogEx
{
	DECLARE_DYNAMIC(TestItemManage)

public:
	TestItemManage(int CamID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TestItemManage();

// 对话框数据
	enum { IDD = IDD_TestItemManage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSettestitem();
	afx_msg void OnBnClickedRemovetestitem();
	//CListCtrl srcList;
	//CListCtrl destList;
	afx_msg void OnBnClickedSelectedup();
	afx_msg void OnBnClickedSelecteddown();
	afx_msg void OnBnClickedSaveoption();

public:
	virtual BOOL OnInitDialog();
	MySrcListCtrl m_ListSrc;
	MyDestListCtrl m_ListDest;
 
	int LoadFileToSrcList(void);
	void InitListCtrl(void);
	CFont font; 
	void SaveDestList(void);
	int LoadConfigToDestList(void);
	afx_msg void OnNMClickListSrc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDest(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRefresh();

public:
	BOOL CheckSameName(CString name);
	int CamID;
	//CString GetFileInfo(LPCTSTR strFilePath,LPCTSTR strItem);
	int CopyToCurrentTestItem(CString m_fileName,CString DestName);
	CreateExTestItem* m_CreateExTestItem;
	CString dllDir;
	CString DestDir;
	int UpdateTestItem(vector<CString>* nameList);
private:
	BOOL bDoModal;
};
