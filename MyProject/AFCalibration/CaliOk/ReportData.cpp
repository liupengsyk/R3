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
}

BOOL CReportData::LoadLastRecode(const CString &fileName, CString &errMsg)
{
    CStdioFile file;
    if (!file.Open(fileName, CFile::modeRead|CFile::typeText))
    {
        errMsg = _T("�򿪲��������ļ�ʧ��!");
        return FALSE;
    }

    CString strLine;
    if (file.ReadString(strLine))
    {
        ImageProc::SeparateString(strLine, ',', &m_head);
    }
    else
    {
        errMsg = _T("û�в�������,���Ȳ���!");
		file.Close();
        return FALSE;
    }

    CString tempLine = _T("");
    while (file.ReadString(tempLine)) //�ܵ����һ��
    {
        strLine = tempLine;
    }

    if (-1 != strLine.Find(_T(",,,")) || strLine == _T(""))
    {
        errMsg = fileName + _T("û�е�ǰģ���������,���Ȳ���!");
		file.Close();
        return FALSE;
    }

    vector<CString> temp;
    ImageProc::SeparateString(strLine, ',', &temp);
    if (temp.size() != m_head.size())
    {
        errMsg = _T("������������ͷ������һ��!");
		file.Close();
        return FALSE;
    }    
  
    float val = 0.0f;

    RECORD record;
    //�����µ�m_data
    record.fuseID = temp[0];
    for (UINT j=3; j<temp.size(); j++)
    {
        val = (float)_ttof(temp[j]); //�п����о�������
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
        errMsg = _T("�򿪱�׼�����ļ�ʧ��!");
        return FALSE;
    }

    CString strLine;
    if (file.ReadString(strLine))
    {
        ImageProc::SeparateString(strLine, ',', &m_head);
    }
    else
    {
		errMsg = _T("û�б�׼����,�����ñ�׼����!");
		file.Close();
		return FALSE;
    }

    while (file.ReadString(strLine))
    {
        vector<CString> temp;
        ImageProc::SeparateString(strLine, ',', &temp);
        if (temp.size() != m_head.size())
        {
			errMsg = _T("������������ͷ������һ��!");
			file.Close();
			return FALSE;
        }

        float val = 0.0f;
        vector<float> data;
        RECORD record;
        //�����µ�m_data
        record.fuseID = temp[0];
        for (UINT j=3; j<temp.size(); j++)
        {
            val = (float)_ttof(temp[j]); //�п����о�������
            record.data.push_back(val);
        }
        m_data.push_back(record);
    }

    file.Close();
    return TRUE;
}


