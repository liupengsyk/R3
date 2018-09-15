#pragma once


// MyMaskedEdit

class MyMaskedEdit : public CMFCMaskedEdit
{
	DECLARE_DYNAMIC(MyMaskedEdit)

public:
	MyMaskedEdit();
	virtual ~MyMaskedEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnVK_Return(void);
};


