#pragma once
#include "resource.h"
// AutoFocusOptionDlg 对话框
#include "ccmBaseInterface.h"

class Inspection_AutoFocus;

class AutoFocusOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(AutoFocusOptionDlg)

public:
	AutoFocusOptionDlg(CWnd* pParent, Inspection_AutoFocus* pInspection);   // 标准构造函数
	virtual ~AutoFocusOptionDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();

	int m_StartCode;
	int m_ROI_Width;
	int m_Resolution_Limit;

	int m_ROI_cx;
	int m_ROI_cy;
	CString str_Alg_Type;
	int m_ROI_Height;

	int m_FramePitchLimit_1;
	int m_FramePitchLimit_2;
	int m_FramePitchLimit_3;
	int m_FramePitchLimit_4;
	int m_SearchTime;
	int m_StepCode_1;
	int m_StepCode_2;
	int m_StepCode_3;
	int m_StepCode_4;
	double m_Diff_1;
	double m_Diff_2;
	double m_Diff_3;
	double m_Diff_4;
	int m_DescendTime_1;
	int m_DescendTime_2;
	int m_DescendTime_3;
	int m_DescendTime_4;

	Inspection_AutoFocus  *m_pInspection;
	int m_Code_lowerlimit;
	int m_Code_upperlimit;
	CString str_ResolutionResult_Alg;
protected:
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
public:
	BOOL m_PosPrompt;
	BOOL m_bAutoFocus;
	BOOL bGetTemperature;
	BOOL bBestCodeForOtp;
	BOOL m_bSaveData;
	CString m_burnItem;

	BOOL m_bGetTemperature2;
	CString m_tempOnOffReg;
	CString m_tempSlaveId;
	CString m_tempAddrReg;

public:
	int m_roiContourLenght;
	int m_roiWidth;
	int m_roiHeight;
	int m_roiFov;	 
	int m_roiSfrFreq;

	int m_ImgType;
	int m_bUseCruve;
	int m_CruveUseStep;
	int m_UsePoint;

	int m_ErrorCode;
	int  m_bTimesLog;
	int m_bUseDelayTime;
	int m_Resolution_Limit_Diff;
	int m_NGTimes;
};
