#pragma once


// MyTestPanel �Ի���

class MyTestPanel : public CDialogEx
{
	DECLARE_DYNAMIC(MyTestPanel)

public:
	MyTestPanel(int CamID,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyTestPanel();

// �Ի�������
	enum { IDD = IDD_TestPanel };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void EnableButton(BOOL bOn);	 
	void ResetAllButtonImage(void);
	int CamID;
};
