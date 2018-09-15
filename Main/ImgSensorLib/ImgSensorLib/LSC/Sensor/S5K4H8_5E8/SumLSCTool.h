#ifdef   SumLSCTool_FILE
#define  SumLSCToolDLLAPI extern "C"  __declspec(dllexport)
#else
#define  SumLSCToolDLLAPI  extern "C"  __declspec(dllimport)
#endif




SumLSCToolDLLAPI int _stdcall  S5K5E8_LSC(BYTE *Raw10Buffer,int iMageWidth,int iMageHeight,int LSCTarget,char *Path,char* ErrorMsg);
SumLSCToolDLLAPI int _stdcall  S5K4H8_LSC(BYTE *Raw10Buffer,int iMageWidth,int iMageHeight,int LSCTarget,int Group,char *Path,char* ErrorMsg);
//SumLSCToolDLLAPI int _stdcall  S5K4H8_LSC(BYTE *Raw10Buffer,char* ErrorMsg,char *Path);


