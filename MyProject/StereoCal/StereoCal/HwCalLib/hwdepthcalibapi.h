/************************************************************************
* Copyright(C), 2014-2024, Huawei Tech. Co., Ltd. 
* All rights reserved.  
* $Id: hwdepthcalibapi.h, v1.0 2014/04/01 10:00:00 liuyuanwen Exp $    
* Description：  模组深度校准接口文件
* Reference Doc：  
* Modification： 
* History： 

v1.0 2014/04/14 - 完成初稿并发布
v1.1 2014/04/16 - 在HDC_Calib_InitParams结构体中增加指针pOutLog，用于信息打印      
v1.2 2014/05/12 - 增加根据深度位置来获取VCM Code值功能  
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
#define HDC_CALIB_MAX_INPUT_IMAGE_GROUP (20)        /**< 最大输入图像组*/ 
#define HDC_CALIB_MAX_PATH		        (260)		/**< 输入路径最大长度 */
#define HDC_CALIB_FALSE			        (0)         /**< false */
#define HDC_CALIB_TRUE			        (1)         /**< true */

/**< 日志级别 */ 
typedef enum HDC_Calib_eDbgInfo
{
	HDC_CALIB_LOG_OFF		= 0,              /**< 关闭日志*/         
	HDC_CALIB_LOG_ERROR		= 1,			  /**< 打开error级别，只上报错误*/ 
	HDC_CALIB_LOG_WARNING	= 2,			  /**< 打开warning级别，上报告警消息*/
	HDC_CALIB_LOG_INFO		= 3,			  /**< 打开info级别，上报重要消息*/   
	HDC_CALIB_LOG_DEBUG		= 4               /**< 打开debug级别，可上报调试消息*/    
}HDC_Calib_eDbgInfo;

/**< 深度校准模组参数检测 */ 
#if 1
typedef enum  HDC_Calib_eStatus
{
    HDC_CALIB_STATUS_FAILE = -1,					                        /**< 深度校准模组参数检测不通过  */
    HDC_CALIB_STATUS_OK = 0,						                        /**< 深度校准模组参数检测通过  */
	
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


#define HDC_CALIB_CHICK_PASS	(0)	  /* 模组阈值判断通过 */
#define HDC_CALIB_CHICK_FAIL	(255) /* 模组阈值判断不通过 */

#else
typedef enum  HDC_Calib_eStatus
{
    HDC_CALIB_STATUS_FAILE = -1,					/**< 深度校准模组参数检测不通过  */
    HDC_CALIB_STATUS_OK = 0,						/**< 深度校准模组参数检测通过  */
}HDC_Calib_eStatus;

#endif

/**< 深度校准过程返回值 */ 
typedef enum  HDC_Calib_eRetFlag
{
	HDC_CALIB_RET_OK  = 0,						/**< 返回正常  */
	HDC_CALIB_RET_FAIL ,						/**< 返回错误  */
    HW_CALIB_RET_IMAGE_NOT_ENOUGH,				/**< 输入图像序列不够  */
    HW_CALIB_RET_IMAGE_ERROR,				    /**< 输入图像序列异常  */
    HDC_CALIB_RET_NULL,							/**< 结束符    */
}HDC_Calib_eRetFlag;

/**< 深度校准输入图像类型 */ 
typedef enum HDC_Calib_eImageType
{
	HDC_CALIB_TYPE_JPG  = 0,					/**< 输入图像类型为jpg，仅支持RGB  */
	HDC_CALIB_TYPE_BMP ,						/**< 输入图像类型为bmp  */
	HDC_CALIB_TYPE_NULL,						/**< 结束符 */
}HDC_Calib_eImageType;


/******************************************************************************
* 函数:  pfnHDC_Calib_LogFun
* 描述:    
*   深度校准过程日志打印回调函数 
* 参数:
*   pOutLog	    - [in]编码器日志输出路径，为NULL则按照default打印日志
*   pszFileName - [in]代码文件名
*   uiLine      - [in]代码行数 
*   eLevel      - [in]日志等级 
*   pszMsg      - [in]日志内容 
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
typedef void(*pfnHDC_Calib_LogFun)(void *pOutLog, const HDC_Calib_INT8 *pszFileName, HDC_Calib_INT32 iLine, HDC_Calib_eDbgInfo eLevel, const HDC_Calib_INT8 *pszMsg, HDC_Calib_VA_LIST pArgs);


/******************************************************************************
* 函数:  pfnHDC_Calib_DrawFun
* 描述:    
*   深度校准过程窗口绘制回调函数 
* 参数:
*   pWinHandle  - [in]绘图区域窗口句柄 
*   pImage		- [in]图像指针
*   pResvArgs	- [in]保留参数项
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
typedef void(*pfnHDC_Calib_DrawFun)(void *pWinHandle,HDC_Calib_VOID *pImage, HDC_Calib_VOID *pResvArgs);


/**< 左/右图像文件名 */
typedef struct HDC_Calib_ImageFiles
{
	HDC_Calib_INT32	 iChartAngle;					            /**< Chart旋转角度 */	
	HDC_Calib_INT8   chLeftImageName[HDC_CALIB_MAX_PATH];		/**< 左图像全路径  */	
	HDC_Calib_INT8   chRightImageName[HDC_CALIB_MAX_PATH];		/**< 右图像全路径  */		
}HDC_Calib_ImageFiles;


/**< 深度校准输入参数 */
typedef struct HDC_Calib_InputParams
{
    HDC_Calib_UINT32            uiDistance;                                     /**< 深度校准的距离(单位:cm)                        */	
	HDC_Calib_UINT32			uiImagesNum;					                /**< 图像数组数目，iImagesNum=左图像数=右图像数     */	
	HDC_Calib_ImageFiles		stImageList[HDC_CALIB_MAX_INPUT_IMAGE_GROUP];   /**< 左右图像数组列表								*/	
}HDC_Calib_InputParams;


/**< 深度校准输出参数:对外(模组/整机)使用 */
typedef struct HDC_Calib_Output
{
    HDC_Calib_UINT32     uiBufSize;
    HDC_Calib_UINT8     *pCalibBuffer;
}HDC_Calib_Output;

   
/***< 深度校准初始化参数 */
typedef struct HDC_Calib_InitParams
{
    HDC_Calib_eImageType		eImageType;					        /**< 图像类型BMP,JPG（仅支持RGB）*/		
    HDC_Calib_UINT8             uiDistanceNum;                      /**< 深度校准的距离数量 */	
	HDC_Calib_INT8			    chCfgName[HDC_CALIB_MAX_PATH];		/**< 配置文件全路径 */	

    HDC_Calib_eDbgInfo          eDbgLevel;                          /**<用户传入的深度校准日志级别-20140420改动*/
    HDC_Calib_VOID              *pOutLog;                           /**<用户传入的深度校准日志输出路径或者日志句柄-20140416改动*/
	pfnHDC_Calib_LogFun			pLogFunc;                           /**<用户传入的深度校准日志打印回调函数 */
    pfnHDC_Calib_DrawFun		pDrawFun;				            /**<用户传入的深度校准过程绘制回调函数 */

}HDC_Calib_InitParams;
	

/******************************************************************************
* 函数:  HDC_Calib_Create
* 描述:    
*   深度校准创建函数 
* 参数:
*   pstInputParams		- [in]初始化参数
*   pCalibHandle   		- [out]深度校准句柄指针
* 返回:
*   HDC_Calib_eRetFlag
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag  HDC_Calib_Create(
    HDC_Calib_InitParams  *pstInitParams,
    HDC_CALIB_HANDLE      *pCalibHandle
    ); 

/******************************************************************************
* 函数:  HDC_Calib_Process
* 描述:    
*   深度校准处理函数 
* 参数:
*   pCalibHandle        - [in]深度校准句柄
*   pstInputParams		- [in]输入参数
* 返回:
*   HDC_Calib_eRetFlag
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_Process	(
    HDC_CALIB_HANDLE                pCalibHandle,
    HDC_Calib_InputParams          *pstInputParams
    );
	
/******************************************************************************
* 函数:  HDC_Calib_CheckParams
* 描述:    
*   深度校准模组参数检测   
* Parameters:
*   pCalibHandle        - [in]深度校准句柄
*   eStatus       		- [out]模组参数检测结果
*   pOutputLog   		- [out]输出检测结果的详细信息，长度为HDC_CALIB_MAX_PATH
* 返回:
*   HDC_Calib_eRetFlag
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_CheckParams	(
    HDC_CALIB_HANDLE				pCalibHandle,
    HDC_Calib_eStatus               *pStatus,
    HDC_Calib_INT8   		        *pOutputLog
    );
    
/******************************************************************************
* 函数:  HDC_Calib_GenerateOTPBuffer
* 描述:    
*   深度校准模组参数输出   
* Parameters:
*   pCalibHandle        - [in]深度校准句柄
*   pstOTPBuffer	    - [out]待写入OTP寄存器的缓存
* 返回:
*   HDC_Calib_eRetFlag
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_GenerateOTPBuffer	(
	HDC_CALIB_HANDLE				pCalibHandle,
	HDC_Calib_Output   		        *pstOTPBuffer
	);


/******************************************************************************
* 函数:  HDC_Calib_ParseAndPrintCameraParams
* 描述:    
*   深度校准模组参数解析   
* Parameters:
*   otpFile     - [in]otp文件
*   outFile	    - [out]输出文件
* 返回:
*   HDC_Calib_INT32
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_INT32 HDC_Calib_ParseAndPrintCameraParams(
	const char *otpFile,
	const char* outFile
	);


/******************************************************************************
* 函数:  HDC_Calib_Delete
* 描述:    
*   深度校准模组参数输出   
* Parameters:
*   pCalibHandle        - [in]深度校准句柄
* 返回:
*   HDC_Calib_eRetFlag
* 历史记录:v1.0 -2014/04/09     
*******************************************************************************/
HDC_CALIB_EXPORT_API HDC_Calib_eRetFlag HDC_Calib_Destory(HDC_CALIB_HANDLE	pCalibHandle);  

/******************************************************************************
* 函数:  HDC_Calib_GetVCMCodeByDepth
* 描述:    
*   获取某个深度距离的马达值   
* Parameters:
*   vcm_code_7cm         - [in]7cm对焦位置处马达code,0~1023,其中0为infiniti,1023为近焦
*   vcm_code_15cm        - [in]15cm对焦位置处马达code,0~1023
*   vcm_code_2000cm      - [in]2000cm对焦位置处马达code,0~1023
*   distance_cm          - [in]目标对焦位置,单位为cm
*   vcm_code             - [out]目标对焦位置对应的马达code,0~1023
* 返回:
*   HDC_Calib_eRetFlag
* 历史记录:v1.0 -2014/05/12     
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
