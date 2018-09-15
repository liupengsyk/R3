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
	TestItemInterface* pItem; //测试项目对象，DLL导出指针
	HMODULE hdll;      //导出的DLL句柄
	int ID;            //为每个测试项分配的ID
	HWND hButton;      //为每个测试项分配的BUTTON句柄
	BOOL enable;       //测试项目的开关
	//vector<int> vStorage; //每个测试项目的可被访问的存储空间
};


 