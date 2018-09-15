#include "StdAfx.h"
#include "ReportData.h"
#include "ImageProc.h"


CReportData::CReportData()
{
    m_head.clear();
    m_data.clear();
}

CReportData::~CReportData(void)
{
}

void CReportData::Init()
{
    m_head.clear();
	for (UINT i=0; i<m_data.size(); i++)
	{
		m_data[i].data.clear();
	}
	m_data.clear();
}

BOOL CReportData::LoadLastRecode(const CString &fileName, CString &errMsg)
{
    CStdioFile file;
    if (!file.Open(fileName, CFile::modeRead|CFile::typeText))
    {
        errMsg = _T("打开测试数据文件失败!");
        return FALSE;
    }

    CString strLine;
    if (file.ReadString(strLine))
    {
        ImageProc::SeparateString(strLine, ',', &m_head);
		OutputDebugString(_T("test head:") + strLine);
    }
    else
    {
        errMsg = _T("没有测试数据,请先测试!");
		file.Close();
        return FALSE;
    }

    CString tempLine = _T("");
    while (file.ReadString(tempLine)) //跑到最后一行
    {
        strLine = tempLine;
		OutputDebugString(_T("test read:") + strLine);
    }

    if (-1 != strLine.Find(_T(",,,")) || strLine == _T(""))
    {
        errMsg = fileName + _T("没有当前模组测试数据,请先测试!");
		file.Close();
        return FALSE;
    }

    vector<CString> temp;
    ImageProc::SeparateString(strLine, ',', &temp);
    if (temp.size() != m_head.size())
    {
		errMsg.Format(_T("data:%d,head:%d,测试数据项与数据头个数不一致"), temp.size(), m_head.size());
		OutputDebugString(_T("last:") + strLine);
		file.Close();
        return FALSE;
    }    
  
    float val = 0.0f;

    RECORD record;
    //插入新的m_data
    record.fuseID = temp[0];
    for (UINT j=3; j<temp.size(); j++)
    {
        val = (float)_ttof(temp[j]); //有可能有精度问题
        record.data.push_back(val);
    }
    m_data.push_back(record);    

    file.Close();
    return TRUE;
}

BOOL CReportData::LoadCaliFile(const CString &fileName, CString &errMsg)
{
    CStdioFile file;
    if (!file.Open(fileName, CFile::modeRead|CFile::typeText))
    {
        errMsg = _T("打开标准数据文件失败!");
        return FALSE;
    }

    CString strLine;
    if (file.ReadString(strLine))
    {
        ImageProc::SeparateString(strLine, ',', &m_head);
		OutputDebugString(_T("std head:") + strLine);
    }
    else
    {
		errMsg = _T("没有标准数据,请设置标准数据!");
		file.Close();
		return FALSE;
    }

    while (file.ReadString(strLine))
    {
        vector<CString> temp;
        ImageProc::SeparateString(strLine, ',', &temp);
        if (temp.size() != m_head.size())
        {
			errMsg.Format(_T("data:%d,head:%d,标准数据项与数据头个数不一致"), temp.size(), m_head.size());
			OutputDebugString(_T("std read:") + strLine);
			file.Close();
			return FALSE;
        }

        float val = 0.0f;
        vector<float> data;
        RECORD record;
        //插入新的m_data
        record.fuseID = temp[0];
        for (UINT j=3; j<temp.size(); j++)
        {
            val = (float)_ttof(temp[j]); //有可能有精度问题
            record.data.push_back(val);
        }
        m_data.push_back(record);
    }

    file.Close();
    return TRUE;
}


