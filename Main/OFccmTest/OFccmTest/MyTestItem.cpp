#include "StdAfx.h"
#include "MyTestItem.h"

MyTestItem::MyTestItem()
{
	pItem   = NULL;
	hdll    = NULL;
	enable  = TRUE;
	hButton = NULL;
	ID      = -1;
	//vStorage.clear();
}
 

MyTestItem::~MyTestItem(void)
{

}
 
void MyTestItem::Free()
{
	if(pItem != NULL) //�ͷ�DLL�����ĶԻ���Ͷ���
	{
		if(pItem->GetOptionDialogHandle() != NULL)
		{
			::DestroyWindow(pItem->GetOptionDialogHandle());
			delete pItem;
			pItem = NULL;
		}
	}
	if(hButton != NULL) //�ͷźͱ�������Ŀ��صİ�ť
	{
		::DestroyWindow(hButton);
		hButton = NULL;
	}
	 
}