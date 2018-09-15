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
 
//!  ������ͼ��ɼ���װ��װ��
/*! �������ý�����WIN7����ϵͳ�� 4G�����ڴ棬 INTEL I3���ϴ�����
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
	//!  �򿪹�װ�� ��ִ����������֮�����򿪹�װ
	/*! �����ڳ���һ��ʼ��ʱ��򿪹�װ���������ʱ�رչ�װ
	\param[in] usPID USB3.0�豸PID�� �˲�����ʱ��������̶���Ϊ0
	\param[in] usVID USB3.0�豸VID�� �˲�����ʱ��������̶���Ϊ0
	\param[in] pstrFriendlyName USB3.0�豸�������ƣ� �˲�����ʱ��������̶���Ϊ0
	\param[in] ucDeviceIndex USB3.0�豸��ţ� �˲�����ʱ��������̶���Ϊ0xFF
	\param[in] puiKey ��װΨһ���кţ� ��puiKey==NULL,���������ӵ����Ե���һ��װ�� 
	                         ��puiKey��ָ�� unsigned int[4]�����ָ��ʱ�������򿪺��������к�ƥ��Ĺ�װ��
							 �˲�����Ҫ����1���������Ӷ����װ�������
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3CloseDevice()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3OpenDevice(unsigned short usPID=0, unsigned short usVID=0, char* pstrFriendlyName=NULL, unsigned char ucDeviceIndex=0xFF, unsigned int* puiKey = NULL, unsigned int uiTransformDelay = 110);

	//!  ��ȡ��װ����
	/*! 
	\return ��װ����
	*/
	unsigned int HisFX3DeviceCount();

	//!  �رչ�װ
	/*! 
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3OpenDevice()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3CloseDevice();

	//! ͨ��1��ʼ��ͼ
	/*! �����˫�㹤װ���˺�����������ͨ��1
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa _HisFX3_PreviewStruct�� HisFX3StopPreview()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3StartPreview(struct _HisFX3_PreviewStruct* pstPrama);

	//! ͨ��1ֹͣ��ͼ
	/*! �����˫�㹤װ���˺�����������ͨ��1
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3StopPreview();

	//! ͨ��2��ʼ��ͼ
	/*! ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa _HisFX3_PreviewStruct�� HisFX3StopPreview_S2()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3StartPreview_S2(struct _HisFX3_PreviewStruct* pstPrama);

	//! ͨ��2ֹͣ��ͼ
	/*! ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview_S2()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3StopPreview_S2();

	int reStartPreview(struct _HisFX3_PreviewStruct* pstPrama);

	//! ͨ��1ץͼ
	/*! �����˫�㹤װ���˺�����������ͨ��1
	\param[out] ppucBuffer ����ͼ�����ݵ�bufferָ�룬������_aligned_malloc(size, _HisCacheLine_Aligned)�����ڴ�ռ䡣
										  �������NULL, ��ֻ�᷵��ui64FramIndex��
	\param[in] uiBufBytes ppucBuffer������ڴ��С���������һ֡ͼ��������ڴ��С
	\param[out] ui64FramIndex ��ǰ֡��������ʼ��ͼ֮��ĵ�һ֡Ϊ1���Դ�����
	\param[in] uiTimeOut ��ȡ�µ�һ֡��timeoutʱ��
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3GrabFrame(unsigned char **ppucBuffer, unsigned int uiBufBytes, unsigned __int64& ui64FramIndex, unsigned int uiTimeOut = 3000);

	//! ͨ��2ץͼ
	/*! �����˫�㹤װ���˺�����������ͨ��2
	\param[out] ppucBuffer ����ͼ�����ݵ�bufferָ�룬������_aligned_malloc(size, _HisCacheLine_Aligned)�����ڴ�ռ䡣
										  �������NULL, ��ֻ�᷵��ui64FramIndex��
	\param[in] uiBufBytes ppucBuffer������ڴ��С���������һ֡ͼ��������ڴ��С
	\param[out] ui64FramIndex ��ǰ֡��������ʼ��ͼ֮��ĵ�һ֡Ϊ1���Դ�����
	\param[in] uiTimeOut ��ȡ�µ�һ֡��timeoutʱ��
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3StartPreview()�� _HisFX3Platform_ErrorCode
	*/
	int HisFX3GrabFrame_S2(unsigned char **ppucBuffer, unsigned int uiBufBytes, unsigned __int64& ui64FramIndex, unsigned int uiTimeOut = 3000);

	//! ��װ�Ƿ��Ѵ�
	/*! 
	\return true: �Ѵ�  false: ��δ��
	\sa HisFX3isstart()�� HisFX3OpenDevice, HisFX3CloseDevice
	*/
	bool HisFX3isopen();

	//! ͨ��1�������Ƿ��ѿ���
	/*! 
	\return true: �ѿ���  false: δ����
	\sa HisFX3isopen()�� HisFX3StartPreview(), HisFX3StopPreview()
	*/
	bool HisFX3isstart();

	//! ͨ��2�������Ƿ��ѿ���
	/*!  ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\return true: �ѿ���  false: δ����
	\sa HisFX3isopen()�� HisFX3StartPreview_S2(), HisFX3StopPreview_S2()
	*/
	bool HisFX3isstart_S2();

	//! ��ȡ��ǰopen��װ��Ψһ���к�
	/*!  ֻ���ڳɹ�����HisFX3OpenDevice֮����ܵ��ô˺���
	\param puiKey ָ��unsigned int[4], ��������ɹ��� �Ḵ���豸Ψһ���кŵ�������ռ�
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa getHardwareKeyList, HisFX3OpenDevice()�� HisFX3CloseDevice(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetHardwareKey(unsigned int* puiKey);

	//! ��ȡ���������й�װ��Ψһ���к�
	/*!  ֻ����HisFX3OpenDevice֮ǰ���ܵ��ô˺���
	\param[out] puiKey ָ��unsigned int, ��������ɹ��� �Ḵ���豸Ψһ���кŵ�������ռ�
	\param[out] uiKeyCount  �����ж��������кţ�һ�����к���4��unsigned int��ʾ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetHardwareKey, HisFX3OpenDevice()�� HisFX3CloseDevice(), _HisFX3Platform_ErrorCode
	*/
	int getHardwareKeyList(unsigned int* puiKeyList, unsigned int& uiKeyCount);
	
	//! ����ͨ��1 RST�ź�
	/*!  
	\param bPullHigh true: ��Ϊ�ߵ�ƽ  false:��Ϊ�͵�ƽ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PullPWND(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullReset(bool bPullHigh);

	//! ����ͨ��2 RST�ź�
	/*!    ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\param bPullHigh true: ��Ϊ�ߵ�ƽ  false:��Ϊ�͵�ƽ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PullPWND_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullReset_S2(bool bPullHigh);

	//! ����ͨ��1 PWDN�ź�
	/*!  
	\param bPullHigh true: ��Ϊ�ߵ�ƽ  false:��Ϊ�͵�ƽ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PullReset(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullPWND(bool bPullHigh);

	//! ����ͨ��2 PWDN�ź�
	/*!    ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\param bPullHigh true: ��Ϊ�ߵ�ƽ  false:��Ϊ�͵�ƽ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PullReset_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PullPWND_S2(bool bPullHigh);

	//! ����дIIC
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] ui64Data Ҫд�������
	\param[in] usType �Ĵ��������ݸ�ʽ, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] back  true: �ж�ACK  false:���ж�ACK(��ʱ���٣���������ȫ)
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3WriteIIC(unsigned char ucSlave, unsigned int uiReg, unsigned __int64 ui64Data, unsigned short usType, bool back=true);

	//! ����дͨ��2 IIC
	/*!    ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] ui64Data Ҫд�������
	\param[in] usType �Ĵ��������ݸ�ʽ, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] back  true: �ж�ACK  false:���ж�ACK(��ʱ���٣���������ȫ)
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3WriteIIC_S2(unsigned char ucSlave, unsigned int uiReg, unsigned __int64 ui64Data, unsigned short usType, bool back=true);

	//! ������IIC
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[out] pui64Data ������������
	\param[in] usType �Ĵ��������ݸ�ʽ, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3ReadI2C(unsigned char ucSlave, unsigned int uiReg, unsigned __int64* pui64Data, unsigned short usType); 

	//! ������IIC(ͨ��2)
	/*!    ֻ����˫�㹤װ�ϣ� �˺�������Ч
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[out] pui64Data ������������
	\param[in] usType �Ĵ��������ݸ�ʽ, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3ReadI2C_S2(unsigned char ucSlave, unsigned int uiReg, unsigned __int64* pui64Data, unsigned short usType); 

	//! ����д��IIC
	/*! 
	\param[in] uiCount  Ҫд���IIC������
	\param[in] pucSlave �豸��ַ����ָ��
	\param[in] puiReg �Ĵ�����ַ����ָ��
	\param[in] puiData ��������ָ��
	\param[in] pusType ��ʽ����ָ��, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay ����֮��ļ��ʱ�䣬��λ: us
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC(), HisFX3ReadIIC(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchWriteIICNoLimit(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);

	//! ����д��IIC(ͨ��2)
	/*! 
	\param[in] uiCount  Ҫд���IIC������
	\param[in] pucSlave �豸��ַ����ָ��
	\param[in] puiReg �Ĵ�����ַ����ָ��
	\param[in] puiData ��������ָ��
	\param[in] pusType ��ʽ����ָ��, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay ����֮��ļ��ʱ�䣬��λ: us
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC_S2(), HisFX3ReadIIC_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchWriteIICNoLimit_S2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);
	
	//! ������ȡIIC
	/*! 
	\param[in] uiCount  Ҫд���IIC������
	\param[in] pucSlave �豸��ַ����ָ��
	\param[in] puiReg �Ĵ�����ַ����ָ��
	\param[out] puiData ��������ָ��
	\param[in] pusType ��ʽ����ָ��, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay ����֮��ļ��ʱ�䣬��λ: us
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC(), HisFX3ReadIIC(), HisFX3BatchWriteIICNoLimit(), HisFX3PageWriteIIC(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchReadIICNoLimit(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);
	
	//! ������ȡIIC(ͨ��2)
	/*! 
	\param[in] uiCount  Ҫд���IIC������
	\param[in] pucSlave �豸��ַ����ָ��
	\param[in] puiReg �Ĵ�����ַ����ָ��
	\param[out] puiData ��������ָ��
	\param[in] pusType ��ʽ����ָ��, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632
	\param[in] usDelay ����֮��ļ��ʱ�䣬��λ: us
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteIIC_S2(), HisFX3ReadIIC_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3BatchReadIICNoLimit_S2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay=10);

	//! Pageд��IIC
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[in] pucData ��������ָ��
	\param[in] uiDataSize Ҫд������ݳ��ȣ���λ:byte
	\param[in] usDelay ÿ��BYTE��ʱusDelay *0.5��ʼ������
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned short usDelay = 4);

	//! Pageģʽд��IIC(ͨ��2)
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[in] pucData ��������ָ��
	\param[in] uiDataSize Ҫд������ݳ��ȣ���λ:byte
	\param[in] usDelay ÿ��BYTE��ʱusDelay *0.5��ʼ������
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C_S2(), HisFX3WriteI2C_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageReadIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIIC_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned short usDelay = 4);
	
	//! Pageģʽ��ȡIIC
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[out] pucData ��������ָ��
	\param[in] uiDataSize Ҫ��ȡ�����ݳ��ȣ���λ:byte
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize);
	
	//! Pageģʽ��ȡIIC(ͨ��2)
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[out] pucData ��������ָ��
	\param[in] uiDataSize Ҫ��ȡ�����ݳ��ȣ���λ:byte
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadI2C_S2(), HisFX3WriteI2C_S2(), HisFX3BatchWriteIICNoLimit_S2(), HisFX3BatchReadIICNoLimit_S2(), HisFX3PageWriteIIC_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIIC_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize);

	//! Pageд��IIC, �޳�������(ͨ��1)
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[in] pucData ��������ָ��
	\param[in] uiDataSize Ҫд������ݳ��ȣ���λ:byte
	\param[in] uiPageSize һ��Page�Ĵ�С����Ҫ����EEPROM�Ĺ���飬һ����128,256,512,1024...
	\param[in] usByteDelay ÿ��BYTE��ʱusDelay *0.5��ʼ������
	\param[in] uiPageDelay ÿ��Pageд��֮�����ʱ����λ:ms
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PageWriteIIC(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIICNoLimit(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned short usByteDelay = 4, unsigned int uiPageDelay= 100);
	
	//! Pageд��IIC, �޳�������(ͨ��2)
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[in] pucData ��������ָ��
	\param[in] uiDataSize Ҫд������ݳ��ȣ���λ:byte
	\param[in] uiPageSize һ��Page�Ĵ�С����Ҫ����EEPROM�Ĺ���飬һ����128,256,512,1024...
	\param[in] usByteDelay ÿ��BYTE��ʱusDelay *0.5��ʼ������
	\param[in] uiPageDelay ÿ��Pageд��֮�����ʱ����λ:ms
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PageWriteIIC_S2(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageReadIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteIICNoLimit_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned short usByteDelay = 4, unsigned int uiPageDelay= 100);

	//! Pageģʽ��ȡIIC, �޳�������(ͨ��1)
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[out] pucData ��������ָ��
	\param[in] uiDataSize Ҫ��ȡ�����ݳ��ȣ���λ:byte
	\param[in] uiPageSize һ��Page�Ĵ�С����Ҫ����EEPROM�Ĺ���飬һ����128,256,512,1024...
	\param[in] uiPageDelay ÿ��Page��ȡ֮�����ʱ����λ:ms
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PageReadIIC(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIICNoLimit(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned int uiPageDelay = 0);

	//! Pageģʽ��ȡIIC, �޳�������(ͨ��2)
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[out] pucData ��������ָ��
	\param[in] uiDataSize Ҫ��ȡ�����ݳ��ȣ���λ:byte
	\param[in] uiPageSize һ��Page�Ĵ�С����Ҫ����EEPROM�Ĺ���飬һ����128,256,512,1024...
	\param[in] uiPageDelay ÿ��Page��ȡ֮�����ʱ����λ:ms
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3PageReadIIC_S2(), HisFX3ReadI2C(), HisFX3WriteI2C(), HisFX3BatchWriteIICNoLimit(), HisFX3BatchReadIICNoLimit(), HisFX3PageWriteIIC(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadIICNoLimit_S2(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, unsigned int uiPageSize, unsigned int uiPageDelay = 0);

	//! Pageģʽд��SPI
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[in] pucData ��������ָ��
	\param[in] uiDataSize Ҫд������ݳ��ȣ���λ:byte
	\param[in] bCSLow ���ò���SPIʱ��CS�źŵ�ƽ�� true:�͵�ƽ�� false:�ߵ�ƽ
	\param[in] bLowFirst true: ��λ��ǰ�� false:��λ��ǰ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3PageReadSPI(), HisFX3WriteSPI(), HisFX3ReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageWriteSPI(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, bool bCSLow=true, bool bLowFirst=true);
	
	//! Pageģʽ��ȡSPI
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] uiRegSize �Ĵ�����ַ�ֽڳ��ȣ�1,2,4��ѡ
	\param[out] pucData ��������ָ��
	\param[in] uiDataSize Ҫд������ݳ��ȣ���λ:byte
	\param[in] bCSLow ���ò���SPIʱ��CS�źŵ�ƽ�� true:�͵�ƽ�� false:�ߵ�ƽ
	\param[in] bLowFirst true: ��λ��ǰ�� false:��λ��ǰ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3PageWriteSPI(), HisFX3WriteSPI(), HisFX3ReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3PageReadSPI(unsigned char ucSlave, unsigned int uiReg, unsigned int uiRegSize, unsigned char* pucData, unsigned int uiDataSize, bool bCSLow=true, bool bLowFirst=true);

	//! ����дSPI
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[in] ui64Data Ҫд�������
	\param[in] usType �Ĵ��������ݸ�ʽ, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] bCSLow ���ò���SPIʱ��CS�źŵ�ƽ�� true:�͵�ƽ�� false:�ߵ�ƽ
	\param[in] bLowFirst true: ��λ��ǰ�� false:��λ��ǰ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3ReadSPI(), HisFX3PageWriteSPI(), HisFX3PageReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3WriteSPI(unsigned char ucSlave, unsigned int uiReg, unsigned __int64 ui64Data, unsigned short usType, bool bCSLow=true, bool bLowFirst=true);
	
	//! ������ȡSPI
	/*!    
	\param[in] ucSlave �ӻ��豸��ַ
	\param[in] uiReg �Ĵ�����ַ
	\param[out] pui64Data ��ȡ������
	\param[in] usType �Ĵ��������ݸ�ʽ, eg:0x1608-��ʾ��ַλд��16bit, ����λд��8bit��
							 ֧�ֵĸ�ʽ:0x0808,0x0816,0x1608,0x1616,0x1632,0x0008,0x0016,0x0032
	\param[in] bCSLow ���ò���SPIʱ��CS�źŵ�ƽ�� true:�͵�ƽ�� false:�ߵ�ƽ
	\param[in] bLowFirst true: ��λ��ǰ�� false:��λ��ǰ
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3WriteSPI(), HisFX3PageWriteSPI(), HisFX3PageReadSPI(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3ReadSPI(unsigned char ucSlave, unsigned int uiReg, unsigned __int64* pui64Data, unsigned short usType, bool bCSLow=true, bool bLowFirst=true);

	//! ���¼���API, ���鲻ʹ��
	int HisFX3BatchWriteIIC(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short usType, unsigned short usDelay=10);
	
	//! ���¼���API, ���鲻ʹ��
	int HisFX3BatchReadIIC(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short usType, unsigned short usDelay=10);
	
	//! ���¼���API, ���鲻ʹ��
	int HisFX3BatchWriteIIC_V2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay);
	
	//! ���¼���API, ���鲻ʹ��
	int HisFX3BatchReadIIC_V2(unsigned int uiCount, unsigned char* pucSlave, unsigned int* puiReg, unsigned int* puiData, unsigned short* pusType, unsigned short usDelay);

	//! ����IICͨ���ٶ�
	/*!    
	\param[in] us1kMultiple 10KHz~1MHz��Χ�ɵ�
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetIICSpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetIICSpeed(unsigned short us1kMultiple);

	//! ����IICͨ���ٶ�(ͨ��2)
	/*!    
	\param[in] us1kMultiple 10KHz~1MHz��Χ�ɵ�
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetIICSpeed_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetIICSpeed_S2(unsigned short us1kMultiple);

	//! ��ȡ��ǰIICͨ���ٶ�
	/*!    
	\param[out] pus1kMultiple ��λ��KHz
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetIICSpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetIICSpeed(unsigned short* pus1kMultiple);

	//! ��ȡ��ǰIICͨ���ٶ�(ͨ��2)
	/*!    
	\param[out] pus1kMultiple ��λ��KHz
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetIICSpeed_S2(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetIICSpeed_S2(unsigned short* pus1kMultiple);

	//! ����SPIͨ���ٶ�
	/*!    
	\param[in] us1kMultiple 10KHz~25MHz��Χ�ɵ�, ��λ: KHz
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetSPISpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetSPISpeed(unsigned short us1kMultiple);

	//! ��ȡ��ǰSPIͨ���ٶ�
	/*!    
	\param[out] pus1kMultiple ��λ��KHz
	\return 0:�ɹ�  ��0:ʧ�ܵĴ������, ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetSPISpeed(), _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetSPISpeed(unsigned short* pus1kMultiple);

	//! ���ͨ��1�Ĵ��豸��ַ��Ӧ
	/*!    
	\param[in] ucSlave ���豸���豸��ַ
	\return 0:����Ӧ  ��0:δ��Ӧ, ��ο�_HisFX3Platform_ErrorCode
	\sa _HisFX3Platform_ErrorCode
	*/
	int HisFX3SlaveResponds(unsigned char ucSlave);

	//! ���ͨ��2�Ĵ��豸��ַ��Ӧ
	/*!    
	\param[in] ucSlave ���豸���豸��ַ
	\return 0:����Ӧ  ��0:δ��Ӧ, ��ο�_HisFX3Platform_ErrorCode
	\sa _HisFX3Platform_ErrorCode
	*/
	int HisFX3SlaveResponds_S2(unsigned char ucSlave);

	//! ����
	int HisFX3GetMIPIDataType(_HisFX3_ImageSenor_DataType* pDataType);

	//! ����5V12V��ѹ�����R5��
	/*!    
	\param[in] uiV5 0��5V�ر�  1��5V��  2��5V������
	\param[in] uiV12 0��12V�ر�  1��12V��  2��12V������
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetVolt, HisFX3SetVolt_S2, _HisFX3Platform_ErrorCode
	*/
	int HisFX3Set5V12V(unsigned int uiV5 = 2, unsigned int uiV12 = 2);

	//! ����ͨ��1��ѹ
	/*!    
	\param[in] flDOVDD 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flDOVDD *flMultiple
	\param[in] flAVDD 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flAVDD *flMultiple
	\param[in] flDVDD 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flDVDD *flMultiple
	\param[in] flAF 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flAF *flMultiple
	\param[in] flVFuse 0.0V~10.0V��Χ�ɵ�, ʵ�������ѹ= flVFuse *flMultiple
	\param[in] flPOW 0.0V~3.5V��Χ�ɵ���R5����R3X����R5X��, ʵ�������ѹ= flPOW *flMultiple
	\param[in] flIODD 0.0V~3.5V��Χ�ɵ����忨�ڲ�IO��ƽ����R5��, ʵ�������ѹ= flIODD *flMultiple
	\param[in] usSwitch ���õ�ѹ���أ� _HisFX3_Platform_VoltFlag��
									  eg,����AVDD��DVDD��ѹ�� _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flMultiple ��ѹֵϵ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3Set5V12V, HisFX3SetVolt_S2, HisFX3GetVolt, _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetVolt(float flDOVDD=0.0f, float flAVDD=0.0f, float flDVDD=0.0f, float flAF = 0.0f, float flVFuse=0.0f, float flPOW=0.0f, float flIODD = 0.0f, unsigned short usSwitch=0xFF, float flMultiple = 0.1f);
	
	//! ����ͨ��2��ѹ
	/*!    
	\param[in] flDOVDD 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flDOVDD *flMultiple
	\param[in] flAVDD 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flAVDD *flMultiple
	\param[in] flDVDD 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flDVDD *flMultiple
	\param[in] flAF 0.0V~3.5V��Χ�ɵ�, ʵ�������ѹ= flAF *flMultiple
	\param[in] flVFuse 0.0V~10.0V��Χ�ɵ�, ʵ�������ѹ= flVFuse *flMultiple
	\param[in] flPOW 0.0V~3.5V��Χ�ɵ���R5��, ʵ�������ѹ= flPOW *flMultiple
	\param[in] flIODD 0.0V~3.5V��Χ�ɵ����忨�ڲ�IO��ƽ����R5��, ʵ�������ѹ= flIODD *flMultiple
	\param[in] usSwitch ���õ�ѹ���أ� _HisFX3_Platform_VoltFlag��
									  eg,����AVDD��DVDD��ѹ�� _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flMultiple ��ѹֵϵ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3Set5V12V, HisFX3SetVolt, HisFX3GetVolt_S2, _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetVolt_S2(float flDOVDD=0, float flAVDD=0, float flDVDD=0, float flAF = 0, float flVFuse=0, float flPOW=0, float flIODD = 0, unsigned short usSwitch=0xFF, float flMultiple=0.1f);

	//! ��ȡͨ��1��ǰ���õ�ѹ
	/*!    
	\param[out] pflDOVDD ��ǰDOVDD���õ�ѹ
	\param[out] pflAVDD ��ǰAVDD���õ�ѹ
	\param[out] pflDVDD ��ǰDVDD���õ�ѹ
	\param[out] pflAF ��ǰAF���õ�ѹ
	\param[out] pflVFuse ��ǰVFuse���õ�ѹ
	\param[out] pflPOW ��ǰPOW���õ�ѹ
	\param[out] pflIODD ��ǰIODD���õ�ѹ
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3SetVolt,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetVolt(float* pflDOVDD=NULL, float* pflAVDD=NULL, float* pflDVDD=NULL, float* pflAF=NULL, float* pflVFuse=NULL, float* pflPOW=NULL, float* pflIODD=NULL);

	//! ��ȡͨ��2��ǰ���õ�ѹ
	/*!    
	\param[out] pflDOVDD ��ǰDOVDD���õ�ѹ
	\param[out] pflAVDD ��ǰAVDD���õ�ѹ
	\param[out] pflDVDD ��ǰDVDD���õ�ѹ
	\param[out] pflAF ��ǰAF���õ�ѹ
	\param[out] pflVFuse ��ǰVFuse���õ�ѹ
	\param[out] pflPOW ��ǰPOW���õ�ѹ
	\param[out] pflIODD ��ǰIODD���õ�ѹ
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3SetVolt_S2,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetVolt_S2(float* pflDOVDD=NULL, float* pflAVDD=NULL, float* pflDVDD=NULL, float* pflAF=NULL, float* pflVFuse=NULL, float* pflPOW=NULL, float* pflIODD=NULL);

	//! ����
	int HisFX3SetVFuseVolt(unsigned char ucVFUSE);

	//! ����
	int HisFX3GetVFuseVolt(float* pflVFUSE = NULL);

	//! ����ͨ��1 MCLKʱ��
	/*!    
	\param[in] flMCLK 0~136MHz�ɵ��� ��λ: MHz
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3GetMCLK,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetMCLK(float flMCLK);

	//! ����ͨ��2 MCLKʱ��
	/*!    
	\param[in] flMCLK 0~136MHz�ɵ��� ��λ: MHz
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3GetMCLK_S2,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetMCLK_S2(float flMCLK);

	//! ��ȡͨ��1��ǰMCLK����ʱ��
	/*!    
	\param[out] pflMCLK ��λ: MHz
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3SetMCLK,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetMCLK(float* pflMCLK);

	//! ��ȡͨ��2��ǰMCLK����ʱ��
	/*!    
	\param[out] pflMCLK ��λ: MHz
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3SetMCLK_S2,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetMCLK_S2(float* pflMCLK);

	//! ����ͨ��1 Sensor���ݸ�ʽ
	/*! 
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetMIPILane, _HisFX3_MIPILane_Stream
	*/
	int HisFX3SetMIPILane(unsigned char ucLane);

	//! ����ͨ��2 Sensor���ݸ�ʽ
	/*! 
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetMIPILane, _HisFX3_MIPILane_Stream
	*/
	int HisFX3SetMIPILane_S2(unsigned char ucLane);

	//! ��ȡ��ǰ���õ�ͨ��1 Sensor���ݸ�ʽ
	/*! 
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetMIPILane, _HisFX3_MIPILane_Stream
	*/
	int HisFX3GetMIPILane(unsigned char* pucLane);	

	//! ��ȡ��ǰ���õ�ͨ��2 Sensor���ݸ�ʽ
	/*! 
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetMIPILane_S2, _HisFX3_MIPILane_Stream
	*/
	int HisFX3GetMIPILane_S2(unsigned char* pucLane);

	//! ���ò���VS, HSͬ���ź�
	/*! һ��ֻ��Ҫ����VS�źţ� HS�źű���Ĭ��
	\param[in] bDVP_VS_ActiveHigh VS�źţ� true:�ߵ�ƽ������Ч�� false: �͵�ƽ������Ч
	\param[in] bDVP_HS_ActiveHigh HS�źţ� true:�ߵ�ƽ������Ч�� false: �͵�ƽ������Ч
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3GetDVPHSVS, _HisFX3Platform_ErrorCode
	*/
	int HisFX3SetDVPHSVS(bool bDVP_VS_ActiveHigh=false, bool bDVP_HS_ActiveHigh=true);

	//! ��ȡ��ǰ���õĲ���VS, HSͬ���ź�
	/*! 
	\param[out] pbDVP_VS_ActiveHigh VS�ź�
	\param[out] pbDVP_HS_ActiveHigh HS�ź�
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa HisFX3SetDVPHSVS, _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetDVPHSVS(bool* pbDVP_VS_ActiveHigh=NULL, bool* pbDVP_HS_ActiveHigh=NULL);

	//! ����
	int HisFX3DVPPCLKPhaseShift(short sCountPs18);

	//! ��ȡ��װ�̼��汾��
	/*!    
	\param[out] puiversion �汾��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa   HisFX3OpenDevice,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetHardwareVersion(unsigned int* puiversion);

	//! ����
	int HisFX3GetHardGeneration(unsigned int* puiGernation);

	int HisFX3CurrentCalibration(unsigned int* puiKey, _HisFX3_Current_Calibration* pstCal);
	int HisFX3CurrentCalibration_S2(unsigned int* puiKey, _HisFX3_Current_Calibration* pstCal);

	int HisFX3SetCurrentCalData(_HisFX3_Current_Calibration* pstCal);
	int HisFX3SetCurrentCalData_S2(_HisFX3_Current_Calibration* pstCal);

	//! ����ͨ��1��������
	/*!    
	\param[out] flAVDD ���ص�AVDD����
	\param[out] flDVDD ���ص�DVDD����
	\param[out] flDOVDD ���ص�DOVDD����
	\param[out] flAF ���ص�AF����
	\param[out] flVFuse ���ص�VFuse����
	\param[out] flPOW ���ص�POW������R5����R3X����R5X��
	\param[in] uiFlag ����Ҫ����Щ·�ĵ���ֵ�� _HisFX3_Platform_VoltFlag��
									 eg,Ҫ��AVDD��DVDD������ _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flAVDD_Cal ���������¼�����Ҫ��
	\param[in] flDVDD_Cal ���������¼�����Ҫ��
	\param[in] flDOVDD_Cal ���������¼�����Ҫ��
	\param[in] flAF_Cal ���������¼�����Ҫ��
	\param[in] flVFuse_Cal ���������¼�����Ҫ��
	\param[in] bScale ���������¼�����Ҫ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa   HisFX3GetStandbyCurrent,  HisFX3GetWorkCurrent_S2, HisFX3GetStandbyCurrent_S2, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetWorkCurrent(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flVFuse, float& flPOW, unsigned int uiFlag=0xFF, \
		float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, float flVFuse_Cal=0.0f, bool bScale = true);

	//! ����ͨ��2��������
	/*!    
	\param[out] flAVDD ���ص�AVDD����
	\param[out] flDVDD ���ص�DVDD����
	\param[out] flDOVDD ���ص�DOVDD����
	\param[out] flAF ���ص�AF����
	\param[out] flVFuse ���ص�VFuse����
	\param[out] flPOW ���ص�POW������R5����R3X����R5X��
	\param[in] uiFlag ����Ҫ����Щ·�ĵ���ֵ�� _HisFX3_Platform_VoltFlag��
									 eg,Ҫ��AVDD��DVDD������ _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] flAVDD_Cal ���������¼�����Ҫ��
	\param[in] flDVDD_Cal ���������¼�����Ҫ��
	\param[in] flDOVDD_Cal ���������¼�����Ҫ��
	\param[in] flAF_Cal ���������¼�����Ҫ��
	\param[in] flVFuse_Cal ���������¼�����Ҫ��
	\param[in] bScale ���������¼�����Ҫ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa   HisFX3GetStandbyCurrent,  HisFX3GetWorkCurrent, HisFX3GetStandbyCurrent_S2, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetWorkCurrent_S2(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flVFuse, float& flPOW, unsigned int uiFlag=0xFF, \
		float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, float flVFuse_Cal=0.0f, bool bScale = true);
	
	//! ����ͨ��1��̬����
	/*!    
	\param[out] flAVDD ���ص�AVDD����
	\param[out] flDVDD ���ص�DVDD����
	\param[out] flDOVDD ���ص�DOVDD����
	\param[out] flAF ���ص�AF����
	\param[out] flPOW ���ص�POW������R5����R3X����R5X��
	\param[in] uiFlag ����Ҫ����Щ·�ĵ���ֵ�� _HisFX3_Platform_VoltFlag��
									 eg,Ҫ��AVDD��DVDD������ _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] bPWND	true:��PWND�������ģʽ��  false:������PWND�ź�
	\param[in] bReset	true:��RST�������ģʽ��  false:������RST�ź�
	\param[in] bMCLK	true:MCLK��Ϊ0MHz�������ģʽ��  false:�����MCLKʼ����Ϊ0MHz
	\param[in] bDVDD	true:DVDD��Ϊ0V�������ģʽ��  false:�����DVDD����0V
	\param[in] uiDelay	�������ģʽ����ʱuiDelay ms֮���ٲ��������
	\param[in] flAVDD_Cal ���������¼�����Ҫ��
	\param[in] flDVDD_Cal ���������¼�����Ҫ��
	\param[in] flDOVDD_Cal ���������¼�����Ҫ��
	\param[in] flAF_Cal ���������¼�����Ҫ��
	\param[in] ucAVDDGrade ���������¼�����Ҫ��
	\param[in] ucDVDDGrade ���������¼�����Ҫ��
	\param[in] ucDOVDDGrade ���������¼�����Ҫ��
	\param[in] ucAFGrade ���������¼�����Ҫ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa   HisFX3GetWorkCurrent,  HisFX3GetWorkCurrent_S2, HisFX3GetStandbyCurrent_S2, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetStandbyCurrent(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flPOW, unsigned int uiFlag=0xFF, \
		bool bPWND=true, bool bReset=true,  bool bMCLK=true, bool bDVDD=false, unsigned int uiDelay=5000, float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, \
		float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, unsigned char ucAVDDGrade=2, unsigned char ucDVDDGrade=2, unsigned char ucDOVDDGrade=2, unsigned char ucAFGrade=2);

	//! ����ͨ��2��̬����
	/*!    
	\param[out] flAVDD ���ص�AVDD����
	\param[out] flDVDD ���ص�DVDD����
	\param[out] flDOVDD ���ص�DOVDD����
	\param[out] flAF ���ص�AF����
	\param[out] flPOW ���ص�POW������R5����R3X����R5X��
	\param[in] uiFlag ����Ҫ����Щ·�ĵ���ֵ�� _HisFX3_Platform_VoltFlag��
									 eg,Ҫ��AVDD��DVDD������ _HisFX3_Platform_VlotOn_AVDD | _HisFX3_Platform_VlotOn_DVDD
	\param[in] bPWND	true:��PWND�������ģʽ��  false:������PWND�ź�
	\param[in] bReset	true:��RST�������ģʽ��  false:������RST�ź�
	\param[in] bMCLK	true:MCLK��Ϊ0MHz�������ģʽ��  false:�����MCLKʼ����Ϊ0MHz
	\param[in] bDVDD	true:DVDD��Ϊ0V�������ģʽ��  false:�����DVDD����0V
	\param[in] uiDelay	�������ģʽ����ʱuiDelay ms֮���ٲ��������
	\param[in] flAVDD_Cal ���������¼�����Ҫ��
	\param[in] flDVDD_Cal ���������¼�����Ҫ��
	\param[in] flDOVDD_Cal ���������¼�����Ҫ��
	\param[in] flAF_Cal ���������¼�����Ҫ��
	\param[in] ucAVDDGrade ���������¼�����Ҫ��
	\param[in] ucDVDDGrade ���������¼�����Ҫ��
	\param[in] ucDOVDDGrade ���������¼�����Ҫ��
	\param[in] ucAFGrade ���������¼�����Ҫ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa   HisFX3GetStandbyCurrent,  HisFX3GetWorkCurrent_S2, HisFX3GetWorkCurrent, _HisFX3_Platform_VoltFlag,  _HisFX3Platform_ErrorCode
	*/
	int HisFX3GetStandbyCurrent_S2(float& flAVDD, float& flDVDD, float& flDOVDD, float& flAF, float& flPOW, unsigned int uiFlag=0xFF, \
		bool bPWND=true, bool bReset=true,  bool bMCLK=true, bool bDVDD=false, unsigned int uiDelay=5000, float flAVDD_Cal=0.0f, float flDVDD_Cal=0.0f, \
		float flDOVDD_Cal=0.0f, float flAF_Cal=0.0f, unsigned char ucAVDDGrade=2, unsigned char ucDVDDGrade=2, unsigned char ucDOVDDGrade=2, unsigned char ucAFGrade=2);
	
	int HisFX3VlotageCalibration(unsigned int* puiKey, _HisFX3_Voltage_Calibration* pstCal);
	int HisFX3VlotageCalibration_S2(unsigned int* puiKey, _HisFX3_Voltage_Calibration* pstCal);
	int HisFX3SetVoltageCalData(_HisFX3_Voltage_Calibration* pstCal);
	int HisFX3SetVoltageCalData_S2(_HisFX3_Voltage_Calibration* pstCal);

	//! ����Եؿ�·����
	/*!
	\param[in] ucSwitch   0�����Ե�һ·MIPI  1�����Բ���	2�����Եڶ�·MIPI
	\param[inout]   vectorConfigconst  ����Ҫ���п�·���Ե�PIN��
	\param[in] cuiCurrentWaitTime �����ȶ�ʱ�䣺cuiCurrentWaitTime x3 (ms)
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  HisFX3OSNegtiveTest,  HisFX3OSShortTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSPositiveTest(unsigned char ucSwitch, std::vector<_HisFX3OS_Positive_Item>& vectorConfigconst, const unsigned int cuiCurrentWaitTime=4);

	//! �����DOVDD��·����
	/*!
	\param[in] ucSwitch   0�����Ե�һ·MIPI  1�����Բ���	2�����Եڶ�·MIPI
	\param[inout]   vectorConfig  ����Ҫ���в��Ե�PIN��
	\param[in] cuiCurrentWaitTime �����ȶ�ʱ�䣺cuiCurrentWaitTime x3 (ms)
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  _HisFX3OS_Positive_Item,  HisFX3OSShortTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSNegtiveTest(unsigned char ucSwitch, std::vector<_HisFX3OS_Negtive_Item>& vectorConfig, const unsigned int cuiCurrentWaitTime=4);

	//! ��·����
	/*!
	\param[in] ucSwitch   0�����Ե�һ·MIPI  1�����Բ���	2�����Եڶ�·MIPI
	\param[inout]   vectorConfig  ����Ҫ���в��Ե�PIN��
	\param[in] cuiCurrentWaitTime �����ȶ�ʱ�䣺cuiCurrentWaitTime x3 (ms)
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  _HisFX3OS_Positive_Item,  HisFX3OSNegtiveTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSShortTest(unsigned char ucSwitch, std::vector<_HisFX3OS_Short_Item>& vectorConfig, const unsigned int cuiCurrentWaitTime=4);

	//! ��ֵ����
	/*!
	\param[in] ucSwitch   0�����Ե�һ·MIPI  1�����Բ���	2�����Եڶ�·MIPI
	\param[inout]   stConfig  ����Ҫ���в��Ե�PIN��
	\param[in] flCurrent ������ֵʱ���õĵ����� ��Ϊ0.0��Ϊ1.3mA, һ����Ա���Ĭ��ֵ
	\param[in] cuiCurrentWaitTime �����ȶ�ʱ�䣺cuiCurrentWaitTime x3 (ms)
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
	\sa  _HisFX3OS_Positive_Item,  HisFX3OSNegtiveTest, HisFX3OSOhmTest, _HisFX3Platform_ErrorCode
	*/
	int HisFX3OSOhmTest(unsigned char ucSwitch, _HisFX3OS_OHM_Item& stConfig, float flCurrent = 0.0f, const unsigned int cuiCurrentWaitTime=4);

	int getOSPinNameList(unsigned char ucSwitch, std::vector<std::string>& vecOSPin);
	
	//! ����
	int HisFX3R2OSSwitch(bool bOS);

	int HisFX3R2OSCalibration(unsigned int* puiKey, _HisFX3R2_OSCal_Item* pstCal, unsigned int uiItemCount);
	int HisFX3SetOSCalData(_HisFX3R2_OSCal_Item* pstCal, unsigned int uiItemCount);
	 
	/***************************R2**********************/
	//ucGPIO0--IO2   ucGPIO1--IO3  ucGPIO2--IO1  ucGPIO3--invalid
	int HisFX3SetGPIO(unsigned char ucGPIO0=0, unsigned char ucGPIO1=0, unsigned char ucGPIO2=0, unsigned char ucGPIO3=0, unsigned int uiFlag=0xFF);
	/***************************R2**********************/
	//pucGPIO0--IO4 pucGPIO1--IO0
	int HisFX3GetGPIO(unsigned char* pucGPIO0=NULL, unsigned char* pucGPIO1=NULL, unsigned char* pucGPIO2=NULL, unsigned char* pucGPIO3=NULL, unsigned int uiFlag=0xFF);

	//! ����
	int HisFX3GetLInfo(unsigned char* pucGeneration, unsigned char* pucApplication, \
		unsigned char* pucFactory, unsigned char* pucAuthorization, unsigned char* pucReserve1, unsigned char* pucReserve2);
	 
	//! ����
	int HisFX3WriteOSIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiData, unsigned short usType);

	//! ����
	int HisFX3ReadOSIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int* puiData, unsigned short usType);

	//! ����
	int HisFX3WriteCurrentIIC(unsigned char ucSlave, unsigned int uiReg, unsigned int uiData, unsigned short usType);

	//! ����
	int HisFX3ReadCurrentI2C(unsigned char ucSlave, unsigned int uiReg, unsigned int* puiData, unsigned short usType);

/*
	int HisFX3GetContinusFrames(bool bC1, bool bC2, unsigned int& uiFrameCount, unsigned int uiBufByte1=0, unsigned char** ppBuffer1=0, unsigned int uiBufByte2=0, unsigned char** ppBuffer2=0);
*/

	int HIsFX3ReadVoltageValue(float* pflAVDD=0, float* pflDVDD=0, float* pflDOVDD=0, float* pflAF=0, float* pflVFuse=0, float* pflPOW=0, unsigned int uiFlag=0);
	int HIsFX3ReadVoltageValue_S2(float* pflAVDD=0, float* pflDVDD=0, float* pflDOVDD=0, float* pflAF=0, float* pflVFuse=0, float* pflPOW=0, unsigned int uiFlag=0);

	//! ����
	bool isDataStreaming(unsigned int uiLoop = 5);

	//! ����
	bool isDataStreaming_S2(unsigned int uiLoop = 5);

	//! ����
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

	//! ����USB���Ĵ�С
	/*!
	\param[in] lsize   ��λ��Byte  ��Χ��256*1024~4*1024*1024  ע�������256*1024��������
	\sa  getCurrentBulkTransformSize
	*/
	void setBulkTransformSize(LONG lsize);

	//! ���õ�ǰUSB���Ĵ�С
	/*!
	\return ���Ĵ�С����λ��Byte
	\sa  setBulkTransformSize
	*/
	LONG getCurrentBulkTransformSize();

	//! ��ȡͨ��1����ͷSensor�����֡����
	/*!
	\return ֡����
	\sa  getSensorFrameIndex_S2
	*/
	unsigned __int64 getSensorFrameIndex();

	//! ��ȡͨ��2����ͷSensor�����֡����
	/*!
	\return ֡����
	\sa  getSensorFrameIndex
	*/
	unsigned __int64 getSensorFrameIndex_S2();


	void testComputerCompatibility(float* pflErrorRate, float* pflErrorRead);
	void setCurrentPlatformType(_HisFX3_Platform_Type platformType);

	//! ����USB��������
	/*!
	\param[in] uiDelay   ��Χ��10~240 ʱ�����ڣ� ԽС����Խ��
	\return 0:�ɹ�  ��0:ʧ�� ��ο�_HisFX3Platform_ErrorCode
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

	//! ����
	void test();

	//! ����
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

//!  ����USB���Ĵ�С
/*! �������������ʱ�򣬻��Զ����ԭ���Ĵ�С
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

