//----------------------------------------------------------------------------
//Name            : IPcomLicense.h
//Function        : Head file for interface class definition
//CreateDate      : 2012-10-18
//Author          : Unknown
//Company         : Huawei Technologies Co.,Ltd
//Modification    : 
//----------------------------------------------------------------------------
#if !defined(__PCOM_INTERFACE_IPcomLicense_H_INCLUDED__)
#define __PCOM_INTERFACE_IPcomLicense_H_INCLUDED__
//lint -library
//comment:do not lint this lib
namespace PcomLicense
{

#define IID_IPcomLicense "IPcomLicense"

interface IPcomLicense : public IUnknown
{

	//virtual HRESULT __stdcall CheckBlemish(const CString& strImgPath,const CString& strOutPath,const double& p1,const double& p2);	

	virtual HRESULT __stdcall CheckBlemish(const CString& strImgPath,const CString& strOutPath,const double& RangeThreshold,const double& medianThreshold,const double& minBetaGray,const double& hv_minBetaFFT) = 0;	

	//virtual HRESULT __stdcall CheckBlemishImage(HObject ho_Image,HObject *ho_resultImage,const double& RangeThreshold,const double& medianThreshold,const double& minBetaGray,const double& hv_minBetaFFT) = 0;	
	
//{{PCOM_INSERT_LOCATION}}
};

}
#endif // !defined(__PCOM_INTERFACE_IPcomLicense_H_INCLUDED__)
