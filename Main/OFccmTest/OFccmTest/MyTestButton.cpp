// MyTestButton.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyTestButton.h"
#include "OFccmDef.h"
#include "TestItemControl.h"


// MyTestButton

IMPLEMENT_DYNAMIC(MyTestButton, CMFCButton)

MyTestButton::MyTestButton(int CamID)
{
	this->CamID = CamID;
	/*if (pCcmCtrl->Cam[CamID].pTestCtrl->VecTestItem.size()>13)
	{
		MaxItemSize = pCcmCtrl->Cam[CamID].pTestCtrl->VecTestItem.size();
	}
	else*/
	{
		MaxItemSize = pCcmCtrl->Cam[CamID].pTestCtrl->TestItemCnt;
	}
	
}

MyTestButton::~MyTestButton()
{
}


BEGIN_MESSAGE_MAP(MyTestButton, CMFCButton)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_MESSAGE(WM_INSPECTIONBUTTON_SHOWSTATUS,&MyTestButton::ShowStatus)
	ON_MESSAGE(WM_INSPECTIONBUTTON_ALIGNWNDPOS,&MyTestButton::AlignWndPos)
END_MESSAGE_MAP()



// MyTestButton 消息处理程序



void MyTestButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 
	pCcmCtrl->ExecuteItemTest(m_ID,CamID); 

	CMFCButton::OnLButtonUp(nFlags, point); 

}


void MyTestButton::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CMFCButton::PostNcDestroy();
}


int MyTestButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	Font3.CreateFont (20, // FONT SIZE
		5, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
		VARIABLE_PITCH|FF_SWISS, _T("Arial"));
	// TODO:  在此添加您专用的创建代码
	CString name;
	GetWindowText(name);

	int len = name.GetLength();
	if (len > 20)
		SetFont(&Font3); //小字体显示
	else if (len > 10)
		SetFont(&Font3);
	else
		SetFont(&Font3);

	m_bRightImage = FALSE;
	m_nAlignStyle = ALIGN_LEFT;
	 	
	SetImage(IDB_BLUE);
	return 0;
}

LRESULT MyTestButton::ShowStatus(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case TestItemControl::BUTTON_RUNNING:
		SetImage(IDC_Yellow);
		break;
	case TestItemControl::BUTTON_RESET:
		SetImage(IDB_BLUE);
		break;
	case TestItemControl::BUTTON_PASS:
		SetImage(IDB_Green);
		break;
	case TestItemControl::BUTTON_FAIL:
		SetImage(IDB_Red);
		break;
	case TestItemControl::BUTTON_NULL:
		SetImage(IDB_Green);
		break;
	}
	Invalidate(FALSE);

	return 0;
}

LRESULT MyTestButton::AlignWndPos(WPARAM wParam, LPARAM lParam)
{
	//得到BUTTON的显示区域
	MaxItemSize = pCcmCtrl->Cam[CamID].pTestCtrl->TestItemCnt+3;
	CRect bgRect;
	GetParent()->GetClientRect(&bgRect);
	
	CRect rect;
	int width  = bgRect.Width();
	int height = bgRect.Height()/MaxItemSize;
	if(height>30) height = 30;
	int Pitch = height + 1;
	int HeightLimit = bgRect.Height(); //到窗口上边的距离

	int ColMax = (HeightLimit) / Pitch;

	//if ( m_ID <= ColMax ) 
	{
		rect.top    =   (m_ID-1)* (Pitch) + 1 ; 
		rect.left   =   bgRect.left + 1;
		rect.bottom =   rect.top  + height;
		rect.right  =   rect.left + width;
	}
	/*else
	{
		rect.top    =   (m_ID-ColMax-1)*(Pitch) + 5 ;
		rect.left   =   bgRect.left + width + 5;
		rect.bottom =   rect.top  + height;
		rect.right  =   rect.left + width;
	}*/

	//if (bgRect.Height()<400)
	//{
	//	SetFont(&Font4);
	//	width  = 140;
	//	height = 30;
	//	int Pitch = height + 3;
	//	int HeightLimit = bgRect.Height() - 5; //到窗口上边的距离

	//	int ColMax = (HeightLimit) / Pitch;
	//	double Col = m_ID*1.0 / ColMax;

	//	if ( Col<=1 ) 
	//	{
	//		rect.top    =   (m_ID-1)* (Pitch) + 1 ; 
	//		rect.left   =   bgRect.left + 5;
	//	}
	//	else if(Col<=2)
	//	{
	//		rect.top    =   (m_ID-ColMax-1)*(Pitch) + 5 ;
	//		rect.left   =   bgRect.left + width + 5;
	//	}
	//	else
	//	{
	//		rect.top    =   (m_ID-2*ColMax-1)*(Pitch) + 5 ;
	//		rect.left   =   bgRect.left + 2*width + 5;
	//	}

	//	rect.bottom =   rect.top  + height;
	//	rect.right  =   rect.left + width;
	//}

	MoveWindow(rect);
	return 0;
}
