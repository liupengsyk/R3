#ifndef _CTSVCM_H_
#define _CTSVCM_H_
#include <vector>


//#include "cts.h"

#define CTS_API extern "C"  __declspec(dllimport)
#pragma comment (lib,"ctsvcm.lib")


#define VCMICNAMEMAXLEN 100

#define DEFAULTININI "Default-in INI"

using namespace std;


CTS_API BOOL Cam_GetVCMICName(char strICName[VCMICNAMEMAXLEN], unsigned int &nDriverICNum);

CTS_API BOOL Cam_VCMInit(DWORD devInst, char *strVCMICName);

CTS_API BOOL Cam_VCMPWDN(DWORD devInst, char *strVCMICName);

CTS_API BOOL Cam_WriteVCM(DWORD devInst, char *strVCMICName, WORD value);

CTS_API BOOL Cam_ReadVCM(DWORD devInst, char *strVCMICName, WORD &value);

CTS_API BOOL Cam_CheckVCMID(DWORD devInst, char *strVCMICName, BYTE bySalveID);



#endif

