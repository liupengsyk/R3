// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RU.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "RU_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, RU_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;

	m_RuMaxSpec.RU_Max_R = 0.0;
	m_RuMaxSpec.RU_Max_Gr = 0.0;
	m_RuMaxSpec.RU_Max_Gb = 0.0;
	m_RuMaxSpec.RU_Max_B = 0.0;
	/*m_podCamera0Enable = FALSE;
	m_podCamera0LeftShieldLines = _T("5");
	m_podCamera0TopShieldLines = _T("5");
	m_podCamera0WLeftShieldLines = _T("5");
	m_podCamera0WTopShieldLines = _T("5");
	m_podCamera0RightShieldLines = _T("5");
	m_podCamera0BottomShieldLines = _T("5");
	m_podCamera0WRightShieldLines = _T("5");
	m_podCamera0WBottomShieldLines = _T("5");
	m_podCamera0AreaWidth = _T("9");
	m_podCamera0AreaHeight = _T("19");
	m_podCamera0SmallAreaWidth = _T("3");
	m_podCamera0SmallAreaHeight = _T("3");
	m_podCamera0DeadEnable = TRUE;
	m_podCamera0DeadUseUnit = TRUE;
	m_podCamera0WoundEnable = TRUE;
	m_podCamera0WoundUseUnit = TRUE;
	m_podCamera0UseLog = TRUE;
	m_podCamera0DeadType = _T("2");
	m_podCamera0DeadSpc = _T("50");
	m_podCamera0WoundSpcEdge = _T("30");
	m_podCamera0WoundSpecCenter = _T("30");

	m_podCamera1Enable = FALSE;
	m_podCamera1LeftShieldLines = _T("5");
	m_podCamera1TopShieldLines = _T("5");
	m_podCamera1WLeftShieldLines = _T("5");
	m_podCamera1WTopShieldLines = _T("5");
	m_podCamera1RightShieldLines = _T("5");
	m_podCamera1BottomShieldLines = _T("5");
	m_podCamera1WRightShieldLines = _T("5");
	m_podCamera1WBottomShieldLines = _T("5");
	m_podCamera1AreaWidth = _T("9");
	m_podCamera1AreaHeight = _T("19");
	m_podCamera1SmallAreaWidth = _T("3");
	m_podCamera1SmallAreaHeight = _T("3");
	m_podCamera1DeadEnable = TRUE;
	m_podCamera1DeadUseUnit = TRUE;
	m_podCamera1WoundEnable = TRUE;
	m_podCamera1WoundUseUnit = TRUE;
	m_podCamera1UseLog = TRUE;
	m_podCamera1DeadType = _T("2");
	m_podCamera1DeadSpc = _T("50");
	m_podCamera1WoundSpcEdge = _T("30");
	m_podCamera1WoundSpecCenter = _T("30");*/
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_RU_MAX_R, m_RuMaxSpec.RU_Max_R);
	DDX_Text(pDX, IDC_RU_MAX_GR, m_RuMaxSpec.RU_Max_Gr);
	DDX_Text(pDX, IDC_RU_MAX_GB, m_RuMaxSpec.RU_Max_Gb);
	DDX_Text(pDX, IDC_RU_MAX_B, m_RuMaxSpec.RU_Max_B);
	/*DDX_Check(pDX, IDC_CHECK_POD_CAMERA0_ENABLE, m_podCamera0Enable);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_LEFT_SHIELD_LINES, m_podCamera0LeftShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_TOP_SHIELD_LINES, m_podCamera0TopShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_WLEFT_SHIELD_LINES, m_podCamera0WLeftShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_WTOP_SHIELD_LINES, m_podCamera0WTopShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_RIGHT_SHIELD_LINES, m_podCamera0RightShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_BOTTOM_SHIELD_LINES, m_podCamera0BottomShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_WRIGHT_SHIELD_LINES, m_podCamera0WRightShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_WBOTTOM_SHIELD_LINES, m_podCamera0WBottomShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_AREA_WIDTH, m_podCamera0AreaWidth);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_AREA_HEIGHT, m_podCamera0AreaHeight);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_SMALL_AREA_WIDTH, m_podCamera0SmallAreaWidth);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_SMALL_AREA_HEIGHT, m_podCamera0SmallAreaHeight);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA0_DEAD_ENABLE, m_podCamera0DeadEnable);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA0_DEAD_USE_UNIT, m_podCamera0DeadUseUnit);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA0_WOUND_ENABLE, m_podCamera0WoundEnable);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA0_WOUND_USE_UNIT, m_podCamera0WoundUseUnit);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA0_USE_LOG, m_podCamera0UseLog);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_DEAD_TYPE, m_podCamera0DeadType);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_DEAD_SPC, m_podCamera0DeadSpc);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_WOUND_SPC_EDGE, m_podCamera0WoundSpcEdge);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA0_WOUND_SPEC_CENTER, m_podCamera0WoundSpecCenter);

	DDX_Check(pDX, IDC_CHECK_POD_CAMERA1_ENABLE, m_podCamera1Enable);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_LEFT_SHIELD_LINES, m_podCamera1LeftShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_TOP_SHIELD_LINES, m_podCamera1TopShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_WLEFT_SHIELD_LINES, m_podCamera1WLeftShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_WTOP_SHIELD_LINES, m_podCamera1WTopShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_RIGHT_SHIELD_LINES, m_podCamera1RightShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_BOTTOM_SHIELD_LINES, m_podCamera1BottomShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_WRIGHT_SHIELD_LINES, m_podCamera1WRightShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_WBOTTOM_SHIELD_LINES, m_podCamera1WBottomShieldLines);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_AREA_WIDTH, m_podCamera1AreaWidth);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_AREA_HEIGHT, m_podCamera1AreaHeight);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_SMALL_AREA_WIDTH, m_podCamera1SmallAreaWidth);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_SMALL_AREA_HEIGHT, m_podCamera1SmallAreaHeight);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA1_DEAD_ENABLE, m_podCamera1DeadEnable);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA1_DEAD_USE_UNIT, m_podCamera1DeadUseUnit);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA1_WOUND_ENABLE, m_podCamera1WoundEnable);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA1_WOUND_USE_UNIT, m_podCamera1WoundUseUnit);
	DDX_Check(pDX, IDC_CHECK_POD_CAMERA1_USE_LOG, m_podCamera1UseLog);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_DEAD_TYPE, m_podCamera1DeadType);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_DEAD_SPC, m_podCamera1DeadSpc);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_WOUND_SPC_EDGE, m_podCamera1WoundSpcEdge);
	DDX_Text(pDX, IDC_EDIT_POD_CAMERA1_WOUND_SPEC_CENTER, m_podCamera1WoundSpecCenter);*/
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