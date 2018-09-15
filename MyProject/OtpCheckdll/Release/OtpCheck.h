#pragma once

#ifndef Tccm_EXP
#define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */
 
extern "C"  Tccm_EXP int OtpCheck(char *pszOutputErrorMsg,int iMaxOutputErrorMsgLen,BYTE *InputOTPData, int iInputOtpSize, int Flag, char *pszFuseID,char *pszStationName);
