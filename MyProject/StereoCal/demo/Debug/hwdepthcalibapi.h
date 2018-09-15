/************************************************************************
* Copyright(C), 2014-2024, Huawei Tech. Co., Ltd. 
* All rights reserved.  
* $Id: hwdepthcalibapi.h, v1.0 2014/04/01 10:00:00 liuyuanwen Exp $    
* Description��  ģ�����У׼�ӿ��ļ�
* Reference Doc��  
* Modification�� 
* History�� 

v1.0 2014/04/14 - ��ɳ��岢����
v1.1 2014/04/16 - ��HDC_Calib_InitParams�ṹ��������ָ��pOutLog��������Ϣ��ӡ      
v1.2 2014/05/12 - ���Ӹ������λ������ȡVCM Codeֵ����  
*************************************************************************/


#ifndef __HW_DEPTH_CALIBRATE_API_H__
#define __HW_DEPTH_CALIBRATE_API_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define HDC_CALIB_OTP_PARSE	(0)


//////////////////////////////////////////////////////////////
//HDC_CALIB_EXPORTS_DLL
#if defined(_MSC_VER)
#if defined(HDC_CALIB_EXPORTS_LIB)
#define HDC_CALIB_EXPORT_API 
#elif defined(HDC_CALIB_EXPORTS_DLL)
#define HDC_CALIB_EXPORT_API extern __declspec(dllexport)
#else
#define HDC_CALIB_EXPORT_API extern __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define HDC_CALIB_EXPORT_API 
#else
#define HDC_CALIB_EXPORT_API 
#endif

//////////////////////////////////////////////////////////////
// base types
typedef unsigned int		HDC_Calib_UINT32;
typedef int					HDC_Calib_INT32;
typedef char				HDC_Calib_INT8;
typedef unsigned char		HDC_Calib_UINT8;
typedef int					HDC_Calib_BOOL;

typedef float				HDC_Calib_FLOAT;
typedef double				HDC_Calib_DOUBLE;
typedef void				HDC_Calib_VOID;
typedef char *				HDC_Calib_VA_LIST;
typedef void*               HDC_CALIB_HANDLE;

#ifndef HDC_CALIB_NULL
#ifdef __cplusplus
#define HDC_CALIB_NULL    0
#else
#define HDC_CALIB_NULL    ((void *)0)
#endif
#endif

#define HDC_CALIB_OTP_BUFFER_SIZE		(600)//(8804)
#define HDC_CALIB_MAX_INPUT_IMAGE_GROUP (20)        /**< �������ͼ����*/ 
#define HDC_CALIB_MAX_PATH		        (260)		/**< ����·����󳤶� */
#define HDC_CALIB_FALSE			        (0)         /**< false */
#define HDC_CALIB_TRUE			        (1)         /**< true */

/**< ��־���� */ 
typedef enum HDC_Calib_eDbgInfo
{
	HDC_CALIB_LOG_OFF		= 0,              /**< �ر���־*/         
	HDC_CALIB_LOG_ERROR		= 1,			  /**< ��error����ֻ�ϱ�����*/ 
	HDC_CALIB_LOG_WARNING	= 2,			  /**< ��warning�����ϱ��澯��Ϣ*/
	HDC_CALIB_LOG_INFO		= 3,			  /**< ��info�����ϱ���Ҫ��Ϣ*/   
	HDC_CALIB_LOG_DEBUG		= 4               /**< ��debug���𣬿��ϱ�������Ϣ*/    
}HDC_Calib_eDbgInfo;

/**< ���У׼ģ�������� */ 
#if 1
typedef enum  HDC_Calib_eStatus
{
    HDC_CALIB_STATUS_FAILE = -1,					                        /**< ���У׼ģ�������ⲻͨ��  */
    HDC_CALIB_STATUS_OK = 0,						                        /**< ���У׼ģ��������ͨ��  */
	
	/* distance 1 */
	HDC_CALIB_STATUS_FAILE_DIST1_BEGIN,										/**<   */
	HDC_CALIB_STATUS_FAILE_DIST1_FOV,										/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_LEFT_OPTICAL_CENTER_X,						/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_LEFT_OPTICAL_CENTER_Y,						/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_RIGHT_OPTICAL_CENTER_X,					/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_RIGHT_OPTICAL_CENTER_Y,					/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_OPTICAL_CENTER_X,						    /**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_OPTICAL_CENTER_Y,						    /**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_ROTATION_X,						        /**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_ROTATION_Y,						        /**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_ROTATION_Z,						        /**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_SHIFT_X,									/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_SHIFT_Y,									/**<   */
    HDC_CALIB_STATUS_FAILE_DIST1_SHIFT_Z,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST1_DEPTH_1,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST1_DEPTH_2,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST1_DEPTH_3,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST1_END,										/**<   */

	/* distance 2 */
	HDC_CALIB_STATUS_FAILE_DIST2_BEGIN,										/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_FOV,										/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_LEFT_OPTICAL_CENTER_X,						/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_LEFT_OPTICAL_CENTER_Y,						/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_RIGHT_OPTICAL_CENTER_X,					/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_RIGHT_OPTICAL_CENTER_Y,					/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_OPTICAL_CENTER_X,						    /**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_OPTICAL_CENTER_Y,						    /**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_ROTATION_X,						        /**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_ROTATION_Y,						        /**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_ROTATION_Z,						        /**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_SHIFT_X,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_SHIFT_Y,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_SHIFT_Z,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_DEPTH_1,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_DEPTH_2,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_DEPTH_3,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST2_END,										/**<   */

	/* distance 3 */
	HDC_CALIB_STATUS_FAILE_DIST3_BEGIN,										/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_FOV,										/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_LEFT_OPTICAL_CENTER_X,						/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_LEFT_OPTICAL_CENTER_Y,						/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_RIGHT_OPTICAL_CENTER_X,					/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_RIGHT_OPTICAL_CENTER_Y,					/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_OPTICAL_CENTER_X,						    /**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_OPTICAL_CENTER_Y,						    /**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_ROTATION_X,						        /**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_ROTATION_Y,						        /**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_ROTATION_Z,						        /**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_SHIFT_X,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_SHIFT_Y,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_SHIFT_Z,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_DEPTH_1,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_DEPTH_2,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_DEPTH_3,									/**<   */
	HDC_CALIB_STATUS_FAILE_DIST3_END,										/**<   */

}HDC_Calib_eStatus;


#define HDC_CALIB_CHICK_PASS	(0)	  /* ģ����ֵ�ж�ͨ�� */
#define HDC_CALIB_CHICK_FAIL	(255) /* ģ����ֵ�жϲ�ͨ�� */

#else
typedef enum  HDC_Calib_eStatus
{
    HDC_CALIB_STATUS_FAILE = -1,					/**< ���У׼ģ�������ⲻͨ��  */
    HDC_CALIB_STATUS_OK = 0,						/**< ���У׼ģ��������ͨ��  */
}HDC_Calib_eStatus;

#endif

/**< ���У׼���̷���ֵ */ 
typedef enum  HDC_Calib_eRetFlag
{
	HDC_CALIB_RET_OK  = 0,						/**< ��������  */
	HDC_CALIB_RET_FAIL ,						/**< ���ش���  */
    HW_CALIB_RET_IMAGE_NOT_ENOUGH,				/**< ����ͼ�����в���  */
    HW_CALIB_RET_IMAGE_ERROR,				    /**< ����ͼ�������쳣  */
    HDC_CALIB_RET_NULL,							/**< ������    */
}HDC_Calib_eRetFlag;

/**< ���У׼����ͼ������ */ 
typedef enum HDC_Calib_eImageType
{
	HDC_CALIB_TYPE_JPG  = 0,					/**< ����ͼ������Ϊjpg����֧��RGB  */
	HDC_CALIB_TYPE_BMP ,						/**< ����ͼ������Ϊbmp  */
	HDC_CALIB_TYPE_NULL,						/**< ������ */
}HDC_Calib_eImageType;


/******************************************************************************
* ����:  pfnHDC_Calib_LogFun
* ����:    
*   ���У׼������־��ӡ�ص����� 
* ����:
*   pOutLog	    - [in]��������־���·����ΪNULL����default��ӡ��־
*   pszFileName - [in]�����ļ���
*   uiLine      - [in]�������� 
*   eLevel      - [in]��־�ȼ� 
*   pszMsg      - [in]��־���� 
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
typedef void(*pfnHDC_Calib_LogFun)(void *pOutLog, const HDC_Calib_INT8 *pszFileName, HDC_Calib_INT32 iLine, HDC_Calib_eDbgInfo eLevel, const HDC_Calib_INT8 *pszMsg, HDC_Calib_VA_LIST pArgs);


/******************************************************************************
* ����:  pfnHDC_Calib_DrawFun
* ����:    
*   ���У׼���̴��ڻ��ƻص����� 
* ����:
*   pWinHandle  - [in]��ͼ���򴰿ھ�� 
*   pImage		- [in]ͼ��ָ��
*   pResvArgs	- [in]����������
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
typedef void(*pfnHDC_Calib_DrawFun)(void *pWinHandle,HDC_Calib_VOID *pImage, HDC_Calib_VOID *pResvArgs);


/**< ��/��ͼ���ļ��� */
typedef struct HDC_Calib_ImageFiles
{
	HDC_Calib_INT32	 iChartAngle;					            /**< Chart��ת�Ƕ� */	
	HDC_Calib_INT8   chLeftImageName[HDC_CALIB_MAX_PATH];		/**< ��ͼ��ȫ·��  */	
	HDC_Calib_INT8   chRightImageName[HDC_CALIB_MAX_PATH];		/**< ��ͼ��ȫ·��  */		
}HDC_Calib_ImageFiles;


/**< ���У׼������� */
typedef struct HDC_Calib_InputParams
{
    HDC_Calib_UINT32            uiDistance;                                     /**< ���У׼�ľ���(��λ:cm)                        */	
	HDC_Calib_UINT32			uiImagesNum;					                /**< ͼ��������Ŀ��iImagesNum=��ͼ����=��ͼ����     */	
	HDC_Calib_ImageFiles		stImageList[HDC_CALIB_MAX_INPUT_IMAGE_GROUP];   /**< ����ͼ�������б�								*/	
}HDC_Calib_InputParams;


/**< ���У׼�������:����(ģ��/����)ʹ�� */
typedef struct HDC_Calib_Output
{
    HDC_Calib_UINT32     uiBufSize;
    HDC_Calib_UINT8     *pCalibBuffer;
}HDC_Calib_Output;

   
/***< ���У׼��ʼ������ */
typedef struct HDC_Calib_InitParams
{
    HDC_Calib_eImageType		eImageType;					        /**< ͼ������BMP,JPG����֧��RGB��*/		
    HDC_Calib_UINT8             uiDistanceNum;                      /**< ���У׼�ľ������� */	
	HDC_Calib_INT8			    chCfgName[HDC_CALIB_MAX_PATH];		/**< �����ļ�ȫ·�� */	

    HDC_Calib_eDbgInfo          eDbgLevel;                          /**<�û���������У׼��־����-20140420�Ķ�*/
    HDC_Calib_VOID              *pOutLog;                           /**<�û���������У׼��־���·��������־���-20140416�Ķ�*/
	pfnHDC_Calib_LogFun			pLogFunc;                           /**<�û���������У׼��־��ӡ�ص����� */
    pfnHDC_Calib_DrawFun		pDrawFun;				            /**<�û���������У׼���̻��ƻص����� */

}HDC_Calib_InitParams;
	

/******************************************************************************
* ����:  HDC_Calib_Create
* ����:    
*   ���У׼�������� 
* ����:
*   pstInputParams		- [in]��ʼ������
*   pCalibHandle   		- [out]���У׼���ָ��
* ����:
*   HDC_Calib_eRetFlag
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag  HDC_Calib_Create(
    HDC_Calib_InitParams  *pstInitParams,
    HDC_CALIB_HANDLE      *pCalibHandle
    ); 

/******************************************************************************
* ����:  HDC_Calib_Process
* ����:    
*   ���У׼������ 
* ����:
*   pCalibHandle        - [in]���У׼���
*   pstInputParams		- [in]�������
* ����:
*   HDC_Calib_eRetFlag
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_Process	(
    HDC_CALIB_HANDLE                pCalibHandle,
    HDC_Calib_InputParams          *pstInputParams
    );
	
/******************************************************************************
* ����:  HDC_Calib_CheckParams
* ����:    
*   ���У׼ģ��������   
* Parameters:
*   pCalibHandle        - [in]���У׼���
*   eStatus       		- [out]ģ����������
*   pOutputLog   		- [out]������������ϸ��Ϣ������ΪHDC_CALIB_MAX_PATH
* ����:
*   HDC_Calib_eRetFlag
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_CheckParams	(
    HDC_CALIB_HANDLE				pCalibHandle,
    HDC_Calib_eStatus               *pStatus,
    HDC_Calib_INT8   		        *pOutputLog
    );
    
/******************************************************************************
* ����:  HDC_Calib_GenerateOTPBuffer
* ����:    
*   ���У׼ģ��������   
* Parameters:
*   pCalibHandle        - [in]���У׼���
*   pstOTPBuffer	    - [out]��д��OTP�Ĵ����Ļ���
* ����:
*   HDC_Calib_eRetFlag
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_GenerateOTPBuffer	(
	HDC_CALIB_HANDLE				pCalibHandle,
	HDC_Calib_Output   		        *pstOTPBuffer
	);


/******************************************************************************
* ����:  HDC_Calib_ParseAndPrintCameraParams
* ����:    
*   ���У׼ģ���������   
* Parameters:
*   otpFile     - [in]otp�ļ�
*   outFile	    - [out]����ļ�
* ����:
*   HDC_Calib_INT32
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_INT32 HDC_Calib_ParseAndPrintCameraParams(
	const char *otpFile,
	const char* outFile
	);


/******************************************************************************
* ����:  HDC_Calib_Delete
* ����:    
*   ���У׼ģ��������   
* Parameters:
*   pCalibHandle        - [in]���У׼���
* ����:
*   HDC_Calib_eRetFlag
* ��ʷ��¼:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_Destory(HDC_CALIB_HANDLE	pCalibHandle);  

/******************************************************************************
* ����:  HDC_Calib_GetVCMCodeByDepth
* ����:    
*   ��ȡĳ����Ⱦ�������ֵ   
* Parameters:
*   vcm_code_7cm         - [in]7cm�Խ�λ�ô����code,0~1023,����0Ϊinfiniti,1023Ϊ����
*   vcm_code_15cm        - [in]15cm�Խ�λ�ô����code,0~1023
*   vcm_code_2000cm      - [in]2000cm�Խ�λ�ô����code,0~1023
*   distance_cm          - [in]Ŀ��Խ�λ��,��λΪcm
*   vcm_code             - [out]Ŀ��Խ�λ�ö�Ӧ�����code,0~1023
* ����:
*   HDC_Calib_eRetFlag
* ��ʷ��¼:v1.0 -2014/05/12     
*******************************************************************************/
//HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_GetVCMCodeByDepth(HDC_Calib_INT32 vcm_code_7cm, HDC_Calib_INT32 vcm_code_15cm, HDC_Calib_INT32 vcm_code_2000cm, HDC_Calib_INT32 distance_cm, HDC_Calib_INT32 *vcm_code);
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_GetVCMCodeByDepth(HDC_Calib_INT32 vcm_code_7cm, HDC_Calib_INT32 vcm_code_15cm, HDC_Calib_INT32 vcm_code_500cm, HDC_Calib_INT32 distance_cm, HDC_Calib_INT32 *vcm_code);

HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_GetVCMCodeByDepthNew(HDC_Calib_INT32 vcm_code_7cm, HDC_Calib_INT32 vcm_code_15cm, HDC_Calib_INT32 vcm_code_2000cm, 
	HDC_Calib_INT32 vcm_code_35cm, HDC_Calib_INT32 distance_cm, HDC_Calib_INT32 *vcm_code_o);


/******************************************************************************
* Function:  HDC_Calib_GetVersion
* Description:    
*   camera depth calibrate init    
* Parameters:
*   pVersion		- [in]output version params
* Return:
*   HDC_Calib_eRetFlag     
* Revision Record: 1.0       
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag  HDC_Calib_GetVersion(HDC_Calib_INT32 *pVersion);


#ifdef __cplusplus
}
#endif

#endif
