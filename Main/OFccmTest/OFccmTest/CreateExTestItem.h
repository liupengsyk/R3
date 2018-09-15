#pragma once


// CreateExTestItem 对话框

class CreateExTestItem : public CDialogEx
{
	DECLARE_DYNAMIC(CreateExTestItem)

public:
	CreateExTestItem(CString ExTestItemName,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CreateExTestItem();

// 对话框数据
	enum { IDD = IDD_CreateExTestItem };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ExTestItemName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL bUsed;
};
