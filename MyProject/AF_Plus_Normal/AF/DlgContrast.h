#pragma once
#include "resource.h"

// CDlgContrast 对话框

class CDlgContrast : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgContrast)

public:
	CDlgContrast(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgContrast();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTRAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
