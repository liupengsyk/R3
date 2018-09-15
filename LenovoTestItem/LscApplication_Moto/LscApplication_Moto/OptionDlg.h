#pragma once


// OptionDlg 对话框
#include "resource.h"

class LscApplication_Moto_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, LscApplication_Moto_TestItem* pTestItem);   // 标准构造函数
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
	LscApplication_Moto_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_QLSC;//LSC计算工具是高通的工具
	BOOL m_MLSC;//LSC计算工具是MTK的工具
	BOOL m_SLSC;//LSC计算工具是Sensor工厂提供的工具
	BOOL m_MLSC_M;//LSC计算工具是MTK_MONO的工具
	BOOL m_bSavShadingDataPerBlock;//Moto项目需要保存各块的Shading数据

	int m_SaveFlag;//LSC数据使用的是【0:ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt】文件中的数据还是【1:AWB_LSC_CALIBRATION_DATA.txt】文件中的数据
	BOOL m_LSCMode;
	int m_BLC;
	int m_MTKSize;
	int m_RMax;
	int m_RMin;
	int m_GrMax;
	int m_GrMin;
	int m_GbMax;
	int m_GbMin;
//	CEdit m_BMax;
	int m_BMax;
	int m_BMin;
	int m_LSCDistance;
	int m_xblock;
	int m_yblock;
	int m_OBvalue;
	int Sensor_LSCTarget;
	int Sensor_LSCGroup;
	int Sensor_OBvalue;
	int nLenCRegCount;
	int LSCerror;//LSC数据中有连续若干个0或者0xFF, 错误, 本变量就是记录这个【若干个】到底是几个
	int RGMAX;
	int RGMIN;
	int BGMAX;
	int BGMIN;
	BOOL m_GetDATForS5K4H7YX;
    int m_MaxYDecay;
    int m_MinYDecay;
	int m_GrGbDiff;
	int m_YDecayDiff;
};
