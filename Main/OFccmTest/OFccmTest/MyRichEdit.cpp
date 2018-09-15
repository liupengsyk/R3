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
	SetSel(0,0); //防止格式出错，先选中，再设置字体

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
	//初始化默认字体
	DEFcf.cbSize  = sizeof(CHARFORMAT);	
	DEFcf.yHeight = 12*12;		
	DEFcf.dwMask  = CFM_FACE|CFM_SIZE|CFM_BOLD|CFM_COLOR ;	
	DEFcf.dwEffects = CFE_BOLD;		
	DEFcf.crTextColor = RGB(0,0,0);
	lstrcpy(DEFcf.szFaceName, _T("微软雅黑")); 	
	SetWordCharFormat(DEFcf);
}

void MyRichEdit::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	InitFont();
	CRichEditCtrl::PreSubclassWindow();
}

void MyRichEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码

	long nStart, nEnd;
	CString strTemp;
	GetSel(nStart, nEnd);

	if(nStart == nEnd)		return;

	CMenu popMenu;
	if (!popMenu.LoadMenu(IDR_MENU1)) return; //载入菜单

	CMenu *pPopup;
	pPopup = popMenu.GetSubMenu(0); //获得子菜单指针

	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this); //显示弹出菜单，参数依次为(鼠标在菜单左边|跟踪右键，x，y，this)

}

void MyRichEdit::OnAdfCopy()
{
	// TODO: 在此添加命令处理程序代码
	CString strTemp = GetSelText();
	TCHAR* pSrc = strTemp.GetBuffer();
	int size = strTemp.GetLength();

	//统计 /n的个数
	int Cnt = 0;
	for (int i = 0; i<size; i++)
	{
		if (pSrc[i] == 0x0D)			Cnt++;
	}

	//将/n 转为 /n/r
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