#pragma once
#include "TestItemInterface.h"
#include <vector>
using namespace std;
class MyTestItem
{
public:
	MyTestItem(void);
	~MyTestItem(void);

public: 
	void Free();
	TestItemInterface* pItem; //������Ŀ����DLL����ָ��
	HMODULE hdll;      //������DLL���
	int ID;            //Ϊÿ������������ID
	HWND hButton;      //Ϊÿ������������BUTTON���
	BOOL enable;       //������Ŀ�Ŀ���
	//vector<int> vStorage; //ÿ��������Ŀ�Ŀɱ����ʵĴ洢�ռ�
};


 