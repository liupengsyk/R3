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
	if(pItem != NULL) //释放DLL导出的对话框和对象
	{
		if(pItem->GetOptionDialogHandle() != NULL)
		{
			::DestroyWindow(pItem->GetOptionDialogHandle());
			delete pItem;
			pItem = NULL;
		}
	}
	if(hButton != NULL) //释放和本测试项目相关的按钮
	{
		::DestroyWindow(hButton);
		hButton = NULL;
	}
	 
}