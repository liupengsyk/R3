// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SFR_OFILM_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SFR_OFILM_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SFR_OFILM_EXPORTS
#define SFR_OFILM_API __declspec(dllexport)
#else
#define SFR_OFILM_API __declspec(dllimport)
#endif

#include <atltypes.h>

// �����Ǵ� SFR_Ofilm.dll ������
class SFR_OFILM_API CSFR_Ofilm {
public:
	CSFR_Ofilm(void);
	// TODO: �ڴ�������ķ�����
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