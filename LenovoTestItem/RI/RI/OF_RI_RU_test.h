#ifdef  MOTO_RI_RU_TEST_EXPORTS
#define OFRIRUtest_API __declspec(dllexport)
#else
#define OFRIRUtest_API __declspec(dllimport)
#endif

struct OFRelativePixInfo
{
	int xIndex;
	int yIndex;
	int Y_Value;
	double Y_Valuef;
}_OFRelativePixInfo_;

struct RIValue
{
	double RI_RChannel;
	double RI_BChannel;
	double RI_GrChannel;
	double RI_GbChannel;
}_RI_VALUE_;

struct RUValue
{
	double RU_RChannel;
	double RU_BChannel;
	double RU_GrChannel;
	double RU_GbChannel;

	double Shding_RChannel;
	double Shding_BChannel;
	double Shding_GrChannel;
	double Shding_GbChannel;
}_RU_VALUE_;

struct RIValue_Mono
{
	double RI;
}_RI_VALUE_MONO_;

struct RUValue_Mono
{
	double RU;
	double Shding;
}_RU_VALUE_MONO_;

OFRIRUtest_API void RelativeIlluminationTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,RIValue *pRIValue);
OFRIRUtest_API void RelativeUniformityTest(BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, RUValue *pRUValue);

OFRIRUtest_API void RelativeIlluminationTest_Mono( BYTE* pRawImage, int nImgWidth, int nImgHeight,RIValue_Mono *pRIValue);
OFRIRUtest_API void RelativeUniformityTest_Mono(BYTE* pRawImage, int nImgWidth, int nImgHeight, RUValue_Mono *pRUValue);
