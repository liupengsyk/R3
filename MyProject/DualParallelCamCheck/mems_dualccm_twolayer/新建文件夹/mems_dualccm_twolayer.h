#pragma once

#pragma pack(8)

typedef struct In_Params{	
	unsigned short img_width, img_height;
	unsigned char *img_rgb_near, *img_rgb_far;//same img
	float roiwidth_ratio_near, roiwidth_ratio_far;//大、小特征点占chart 比例
	float roifield_max_near, roifield_min_near, roifield_max_far, roifield_min_far;//roi 以视场计算
	float dot_distance_x_mm, dot_distance_y_mm; //特征点间距	
	float chart_distance_mm;
	float focal_length_mm, pixel_size_um;
} IN_PARAMS;

typedef struct Out_Params{	
	float dot_locLT_x_near, dot_locLT_y_near, dot_locLT_x_far, dot_locLT_y_far;//left top
	float dot_locT_x_near, dot_locT_y_near, dot_locT_x_far, dot_locT_y_far;//top mid
	float dot_locRT_x_near, dot_locRT_y_near, dot_locRT_x_far, dot_locRT_y_far;//right top
	float dot_locLB_x_near, dot_locLB_y_near, dot_locLB_x_far, dot_locLB_y_far;//left bot
	float dot_locB_x_near, dot_locB_y_near, dot_locB_x_far, dot_locB_y_far;//left mid
	float dot_locRB_x_near, dot_locRB_y_near, dot_locRB_x_far, dot_locRB_y_far;//right bot

	float rotation_rad, tilt_x_rad, tilt_y_rad;
	unsigned short opticalcenter_x_pixel, opticalcenter_y_pixel;//中心坐标
} OUT_PARAMS;

#pragma pack()


#ifdef MEMS_EXPORTS
	#define MEMS_API extern "C" __declspec(dllexport)
#else
	#define MEMS_API extern "C" __declspec(dllimport)
#endif

MEMS_API void Mems_DualCCM_TwoLayer( IN_PARAMS in_val, OUT_PARAMS *out_val );
