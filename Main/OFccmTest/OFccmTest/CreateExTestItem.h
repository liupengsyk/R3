#pragma once


// CreateExTestItem �Ի���

class CreateExTestItem : public CDialogEx
{
	DECLARE_DYNAMIC(CreateExTestItem)

public:
	CreateExTestItem(CString ExTestItemName,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CreateExTestItem();

// �Ի�������
	enum { IDD = IDD_CreateExTestItem };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString ExTestItemName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL bUsed;
};
