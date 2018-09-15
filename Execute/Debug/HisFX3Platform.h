//
#ifndef HISFX3PLATFORM_H
#define HISFX3PLATFORM_H

#include <vector>
#include <string>
#include "HisGlobalDefine.h"

 
#ifdef HISFX3PLATFORM_LIB
#define HISFX3PLATFORM_EXPORT /*extern "C"*/ __declspec(dllexport)
#else
#define HISFX3PLATFORM_EXPORT /*extern "C"*/ __declspec(dllimport)
#endif
 
//!  软龙格图像采集工装封装类
/*! 电脑配置建议在WIN7以上系统， 4G以上内存， INTEL I3以上处理器
*/

struct _RolongoXMLOtherSectionItem
{
	std::string key;
	std::string value;
};

class HISFX3PLATFORM_EXPORT CHisFX3Platform
{
public:
	CHisFX3Platform(void);
	~CHisFX3Platform(void);
	//!  打开工装， 在执行其他操作之间必须打开工装
	/*! 建议在程序一开始的时候打开工装，程序结束时关闭工装
	\param[in] usPID USB3.0设备PID， 此参数暂时保留，请固定设为0
	\param[in] usVID USB3.0设备VID， 此参数暂时保留，请固定设为0
	\param[in] pstrFriendlyName USB3.0设备描述名称， 此参数暂时保留，请固定设为0
	\param[in] ucDeviceIndex USB3.0设备序号， 此参数暂时保留，请固定设为0xFF
	\param[in] puiKey 工装唯一序列号， 当puiKey==NULL,函数打开连接到电脑的任一工装； 
	                         当puiKey是指向 unsigned int[4]数组的指针时，函数打开和输入序列号匹配的工装。
							 此参数主要用于1个电脑连接多个工装的情况。
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3CloseDevice()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3OpenDevice(unsigned short usPID=0, unsigned short usVID=0, char* pstrFriendlyName=NULL, unsigned char ucDeviceIndex=0xFF, unsigned int* puiKey = NULL, unsigned int uiTransformDelay = 110);

	//!  获取工装数量
	/*! 
	\return 工装数量
	*/
	unsigned int HisFX3DeviceCount();

	//!  关闭工装
	/*! 
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3OpenDevice()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3CloseDevice();

	//! 通道1开始出图
	/*! 如果是双摄工装，此函数操作的是通道1
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa _HisFX3_PreviewStruct， HisFX3StopPreview()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3StartPreview(struct _HisFX3_PreviewStruct* pstPrama);

	//! 通道1停止出图
	/*! 如果是双摄工装，此函数操作的是通道1
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3StopPreview();

	//! 通道2开始出图
	/*! 只有在双摄工装上， 此函数才有效
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa _HisFX3_PreviewStruct， HisFX3StopPreview_S2()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3StartPreview_S2(struct _HisFX3_PreviewStruct* pstPrama);

	//! 通道2停止出图
	/*! 只有在双摄工装上， 此函数才有效
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview_S2()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3StopPreview_S2();

	int reStartPreview(struct _HisFX3_PreviewStruct* pstPrama);

	//! 通道1抓图
	/*! 如果是双摄工装，此函数操作的是通道1
	\param[out] ppucBuffer 接收图像数据的buffer指针，必须用_aligned_malloc(size, _HisCacheLine_Aligned)申请内存空间。
										  如果传入NULL, 则只会返回ui64FramIndex。
	\param[in] uiBufBytes ppucBuffer申请的内存大小，必须大于一帧图像所需的内存大小
	\param[out] ui64FramIndex 当前帧索引，开始出图之后的第一帧为1，以此类推
	\param[in] uiTimeOut 获取新的一帧的timeout时间
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3GrabFrame(unsigned char **ppucBuffer, unsigned int uiBufBytes, unsigned __int64& ui64FramIndex, unsigned int uiTimeOut = 3000);

	//! 通道2抓图
	/*! 如果是双摄工装，此函数操作的是通道2
	\param[out] ppucBuffer 接收图像数据的buffer指针，必须用_aligned_malloc(size, _HisCacheLine_Aligned)申请内存空间。
										  如果传入NULL, 则只会返回ui64FramIndex。
	\param[in] uiBufBytes ppucBuffer申请的内存大小，必须大于一帧图像所需的内存大小
	\param[out] ui64FramIndex 当前帧索引，开始出图之后的第一帧为1，以此类推
	\param[in] uiTimeOut 获取新的一帧的timeout时间
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview()， _HisFX3Platform_ErrorCode
	*/
	int HisFX3GrabFrame_S2(unsigned char **ppucBuffer, unsigned int uiBufBytes, unsigned __int64& ui64FramIndex, unsigned int uiTimeOut = 3000);

	//! 工装是否已打开
	/*! 
	\return true: 已打开  false: 还未打开
	\sa HisFX3isstart()， HisFX3OpenDevice, HisFX3CloseDevice
	*/
	bool HisFX3isopen();

	//! 通道1数据流是否已开启
	/*! 
	\return true: 已开启  false: 未开启
	\sa HisFX3isopen()， HisFX3StartPreview(), HisFX3StopPreview()
	*/
	bool HisFX3isstart();

	//! 通道2数据流是否已开启
	/*!  只有在双摄工装上， 此函数才有效
	\return true: 已开启  false: 未开启
	\sa HisFX3isopen()， HisFX3StartPreview_S2(), HisFX3StopPreview_S2()
	*/
	bool HisFX3isstart_S2();

	//! 获取当前open工装的唯一序列号
	/*!  只有在成功调用HisFX3OpenDevice之后才能调用此函数
	\param puiKey 指向unsigned int[4], 如果函数成功， 会复制设备唯一序列号到此数组空间
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa getHardwareKeyList, HisFX3OpenDevice()， HisFX3CloseDevice(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetHardwareKey(unsigned int* puiKey);

	//! 获取电脑上所有工装的唯一序列号
	/*!  只有在HisFX3OpenDevice之前才能调用此函数
	\param[out] puiKey 指向unsigned int, 如果函数成功， 会复制设备唯一序列号到此数组空间
	\param[out] uiKeyCount  返回有多少组序列号，一组序列号用4个unsigned int表示
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetHardwareKey, HisFX3OpenDevice()， HisFX3CloseDevice(), _HisFX3Platform_ErrorCode
	*/
	int getHardwareKeyList(unsigned int* puiKeyList, unsigned int& uiKeyCount);
	
	//! 操作通道1 RST信号
	/*!  
	\param bPullHigh true: 置为高电平  false:置为低电平
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PullPWND(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullReset(bool bPullHigh);

	//! 操作通道2 RST信号
	/*!    只有在双摄工装上， 此函数才有效
	\param bPullHigh true: 置为高电平  false:置为低电平
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PullPWND_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullReset_S2(bool bPullHigh);

	//! 操作通道1 PWDN信号
	/*!  
	\param bPullHigh true: 置为高电平  false:置为低电平
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PullReset(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullPWND(bool bPullHigh);

	//! 操作通道2 PWDN信号
	/*!    只有在双摄工装上， 此函数才有效
	\param bPullHigh true: 置为高电平  false:置为低电平
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PullReset_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullPWND_S2(bool bPullHigh);

	//! 单条写IIC
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] ui64Data 要写入的数据
	\param[in] usType 寄存器和数据格式, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] back  true: 判断ACK  false:不判断ACK(耗时更少，但更不安全)
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3WriteIIC(unsigned char ucSlave, unsigned int uiReg, unsigned __int64 ui64Data, unsigned short usType, bool back=true);

	//! 单条写通道2 IIC
	/*!    只有在双摄工装上， 此函数才有效
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] ui64Data 要写入的数据
	\param[in] usType 寄存器和数据格式, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] back  true: 判断ACK  false:不判断ACK(耗时更少，但更不安全)
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3WriteIIC_S2(unsigned char ucSlave, unsigned int uiReg, unsigned __int64 ui64Data, unsigned short usType, bool back=true);

	//! 单条读IIC
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[out] pui64Data 读出来的数据
	\param[in] usType 寄存器和数据格式, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3ReadI2C(unsigned char ucSlave, unsigned int uiReg, unsigned __int64* pui64Data, unsigned short usType); 

	//! 单条读IIC(通道2)
	/*!    只有在双摄工装上， 此函数才有效
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[out] pui64Data 读出来的数据
	\param[in] usType 寄存器和数据格式, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3ReadI2C_S2(unsigned char ucSlave, unsigned int uiReg, unsigned __int64* pui64Data, unsigned short usType); 

	//! 批量写入IIC
	/*! 
	\param[in] uiCount  要写入的IIC的条数
	\param[in] pucSlave 设备地址数组指针
	\param[in] puiReg 寄存器地址数组指针
	\param[in] puiData 数据数组指针
	\param[in] pusType 格式数组指针, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay 命令之间的间隔时间，单位: us
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC(), HisFX3ReadIIC(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchWriteIICNoLimit(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);

	//! 批量写入IIC(通道2)
	/*! 
	\param[in] uiCount  要写入的IIC的条数
	\param[in] pucSlave 设备地址数组指针
	\param[in] puiReg 寄存器地址数组指针
	\param[in] puiData 数据数组指针
	\param[in] pusType 格式数组指针, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay 命令之间的间隔时间，单位: us
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC_S2(), HisFX3ReadIIC_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchWriteIICNoLimit_S2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);
	
	//! 批量读取IIC
	/*! 
	\param[in] uiCount  要写入的IIC的条数
	\param[in] pucSlave 设备地址数组指针
	\param[in] puiReg 寄存器地址数组指针
	\param[out] puiData 数据数组指针
	\param[in] pusType 格式数组指针, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay 命令之间的间隔时间，单位: us
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC(), HisFX3ReadIIC(), HisFX3BatchWriteIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchReadIICNoLimit(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);
	
	//! 批量读取IIC(通道2)
	/*! 
	\param[in] uiCount  要写入的IIC的条数
	\param[in] pucSlave 设备地址数组指针
	\param[in] puiReg 寄存器地址数组指针
	\param[out] puiData 数据数组指针
	\param[in] pusType 格式数组指针, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay 命令之间的间隔时间，单位: us
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC_S2(), HisFX3ReadIIC_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchReadIICNoLimit_S2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);

	//! Page写入IIC
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[in] pucData 数据数组指针
	\param[in] uiDataSize 要写入的数据长度，单位:byte
	\param[in] usDelay 每个BYTE延时usDelay *0.5个始终周期
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned short usDelay = 4);

	//! Page模式写入IIC(通道2)
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[in] pucData 数据数组指针
	\param[in] uiDataSize 要写入的数据长度，单位:byte
	\param[in] usDelay 每个BYTE延时usDelay *0.5个始终周期
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C_S2(), HisFX3WriteI2C_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIIC_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned short usDelay = 4);
	
	//! Page模式读取IIC
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[out] pucData 数据数组指针
	\param[in] uiDataSize 要读取的数据长度，单位:byte
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize);
	
	//! Page模式读取IIC(通道2)
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[out] pucData 数据数组指针
	\param[in] uiDataSize 要读取的数据长度，单位:byte
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C_S2(), HisFX3WriteI2C_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIIC_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize);

	//! Page写入IIC, 无长度限制(通道1)
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[in] pucData 数据数组指针
	\param[in] uiDataSize 要写入的数据长度，单位:byte
	\param[in] uiPageSize 一个Page的大小，需要查阅EEPROM的规格书，一般是128,256,512,1024...
	\param[in] usByteDelay 每个BYTE延时usDelay *0.5个始终周期
	\param[in] uiPageDelay 每个Page写入之间的延时，单位:ms
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PageWriteIIC(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIICNoLimit(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned short usByteDelay = 4, unsigned int uiPageDelay= 100);
	
	//! Page写入IIC, 无长度限制(通道2)
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[in] pucData 数据数组指针
	\param[in] uiDataSize 要写入的数据长度，单位:byte
	\param[in] uiPageSize 一个Page的大小，需要查阅EEPROM的规格书，一般是128,256,512,1024...
	\param[in] usByteDelay 每个BYTE延时usDelay *0.5个始终周期
	\param[in] uiPageDelay 每个Page写入之间的延时，单位:ms
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PageWriteIIC_S2(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIICNoLimit_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned short usByteDelay = 4, unsigned int uiPageDelay= 100);

	//! Page模式读取IIC, 无长度限制(通道1)
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[out] pucData 数据数组指针
	\param[in] uiDataSize 要读取的数据长度，单位:byte
	\param[in] uiPageSize 一个Page的大小，需要查阅EEPROM的规格书，一般是128,256,512,1024...
	\param[in] uiPageDelay 每个Page读取之间的延时，单位:ms
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PageReadIIC(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIICNoLimit(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned int uiPageDelay = 0);

	//! Page模式读取IIC, 无长度限制(通道2)
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[out] pucData 数据数组指针
	\param[in] uiDataSize 要读取的数据长度，单位:byte
	\param[in] uiPageSize 一个Page的大小，需要查阅EEPROM的规格书，一般是128,256,512,1024...
	\param[in] uiPageDelay 每个Page读取之间的延时，单位:ms
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3PageReadIIC_S2(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIICNoLimit_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned int uiPageDelay = 0);

	//! Page模式写入SPI
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[in] pucData 数据数组指针
	\param[in] uiDataSize 要写入的数据长度，单位:byte
	\param[in] bCSLow 设置操作SPI时的CS信号电平。 true:低电平； false:高电平
	\param[in] bLowFirst true: 低位在前； false:高位在前
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3PageReadSPI(), HisFX3WriteSPI(), HisFX3ReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteSPI(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, bool bCSLow=true, bool bLowFirst=true);
	
	//! Page模式读取SPI
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] uiRegSize 寄存器地址字节长度，1,2,4可选
	\param[out] pucData 数据数组指针
	\param[in] uiDataSize 要写入的数据长度，单位:byte
	\param[in] bCSLow 设置操作SPI时的CS信号电平。 true:低电平； false:高电平
	\param[in] bLowFirst true: 低位在前； false:高位在前
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3PageWriteSPI(), HisFX3WriteSPI(), HisFX3ReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadSPI(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, bool bCSLow=true, bool bLowFirst=true);

	//! 单条写SPI
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[in] ui64Data 要写入的数据
	\param[in] usType 寄存器和数据格式, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] bCSLow 设置操作SPI时的CS信号电平。 true:低电平； false:高电平
	\param[in] bLowFirst true: 低位在前； false:高位在前
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3ReadSPI(), HisFX3PageWriteSPI(), HisFX3PageReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3WriteSPI(unsigned char ucSlave, unsigned int uiReg, unsigned __int64 ui64Data, unsigned short usType, bool bCSLow=true, bool bLowFirst=true);
	
	//! 单条读取SPI
	/*!    
	\param[in] ucSlave 从机设备地址
	\param[in] uiReg 寄存器地址
	\param[out] pui64Data 读取的数据
	\param[in] usType 寄存器和数据格式, eg:0x1608-表示地址位写入16bit, 数据位写入8bit。
							 支持的格式:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] bCSLow 设置操作SPI时的CS信号电平。 true:低电平； false:高电平
	\param[in] bLowFirst true: 低位在前； false:高位在前
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3WriteSPI(), HisFX3PageWriteSPI(), HisFX3PageReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3ReadSPI(unsigned char ucSlave, unsigned int uiReg, unsigned __int64* pui64Data, unsigned short usType, bool bCSLow=true, bool bLowFirst=true);

	//! 向下兼容API, 建议不使用
	int HisFX3BatchWriteIIC(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short usType, unsigned short usDelay=10);
	
	//! 向下兼容API, 建议不使用
	int HisFX3BatchReadIIC(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short usType, unsigned short usDelay=10);
	
	//! 向下兼容API, 建议不使用
	int HisFX3BatchWriteIIC_V2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay);
	
	//! 向下兼容API, 建议不使用
	int HisFX3BatchReadIIC_V2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay);

	//! 设置IIC通信速度
	/*!    
	\param[in] us1kMultiple 10KHz~1MHz范围可调
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetIICSpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetIICSpeed(unsigned short us1kMultiple);

	//! 设置IIC通信速度(通道2)
	/*!    
	\param[in] us1kMultiple 10KHz~1MHz范围可调
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetIICSpeed_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetIICSpeed_S2(unsigned short us1kMultiple);

	//! 获取当前IIC通信速度
	/*!    
	\param[out] pus1kMultiple 单位：KHz
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetIICSpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetIICSpeed(unsigned short* pus1kMultiple);

	//! 获取当前IIC通信速度(通道2)
	/*!    
	\param[out] pus1kMultiple 单位：KHz
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetIICSpeed_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetIICSpeed_S2(unsigned short* pus1kMultiple);

	//! 设置SPI通信速度
	/*!    
	\param[in] us1kMultiple 10KHz~25MHz范围可调, 单位: KHz
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetSPISpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetSPISpeed(unsigned short us1kMultiple);

	//! 获取当前SPI通信速度
	/*!    
	\param[out] pus1kMultiple 单位：KHz
	\return 0:成功  非0:失败的错误代码, 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetSPISpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetSPISpeed(unsigned short* pus1kMultiple);

	//! 侦测通道1的从设备地址响应
	/*!    
	\param[in] ucSlave 从设备的设备地址
	\return 0:有响应  非0:未响应, 请参考_HisFX3Platform_ErrorCode
	\sa _HisFX3Platform_ErrorCode
	*/
	int HisFX3SlaveResponds(unsigned char ucSlave);

	//! 侦测通道2的从设备地址响应
	/*!    
	\param[in] ucSlave 从设备的设备地址
	\return 0:有响应  非0:未响应, 请参考_HisFX3Platform_ErrorCode
	\sa _HisFX3Platform_ErrorCode
	*/
	int HisFX3SlaveResponds_S2(unsigned char ucSlave);

	//! 保留
	int HisFX3GetMIPIDataType(_HisFX3_ImageSenor_DataType* pDataType);

	//! 开关5V12V电压输出【R5】
	/*!    
	\param[in] uiV5 0：5V关闭  1：5V打开  2：5V不设置
	\param[in] uiV12 0：12V关闭  1：12V打开  2：12V不设置
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetVolt, HisFX3SetVolt_S2, _HisFX3Platform_ErrorCode
	*/
	int HisFX3Set5V12V(unsigned int uiV5 = 2, unsigned int uiV12 = 2);

	//! 设置通道1电压
	/*!    
	\param[in] flDOVDD 0.0V~3.5V范围可调, 实际所设电压= flDOVDD *flMultiple
	\param[in] flAVDD 0.0V~3.5V范围可调, 实际所设电压= flAVDD *flMultiple
	\param[in] flDVDD 0.0V~3.5V范围可调, 实际所设电压= flDVDD *flMultiple
	\param[in] flAF 0.0V~3.5V范围可调, 实际所设电压= flAF *flMultiple
	\param[in] flVFuse 0.0V~10.0V范围可调, 实际所设电压= flVFuse *flMultiple
	\param[in] flPOW 0.0V~3.5V范围可调【R5】【R3X】【R5X】, 实际所设电压= flPOW *flMultiple
	\param[in] flIODD 0.0V~3.5V范围可调【板卡内部IO电平】【R5】, 实际所设电压= flIODD *flMultiple
	\param[in] usSwitch 设置电压开关， _HisFX3_Platform_VoltFlag，
									  eg,设置AVDD和DVDD电压， _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flMultiple 电压值系数
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3Set5V12V, HisFX3SetVolt_S2, HisFX3GetVolt, _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetVolt(float flDOVDD=0.0f, float flAVDD=0.0f, float flDVDD=0.0f, float flAF = 0.0f, float flVFuse=0.0f, float flPOW=0.0f, float flIODD = 0.0f, unsigned short usSwitch=0xFF, float flMultiple = 0.1f);
	
	//! 设置通道2电压
	/*!    
	\param[in] flDOVDD 0.0V~3.5V范围可调, 实际所设电压= flDOVDD *flMultiple
	\param[in] flAVDD 0.0V~3.5V范围可调, 实际所设电压= flAVDD *flMultiple
	\param[in] flDVDD 0.0V~3.5V范围可调, 实际所设电压= flDVDD *flMultiple
	\param[in] flAF 0.0V~3.5V范围可调, 实际所设电压= flAF *flMultiple
	\param[in] flVFuse 0.0V~10.0V范围可调, 实际所设电压= flVFuse *flMultiple
	\param[in] flPOW 0.0V~3.5V范围可调【R5】, 实际所设电压= flPOW *flMultiple
	\param[in] flIODD 0.0V~3.5V范围可调【板卡内部IO电平】【R5】, 实际所设电压= flIODD *flMultiple
	\param[in] usSwitch 设置电压开关， _HisFX3_Platform_VoltFlag，
									  eg,设置AVDD和DVDD电压， _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flMultiple 电压值系数
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3Set5V12V, HisFX3SetVolt, HisFX3GetVolt_S2, _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetVolt_S2(float flDOVDD=0, float flAVDD=0, float flDVDD=0, float flAF = 0, float flVFuse=0, float flPOW=0, float flIODD = 0, unsigned short usSwitch=0xFF, float flMultiple=0.1f);

	//! 获取通道1当前设置电压
	/*!    
	\param[out] pflDOVDD 当前DOVDD设置电压
	\param[out] pflAVDD 当前AVDD设置电压
	\param[out] pflDVDD 当前DVDD设置电压
	\param[out] pflAF 当前AF设置电压
	\param[out] pflVFuse 当前VFuse设置电压
	\param[out] pflPOW 当前POW设置电压
	\param[out] pflIODD 当前IODD设置电压
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3SetVolt,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetVolt(float* pflDOVDD=NULL, float* pflAVDD=NULL, float* pflDVDD=NULL, float* pflAF=NULL, float* pflVFuse=NULL, float* pflPOW=NULL, float* pflIODD=NULL);

	//! 获取通道2当前设置电压
	/*!    
	\param[out] pflDOVDD 当前DOVDD设置电压
	\param[out] pflAVDD 当前AVDD设置电压
	\param[out] pflDVDD 当前DVDD设置电压
	\param[out] pflAF 当前AF设置电压
	\param[out] pflVFuse 当前VFuse设置电压
	\param[out] pflPOW 当前POW设置电压
	\param[out] pflIODD 当前IODD设置电压
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3SetVolt_S2,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetVolt_S2(float* pflDOVDD=NULL, float* pflAVDD=NULL, float* pflDVDD=NULL, float* pflAF=NULL, float* pflVFuse=NULL, float* pflPOW=NULL, float* pflIODD=NULL);

	//! 废弃
	int HisFX3SetVFuseVolt(unsigned char ucVFUSE);

	//! 废弃
	int HisFX3GetVFuseVolt(float* pflVFUSE = NULL);

	//! 设置通道1 MCLK时钟
	/*!    
	\param[in] flMCLK 0~136MHz可调， 单位: MHz
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3GetMCLK,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetMCLK(float flMCLK);

	//! 设置通道2 MCLK时钟
	/*!    
	\param[in] flMCLK 0~136MHz可调， 单位: MHz
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3GetMCLK_S2,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetMCLK_S2(float flMCLK);

	//! 获取通道1当前MCLK设置时钟
	/*!    
	\param[out] pflMCLK 单位: MHz
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3SetMCLK,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetMCLK(float* pflMCLK);

	//! 获取通道2当前MCLK设置时钟
	/*!    
	\param[out] pflMCLK 单位: MHz
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3SetMCLK_S2,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetMCLK_S2(float* pflMCLK);

	//! 设置通道1 Sensor数据格式
	/*! 
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetMIPILane, _HisFX3_MIPILane_Stream
	*/
	int HisFX3SetMIPILane(unsigned char ucLane);

	//! 设置通道2 Sensor数据格式
	/*! 
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetMIPILane, _HisFX3_MIPILane_Stream
	*/
	int HisFX3SetMIPILane_S2(unsigned char ucLane);

	//! 获取当前设置的通道1 Sensor数据格式
	/*! 
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetMIPILane, _HisFX3_MIPILane_Stream
	*/
	int HisFX3GetMIPILane(unsigned char* pucLane);	

	//! 获取当前设置的通道2 Sensor数据格式
	/*! 
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetMIPILane_S2, _HisFX3_MIPILane_Stream
	*/
	int HisFX3GetMIPILane_S2(unsigned char* pucLane);

	//! 设置并口VS, HS同步信号
	/*! 一般只需要设置VS信号， HS信号保持默认
	\param[in] bDVP_VS_ActiveHigh VS信号， true:高电平数据有效， false: 低电平数据有效
	\param[in] bDVP_HS_ActiveHigh HS信号， true:高电平数据有效， false: 低电平数据有效
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3GetDVPHSVS, _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetDVPHSVS(bool bDVP_VS_ActiveHigh=false, bool bDVP_HS_ActiveHigh=true);

	//! 获取当前设置的并口VS, HS同步信号
	/*! 
	\param[out] pbDVP_VS_ActiveHigh VS信号
	\param[out] pbDVP_HS_ActiveHigh HS信号
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa HisFX3SetDVPHSVS, _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetDVPHSVS(bool* pbDVP_VS_ActiveHigh=NULL, bool* pbDVP_HS_ActiveHigh=NULL);

	//! 保留
	int HisFX3DVPPCLKPhaseShift(short sCountPs18);

	//! 获取工装固件版本号
	/*!    
	\param[out] puiversion 版本号
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa   HisFX3OpenDevice,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetHardwareVersion(unsigned int* puiversion);

	//! 保留
	int HisFX3GetHardGeneration(unsigned int* puiGernation);

	int HisFX3CurrentCalibration(unsigned int* puiKey, _HisFX3_Current_Calibration* pstCal);
	int HisFX3CurrentCalibration_S2(unsigned int* puiKey, _HisFX3_Current_Calibration* pstCal);

	int HisFX3SetCurrentCalData(_HisFX3_Current_Calibration* pstCal);
	int HisFX3SetCurrentCalData_S2(_HisFX3_Current_Calibration* pstCal);

	//! 测量通道1工作电流
	/*!    
	\param[out] flAVDD 返回的AVDD电流
	\param[out] flDVDD 返回的DVDD电流
	\param[out] flDOVDD 返回的DOVDD电流
	\param[out] flAF 返回的AF电流
	\param[out] flVFuse 返回的VFuse电流
	\param[out] flPOW 返回的POW电流【R5】【R3X】【R5X】
	\param[in] uiFlag 设置要测哪些路的电流值， _HisFX3_Platform_VoltFlag，
									 eg,要测AVDD和DVDD电流， _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flAVDD_Cal 废弃，向下兼容需要的
	\param[in] flDVDD_Cal 废弃，向下兼容需要的
	\param[in] flDOVDD_Cal 废弃，向下兼容需要的
	\param[in] flAF_Cal 废弃，向下兼容需要的
	\param[in] flVFuse_Cal 废弃，向下兼容需要的
	\param[in] bScale 废弃，向下兼容需要的
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa   HisFX3GetStandbyCurrent,  HisFX3GetWorkCurrent_S2, HisFX3GetStandbyCurrent_S2, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetWorkCurrent(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flVFuse, float& flPOW, unsigned int uiFlag=0xFF, \
		float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, float flVFuse_Cal=0.0f, bool bScale = true);

	//! 测量通道2工作电流
	/*!    
	\param[out] flAVDD 返回的AVDD电流
	\param[out] flDVDD 返回的DVDD电流
	\param[out] flDOVDD 返回的DOVDD电流
	\param[out] flAF 返回的AF电流
	\param[out] flVFuse 返回的VFuse电流
	\param[out] flPOW 返回的POW电流【R5】【R3X】【R5X】
	\param[in] uiFlag 设置要测哪些路的电流值， _HisFX3_Platform_VoltFlag，
									 eg,要测AVDD和DVDD电流， _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flAVDD_Cal 废弃，向下兼容需要的
	\param[in] flDVDD_Cal 废弃，向下兼容需要的
	\param[in] flDOVDD_Cal 废弃，向下兼容需要的
	\param[in] flAF_Cal 废弃，向下兼容需要的
	\param[in] flVFuse_Cal 废弃，向下兼容需要的
	\param[in] bScale 废弃，向下兼容需要的
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa   HisFX3GetStandbyCurrent,  HisFX3GetWorkCurrent, HisFX3GetStandbyCurrent_S2, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetWorkCurrent_S2(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flVFuse, float& flPOW, unsigned int uiFlag=0xFF, \
		float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, float flVFuse_Cal=0.0f, bool bScale = true);
	
	//! 测量通道1静态电流
	/*!    
	\param[out] flAVDD 返回的AVDD电流
	\param[out] flDVDD 返回的DVDD电流
	\param[out] flDOVDD 返回的DOVDD电流
	\param[out] flAF 返回的AF电流
	\param[out] flPOW 返回的POW电流【R5】【R3X】【R5X】
	\param[in] uiFlag 设置要测哪些路的电流值， _HisFX3_Platform_VoltFlag，
									 eg,要测AVDD和DVDD电流， _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] bPWND	true:拉PWND进入待机模式，  false:无需拉PWND信号
	\param[in] bReset	true:拉RST进入待机模式，  false:无需拉RST信号
	\param[in] bMCLK	true:MCLK设为0MHz进入待机模式，  false:无需把MCLK始终设为0MHz
	\param[in] bDVDD	true:DVDD设为0V进入待机模式，  false:无需把DVDD进入0V
	\param[in] uiDelay	进入待机模式后延时uiDelay ms之后再测待机电流
	\param[in] flAVDD_Cal 废弃，向下兼容需要的
	\param[in] flDVDD_Cal 废弃，向下兼容需要的
	\param[in] flDOVDD_Cal 废弃，向下兼容需要的
	\param[in] flAF_Cal 废弃，向下兼容需要的
	\param[in] ucAVDDGrade 废弃，向下兼容需要的
	\param[in] ucDVDDGrade 废弃，向下兼容需要的
	\param[in] ucDOVDDGrade 废弃，向下兼容需要的
	\param[in] ucAFGrade 废弃，向下兼容需要的
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa   HisFX3GetWorkCurrent,  HisFX3GetWorkCurrent_S2, HisFX3GetStandbyCurrent_S2, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetStandbyCurrent(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flPOW, unsigned int uiFlag=0xFF, \
		bool bPWND=true, bool bReset=true,  bool bMCLK=true, bool bDVDD=false, unsigned int uiDelay=5000, float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, \
		float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, unsigned char ucAVDDGrade=2, unsigned char ucDVDDGrade=2, unsigned char ucDOVDDGrade=2, unsigned char ucAFGrade=2);

	//! 测量通道2静态电流
	/*!    
	\param[out] flAVDD 返回的AVDD电流
	\param[out] flDVDD 返回的DVDD电流
	\param[out] flDOVDD 返回的DOVDD电流
	\param[out] flAF 返回的AF电流
	\param[out] flPOW 返回的POW电流【R5】【R3X】【R5X】
	\param[in] uiFlag 设置要测哪些路的电流值， _HisFX3_Platform_VoltFlag，
									 eg,要测AVDD和DVDD电流， _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] bPWND	true:拉PWND进入待机模式，  false:无需拉PWND信号
	\param[in] bReset	true:拉RST进入待机模式，  false:无需拉RST信号
	\param[in] bMCLK	true:MCLK设为0MHz进入待机模式，  false:无需把MCLK始终设为0MHz
	\param[in] bDVDD	true:DVDD设为0V进入待机模式，  false:无需把DVDD进入0V
	\param[in] uiDelay	进入待机模式后延时uiDelay ms之后再测待机电流
	\param[in] flAVDD_Cal 废弃，向下兼容需要的
	\param[in] flDVDD_Cal 废弃，向下兼容需要的
	\param[in] flDOVDD_Cal 废弃，向下兼容需要的
	\param[in] flAF_Cal 废弃，向下兼容需要的
	\param[in] ucAVDDGrade 废弃，向下兼容需要的
	\param[in] ucDVDDGrade 废弃，向下兼容需要的
	\param[in] ucDOVDDGrade 废弃，向下兼容需要的
	\param[in] ucAFGrade 废弃，向下兼容需要的
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa   HisFX3GetStandbyCurrent,  HisFX3GetWorkCurrent_S2, HisFX3GetWorkCurrent, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetStandbyCurrent_S2(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flPOW, unsigned int uiFlag=0xFF, \
		bool bPWND=true, bool bReset=true,  bool bMCLK=true, bool bDVDD=false, unsigned int uiDelay=5000, float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, \
		float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, unsigned char ucAVDDGrade=2, unsigned char ucDVDDGrade=2, unsigned char ucDOVDDGrade=2, unsigned char ucAFGrade=2);
	
	int HisFX3VlotageCalibration(unsigned int* puiKey, _HisFX3_Voltage_Calibration* pstCal);
	int HisFX3VlotageCalibration_S2(unsigned int* puiKey, _HisFX3_Voltage_Calibration* pstCal);
	int HisFX3SetVoltageCalData(_HisFX3_Voltage_Calibration* pstCal);
	int HisFX3SetVoltageCalData_S2(_HisFX3_Voltage_Calibration* pstCal);

	//! 正向对地开路测试
	/*!
	\param[in] ucSwitch   0：测试第一路MIPI  1：测试并口	2：测试第二路MIPI
	\param[inout]   vectorConfigconst  配置要进行开路测试的PIN脚
	\param[in] cuiCurrentWaitTime 电流稳定时间：cuiCurrentWaitTime x3 (ms)
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  HisFX3OSNegtiveTest,  HisFX3OSShortTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSPositiveTest(unsigned char ucSwitch, std::vector<_HisFX3OS_Positive_Item>& vectorConfigconst, const unsigned int cuiCurrentWaitTime=4);

	//! 负向对DOVDD开路测试
	/*!
	\param[in] ucSwitch   0：测试第一路MIPI  1：测试并口	2：测试第二路MIPI
	\param[inout]   vectorConfig  配置要进行测试的PIN脚
	\param[in] cuiCurrentWaitTime 电流稳定时间：cuiCurrentWaitTime x3 (ms)
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  _HisFX3OS_Positive_Item,  HisFX3OSShortTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSNegtiveTest(unsigned char ucSwitch, std::vector<_HisFX3OS_Negtive_Item>& vectorConfig, const unsigned int cuiCurrentWaitTime=4);

	//! 短路测试
	/*!
	\param[in] ucSwitch   0：测试第一路MIPI  1：测试并口	2：测试第二路MIPI
	\param[inout]   vectorConfig  配置要进行测试的PIN脚
	\param[in] cuiCurrentWaitTime 电流稳定时间：cuiCurrentWaitTime x3 (ms)
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  _HisFX3OS_Positive_Item,  HisFX3OSNegtiveTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSShortTest(unsigned char ucSwitch, std::vector<_HisFX3OS_Short_Item>& vectorConfig, const unsigned int cuiCurrentWaitTime=4);

	//! 阻值测试
	/*!
	\param[in] ucSwitch   0：测试第一路MIPI  1：测试并口	2：测试第二路MIPI
	\param[inout]   stConfig  配置要进行测试的PIN脚
	\param[in] flCurrent 测试阻值时所用的电流， 设为0.0则为1.3mA, 一般可以保持默认值
	\param[in] cuiCurrentWaitTime 电流稳定时间：cuiCurrentWaitTime x3 (ms)
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  _HisFX3OS_Positive_Item,  HisFX3OSNegtiveTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSOhmTest(unsigned char ucSwitch, _HisFX3OS_OHM_Item& stConfig, float flCurrent = 0.0f, const unsigned int cuiCurrentWaitTime=4);

	int getOSPinNameList(unsigned char ucSwitch, std::vector<std::string>& vecOSPin);
	
	//! 保留
	int HisFX3R2OSSwitch(bool bOS);

	int HisFX3R2OSCalibration(unsigned int* puiKey, _HisFX3R2_OSCal_Item* pstCal, unsigned int uiItemCount);
	int HisFX3SetOSCalData(_HisFX3R2_OSCal_Item* pstCal, unsigned int uiItemCount);
	 
	/***************************R2**********************/
	//ucGPIO0--IO2   ucGPIO1--IO3  ucGPIO2--IO1  ucGPIO3--invalid
	int HisFX3SetGPIO(unsigned char ucGPIO0=0, unsigned char ucGPIO1=0, unsigned char ucGPIO2=0, unsigned char ucGPIO3=0, unsigned int uiFlag=0xFF);
	/***************************R2**********************/
	//pucGPIO0--IO4 pucGPIO1--IO0
	int HisFX3GetGPIO(unsigned char* pucGPIO0=NULL, unsigned char* pucGPIO1=NULL, unsigned char* pucGPIO2=NULL, unsigned char* pucGPIO3=NULL, unsigned int uiFlag=0xFF);

	//! 保留
	int HisFX3GetLInfo(unsigned char* pucGeneration, unsigned char* pucApplication, \
		unsigned char* pucFactory, unsigned char* pucAuthorization, unsigned char* pucReserve1, unsigned char* pucReserve2);
	 
	//! 保留
	int HisFX3WriteOSIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiData, unsigned short usType);

	//! 保留
	int HisFX3ReadOSIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int* puiData, unsigned short usType);

	//! 保留
	int HisFX3WriteCurrentIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiData, unsigned short usType);

	//! 保留
	int HisFX3ReadCurrentI2C(unsigned char ucSlave, unsigned int uiReg, unsigned int* puiData, unsigned short usType);

/*
	int HisFX3GetContinusFrames(bool bC1, bool bC2, unsigned int& uiFrameCount, unsigned int uiBufByte1=0, unsigned char** ppBuffer1=0, unsigned int uiBufByte2=0, unsigned char** ppBuffer2=0);
*/

	int HIsFX3ReadVoltageValue(float* pflAVDD=0, float* pflDVDD=0, float* pflDOVDD=0, float* pflAF=0, float* pflVFuse=0, float* pflPOW=0, unsigned int uiFlag=0);
	int HIsFX3ReadVoltageValue_S2(float* pflAVDD=0, float* pflDVDD=0, float* pflDOVDD=0, float* pflAF=0, float* pflVFuse=0, float* pflPOW=0, unsigned int uiFlag=0);

	//! 保留
	bool isDataStreaming(unsigned int uiLoop = 5);

	//! 保留
	bool isDataStreaming_S2(unsigned int uiLoop = 5);

	//! 保留
	int HisFX3VoltMonitorPin8(float* pflVoltage, unsigned short usFlag);

	int HisFX3UpdateFPGA(wchar_t* pwstrPath);
	int HisFX3SetOffLineMode(bool bEnable, wchar_t* pwstrPath);
	bool HisFX3GetOfflineModeInfo(unsigned int* puiWidth, unsigned int* puiHeight, _HisFX3_BaylorMode* pucDataformat, unsigned int* puiFrameByte);
	int HisFX3SetOffLineMode_S2(bool bEnable, wchar_t* pwstrPath);
	bool HisFX3GetOfflineModeInfo_S2(unsigned int* puiWidth, unsigned int* puiHeight, _HisFX3_BaylorMode* pucDataformat, unsigned int* puiFrameByte);
	void getProtocalSupported(std::vector<unsigned int>& vecList, bool bChannel1 = true);
	bool isWorkCurrentSupported();
	bool isStandbyCurrentSupported(); 
	bool isOpenShortSupported(bool* bAllPin);
	bool getVoltageSupported(float& flmin, float& flmax, unsigned int uiFlag, bool bChannel1 = true);
	bool getCommunicationProtocalSupported(unsigned int uiFlag, bool bChannel1 = true);
	void getMCLKSupported(float& flmin, float& flmax);
	bool getDDRSupported();
	unsigned int getCameraCountSupported();
	_HisFX3_Platform_Type getCurrentPlatformType();

	//! 设置USB包的大小
	/*!
	\param[in] lsize   单位：Byte  范围：256*1024~4*1024*1024  注意必须是256*1024的整数倍
	\sa  getCurrentBulkTransformSize
	*/
	void setBulkTransformSize(LONG lsize);

	//! 设置当前USB包的大小
	/*!
	\return 包的大小，单位：Byte
	\sa  setBulkTransformSize
	*/
	LONG getCurrentBulkTransformSize();

	//! 获取通道1摄像头Sensor输出端帧索引
	/*!
	\return 帧索引
	\sa  getSensorFrameIndex_S2
	*/
	unsigned __int64 getSensorFrameIndex();

	//! 获取通道2摄像头Sensor输出端帧索引
	/*!
	\return 帧索引
	\sa  getSensorFrameIndex
	*/
	unsigned __int64 getSensorFrameIndex_S2();


	void testComputerCompatibility(float* pflErrorRate, float* pflErrorRead);
	void setCurrentPlatformType(_HisFX3_Platform_Type platformType);

	//! 设置USB传输速率
	/*!
	\param[in] uiDelay   范围：10~240 时钟周期， 越小速率越大
	\return 0:成功  非0:失败 请参考_HisFX3Platform_ErrorCode
	\sa  _HisFX3Platform_ErrorCode
	*/
	int setUSBSpeed(unsigned int uiDelay);


	const wchar_t* GetLastError(void);
	void ClearError(void);


	int DothinkeyINI2Rolongo(char* pstrRolongoIniPath, _HisFX3_PreviewStruct& stPreivewConfig);

	int decodeRolongoXML_A(char* pstrPath, _HisFX3_PreviewStruct& previewConfig, std::vector<_RolongoXMLOtherSectionItem>& vecPair);
	std::wstring wstrLastError;



#ifdef _HisFX3_Platform_Jigboard
	FuncOtpGetToolVersion           Jig_GetToolVersion;
	FuncOtpFreeBuffer               Jig_FreeBuffer;
	FuncOtpOpenFile                 Jig_OpenFile;
	FuncOtpSaveFile                 Jig_SaveFile;
	FuncOtpGetAdbPath               Jig_GetAdbPath;
	FuncOtpSetAdbPath               Jig_SetAdbPath;
	FuncOtpGetPhoneList             Jig_GetPhoneList;
	FuncOtpInitialize               Jig_Initialize;
	FuncOtpDispose                  Jig_Dispose;
	FuncOtpConnect                  Jig_Connect;
	FuncOtpDisconnect               Jig_Disconnect;
	FuncOtpIsConnected             Jig_IsConnected;
	FuncOtpGetVersionId             Jig_GetVersionId;
	FuncOtpGetKeyValueList          Jig_GetKeyValueList;
	FuncOtpModifySerialNumber       Jig_ModifySerialNumber;
	FuncOtpStartPreview             Jig_StartPreview;
	FuncOtpStopPreview              Jig_StopPreview;
	FuncOtpIsStartedPreview         Jig_IsStartedPreview;
	FuncOtpGetImageData             Jig_GetImageData;
	FuncOtpReadImage                Jig_ReadImage;
	FuncOtpOpenImage                Jig_OpenImage;
	FuncOtpDisplayImage             Jig_DisplayImage;
	FuncOtpCustomDisplayImage       Jig_CustomDisplayImage;
	FuncOtpSelectReload             Jig_SelectReload;
	FuncOtpUnselectReload           Jig_UnselectReload;
	FuncOtpCameraOn                 Jig_CameraOn;
	FuncOtpCameraOff                Jig_CameraOff;
	FuncOtpStart3A                  Jig_Start3A;
	FuncOtpStop3A                   Jig_Stop3A;
	FuncOtpStartAwb                 Jig_StartAwb;
	FuncOtpStopAwb                  Jig_StopAwb;
	FuncOtpStartAf                  Jig_StartAf;
	FuncOtpGetAfState               Jig_GetAfState;
	FuncOtpStartLsc                 Jig_StartLsc;
	FuncOtpStopLsc                  Jig_StopLsc;
	FuncOtpReadSetting              Jig_ReadSetting;
	FuncOtpWriteSetting             Jig_WriteSetting;
	FuncOtpReadRegister             Jig_ReadRegister;
	FuncOtpWriteRegister            Jig_WriteRegister;
	FuncOtpCalibrateOtp             Jig_CalibrateOtp;
	FuncOtpReadCalibratedOtp        Jig_ReadCalibratedOtp;
	FuncOtpWriteCalibratedOtp       Jig_WriteCalibratedOtp;
	FuncOtpReadCalibratedOtpPart    Jig_ReadCalibratedOtpPart;
	FuncOtpWriteCalibratedOtpPart   Jig_WriteCalibratedOtpPart;
	FuncOtpCalibrateGolden          Jig_CalibrateGolden;
	FuncOtpEraseEFlash              Jig_EraseEFlash;
	FuncOtpReadStatRgb              Jig_ReadStatRgb;
	 
	unsigned int Jig_m_uPhoneIndex;
#endif

	//! 保留
	void test();

	//! 保留
	bool checkSenseShieldEnabled(unsigned int uiLincenseID, unsigned char ucClient, unsigned char ucProduct);

private:
	void* classInterLayer;
	void* classRTSP;

	bool bOfflineMode;
	std::wstring wstrOfflineImagePath;
	unsigned __int64 ui64OfflineFrameIndex; 

	bool bOfflineMode_S2;
	std::wstring wstrOfflineImagePath_S2;
	unsigned __int64 ui64OfflineFrameIndex_S2; 

	//1: construct class fail

	std::string strSensorName;
	

	int HisFX3SetDataMode(unsigned char ucdataformat, unsigned int uiWidth, unsigned int uiHeight);
	int HisFX3ResetVolt();
	int getPlatformType(unsigned short usPID, unsigned short usVID, char* pstrFriendlyName, unsigned char ucDeviceIndex, unsigned int* puiKey);
	int getFPGADSN(unsigned char* ucnumber, unsigned int uiFX3SerialNumber = 0);

	bool HisFX3IsOpenLight();

	 
#ifdef _HisFX3_Platform_Jigboard
	HMODULE Jig_m_hDllHandle;
	int HisFX3OpenDevice_Jigboard(wchar_t* szYB);
	int HisFX3CloseDevice_Jigboard();
	int HisFX3StartPreview_Jigboard();
	int HisFX3StopPreview_Jigboard();
	int HisFX3GrabFrame_Jigboard(unsigned char **ppucBuffer, unsigned int uiBufBytes, unsigned __int64& ui64FramIndex);
	unsigned __int64 Jig_ui64FramIndex;
	wchar_t Jig_strIniPath[MAX_PATH];
#endif
};

//!  设置USB包的大小
/*! 当类对象析构的时候，会自动设回原来的大小
*/
class HISFX3PLATFORM_EXPORT QHisFX3PlatformAutoTransformSize
{
public:
	QHisFX3PlatformAutoTransformSize(CHisFX3Platform* classPlatform2, LONG lNewSize);
	~QHisFX3PlatformAutoTransformSize(void);

private:
	LONG lOriginalSize;
	CHisFX3Platform* classPlatform;
};

#endif

