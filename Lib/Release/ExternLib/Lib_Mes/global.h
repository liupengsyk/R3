
#ifndef _OFILMLIB
#define _OFILMLIB extern "C" _declspec(dllexport)

#ifdef OFUSB_EXPORTS
#define OFILM_INI __declspec(dllexport)
#else
#define OFILM_INI __declspec(dllimport)
#endif

#include "MesLink.h"
typedef struct _TestParamd
{
	char Name[512];
	char Value[512];
}TestParam;

class OFILM_INI CMsgBox 
{

public:
	CMsgBox();
	void ShowMsgBox(char *sMsg,int Res);
	char m_sPASS[128];
	char m_sFAIL[128];
	char m_Other[128];

	int XScale;
	int YScale;
	int WidthScale;
	int HeightScale;	
};

class OFILM_INI CMesSystem  
{
public:

	CMesSystem();
	~CMesSystem();
	
	bool MesCheck(char *ReturnMsg);
	bool MesUpdateData(char *ResurnMsg);
	bool AddTestItem(char *Name,char *Result);
	bool AddParameter(char *Name,char *Value);
	bool DeleteMesData(char *ReturnMsg);
	bool ShowLoginDlg();
	void ResetDataAfterMesUpdateMaybeFail();//mesupdate有可能会失败，需要清除测试数据
	
	char Run_Card[512];
	char Barcode_SN[512];
	char Sensor_ID[512];
	char TestVersion[512];
	char TestResult[512];
	char ErrorCode[512];	
	
private:
	bool BeginSoap(char *,char *,char *,char *,char *);
	char LineNO[512];
	char StationNo[512];
	char Operator[512];
	char Machine_NO[512];
	
	char ComputerIP[512];
	char TestTime[512];
	
	List < TestParam >TestParamList;
	List < TestParam >::MesItor iPam;
	char TestData[16384];
};

_OFILMLIB void GetLocalIp(char *szIp);
_OFILMLIB void GetLocalMac(char *szMac);
_OFILMLIB int  ShowInputPwbBox(char *cWndText,char *cShowText,char *ReturnText);
_OFILMLIB int  GetTimeTag();//获取从今年1月1日零点开始到现在所经过的小时数
_OFILMLIB void DebugInfo(char * format,...);//Debug信息
_OFILMLIB bool fSleep(unsigned short uiMillsecond);
_OFILMLIB bool GetFileVersionInfoLib(char *FileName,char *VersionInfo,int *dwSize);
_OFILMLIB bool RegReadKey(char *SubReg,char *SubKeyName,char *keyValue);//读注册表
_OFILMLIB bool RegSetKey(char *SubReg,char *SubKeyName,char *keyValue);//写注册表
_OFILMLIB bool GetFileUpdateTime(char *FilePath,char *ReturnValue);
_OFILMLIB bool GetPwdInfo(char *tBuffer);
_OFILMLIB void DoEncrypt(char *sCont,int seed, BOOL encr);
_OFILMLIB void GetNowTime(char *m_sTime);//Sample:2014-02-12 18:30:30
_OFILMLIB void GetNowDate(char *m_sDate);//Sample:20140212
_OFILMLIB void GetNowTimeT(char *m_sTime);//Sample:20140212183030
_OFILMLIB bool GetFileMD5(char *m_FilePath,char *DataMD5);
_OFILMLIB bool GetUSBKeyID(char *sUSBID);
_OFILMLIB bool ReadBMPtoBuffer(char *BmpFileName,BYTE *RGBBuffer, int &Width,int &Height);
_OFILMLIB bool SaveBuffertoBmp(BYTE *pBuffer,char *BmpFileName, int width, int height);
_OFILMLIB bool GetBmpSize(char *BmpFileName,int &Width,int &Height);
_OFILMLIB bool GetBarcode(char *RuncardSN,char Barcode[][32]);
_OFILMLIB bool GetPwd(char *PwdName,char *Pwd,char *ErrDes);
_OFILMLIB void DrawResult(HWND hParent,int Resource,COLORREF BKColor,int FontSize,char *Msg);
_OFILMLIB int CheckBarCode(char *Barcode,char *ErrorMsg);
_OFILMLIB int CheckPKList(char *PKList,char *ErrorMsg);
_OFILMLIB int Pack(char *PKList,char *barcode,char *ErrorMsg);
_OFILMLIB bool GetModelforBarcode(char *Barcode,char Model[32],char *sErrorMsg);//MES GetModelforBarcode接口
_OFILMLIB bool GetModelforFuseid(char *Fuseid, char Model[32],char *sErrorMsg);

#endif