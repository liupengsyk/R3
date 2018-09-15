#include "StdAfx.h"
#include "MyRichEdit.h"
#include "resource.h"
 
 

MyRichEdit::MyRichEdit(void)
{
 
}


MyRichEdit::~MyRichEdit(void)
{

}

BEGIN_MESSAGE_MAP(MyRichEdit, CRichEditCtrl)
	ON_MESSAGE(WM_ADDLOG, &MyRichEdit::Add)
	ON_MESSAGE(WM_CLEARLOG, &MyRichEdit::Clearlog)
	
END_MESSAGE_MAP()

static int linelengthReserve =0;
LRESULT MyRichEdit::Add(WPARAM wParam,LPARAM lparam)
{ 
	logTab* fontInfo = reinterpret_cast<logTab*> (wParam);
	SetSel(0,0); //��ֹ��ʽ������ѡ�У�����������

	DEFcf.yHeight = fontInfo->height;
	DEFcf.crTextColor = fontInfo->color;

	SetWordCharFormat(DEFcf);
	CTime time=CTime::GetCurrentTime();
	CString str ;
	str.Format(_T(" %d:%d"),time.GetMinute(),time.GetSecond());
	str = fontInfo->logstr+str + _T("\n") ;
	ReplaceSel(str);
	//SendMessage(WM_VSCROLL, SB_BOTTOM, 0);	
	//SendMessage(WM_VSCROLL, SB_TOP, 0);//	  SB_TOP
	//SetSel(-1,-1); 
	return 0;
 
}
 
LRESULT MyRichEdit::Clearlog(WPARAM wparam,LPARAM lparam)
{ 
	linelengthReserve =0;
    SetWindowText(_T(""));	 
	return 0;
}
 
void MyRichEdit::InitFont(void)
{
	//��ʼ��Ĭ������
	DEFcf.cbSize  = sizeof(CHARFORMAT);	
	DEFcf.yHeight = 12*12;		
	DEFcf.dwMask  = CFM_FACE|CFM_SIZE|CFM_BOLD|CFM_COLOR ;	
	DEFcf.dwEffects = CFE_BOLD;		
	DEFcf.crTextColor = RGB(0,0,0);
	lstrcpy(DEFcf.szFaceName, _T("΢���ź�")); 	
	SetWordCharFormat(DEFcf);
}

void MyRichEdit::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	InitFont();
	CRichEditCtrl::PreSubclassWindow();
}

void MyRichEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������

	long nStart, nEnd;
	CString strTemp;
	GetSel(nStart, nEnd);

	if(nStart == nEnd)		return;

	CMenu popMenu;
	if (!popMenu.LoadMenu(IDR_MENU1)) return; //����˵�

	CMenu *pPopup;
	pPopup = popMenu.GetSubMenu(0); //����Ӳ˵�ָ��

	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this); //��ʾ�����˵�����������Ϊ(����ڲ˵����|�����Ҽ���x��y��this)

}

void MyRichEdit::OnAdfCopy()
{
	// TODO: �ڴ���������������
	CString strTemp = GetSelText();
	TCHAR* pSrc = strTemp.GetBuffer();
	int size = strTemp.GetLength();

	//ͳ�� /n�ĸ���
	int Cnt = 0;
	for (int i = 0; i<size; i++)
	{
		if (pSrc[i] == 0x0D)			Cnt++;
	}

	//��/n תΪ /n/r
	int DestSize = size + Cnt + 1;
	TCHAR* pDest = new TCHAR[DestSize];
	pDest[DestSize-1] = 0;

	int k = 0;
	for (int i = 0; i<size; i++)
	{
		pDest[k] = pSrc[i];
		k++;
		if (pSrc[i] == 0x0D)
		{
			pDest[k] = 0x0A;
			k++;
		}
	}

	if(OpenClipboard())
	{ 
		HGLOBAL  clipbuffer; 
		TCHAR *buffer; 
		EmptyClipboard(); 
		clipbuffer = GlobalAlloc(GMEM_DDESHARE,   DestSize*sizeof(TCHAR) + 2); 
		buffer = (TCHAR *)GlobalLock(clipbuffer); 
		wcscpy_s(buffer, wcslen(pDest)+1, pDest);
		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_UNICODETEXT,clipbuffer); 
		CloseClipboard(); 
	}

	delete [] pDest;
}