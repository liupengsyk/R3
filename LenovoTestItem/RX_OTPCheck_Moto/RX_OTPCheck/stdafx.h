// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>


#define LSC_MODE_1													0	//LSC计算方式一，存储的是通道平均值
#define LSC_MODE_2													1	//LSC计算方式二，存储的是【通道平均值 * 1023 / 通道最大值】
#define LSC_MODE													LSC_MODE_1

#define MAP_OFFSET													0x0600

#define MASTER_MODULE_NAME											_T("L476F00")
#define MASTER_MODULE_STATION_CHK_ITEM_NAME							_T("L476F00OTPCheck")

#define SLAVER_MODULE_NAME											_T("OLZ0776_Slave")
#define SLAVER_MODULE_STATION_CHK_ITEM_NAME							_T("OLS0773OTPCheck_Slave")

#define Info_StartAdd		0x0000
#define Info_EndAdd			0x0026
#define AF_StartAdd			0x0027
#define AF_EndAdd			0x0030
#define AWB_StartAdd		0x0031
#define AWB_EndAdd			0x0054
#define OC_StartAdd			0x0055
#define OC_EndAdd			0x0066
#define SFR_StartAdd		0x0067

#define SFR_Macro_StartAdd	0x0067
#define SFR_Macro_EndAdd	0x0085

#define SFR_Mid_StartAdd	0x0086
#define SFR_Mid_EndAdd		0x00A4

#define SFR_Inf_StartAdd	0x00A5
#define SFR_Inf_EndAdd		0x00C3

#define SFR_EndAdd			0x00C5

#define Arcsoft_StartAdd	0x00C6
#define Arcsoft_EndAdd		0x08C7

#define AEC_StartAdd		0x08C8
#define AEC_EndAdd			0x08D3

#define LSC_StartAdd		0x08D4
#define LSC_MaxAdd			0x0FBC
#define LSC_EndAdd			0x0FBD

#define Gainmap_StartAdd	0x0FBE
#define Gainmap_EndAdd		0x133C

#define DCC_StartAdd		0x133D
#define DCC_EndAdd			0x13A4

#define CTalk_StartAdd		0x13A5
#define CTalk_EndAdd		0x17C6