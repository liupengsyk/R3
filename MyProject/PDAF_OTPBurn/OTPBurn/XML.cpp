#include "StdAfx.h"
#include "XML.h"


CXML::CXML(CString fileName)
{
	this->fileName = fileName;

	::CoInitialize(NULL);

	HRESULT HR = XMLDOC.CreateInstance(_uuidof(MSXML2::DOMDocument30)); 
	if(!SUCCEEDED(HR)) 
	{ 
		 
	} 
	XMLROOT = XMLDOC->createElement("ROOT"); 
	XMLDOC->appendChild(XMLROOT); 
}


CXML::~CXML(void)
{
	XMLROOT.Release(); 
	XMLDOC.Release(); 
	::CoUninitialize(); 
}

BOOL CXML::FileExist()
{
	return PathFileExists(fileName);
}
BOOL CXML::GetItems(vector<ItemInfo> &list)
{
	MSXML2::IXMLDOMNodeListPtr XMLNODES;			//某个节点的所以字节点
	MSXML2::IXMLDOMNamedNodeMapPtr XMLNODEATTS;		//某个节点的所有属性;
	MSXML2::IXMLDOMNodePtr XMLNODE; 
	VARIANT_BOOL bhr = XMLDOC->load((_bstr_t)fileName);
	if(VARIANT_TRUE != bhr)
	{
		return FALSE;
	}

	XMLROOT = XMLDOC->GetdocumentElement();//获得根节点;
	XMLROOT->get_childNodes(&XMLNODES);//获得根节点的所有子节点;
	long XMLNODESNUM,ATTSNUM; 
	XMLNODES->get_length(&XMLNODESNUM);//获得所有子节点的个数;
	
	for (int i = 0; i < XMLNODESNUM; i++)
	{
		ItemInfo info;
		XMLNODES->get_item(i,&XMLNODE);
		/*pInfo[i].ItemName = strAttr;*/
		strcpy(info.ItemName,XMLNODE->nodeName);
		XMLNODE->get_attributes(&XMLNODEATTS);
		XMLNODEATTS->get_length(&ATTSNUM);
		MSXML2::IXMLDOMNodePtr XMLATTRNODE;
		for (int iAttr =0; iAttr < ATTSNUM; iAttr++)
		{
			XMLNODEATTS->get_item(iAttr,&XMLATTRNODE);
			CString strAttr((_variant_t)XMLATTRNODE->nodeName);
			if (strAttr.CompareNoCase(_T("StartAddr")) == 0)
			{
				strcpy(info.startAddr,XMLATTRNODE->text);
				//pInfo[i].startAddr = (_variant_t)XMLATTRNODE->nodeName;
			}else if (strAttr.CompareNoCase(_T("EndAddr")) == 0)
			{
				strcpy(info.endAddr,XMLATTRNODE->text);
				//pInfo[i].endAddr = (_variant_t)XMLATTRNODE->nodeName;
			}else if (strAttr.CompareNoCase(_T("DataLength")) == 0)
			{

				info.length = atoi(XMLATTRNODE->text);
			}
		}

		MSXML2::IXMLDOMNodeListPtr XMLDetail;
		XMLNODE->get_childNodes(&XMLDetail );
		long length = 0;
		XMLDetail->get_length(&length);
		for (int index = 0; index < length; index++ )
		{
			MSXML2::IXMLDOMNodePtr XMLNODETMP;
			XMLDetail->get_item(index,&XMLNODETMP);
			//strcpy(pInfo[i].dataList[index],XMLNODE->text);
		
			info.dataList.push_back(strtol(XMLNODETMP->text,NULL,16));
		}
		list.push_back(info);
	}
	return TRUE;
}


BOOL CXML::AddItem(ItemInfo* info)
{
	if (FileExist())
	{
		//return FALSE;
		XMLDOC->load((_bstr_t)fileName);
		XMLROOT.Release();
		XMLROOT = XMLDOC->GetdocumentElement(); 
	}
	MSXML2::IXMLDOMElementPtr XMLParentNODE; 
	BOOL bDatavalid = TRUE;
	for (int i = 0; i <1; i++)
	{
		CStringA TMP;
		XMLParentNODE = XMLDOC->createElement((_bstr_t)info[i].ItemName);

		XMLParentNODE->setAttribute((_bstr_t)("StartAddr"),(_bstr_t)info[i].startAddr);
		XMLParentNODE->setAttribute((_bstr_t)("EndAddr"),(_bstr_t)info[i].endAddr);
		TMP.Format("%d",info[i].length);
		XMLParentNODE->setAttribute((_bstr_t)("DataLength"),(_bstr_t)TMP);
		{
			int datalen =info[i].length;
			MSXML2::IXMLDOMElementPtr XMLNODE2;
			for (int iIndex = 0; iIndex < datalen; iIndex++)
			{
				TMP.Format("%d",iIndex); 
				XMLNODE2 = XMLDOC->createElement((_bstr_t)("Data" + TMP)); 
				TMP.Format("0x%02x",info[i].dataList[iIndex]);
				XMLNODE2->put_text((_bstr_t)TMP);//设置标签的文本内容; 
				XMLParentNODE->appendChild(XMLNODE2); 
				XMLNODE2.Release();
			}
		}
		XMLROOT->appendChild(XMLParentNODE);
		XMLParentNODE.Release();

	}
	HRESULT HR = XMLDOC->save((_bstr_t)fileName); 
	if(!SUCCEEDED(HR)) 
	{ 
		return FALSE;
	} 
	return TRUE;
}


