/*******************************************/
/*
*函数原型：GetSFR_ofilm(float res , _sfrresult* p,float freq ,  const void *_imageBuffer,enum dataformat databyte, int _height, int _width);
*函数功能：计算单通道的SFR值
*用于计算的参数：图像数组地址 _imageBuffer, 数据格式databyte, 图像尺寸_height,  _width，频率freq
*                databyte=bayer8，一个字节表示一个数据
*                databyte=bayer10，两个字节表示一个数据              
*计算结果：结构体 _sfrresult   ，  res
*                _sfrresult中的SFR数组是频率从0~1的SFR值
*                _sfrresult中的ValidNum是SFR数组中有效的数据个数。
*                _sfrresult中的err是错误类型
*                _sfrresult中的errtip是错误信息
*                res是取某一频率处的SFR值。例如，freq=4，则res = 1/4Nyquist频率处的SFR值
*
*函数返回：错误类型。例如，返回值等于0，则错误类型为noerr（见enum errtype），对应的错误信息则可通过查询字符数组errtip查看
*函数基于：ago4501.dll    v4501.dll   OflimSFR.dll    OflimSFR.lib 
*调用实例：
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

//***********用于记录结果*************
struct _sfrresult
{
	//max size=500, 计算结果数据最多为500个
	float  SFR[500];  //MTF的结果
	float  Freq[500]; //频率结果
	int     ValidNum;  //数组中有效的数据个数
	enum errtype err;  //错误类型
	char  errtip[512];  //错误信息
};

 SFR_API  int  GetSFR_ofilm(float& res , _sfrresult* p,float freq , const void *_imageBuffer,enum dataformat databyte, int _height, int _width);
 #endif