#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"

typedef struct
{
	int m_R;
	int m_Gr;
	int m_Gb;
	int m_B;

	int m_RGain;
	int m_BGain;
	int m_GGain;
}_DNP_COLOR_VAL_;

typedef enum
{
	MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC,
	MOTO_LIGHT_SRC_CAL_MODE_STATIC,

	MOTO_LIGHT_SRC_CAL_MODE_MAX
}_MOTO_LIGHT_SRC_CAL_MODE_;
#define MAXGODLENNUM    20
class GetDNPIndex_Moto_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, GetDNPIndex_Moto_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
	virtual ~OptionDlg();

// ¶Ô»°¿òÊý¾Ý
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	GetDNPIndex_Moto_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_CalDiff;	//两次光源校准之间的时间间隔--Added by LiHai--21071221
	CString FuseID[MAXGODLENNUM];
	CString m_SlaveID;
	CString R_StartAdd;
	CString Gr_StartAdd;
	CString Gb_StartAdd;
	CString B_StartAdd;
	CString RG_StartAdd;
	CString BG_StartAdd;
	CString GG_StartAdd;
	int R_min;
	int R_max;
	int Gr_min;
	int Gr_max;
	int Gb_min;
	int Gb_max;
	int B_min;
	int B_max;
	int RG_min;
	int RG_max;
	int BG_min;
	int BG_max;
	int GG_min;
	int GG_max;
	int R_index_min;
	int R_index_max;
	int Gr_index_min;
	int Gr_index_max;
	int Gb_index_min;
	int Gb_index_max;
	int B_index_min;
	int B_index_max;
	int RG_index_min;
	int RG_index_max;
	int BG_index_min;
	int BG_index_max;
	int GG_index_min;
	int GG_index_max;
	int index_error;
	CString ByteCount;
	CString A_Multi;
	CString B_Multi;
	CString Station;
	BOOL SingleChannelEnable;
	BOOL GainEnable;
	int m_ROI_H;//对应UI上的Center ROI，非高通工具时使用，表示N*N的切块，但是UI上只能设置一个值，所以切块是横竖相同的切块，即横切N块，竖也是切N块
	int m_ROI_V;
	int m_BLC;
	int m_Multiple;
	BOOL m_Raw8_EN;
	int page;
	
	int m_DNPDistance;
	BOOL m_GG_GainEn;
	BOOL m_flow;
	int m_count;//多色, 0：单色温，3：二色温，6：三色温，9：四色温
	BOOL m_QTITool;

	int m_ColorNum;
	int m_ModuleNum;//需要点检的模组总数
	CString m_ColorName;//对应UI上的[当前色温]，看变量名应该是色温名称，比如5100K等等

	/** 联想的动态光源点检数据 **/
	int	m_Ev_HighCCT_Min;
	int	m_Ev_HighCCT_Max;
	int	m_U_HighCCT_Max;
	int	m_U_HighCCT_Min;
	int	m_V_HighCCT_Max;
	int	m_V_HighCCT_Min;
	float m_Tolerance_HighCCT_Max;

	int m_U_Standard_HighCCT;
	int m_V_Standard_HighCCT;

	float m_GoldenDiff;
	float m_CurrentDiff;
	_DNP_COLOR_VAL_ m_TypVal;	//Typical Module Val

	int m_MotoLightSrcMode;	//摩托光源点检方式，0：动态点检，1：静态点检

	void MotoDynamicLightSrcEnable(BOOL bEnable);
	void MotoStaticLightSrcEnable(BOOL bEnable);
	void MotoLightSrcCfg(BOOL bDynamic);

	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
};
