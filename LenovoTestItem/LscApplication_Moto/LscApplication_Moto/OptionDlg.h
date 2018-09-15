#pragma once


// OptionDlg �Ի���
#include "resource.h"

class LscApplication_Moto_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, LscApplication_Moto_TestItem* pTestItem);   // ��׼���캯��
	virtual ~OptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	LscApplication_Moto_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_QLSC;//LSC���㹤���Ǹ�ͨ�Ĺ���
	BOOL m_MLSC;//LSC���㹤����MTK�Ĺ���
	BOOL m_SLSC;//LSC���㹤����Sensor�����ṩ�Ĺ���
	BOOL m_MLSC_M;//LSC���㹤����MTK_MONO�Ĺ���
	BOOL m_bSavShadingDataPerBlock;//Moto��Ŀ��Ҫ��������Shading����

	int m_SaveFlag;//LSC����ʹ�õ��ǡ�0:ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt���ļ��е����ݻ��ǡ�1:AWB_LSC_CALIBRATION_DATA.txt���ļ��е�����
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
	int LSCerror;//LSC���������������ɸ�0����0xFF, ����, ���������Ǽ�¼��������ɸ��������Ǽ���
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
