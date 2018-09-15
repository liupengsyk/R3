
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYUNEW2D_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
extern "C"   
{

//获到锁的版本
int _stdcall NT_GetIDVersion(int *Version,char *InPath);
//写一批字节到加密锁中	
int _stdcall YWriteEx(BYTE *InData,short address,short len,char *HKey,char *LKey,char *Path );					
//写一个字节到加密锁中，一般不使用
int __stdcall YWrite(BYTE InData,short address,char * HKey,char *LKey,char *Path );
//从加密锁中读取一批字节
int _stdcall YReadEx(BYTE *OutData,short address,short len,char *HKey,char *LKey,char *Path );						
//从加密锁中读取一个字节，一般不使用
int __stdcall YRead(BYTE *OutData,short address,char * HKey,char *LKey,char *Path );	
//查找加密锁
int _stdcall FindPort(  int start,char *OutPath);
//获取锁的ID
int _stdcall GetID(  DWORD *ID_1,DWORD *ID_2,char *InPath);
//从加密锁中读字符串
int __stdcall YReadString(char *string ,short Address,int len,char * HKey,char *LKey,char *Path );
//写字符串到加密锁中
int __stdcall YWriteString(char *InString,short Address,char * HKey,char *LKey,char *Path );
//设置写密码
int __stdcall SetWritePassword(char * W_HKey,char *W_LKey,char * new_HKey,char *new_LKey,char *InPath);
//设置读密码
int __stdcall SetReadPassword(char * W_HKey,char *W_LKey,char * new_HKey,char *new_LKey,char *InPath);
//设置扩展算法密钥
int __stdcall SetCal_2(char *Key,char *InPath);
//使用增强算法对字符串进行加密
int _stdcall EncString(  char *InString,char *OutString,char *Path);
//使用增强算法对二进制数据进行加密
int _stdcall Cal(  BYTE *InBuf,BYTE *OutBuf,char *Path);

//使用增强算法对字符串进行解密(使用软件)
void _stdcall DecString(  char *InString,char *OutString,char *Key);
//使用增强算法对二进制数据进行加密(使用软件)
void _stdcall  DecBySoft(    BYTE  *   aData,   BYTE   *   aKey   )  ;
void _stdcall  EncBySoft(   BYTE  *   aData,  BYTE   *   aKey   )   ;
//字符串及二进制数据的转换
 void __stdcall  HexStringToByteArray(char * InString,BYTE *out_data);
 void __stdcall  ByteArrayToHexString(BYTE *in_data,char * OutString,int len);
//重新初始化函数
int __stdcall  ReSet(char *Path );
}
	