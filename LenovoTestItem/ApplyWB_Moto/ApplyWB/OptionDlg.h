#pragma once


// OptionDlg 对话框
#include "resource.h"

typedef enum
{
	MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC,
	MOTO_LIGHT_SRC_CAL_MODE_STATIC,

	MOTO_LIGHT_SRC_CAL_MODE_MAX
}_MOTO_LIGHT_SRC_CAL_MODE_;


class ApplyWB_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, ApplyWB_TestItem* pTestItem);   // 标准构造函数
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
	ApplyWB_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_RHigh;
	int m_RLow;
	int m_Multiple;
	/*int m_RGHigh;
	int m_RGLow;*/
	int m_GoldenB;
	int m_GoldenGb;
	int m_GoldenGr;
	int m_GoldenR;
	int m_GoldenRG;
	int m_GoldenBG;
	int m_GoldenGG;
	
	//int m_ROIWidth;
	int m_ROI_H;
	int m_ROI_V;
	
	int m_BHigh;
	int m_BLow;
	/*int m_BGHigh;
	int m_BGLow;*/
	int m_BLC;
	BOOL m_Raw8_EN;
	BOOL m_Raw10_EN;
	BOOL m_Channel_EN;//界面上[测试项设置]里面的[ApplyAWB]里的[单通道]复选框
	BOOL m_Gain_EN;	//界面上[测试项设置]里面的[ApplyAWB]里的[比值]复选框
	int m_GbHigh;
	int m_GbLow;
	int m_GGHigh;
	int m_GGLow;
	int m_GrHigh;
	int m_GrLow;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	int m_OTP_Distance;
	BOOL m_SaveImage;
	int m_BGIndex;
	int m_BIndex;
	int m_GbIndex;
	int m_GGIndex;
	int m_GrIndex;
	int m_RGIndex;
	int m_RIndex;
	int m_OTP_BeforeDistance;
	BOOL m_GG_GainEn;
	BOOL m_flow;	//是否需要进行四舍五入, 要的话会加一个0.5在取整
	int m_count;	//单色还是多色光源
	BOOL m_QTITool;	//表示是否使用高通工具计算AWB数据, TRUE:使用高通工具， FALSE:我们自己计算

	int m_Ev_HighCCT;
	int m_u_HighCCT;
	int m_v_HighCCT;
	float m_RGainHigh_diff; 
	float m_RGainlow_diff;
	float m_BGainHigh_diff;
	float m_BGainLow_diff;

	//Golden Range
	int R_Golden_Min;
	int R_Golden_Max;
	int Gr_Golden_Min;
	int Gr_Golden_Max;
	int Gb_Golden_Min;
	int Gb_Golden_Max;
	int B_Golden_Min;
	int B_Golden_Max;
	int RG_Golden_Min;
	int RG_Golden_Max;
	int BG_Golden_Min;
	int BG_Golden_Max;
	int GG_Golden_Min;
	int GG_Golden_Max;

	int ReCal_RG_diff;
	int ReCal_BG_diff;
	int ReCal_GG_diff;

	void MotoStaticLightSrcEnable(BOOL bEnable);
};
