#ifndef SFRMAT
#define SFRMAT extern "C" _declspec(dllexport)

typedef struct _CSFRData
{
	double freq;
	double SFR;
}CSFRData;

SFRMAT double SFRMatbmpTest(unsigned short *pBmpBuffer, double NyquistFreq, CSFRData *SFRData, int Width, int Height);

#endif