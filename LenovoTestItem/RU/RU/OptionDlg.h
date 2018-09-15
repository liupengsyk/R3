#pragma once


// OptionDlg 对话框
#include "resource.h"

typedef struct 
{
	float RU_Max_R;
	float RU_Max_Gr;
	float RU_Max_Gb;
	float RU_Max_B;
}_RU_MAX_SPEC_;

class RU_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, RU_TestItem* pTestItem);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	RU_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	_RU_MAX_SPEC_ m_RuMaxSpec;

	/*BOOL m_podCamera0Enable;
	CString m_podCamera0LeftShieldLines;
	CString m_podCamera0TopShieldLines;
	CString m_podCamera0WLeftShieldLines;
	CString m_podCamera0WTopShieldLines;
	CString m_podCamera0RightShieldLines;
	CString m_podCamera0BottomShieldLines;
	CString m_podCamera0WRightShieldLines;
	CString m_podCamera0WBottomShieldLines;
	CString m_podCamera0AreaWidth;
	CString m_podCamera0AreaHeight;
	CString m_podCamera0SmallAreaWidth;
	CString m_podCamera0SmallAreaHeight;
	BOOL m_podCamera0DeadEnable;
	BOOL m_podCamera0DeadUseUnit;
	BOOL m_podCamera0WoundEnable;
	BOOL m_podCamera0WoundUseUnit;
	BOOL m_podCamera0UseLog;
	CString m_podCamera0DeadType;
	CString m_podCamera0DeadSpc;
	CString m_podCamera0WoundSpcEdge;
	CString m_podCamera0WoundSpecCenter;

	BOOL m_podCamera1Enable;
	CString m_podCamera1LeftShieldLines;
	CString m_podCamera1TopShieldLines;
	CString m_podCamera1WLeftShieldLines;
	CString m_podCamera1WTopShieldLines;
	CString m_podCamera1RightShieldLines;
	CString m_podCamera1BottomShieldLines;
	CString m_podCamera1WRightShieldLines;
	CString m_podCamera1WBottomShieldLines;
	CString m_podCamera1AreaWidth;
	CString m_podCamera1AreaHeight;
	CString m_podCamera1SmallAreaWidth;
	CString m_podCamera1SmallAreaHeight;
	BOOL m_podCamera1DeadEnable;
	BOOL m_podCamera1DeadUseUnit;
	BOOL m_podCamera1WoundEnable;
	BOOL m_podCamera1WoundUseUnit;
	BOOL m_podCamera1UseLog;
	CString m_podCamera1DeadType;
	CString m_podCamera1DeadSpc;
	CString m_podCamera1WoundSpcEdge;
	CString m_podCamera1WoundSpecCenter;*/
};
