#ifndef _TEST_DEFINE_H

#define MAX_SENSOR     500
#define MTF_COUNTER    14
#define IDCheck        0x00
#define SensorID       0x73
#define SensorInfo     "OV7673"

#define _TEST_DEFINE_H

#define WM_GETDNPINDEX			WM_USER + 0x31
#define WM_DEL_DNPINDEX_DLG     WM_USER + 0x32
#define WM_DEL_DEBUG_DLG        WM_USER + 0X33
#define WM_DEL_OPTION_DLG       WM_USER + 0x34
	
#define WM_UPDATE_DATA          WM_USER + 0x35
#define WM_LOADPARA             WM_USER + 0x36
#define WM_SAVEPARA             WM_USER + 0x37
#define WM_SAVEDATA             WM_USER + 0x38
#define WM_DEL_PROTERTY_DLG     WM_USER + 0X39
#define WM_STARTTEST            WM_USER + 0x3a
#define WM_CHANGESELECTEDITEM   WM_USER + 0x3b

#define WM_AF_MIN               WM_USER + 0x3c
#define WM_AF_MAX               WM_USER + 0x3d
#define WM_AF_AUTO              WM_USER + 0x3e
#define WM_AF_CHECK             WM_USER + 0x3f

#define WM_DISP_UL              WM_USER + 0x40
#define WM_DISP_UR              WM_USER + 0x41
#define WM_DISP_DL              WM_USER + 0x42
#define WM_DISP_DR              WM_USER + 0x43
#define WM_DISP_CENTER          WM_USER + 0x44

#define WM_PLAY_VIDEO           WM_USER + 0x45
#define WM_STOP_VIEDO           WM_USER + 0x46

#define WM_OTP_DATA             WM_USER + 0x47
#define WM_OTP_WRITE            WM_USER + 0x48
#define WM_OTP_READ             WM_USER + 0x49

#define WM_PINCHECK_RESET            WM_USER + 0x50


//define ap's function.is focus or test...
#define TEST_APP      0
#define FOCUS_APP     1

#ifndef FOCUS_COMPILE
	#define CURAPP  TEST_APP 
#else
	#define CURAPP  FOCUS_APP 
#endif
	
#define PARTICLE_BLOCK_X       16
#define PARTICLE_BLOCK_Y       16

#define PARTICLE_BREAKVALUE     6
#define PARTICLE_MAXCOUNT      100
#define INFOSTRING_NUM         14

#define PARTICLE_SAMEDISTANCE  20

#define DISPLAYWIDTH           PARTICLE_BLOCK_X
#define DISPLAYHEIGHT          PARTICLE_BLOCK_Y

#define B_OFFSET               0
#define G_OFFSET               1
#define R_OFFSET               2

#define ODD(i)                 (  (i) & 1)
#define EVEN(i)                (!((i) & 1))

#define AUTOPOS_WIDTH          160
#define AUTOPOS_HEIGHT         120

enum FOCUSSTATE
{
	FOCUS_START = 0,
	FOCUS_UP,
	FOCUS_EQUAL,
	FOCUS_DOWN,
	FOCUS_BACK,
	FOCUS_OVER,
	FOCUS_STOP,

};

enum RUNMODE
{
	RUNMODE_PLAY = 0,
	RUNMODE_PAUSE,
	RUNMODE_STOP,
};

enum DISPLAYMODE
{
	DISPLAY_25 = 0,
	DISPLAY_50,
	DISPLAY_75,
	DISPLAY_100,
	DISPLAY_FULL,
	DISPLAY_FIT ,
	DISPLAY_1X5, 
	DISPLAY_2X,
	DISPLAY_3X,
	DISPLAY_1_1, 
};

enum TESTITEM
{
    TEST_NOTHING = 0,

    TEST_MTF,
    TEST_MTF_NEAR,

    TEST_GETSHADINGDATA,
    TEST_APPLYLSC,
    TEST_GETWBDATA,          // use three in i chart

    TEST_SHADING,
    TEST_COLORSHADING,
    TEST_AEAWB,
    TEST_BLEMISH,      // use white chart..
    TEST_DEAD,         // white dead/color dead... use black chart  TEST_STANDBYCURRENT
    TEST_AWB_OTP,
    TEST_COLOR_DOT_OTP,

	TEST_OTP_CHECK, 

};

enum TESTRESULT
{
	TEST_PASS = 0,
	TEST_FAIL,
	TEST_OK1,
	TEST_OK2,
	TEST_NG,
};

typedef struct _StrYCbYCr
{
	BYTE Y0;
	BYTE Cb;
	BYTE Y1;
	BYTE Cr;
}StrYCbYCr, *pStrYCbYCr;

typedef struct _StrCbCr
{
    BYTE   Cb;
    BYTE   Cr;
    double DeltaE;
    float  L;
    float  A;
    float  B;
}StrCbCr, *pStrCbCr;

typedef struct _StrYCbCr
{
	BYTE Y;
	BYTE Cb;
	BYTE Cr;
}StrYCbCr, *pStrYCbCr;

typedef struct _StrYrgb
{
	double Y;
	double R;
	double G;
	double B;
}StrYrgb, *pStrYrgbr;

typedef struct _BlockInfo
{
	BYTE mean;
	BYTE min;
	BYTE max;
	BYTE highcount;
	BYTE lowcount;
}BlockInfo, *pBlockInfo;

typedef struct _ParticleInfo
{
	int  x;
	int  y;
	int  StartX;
	int  StartY;
	int  EndX;
	int  EndY;
	int  iParticleDepth;
	int  iParticleSize;
	BOOL bIsInBlockA;
}ParticleInfo, *pParticleInfo;

typedef struct _ParticleResult
{
	int  iAzoneNum;
	int  iBzoneNum;
	int  iTotalNum;
	BOOL bIsAPass;
	BOOL bIsBPass;
	BOOL bIsPass;
}ParticleResult;

typedef struct _SShading
{
	CPoint Block[10];
	int    width;
	int    height;
	int    DeltaY;
	int    DeltaC;
	int    Ydif;
}SShading;

typedef struct _SColorSpec
{
	double ColorSpecMin;
	double ColorSpecMax;
}SColorSpec;

typedef struct _SColorShading
{
	CPoint     XYPosition;
	int        BlockWidth;
	int        BlockHeight;
	SColorSpec RGSpec;
	SColorSpec BGSpec;
	float      RGResult;
	float      BGResult;
	BOOL       ResultFlag;
}SColorShading;

typedef struct _SColorShadingPara
{
	int        HeightNumber;
	int        WidthNumber;
	SColorSpec ColorSpec;
}SColorShadingPara;

typedef struct _MTFBlock
{
	int  x;
	int  y;
	int  limit;
	int  limit2;
	BOOL enable;
	int  width;
	int  height;
}MTFBlock;

typedef struct _SMTF
{
	MTFBlock Block[20];

	int		 width;
	int		 height;
	int      Near;
	int      XOff;
	int      YOff;
	int      Counter;
	int      APWidth;
	int      APHeight;
	BOOL     m_bOnlyShowOK;
}SMTF;

typedef struct _SBlemish
{
	int  POGMedianA;     // Fast median filter large kernel size
	int  POGMedianB;     // Fast Median filter small kernel size
	int  POGThreshold;   // Actual threshold value equal (Threshold + GlobalMean)
	int  POGPixelLimit;  // For eliminate small connected object
	int  POGEdgeStd;
	int  POGEdgeStep;
	int  POGBigBlockPixelLimit;
}SBlemish;

typedef struct _SNormal
{
	BOOL YUVDirectDraw_En;
	BOOL AutoPlayOnStart;
	BOOL AutoFindSensor;
	BOOL InfoOnVideo_En;
	BOOL PinChk_En;
	BOOL ResetChk_En;
	BOOL PwdnChk_En;
	BOOL I2CChk_En;
	BOOL Y1Chk_En;
	BOOL Y0Chk_En;
	BOOL MTF_En;
	BOOL MTF_Near_En;
	BOOL Color_En;
	BOOL GrayScale_En;
	BOOL Blemish_En;
	BOOL Shading_En;
	BOOL ColorShading_En;
	BOOL SNR_En;
	BOOL AEAWB_En;
	BOOL Dead_En;
	BOOL AWB_OTP_En;
	BOOL OTP_Check_En;
	BOOL ColorDot_OTP_En;
	BOOL QuickTest_En;
	BOOL AutoChange_En;
	UINT AFFar;
	UINT AFNear;
	UINT AFStart;
	UINT AFEnd;
	UINT AFMTFRange;
	UINT AFMTFTopValue;
	UINT AFWaveX2;
	UINT AFContinueNumber;
}SNormal;

typedef struct _SAEAWB
{
	int  AELow;
	int  AEHigh;
	int  AWBLimit;
}SAEAWB;

typedef struct _SLAB
{
	float L;
	float A;
	float B;
}SLAB;

typedef struct _SCOLOR
{
	CPoint  White;
	CPoint  Block[6];
	StrCbCr CbCr[6];
	int     Width;
	int     Height;
	int     Offset;
}SCOLOR;

typedef struct _SGRAYSCALE
{
	CPoint  Block[6];
	BYTE    Lux[6];
	int     Width;
	int     Height;
	int     Offset;
}SGRAYSCALE;

typedef struct _SSNR
{
	int  StdDev;
}SSNR;

typedef struct _SDEAD
{
	int  DeadA_Size;
	int  DeadB_Size;
	int  DeadA_Count;
	int  DeadB_Count;
	int  DeadA_Std;
	int  DeadB_Std;

	int  WoundA_Size;
	int  WoundB_Size;
	int  WoundA_Count;
	int  WoundB_Count;
	int  WoundA_Std;
	int  WoundB_Std;
}SDEAD;

typedef struct _SWhiteDefect
{
	int  AreaHeight;
	int  AreaWidth;
	int  ClusterHeight;
	int  ClusterWidth;
	
	int  DeadHight;
	int  RightSL;
	int  LeftSL;

	int  TopSL;
	int  BottomSL;

	int  WoundHight;
	int  ZoneIDead;
	int  ZoneIIBotton;
	int  ZoneIIDead;
	int  ZoneIILeft;
	int  ZoneIIRight;
	int  ZoneIITop;
	int  ZoneIIWound;
	int  ZoneIWound;
	
	UINT BClusterWidth;
	UINT BClusterHeight;
	UINT BDeadHeight;
	UINT BWoundHeight;
	int  DeadUnit;
	int  WoundUnit;

	UINT ClusterType;
	
}SWhiteDefect;

typedef struct _SConfigTab
{
	SMTF      MTF;
	SBlemish  Blemish;
	SShading  Shading;
	SAEAWB    AEAWB;
	SNormal   Normal;
	SCOLOR    Color;
	SSNR      SNR;
	SDEAD     Dead;
	
	SWhiteDefect       WhiteDefect;
	SGRAYSCALE         GrayScale;
	SColorShadingPara  ColorShadingPara;
}SConfigTab;


struct PixInfo
{
	int xIndex;
	int yIndex;
	int Y_Value;
};

#endif