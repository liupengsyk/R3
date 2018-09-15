// PictureCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyPictureCtrl.h"


// MyPictureCtrl

IMPLEMENT_DYNAMIC(MyPictureCtrl, CStatic)

MyPictureCtrl::MyPictureCtrl(int CamID)
{
	 this->CamID = CamID;
}

MyPictureCtrl::~MyPictureCtrl()
{
}


BEGIN_MESSAGE_MAP(MyPictureCtrl, CStatic)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// MyPictureCtrl 消息处理程序




BOOL MyPictureCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pCcmCtrl->isOffline(CamID))
	{
		CRect rect;
		GetWindowRect(&rect);//判断是否在方框内
		if (rect.PtInRect(pt))
		{	   
			pCcmCtrl->Cam[CamID].pImgExplorer->OnMouseWheel(zDelta,pt);
		}

	}
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void MyPictureCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pCcmCtrl->isOffline(CamID))
	{
	   SetFocus();
	   pCcmCtrl->Cam[CamID].pImgExplorer->OnMouseMove(point);
	}

	CStatic::OnMouseMove(nFlags, point);
}


void MyPictureCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pCcmCtrl->isOffline(CamID))
	{
		pCcmCtrl->Cam[CamID].pImgExplorer->OnLButtonDown(point);
	}

	CStatic::OnLButtonDown(nFlags, point);
}


void MyPictureCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pCcmCtrl->isOffline(CamID))
	{
		pCcmCtrl->Cam[CamID].pImgExplorer->OnLButtonUp(point);
	}

	pCcmCtrl->OnLButtonUpPoint( CamID, point);


	CStatic::OnLButtonUp(nFlags, point);
}


void MyPictureCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	if (isCreateCtrl)
	{
		CDC* pDC = pCcmCtrl->Ctrl_GetDisplayWndMemDC(CamID);

		if (pCcmCtrl->isOffline(CamID))
		{
			if (!pCcmCtrl->isStandby(CamID))
			{
				pCcmCtrl->Cam[CamID].pImgExplorer->ShowScrImage();
			}
			//else
			//{
			//	//pCcmCtrl->Cam[CamID].pImgExplorer->ShowScrImage();
			//}
			
			return;
		}
		else if (pCcmCtrl->isStandby(CamID))
		{
			if (!pCcmCtrl->Cam[CamID].pTestCtrl->m_TestMode)
			{
				pCcmCtrl->ResetMemdcBackGround(CamID);
			}
			
		}
		pCcmCtrl->Ctrl_ShowMemDC(CamID);
	}
	
	CStatic::OnPaint();
}


void MyPictureCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码

}
