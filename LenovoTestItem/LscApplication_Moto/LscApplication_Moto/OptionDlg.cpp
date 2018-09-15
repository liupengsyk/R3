// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LscApplication_Moto.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "LscApplication_Moto_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, LscApplication_Moto_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_MTKSize(0)
    , m_MaxYDecay(0)
    , m_MinYDecay(0)
{
	m_pTestItem = pTestItem;
	m_SaveFlag = 0;
	m_BLC = 0;
	m_RMax = 0;
	m_RMin = 0;
	m_GrMax = 0;
	m_GrMin = 0;
	m_GbMax = 0;
	m_GbMin = 0;
	m_BMax = 0;
	m_BMin = 0;
	m_LSCDistance = 0;
	m_xblock = 0;
	m_yblock = 0;
	m_OBvalue = 0;
	m_bSavShadingDataPerBlock = FALSE;
	m_GrGbDiff = 0;
	m_YDecayDiff = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_QLSC);
	DDX_Check(pDX, IDC_CHECK2, m_MLSC);
	DDX_Check(pDX, IDC_CHECK3, m_SLSC);
	DDX_Text(pDX, IDC_SaveFlag, m_SaveFlag);
	DDX_Check(pDX, IDC_CHECK7, m_LSCMode);
	DDX_Text(pDX, IDC_BLC, m_BLC);
	DDX_Check(pDX, IDC_CHECK8, m_MLSC_M);
	DDX_Check(pDX, IDC_CHECK9, m_bSavShadingDataPerBlock);
	DDX_Text(pDX, IDC_Size, m_MTKSize);
	DDX_Text(pDX, IDC_EDIT1, m_RMax);
	DDX_Text(pDX, IDC_EDIT2, m_RMin);
	DDX_Text(pDX, IDC_EDIT3, m_GrMax);
	DDX_Text(pDX, IDC_EDIT4, m_GrMin);
	DDX_Text(pDX, IDC_EDIT5, m_GbMax);
	DDX_Text(pDX, IDC_EDIT6, m_GbMin);
	//  DDX_Control(pDX, IDC_EDIT7, m_BMax);
	DDX_Text(pDX, IDC_EDIT7, m_BMax);
	DDX_Text(pDX, IDC_EDIT8, m_BMin);
	DDX_Text(pDX, IDC_EDIT9, m_LSCDistance);
	DDX_Text(pDX, IDC_EDIT10, m_xblock);
	DDX_Text(pDX, IDC_EDIT11, m_yblock);
	DDX_Text(pDX, IDC_EDIT12, m_OBvalue);
	DDX_Text(pDX, IDC_LSCTARGET,Sensor_LSCTarget);
	DDX_Text(pDX, IDC_GROUP,Sensor_LSCGroup);
	DDX_Text(pDX, IDC_OBVALUE,Sensor_OBvalue);
	DDX_Text(pDX, IDC_LENCREGCOUNT,nLenCRegCount);

	DDX_Text(pDX, IDC_RGMAX, RGMAX);
	DDX_Text(pDX, IDC_RGMIN, RGMIN);
	DDX_Text(pDX, IDC_BGMAX, BGMAX);
	DDX_Text(pDX, IDC_BGMIN, BGMIN);

    DDX_Text(pDX, IDC_LSCERROR, LSCerror);
    DDX_Check(pDX, IDC_GetDATForS5K4H7YX, m_GetDATForS5K4H7YX);
    DDX_Text(pDX, IDC_MaxYDecay, m_MaxYDecay);
    DDX_Text(pDX, IDC_MinYDecay, m_MinYDecay);
	DDX_Text(pDX, IDC_EDIT_GrGbDiff, m_GrGbDiff);
	DDX_Text(pDX, IDC_YDecayDiff, m_YDecayDiff);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
END_MESSAGE_MAP()


// OptionDlg 消息处理程序
void OptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

//对话框窗口数据更新的响应函数
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE 控件的值—>变量    
	//FALSE 变量—>控件的值
	BOOL	bSaveAndValidate = (BOOL) wParam; 

	int ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{

	}
	else
	{

	}

	return ret;
}

//主程序按下ctrl+数字键的响应消息，通过此消息接受外界指令。
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

//响应模组关闭消息
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}