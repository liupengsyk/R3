//#ifndef SFRMAT
#define SFRMAT extern "C" _declspec(dllexport)

typedef struct _CSFRData
{
	double freq;
	double SFR;
}CSFRData;
enum OUTFORMAT
{
	OUTFORMAT_RGGB =0,
	OUTFORMAT_GRBG,
	OUTFORMAT_GBRG,
	OUTFORMAT_BGGR
};

SFRMAT bool fChannelBalance(unsigned char *m_pBuff,OUTFORMAT outformat,int m_width,int m_height,unsigned char *pOutRaw10);
SFRMAT double SFRMatTest(BYTE *Raw10Buffer,CSFRData *SFRData,int Width,int Height);

SFRMAT double SFRMatbmpTest(unsigned short *pBmpBuffer, double NyquistFreq, CSFRData *SFRData, int Width, int Height);
//#endif