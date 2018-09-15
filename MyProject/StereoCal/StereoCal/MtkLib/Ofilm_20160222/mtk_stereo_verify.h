/* input of IQC

gVerify_Items:	Store the output information. The array length is equal to 180. 
main_raw_near:	The raw image (bit depth: 16) of main camera captured from 20 cm.
main_raw_far:	The raw image (bit depth: 16) of main camera captured from 90 cm.
main_w:			The width of image in main camera.
main_h:			main_h
sub_raw_near:	The raw image (bit depth: 16) of sub camera captured from 20 cm.
sub_raw_far:	The raw image (bit depth: 16) of sub camera captured from 90 cm.
sub_w:			The width of image in sub camera.
sub_h:			The height of image in sub camera.
configname:		The file name of the configuration file.

*/

extern "C"
{
	__declspec( dllexport ) int IQC(int *gVerify_Items, unsigned short *main_raw_near, unsigned short *main_raw_far, int main_w, int main_h, unsigned short *sub_raw_near, unsigned short *sub_raw_far, int sub_w, int sub_h, char Config[]);
}
