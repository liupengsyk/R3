#pragma once


// OptionDlg 对话框
#include "resource.h"

typedef enum
{
	OC_MODE_BAYER,
	OC_MODE_MONO,

	OC_MODE_MAX
}_OC_MODE_;

typedef enum
{
	OC_BAYER_MODE_Y,
	OC_BAYER_MODE_SINGLE_CHANNEL,

	OC_BAYER_MODE_MAX
}_OC_BAYER_MODE_;

class OC_Item_Moto_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, OC_Item_Moto_TestItem* pTestItem);   // 标准构造函数
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
	OC_Item_Moto_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:

	BOOL m_Continue;
	BOOL m_OCSpec_Shift;
	/** Mono Sensor OC 配置参数 **/
	float m_OC_MonoSpec;

	/** Bayer Sensor OC 配置参数 **/
	float m_OC_BayerSpec_R;
	float m_OC_BayerSpec_Gr;
	float m_OC_BayerSpec_Gb;
	float m_OC_BayerSpec_B;

	/** Bayer Sensor OC 配置参数 **/
	float m_OC_BayerSpec_Y;

	float m_OCSpec_AfterShift;
	float m_PixelSize;
	BOOL m_isMonoSensor;	//0:Bayer, 1:Mono
	BOOL m_isBayerSingleChannel;	//0:Y通道, 1:单通道

	void OCModeSel(int OCMode);
	void OCBayerModeSel(int OCMode);

	void OCMonoModeEnable(BOOL bEnable);
	void OCBayerModeEnable(BOOL bEnable);
	void OCBayerModeSingleChannelEnable(BOOL bEnable);
	void OCBayerModeYChannelEnable(BOOL bEnable);
	afx_msg void OnBnClickedRadioBayer();
	afx_msg void OnBnClickedRadioMono();
	afx_msg void OnBnClickedRadioBayerY();
	afx_msg void OnBnClickedRadioChannel();
};
