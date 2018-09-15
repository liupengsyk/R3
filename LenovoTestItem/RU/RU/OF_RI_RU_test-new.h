#ifndef  __MOTO_RI_RU_TEST_EXPORTS__
#define __MOTO_RI_RU_TEST_EXPORTS__

typedef struct OFRelativePixInfo
{
	int xIndex;
	int yIndex;
	int Y_Value;
	double Y_Valuef;
}_OFRelativePixInfo_;

typedef struct RIValue
{
	double RI_RChannel;
	double RI_BChannel;
	double RI_GrChannel;
	double RI_GbChannel;

	double *RI_RChannelValue;
	double *RI_GrChannelValue;
	double *RI_GbChannelValue;
	double *RI_BChannelValue;
}_RI_VALUE_;

typedef struct RUValue
{
	double RU_RChannel;
	double RU_BChannel;
	double RU_GrChannel;
	double RU_GbChannel;

	double Shding_RChannel;
	double Shding_BChannel;
	double Shding_GrChannel;
	double Shding_GbChannel;

	double *RU_RChannelValue;
	double *RU_GrChannelValue;
	double *RU_GbChannelValue;
	double *RU_BChannelValue;
}_RU_VALUE_;

/*OFRIRUtest_API */void RelativeIlluminationTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,RIValue *pRIValue);
/*OFRIRUtest_API */void RelativeUniformityTest(BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, RUValue *pRUValue);
#endif