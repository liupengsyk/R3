// MyMaskedEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyMaskedEdit.h"


// MyMaskedEdit

IMPLEMENT_DYNAMIC(MyMaskedEdit, CMFCMaskedEdit)

MyMaskedEdit::MyMaskedEdit()
{

}

MyMaskedEdit::~MyMaskedEdit()
{
}


BEGIN_MESSAGE_MAP(MyMaskedEdit, CMFCMaskedEdit)
END_MESSAGE_MAP()



// MyMaskedEdit 消息处理程序



BOOL MyMaskedEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) //
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN: //响应回车
			{
				OnVK_Return();
				return TRUE;
			}
		}
	}
	return CMFCMaskedEdit::PreTranslateMessage(pMsg);
}

void MyMaskedEdit::OnVK_Return(void)
{
	::SendMessage(GetParent()->m_hWnd,WM_GETMASKEDEDIT_TEXT,0,0);
}
