 
#pragma once
 
//extern "C"  BOOL LoadData(unsigned short* raw_main_near,unsigned short* raw_main_far,unsigned short* raw_sub_near,unsigned short* raw_sub_far);    // verify 
extern "C"  int IQC(int* gVerify_Items, USHORT* raw_main_near, USHORT* raw_main_far,int main_w, int main_h,USHORT* raw_sub_near,USHORT* raw_sub_far,int sub_w,int sub_h,char* ConfigName); 