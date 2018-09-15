#pragma once

#pragma pack(8)
 
typedef struct In_Params{	
	unsigned short img_width, img_height; 
	float* Oblect_xyz;
	int Oblect_xyzSize;
	float* imgPoint_xy ;
	int imgPoint_xySize;
	float pixel_size_um;
	float focal_length_mm;
	 
} IN_PARAMS;

typedef struct Out_Params{	
	 

	float rotation_rad, tilt_x_rad, tilt_y_rad;
	float shift_x,shift_y,shift_z;
	unsigned short opticalcenter_x_pixel, opticalcenter_y_pixel;
} OUT_PARAMS;

#pragma pack()


#ifdef MEMS_EXPORTS
	#define MEMS_API extern "C" __declspec(dllexport)
#else
	#define MEMS_API extern "C" __declspec(dllimport)
#endif

 

MEMS_API void DualCCM_TwoLayerCalibrate( IN_PARAMS in_val, OUT_PARAMS *out_val);
