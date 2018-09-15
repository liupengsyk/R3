#pragma once

#pragma pack(8)

typedef struct In_Params{	
	unsigned short img_width, img_height;
	unsigned char *img_rgb;
} IN_PARAMS;

#pragma pack()


#ifdef MEMS_EXPORTS
	#define MEMS_API extern "C" __declspec(dllexport)
#else
	#define MEMS_API extern "C" __declspec(dllimport)
#endif

MEMS_API void Mems_NV21_Converter( IN_PARAMS in_val, unsigned char *out_yuv );
