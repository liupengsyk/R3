// LSC DLL Header

#if defined(LSCDLL_EXPORTS)
#define DLLFUNC extern "C" __declspec(dllexport)
#elif defined(__cplusplus)
#define DLLFUNC extern "C" __declspec(dllimport)
#else
#define DLLFUNC __declspec(dllimport)
#endif

// Set parameters
DLLFUNC void SetParameter_5E8( int nPedestal, int nWidth, int nHeight, int nGr, int nR, 
   int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );

// Run LSC
DLLFUNC BOOL Run_5E8( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, CString strWorkDir );

// Get Buffer Sizes
DLLFUNC int GetOTPSetfileSize_5E8();
DLLFUNC int GetSRAMSetfileSize_5E8();

// Output to Files
DLLFUNC BOOL SaveOTPSetfile_5E8( char *szFile );
DLLFUNC BOOL SaveSRAMSetfile_5E8( char *szFile );

// Output to Memory Buffers
DLLFUNC BOOL GetOTPSetfile_5E8( char *szBuf );
DLLFUNC BOOL GetSRAMSetfile_5E8( char *szBuf );

DLLFUNC void SetUserLensRI_5E8( double RI_1, double RI_2, double RI_3, double RI_4,
								double RI_5, double RI_6, double RI_7, double RI_8,
								double RI_9, double RI_10, double RI_11);
// Set parameters
DLLFUNC void SetParameter( int nPedestal, int nWidth, int nHeight, int nGr, int nR, 
   int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );   //4H8

// Run LSC
DLLFUNC BOOL Run( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, CString strWorkDir, BOOL b2ndSet, BOOL *is_SINGULAR, BOOL *bSEED_overflow );

//4H8
// Get Buffer Sizes
DLLFUNC int GetOTPSetfileSize();   //4H8
DLLFUNC int GetSRAMSetfileSize();   //4H8

// Output to Files
DLLFUNC BOOL SaveOTPSetfile( char *szFile );   //4H8
DLLFUNC BOOL SaveSRAMSetfile( char *szFile );  //4H8

// Output to Memory Buffers
DLLFUNC BOOL GetOTPSetfile( char *szBuf );  //4H8
DLLFUNC BOOL GetSRAMSetfile( char *szBuf );  //4H8

DLLFUNC void SetUserLensRI( double RI_1, double RI_2, double RI_3, double RI_4,
								double RI_5, double RI_6, double RI_7, double RI_8,
								double RI_9, double RI_10, double RI_11);

//4H8
