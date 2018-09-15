#include "stdafx.h"
#include "Inspection_AutoFocus.h"
#include "ImageProc.h"
//#include "SFR_Ofilm.h"


#ifndef TCCM_EXP
    #define TCCM_EXP	__declspec(dllexport)
#endif	/* TCCM_EXP */


extern "C" TCCM_EXP TestItemInterface*  CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Inspection_AutoFocus(pInterface, lpszName);
}
 

Inspection_AutoFocus::~Inspection_AutoFocus(void)
{
	SAFE_DELETE(pDlg);
}
int Inspection_AutoFocus::RecomAFcode = 0;
Inspection_AutoFocus::Inspection_AutoFocus(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetName(lpszName);	 //设置测试项目的名字
	
		
	pDlg = new AutoFocusOptionDlg(NULL,this);        //生成对话框对象
      
	//读取参数
	pDlg->Create(AutoFocusOptionDlg::IDD,NULL);      //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_MACRO);

	//其他类成员构造初始化
	Lock_OneTime = FALSE;
	

	//pMyStorage = NULL;
}

int Inspection_AutoFocus::InitItem()
{
	CamID = GetCamID(); 
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption();  
	return 0;
}

int Inspection_AutoFocus::Initialize()
{
	TestItemInterface::Initialize();

	SetbStopTest(FALSE);
	RecomAFcode = pDlg->m_StartCode;

	Code_upperlimit = pDlg->m_Code_upperlimit;
	ZeroNum = 0;
	NGTimes = 0;

	//if(!Lock_OneTime)
	{
		m_pInterface->VcmDr_InitAF(CamID);
		m_pInterface->VcmDr_WriteAF_Code(0,CamID); //CODE归0
		Sleep(100);
		m_Curve.Init();      //清空节点信息
		
		m_SearchParam[0] = SearchParam(pDlg->m_DescendTime_1,pDlg->m_Diff_1,pDlg->m_StepCode_1);
		m_SearchParam[1] = SearchParam(pDlg->m_DescendTime_2,pDlg->m_Diff_2,pDlg->m_StepCode_2);
		m_SearchParam[2] = SearchParam(pDlg->m_DescendTime_3,pDlg->m_Diff_3,pDlg->m_StepCode_3);
		m_SearchParam[3] = SearchParam(pDlg->m_DescendTime_4,pDlg->m_Diff_4,pDlg->m_StepCode_4);
		
		CurSearchStep = -1;

		//最佳对焦点保存
		BestAFcode = -1;
		 
		//DrawBackGround();
		Lock_OneTime = TRUE; 
	}
	
    FramePitch = 0; //帧间隔归0

	if(pDlg->str_Alg_Type == _T("MTF"))
	{
		m_Alg_Type = 0;
	}
	else if(pDlg->str_Alg_Type == _T("SFR"))
	{
		m_Alg_Type = 2;
	}
	else if (pDlg->str_Alg_Type == _T("QCAF"))
	{
		m_Alg_Type = 3;
	}
	else if (pDlg->str_Alg_Type == L"TenenGradS")
	{
		m_Alg_Type = 4;
	}
	else if (pDlg->str_Alg_Type == L"TenenGrad4")
	{
		m_Alg_Type = 5;
	}
	else if (pDlg->str_Alg_Type == L"TenenGrad2")
	{
		m_Alg_Type = 6;
	}
	else
	{
		m_Alg_Type = 1;
	}

	if(pDlg->str_ResolutionResult_Alg == _T("MTF"))
	{
		m_ResolutionLimit_Alg = 0;
	}
	else if(pDlg->str_ResolutionResult_Alg == _T("SFR"))
	{
		m_ResolutionLimit_Alg = 2;
	}
	else if (pDlg->str_ResolutionResult_Alg == _T("QCAF"))
	{
		m_ResolutionLimit_Alg = 3;
	}
	else if (pDlg->str_ResolutionResult_Alg == L"TenenGradS")
	{
		m_ResolutionLimit_Alg = 4;
	}
	else if (pDlg->str_ResolutionResult_Alg == L"TenenGrad4")
	{
		m_ResolutionLimit_Alg = 5;
	}
	else if (pDlg->str_ResolutionResult_Alg == L"TenenGrad2")
	{
		m_ResolutionLimit_Alg = 6;
	}
	else
	{
		m_ResolutionLimit_Alg = 1;
	} 

 
	vecCurrentTemperature.clear();
	vecCode.clear();
	vecValue.clear();
	Y_buffer = NULL;
	 img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
	Y_buffer = new BYTE[img.imgwidth *img.imgheight];
	memset(Y_buffer,0,img.imgwidth*img.imgheight);

	CalculateCode = 0;
	SearchCode = 0;
	memset(t,0,10*sizeof(DWORD));
	strLog = L"";
	return 0;

}

int Inspection_AutoFocus::Testing()
{
	Lock_OneTime = FALSE;
	if ( AutoFocus() == 0 ) 
	{
		return  0;
	}
	else
	{
		return -1;
	}
}

int Inspection_AutoFocus::Finalize()
{
	Lock_OneTime = FALSE;
	SAFE_DELETE_ARRAY(Y_buffer);
	//SAFE_DELETE_ARRAY(ROI_Buffer);
	//保存最佳点
	//pMyStorage->push_back(BestAFcode);
	strLog = L"";
	NGTimes = 0;
	return 0;
}

int Inspection_AutoFocus::LoadOption()
{

	pDlg->m_StartCode        = ReadConfigInt(_T("startCode"), 400);
	pDlg->m_Resolution_Limit = ReadConfigInt(_T("Resolution_Limit"), 0);
	pDlg->m_Resolution_Limit_Diff = ReadConfigInt(_T("Resolution_Limit_Diff"),0);

	pDlg->str_ResolutionResult_Alg = ReadConfigString(_T("ResolutionLimit_Alg"), _T("MTF"));
	pDlg->m_Code_lowerlimit  = ReadConfigInt(_T("Code_lowerlimit"), 0);
	pDlg->m_Code_upperlimit  = ReadConfigInt(_T("Code_upperlimit"), 1023);

	pDlg->m_ROI_Width        = ReadConfigInt(_T("ROI_Width"), 10);
	pDlg->m_ROI_Height       = ReadConfigInt(_T("ROI_Height"), 10);
	pDlg->m_ROI_cx           = ReadConfigInt(_T("ROI_cx"), 50);
    pDlg->m_ROI_cy           = ReadConfigInt(_T("ROI_cy"), 50);

	pDlg->str_Alg_Type         = ReadConfigString(_T("Alg_Type"), _T("MTF"));

	 

	pDlg->m_FramePitchLimit_1  = ReadConfigInt(_T("FramePitchLimit_1"),2);
	pDlg->m_FramePitchLimit_2  = ReadConfigInt(_T("FramePitchLimit_2"),2);
	pDlg->m_FramePitchLimit_3  = ReadConfigInt(_T("FramePitchLimit_3"),2);
	pDlg->m_FramePitchLimit_4  = ReadConfigInt(_T("FramePitchLimit_4"),2);

	pDlg->m_SearchTime       = ReadConfigInt(_T("SearchTime"),2);
	pDlg->m_StepCode_1       = ReadConfigInt(_T("StepCode_1"),50);
	pDlg->m_StepCode_2       = ReadConfigInt(_T("StepCode_2"),20);
	pDlg->m_StepCode_3       = ReadConfigInt(_T("StepCode_3"),5);
	pDlg->m_StepCode_4       = ReadConfigInt(_T("StepCode_4"),2);

	pDlg->m_DescendTime_1    = ReadConfigInt(_T("DescendTime_1"),2);
	pDlg->m_DescendTime_2    = ReadConfigInt(_T("DescendTime_2"),1);
	pDlg->m_DescendTime_3    = ReadConfigInt(_T("DescendTime_3"),1);
	pDlg->m_DescendTime_4    = ReadConfigInt(_T("DescendTime_4"),1);

	pDlg->m_Diff_1           = ReadConfigInt(_T("Diff_1"),500)/100;
	pDlg->m_Diff_2           = ReadConfigInt(_T("Diff_2"),0)/100;
	pDlg->m_Diff_3           = ReadConfigInt(_T("Diff_3"),0)/100;
	pDlg->m_Diff_4           = ReadConfigInt(_T("Diff_4"),0)/100;
	

	pDlg->m_PosPrompt        = ReadConfigInt(_T("PosPrompt"), 0);
	pDlg->m_bAutoFocus       = ReadConfigInt(_T("AutoFocusEn"), 0);
	pDlg->bGetTemperature    = ReadConfigInt(_T("bGetTemperature"), 0);
	pDlg->bBestCodeForOtp    = ReadConfigInt(_T("bBestCodeForOtp"), 0);
	pDlg->m_burnItem		 = ReadConfigString(_T("m_burnItem"), _T("BestCode"));

	pDlg->m_bGetTemperature2    = ReadConfigInt(_T("m_bGetTemperature2"), 0);
	pDlg->m_tempOnOffReg		 = ReadConfigString(_T("m_tempOnOffReg"), _T(""));
	pDlg->m_tempSlaveId    = ReadConfigString(_T("m_tempSlaveId"), _T(""));
	pDlg->m_tempAddrReg    = ReadConfigString(_T("m_tempAddrReg"), _T(""));


	pDlg->m_roiContourLenght = ReadConfigInt(_T("SFR_ROI_CL"), 200);
	pDlg->m_roiWidth = ReadConfigInt(_T("SFR_ROI_WIDTH"), 80);
	pDlg->m_roiHeight = ReadConfigInt(_T("SFR_ROI_HEIGHT"), 60);
	pDlg->m_roiFov = ReadConfigInt(_T("SFR_ROI_FOV"), 80);
	 
	pDlg->m_roiSfrFreq = ReadConfigInt(_T("SFR_FREQ"), 4);

	pDlg->m_ImgType = ReadConfigInt(L"ImgType",0);
	pDlg->m_bUseCruve = ReadConfigInt(L"bUseCruve",0);
	pDlg->m_bSaveData = ReadConfigInt(L"bSaveData",0);
	pDlg->m_CruveUseStep = ReadConfigInt(L"CruveUseStep",2);
	pDlg->m_UsePoint = ReadConfigInt(L"UsePoint",3);

	pDlg->m_ErrorCode = ReadConfigInt(L"ErrorCode",1);
	pDlg->m_bTimesLog = ReadConfigInt(L"bTimesLog",0);

	pDlg->m_bUseDelayTime = ReadConfigInt(L"bUseDelayTime",1);
	pDlg->m_NGTimes = ReadConfigInt(L"NGTimes",1);


	return TestItemInterface::LoadOption();
}

int Inspection_AutoFocus::SaveOption()
{
	if (!TestItemInterface::SaveOption())   return -1;
//TODO

	WriteConfigInt(_T("startCode       "),pDlg->m_StartCode);
	WriteConfigInt(_T("Resolution_Limit"),pDlg->m_Resolution_Limit);
	WriteConfigInt(_T("Resolution_Limit_Diff"),pDlg->m_Resolution_Limit_Diff);

	WriteConfigString(_T("ResolutionLimit_Alg"),pDlg->str_ResolutionResult_Alg);
	WriteConfigInt(_T("Code_upperlimit"), pDlg->m_Code_upperlimit);
	WriteConfigInt(_T("Code_lowerlimit"), pDlg->m_Code_lowerlimit);

	WriteConfigInt(_T("ROI_Width       "),pDlg->m_ROI_Width);
	WriteConfigInt(_T("ROI_Height      "),pDlg->m_ROI_Height);
    WriteConfigInt(_T("ROI_cx          "),pDlg->m_ROI_cx);
	WriteConfigInt(_T("ROI_cy          "),pDlg->m_ROI_cy);
    
	WriteConfigString(_T("Alg_Type        "),pDlg->str_Alg_Type);
	 
	WriteConfigInt(_T("FramePitchLimit_1      "),pDlg->m_FramePitchLimit_1);
	WriteConfigInt(_T("FramePitchLimit_2      "),pDlg->m_FramePitchLimit_2);
	WriteConfigInt(_T("FramePitchLimit_3      "),pDlg->m_FramePitchLimit_3);
	WriteConfigInt(_T("FramePitchLimit_4      "),pDlg->m_FramePitchLimit_4);

	WriteConfigInt(_T("SearchTime"), pDlg->m_SearchTime);
	WriteConfigInt(_T("StepCode_1"), pDlg->m_StepCode_1);
	WriteConfigInt(_T("StepCode_2"), pDlg->m_StepCode_2);
	WriteConfigInt(_T("StepCode_3"), pDlg->m_StepCode_3);
	WriteConfigInt(_T("StepCode_4"), pDlg->m_StepCode_4);

	WriteConfigInt(_T("DescendTime_1"), pDlg->m_DescendTime_1);
	WriteConfigInt(_T("DescendTime_2"), pDlg->m_DescendTime_2);
	WriteConfigInt(_T("DescendTime_3"), pDlg->m_DescendTime_3);
	WriteConfigInt(_T("DescendTime_4"), pDlg->m_DescendTime_4);

	WriteConfigInt(_T("Diff_1"), int(pDlg->m_Diff_1*100));
	WriteConfigInt(_T("Diff_2"), int(pDlg->m_Diff_2*100));
	WriteConfigInt(_T("Diff_3"), int(pDlg->m_Diff_3*100));
	WriteConfigInt(_T("Diff_4"), int(pDlg->m_Diff_4*100));

	WriteConfigInt(_T("PosPrompt"), pDlg->m_PosPrompt);

	WriteConfigInt(_T("AutoFocusEn"), pDlg->m_bAutoFocus);

	WriteConfigInt(_T("bGetTemperature"), pDlg->bGetTemperature  );
	WriteConfigInt(_T("bBestCodeForOtp"), pDlg->bBestCodeForOtp  );
	WriteConfigString(_T("m_burnItem"), pDlg->m_burnItem);

	WriteConfigInt(_T("m_bGetTemperature2"), pDlg->m_bGetTemperature2);
	WriteConfigString(_T("m_tempOnOffReg"), pDlg->m_tempOnOffReg);
	WriteConfigString(_T("m_tempSlaveId"), pDlg->m_tempSlaveId);
	WriteConfigString(_T("m_tempAddrReg"), pDlg->m_tempAddrReg);

	WriteConfigInt(_T("SFR_ROI_CL"), pDlg->m_roiContourLenght);
	WriteConfigInt(_T("SFR_ROI_WIDTH"), pDlg->m_roiWidth);
	WriteConfigInt(_T("SFR_ROI_HEIGHT"), pDlg->m_roiHeight);
	WriteConfigInt(_T("SFR_ROI_FOV"), pDlg->m_roiFov);	 
	WriteConfigInt(_T("SFR_FREQ"), pDlg->m_roiSfrFreq);

	WriteConfigInt(L"ImgType",pDlg->m_ImgType);
	WriteConfigInt(L"bUseCruve",pDlg->m_bUseCruve);

	WriteConfigInt(L"bSaveData",pDlg->m_bSaveData);
	WriteConfigInt(L"CruveUseStep",pDlg->m_CruveUseStep);
	WriteConfigInt(L"UsePoint",pDlg->m_UsePoint);

	WriteConfigInt(L"ErrorCode",pDlg->m_ErrorCode);
	WriteConfigInt(L"bTimesLog",pDlg->m_bTimesLog);

	WriteConfigInt(L"bUseDelayTime",pDlg->m_bUseDelayTime);
	WriteConfigInt(L"NGTimes",pDlg->m_NGTimes);

	return 0;
}

LPCTSTR Inspection_AutoFocus::GetReportHeads()
{
	// TODO : Make report headers of this inspection and return that
	static CString heads(_T("BestAFCode,CalculateCode"));
   return heads;
}

LPCTSTR Inspection_AutoFocus::GetReportLowLimit()
{
	// TODO : Make report lower limits of this inspection and return that
	 
	return _T("");
}

LPCTSTR Inspection_AutoFocus::GetReportHightLimit()
{
	// TODO : Make report upper limits of this inspection and return that

	return _T("");
}

LPCTSTR Inspection_AutoFocus::GetReportContents()
{
	// TODO : Make report contents of this inspection and return that
	static CString Content;
	CString str;
	
	if(pDlg->m_bGetTemperature2)
		Content.Format(_T(",%d, %f, %d"),BestAFcode, vecValue[vecValue.size()-1], vecCurrentTemperature[vecCurrentTemperature.size()-1]);
	else
		Content.Format(_T("%d,%d"),BestAFcode,CalculateCode);

	Content  = Content;

	if (pDlg->m_bSaveData)
	{
		for (UINT i = 0; i<m_Curve.AFinfoArray.size(); i++)
		{
			str.Format(_T("\r, ,%02d,%d,%.4lf,%d"),m_Curve.AFinfoArray[i].m_Index,m_Curve.AFinfoArray[i].m_Code,m_Curve.AFinfoArray[i].m_Value,m_Curve.AFinfoArray[i].m_Flag);
			Content += str; 
		}
	}
	
	if(pDlg->bGetTemperature)
	{
		
		for(UINT i=0;i<vecValue.size();i++)
		{
			str.Format(_T(",%d"),vecCode[i]);
			Content += str; 

			str.Format(_T(",%.4lf"),vecValue[i]);
			Content += str; 

			str.Format(_T(",%d"),vecCurrentTemperature[i]);
			Content += (str+_T("\r")); 
		} 
	} 
	return Content;
}

int Inspection_AutoFocus::AutoFocus()
{	
	img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
	if (pDlg->m_ImgType==0)
	{
		ImageProc::RGB24toY(img.RGB24_buffer,Y_buffer,img.imgwidth,img.imgheight);
	}
	else
	{
		ImageProc::RawToRaw8(img.Raw_buffer,Y_buffer,img.imgwidth,img.imgheight,img.RawBits);
	}
	
	if(pDlg->m_bAutoFocus)
	{
		m_pInterface->VcmDr_AutoFocus(CamID);
		if(FramePitch < pDlg->m_FramePitchLimit_1)
		{
			FramePitch++;
			return 1;
		}
		else
			FramePitch = 0;
		return 0 ;
	}
	//帧间隔
	
	int m_FramePitchLimit = 0;
	switch(CurSearchStep)
	{
	case -1:
		m_FramePitchLimit = pDlg->m_FramePitchLimit_1;
		break;
	case 0:
		m_FramePitchLimit = pDlg->m_FramePitchLimit_1;
		break;
	case 1:
		m_FramePitchLimit = pDlg->m_FramePitchLimit_2;
		break;
	case 2:
		m_FramePitchLimit = pDlg->m_FramePitchLimit_3;
		break;
	case 3:
		m_FramePitchLimit = pDlg->m_FramePitchLimit_4;
		break;
	default:
		m_FramePitchLimit = pDlg->m_FramePitchLimit_2;
		break;
	}

	if (pDlg->m_bUseDelayTime)
	{
		if (m_FramePitchLimit<10)
		{
			m_FramePitchLimit = m_FramePitchLimit*100;
		}
		t[3] = GetTickCount();
		DWORD FrameTimeDiff = t[3] - t[0];
		if (FrameTimeDiff < m_FramePitchLimit)
		{
			FramePitch++;
			return 1;
		}else 
		{
			if (pDlg->m_bTimesLog)
			{
				strLog.Format(L"delay:%d,nCnt:%d",FrameTimeDiff,FramePitch);
				m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
			}
			FramePitch = 0;
		}
	}
	else
	{
		if (m_FramePitchLimit>10)
		{
			m_FramePitchLimit = m_FramePitchLimit/100;
		}
		if(FramePitch < m_FramePitchLimit)
		{
			if (pDlg->m_bTimesLog)
			{
				if (FramePitch == 0)
				{
					t[3] = GetTickCount();
				}
				if (FramePitch == 1)
				{
					t[4] = GetTickCount();
					strLog.Format(L"帧间隔:%d",t[4]-t[3]);
					m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
				}
			}
			FramePitch++;
			return 1;
		}
		else
			FramePitch = 0;
	}
	
	int AFcode=0;
	USHORT temp=0;
	double Value = 0.0;
	if(CurSearchStep!=-1)
	{
		if (pDlg->m_bTimesLog)
		{
			t[1] = GetTickCount();
			//strLog.Format(L"Time:%d",t[1]);
			m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
			strLog.Format(L"FrameDiff:%d",t[1]-t[0]);
			m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
		}
		
		//记录得到当前点信息
		  Value  = GetROIValue(m_Alg_Type);//得到搜索区域的解像力值

		  if (pDlg->m_bTimesLog)
		  {
			  t[2] = GetTickCount();
			  strLog.Format(L"Time:%d",t[2]);
			  m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
			  strLog.Format(L"CalcuDiff:%d",t[2]-t[1]);
			  m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
		  }
		  
		   
		if (0)
		{
			m_pInterface->VcmDr_ReadAF_Code(&temp,CamID); //读当前的DIRIVER IC值
			AFcode = temp;
		}
		else
		{
			AFcode = RecomAFcode;
		}

		m_Curve.AddNode(Value, AFcode,CurSearchStep);	//保存计算节点信息
		if(pDlg->bGetTemperature) 
		{
			vecCode.push_back(AFcode);
			vecValue.push_back(Value);
			//vecCurrentTemperature.push_back(GetTemperate());
			CString fileName = _T("");
			fileName.Format(_T("%s_%d"), m_pInterface->Ctrl_GetCamCode(), AFcode);
			m_pInterface->Ctrl_SaveImage(CamID, TRUE, m_pInterface->Ctrl_GetCurrentPath()+_T("\\ImgSave\\"), fileName, 2);
		}
		//显示信息
		m_pInterface->Ctrl_Addlog(CamID,m_Curve.GetCurrentNodeInfo(),COLOR_BLUE,220);
	}
	 
    //显示AF曲线
	if (1)
	{
		Show(&m_Curve.AFinfoArray);
	}
	

	switch(CurSearchStep)
	{
	case -1: //第一次写入初始CODE值
		{
			AFcode=RecomAFcode ;
			int wFlag = m_pInterface->VcmDr_WriteAF_Code(AFcode,CamID); 
			t[0] = GetTickCount();
			Sleep(200);
		//	t1 = GetTickCount();
			if(wFlag<0)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("AF 写入失败"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("AF 写入失败"));
				SetResult(RESULT_FAIL);
				return 0;
			}
			if(pDlg->bGetTemperature) 
			{ 
				vecCurrentTemperature.push_back(GetTemperate()); 
			} 
			CurSearchStep ++;
			m_Curve.SetNextSearchTarget(m_SearchParam[0].m_DescendTime, m_SearchParam[0].m_Diff);
			m_CurStepCode = m_SearchParam[0].m_StepCode; 
			Code_upperlimit = pDlg->m_Code_upperlimit + (2 * m_CurStepCode);
			RecomAFcode = AFcode;
			return 1;
		}
	case AF_EndFlag://AF结束的动作
		{
			BestAFcode = AFcode;
			if (AF_End_Check() == 0)
			{
				RecomAFcode = BestAFcode - 100;
				m_Curve.Init();      //清空节点信息
				vecCurrentTemperature.clear();
				vecCode.clear();
				vecValue.clear();

				m_SearchParam[0] = SearchParam(pDlg->m_DescendTime_1,pDlg->m_Diff_1,pDlg->m_StepCode_1);
				m_SearchParam[1] = SearchParam(pDlg->m_DescendTime_2,pDlg->m_Diff_2,pDlg->m_StepCode_2);
				m_SearchParam[2] = SearchParam(pDlg->m_DescendTime_3,pDlg->m_Diff_3,pDlg->m_StepCode_3);
				m_SearchParam[3] = SearchParam(pDlg->m_DescendTime_4,pDlg->m_Diff_4,pDlg->m_StepCode_4);

				FramePitch = 0; 
				CurSearchStep = -1;
				BestAFcode = -1;
				Lock_OneTime = TRUE; 
				
				return 1;
			}			
			return 0;
		}
	default:
		{
			//当前的搜索完成了吗
			if (m_Curve.isSearchTargetReached()||AFcode==Code_upperlimit)
			{
				//完成了
				//得到下一次搜索方向
				/*m_Direction = m_Curve.GetNextSearchDirection();*/
				AFinfo m_AFinfo;
				m_AFinfo = m_Curve.FindMaxNode(CurSearchStep);
				m_Direction = 1;
				if(m_AFinfo.m_Index-1>=0)
				{
				   AFcode  = m_Curve.AFinfoArray[m_AFinfo.m_Index-1].m_Code;
				}
				else
				{
					//AFcode  = m_Curve.AFinfoArray[m_AFinfo.m_Index].m_Code;
					///只有下降没有上升，重新开始爬山
					CurSearchStep = -1;
					RecomAFcode -= 100;
					if(RecomAFcode < 0)
					{
						RecomAFcode = 0;
					}
					m_Curve.NewStartFlag = TRUE;
					m_Curve.Init();      //清空节点信息
					return 1;
				}
				
				RecomAFcode = AFcode;
				//设置下一次的搜索目标
				CurSearchStep++;		
				//是否到了最后一次搜索
				if (CurSearchStep > pDlg->m_SearchTime||AFcode==Code_upperlimit)
				{
					AFcode = m_Curve.FindMaxNode(CurSearchStep-1).m_Code;		
					SearchCode = AFcode;
					if (pDlg->m_bUseCruve)
					{
						AFcode = GetCruveAF(CurSearchStep-1);
						if ((AFcode == -1) || (AFcode==65535))
						{
								CurSearchStep = -1;
								RecomAFcode -= 100;
								if(RecomAFcode <= 0)
								{
									ZeroNum++;
									RecomAFcode = 0;
								}
								if(ZeroNum >= 2)
								{
									m_pInterface->Ctrl_Addlog(CamID, _T("AF 寻焦失败！"), COLOR_RED, 200);
									m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("AF 寻焦失败！"));
									SetResult(RESULT_FAIL);
									return 0;
								}
								m_Curve.NewStartFlag = TRUE;
								m_Curve.Init();      //清空节点信息
								return 1;
						}
						CalculateCode = AFcode;
					}
					RecomAFcode = AFcode;
					m_Curve.NewStartFlag = TRUE;
					CurSearchStep = AF_EndFlag;
					break;
				}
				m_Curve.SetNextSearchTarget(m_SearchParam[CurSearchStep].m_DescendTime, m_SearchParam[CurSearchStep].m_Diff);
				//设置下一次的步进值
				m_CurStepCode = m_SearchParam[CurSearchStep].m_StepCode ;
				Code_upperlimit = pDlg->m_Code_upperlimit + (2 * m_CurStepCode);
			}
			else //没完成，继续加
			{
				AFcode += m_CurStepCode;
				if (AFcode > Code_upperlimit)
				{
					AFcode = Code_upperlimit;
				}
				/*if(AFcode > pDlg->m_Code_upperlimit)
				{
					AFcode = pDlg->m_Code_upperlimit;
				}*/
				RecomAFcode  = AFcode;
			} 
		}
	}
	/*if (AFcode<pDlg->m_Code_lowerlimit)
	{
	AFcode = pDlg->m_Code_lowerlimit;
	}*/
	if (AFcode >= Code_upperlimit)
	{
		AFcode = m_Curve.FindMaxNode(CurSearchStep-1).m_Code;
		m_Curve.NewStartFlag = TRUE;
		CurSearchStep = AF_EndFlag;
	}
	
	
	m_pInterface->VcmDr_WriteAF_Code((USHORT)AFcode,CamID);
	t[0] = GetTickCount();
	if (pDlg->m_bTimesLog)
	{
		strLog.Format(L"Time:%d",t[0]);
		m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
	}
	

	if(pDlg->bGetTemperature) 
	{ 
		vecCurrentTemperature.push_back(GetTemperate()); 
	} 
	 
	return 1;
}

int Inspection_AutoFocus::AF_End_Check(void)
{
	//计算最清晰解像力是否达标，同时code范围在标准之内
	if(pDlg->bGetTemperature) 
	{ 
		vecCurrentTemperature.push_back(GetTemperate()); 
	} 

	BOOL Code_OK = FALSE, Resolution_OK = FALSE;
	if ((m_Curve.GetCurrentNode().m_Code >= pDlg->m_Code_lowerlimit) &&
		(m_Curve.GetCurrentNode().m_Code <= pDlg->m_Code_upperlimit))
	{
		Code_OK = TRUE;
	}

	//计算解像力
	double Value  = GetROIValue(m_ResolutionLimit_Alg);//得到搜索区域的解像力值
	CString str;
	str.Format(_T("当前解像力值: %.1lf"),Value);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,220);

	int Len = m_Curve.AFinfoArray.size();
	int MaxVal = 1;
	int tempVal = 0;
	for (int i=0; i<Len; i++)
	{
		tempVal = m_Curve.AFinfoArray.at(i).m_Value;
		if (tempVal > MaxVal)
		{
			MaxVal = tempVal; 
		}
	}

	float Val_Diff = 100.0*Value/MaxVal;
	str.Format(_T("当前解像力值和搜寻过程最大值比值为: %.1lf"),Val_Diff);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,220);

	if (Code_OK && 
		Value >= pDlg->m_Resolution_Limit && 
		Val_Diff < pDlg->m_Resolution_Limit_Diff &&
		NGTimes < pDlg->m_NGTimes)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("解像力差值过大，重新测试!"),COLOR_RED,200);
		NGTimes++;
		return 0;
	}
	
	if(Value >= pDlg->m_Resolution_Limit && Val_Diff >= pDlg->m_Resolution_Limit_Diff)
	{
		Resolution_OK =  TRUE;
	}

	if (Code_OK && Resolution_OK) //Code结果OK和解像力OK 才算PASS
	{
		SetResult(RESULT_PASS);
		m_pInterface->Ctrl_Addlog(CamID,_T("AF PASS"), COLOR_GREEN, 330);
	}
	else
	{
		m_pInterface->Ctrl_SetCamErrorCode(CamID,pDlg->m_ErrorCode);
		str.Format(L"ErrorCode:%02d",pDlg->m_ErrorCode);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
		SetResult(RESULT_FAIL); // 失败
		if (!Resolution_OK)
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("AF NG 无法聚焦清晰"));
			m_pInterface->Ctrl_Addlog(CamID,_T(" AF NG 无法聚焦清晰"),COLOR_RED, 330);
		}
		if (!Code_OK)
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("AF NG Code值范围超标"));
			m_pInterface->Ctrl_Addlog(CamID,_T("AF NG Code值范围超标"),COLOR_RED, 330); 
		}
	}
	if(pDlg->bBestCodeForOtp) WriteConfigInt(_T("BestCode"), BestAFcode);
	if (pDlg->m_bGetTemperature2)
	{
		vecCurrentTemperature.push_back(GetTemperate());
		vecCode.push_back(BestAFcode);
		vecValue.push_back(Value);
		
		SYSTEMTIME currTime;
		GetLocalTime(&currTime);
		CString fileName = _T("");
		fileName.Format(_T("%d%d%d_%s_%d"), currTime.wHour, currTime.wMinute, currTime.wSecond, m_pInterface->Ctrl_GetCamCode(), BestAFcode);
		m_pInterface->Ctrl_SaveImage(CamID, TRUE, m_pInterface->Ctrl_GetCurrentPath()+_T("\\ImgSave\\"), fileName, 2);
	}
	return 1;
}


int Inspection_AutoFocus::Show(vector<AFinfo>* Array)
{  
	//********显示对焦框************//
	if(m_Alg_Type==2) return 0;
	//if(m_ResolutionLimit_Alg == 2)  return ShowSFR_Info();
	//得到窗口的MEM DC
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
 

	pDisplayWndMemDC->SetBkMode(TRANSPARENT);        //设置透明模式
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH); //设置空刷
	CPen pen(PS_SOLID,2,COLOR_BLUE);                 //设置画笔，对焦框的颜色和边框宽度
	pDisplayWndMemDC->SelectObject(&pen);            //选中画笔

	//计算方框的大小和位置
	CRect AFRect = m_pInterface->Ctrl_CalcRect_DisplayScreen(pDlg->m_ROI_cx, 
		pDlg->m_ROI_cy,
		pDlg->m_ROI_Width,
		pDlg->m_ROI_Height,
		CamID);

	//在MEMDC上画方框
	pDisplayWndMemDC->Rectangle(&AFRect);

	//显示
	m_pInterface->Ctrl_ShowMemDC(CamID);
	return 0;
}

/*
int Inspection_AutoFocus::ShowSFR_InfoAF()
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	BOOL bShowCen = FALSE;
	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	//if (bParallel)
	{
		for (UINT i=0; i<m_sfrPara.RoiRect.size(); i++)
		{ 
			CurrentColor = COLOR_YELLOW;
			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			
			cx1 = m_sfrPara.RoiRect[i].left;
			cy1 = m_sfrPara.RoiRect[i].top;
			px1 = (int)(cx1* Xratio);
			py1 = (int)(cy1* Yratio);

			cx2 = m_sfrPara.RoiRect[i].right;
			cy2 = m_sfrPara.RoiRect[i].bottom;
			px2 = (int)(cx2* Xratio);
			py2 = (int)(cy2* Yratio);
			bShowCen = TRUE;
			if(bShowCen)
				pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		}

		cx1 = 0;
		cy1 = ImgHeight/2;
		px1 = (int)(cx1* Xratio);
		py1 = (int)(cy1* Yratio);

		cx2 = ImgWidth-1;
		cy2 = ImgHeight/2;
		px2 = (int)(cx2* Xratio);
		py2 = (int)(cy2* Yratio);
		bShowCen = TRUE;
		if(bShowCen)
		{
			pDisplayWndMemDC->MoveTo(px1, py1);
			pDisplayWndMemDC->LineTo(px2, py2);
		}

		cx1 = ImgWidth/2;
		cy1 = 0;
		px1 = (int)(cx1* Xratio);
		py1 = (int)(cy1* Yratio);

		cx2 = ImgWidth/2;
		cy2 = ImgHeight-1;
		int px2 = (int)(cx2* Xratio);
		int py2 = (int)(cy2* Yratio);
		bShowCen = TRUE;
		if(bShowCen)
		{
			pDisplayWndMemDC->MoveTo(px1, py1);
			pDisplayWndMemDC->LineTo(px2, py2);
		}

		int x = DisplayWndRect.Width()/2;
		int y =DisplayWndRect.Height()/2;

		CString str = _T("");
		CurrentColor = COLOR_YELLOW;
		pDisplayWndMemDC->SetTextColor(CurrentColor);
		if(m_sfrPara.RoiSFRValue.size()!=4) return 0;
		str.Format(_T("CT = %.2f"), (m_sfrPara.RoiSFRValue[0]+m_sfrPara.RoiSFRValue[1]+m_sfrPara.RoiSFRValue[2]+m_sfrPara.RoiSFRValue[3])/4);
		pDisplayWndMemDC->TextOut(x*(1+m_sfrPara.RoiPosition)/2,y*(1+m_sfrPara.RoiPosition)/2+50,str);
	} 
	m_pInterface->Ctrl_ShowMemDC(CamID);
	return 0;
}


int Inspection_AutoFocus::ShowSFR_Info()
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	//if (bParallel)
	{
		for (UINT i=0; i<m_sfrPara.RoiRect.size(); i++)
		{ 
			CurrentColor = COLOR_YELLOW;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			int cx1,cy1, cx2, cy2;
			cx1 = m_sfrPara.RoiRect[i].left;
			cy1 = m_sfrPara.RoiRect[i].top;
			int px1 = (int)(cx1* Xratio);
			int py1 = (int)(cy1* Yratio);

			cx2 = m_sfrPara.RoiRect[i].right;
			cy2 = m_sfrPara.RoiRect[i].bottom;
			int px2 = (int)(cx2* Xratio);
			int py2 = (int)(cy2* Yratio);
			BOOL bShowCen = TRUE;
			if(bShowCen)
				pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
			    
		}

		int x = DisplayWndRect.Width()/2;
		int y =DisplayWndRect.Height()/2;

		CString str = _T("");
		CurrentColor = COLOR_YELLOW;
		pDisplayWndMemDC->SetTextColor(CurrentColor);
		if(m_sfrPara.RoiSFRValue.size()!=12) return 0;
		str.Format(_T("LT = %.2f"), (m_sfrPara.RoiSFRValue[0]+m_sfrPara.RoiSFRValue[1])/2);
		pDisplayWndMemDC->TextOut(x*(1-m_sfrPara.RoiPosition)/2,y*(1-m_sfrPara.RoiPosition)/2+50,str); 
		str.Format(_T("RT = %.2f"), (m_sfrPara.RoiSFRValue[2]+m_sfrPara.RoiSFRValue[3])/2);
		pDisplayWndMemDC->TextOut(x*(1+m_sfrPara.RoiPosition)-50,y*(1-m_sfrPara.RoiPosition)/2+50,str); 
		str.Format(_T("CT = %.2f"), (m_sfrPara.RoiSFRValue[4]+m_sfrPara.RoiSFRValue[5]+m_sfrPara.RoiSFRValue[6]+m_sfrPara.RoiSFRValue[7])/4);
		pDisplayWndMemDC->TextOut(x*(1+m_sfrPara.RoiPosition)/2,y*(1+m_sfrPara.RoiPosition)/2+50,str);
		str.Format(_T("LD = %.2f"), (m_sfrPara.RoiSFRValue[8]+m_sfrPara.RoiSFRValue[9])/2);
		pDisplayWndMemDC->TextOut(x*(1-m_sfrPara.RoiPosition)/2,y*(1+m_sfrPara.RoiPosition)-50,str); 
		str.Format(_T("RD = %.2f"), (m_sfrPara.RoiSFRValue[10]+m_sfrPara.RoiSFRValue[11])/2);
		pDisplayWndMemDC->TextOut(x*(1+m_sfrPara.RoiPosition)-50,y*(1+m_sfrPara.RoiPosition)-50,str); 
	} 
	m_pInterface->Ctrl_ShowMemDC(CamID);
	return 0;
}
*/

double Inspection_AutoFocus::GetROIValue(int Alg_type)
{ 
	int width =  img.imgwidth;
	int height = img.imgheight;
   
	AFRect.left   =  int((pDlg->m_ROI_cx - pDlg->m_ROI_Width/2.0) * width/100);
	AFRect.right  =  int((pDlg->m_ROI_cx + pDlg->m_ROI_Width/2.0) * width/100);
	AFRect.top    =  int((pDlg->m_ROI_cy - pDlg->m_ROI_Height/2.0) * height/100);
	AFRect.bottom =  int((pDlg->m_ROI_cy + pDlg->m_ROI_Height/2.0) * height/100);
	 
	double value = 0;
	switch(Alg_type)
	{
	case 6:
		{
			value = GetContrastAF(Y_buffer,width,height,AFRect,Alg_type);
		}
		break;
	case 5:
		{
			value = GetContrastAF(Y_buffer,width,height,AFRect,Alg_type);
		}
		break;
	case 4:
		{
			value = GetContrastAF(Y_buffer,width,height,AFRect,Alg_type);
		}
		break;
	case 3:
		{
			value = 0.0;
			/*
			HINSTANCE hInstance = LoadLibrary(_T("QCAF.dll"));
			if (hInstance == NULL)
			{
				m_pInterface->Ctrl_Addlog(CamID,L"获取QCAFDLL地址失败",COLOR_RED,200);
				FreeLibrary(hInstance);
			}
			typedef unsigned long (*QCAF)(BYTE* Buff,int imagewidth,int imageheight,int outformat);
			QCAF lpGetDiffValue = (QCAF)GetProcAddress(hInstance, "GetDiffValue");
			if (lpGetDiffValue != NULL)
			{
				BYTE *raw8buffer = new BYTE[width*height];
				memset(raw8buffer,0,width*height);
				ImageProc::RawToRaw8(img.Raw_buffer,raw8buffer,width,height,img.RawBits);
				if (AFRect.top%2!=0)
				{
					AFRect.top += 1;
				}
				if (AFRect.left%2!=0)
				{
					AFRect.left += 1;
				}
				if (AFRect.right%2!=0)
				{
					AFRect.right += 1;
				}
				if (AFRect.bottom%2 != 0)
				{
					AFRect.bottom += 1;
				}
				int nRoiWidth = AFRect.right-AFRect.left+1;
				int nRoiHeight = AFRect.bottom-AFRect.top+1;
				for (int h=0; h<nRoiHeight; h++)
				{
					for (int w=0; w<nRoiWidth; w++)
					{
						ROI_Buffer[h*nRoiWidth+w] = raw8buffer[(AFRect.top+h)*width+(AFRect.left+w)];
					}
				}
				SAFE_DELETE_ARRAY(raw8buffer);
				value = lpGetDiffValue(ROI_Buffer,nRoiWidth,nRoiHeight,img.RawFormat);
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,L"获取QCAF函数地址失败",COLOR_RED,200);
			}
			FreeLibrary(hInstance);
			*/
		}
		break;
	case 0: 
		value = ImageProc::GetMTF(Y_buffer,width,height,AFRect)*100;
		break;
	case 1:
		value = ImageProc::GetResolution_FV(Y_buffer,width,height,AFRect);
		break;
	case 2:
		value = 0.0;
/*
#if 1
		HINSTANCE hInstance = LoadLibrary(_T("SFR_Ofilm.dll"));
		if (hInstance == NULL)
			FreeLibrary(hInstance);

		typedef int (*fnSfrTest)(SFR_INPUT inPara, SFR_OUTPUT * outPara);
		fnSfrTest lpSfrTest = (fnSfrTest)GetProcAddress(hInstance, "SFR_TestForAF");
		if (lpSfrTest != NULL)
		{
			SFR_INPUT m_in;
			memset(&m_in, 0, sizeof(SFR_INPUT));
			m_in.imgBuff = Y_buffer;
			m_in.imgWidth = img.imgwidth;
			m_in.imgHeight = img.imgheight;
			m_in.dataFormat = 8;
			m_in.freq = pDlg->m_roiSfrFreq;
			m_in.roiFov = pDlg->m_roiFov / 10;
			m_in.roiWidth = pDlg->m_roiWidth;
			m_in.roiHeight = pDlg->m_roiHeight;
			m_in.reserved = 0;

			SFR_OUTPUT m_out[ROI_COUNT];
			memset(m_out, 0, ROI_COUNT * sizeof(SFR_OUTPUT));
			int ret =lpSfrTest(m_in, m_out);

			m_sfrPara.RoiRect.clear();
			m_sfrPara.RoiSFRValue.clear();
			for (UINT i=0; i<4; i++)
			{
				m_sfrPara.RoiRect.push_back(m_out[i].roiPosi);
				m_sfrPara.RoiSFRValue.push_back(m_out[i].roiSfrValue * 65536);
			}

			value = (m_sfrPara.RoiSFRValue[0]+ m_sfrPara.RoiSFRValue[1]+ m_sfrPara.RoiSFRValue[2]+ m_sfrPara.RoiSFRValue[3])/4.0f;

			ShowSFR_InfoAF();
		}

		FreeLibrary(hInstance);
#else
		m_sfrPara.RoiRect.clear();
	    m_sfrPara.RoiSFRValue.clear();
		ImageProc::GetSfrRoiForAF_byHalcon(img.RGB24_buffer,img.imgwidth,img.imgheight,&m_sfrPara);
		float SfrValue = 0;
		value = 0;
		if(m_sfrPara.RoiRect.size()!=4) return 0;
		_sfrresult sfrValue;
		for(int i=0;i<4;i++)
		{
			//m_sfrPara.RoiRect[i]
			 ImageProc::GetSFR_ofilm(img.Y_buffer,height,width,m_sfrPara.RoiRect[i],pDlg->m_roiSfrFreq,SfrValue,&sfrValue);
			 m_sfrPara.RoiSFRValue.push_back(SfrValue*65535);
		}
		value = (m_sfrPara.RoiSFRValue[0]+ m_sfrPara.RoiSFRValue[1]+ m_sfrPara.RoiSFRValue[2]+ m_sfrPara.RoiSFRValue[3])/4.0f;

		ShowSFR_InfoAF();
		
#endif
		*/
		break;
	} 
	return value;
}

 
int Inspection_AutoFocus::OnCtrlKeyNum()
{
	m_pInterface->VcmDr_WriteAF_Code(0,CamID);
	return 0;
}

int Inspection_AutoFocus::GetTemperate()
{
	//Sleep(500); 
	USHORT temperate;
	Sleep(500);
#if 1
	vector <CString> vecTmp;
	SeparateString(pDlg->m_tempOnOffReg, ',', &vecTmp);
	USHORT reg, val, slaveId;
	swscanf(vecTmp[0], _T("0x%x"), &reg);
	swscanf(vecTmp[1], _T("0x%x"), &val);
	swscanf(pDlg->m_tempSlaveId, _T("0x%x"), &slaveId);
	m_pInterface->Device_WriteI2c((UCHAR)slaveId,reg,val,3,CamID);

	swscanf(pDlg->m_tempAddrReg, _T("0x%x"), &reg);
	m_pInterface->Device_ReadI2c((UCHAR)slaveId,reg,&temperate,3,CamID);
#else
	if (CamID == 0)
		m_pInterface->Device_ReadI2c(0x34,0x013A,&temperate,3,CamID);
	else
		m_pInterface->Device_ReadI2c(0x20,0x013A,&temperate,3,CamID);
#endif
	Sleep(500);

	return (int)temperate; 
} 
 
void Inspection_AutoFocus::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();
	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //判断是否已','结尾
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//如果最后一个不是分割字符，再把后面的一段保存再退出
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
				strlist->push_back(str);
			}
			break;//如果最后一个就是分割字符，退出
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
		strlist->push_back(str);
		nStart = index + 1;
	}
}

double Inspection_AutoFocus::GetContrastAF(BYTE *pBuffer,int width, int height,CRect rect,int type)
{
	double returnValue = 0.0;
	if (pDlg->m_ImgType==0)
	{
		switch(type)
		{
		case 4:
			returnValue = GetResolution_Sobel(pBuffer,width,height,rect);
			break;
		case 5:
			returnValue = GetResolution_Variance(pBuffer,width,height,rect);
			break;
		case 6:
			returnValue = GetResolution_FVUp(Y_buffer,width,height,rect);
		default:
			returnValue = 0.0;
			break;
		}
		return returnValue;
	}
	//GetGBuffer; 
	int nWidth = img.imgwidth;
	int nHeight = img.imgheight;

	int GrWidth = nWidth/2;
	int GrHeight = nHeight/2;
	
    int nSize = GrWidth*GrHeight; //Gr Channel Only;
	
	BYTE *pTemp = new BYTE[nSize];
	memset( pTemp,0, nSize);
	int nCnt = 0;
	int i = 0, j = 0;
	switch (img.RawFormat)
	{
	case 0: //RGGB
		for(   i = 0; i < nHeight; i = i+2 )
		{
			for(   j = 0; j < nWidth; j = j+2)
			{
				pTemp[nCnt++] = pBuffer[i*nWidth+j+1];
			}
		}
		break;
		
	case 1: //GRBG
		for(   i = 0; i < nHeight; i = i+2 )
		{
			for(   j = 0; j < nWidth; j = j+2)
			{
				pTemp[nCnt++] = pBuffer[i*nWidth+j];
			}
		}		
		break;
		
	case 2://GBRG
		for(   i = 0; i < nHeight; i = i+2 )
		{
			for(   j = 0; j < nWidth; j = j+2)
			{
				pTemp[nCnt++] = pBuffer[(i+1)*nWidth+(j+1)];
			}
		}		
		break;
		
	case 3://OUTFORMAT_BGGR
		for(   i = 0; i < nHeight; i = i+2 )
		{
			for(   j = 0; j < nWidth; j = j+2)
			{
				pTemp[nCnt++] = pBuffer[(i+1)*nWidth+(j)];
			}
		}		
		break;
		
	default:
		break;
	}
	CRect GrRect;
	GrRect.top = rect.top/2;
	GrRect.bottom = rect.bottom/2;
	GrRect.left = rect.left/2;
	GrRect.right = rect.right/2;
	
	if (pDlg->m_ImgType==1)
	{
		switch(type)
		{
		case 4:
			returnValue = GetResolution_Sobel(pTemp,GrWidth,GrHeight,GrRect);
			break;
		case 5:
			returnValue = GetResolution_Variance(pTemp,GrWidth,GrHeight,GrRect);
			break;
		case 6:
			returnValue = GetResolution_FVUp(pTemp,GrWidth,GrHeight,GrRect);
		default:
			returnValue = 0;
			break;
		}
	}
	SAFE_DELETE_ARRAY(pTemp);
	return returnValue;
}

void Inspection_AutoFocus::MIPIP10ToRaw8(USHORT * pIn, USHORT * pOut, long number)
{
	int num = 0;
	for (long i=0; i<number; i ++)
	{
		pOut[i] = (pIn[i]>>2) & 0xff;
	}
}

//列主元方法求解n阶线性方程组Ax=b  
BOOL Inspection_AutoFocus::Solve(long double** A, long double* b, long double* x, int n)  
{  



	long double** M = new long double*[n];  
	int i,j,k,maxLineIndex,m;
	for ( i=0; i<n; i++)  

	{  

		M[i] = new long double[n+1];  

		for ( j=0; j<n; j++)  

		{  

			M[i][j] = A[i][j];  

		}  

		M[i][n] = b[i];  

	}  

	for ( k=0; k<n; k++)  

	{//n个主元  

		long double colMax = fabs(M[k][k]);  

		maxLineIndex = k;  

		for( i=k+1; i<n; i++)  

		{//寻找第k列的最大元素  

			if(fabs(M[i][k]) > colMax)  

			{  

				colMax = fabs(M[i][k]);  

				maxLineIndex = i;  

			}  

		}  
		const long double EPSILONG = 0.001;
		if(colMax < EPSILONG)  

		{//奇异矩阵  

			for ( i=0; i<n; i++)  

			{  

				delete M[i];  

			}  

			delete M;  

			return false;  

		}  

		long double temp;  

		//交换k行和maxLineIndex行  

		for ( m=0; m<n+1; m++)  

		{  

			temp = M[k][m];  

			M[k][m] = M[maxLineIndex][m];  

			M[maxLineIndex][m] = temp;  

		}  



		//消去  

		for( i=k+1; i<n; i++)  

		{  

			for ( j=k+1; j<n+1; j++)  

			{  

				M[i][j] = M[k][k]*M[i][j]/M[i][k] - M[k][j];  

			}  

		}  

	}  

	//回归求解  

	for ( i=n-1; i>=0; i--)  

	{  

		x[i] = M[i][n];  

		for ( j=i+1; j<n; j++)  

		{  

			x[i] -= M[i][j]*x[j];  

		}  

		x[i] /= M[i][i];  

	}  

	for ( i=0; i<n; i++)  

	{  

		delete M[i];  

	}  

	delete M;  

	return true;  

}  

//抛物线拟合

//  

//a0x^2+a1x+a2 = y  

//r:相关系数^2  

BOOL Inspection_AutoFocus::Parabola(long double* x, long double* y, int n, long double* a, long double* r)  
{  int i=0;

const int  CURVE_INDEX = 3;  

long double B[CURVE_INDEX];  

long double **A =new long double*[CURVE_INDEX];  

for ( i=0; i<CURVE_INDEX; i++)  

{  

	A[i] = new long double[CURVE_INDEX];  

}  
//long double A[CURVE_INDEX][CURVE_INDEX] = {0.0,0.0};

long double sumX=0, sumXX=0, sumXXX=0, sumXXXX=0, sumY=0, sumXY=0, sumXXY=0;  

for ( i=0; i<n; i++)  

{  

	sumX += x[i];  

	sumXX += x[i]*x[i];  

	sumXXX += x[i]*x[i]*x[i];  

	sumXXXX += x[i]*x[i]*x[i]*x[i];  

	sumY += y[i];  

	sumXY+= x[i]*y[i];  

	sumXXY += x[i]*x[i]*y[i];  

}  

A[0][0] = sumXXXX;A[0][1]=sumXXX;A[0][2]=sumXX;  

A[1][0] = sumXXX;A[1][1]=sumXX;A[1][2]=sumX;  

A[2][0] = sumXX;A[2][1]=sumX;A[2][2]=n;  

//  

B[0] = sumXXY; B[1] = sumXY; B[2] = sumY;  



bool solved = Solve(A, B, a, CURVE_INDEX);  

for ( i=0; i<CURVE_INDEX; i++)  

{  

	SAFE_DELETE_ARRAY(A[i]);
//	delete A[i];  

}  
SAFE_DELETE_ARRAY(A);
//delete A;  

//  

long double *ny = new long double[n];  

long double meanNY=0, meanYNY=0;  

for ( i=0; i<n; i++)  

{  

	ny[i] = a[0]*x[i]*x[i]+a[1]*x[i]+a[2];  

	meanNY += ny[i];  

	meanYNY += y[i]*ny[i];  

}  

meanYNY /= n;  

long double meanY = sumY / n;  

meanNY /= n;  

long double sumNY2=0, sumY2=0, sumYNY2=0;  

for ( i=0; i<n; i++)  

{  

	sumNY2 += (ny[i]-meanNY)*(ny[i]-meanNY);  

	sumY2 += (y[i] - meanY)*(y[i] - meanY);  

	sumYNY2 += (ny[i]-meanNY)*(y[i] - meanY);  

}  

*r=0;  
const long double EPSILONG = 0.001;
if(fabs(sumNY2) > EPSILONG && fabs(sumY2) > EPSILONG)  

{  

	*r = (sumYNY2*sumYNY2)/(sumNY2*sumY2);  

}  

delete []ny;  



return solved;  

}  

int Inspection_AutoFocus::GetCruveAF(int m_CutStep)
{
	int nCruveCode=0;
	int MaxIndex  = 0;
	//搜索结束后，进行曲线模拟
	MaxIndex = m_Curve.FindMax(m_CutStep);
	int num =  3;//AFData[LoopFlag].Code.size();
	long double x[100] = {0.0};
	long double y[100] = {0.0};
	long double coe[3] = {0.0};
	if (pDlg->m_UsePoint<=3)
	{
		x[0] = m_Curve.AFinfoArray[MaxIndex-1].m_Code*1.0;
		x[1] = m_Curve.AFinfoArray[MaxIndex].m_Code*1.0;	
		x[2] = m_Curve.AFinfoArray[MaxIndex+1].m_Code*1.0;
		y[0] = (long double)(m_Curve.AFinfoArray[MaxIndex-1].m_Value);  
		y[1] = (long double)(m_Curve.AFinfoArray[MaxIndex].m_Value);
		y[2] = (long double)(m_Curve.AFinfoArray[MaxIndex+1].m_Value);
	}
	else
	{
		x[0] = m_Curve.AFinfoArray[MaxIndex-2].m_Code*1.0;
		x[1] = m_Curve.AFinfoArray[MaxIndex-1].m_Code*1.0;	
		x[2] = m_Curve.AFinfoArray[MaxIndex].m_Code*1.0;
		x[3] = m_Curve.AFinfoArray[MaxIndex+1].m_Code*1.0;
		x[4] = m_Curve.AFinfoArray[MaxIndex+2].m_Code*1.0;	
		y[0] = (long double)(m_Curve.AFinfoArray[MaxIndex-2].m_Value);  
		y[1] = (long double)(m_Curve.AFinfoArray[MaxIndex-1].m_Value);
		y[2] = (long double)(m_Curve.AFinfoArray[MaxIndex].m_Value);
		y[3] = (long double)(m_Curve.AFinfoArray[MaxIndex+1].m_Value);  
		y[4] = (long double)(m_Curve.AFinfoArray[MaxIndex+2].m_Value);
	}
	long double r;
	Parabola(x,y,num,coe,&r);
	long double a=coe[0];
	long double b=coe[1];
	long double c=coe[2];
	nCruveCode = (int)(((-1)*(b/(2.0*a)))+0.5);
	strLog.Format(L"a:%f,b:%f,c:%f,Code:%d",a,b,c,nCruveCode);
	m_pInterface->Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);
	if (a>0.0)//开口朝上的情况
	{
		nCruveCode = -1;
	}
	return nCruveCode;
}

double Inspection_AutoFocus::GetResolution_Sobel(BYTE* pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;

	double Row_sum = 0.0;
	double Col_sum = 0.0;
	double sum     = 0.0;
	double count   = 0.0;
	double temp = 0.0;
	for(int i=rect.top+1;i<rect.bottom-1;i++)   
	{
		for(int j=rect.left+1;j<rect.right-1;j++)
		{
			Row_sum = (pSrc[(i+1)*width + (j-1)] + 2*pSrc[(i+1)*width + j] + pSrc[(i+1)*width + (j+1)]
			- pSrc[(i-1)*width + (j-1)] - 2*pSrc[(i-1)*width + j] - pSrc[(i-1)*width + (j+1)]);

			Col_sum= (pSrc[(i-1)*width + (j+1)] + 2*pSrc[i*width + (j+1)] + pSrc[(i+1)*width + (j+1)]
			-pSrc[(i-1)*width + (j-1)] - 2*pSrc[i*width + (j-1)] - pSrc[(i+1)*width + (j-1)]);

			temp = ((Row_sum*Row_sum) + (Col_sum*Col_sum));
			sum += temp;
			count++;
		}
	}
	if (count == 0) return 0;
	return sum*100.0/ count;
}

double Inspection_AutoFocus::GetResolution_Variance(BYTE* pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;

	LONG64 sum     = 0;
	int  count   = 0;
	int  temp = 0;
	for(int i=rect.top+1;i<rect.bottom-1;i++)   
	{
		for(int j=rect.left+1;j<rect.right-1;j++)
		{
			temp =(pow((double)(pSrc[i*width+j] - pSrc[(i-1)*width+j]),4) +
				pow((double)(pSrc[i*width+j] - pSrc[i*width+j-1]),4) +
				pow((double)(pSrc[i*width+j] - pSrc[i*width+j+1]),4)+
				pow((double)(pSrc[i*width+j] - pSrc[(i+1)*width+j]),4)) ;
			sum += temp;
			count++;
		}
	}
	if (count == 0) return 0;
	return sum*1.0/ count*1.0;
}

double Inspection_AutoFocus::GetResolution_FVUp(BYTE* pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;

	double Row_sum = 0;
	double Col_sum = 0;
	double sum     = 0;
	double count   = 0;
	for(int i=rect.top;i<rect.bottom-2;i++)   
	{
		for(int j=rect.left;j<rect.right-2;j++)
		{
			Row_sum += abs(pSrc[j+i*width]*pSrc[j+1+i*width] - pSrc[j+i*width]*pSrc[j+2+i*width])*1.0;
			Col_sum += abs(pSrc[j+i*width]*pSrc[j+(i+1)*width] - pSrc[j+i*width]*pSrc[j+(i+2)*width])*1.0;
			count++;
		}
	}
	if (count == 0) return 0;
	sum = Row_sum + Col_sum;
	return sum*100 / count;
}
