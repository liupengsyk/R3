#ifndef HW_BLEMISH
#define HW_BLEMISH extern "C" _declspec(dllexport)

HW_BLEMISH bool HWBlemishTest(BYTE *pBMPBuffer,int Width,int Height,double p1,double p2);

#endif