// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SFR_OFILM_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SFR_OFILM_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SFR_OFILM_EXPORTS
#define SFR_OFILM_API __declspec(dllexport)
#else
#define SFR_OFILM_API __declspec(dllimport)
#endif

#include <atltypes.h>

// 此类是从 SFR_Ofilm.dll 导出的
class SFR_OFILM_API CSFR_Ofilm {
public:
	CSFR_Ofilm(void);
	// TODO: 在此添加您的方法。
};

extern SFR_OFILM_API int nSFR_Ofilm;

SFR_OFILM_API int fnSFR_Ofilm(void);

#define  ROI_COUNT		12

typedef struct _SFR_INPUT
{
	BYTE * imgBuff;
	int imgWidth;
	int imgHeight;
	int dataFormat;
	int roiFov;
	int roiWidth;
	int roiHeight;
	int freq;
	int reserved;
} SFR_INPUT, *PSFR_INPUT;

typedef struct _SFR_OUTPUT
{
	CRect roiPosi;
	float roiSfrValue;
	int reserved;
} SFR_OUTPUT, *PSFR_OUTPUT;

SFR_OFILM_API int SFR_Test(SFR_INPUT inPara, PSFR_OUTPUT outPara);
SFR_OFILM_API int SFR_TestForAF(SFR_INPUT inPara, PSFR_OUTPUT outPara);