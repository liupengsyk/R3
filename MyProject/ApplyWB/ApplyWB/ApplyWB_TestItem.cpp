#include "StdAfx.h"
#include "ApplyWB_TestItem.h"
#include "ImageProc.h"

#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new ApplyWB_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
ApplyWB_TestItem::ApplyWB_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化


	//.....
}

ApplyWB_TestItem::~ApplyWB_TestItem(void)
{

}


int ApplyWB_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	return 0;
}


void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
void ApplyWB_TestItem::GetAWBInfoRaw8(ImageInfo img)
{

	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	BYTE * Raw8buffer=new BYTE[width*height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer buffer 申请失败!"),COLOR_RED,200);
		return;
	}
	Raw10toRaw8(img.Raw_buffer,Raw8buffer,width*height);
	//calculate rawWidth & rawHeight  , size of ROI 
	int    rawWidth  = width / (pDlg->m_ROIWidth*2) * 2;    //multiply two is to make sure that width & height is even  because of bayer order
	int    rawHeight = height / (pDlg->m_ROIWidth*2) * 2; 

	int  StartX = (width  - rawWidth ) / 4 * 2;
	int  StartY = (height  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	rect.left       = StartX;
	rect.right     = EndX;
	rect.top       = StartY;
	rect.bottom = EndY;

	
	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;


	if ( img.RawFormat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumR  += Raw8buffer[Pos1];
				SumGr += Raw8buffer[Pos1 + 1];
				SumGb += Raw8buffer[Pos2];
				SumB  += Raw8buffer[Pos2 + 1];
				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGr += Raw8buffer[Pos1];
				SumR  += Raw8buffer[Pos1 + 1];
				SumB  += Raw8buffer[Pos2];
				SumGb += Raw8buffer[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGb += Raw8buffer[Pos1];
				SumB  += Raw8buffer[Pos1 + 1];
				SumR  += Raw8buffer[Pos2];
				SumGr += Raw8buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumB  += Raw8buffer[Pos1];
				SumGb += Raw8buffer[Pos1 + 1];
				SumGr += Raw8buffer[Pos2];
				SumR  += Raw8buffer[Pos2 + 1];

				Count++;
				//so,OV8865 run to here
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;
     
	m_AWBInfo.Unit_Gr= int(Gr);
	m_AWBInfo.Unit_Gb = int(Gb);
	m_AWBInfo.Unit_R  = int(R);
	m_AWBInfo.Unit_B  = int(B);
	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if(GAverFlag==1)
	{
		m_AWBInfo.RGain = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==2)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==3)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gr-pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) +AA );
	}
	
	if (pDlg->m_Channel_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC)*m_AWBSpec.RIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC)*m_AWBSpec.GrIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC)*m_AWBSpec.GbIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC)*m_AWBSpec.BIndex /10000.0 + 0.5);
	}

	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC));
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC));
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC));
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC));
	}
	


	if (pDlg->m_Channel_EN)
	{
		if(GAverFlag==1)
		{
			m_AWBInfo.RGain_fin = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_BG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_GG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gr/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
		}
		else if (GAverFlag==2)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gr)+0.5);
			m_AWBSpec.Typical_BG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gr)+0.5);
			m_AWBSpec.Typical_GG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gb/(m_AWBSpec.Typical_Gr)+0.5);
		}
		else if (GAverFlag==3)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gb_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gr_fin)/( m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_BG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_GG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gr/(m_AWBSpec.Typical_Gb)+0.5);
		}
		CString temp;
		temp.Format(_T("Typical_R=%d\nTypical_Gr=%d\nTypical_Gb=%d\nTypical_B=%d"),m_AWBSpec.Typical_R,m_AWBSpec.Typical_Gr,m_AWBSpec.Typical_Gb,m_AWBSpec.Typical_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.RGain_fin = int(m_AWBInfo.RGain * m_AWBSpec.RGIndex /10000.0 + 0.5);
		m_AWBInfo.BGain_fin = int(m_AWBInfo.BGain * m_AWBSpec.BGIndex /10000.0 + 0.5);
		m_AWBInfo.GGain_fin = int(m_AWBInfo.GGain * m_AWBSpec.GGIndex /10000.0 + 0.5);
	}

	SAFE_DELETE_ARRAY(Raw8buffer);
}

void ApplyWB_TestItem::GetAWBInfoRaw10(ImageInfo img)
{
	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	//calculate rawWidth & rawHeight  , size of ROI 
	int    rawWidth  = width / (pDlg->m_ROIWidth*2) * 2;    //multiply two is to make sure that width & height is even  because of bayer order
	int    rawHeight = height / (pDlg->m_ROIWidth*2) * 2; 

	int  StartX = (width  - rawWidth ) / 4 * 2;
	int  StartY = (height  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	rect.left       = StartX;
	rect.right     = EndX;
	rect.top       = StartY;
	rect.bottom = EndY;


	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;


	if ( img.RawFormat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumR  += img.Raw_buffer[Pos1];
				SumGr += img.Raw_buffer[Pos1 + 1];
				SumGb += img.Raw_buffer[Pos2];
				SumB  += img.Raw_buffer[Pos2 + 1];
				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGr += img.Raw_buffer[Pos1];
				SumR  += img.Raw_buffer[Pos1 + 1];
				SumB  += img.Raw_buffer[Pos2];
				SumGb += img.Raw_buffer[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGb += img.Raw_buffer[Pos1];
				SumB  += img.Raw_buffer[Pos1 + 1];
				SumR  += img.Raw_buffer[Pos2];
				SumGr += img.Raw_buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumB  += img.Raw_buffer[Pos1];
				SumGb += img.Raw_buffer[Pos1 + 1];
				SumGr += img.Raw_buffer[Pos2];
				SumR  += img.Raw_buffer[Pos2 + 1];

				Count++;
				//so,OV8865 run to here
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;
     
	m_AWBInfo.Unit_Gr= int(Gr);
	m_AWBInfo.Unit_Gb = int(Gb);
	m_AWBInfo.Unit_R  = int(R);
	m_AWBInfo.Unit_B  = int(B);

	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if (GAverFlag==1)
	{
		m_AWBInfo.RGain = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==2)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==3)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gr-pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) +AA );
	}
	
	if (pDlg->m_Channel_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC)*m_AWBSpec.RIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC)*m_AWBSpec.GrIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC)*m_AWBSpec.GbIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC)*m_AWBSpec.BIndex /10000.0 + 0.5);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC));
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC));
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC));
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC));
	}
	
	if (pDlg->m_Channel_EN)
	{
		if(GAverFlag==1)
		{
			m_AWBInfo.RGain_fin = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_BG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_GG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gr/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
		}
		else if (GAverFlag==2)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gr)+0.5);
			m_AWBSpec.Typical_BG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gr)+0.5);
			m_AWBSpec.Typical_GG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gb/(m_AWBSpec.Typical_Gr)+0.5);
		}
		else if (GAverFlag==3)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gb_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gr_fin)/( m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_BG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_GG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gr/(m_AWBSpec.Typical_Gb)+0.5);
		}

		CString temp;
		temp.Format(_T("Typical_R=%d\nTypical_Gr=%d\nTypical_Gb=%d\nTypical_B=%d"),m_AWBSpec.Typical_R,m_AWBSpec.Typical_Gr,m_AWBSpec.Typical_Gb,m_AWBSpec.Typical_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.RGain_fin = int(m_AWBInfo.RGain * m_AWBSpec.RGIndex /10000.0 + 0.5);
		m_AWBInfo.BGain_fin = int(m_AWBInfo.BGain * m_AWBSpec.BGIndex /10000.0 + 0.5);
		m_AWBInfo.GGain_fin = int(m_AWBInfo.GGain * m_AWBSpec.GGIndex /10000.0 + 0.5);
	}
}

BOOL ApplyWB_TestItem::CheckAWBRange()
{
	if ((m_AWBInfo.RGain_fin > m_AWBSpec.RGainHigh) || (m_AWBInfo.RGain_fin< m_AWBSpec.RGainlow) )
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("RGain值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if ((m_AWBInfo.BGain_fin > m_AWBSpec.BGainHigh) || (m_AWBInfo.BGain_fin < m_AWBSpec.BGainlow) )
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("BGain值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if ((m_AWBInfo.GGain_fin > m_AWBSpec.GGainHigh) || (m_AWBInfo.GGain_fin < m_AWBSpec.GGainlow) )
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("GGain值不在范围!"),COLOR_RED,200);
		return 0;
	}

	if (m_AWBInfo.Unit_R_fin > m_AWBSpec.RHigh || m_AWBInfo.Unit_R_fin < m_AWBSpec.RLow)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("R值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.Unit_Gr_fin > m_AWBSpec.GrHigh || m_AWBInfo.Unit_Gr_fin < m_AWBSpec.GrLow)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Gr值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.Unit_Gb_fin > m_AWBSpec.GbHigh || m_AWBInfo.Unit_Gb_fin < m_AWBSpec.GbLow)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Gb值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.Unit_B_fin > m_AWBSpec.BHigh || m_AWBInfo.Unit_B_fin < m_AWBSpec.BLow)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("B值不在范围!"),COLOR_RED,200);
		return 0;
	}
	return 1;
}
/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int ApplyWB_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
	ApplyAWBFlag=TRUE;
	CheckCount=0;
	Beforedistance=0.0;
	Afterdistance=0.0;

	m_AWBInfo.Unit_R_fin = -1;
	m_AWBInfo.Unit_Gr_fin= -1;
	m_AWBInfo.Unit_Gb_fin= -1;
	m_AWBInfo.Unit_B_fin = -1;

	m_AWBInfo.RGain_fin = -1;
	m_AWBInfo.BGain_fin = -1;
	m_AWBInfo.GGain_fin = -1;

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int ApplyWB_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	m_AWBSpec.RGIndex  = pDlg->m_RGIndex;
	m_AWBSpec.BGIndex  = pDlg->m_BGIndex;
	m_AWBSpec.GGIndex  = pDlg->m_GGIndex;

	m_AWBSpec.RIndex  = pDlg->m_RIndex;
	m_AWBSpec.GrIndex  = pDlg->m_GrIndex;
	m_AWBSpec.GbIndex  = pDlg->m_GbIndex;
	m_AWBSpec.BIndex  = pDlg->m_BIndex;

	TestResult=TEST_PASS;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID); 
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧1失败!"));
		SetResult(RESULT_FAIL);
		TestResult=TEST_FAIL;
		return 0;
	}
	if (ApplyAWBFlag==TRUE)
	{

		//Save Image
		USES_CONVERSION;
		if (pDlg->m_SaveImage)
		{
			FilePath = m_pInterface->Ctrl_GetCurrentPath()+_T("\\ApplyBefore\\");
			_mkdir(T2A(FilePath));
			FileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyBefore.raw");
			CString BmpFileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyBefore.bmp");
			TempMsg.Format(_T("%d_%s"),CamID,FileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,3,NULL);   //BYTE *imgBuffer
			TempMsg.Format(_T("%d_%s"),CamID,BmpFileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,2,NULL);   //BYTE *imgBuffer
		}

		//Calculate AWB
		if (pDlg->m_QTITool)
		{
			if(!GetAWBData(img))
			{
			   m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			   m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("GetAWBData Fail"));
			   SetResult(RESULT_FAIL);
			   TestResult=TEST_FAIL;
			   return 0;
			}
		}
		else
		{
			if (pDlg->m_Raw8_EN)
				GetAWBInfoRaw8(img);
			else
				GetAWBInfoRaw10(img);
		}
		
		OTP_R=m_AWBInfo.Unit_R_fin;
		OTP_Gr=m_AWBInfo.Unit_Gr_fin;
		OTP_Gb=m_AWBInfo.Unit_Gb_fin;
		OTP_B=m_AWBInfo.Unit_B_fin;

		OTP_RGain=m_AWBInfo.RGain_fin;
		OTP_BGain=m_AWBInfo.BGain_fin;
		OTP_GGain=m_AWBInfo.GGain_fin;

		TempMsg.Format(_T("RawR=%d\nRawGr=%d\nRawGb=%d\nRawB=%d"),m_AWBInfo.Unit_R,m_AWBInfo.Unit_Gr,m_AWBInfo.Unit_Gb,m_AWBInfo.Unit_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("BLC = %d"),pDlg->m_BLC);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("R_OTP=%d\nGr_OTP=%d\nGb_OTP=%d\nB_OTP=%d"),OTP_R,OTP_Gr,OTP_Gb,OTP_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		TempMsg.Format(_T("Rindex=%d\nGrindex=%d\nGbindex=%d Bindex=%d"),m_AWBSpec.RIndex,m_AWBSpec.GrIndex,m_AWBSpec.GbIndex,m_AWBSpec.BGIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("RGain=%d index=%d\nBGain=%d index=%d\nGGain=%d index=%d"),OTP_RGain,m_AWBSpec.RGIndex,OTP_BGain,m_AWBSpec.BGIndex,OTP_GGain,m_AWBSpec.GGIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		if (!CheckAWBRange())
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB Value not in range!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Fail!"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;
		}

		float m_OTPDistance =100;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Typical_RG)*1.0/m_AWBSpec.Typical_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Typical_BG)*1.0/m_AWBSpec.Typical_BG,2)+pow((m_AWBInfo.GGain_fin-m_AWBSpec.Typical_GG)*1.0/m_AWBSpec.Typical_GG,2));

		}
		else
		m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Typical_RG)*1.0/m_AWBSpec.Typical_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Typical_BG)*1.0/m_AWBSpec.Typical_BG,2));

		TempMsg.Format(_T("Apply Before: OTPDistance=%4.2f"),m_OTPDistance*100);
		Beforedistance=m_OTPDistance*100;
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		if (m_OTPDistance * 1000 > pDlg->m_OTP_BeforeDistance)
		{
			TempMsg.Format(_T("Distance 超出范围！"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("m_OTPDistance Fail"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;	
		}
	}
	if (ApplyAWBFlag==TRUE)
	{
		m_pInterface->Ctrl_PauseGrabFrame(CamID);
		Sleep(500);
		m_pInterface->Sensor_ApplyAWBGain(CamID,OTP_RGain,OTP_BGain,m_AWBSpec.Typical_RG,m_AWBSpec.Typical_BG);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(1000);
		ApplyAWBFlag=FALSE;
		return 1;
	}

	
	return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int ApplyWB_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	if (CheckCount<4)
	{
		CheckCount++;
		return 1;
	}
	if (TestResult==TEST_PASS)
	{
		ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID); 
		if (NULL == img.Raw_buffer)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("抓帧2失败!"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;
		}

		//Save Image
		USES_CONVERSION;
		if (pDlg->m_SaveImage)
		{
			FilePath = m_pInterface->Ctrl_GetCurrentPath()+_T("\\ApplyAfter\\");
			_mkdir(T2A(FilePath));
			FileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyAfter.raw");
			CString BmpFileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyAfter.bmp");
			TempMsg.Format(_T("%d_%s"),CamID,FileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,3,NULL);   //BYTE *imgBuffer
			TempMsg.Format(_T("%d_%s"),CamID,BmpFileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,2,NULL);   //BYTE *imgBuffer
		}

		//Calculate AWB
		if (pDlg->m_QTITool)
		{
			if(!GetAWBData(img))
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
				SetResult(RESULT_FAIL);
				TestResult=TEST_FAIL;
				return 0;
			}
		}
		else
		{
			if (pDlg->m_Raw8_EN)
				GetAWBInfoRaw8(img);
			else
				GetAWBInfoRaw10(img);
		}

		TempMsg.Format(_T("RawR=%d\nRawGr=%d\nRawGb=%d\nRawB=%d"),m_AWBInfo.Unit_R,m_AWBInfo.Unit_Gr,m_AWBInfo.Unit_Gb,m_AWBInfo.Unit_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("R_fin=%d\nGr_fin=%d\nGb_fin=%d\nB_fin=%d"),m_AWBInfo.Unit_R_fin,m_AWBInfo.Unit_Gr_fin,m_AWBInfo.Unit_Gb_fin,m_AWBInfo.Unit_B_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("RGain=%d\nBGain=%d\nGGain=%d"),m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		TempMsg.Format(_T("RGain=%d\nBGain=%d\nGGain=%d"),m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		TempMsg.Format(_T("Typical_RGain=%d\nTypical_BGain=%d\nTypical_GGain=%d"),m_AWBSpec.Typical_RG,m_AWBSpec.Typical_BG,m_AWBSpec.Typical_GG);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		 
		float m_OTPDistance = 100;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Typical_RG)*1.0/m_AWBSpec.Typical_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Typical_BG)*1.0/m_AWBSpec.Typical_BG,2)+ pow((m_AWBInfo.GGain_fin-m_AWBSpec.Typical_GG)*1.0/m_AWBSpec.Typical_GG,2));
		}
		else
		m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Typical_RG)*1.0/m_AWBSpec.Typical_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Typical_BG)*1.0/m_AWBSpec.Typical_BG,2));
		Afterdistance=m_OTPDistance*100;
		TempMsg.Format(_T("Apply After: OTPDistance=%4.2f"),m_OTPDistance*100);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		if (m_OTPDistance * 1000 > pDlg->m_OTP_Distance)
		{
			TempMsg.Format(_T("Distance 超出范围！"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超出范围！"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;	
		}
		//保存烧录数据
		SaveAWBData();
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Pass!"));
		SetResult(RESULT_PASS);
	}
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int ApplyWB_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_count=ReadConfigInt(_T("count"),12);
	CString suffix=_T("");
	if (pDlg->m_count!=0)
		suffix.Format(_T("_%d"),pDlg->m_count);
	CString CurrentName=GetName();
	SetName(_T("WBSetting"));
	pDlg->m_QTITool=ReadConfigInt(_T("QTITool"),0);
	pDlg->m_flow=ReadConfigInt(_T("flow"),1);
	GAverFlag=ReadConfigInt(_T("GAverFlag"),1);
	if (GAverFlag==1)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO1))->SetCheck(1);
	}
	else if (GAverFlag==2)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	else if (GAverFlag==3)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO3))->SetCheck(1);
	}
	pDlg->m_GG_GainEn=ReadConfigInt(_T("GG_GainEn"),0);
	pDlg->m_OTP_BeforeDistance=ReadConfigInt(_T("OTP_BeforeDistance")+suffix,0);
	pDlg->m_RGIndex=ReadConfigInt(_T("RG_index")+suffix,10000);
	pDlg->m_BGIndex  = ReadConfigInt(_T("BG_index")+suffix,10000);
	pDlg->m_GGIndex  = ReadConfigInt(_T("GG_index")+suffix,10000);

	pDlg->m_RIndex  = ReadConfigInt(_T("R_index")+suffix,10000);
	pDlg->m_GrIndex  = ReadConfigInt(_T("Gr_index")+suffix,10000);
	pDlg->m_GbIndex  = ReadConfigInt(_T("Gb_index")+suffix,10000);
	pDlg->m_BIndex  = ReadConfigInt(_T("B_index")+suffix,10000);

	//Typical Gain
	pDlg->m_TypicalRG  = ReadConfigInt(_T("Typical_RG")+suffix,200);
	pDlg->m_TypicalBG  = ReadConfigInt(_T("Typical_BG")+suffix,200);
	pDlg->m_TypicalGG  = ReadConfigInt(_T("Typical_GG")+suffix,200);

	m_AWBSpec.Typical_RG  = pDlg->m_TypicalRG ;
	m_AWBSpec.Typical_BG = pDlg->m_TypicalBG;
	m_AWBSpec.Typical_GG  = pDlg->m_TypicalGG;

	//Typical Channel
	pDlg->m_TypicalR  = ReadConfigInt(_T("Typical_R")+suffix,200);
	pDlg->m_TypicalGr  = ReadConfigInt(_T("Typical_Gr")+suffix,200);
	pDlg->m_TypicalGb  = ReadConfigInt(_T("Typical_Gb")+suffix,200);
	pDlg->m_TypicalB  = ReadConfigInt(_T("Typical_B")+suffix,200);
	m_AWBSpec.Typical_R  = pDlg->m_TypicalR ;
	m_AWBSpec.Typical_Gr  = pDlg->m_TypicalGr;
	m_AWBSpec.Typical_Gb  = pDlg->m_TypicalGb  ;
	m_AWBSpec.Typical_B  = pDlg->m_TypicalB ;

	//Channel Range
	pDlg->m_RLow=ReadConfigInt(_T("R_min")+suffix,30);
	pDlg->m_RHigh=ReadConfigInt(_T("R_max")+suffix,220);
	pDlg->m_GrLow=ReadConfigInt(_T("Gr_min")+suffix,30);
	pDlg->m_GrHigh=ReadConfigInt(_T("Gr_max")+suffix,220);
	pDlg->m_GbLow=ReadConfigInt(_T("Gb_min")+suffix,30);
	pDlg->m_GbHigh=ReadConfigInt(_T("Gb_max")+suffix,220);
	pDlg->m_BLow=ReadConfigInt(_T("B_min")+suffix,30);
	pDlg->m_BHigh=ReadConfigInt(_T("B_max")+suffix,220);

	m_AWBSpec.RHigh=pDlg->m_RHigh;
	m_AWBSpec.RLow=pDlg->m_RLow;
	m_AWBSpec.GrHigh=pDlg->m_GrHigh;
	m_AWBSpec.GrLow=pDlg->m_GrLow;
	m_AWBSpec.GbHigh=pDlg->m_GbHigh;
	m_AWBSpec.GbLow=pDlg->m_GbLow;
	m_AWBSpec.BHigh=pDlg->m_BHigh;
	m_AWBSpec.BLow=pDlg->m_BLow;


	//Gain Range
	pDlg->m_RGLow=ReadConfigInt(_T("RG_min")+suffix,220);
	pDlg->m_RGHigh=ReadConfigInt(_T("RG_max")+suffix,1000);
	pDlg->m_BGLow=ReadConfigInt(_T("BG_min")+suffix,220);
	pDlg->m_BGHigh=ReadConfigInt(_T("BG_max")+suffix,1000);
	pDlg->m_GGLow=ReadConfigInt(_T("GG_min")+suffix,220);
	pDlg->m_GGHigh=ReadConfigInt(_T("GG_max")+suffix,1000);


	m_AWBSpec.RGainHigh=pDlg->m_RGHigh;
	m_AWBSpec.RGainlow=pDlg->m_RGLow;
	m_AWBSpec.BGainHigh=pDlg->m_BGHigh;
	m_AWBSpec.BGainlow=pDlg->m_BGLow;
	m_AWBSpec.GGainHigh=pDlg->m_GGHigh;
	m_AWBSpec.GGainlow=pDlg->m_GGLow;


	pDlg->m_Raw8_EN  = ReadConfigInt(_T("Raw8_EN"),1);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_ROIWidth=ReadConfigInt(_T("Ratio"),5);
	pDlg->m_Ratio=ReadConfigInt(_T("index"),512);

	SetName(CurrentName);
	pDlg->m_OTP_Distance  = ReadConfigInt(_T("OTP_Distance"),50);
	pDlg->m_OTP_BeforeDistance  = ReadConfigInt(_T("m_OTP_BeforeDistance"),150);
	pDlg->m_Channel_EN  = ReadConfigInt(_T("Channel_EN"),200);
	pDlg->m_Gain_EN  = ReadConfigInt(_T("Gain_EN"),200);
	pDlg->m_Raw10_EN  = ReadConfigInt(_T("Raw10_EN"),1);
	pDlg->m_SaveImage  = ReadConfigInt(_T("SaveImage"),200);


	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int ApplyWB_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	
	WriteConfigInt(_T("count"),pDlg->m_count);

	CString suffix=_T("");
	if (pDlg->m_count!=0)
		suffix.Format(_T("_%d"),pDlg->m_count);

	CString CurrentName=GetName();
	SetName(_T("WBSetting"));
	WriteConfigInt(_T("QTITool"),pDlg->m_QTITool);
	WriteConfigInt(_T("flow"),pDlg->m_flow);
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		GAverFlag=1;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		GAverFlag=2;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		GAverFlag=3;
	}
	WriteConfigInt(_T("GAverFlag"),GAverFlag);
	WriteConfigInt(_T("GG_GainEn"),pDlg->m_GG_GainEn);
	WriteConfigInt(_T("OTP_BeforeDistance"+suffix),pDlg->m_OTP_BeforeDistance);
	//Typical Channel
	WriteConfigInt(_T("Typical_R")+suffix, pDlg->m_TypicalR);
	WriteConfigInt(_T("Typical_Gr")+suffix, pDlg->m_TypicalGr);
	WriteConfigInt(_T("Typical_Gb")+suffix, pDlg->m_TypicalGb);
	WriteConfigInt(_T("Typical_B")+suffix, pDlg->m_TypicalB);

	//Typical Gain
	WriteConfigInt(_T("Typical_RG")+suffix, pDlg->m_TypicalRG);
	WriteConfigInt(_T("Typical_BG")+suffix, pDlg->m_TypicalBG);
	WriteConfigInt(_T("Typical_GG")+suffix, pDlg->m_TypicalGG);

	//Channel Range
	WriteConfigInt(_T("R_max")+suffix, pDlg->m_RHigh);
	WriteConfigInt(_T("R_min")+suffix, pDlg->m_RLow);
	WriteConfigInt(_T("Gr_max")+suffix, pDlg->m_GrHigh);
	WriteConfigInt(_T("Gr_min")+suffix, pDlg->m_GrLow);
	WriteConfigInt(_T("Gb_max")+suffix, pDlg->m_GbHigh);
	WriteConfigInt(_T("Gb_min")+suffix, pDlg->m_GbLow);
	WriteConfigInt(_T("B_max")+suffix, pDlg->m_BHigh);
	WriteConfigInt(_T("B_min")+suffix, pDlg->m_BLow);

	//Gain Range
	WriteConfigInt(_T("RG_max")+suffix, pDlg->m_RGHigh);
	WriteConfigInt(_T("RG_min")+suffix, pDlg->m_RGLow);
	WriteConfigInt(_T("BG_max")+suffix, pDlg->m_BGHigh);
	WriteConfigInt(_T("BG_min")+suffix, pDlg->m_BGLow);
	WriteConfigInt(_T("GG_max")+suffix, pDlg->m_GGHigh);
	WriteConfigInt(_T("GG_min")+suffix, pDlg->m_GGLow);

	WriteConfigInt(_T("Raw8_EN"), pDlg->m_Raw8_EN);

	WriteConfigInt(_T("BLC"), pDlg->m_BLC);
	WriteConfigInt(_T("Ratio"), pDlg->m_ROIWidth);
	WriteConfigInt(_T("index"), pDlg->m_Ratio);

	WriteConfigInt(_T("R_index")+suffix,pDlg->m_RIndex);
	WriteConfigInt(_T("Gr_index")+suffix,pDlg->m_GrIndex);
	WriteConfigInt(_T("Gb_index")+suffix,pDlg->m_GbIndex);
	WriteConfigInt(_T("B_index")+suffix,pDlg->m_BIndex);
	WriteConfigInt(_T("RG_index")+suffix,pDlg->m_RGIndex);
	WriteConfigInt(_T("BG_index")+suffix,pDlg->m_BGIndex);
	WriteConfigInt(_T("GG_index")+suffix,pDlg->m_GGIndex);

	SetName(CurrentName);
	WriteConfigInt(_T("OTP_Distance"), pDlg->m_OTP_Distance);
	WriteConfigInt(_T("m_OTP_BeforeDistance"), pDlg->m_OTP_BeforeDistance);
	WriteConfigInt(_T("SaveImage"), pDlg->m_SaveImage);
	WriteConfigInt(_T("Channel_EN"), pDlg->m_Channel_EN);
	WriteConfigInt(_T("Gain_EN"), pDlg->m_Gain_EN);
	WriteConfigInt(_T("Raw10_EN"), pDlg->m_Raw10_EN); 


	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR ApplyWB_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");

	header += _T("OTP_R,OTP_Gr,OTP_Gb,OTP_B,");

	header += _T("OTP_RGain,OTP_BGain,OTP_GGain,");

	header += _T("APPLY_R,APPLY_Gr,APPLY_Gb,APPLY_B,");

	header += _T("APPLY_RGain,APPLY_BGain,APPLY_GGain,");
	header += _T("BeforeDistance,AfterDistance,");
	if(pDlg->m_Gain_EN)
	{
		header += _T("Typical_RG,Typical_BG,Typical_GG,RGIndex,BGIndex,GGIndex,");
	}

	if(pDlg->m_Channel_EN)
	{
		header += _T("Typical_R,Typical_Gr,Typical_Gb,Typical_B,RIndex,GrIndex,GbIndex,BIndex");
	}


	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR ApplyWB_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR ApplyWB_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR ApplyWB_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	temp.Format(_T("%d,%d,%d,%d,"),OTP_R,OTP_Gr,OTP_Gb,OTP_B);
	Content+=temp;

	temp.Format(_T("%d,%d,%d,"),OTP_RGain,OTP_BGain,OTP_GGain);

	Content+=temp;

	/**
	*	GetReportContents()和GetReportHeads()中内容长度不一致时会导致MES上传失败
	*	--Added by LiHai--20180131
	**/
	//if (TestResult==TEST_PASS)
	{
		temp.Format(_T("%d,%d,%d,%d,"),m_AWBInfo.Unit_R_fin,m_AWBInfo.Unit_Gr_fin,m_AWBInfo.Unit_Gb_fin,m_AWBInfo.Unit_B_fin);
		Content+=temp;

		temp.Format(_T("%d,%d,%d,"),m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin);
		Content+=temp;
	}
	temp.Format(_T("%4.2f,%4.2f,"),Beforedistance,Afterdistance);
	Content+=temp;



	if(pDlg->m_Gain_EN)
	{
		temp.Format(_T("%d,%d,%d,%d,%d,%d,"),m_AWBSpec.Typical_RG,m_AWBSpec.Typical_BG,m_AWBSpec.Typical_GG,pDlg->m_RGIndex, pDlg->m_BGIndex,pDlg->m_GGIndex);
		Content+=temp;
	}

	if(pDlg->m_Channel_EN)
	{
		temp.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d"),m_AWBSpec.Typical_R,m_AWBSpec.Typical_Gr,m_AWBSpec.Typical_Gb,m_AWBSpec.Typical_B,pDlg->m_RIndex,pDlg->m_GrIndex,pDlg->m_GbIndex,pDlg->m_BIndex);
		Content+=temp;
	}


	return Content;
}

BOOL ApplyWB_TestItem::SaveAWBData()
{
	USES_CONVERSION;
	FilePath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\"),CamID);
	FilePath=m_pInterface->Ctrl_GetCurrentPath()+FilePath;

	int siteNo=m_pInterface->GetSiteNo();
	_mkdir(T2A(FilePath));
	if (pDlg->m_count!=0)
        FileName.Format(_T("AWBData_%d_%d.ini"), siteNo,pDlg->m_count);
	else
	    FileName.Format(_T("AWBData_%d.ini"), siteNo);
	FileName = FilePath + FileName;
	//删除旧文件
	if(PathFileExists(FileName))
	{	
		if (!DeleteFile(FileName))
		{
			TempMsg.Format(_T("无法删除旧OTP文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			return FALSE;
		}
	}
	ofstream outfile(FileName,ios::out|ios::trunc);

	outfile<<"[AWBData]"<<endl;

	outfile<<"OTP_R="<<OTP_R<<endl;
	outfile<<"OTP_Gr="<<OTP_Gr<<endl;
	outfile<<"OTP_Gb="<<OTP_Gb<<endl;
	outfile<<"OTP_B="<<OTP_B<<endl;

	outfile<<"OTP_RGain="<<OTP_RGain<<endl;
	outfile<<"OTP_BGain="<<OTP_BGain<<endl;
	outfile<<"OTP_GGain="<<OTP_GGain<<endl;

	outfile<<"Typical_R="<<pDlg->m_TypicalR<<endl;
	outfile<<"Typical_Gr="<<pDlg->m_TypicalGr<<endl;
	outfile<<"Typical_Gb="<<pDlg->m_TypicalGb<<endl;
	outfile<<"Typical_B="<<pDlg->m_TypicalB<<endl;

	outfile<<"Typical_RG="<<pDlg->m_TypicalRG<<endl;
	outfile<<"Typical_BG="<<pDlg->m_TypicalBG<<endl;
	outfile<<"Typical_GG="<<pDlg->m_TypicalGG<<endl;
	
	outfile.close();

	return TRUE;
}
BOOL ApplyWB_TestItem::GetAWBData(ImageInfo img)
{
	int R,Gr,Gb,B;
	CString ToolPath;
	ToolPath.Format(_T("AWBData\\Cam%d\\"),CamID);
	ToolPath=m_pInterface->Ctrl_GetCurrentPath()+ToolPath;
	SetCurrentDirectory(ToolPath);

	CString sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
	USES_CONVERSION;

	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB_LSC_CALIBRATION_DATA.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");

	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7);

	HINSTANCE hDll;   //DLL句柄 
	sfilename=ToolPath+_T("LSCCalibrationDll.dll");
	hDll = LoadLibrary(sfilename);
	if (NULL==hDll)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("LSCCalibrationDll.dll丢失,请将文件放回程式目录!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}

	lpFun LensCorrection = (lpFun)GetProcAddress(hDll,"LensCorrectionLib");
	if (NULL==LensCorrection)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("DLL中函数寻找失败!!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}
	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	BYTE * Raw8buffer=new BYTE[width*height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer buffer 申请失败!!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}
	Raw10toRaw8(img.Raw_buffer,Raw8buffer,width*height);
	int CFA_Pattern=0;
	if ((img.RawFormat == 0) ||
		(img.RawFormat == 1))
	{
		CFA_Pattern =img.RawFormat;
	}
	if (img.RawFormat == 2)
	{
		CFA_Pattern = 3;
	}
	if (img.RawFormat == 3)
	{
		CFA_Pattern = 2;
	}
	CFile file1;
	BOOL ret = file1.Open(ToolPath+_T("Raw8.raw"), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (ret)
	{
		file1.Write((void*)Raw8buffer, width * height * sizeof(BYTE) );
		file1.Close();
	}

	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0);
	SAFE_DELETE_ARRAY(Raw8buffer);
	FreeLibrary(hDll);
	CStdioFile file;
	CString Path = ToolPath;
	Path = Path + _T("AWB_LSC_CALIBRATION_DATA.txt");

	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("r_oc_ave"),0) == -1)
			{
				continue;
			}
			else
			{
				float temp=0.00;
				a = strTemp.Mid(10,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				R=(temp+0.5); //R


				file.ReadString(strTemp);
				a = strTemp.Mid(10,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				B=(temp+0.5); //B

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				Gr=(temp+0.5); //Gr

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				Gb=(temp+0.5); //Gb
				break;

			}	
		}
		file.Close();
	}
	else {
		m_pInterface->Ctrl_Addlog(CamID,_T("打开AWB_LSC_CALIBRATION_DATA.txt失败!"),COLOR_RED,200);
		return FALSE;
	}

	m_AWBInfo.Unit_Gr= int(Gr)+pDlg->m_BLC;
	m_AWBInfo.Unit_Gb = int(Gb)+pDlg->m_BLC;
	m_AWBInfo.Unit_R  = int(R)+pDlg->m_BLC;
	m_AWBInfo.Unit_B  = int(B)+pDlg->m_BLC;
	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if(GAverFlag==1)
	{
		m_AWBInfo.RGain = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==2)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==3)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gr-pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) +AA );
	}


	if (pDlg->m_Channel_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC)*m_AWBSpec.RIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC)*m_AWBSpec.GrIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC)*m_AWBSpec.GbIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC)*m_AWBSpec.BIndex /10000.0 + 0.5);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC));
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC));
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC));
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC));
	}
	
	if (pDlg->m_Channel_EN)
	{
		if(GAverFlag==1)
		{
			m_AWBInfo.RGain_fin = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 2.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_BG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_GG=int(2.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gr/(m_AWBSpec.Typical_Gr+m_AWBSpec.Typical_Gb)+0.5);
		}
		else if (GAverFlag==2)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gr)+0.5);
			m_AWBSpec.Typical_BG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gr)+0.5);
			m_AWBSpec.Typical_GG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gb/(m_AWBSpec.Typical_Gr)+0.5);
		}
		else if (GAverFlag==3)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Ratio*(m_AWBInfo.Unit_Gr_fin)/( m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Typical_RG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_R/(m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_BG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_B/(m_AWBSpec.Typical_Gb)+0.5);
			m_AWBSpec.Typical_GG=int(1.0*pDlg->m_Ratio*m_AWBSpec.Typical_Gr/(m_AWBSpec.Typical_Gb)+0.5);
		}
		CString temp;
		temp.Format(_T("Typical_R=%d\nTypical_Gr=%d\nTypical_Gb=%d\nTypical_B=%d"),m_AWBSpec.Typical_R,m_AWBSpec.Typical_Gr,m_AWBSpec.Typical_Gb,m_AWBSpec.Typical_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.RGain_fin = int(m_AWBInfo.RGain * m_AWBSpec.RGIndex /10000.0 + 0.5);
		m_AWBInfo.BGain_fin = int(m_AWBInfo.BGain * m_AWBSpec.BGIndex /10000.0 + 0.5);
		m_AWBInfo.GGain_fin = int(m_AWBInfo.GGain * m_AWBSpec.GGIndex /10000.0 + 0.5);
	}

	return TRUE;
}