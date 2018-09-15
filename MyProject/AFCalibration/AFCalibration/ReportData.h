#include <vector>

using namespace std;

typedef struct _RECORD
{
    CString fuseID;
    vector<float> data;

}RECORD, *PRECORD;

#pragma once
class CReportData
{
public:
    CReportData();
    ~CReportData(void);

public:
    vector<CString> m_head;
    vector<RECORD>  m_data;


    void Init();
    BOOL LoadLastRecode(const CString &fileName, CString &errMsg); //�������һ�����ݣ����������� 
    BOOL LoadCaliFile(const CString &fileName, CString &errMsg);
};

