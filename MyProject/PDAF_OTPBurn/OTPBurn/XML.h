#pragma once
#include<iostream>
#include<vector>
using namespace std;

#import "msxml3.dll" 
using namespace MSXML2; 

struct ItemInfo
{
	char ItemName[20];
	char startAddr[20];
	char endAddr[20];

	USHORT length;
	vector<BYTE> dataList;
};
class CXML
{
public:
	CXML(CString fileName);
	~CXML(void);

private:
	BOOL FileExist();
public: 
	BOOL AddItem(ItemInfo* info);
	BOOL GetItems(vector<ItemInfo> &list);


private:
	MSXML2::IXMLDOMDocumentPtr XMLDOC; 
	MSXML2::IXMLDOMElementPtr XMLROOT; 

	CString fileName;
};

