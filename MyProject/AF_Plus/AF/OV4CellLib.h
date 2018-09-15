// OV4CellLib.h : main header file for the OV4CellLib DLL
//

#pragma once

#ifdef OV4CELL_DLL_EXPORTS
#define OV4C_API __declspec(dllexport)
#else
#define OV4C_API __declspec(dllimport)
#endif

typedef int (*ov4c_i2c_callback)(void*, int, int);

const int MAX_ROI_NUM = 64;

struct ROI_PARA
{
	int w, h;
	int x[MAX_ROI_NUM], y[MAX_ROI_NUM];
	int roi_x_num;
	int roi_y_num;		// roi_x_num * roi_y_num <= MAX_ROI_NUM
	int enable_dpc;
	int enable_otp_dpc;
	int *otp_dpc_data;
	int otp_dpc_data_length;

	ROI_PARA()
	{
		w = h = 200;
		roi_x_num = 18;
		roi_y_num = 1;
		enable_dpc = 0;
		enable_otp_dpc = 0;
		otp_dpc_data = NULL;
		otp_dpc_data_length = 0;
		memset(x, 0, MAX_ROI_NUM*sizeof(int));
		memset(y, 0, MAX_ROI_NUM*sizeof(int));
	}
};

OV4C_API int ov4c_get_version();
OV4C_API int ov4c_set_i2c_callback(void* handle, int sensor_id, ov4c_i2c_callback i2c_callback);
OV4C_API int ov4c_init(ROI_PARA *roi_para);
OV4C_API int ov4c_20880_transform_bayer_raw10(unsigned short *raw_in, unsigned short *raw_out, ROI_PARA *roi_para);
OV4C_API int ov4c_20880_crosstalk_calibration_raw10(unsigned short *raw, unsigned char *eeprom_buf, int &size, int blc, double &max_gain, double &min_gain, double criteria = 9.0);
OV4C_API int ov4c_20880_crosstalk_verification_raw10(unsigned short *raw, unsigned char *eeprom_buf, int size, int blc, double &max_gain, double &min_gain, double criteria = 0.6);
OV4C_API int ov4c_16885_transform_bayer_raw10(unsigned short *raw_in, unsigned short *raw_out, ROI_PARA *roi_para);
OV4C_API int ov4c_16885_crosstalk_calibration_raw10(unsigned short *raw, unsigned char *eeprom_buf, int &size, int blc, double &max_gain, double &min_gain, double criteria = 9.0);
OV4C_API int ov4c_16885_crosstalk_verification_raw10(unsigned short *raw, unsigned char *eeprom_buf, int size, int blc, double &max_gain, double &min_gain, double criteria = 0.6);
OV4C_API void ov4c_release();
