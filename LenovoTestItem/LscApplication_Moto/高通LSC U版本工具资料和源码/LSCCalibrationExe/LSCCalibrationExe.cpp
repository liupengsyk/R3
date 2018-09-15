// LSCCalibrationExe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

using namespace std;

#pragma comment(lib, "LSCCalibrationDll.lib")


extern "C" _declspec(dllimport) int LensCorrectionLib(unsigned char * pImage, int nWidth, int nHeight,
    int CFA_pattern, short R_black_offset, short Gr_black_offset,
    short Gb_black_offset, short B_black_offset, bool bMode9x7, CalibrationDataStruct *pCalData);


typedef unsigned char BYTE;

typedef enum Raw_Type
{ 
	Raw_8Bit,
	Raw_10bit_Packed,
	Raw_10bit_Unpacked,
	Raw_12bit_Unpacked,        
	NONE
}	Raw_Type; 

// Check Range
template <class T>
T CheckRange(T Value, double Min, double Max)
{
	if (Value < (T)Min)
	{
		Value = (T)Min;
	}
	else if (Value > (T)Max)
	{
		Value = (T)Max;
	}

	return Value;
}	// CheckRange()

int _tmain(int argc, _TCHAR* argv[])
{

	FILE* fm;
	long lSize;
	//int i;
	int m_imageWidth;
	int m_imageHeight;
	int RawDataFormat;
	int BayerPattern;

	BYTE * p8bit, *PTemp;

	//  int bits_shift;
	unsigned char ctmp;

#if 0
	m_imageWidth = 2592; // Customer needs to change it
	m_imageHeight = 1968; // Customer needs to change it
	RawDataFormat = Raw_8Bit; // Customer needs to change it
	fm = fopen("8bit.raw","rb"); // Customer needs to change it
	BayerPattern = GRBG_PATTERN; // Customer needs to change it
#elif 1
	//m_imageWidth = 4212; // Customer needs to change it
	//m_imageHeight = 3120; // Customer needs to change it
	//RawDataFormat = Raw_10bit_Packed; // Customer needs to change it
	//fm = fopen("10bit_Packed.raw","rb"); // Customer needs to change it
	//BayerPattern = RGGB_PATTERN;  // Customer needs to change it

    //m_imageWidth = 4212; // Customer needs to change it
	//m_imageHeight = 3120; // Customer needs to change it
	//RawDataFormat = Raw_10bit_Packed; // Customer needs to change it
	//fm = fopen("rolloff_TL84.raw","rb"); // Customer needs to change it
	//BayerPattern = BGGR_PATTERN;  // Customer needs to change it

	//m_imageWidth = 4212; // Customer needs to change it
	//m_imageHeight = 3120; // Customer needs to change it
	//RawDataFormat = Raw_10bit_Unpacked; // Customer needs to change it
	//fm = fopen("rolloff_TL84_unpacked.raw","rb"); // Customer needs to change it
	//BayerPattern = BGGR_PATTERN;  // Customer needs to change it

	m_imageWidth = 3280; // Customer needs to change it
	m_imageHeight = 2464; // Customer needs to change it
	RawDataFormat = Raw_10bit_Packed; // Customer needs to change it
	fm = fopen("10bit_Packed_Pollux.raw","rb"); // Customer needs to change it
	BayerPattern = GBRG_PATTERN;  // Customer needs to change it


	//m_imageWidth = 3280; // Customer needs to change it
	//m_imageHeight = 2464; // Customer needs to change it
	//RawDataFormat = Raw_10bit_Unpacked; // Customer needs to change it
	//fm = fopen("10bit_Packed_Pollux_unpacked.raw","rb"); // Customer needs to change it
	//BayerPattern = GBRG_PATTERN;  // Customer needs to change it

	
#elif 0
	m_imageWidth = 3268; // Customer needs to change it
	m_imageHeight = 2454; // Customer needs to change it
	RawDataFormat = Raw_12bit_Unpacked; // Customer needs to change it
	fm = fopen("12bit_Unpacked.raw","rb"); // Customer needs to change it
	BayerPattern = BGGR_PATTERN;  // Customer needs to change it
#else
	m_imageWidth = 2608; // Customer needs to change it
	m_imageHeight = 1960; // Customer needs to change it
	RawDataFormat = Raw_10bit_Unpacked; // Customer needs to change it
	fm = fopen("10bit_Unpacked 3.raw","rb"); // Customer needs to change it
	BayerPattern = GRBG_PATTERN;  // Customer needs to change it
#endif

	cout << "Starting" << endl;
	if (fm==NULL) {fputs ("File error while opening\n",stderr); exit (1);}

	fseek (fm , 0 , SEEK_END);
	lSize = ftell (fm);
	rewind (fm);

	p8bit = new BYTE[m_imageWidth * m_imageHeight];

	switch(RawDataFormat)
	{
		case Raw_8Bit:
			PTemp = new BYTE[m_imageWidth * m_imageHeight];
			if (fread(PTemp, sizeof ( BYTE ), m_imageWidth * m_imageHeight, fm) !=
				(unsigned int )(m_imageWidth * m_imageHeight))
			{
				fputs ("File error while reading\n",stderr); exit (4);
				delete [] PTemp;
				m_imageWidth = 0;
				m_imageHeight = 0;
			}
			else
			{
				memcpy(p8bit, PTemp, m_imageWidth * m_imageHeight);
			}
			break;

		case Raw_10bit_Packed:
			PTemp = new BYTE[(m_imageWidth+5)/6*m_imageHeight*8];
			if (fread(PTemp, sizeof ( BYTE ), (m_imageWidth+5)/6*m_imageHeight*8, fm) !=
				(unsigned int )((m_imageWidth+5)/6*m_imageHeight*8))
			{
				fputs ("File error while reading\n",stderr); exit (4);
				delete [] PTemp;
				m_imageWidth = 0;
				m_imageHeight = 0;
			}
			else
			{
				//for (int i=0; i<m_imageHeight*m_imageWidth/6; i++)
				//{
				//	BYTE *SBayer, *TBayer;
				//	unsigned __int64 DW_64Bayer, DWord;
				//	
				//	SBayer = (BYTE *) (PTemp + i*8);
				//	DWord = *((unsigned __int64 *) SBayer);

				//	TBayer = (BYTE *) (p8bit + i*6);

				//	DW_64Bayer = DWord;

				//	*(TBayer + 0) = (DW_64Bayer >> (2 + (0*10))) & 0xff;
				//	*(TBayer + 1) = (DW_64Bayer >> (2 + (1*10))) & 0xff;
				//	*(TBayer + 2) = (DW_64Bayer >> (2 + (2*10))) & 0xff;
				//	*(TBayer + 3) = (DW_64Bayer >> (2 + (3*10))) & 0xff;
				//	*(TBayer + 4) = (DW_64Bayer >> (2 + (4*10))) & 0xff;
				//	*(TBayer + 5) = (DW_64Bayer >> (2 + (5*10))) & 0xff;
				//} 

				//short* data = (short*) malloc(m_imageHeight*m_imageWidth*sizeof(short));
				int count = 0;

				for (int i=0; i<m_imageHeight; i++)
				{
					for(int j=0; j<m_imageWidth; j+=6)
				    {
					BYTE *SBayer, *TBayer;
					unsigned __int64 DW_64Bayer, DWord;
					
					SBayer = (BYTE *) (PTemp + count);
					DW_64Bayer = *((unsigned __int64 *) SBayer);
					count +=8;

					TBayer = (BYTE *) (p8bit + i*m_imageWidth+j);
					*(TBayer + 0) = (DW_64Bayer >> (2 + (0*10))) & 0xff;
					*(TBayer + 1) = (DW_64Bayer >> (2 + (1*10))) & 0xff;
					*(TBayer + 2) = (DW_64Bayer >> (2 + (2*10))) & 0xff;
					*(TBayer + 3) = (DW_64Bayer >> (2 + (3*10))) & 0xff;
					*(TBayer + 4) = (DW_64Bayer >> (2 + (4*10))) & 0xff;
					*(TBayer + 5) = (DW_64Bayer >> (2 + (5*10))) & 0xff;

					//data[i*m_imageWidth+j+0] = (DW_64Bayer >> (0*10)) & 0x3ff;
					//data[i*m_imageWidth+j+1] = (DW_64Bayer >> (1*10)) & 0x3ff;
					//data[i*m_imageWidth+j+2] = (DW_64Bayer >> (2*10)) & 0x3ff;
					//data[i*m_imageWidth+j+3] = (DW_64Bayer >> (3*10)) & 0x3ff;
					//data[i*m_imageWidth+j+4] = (DW_64Bayer >> (4*10)) & 0x3ff;
					//data[i*m_imageWidth+j+5] = (DW_64Bayer >> (5*10)) & 0x3ff;
					}
				} 


				/*for (int i=0; i<m_imageHeight; i++)
				{
					for(int j=0; j<m_imageWidth; j+=6)
					{

						unsigned short p0, p1, p2, p3, p4, p5;
						unsigned short p00, p11, p22, p33, p44, p55;

						p0 = ((unsigned short) PTemp[count+0]);
						p00 = ((unsigned short) (PTemp[count+1]&0x03))<<8;
						p1 = (unsigned short) (PTemp[count+1]&0xFC)>>2;
						p11 = ((unsigned short) (PTemp[count+2]&0x0F))<<6;
						p2 = ((unsigned short) (PTemp[count+2]&0xF0))>>4;
						p22 = ((unsigned short) (PTemp[count+3]&0x3F))<<4;
						p3 = ((unsigned short) (PTemp[count+3]&0xC0))>>6;
						p33 = ((unsigned short) PTemp[count+4])<<2;
						p4 = ((unsigned short) PTemp[count+5]);
						p44 = ((unsigned short) (PTemp[count+6]&0x03))<<8;
						p5 = (unsigned short) (PTemp[count+6]&0xFC)>>2;
						p55 = ((unsigned short) (PTemp[count+7]&0x0F))<<6;

						count +=8;


						*(p8bit + i*m_imageWidth+j + 0) = (unsigned char)((p0 + p00)>>2);
						*(p8bit + i*m_imageWidth+j + 1) = (unsigned char)((p1 + p11)>>2);
						*(p8bit + i*m_imageWidth+j + 2) = (unsigned char)((p2 + p22)>>2);
						*(p8bit + i*m_imageWidth+j + 3) = (unsigned char)((p3 + p33)>>2);
						*(p8bit + i*m_imageWidth+j + 4) = (unsigned char)((p4 + p44)>>2);
						*(p8bit + i*m_imageWidth+j + 5) = (unsigned char)((p5 + p55)>>2);

						data[i*m_imageWidth+j+0] = (p0 + p00);
						data[i*m_imageWidth+j+1] = (p1 + p11);
						data[i*m_imageWidth+j+2] = (p2 + p22);
						data[i*m_imageWidth+j+3] = (p3 + p33);
						data[i*m_imageWidth+j+4] = (p4 + p44);
						data[i*m_imageWidth+j+5] = (p5 + p55);
					} 
				}*/

				//FILE * fp = fopen("image_unpacked.raw", "wb");

				//fwrite(data, 2, m_imageHeight*m_imageWidth, fp);
				//fclose (fp);
				//fprintf(stderr,"Success!\n");
			}
			break;
			
		case Raw_10bit_Unpacked:
   			PTemp = new BYTE[m_imageWidth * m_imageHeight * 2];
			if (fread(PTemp, sizeof ( BYTE ), m_imageWidth * m_imageHeight * 2, fm) !=
				(unsigned int )(m_imageWidth * m_imageHeight * 2))
			{
				fputs ("File error while reading\n",stderr); exit (4);
				delete [] PTemp;
				m_imageWidth = 0;
				m_imageHeight = 0;
			}
			else
			{
#if 0 
				for(int i=0; i<m_imageWidth*m_imageHeight;i++)
				{
					BYTE *SBayer, *TBayer;
					unsigned __int16 DWord;
                                
					SBayer = (BYTE *) (PTemp + i*2);
					DWord = *((unsigned __int16 *) SBayer);
					TBayer = (BYTE *) (p8bit + i);
					*(TBayer) = (DWord >> 2 ) & 0xff;
				}
#else
				unsigned short *src_addr = (unsigned short *)PTemp;

				for (int y=0; y<m_imageHeight; y++)
				{
					for (int x=0; x< m_imageWidth; x++)
					{
						unsigned short *SBayer;
						BYTE *TBayer;

						SBayer = (src_addr + y * m_imageWidth  + x);
						TBayer = (p8bit    + y * m_imageWidth  + x);

						*TBayer = (BYTE) CheckRange((int)((*SBayer)>>2), 0, 255);
					}
				}
#endif
			}
			break;
			
		case Raw_12bit_Unpacked:
			PTemp = new BYTE[m_imageWidth * m_imageHeight * 2];
			if (fread(PTemp, sizeof ( BYTE ), m_imageWidth * m_imageHeight * 2, fm) !=
				(unsigned int )(m_imageWidth * m_imageHeight * 2))
			{
				fputs ("File error while reading\n",stderr); exit (4);
				delete [] PTemp;
				m_imageWidth = 0;
				m_imageHeight = 0;
			}		
			else
			{
				for (int i=0; i<m_imageHeight; i++)
				{
					for (int j=0; j< m_imageWidth; j++)
					{
						BYTE *SBayer, *TBayer;
						int MSB, LSB;

						SBayer = (BYTE *) (PTemp + i * m_imageWidth * 2 + j*2);
						TBayer = (BYTE *) (p8bit + i * m_imageWidth     + j  );

						MSB = *(SBayer+1) << 4;
						LSB = *(SBayer) >> 4;
						int K = (*(SBayer+1) << 4) + (*(SBayer) >> 4);

						*TBayer = (BYTE) CheckRange((int) (MSB + LSB), 0, 255);
					}
				}
			}
			break;
			
		default:
			break;
	}

	fclose(fm);

	CalibrationDataStruct CalData;
	
	CalData.R_LSC = (unsigned short int *) malloc (sizeof(unsigned short int)*17*13);
	CalData.Gr_LSC = (unsigned short int *) malloc (sizeof(unsigned short int)*17*13);
	CalData.Gb_LSC = (unsigned short int *) malloc (sizeof(unsigned short int)*17*13);
	CalData.B_LSC = (unsigned short int *) malloc (sizeof(unsigned short int)*17*13);
	CalData.AWB = (unsigned short int *) malloc (sizeof(unsigned short int)*6);




#if 0 
	int CropX_Left, CropX_Right, CropY_Top, CropY_Bottom;

	CropX_Left = 6; //3264
	CropX_Right = 6; //3264
	CropY_Top = 6;	//2454
	CropY_Bottom = 4;	//2454

	for (int i=0 + CropY_Top; i<m_imageHeight-CropY_Bottom ; i++)
	{
		for (int j=0 + CropX_Left; j<m_imageWidth-CropX_Right; j++)
			p8bit[(i-CropY_Top)*(m_imageWidth-(CropX_Left+CropX_Right)) + j-CropX_Left] = p8bit[i*m_imageWidth + j];
	}

	LensCorrectionLib((unsigned char *) p8bit, m_imageWidth-(CropX_Left+CropX_Right), m_imageHeight-(CropY_Top+CropY_Bottom),
		BayerPattern,
		16, 16, 16, 16, // black level value. needs to be 8 bit. it's always R Gr Gb B order. // Customer needs to change it
		0); // 0 : 17 x 13, 1 : 9 x 7 // Customer needs to change it
#else
	int CropY, CropX;
	CropX = 2;
	CropY = 2;
	
	for (int i=0 + CropY; i<m_imageHeight-CropY ; i++)
	{
		for (int j=0 + CropX; j<m_imageWidth-CropX; j++)
			p8bit[(i-CropY)*(m_imageWidth-CropX*2) + j-CropX] = p8bit[i*m_imageWidth + j];
	}

	bool bMode9x7 = 0; // 0 : 17 x 13, 1 : 9 x 7 // Customer needs to change it

	//LensCorrectionLib((unsigned char *) p8bit, m_imageWidth-CropX*2, m_imageHeight-CropY*2,
	//	BayerPattern,
	//	251/16, 248/16, 251/16, 249/16, // black level value. needs to be 8 bit. it's always R Gr Gb B order. // Customer needs to change it
	//	bMode9x7,								
	//	&CalData);

	LensCorrectionLib((unsigned char *) p8bit, m_imageWidth-CropX*2, m_imageHeight-CropY*2,
		BayerPattern,
		257/16, 257/16, 257/16, 257/16, // black level value. needs to be 8 bit. it's always R Gr Gb B order. // Customer needs to change it
		bMode9x7,								
		&CalData);
#endif

	{
		FILE* fb;
		FILE* fwb;	
		unsigned short int temp;

		fb = fopen("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT_Pointer.txt","wb");
		fwb = fopen("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT_Pointer.txt","wb");

		if (bMode9x7)
		{
			fwrite(CalData.R_LSC, sizeof(unsigned short int), 9*7, fb);
			fwrite(CalData.Gr_LSC, sizeof(unsigned short int), 9*7, fb);
			fwrite(CalData.Gb_LSC, sizeof(unsigned short int), 9*7, fb);
			fwrite(CalData.B_LSC, sizeof(unsigned short int), 9*7, fb);
		}
		else
		{
			fwrite(CalData.R_LSC, sizeof(unsigned short int), 17*13, fb);
			fwrite(CalData.Gr_LSC, sizeof(unsigned short int), 17*13, fb);
			fwrite(CalData.Gb_LSC, sizeof(unsigned short int), 17*13, fb);
			fwrite(CalData.B_LSC, sizeof(unsigned short int), 17*13, fb);
		}

		fwrite(CalData.AWB, sizeof(unsigned short int), 3, fwb);
	}



	free(CalData.R_LSC);
	free(CalData.Gr_LSC);
	free(CalData.Gb_LSC);
	free(CalData.B_LSC);
	free(CalData.AWB);

	return 0;
}

