#pragma once


// MyTestPanel 对话框

class MyTestPanel : public CDialogEx
{
	DECLARE_DYNAMIC(MyTestPanel)

public:
	MyTestPanel(int CamID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyTestPanel();

// 对话框数据
	enum { IDD = IDD_TestPanel };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void EnableButton(BOOL bOn);	 
	void ResetAllButtonImage(void);
	int CamID;
};
