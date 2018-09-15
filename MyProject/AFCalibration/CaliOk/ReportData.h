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
    BOOL LoadLastRecode(const CString &fileName, CString &errMsg); //加载最后一条数据，点检测试数据 
    BOOL LoadCaliFile(const CString &fileName, CString &errMsg);
};

