// MyMaskedEdit.cpp : ʵ���ļ�
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



// MyMaskedEdit ��Ϣ�������



BOOL MyMaskedEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN) //
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN: //��Ӧ�س�
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
