#pragma once


// MyErrorCode dialog

class MyErrorCode : public CPropertyPage
{
	DECLARE_DYNAMIC(MyErrorCode)

public:
	MyErrorCode(int CamID);
	virtual ~MyErrorCode();

	BOOL ErrorCode_en;
	CString strSocket[15];
	CString strTestID[15];
	int CamID;

	void SavePage();

// Dialog Data
	enum { IDD = IDD_MyErrorCode };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
