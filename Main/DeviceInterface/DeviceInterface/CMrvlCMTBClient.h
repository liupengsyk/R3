#pragma  once
#include "stdafx.h"
using namespace std;

typedef enum
{
	CAM_IMGFMT_RGGB8      = 1000,
	CAM_IMGFMT_BGGR8      = 1001,
	CAM_IMGFMT_GRBG8      = 1002,
	CAM_IMGFMT_GBRG8      = 1003,
	CAM_IMGFMT_RGGB10     = 1004,
	CAM_IMGFMT_BGGR10     = 1005,
	CAM_IMGFMT_GRBG10     = 1006,
	CAM_IMGFMT_GBRG10     = 1007,
	CAM_IMGFMT_BGGR16     = 1008,
	CAM_IMGFMT_GBRG16     = 1009,

	// low->high  R,G,B
	CAM_IMGFMT_RGB888     = 2000,
	CAM_IMGFMT_RGB444     = 2001,
	CAM_IMGFMT_RGB555     = 2002,
	CAM_IMGFMT_RGB565     = 2003,
	CAM_IMGFMT_RGB666     = 2004,
	CAM_IMGFMT_BGR888     = 2005,
	CAM_IMGFMT_BGR444     = 2006,
	CAM_IMGFMT_BGR555     = 2007,
	CAM_IMGFMT_BGR565     = 2008,
	CAM_IMGFMT_BGR666     = 2009,
	// with alpha channel
	CAM_IMGFMT_BGRA8888   = 2010,
	CAM_IMGFMT_RGBA8888   = 2011,

	CAM_IMGFMT_YCbCr444P  = 3000,
	CAM_IMGFMT_YCbCr444I  = 3001,
	CAM_IMGFMT_YCbCr422P  = 3002,
	CAM_IMGFMT_YCbYCr     = 3003,
	CAM_IMGFMT_CbYCrY     = 3004,
	CAM_IMGFMT_YCrYCb     = 3005,
	CAM_IMGFMT_CrYCbY     = 3006,
	CAM_IMGFMT_YCrCb420P  = 3007, // fourcc: YV12
	CAM_IMGFMT_YCbCr420P  = 3008, // fourcc: IYUV, I420
	CAM_IMGFMT_YCbCr420SP = 3009, // fourcc: NV12
	CAM_IMGFMT_YCrCb420SP = 3010, // fourcc: NV21

	CAM_IMGFMT_JPEG       = 4000,

	CAM_IMGFMT_LIMIT      = 0x7fffffff,
} CAM_ImageFormat;

// focus state
typedef enum
{
	CAM_FOCUSSTATE_PERSIST    = 0x1,
	CAM_FOCUSSTATE_INFOCUS    = 0x2,
	CAM_FOCUSSTATE_OUTOFFOCUS = 0x3,
	CAM_FOCUSSTATE_FAIL       = 0x4,

	CAM_FOCUSSTATE_NUM,

	CAM_FOCUSSTATE_LIMIT      = 0x7fffffff,
}CAM_FocusState;

typedef enum {
	CMTB_CMD_START_CAMERA,              //Start Camera ( multi camera is not allowed ). return value: 0->fail, other values->success
	CMTB_CMD_STOP_CAMERA,               //Exit Camera. return value: 0->fail, other values->success
	CMTB_CMD_RESET_CAMERA,              //camera module hardware reset ( fd will be still valid ). return value: 0->fail, other values->success
	CMTB_CMD_RESET_APP,                 //CMTB app reset ( fd will be no longer valid ). return value: 0->fail, other values->success
	CMTB_CMD_RESET_BOARD,               //board Reset ( board reboot ). return value: 0->fail, other values->success
	CMTB_CMD_SET_CAPTURE_SIZE_FORMAT,   //set image type. return value: 0->fail, other values->success
	CMTB_CMD_CAPTURE,                   //capture and send an image, data area should be pre-allocated, and variable size is used to store the actual size of the image. return value: 0->fail, other values->success
	CMTB_CMD_LOCK_AE,                   //lock AE/unlock AE. return value: 0->fail, other values->success
	CMTB_CMD_LOCK_AWB,                  //lock AWB/unlock AWB. return value: 0->fail, other values->success
	CMTB_CMD_LOCK_LSC,                  //Turn on LSC/Turen off LSC. return value: 0->fail, other values->success
	CMTB_CMD_SET_BPC,                   //Turn on BPC(DPC)/Turn off BPC(DPC). return value: 0->fail, other values->success
	CMTB_CMD_SET_EV,                    //Set exposure value. return value: 0->fail, other values->success
	CMTB_CMD_TRIGGER_FOCUS,             //Focus a target object. return value: 0->fail, other values->success
	CMTB_CMD_SET_FOCUS_POSITION,        //Set AF position. return value: 0->fail, other values->success
	CMTB_CMD_GET_FOCUS_POSITION,        //Read current focus position. return value: 0->fail, other values->success
	CMTB_CMD_GET_FOCUS_STATUS,          //get AF status(AF idle or running). return value: 0->fail, other values->success
	CMTB_CMD_GET_FOCUS_SCORE,           //get AF score(AF filter output value). return value: 0->fail, other values->success
	CMTB_CMD_SET_FOCUS_MODE,            //set to Manual AF mode/set to Single AF mode/set to Continuous AF mode/set to Macro AF mode. return value: 0->fail, other values->success
	CMTB_CMD_ERASE_FROM,                //erase eeprom specified area. return value: 0->fail, other values->success
	CMTB_CMD_READ_FROM,                 //read eeprom specified area. return value: 0->fail, other values->success
	CMTB_CMD_WRITE_FROM,                //write eeprom specified area. return value: 0->fail, other values->success
	CMTB_CMD_SET_FROM_READ_MODE,        //set FROM read mode. return value: 0->fail, other values->success
	CMTB_CMD_READ_AF_I2C_DATA,          //Read VCM specified address. return value: 0->fail, other values->success
	CMTB_CMD_WRITE_AF_I2C_DATA,         //Write VCM specified address, data. return value: 0->fail, other values->success
	CMTB_CMD_READ_SENSOR_I2C_DATA,      //Read sensor specified address. return value: 0->fail, other values->success
	CMTB_CMD_WRITE_SENSOR_I2C_DATA,     //Write sensor specified address, data. return value: 0->fail, other values->success
	CMTB_CMD_GET_CMTB_BINARY_VERSION,   //get board binary version. return value: 0->fail, other values->success

	CMTB_CMD_SET_PREVIEW_SIZE,          // Preview frame size
	CMTB_CMD_SET_PREVIEW_FORMAT,        // Set preview/yuv capture format(yuv format).
	CMTB_CMD_SET_REGISTER,              // Set sensor/isp register
	CMTB_CMD_JPEG_CAPTURE,              // set jpeg format + capture
} CMTB_CMD;

// focus mode
typedef enum
{
	CAM_FOCUS_AUTO_ONESHOT            = 0x0,  // need CAM_CMD_START_FOCUS trigger focus
	CAM_FOCUS_AUTO_CONTINUOUS_VIDEO   = 0x2,
	CAM_FOCUS_AUTO_CONTINUOUS_PICTURE = 0x3,  // use CAM_CMD_START_FOCUS to pasue CAF, CAM_CMD_CANCEL_FOCUS to resume CAF
	CAM_FOCUS_SUPERMACRO              = 0x4,
	CAM_FOCUS_MACRO                   = 0x5,  // need CAM_CMD_START_FOCUS trigger focus
	CAM_FOCUS_HYPERFOCAL              = 0x6,
	CAM_FOCUS_INFINITY                = 0x7,
	CAM_FOCUS_MANUAL                  = 0x8,

	CAM_FOCUS_NUM,

	CAM_FOCUS_LIMIT                  = 0x7fffffff,
} CAM_FocusMode;

class __declspec(dllexport) CMrvlCMTBClient  //camera control 
{
public:
	CMrvlCMTBClient(void);
	~CMrvlCMTBClient(void);
	int Initialize(int nBoardIndex, char *strAdbID);
	void Cleanup();
	int CameraOn(int cameraId); //Start Camera ( multi camera is not allowed ). return value: 0->fail, other values->success
	int CameraOff(); //Exit Camera. return value: 0->fail, other values->success
	int CamReset(); //camera module hardware reset ( fd will be still valid ). return value: 0->fail, other values->success
	int AppReset(); //CMTB app reset ( fd will be no longer valid ). return value: 0->fail, other values->success
	int BoardReset(); //board Reset ( board reboot ). return value: 0->fail, other values->success
	int Set_imageType(int width, int height, CAM_ImageFormat format); //set image type. return value: 0->fail, other values->success
	int Get_CaptureData(char *data, int *size); //capture and send an image, data area should be pre-allocated, and variable size is used to store the actual size of the image. return value: 0->fail, other values->success
	int SetAELockUnLock(int on); //lock AE/unlock AE. return value: 0->fail, other values->success
	int SetAWBLockUnLock(int on); //lock AWB/unlock AWB. return value: 0->fail, other values->success
	int SetShadingLockOnOff(int on); //Turn on LSC/Turen off LSC. return value: 0->fail, other values->success
	int SetBPCOnOff(int on); //Turn on BPC(SOCKET s, DPC)/Turn off BPC(SOCKET s, DPC). return value: 0->fail, other values->success
	int SetEVControl(int value); //Set exposure value. return value: 0->fail, other values->success
	int TriggerSingleAF(); //Focus a target object. return value: 0->fail, other values->success
	int SetAFPosition(int pos); //Set AF position. return value: 0->fail, other values->success
	int GetAFPosition(int *pos); //Read current focus position. return value: 0->fail, other values->success
	int GetAFStatus(CAM_FocusState *state); //get AF status(AF idle or running). return value: 0->fail, other values->success
	int GetAFScore(int *score); //get AF score(SOCKET s, AF filter output value). return value: 0->fail, other values->success
	int SetAFMode(CAM_FocusMode mode); //set to Manual AF mode/set to Single AF mode/set to Continuous AF mode/set to Macro AF mode. return value: 0->fail, other values->success
	int EraseFROM(int start, int length); //erase eeprom specified area. return value: 0->fail, other values->success
	int ReadFROM(int start, int length, char *data); //read eeprom specified area. return value: 0->fail, other values->success
	int WriteFROM(int start, int length, char *data); //write eeprom specified area. return value: 0->fail, other values->success
	int SetMode_StartReadFROM(int mode); //set FROM read mode. return value: 0->fail, other values->success
	int ReadAFI2CData(int address, char *data); //Read VCM specified address. return value: 0->fail, other values->success
	int WriteAFI2CData(int address, char value); //Write VCM specified address, data. return value: 0->fail, other values->success
	int ReadSensorI2CData(int address, char *data); //Read sensor specified address. return value: 0->fail, other values->success
	int WriteSensorI2CData(int address, char value); //Write sensor specified address, data. return value: 0->fail, other values->success

	int GetOTPdata(char* data, int &nDataSize);

	int GetBoardBinaryVersion(char *string); //get board binary version. return value: 0->fail, other values->success
	char* GetHostBinaryVersion(); //get host binary version. return value: 0->fail, other values->success

	int SetPreviewCallFunc(void (*func)(void*, int*));

	int ConnectCMTB_Preview();
	int	ConnectCMTB_Command();
	int DisconnectCMTB_Preview();
	int DisconnectCMTB_Command();
	int sendData(CMTB_CMD commandId, char *data, int len, int headerId, int type);
	int recvData(CMTB_CMD *commandId, char *data, int *len, int *headerId, int *type, int nSocketSelect = 0);
	int CommunicateWithCMTB(CMTB_CMD commandId, int intParam1, int intParam2, int intParam3, char *outData, int *outSize);

	BOOL m_bWSAInitialized;
	SOCKET m_Socket_Command;
	SOCKET m_Socket_Preview;
	void (*m_pPreviewCallback)(void*, int*);

private:
	int isRun_JigServer();
	int RunJigServer();
	void ClearOTPdata();
	int KillPS_JigServer();
	int m_nPortNum_Command;
	int m_nPortNum_Preview;
	char m_strAdbId[64];
	DWORD m_nCameraOffTime;

};