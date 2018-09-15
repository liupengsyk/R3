/*******************************************/
/*
*����ԭ�ͣ�GetSFR_ofilm(float res , _sfrresult* p,float freq ,  const void *_imageBuffer,enum dataformat databyte, int _height, int _width);
*�������ܣ����㵥ͨ����SFRֵ
*���ڼ���Ĳ�����ͼ�������ַ _imageBuffer, ���ݸ�ʽdatabyte, ͼ��ߴ�_height,  _width��Ƶ��freq
*                databyte=bayer8��һ���ֽڱ�ʾһ������
*                databyte=bayer10�������ֽڱ�ʾһ������              
*���������ṹ�� _sfrresult   ��  res
*                _sfrresult�е�SFR������Ƶ�ʴ�0~1��SFRֵ
*                _sfrresult�е�ValidNum��SFR��������Ч�����ݸ�����
*                _sfrresult�е�err�Ǵ�������
*                _sfrresult�е�errtip�Ǵ�����Ϣ
*                res��ȡĳһƵ�ʴ���SFRֵ�����磬freq=4����res = 1/4NyquistƵ�ʴ���SFRֵ
*
*�������أ��������͡����磬����ֵ����0�����������Ϊnoerr����enum errtype������Ӧ�Ĵ�����Ϣ���ͨ����ѯ�ַ�����errtip�鿴
*�������ڣ�ago4501.dll    v4501.dll   OflimSFR.dll    OflimSFR.lib 
*����ʵ����
*	unsighed char ROIBuffer[128*128]={.............};
*	int ROIheight=128;
*	int ROIwidth =128;
*	float Frequency=4;
*	int SFR_ERR=0;
*	float SFRValue=0;
*	_sfrresult sfrresult;
*	memset(&sfrresult , 0 , sizeof(_sfrresult));
*	SFR_ERR = GetSFR_ofilm(SFRValue , &sfrresult , Frequency , ROIBuffer , bayer8, ROIheight , ROIwidth);
*				  
*/
/*********************************************/

#ifndef SFR_API
#define SFR_API extern "C" _declspec(dllexport)

enum dataformat{ bayer8 , bayer10 };

enum errtype { noerr=0, noimadata, smallimasize, bigimasize, lumdif, edgelocate, smallangle , bigangle };

//***********���ڼ�¼���*************
struct _sfrresult
{
	//max size=500, �������������Ϊ500��
	float  SFR[500];  //MTF�Ľ��
	float  Freq[500]; //Ƶ�ʽ��
	int     ValidNum;  //��������Ч�����ݸ���
	enum errtype err;  //��������
	char  errtip[512];  //������Ϣ
};

 SFR_API  int  GetSFR_ofilm(float& res , _sfrresult* p,float freq , const void *_imageBuffer,enum dataformat databyte, int _height, int _width);
 #endif