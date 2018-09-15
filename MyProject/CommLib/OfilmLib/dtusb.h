//////////////////////////////////////////////////////////////////////
// dtusb.h: interface for the cdtusb class.
//////////////////////////////////////////////////////////////////////
//CopyRight Reserved by DOTHINKEY 
//Last update : 2011.06.01 
//Version : Pre 0.9
//useful for HS100 HS200 HS128 HS230 HS300 HV810 HV910
//////////////////////////////////////////////////////////////////////
// 2010 07 24 added...
//setyuvcoefficient(16, 1.164f, -0.391f, -0.813f, 1.596f, 2.018f);  //dothinkey
//setyuvcoefficient(0, 1.0f, -0.698f, -0.336f, 1.371f, 1.732f);  //samsung
//////////////////////////////////////////////////////////////////////

#ifndef _DTUSB_H_
#define _DTUSB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <vfw.h>
#include "sensor.h"


//Class Cdtusb definition..
class Cdtusb  
{
public:
	void GetDPSOffset(int *avdd_offset, int *dovdd_offset, int *dvdd_offset, int *afvcc_offset, int *vpp_offset);
	void SetDPSOffset(int avdd_offset, int dovdd_offset, int dvdd_offset, int afvcc_offset, int vpp_offset);
	BOOL bReadDPSOffsetFromEEPROM(int *avdd_offset, int *dovdd_offset, int *dvdd_offset, int * afvcc_offset, int *vpp_offset);
	BOOL bWriteDPSOffsetToEEPROM(int avdd_offset, int dovdd_offset, int dvdd_offset, int afvcc_offset, int vpp_offset);
	//////////////////////////////////////////////////////////////////////////
	// USB function.
	//////////////////////////////////////////////////////////////////////////
	//Grab one frame from the video.. use ansync mode...
	int bGrabFrame(BYTE* inbuffer, ULONG buffersize);

	//////////////////////////////////////////////////////////////////////////
    //to check whether the demo board is online.
	BOOL bFX2IsMe(BOOL bInit = TRUE, int DeviceNum = 0);
	//set FX2 to start to receive the video data from the sensor.
	void FX2OpenVideo();      
	//set FX2 to stop the video data from sensor..
	void FX2CloseVideo();     

	//enable or disable mclk....
	BYTE SetSensorMclkEnable(BOOL bOn);
	//set sensor MClk in Mhz.
	//2011 05 22 added....
	//effect is the same as SetSensorClock..
	void SetSensorMClk(double mhz);
	//mclk output setup.. in enum mode...
	//can use SetSensorClock directly....
	void SetFX2Clkout(BYTE mode);
	//unit is 0.1MHZ... this is more direct style...
	void SetSensorClock(USHORT hundkhz);

	//no use here....
	void SetGPIOInOrOut(BYTE IoFlag);
	//return value is GPIO port input state...
    //Parameter is output GPIO value,return is Get value of the GPIO input..
	BYTE GPIOInOut(BYTE outvalue); 
	
	//20130301
	///////////////////////////////////////////////////////////
	//该函数写扩展IO BIT0的翻转周期//
	//
	///////////////////////////////////////////////////////////
	void SetExtendPin0Freq(USHORT value);

	//Set Extend IO input or ouput.
	//if bit[x] is 1, then IO[X] is output
	//if bit[x] is 0, then IO[X] is input...
	void SetExtendIOFlag(BYTE inout);
	//write value to extend IO
	void WriteExtendIO(BYTE value);
	//Read Extend IO level....
	BYTE ReadExtendIO();
	
	//Read PIO[26:1] value..
	//return [25:0] is PIO[26:1]'s level state.... 
	ULONG ReadSoftPin();
	//write GPIO4-1...	
	//this IO is in sensor port...
	//you should define it in pin definition.. that means this 4 pin is in GPIO26-GPIO1.
	void WriteIO(BYTE value);
	//set PIO[26:1] pullup or not...
	//before search the sensor, should set pullup to 1...
	int SetSoftPinPullUp(BYTE pullup);

	//get key info...
	WORD GetKey();     
	
public:
	//no use ,just for kill error report...
	short ReadDTOSVoltage(BYTE ch1, BYTE ch2, BYTE fasten);
	//no use ,just for kill error report...
	BYTE SetSensoruAmA(BOOL buA);
	//no use ,just for kill error report...
	WORD CheckSensorPowerIsReady(BYTE value);
	//no use ,just for kill error report...
	short ReadADValue(BYTE port);
	//no use ,just for kill error report...
	void SetVideoDataWidth(BYTE WordWidth); 
	//////////////////////////////////////////////////////////////////////////
public:	
	//////////////////////////////////////////////////////////////////////////
	//I2C related function  main for sensor or AF motor driver IC...
	//Reset I2C bus and related register..
	BOOL bResetI2CBus();
	//b400K = 1, 400K I2C rate...; 0:100K rate...
	void SetI2CRate(BOOL b400k);
	//select I2C channel 0:sensor port; 1:extend port...
	//default is sensor port.
	void SetI2CChannel(BYTE chan);
	//interval time between I2C bytes...
	void SetI2CInterval(BYTE nDly);
	//1:SCL输出模式; 0:高组态模式。。。
	void SetI2CSclMode(BOOL bOutput);  //20111115added...

    //read i2c register.
	BOOL bFX2ReadSensor(USHORT addr, USHORT reg, USHORT* value, BYTE mode=0);
	//write i2c register.
	BOOL bFX2WriteSensor(USHORT addr, USHORT reg, USHORT value, BYTE mode=0);   

	//multi bytes read or write....
	//called by user....
	//20111027 up to max 255 bytes read and write....
	BYTE FX2I2CMultiBytesRead(USHORT Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length);
	BYTE FX2I2CMultiBytesWrite(USHORT Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length);

	//set sensor PWDN or RESET pin. PWDN is data.0,RESET is data.1
	//enable=1 set to data
	//enable=0,set t0 ~data.
	BOOL bSensorEnable(BYTE data, BYTE enable=1); 
	//read the version register as Info specified,then check whether is or not.
	int bSensorIsMe(SensorTab *Info, BOOL bReset = TRUE);  
	//Dual Camera camera = 1,init the 1st camera; = 2 init the second,other return.
	int bSensorIsMe_Dual(SensorTab *Info, int Camera ,BOOL bReset = TRUE);
	//Triple Camera, camera = 1,init the 1st camera; = 2 init the second,=3,init the third.
	int bSensorIsMe_Triple(SensorTab *Info, int Camera, BOOL bReset = TRUE);
	////init the sensor register...
	BOOL bInitSensor(USHORT addr, USHORT* ParaList, USHORT Length, BYTE mode=0);  

	//AF write /read.
    //value:read value
	//return TRUE:success;FALSE:fail
	//this function is not working properly now.don't call this..
	BOOL bReadDAValue(USHORT addr, USHORT* value);
	//return TRUE:success;FALSE:fail
	BOOL bWriteDAValue(USHORT addr, USHORT value, BYTE mode = 0);
	//get the AD value for Current /Voltage Test...
	//get the AF device I2C address 8bit
	USHORT GetAFAddr();
	//find the AF device use 0x18 or 0x1c.
	BOOL bFindAFDevice();
	//write AF value.
	BOOL bWriteAFValue(USHORT value, BYTE mode = 0);
	//set AF address such as 0x18 or 0x1c.
	void SetAFAddr(USHORT addr);

  //20121223 added...
  //get the af type...
	/*
#define AF_DRV_AD5820  0
#define AF_DRV_DW9710  0
#define AF_DRV_DW9714  0

#define AF_DRV_AD5823  1
#define AF_DRV_FP5512  2
#define AF_DRV_DW9718  3
#define AF_DRV_BU64241 4
#define AF_DRV_MAX	   5  //not supported...
*/
  BYTE GetAFDevType();
  //20121223 added...
  //set the af type..
  void SetAFDevType(BYTE type);

	//OS test....
	int DTOS_OpenTest(ULONG osmaskl, ULONG osmaskh, BYTE kc, BYTE oseasy, USHORT openStd, USHORT shortStd);
	//reset OS 
	void DTOS_Reset();
	//return -3 communication error...
	//return -2 not finished...
	//return -1 no error...
	//return 0 no error...
	//else return OS result 
	int DTOS_WaitTestFinished(BYTE* pResult, USHORT MaxWaitms);
	//Current overload....
	/*
	*  对应位为1 过流，为0无过流
	*  OverBuf[0].0位是AVDD;   OverBuf[0].1位是DOVDD;  OverBuf[0].2位是DVDD；
	*  OverBuf[0].3位是AFVCC;  OverBuf[0].4位是VPP;
	*  OverBuf[0]是当前过流值，
	*  OverBuf[1]是当前过流值，无过流时保持上次值
	*  OverBuf[2]是OverBuf[1]上一次过流值，
	   OVER-C1=AVDD，OVER-C2=DOVDD，OVER-C3=DVDD，OVER-C4=AFVCC，OVER-C5=VPP	
	*/
	int GetCurrentOverload(BYTE *OverBuf);
	//set device beep time and beep count...
	void SetAlarm(USHORT beeptime, BYTE beepcount);	
	//PMU....
	int ResetPMU();
	//set PMU range.... AVDD, DOVDD, DVDD, AFVCC, VPP...
	BYTE SetPMURange(BYTE pmu1, BYTE pmu2, BYTE pmu3, BYTE pmu4, BYTE pmu5);
	//should set to 0,if get current
	void SetPMUSelfTestRange(BYTE range);
	//Enable PMU or not....
	BYTE SetPMUOnOff(BOOL bOn);
	//get the Current Value.
	int GetPMUData(BYTE *pBuf, BYTE length);

	//set sensor current limit. max_100ma = 1, maxium current is 100mA. else 300mA.
	//if the current exceed the limit, the system will beep ...
	void SetSensorCurrentLimit(BYTE max_100ma);
	//set sensor working voltage.. 
	//2011 05 14 added afvcc settings.
	//default afvcc is 2.8V...
	void SetSensorVoltage(BYTE avdd, BYTE dovdd, BYTE dvdd, BYTE afvcc = AFVCC_28);
	//Sensor Power (AVDD DOVDD DVDD AFVCC) / VPP on or off....
	BYTE SetSensorPowerOnOff(BOOL bSensorPowerOn, BOOL bVPPOn = 0);
	//sensor power on or off...
	//BYTE SetSensorPowerOnOff(BOOL bOn);
	//VPP voltage 0.5-9V
	BYTE SetSensorVPPVoltage(double vol);
	//only use in digital power source....
	void SetDPSOnOff(BOOL avdd_sw,  BOOL dovdd_sw, BOOL dvdd_sw, BOOL afvcc_sw, BOOL vpp_sw);
	BOOL SetDPSInChannel(BYTE channel, double mv);
	void SetDPS(double avdd, double dovdd, double dvdd, double  afvcc, double vpp);

	//set receiver buffer voltage.
	//this should set to same as DOVDD normally, but you can set it to your need...
	//this can be used to remove noise problem....
	void SetBufIOVoltage(BYTE bufio);

	//Set the control of feedback to sensor.
	//bExtendVPPFB control the extend line (remote) feedback of VPP to the the sensor.if it have.
	//bExtendSensorFB control the extend line (remote) feedback of AVDD/DOVDD/DVDD/AFVCC to the the sensor.if it have.
	//bInnerVppFB control the VPP feedback(only in the DemoKit sensor port).
	//bInnferSensorFB control the AVDD/DOVDD/DVDD/AFVCC feedback to the sensor.
	BYTE SetSensorPowerFB(BOOL bExtendVppFB, BOOL bExtendSensorFB, BOOL bInnerVppFB, BOOL bInnerSensorFB);

	//set to image mode or dtos mode...
	//for HV910 . mode = 1, dtos mode; mode = 0, image mode...
	//other system , this is no use...
	void SetWorkMode(BYTE mode);

	//check reset or power down pin ok or not...
	BYTE CheckRstPwdnPin(SensorTab *Info, BOOL RstChkEn, BOOL PwdnChkEn, BYTE CurCam = 0);

	BOOL GetMIPIClkFrequency(ULONG *freq);
	//for correct mipi receiver, should set this to enable ....
	// if disable ,no clk_n or no clk_p can output image...
	void CheckMIPIClk(BOOL bEnable);
	//using this function to check MIPI siginals whether link to GND or not...
	//should set MIPI sensor to LP state or reset state or Pwdn state firstly... 
	//BIT7-0 DATA N P
	//BIT9-8 CLK N P
	USHORT ReadMIPIPortInLPState(void);

	//roi mode...
	//roi_x0 roi start x;
	//roi_y0 roi stat y;
	//roi_hw roi horizontal width
	//roi_vw roi vertical height.
	//roi_hb roi horizontal blank width
	//roi_bw roi_vertical blank height
	//roi_hnum roi horizontal total count...
	//roi_vnum roi vertical total count....
	//type:sensor type...
	void SetROI(USHORT roi_x0, USHORT roi_y0, USHORT roi_hw,USHORT roi_vw, USHORT roi_hb, USHORT roi_vb, USHORT roi_hnum, USHORT roi_vnum, BYTE type,BOOL roi_en = TRUE);
	//set preview width and height and type....
	//here isp_en is no use...because usb2.0 don't this function...
	BOOL SetPreview(USHORT width, USHORT height, BYTE type, BOOL isp_en = FALSE, BOOL bEnMono = FALSE);
	
	//set sensor port type:MIPI /Parallel/MTK/SPI/TEST...
	BYTE GetSensorOutPort();
	//type = 0 : MIPI
	//type = 1 : Parallel
	//type = 2 : MTK
	//type = 3 : SPI
	//type = 4 : test
	void SetSensorOutPort(BYTE type);

	//multicam mode
	BYTE GetMultiCamMode();//
	//set multicam mode...
	void SetMultiCamMode(BYTE mode);

	//set the Pclk and Hsync polarity;
	//0 is invert the clk. 1 is postive clk...
	//Vsync = -1; auto.default
	//Vsync = 0; when 0 output valid hsync 
	//Vsync = 1; when 1 output valid hsync 
	BOOL bSetPClkHSync(BOOL bPClk, BOOL bHSync, char Vsync = -1);

	//no use here
	void CancelIoWait();
	//no use here...
	BOOL GetLoadInfo(UINT *FrameOk,UINT *FrameErr,UINT *LastSize);

///////////////////////////////////////////////////////////////////////////////////////////////
public:
	//these functions only useful in HS100 HS200...
	//soft pin definion..
	void SetFPGAPin(BYTE* PinList);
	//set fpga to spi serial mode...
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGAToSPI(BOOL bEnable);
	//set fpga to mtk serial mode...
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGAToMTKSerial(BOOL bEnable);
	//set fpga output hblank and vblank.... normally don't change this...
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGAOutHVblank(USHORT hblank, USHORT vblank);
	//multi lines output for fpga to usb2.0 ic.
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGAOutMultiLines(UINT bits);
	//fpga output enable or not
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGAGPIOOut(BYTE enable);
	//fpga pclkin selection.
	//0 pclk ; 1:pclk dly1; 2 mclk; 3:pclk dly2..
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGAPclkIn(BYTE clk);
	//reset fpga timing ,but not register...
	//no use for HS130 HS300 HV810 HV910....
	void ResetFPGA();
	//set fpga to direct mode,just as pass through the ic, no buffer or no process image...
	//no use for HS130 HS300 HV810 HV910....
	void SetFPGABypassMode(BYTE enable);
	//set fpga output clock... this is useful for usb2.0 receiver.
	//0: 40M
	//1: 36M
	//2: 24M
	//3: 48M
	// only usefull in hs100 hs200. other system no use....
	void SetFPGAOutClock(BYTE speed);

public:
	//////////////////////////////////////////////////////////////////////////
	// Data process..
	//////////////////////////////////////////////////////////////////////////
	int GetCenterAreaFocusValue(BYTE* pData, int width, int height);
	//mode should be 0-3;
	//Set YUV422 mode, 
	//0:YCbYCr;
	//1:YCrYCb;
	//2:CbYCrY;
	//3:CrYCbY;
	void SetYUV422Mode(BYTE mode);
	//mode should be 0-3;
	//Set RGB raw mode, 
	//0:RGGB; first line is RGRG.... ;second line is GBGB....;
	//1:GRBG; first line is GRGR.... ;second line is BGBG....; 
	//2:GBRG; first line is GBGB.... ;second line is RGRG....; 
	//3:BGGR; first line is BGBG.... ;second line is GRGR....; 
	void SetRawMode(BYTE mode);

	//change the captured from usb(sensor) to RGB24 bmp data..
	//pIn: usb data buffer pointer
	//pOut:RGB24 data buffer pointer
	//width:Image source width
	//height Image source height
	//type: sensor type. should be 0:10bit raw ;1:8bit raw;2:YUV422 data;3:16bit raw.
	void USBDataToRGB24(BYTE* pIn, BYTE* pOut, USHORT width, USHORT height, BYTE type);

	//RGB24 to YUV422 (YcbYcr mode)
	//pBmp24 :RGB24 data pointer;
	//pOut   :YUV422 pointer
	//width  :Image width;
	//height :Image height;
	void RGB24ToYUV422(BYTE* pBmp24, BYTE* pOut, int width, int height);
	//RGB24 to YUV444 (Ycbcr )
	//pBmp24 :RGB24 data pointer;
	//pOut   :YUV422 pointer
	//width  :Image width;
	//height :Image height;
	void RGB24ToYUV444(BYTE *pBmp24, BYTE *pOut, int width, int height);

	//Raw data to YUV422 (YcbYcr Mode)
	//pRaw : RGB raw data pointer;
	//width: Image width;
	//height:Image height;
	//two steps:1. change to RGB24, call USBDataToRGB24()
	//          2. change RGB24 to YUV422 data format ,call RGB24ToYUV422()
	void RawToYUV422(BYTE *pRaw, int width, int height, int type);

	//RGB raw 8 to RGB24 according to the mode
	//pIn: raw 8 bit data buffer pointer.
	//pOut:RGB24 data buffer pointer.
	//nWidth:Image source width;
	//nHeight:Image source height;
	//Mode: Raw mode.should be 0-3.
	//0:RGGB; first line is RGRG.... ;second line is GBGB....;
	//1:GRBG; first line is GRGR.... ;second line is BGBG....; 
	//2:GBRG; first line is GBGB.... ;second line is RGRG....; 
	//3:BGGR; first line is BGBG.... ;second line is GRGR....; 
	void Raw8ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight , BYTE Mode);

	//call Raw8ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight , BYTE Mode);
	//Mode is set by void SetRawMode(BYTE mode);
	void Raw8ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	//RGB raw 10 bit to RGB24 according to the mode
	//pIn: raw 10 bit data buffer pointer.
	// pIn: even byte is high 8 bit ; odd byte 2LSB is low 2 bits.
	//pOut:RGB24 data buffer pointer.
	//nWidth:Image source width;
	//nHeight:Image source height;
	//Mode: Raw mode.should be 0-3.
	//0:RGGB; first line is RGRG.... ;second line is GBGB....;
	//1:GRBG; first line is GRGR.... ;second line is BGBG....; 
	//2:GBRG; first line is GBGB.... ;second line is RGRG....; 
	//3:BGGR; first line is BGBG.... ;second line is GRGR....; 
	void Raw10ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight , BYTE Mode);

	//call Raw10ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight , BYTE Mode);
	//Mode is set by void SetRawMode(BYTE mode);
	void Raw10ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);

	//RGB raw 16 bit to RGB24 according to the mode
	//pIn: raw 16 bit data buffer pointer.
	// pIn: even byte is high 8 bit ; odd byte is low 8 bits.
	//pOut:RGB24 data buffer pointer.
	//nWidth:Image source width;
	//nHeight:Image source height;
	//Mode: Raw mode.should be 0-3.
	//0:RGGB; first line is RGRG.... ;second line is GBGB....;
	//1:GRBG; first line is GRGR.... ;second line is BGBG....; 
	//2:GBRG; first line is GBGB.... ;second line is RGRG....; 
	//3:BGGR; first line is BGBG.... ;second line is GRGR....; 
	void Raw16ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight , BYTE Mode);
	//call Raw16ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight , BYTE Mode);
	//Mode is set by void SetRawMode(BYTE mode);
	void Raw16ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);

	//YUV422 change to RGB24 data format.
	//pIn: YUV422 data buffer pointer;
	//pOut:RGB24 data buffer pointer;
	//nWidth: Image source width;
	//nHeight:Image source height;
	//mode: YUV422 mode..
	//0:YCbYCr;
	//1:YCrYCb;
	//2:CbYCrY;
	//3:CrYCbY;
	//call YUV422ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight,BYTE mode);
	//mode is set by  SetYUV422Mode(BYTE mode)
	void YUV422ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	//RGB565 to RGB24...
	void RGB565ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	//find edge and set flag in pOutE;
	void FindEdge(BYTE* pIn, int width, int height, BYTE* pOutE);
	//edge enhance according to EdgeFlag buff...
	void EdgeEnhCalculation(BYTE* pIn, BYTE* pInE, BYTE* pOutImage, int width, int height);
	void SetEdge(BYTE THflat, BYTE THedge, double EdgeGain, BOOL EdgeEnable);

	void KillBmpNoise(BYTE *pBmp, int nWidth, int nHeight);
	//////////////////////////////////////////////////////////////////////////
public:
	//verticl flip or not.
	void SetVFlip(BOOL bEnable);
	//horizontal flip or not...
	void SetHFlip(BOOL bEnable);
	//set mono or color mode...
	void SetMonoMode(BOOL bEnable);
	//change from color to gray(mono) bmp24
	void Gray(BYTE* pData, LONG nWidth, LONG nHeight);

	//white balance.here is very simple. Rall = Gall = Ball...
	void WBControl(BYTE *pBmp24, int nWidth, int nHeight);
	void UpdateGainTable();
	BOOL HFlipBuf(BYTE *pData, LONG nWidth, LONG nHeight);
	BOOL VFlipBuf(BYTE *inbuf, UINT widthBytes, UINT height);
	
	int GetMatrixData(BYTE color,BYTE channel);
	void SetMatrixData(int R00, int R01, int R02, int G00, int G01, int G02, int B00, int B01, int B02);

	//////////////////////////////////////////////////////////////////////////
	//color balance control :digital control.
	//////////////////////////////////////////////////////////////////////////
	//White balance and digital gain control function
	// Set the AWB Gain Coefficent.
	//default Red,Blue are 1.0f;1.0f.
	//enable auto white balance or disable it..
	void SetAWB(BOOL bAWBEn);
	// check the now AWB state:AWB is ON or OFF.
	BOOL GetAWB();
	//get the current gain..
	void GetDigitalGain(int* pRGain,int* pGGain,int* pBGain );
	//set the digital gain..
	void SetDigitalGain(int RGain,int GGain,int BGain );
	//////////////////////////////////////////////////////////////////////////
	//Set Gamma /Contrast/Saturation parameter...
	//////////////////////////////////////////////////////////////////////////
	void SetGamma(BYTE gamma);
	BYTE GetGamma();
	//get the contrast value of the video... only for rgb raw data
	BYTE GetContrast();
	//set the contrast value of the video... only for rgb raw data
	void SetContrast(BYTE value);
	//get the saturation value of the video... only for rgb raw data
	BYTE GetSaturation();
	//set the saturation of the video only for rgb raw data
	void SetSaturation(BYTE value);
	//get the color matrix correction enable or disable state...
	//set the color matrix enable or disbale .
	void ImageProcess(BYTE *pRaw, BYTE *pBmp24, int nWidth, int nHeight);
	void ImageISP(BYTE *pBmp24, int width, int height, BYTE type);
	//set yuv color matrix...
	void setyuvcoefficient (int yoff, float ycoef, float cbcoef, float crcoef,float rdif, float bdif);

public:	
	BYTE bPCTransferToUSBAndWriteSensor(BYTE *ParaList, BYTE ParaSize);
	BOOL bCheckIsDothinkeyPC();

	BOOL GetHardwareVersion(BYTE* pBuf, BYTE length);
	//return device serial no....,8 bytes....
	void GetDeviceSN(BYTE* pSN);
	BOOL bCheckDPS(void);

	WORD GetSensorVideoFrameCount();

	WORD GetSensorVideoHeight();
	WORD GetSensorVideoWidth();
	void ReSendImage();
	void SetMIPILongPacketOut(BOOL bEnable);
	BYTE ReadSpecialI2C(BYTE addr, BYTE reg, BYTE *ParaList, BYTE length);


	void SetMIPIClk(BOOL bContinous);
	BYTE GetMIPIClkPhase(void);
	void SetMIPIClkPhase(BYTE phase);
	void EnableFPGARW(BOOL bEn);
	USHORT ReadMIPICRC(void);


	void TriggerSnap();
	void SetSnap(BOOL snap_en, BYTE snap_count);
	void CopySoftGridData(BYTE *pBmp24);
	void DisplaySoftGrid(BYTE* pData);
	void SetSoftGrid(BOOL sg_en, 
					 int lux0, int luy0, int rux0, int ruy0, int ldx0, int ldy0, int rdx0, int rdy0, 
					 int corner_h, int corner_v, 
					 int ctx0, int cty0, 
					 int ct_h, int ct_v);


	void ClearUSBFIFO();

    //set video data port width...
	//wait the vsync .Time is the max time to wait.
	//get the current Frame count;
	//get the usb err code.
	BYTE GetUSBErrCode();
	//return firmware version
	WORD GetFWVersion();
	//return lib version;
	DWORD GetLibVersion();

	//show cross on image
	void ShowCrossOnImage(BYTE * pBmp, int width, int height);
	//enable cross on or off on the video....
	void SetCrossOn(BOOL bEnable);
	
	//init the display handle and the image source width and height and also sensor type.
	//hWnd: the display video window handle.
	//ImgWidth: image source width;
	//ImgHeight:image source height;
	//SensorType:Sensor output format,should be 0:10bit raw ;1:8bit raw;2:YUV422 data;3:16bit raw.
	void InitDisplay(HWND hWnd, USHORT ImgWidth, USHORT ImgHeight, BYTE SensorType);
	//display the RGB24 image data.
	//pBmp24: RGB24 bmp data buffer pointer.
	void DisplayRGB24(BYTE* pBmp24);
	//display the usb data
	//two steps:
	//1: call USBDataToRGB24() to change the usb data to the RGB24 data format.
	//2: call DisplayRGB24() to display it...
	void bDisplayVideo(BYTE* pData);

	void DisplayRGB24(BYTE* pBmp24, int x0, int y0);
	void bDisplayVideo(BYTE *pData, int x0, int y0);
	//////////////////////////////////////////////////////////////////////////
	void VertFlipBuf(BYTE *inbuf, UINT widthBytes, UINT height);

	//set async read wait time....
	void SetWaitTime(ULONG tmS, BOOL WaitEn = FALSE);
	
	//decode data... 
	//code data is for usb2.0 bug...,here should decode it before process...
	void BitUnSwizzling(BYTE* pBuffer, ULONG size);
	
	Cdtusb();
	virtual ~Cdtusb();
	//RGB24 bmp bufffer... no header ,just only image data...
	BYTE  *m_BmpBuffer;
	//io i2c
	BYTE FX2MultiDataI2CRead(USHORT Addr, BYTE *ParaList, USHORT Length);
	BYTE FX2MultiDataI2CWrite(USHORT Addr, BYTE *ParaList, USHORT Length, BOOL bNoStop = 0);
	//for hs128 hs230 hs130 hs300 hv810 hv910, other kit no use....
	BOOL bFX2ReadFPGAI2C(BYTE reg, BYTE* val);
	BOOL bFX2WriteFPGAI2C(BYTE reg, BYTE val);

	BOOL WriteHiSPIEnable();

//don't change follow function or viarable.!!!
private:
	BOOL CheckHiSPIEnable(void);

	void WriteRegInfo(void);
	BOOL WriteGuidToProm(void);

	BOOL Vendor2(WORD Addr, WORD Reg, BYTE *buffer, LONG length, BYTE Dir, BYTE ReqCode);
	BOOL Vendor(BYTE *buffer, LONG length, BYTE Dir, BYTE ReqCode);


	//20111027 up to max 255 bytes read and write....
	BYTE Inner_I2CMultiBytesRead(USHORT Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length);
	BYTE Inner_I2CMultiBytesWrite(USHORT Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length);
	
	//for hs100 hs200. others no use....
	
	BOOL DT_FX2WriteMIPI(BYTE reg, BYTE val);
	BOOL DT_FX2ReadMIPI(BYTE reg,  BYTE* val);	
	

	
	//set fpga mask...
	void SetDataMask(BYTE softmask, BYTE fpgamask);
	
	//eeprom enable write or not
	BYTE EEPromEnableRW(BOOL bEn);
	//addr:slave ID;reg,register address;value:read value...
	BOOL bReadWordFromEEProm(USHORT addr, USHORT reg, USHORT *value);
	//write one word data to eeprom
	//addr:slave ID;reg,register address;value:write value...
	BOOL bWriteWordToEEProm(USHORT addr, USHORT reg, USHORT value);
	
	BOOL SpiRW(DWORD dwOut,DWORD *In,int nBytes, BOOL bStart = true,BOOL bStop = true,BOOL bMsb = true);
	
	BOOL EpcsCheckID();
	BYTE EpcsStatus();
	BOOL EpcsBusyWait(int nTimeout=500);
	
	BOOL EpcsRead(DWORD dwAddr,BYTE *pData,int nSize);
	BOOL EpceWriteEnable(BOOL bEn);
	BOOL EpcsErase();
	BOOL EpcsWrite(DWORD dwAddr,BYTE *pData,int nSize);



	//system i2c
	BYTE DT_FX2MultiDataI2CRead(USHORT Addr, BYTE *ParaList, USHORT Length);
	BYTE DT_FX2MultiDataI2CWrite(USHORT Addr, BYTE *ParaList, USHORT Length, BOOL bNoStop = 0);
	
	//to check MIPI signals ready or not. 
	//enable mipi receiver....
	//bEnable = 1,mipi working; = 0, disable mipi decode and disable mipi output....
	BOOL EnableMIPIReceiver(BOOL bEnable);

	BOOL InitParameter();
	//Grab one frame from the video.. use sync mode
	int bGrabFrameOnce(BYTE *inbuffer, ULONG buffersize);
	//called by grabFrame,don't change this...!!!
	int bGrabFrameQueue(BYTE *inbuffer,ULONG buffersize);

	void UpdateGammaMatrix(BYTE gamma,BYTE contrast);

	BYTE ContrastValue(BYTE data, BYTE contrast);

	//Caculate WB for statistics. then caculate the R G B gain..
	void CaculateWB(BYTE *pData,UINT nWidth,UINT nHeight);

	//according to display format,(32 24 16 ) change data format.

	//////////////////////////////////////////////////////////////////////////
	//Image dsp function. such as saturation and contrast,Gamma,scale 
	//////////////////////////////////////////////////////////////////////////
	//control the color translation .. such as contrast,digital gain...
	void ColorChange(BYTE* pData,UINT nWidth,UINT nHeight);
	//set the saturation value...
	void SaturationChange(BYTE* pIn,int nWidth,int nHeight,int ValueChange);
	//set the contrast value..
	//Gamma correction function.
	void InitGammaTable();
	void GammaCorrection(BYTE *pData,int nWidth,int nHeight);

	//////////////////////////////////////////////////////////////////////////
	//addr:Slave ID address
	//value:write value
	//bRead eeprom one word data..

	BYTE FX2ReadSensorData(USHORT reg, USHORT* value, BYTE mode);
	BYTE FX2WriteSensorData(USHORT reg, USHORT value, BYTE mode);

	BOOL bFX2SetSensorSlaveAddress(USHORT addr);

	BOOL bEnableASyncRead(BOOL bState);
	BOOL bOpenDriver(HANDLE* phDeviceHandle, PCHAR devname);

	void WriteIIS(BYTE reg, WORD value);

	BOOL bCheckDeviceType();
	BOOL bVerifyFX2IsSafe();

	//20130301 added...
	BOOL bReadPLLReg(BYTE reg, BYTE* value);

	BOOL bWritePLLReg(BYTE reg, BYTE value);
	void SetPLL(BYTE mode);
	
	//////////////////////////////////////////////////////////////////////////
private:
	BOOL ReadParameter();
	BOOL SaveParameter();
	BOOL bSetDataMode(BYTE mode);
	BOOL bFX2Init();
	BOOL bResetI2C();
	BOOL bFX2GetString(char*Descriptor, BYTE index);
	BOOL bFX2Sleep(HANDLE hDevice, BOOL bState);
	BOOL bFX2VideoOn(HANDLE hDevice, BOOL bState);
	BOOL bFX2DataFormat(BOOL bWordwide);
	BOOL bOpenDevice(int DeviceNum = 0);
	BOOL bCloseDevice();

	//////////////////////////////////////////////////////////////////////////
private:		
	//////////////////////////////////////////////////////////////////////////

	void MipiRaw10ToP8Raw(BYTE *pIn, USHORT DesW, USHORT DesH);
	void MipiRaw12ToP8Raw(BYTE *pIn, USHORT DesW, USHORT DesH);

	void Raw8_RGGB_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw8_GRBG_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw8_GBRG_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw8_BGGR_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);

	void Raw10_RGGB_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw10_GRBG_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw10_GBRG_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw10_BGGR_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);

	void Raw16_RGGB_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw16_GRBG_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw16_GBRG_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);
	void Raw16_BGGR_ToRGB24(BYTE *pIn, BYTE *pOut, int nWidth, int nHeight);

	void identmat(long matrix[4][4], long matrixnew[4][4]);
	void matrixmult(long a[4][4], long b[4][4],long c[4][4]);
	void applymatrix(BYTE* lptr, long mat[4][4],int n);
	void cscalemat(long mat[4][4], int rscale,int gscale,int bscale);
	void saturatemat(long mat[4][4], int sat);
	//////////////////////////////////////////////////////////////////////////

private:
	BOOL bVerifyAllFirmware();
	BOOL CheckDothinkeyKey();
	BOOL bCheckFirmwareIsReady(BYTE *pBuffer, USHORT size);
	void SetSafeState(DWORD state);
	WORD GetSafeState();
	BOOL bPC100IsMe();
	BOOL CheckPromIsReady(void);
	WORD CheckFX2IsReady(BYTE value);
	BOOL bReadRegGUIDData(BYTE mode = 0);

	BOOL DT_FX2ReadGuidI2C(BYTE addr, BYTE reg, USHORT* vuale);
	BOOL DT_FX2WriteGuidI2C(BYTE addr,  BYTE reg, USHORT value);

	
	WORD _get_CRC16( BYTE *buf, WORD count );
	BOOL bCheckFWCRC16();

	BOOL bReadWordFromEEProm(USHORT reg, USHORT *value);
	BOOL bWriteWordToEEProm(USHORT reg, USHORT value);
	
	void ResetMIPIReceiver();

	
	ULONG gcd(ULONG x,ULONG y);
	ULONG lcm(ULONG x,ULONG y);
	BYTE GammaValue(BYTE data,BYTE Gamma);
	
private:

	CRITICAL_SECTION m_usbLock;

	BYTE m_bMIPI_I2C_Std;
	BYTE m_MultiCamMode;
	BOOL  m_bAWBEn;        //White Balance enable or disable.. 
	BOOL  m_bColorCorrectEn;
	

	//saturation value
	BYTE m_Saturation;     //Saturation value...
	//contrast value..
	BYTE m_Contrast;      //contrast value
	int m_RGain;          //Red gain digital
	int m_GGain;          //Green gain digital
	int m_BGain;          //Blue gain digital
	
	BOOL m_bMono;
	BOOL m_bVFlip;
	BOOL m_bHFlip;
	
	BOOL m_bCross;
	
	BOOL m_bCheckFail;
	BOOL m_CheckAddr;
	
	BOOL m_bInited;

	BYTE m_ComOff;

	BYTE m_SensorEnable;
	ULONG m_USBBlockSize;

	LONG m_TimeOutCnt;
	BOOL m_bWaitEn;


	USHORT m_Height;   //Image Height
	USHORT m_Width;    //Image Width
	BYTE   m_SensorType; //Sensor Type:0-3 :0:RAW8;1:RAW10;2:YUV422;3:RAW16
	BYTE	m_SensorPort;

	HWND   m_hWnd;         // the show area HANDLE
	BITMAPINFOHEADER m_BmpInfoHeader;  //Image info
	HDC      m_hDC ;
	HDRAWDIB m_bmpDC;

	OVERLAPPED m_Overlapped[4];             //the data transfer to driver for Async mode..

	BYTE   m_RawMode;    //Sensor output raw data format:0:RGRG...GBGB...,
	BYTE   m_YUVMode;    //Sensor output YUV422 format:0:YUYV;1:YVYU;2:UYVY;3:VYUY
	BYTE   m_ErrCode;    //Err code
	
	BOOL   m_bIsMe;
    UINT   fm_version;
	HANDLE m_hDevice;

	USHORT m_nSlaveAddress;
	USHORT m_nAFAddr;
	BYTE   m_AFDevType;  //20120222 added... AF芯片类型。

	BYTE m_DataMask;

	USHORT m_GrabW;
	USHORT m_GrawH;

	int *m_yuv_bdif;
	int *m_yuv_rdif;
	int *m_yuv_y;
	int *m_yuv_inv_cb;
	int *m_yuv_inv_cr;
	BYTE *m_GammaTab; //Gamma table releated to m_Gamma..
	BYTE *m_RGainTab;
	BYTE *m_GGainTab;
	BYTE *m_BGainTab;


	BYTE  *m_pBlock[5];

	BYTE m_Gamma;        //Gamma value default is 1.0f
	long  m_Matrix[4][4];

	BYTE m_DriverVer;	

	BYTE m_THflat;
	BYTE m_THedge;	
	BYTE* m_pEdgeFlag;
	int m_EdgeGain;
	BOOL m_EdgeEnable;
	//20120203
	BOOL m_mipi_support;
	BOOL m_hispi_support;

	BOOL m_dps_support;

	//20110621 added...
	int m_sg_lux0;
	int m_sg_luy0;
	int m_sg_rux0;
	int m_sg_ruy0;
	int m_sg_ctx0;
	int m_sg_cty0;
	int m_sg_ldx0;
	int m_sg_ldy0;
	int m_sg_rdx0;
	int m_sg_rdy0;
	int m_sg_corner_h;
	int m_sg_corner_v;
	int m_sg_ct_h;
	int m_sg_ct_v;
	int m_sg_en;

	int m_avdd_offset;
	int m_dovdd_offset;
	int m_dvdd_offset;
	int m_afvcc_offset;
	int m_vpp_offset;

};

#endif // !defined(AFX_FLYUSB_H__AF562563_84FB_45AE_915D_67DBE642F995__INCLUDED_)
