
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYUNEW2D_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
extern "C"   
{

//�����İ汾
int _stdcall NT_GetIDVersion(int *Version,char *InPath);
//дһ���ֽڵ���������	
int _stdcall YWriteEx(BYTE *InData,short address,short len,char *HKey,char *LKey,char *Path );					
//дһ���ֽڵ��������У�һ�㲻ʹ��
int __stdcall YWrite(BYTE InData,short address,char * HKey,char *LKey,char *Path );
//�Ӽ������ж�ȡһ���ֽ�
int _stdcall YReadEx(BYTE *OutData,short address,short len,char *HKey,char *LKey,char *Path );						
//�Ӽ������ж�ȡһ���ֽڣ�һ�㲻ʹ��
int __stdcall YRead(BYTE *OutData,short address,char * HKey,char *LKey,char *Path );	
//���Ҽ�����
int _stdcall FindPort(  int start,char *OutPath);
//��ȡ����ID
int _stdcall GetID(  DWORD *ID_1,DWORD *ID_2,char *InPath);
//�Ӽ������ж��ַ���
int __stdcall YReadString(char *string ,short Address,int len,char * HKey,char *LKey,char *Path );
//д�ַ�������������
int __stdcall YWriteString(char *InString,short Address,char * HKey,char *LKey,char *Path );
//����д����
int __stdcall SetWritePassword(char * W_HKey,char *W_LKey,char * new_HKey,char *new_LKey,char *InPath);
//���ö�����
int __stdcall SetReadPassword(char * W_HKey,char *W_LKey,char * new_HKey,char *new_LKey,char *InPath);
//������չ�㷨��Կ
int __stdcall SetCal_2(char *Key,char *InPath);
//ʹ����ǿ�㷨���ַ������м���
int _stdcall EncString(  char *InString,char *OutString,char *Path);
//ʹ����ǿ�㷨�Զ��������ݽ��м���
int _stdcall Cal(  BYTE *InBuf,BYTE *OutBuf,char *Path);

//ʹ����ǿ�㷨���ַ������н���(ʹ�����)
void _stdcall DecString(  char *InString,char *OutString,char *Key);
//ʹ����ǿ�㷨�Զ��������ݽ��м���(ʹ�����)
void _stdcall  DecBySoft(    BYTE  *   aData,   BYTE   *   aKey   )  ;
void _stdcall  EncBySoft(   BYTE  *   aData,  BYTE   *   aKey   )   ;
//�ַ��������������ݵ�ת��
 void __stdcall  HexStringToByteArray(char * InString,BYTE *out_data);
 void __stdcall  ByteArrayToHexString(BYTE *in_data,char * OutString,int len);
//���³�ʼ������
int __stdcall  ReSet(char *Path );
}
	