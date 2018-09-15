#pragma once

typedef struct _GetTestSpec
{
	CString m_ProjectName1;
	CString m_ProjectName2;
	CString m_TestStation;
	int		m_Project1Check_En;
	int		m_Project2Check_En;

	int		m_OTPCheckAll_En;
	int		m_ModuleInfoCheck_En;
	int		m_AWBLSCCheck_En;
	int		m_AFCheck_En;
	int		m_PDAFCheck_En;
	int		m_HDCCheck_En;
	int		m_OISCheck_En;
	int		m_OCCheck_En;
	int		m_SFRCheck_En;
	int		m_AECCheck_En;
	int		m_SPCCheck_En;
	int		m_CrossTalkCheck_En;	
	int		m_ArcsoftCheck_En;
	int		m_OthersCheck_En;
	int		m_SegmentInfoCheck_En;
	int		m_DualCamCheck_En;
	int		m_OffLineCheck_En;				//离线验证数据隐藏接口，外发中不添加
    int     m_LRCCheck_En;
    int     m_OVPDAFStep_En;
	int		m_AFSyncCheck_En;		//AF Sync数据校验

	int		m_LSCCurveCheck;				//等于1表示用新算法及国内标准卡控，等于2采用锥形及国内标准卡控，等于其它仅采用国内标准卡控
	int		m_SaveOTPCheckData;
	int		m_EqualCountSpec; 				//连续多少颗模组不能存在相同的值
	int		m_HisiMasterFlag;
	int		m_TotalCheck;

	// check module Info
	int		m_EepromTableRevision;						// Fixed Value,refer to OTP Guide
	int		m_CalHwVersion;
	int		m_CalSwVersion;
	int		m_PartNumber1;
	int		m_PartNumber2;
	int		m_PartNumber3;
	int		m_PartNumber4;
	int		m_ActuatorID;						// Fixed Value,refer to OTP Guide
	int		m_LensID;	      				// Fixed Value,refer to OTP Guide
	int		m_ManufacturerID;					// Fixed Value,refer to OTP Guide
	int		m_FactoryID;					// Fixed Value,refer to OTP Guide
	int		m_ManufactureLine;
	int		m_Year;							// Fixed Value,refer to OTP Guide
	int		m_Month;						// Fixed Value,refer to OTP Guide
	int		m_Day;							// Fixed Value,refer to OTP Guide


	// check AWB Info
	int		m_MaxCIE_Ev_Source;
	int		m_MinCIE_Ev_Source;
	int		m_MaxCIE_U_Source;
	int		m_MinCIE_U_Source;
	int		m_MaxCIE_V_Source;
	int		m_MinCIE_V_Source;
	int		m_u_standard_Source;
	int		m_v_standard_Source;
	int		m_MaxTolerance_Source;
	int		m_MinTolerance_Source;

	int		m_MaxRValue;					// Set by ok module CPK,refer to OTP Guide	
	int		m_MinRValue;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxBValue;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinBValue;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGrValue;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGrValue;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGbValue;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGbValue;					// Set by ok module CPK,refer to OTP Guide

	int		m_ReCal_RG_diff;
	int		m_ReCal_BG_diff;
	int		m_ReCal_GG_diff;

	int		m_MaxRGain;						// Set by ok module CPK,refer to OTP Guide
	int		m_MinRGain;						// Set by ok module CPK,refer to OTP Guide
	int		m_MaxBGain;						// Set by ok module CPK,refer to OTP Guide
	int		m_MinBGain;						// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGGGain;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGGGain;					// Set by ok module CPK,refer to OTP Guide

	float	m_MaxRGRatio_UUT_diff;
	float	m_MinRGRatio_UUT_diff;
	float	m_MaxBGRatio_UUT_diff;
	float	m_MinBGRatio_UUT_diff;

	int		m_MaxRGolden;
	int		m_MinRGolden;
	int		m_MaxGrGolden;
	int		m_MinGrGolden;
	int		m_MaxGbGolden;
	int		m_MinGbGolden;
	int		m_MaxBGolden;
	int		m_MinBGolden;

	int		m_MaxRGainGolden;
	int		m_MinRGainGolden;
	int		m_MaxBGainGolden;
	int		m_MinBGainGolden;
	int		m_MaxGGainGolden;
	int		m_MinGGainGolden;
	int		m_BLC;
	int		m_Multiple;

	//OC Info
	int		m_PixelHeight;
	int		m_PixelWidth;
	int		m_MaxOCDistance;
	float	m_PixelSize;


	int		m_AWBFlag;						// Fixed Value,refer to OTP Guide
	int		m_SegmentAWBTagID;				// Fixed Value,refer to OTP Guide
	int		m_AWBTypeNumber;				// Fixed Value,refer to OTP Guide
	int		m_AWBMirror;					// Fixed Value,refer to OTP Guide
	int		m_AWBFilp;						// Fixed Value,refer to OTP Guide
	int     m_LSCHighByte;
	int     m_LSCLowByte;

	
	int		m_MaxRBGain;					// RGain/BGain ratio *1000,if RGain > BGain that MinRBGain > 1000
	int		m_MinRBGain;					// RGain/BGain ratio *1000,if RGain < BGain that MaxRBGain < 1000
	int		m_Typical_RG;					// Fixed Value,refer to OTP Guide
	int		m_Typical_BG;					// Fixed Value,refer to OTP Guide
	int		m_Typical_GG;					// Fixed Value,refer to OTP Guide
	int		m_RGainEqualCountSpec;			//连续多少颗模组不能存在相同的RGain值
	int		m_BGainEqualCountSpec;			//连续多少颗模组不能存在相同的BGain值
	int		m_GGGainEqualCountSpec;			//连续多少颗模组不能存在相同的GGain值

	
	int		m_Typical_R;					// Fixed Value,refer to OTP Guide
	int		m_Typical_B;					// Fixed Value,refer to OTP Guide
	int		m_Typical_Gr;					// Fixed Value,refer to OTP Guide
	int		m_Typical_Gb;					// Fixed Value,refer to OTP Guide
	int		m_MaxISO;						// Fixed Value,refer to OTP Guide
	int		m_MinISO;						// Fixed Value,refer to OTP Guide
	//2017.7.31新增(D50_5100K)
	int		m_MaxRGain_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinRGain_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxBGain_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinBGain_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGGGain_HighCCT;			// Set by ok module CPK,refer to OTP Guide
	int		m_MinGGGain_HighCCT;			// Set by ok module CPK,refer to OTP Guide
	int		m_Typical_RG_HighCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_BG_HighCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_GG_HighCCT;			// Fixed Value,refer to OTP Guide
	//2017.12.14新增光源系数卡空
	int m_MaxRGRatio_HighCCT;
	int	m_MinRGRatio_HighCCT;
	int	m_MaxBGRatio_HighCCT;
	int	m_MinBGRatio_HighCCT;
	int	m_MaxGGRatio_HighCCT;
	int	m_MinGGRatio_HighCCT;

	int m_MaxRGRatio_LowCCT;
	int	m_MinRGRatio_LowCCT;
	int	m_MaxBGRatio_LowCCT;
	int	m_MinBGRatio_LowCCT;
	int	m_MaxGGRatio_LowCCT;
	int	m_MinGGRatio_LowCCT;
	//(TL84_4000K)
	int		m_MaxRGain_Fluore;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinRGain_Fluore;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxBGain_Fluore;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinBGain_Fluore;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGGGain_Fluore;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinGGGain_Fluore;				// Set by ok module CPK,refer to OTP Guide
	int		m_Typical_RG_Fluore;			// Fixed Value,refer to OTP Guide
	int		m_Typical_BG_Fluore;			// Fixed Value,refer to OTP Guide
	int		m_Typical_GG_Fluore;			// Fixed Value,refer to OTP Guide
	//(A_3100K)
	int		m_MaxRGain_LowCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinRGain_LowCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxBGain_LowCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinBGain_LowCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGGGain_LowCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinGGGain_LowCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_Typical_RG_LowCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_BG_LowCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_GG_LowCCT;			// Fixed Value,refer to OTP Guide

	//2017.8.7新增(5100K)
	int		m_MaxR_HighCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinR_HighCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGr_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinGr_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGb_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinGb_HighCCT;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxB_HighCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinB_HighCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_Typical_R_HighCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_Gr_HighCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_Gb_HighCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_B_HighCCT;			// Fixed Value,refer to OTP Guide
	int		m_AWBHighCCT_Flag;				// Fixed Value,refer to OTP Guide
	//(3000K)
	int		m_MaxR_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinR_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGr_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGr_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGb_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGb_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxB_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinB_LowCCT;					// Set by ok module CPK,refer to OTP Guide
	int		m_Typical_R_LowCCT;				// Fixed Value,refer to OTP Guide
	int		m_Typical_Gr_LowCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_Gb_LowCCT;			// Fixed Value,refer to OTP Guide
	int		m_Typical_B_LowCCT;				// Fixed Value,refer to OTP Guide
	int		m_AWBLowCCT_Flag;				// Fixed Value,refer to OTP Guide
	//(4000K)
	int		m_MaxR_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinR_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGr_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGr_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxGb_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGb_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxB_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinB_Fluore;					// Set by ok module CPK,refer to OTP Guide
	int		m_Typical_R_Fluore;				// Fixed Value,refer to OTP Guide
	int		m_Typical_Gr_Fluore;			// Fixed Value,refer to OTP Guide
	int		m_Typical_Gb_Fluore;			// Fixed Value,refer to OTP Guide
	int		m_Typical_B_Fluore;				// Fixed Value,refer to OTP Guide
	int		m_AWBFluore_Flag;				// Fixed Value,refer to OTP Guide

	// check 光源系数
	//(5100K)
	int		m_MaxHighCCTLightSource_RG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MinHighCCTLightSource_RG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MaxHighCCTLightSource_BG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MinHighCCTLightSource_BG;		// Set by ok module CPK,refer to OTP Guide
	int		m_LSCHighCCT_Flag;				// Fixed Value,refer to OTP Guide
	//(3000K)
	int		m_MaxLowCCTLightSource_RG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MinLowCCTLightSource_RG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MaxLowCCTLightSource_BG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MinLowCCTLightSource_BG;		// Set by ok module CPK,refer to OTP Guide
	int		m_LSCLowCCT_Flag;				// Fixed Value,refer to OTP Guide
	//(4000K)
	int		m_MaxFluoreLightSource_RG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MinFluoreLightSource_RG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MaxFluoreLightSource_BG;		// Set by ok module CPK,refer to OTP Guide
	int		m_MinFluoreLightSource_BG;		// Set by ok module CPK,refer to OTP Guide

	// NOT use
	int		m_ModulInfoFlag;				// Fixed Value,refer to OTP Guide
	int		m_ModuleID;      				// Fixed Value,refer to OTP Guide		
	int		m_DriverIcID;					// Fixed Value,refer to OTP Guide
	int		m_LSCToolVersion;				// Fixed Value,refer to OTP Guide
	int		m_PDAFToolVersion;				// Fixed Value,refer to OTP Guide
	int		m_ColorTemperatureID;			// Fixed Value,refer to OTP Guide
	int		m_FirstPixel;					// Fixed Value,refer to OTP Guide
	int		m_MirrorFlip;					// Fixed Value,refer to OTP Guide
	int		m_SensorNameID;					// Fixed Value,refer to OTP Guide Sensor ID
	int		m_ModuleCode;					// Fixed Value,refer to OTP Guide
	int		m_LightSourceFlag;				// Fixed Value,refer to OTP Guide
	int		m_AForFF_Flag;					// Fixed Value,refer to OTP Guide
	int		m_IRBGID; 	
	int		m_PlatformID;
	int		m_DualCameraFlag;
    int     m_OVPDAFStep;
	//新增20170821
	int		m_VendorID;
	int		m_OISFWVER;
	int		m_ProductID;
	int		m_MapVersionID;
	int		m_MapHistoryID;
	int		m_EepromorOTP;
	//新增20170524
	int		m_ModuleVersion;				// Fixed Value,refer to OTP Guide
	int		m_SoftVersion;					// Fixed Value,refer to OTP Guide
	int		m_AWBLSCStation;				// Fixed Value,refer to OTP Guide
	int		m_PDAFStation;					// Fixed Value,refer to OTP Guide
	int		m_AFStation;					// Fixed Value,refer to OTP Guide
	//新增20170704
	int		m_FnumberID;					// Fixed Value,refer to OTP Guide	
	//新增20170725
	int		m_CalibrationVersion;			// Fixed Value,refer to OTP Guide
	int		m_OISID;						// Fixed Value,refer to OTP Guide
	//新增20170731
	int		m_ProgramName;					// Fixed Value,refer to OTP Guide
	int		m_SensorSlaveID;				// Fixed Value,refer to OTP Guide
	int		m_EEPROMSlaveID;				// Fixed Value,refer to OTP Guide
	int		m_VCMSlaveID;					// Fixed Value,refer to OTP Guide
	int		m_SensorStatus;					// Fixed Value,refer to OTP Guide		
	//新增20170807
	int		m_CheckSN_En;					// 是否Check SN，“1”表示Check，反之不Check

	//新增20171214
	int     m_GyroID;
	int		m_OisDriverIcID;
	int		m_FpcPcbVersion;
	
	

	// check Segment Info
	int		m_SegmentInfoFlag;				// Fixed Value,refer to OTP Guide
	int		m_SegmentCount;					// Fixed Value,refer to OTP Guide
	int		m_SegmentAFOffset;				// Fixed Value,refer to OTP Guide
	int		m_SegmentAWBOffset;				// Fixed Value,refer to OTP Guide
	int		m_SegmentLSCOffset;				// Fixed Value,refer to OTP Guide
	int		m_SegmentAECOffset;				// Fixed Value,refer to OTP Guide
	int		m_SegmentPDAFOffset;			// Fixed Value,refer to OTP Guide
	int		m_SegmentOISOffset;				// Fixed Value,refer to OTP Guide
	int		m_SegmentDualCamOffset;			// Fixed Value,refer to OTP Guide
	int		m_SegmentChecksumOffset;		// Fixed Value,refer to OTP Guide


	int		m_LSCFluore_Flag;				// Fixed Value,refer to OTP Guide

	// check SN
	int		m_SNFlag;						// Fixed Value,refer to OTP Guide

	// check LSC
	int		m_LSCFlag;						// Fixed Value,refer to OTP Guide
	int		m_SegmentLSCTagID;				// Fixed Value,refer to OTP Guide
	int		m_LSCVersionNumber;				// Fixed Value,refer to OTP Guide
	int		m_LSCTypeNumber;				// Fixed Value,refer to OTP Guide
	int		m_LSCMirror;					// Fixed Value,refer to OTP Guide
	int		m_LSCFilp;						// Fixed Value,refer to OTP Guide
	int		m_LSCSize;		     
	int		m_LSCDataEqualCountSpec;		// LSCData不允许存在多少个连续相等的值
	int		m_ToleranceValueError;			// if LSCCurveCheck equal "2" that check four channel curve,allow a maximum value diff
	int		m_ToleranceCountError;			// if LSCCurveCheck equal "2" that check four channel curve,allow a maximum number of 

	//新增
	int		m_SaveLSCALLDataEn;				// 保存新算法的数据，生产时请勿开启，会存在卡死风险
	int		m_SaveLSCOKDataEn;				// 保存新算法的数据，只保存OK 模组的数据

	int		m_LSC_R_Max;				    // Use Standard.ini check
	int		m_LSC_R_Min;					// Use Standard.ini check
	int		m_LSC_Gr_Max;				    // Use Standard.ini check
	int		m_LSC_Gr_Min;					// Use Standard.ini check
	int		m_LSC_Gb_Max;			        // Use Standard.ini check
	int		m_LSC_Gb_Min;					// Use Standard.ini check
	int		m_LSC_B_Max;				    // Use Standard.ini check
	int		m_LSC_B_Min;					// Use Standard.ini check
	int		m_LSCDistanceSpec;				// Use Standard.ini check
    int     m_GainMapHighByte;
    int     m_GainMapLowByte;
    int     m_DCCMapHighByte;
    int     m_DCCMapLowByte;
   


	// Check LSC of New algorithm
	int		m_CenterValueEqualcount;		// 中心LSC最大值为1023或255，最多允许出现CenterValueEqualcount 个
	int		m_Curvediff;					// 允许相邻递增或递减趋势存在的误差值
	int		m_ErrorCount; 					// 允许每一行最多错误数
	int		m_TotalErrorCount;				// 允许每个channel 最多错误数
	float	m_edg_center_LimitRatio;		// 第1,2,16,17列与中心的第9列的比值
	float	m_edg_center_HighRatio;			// 第1,2,16,17列与中心的第9列的比值
	int		m_MaxSymColDiff;				// 允许对称列的最大差值
	int		m_MaxSymRowDiff;				// 允许对称行的最大差值

	// check AEC
	int		m_SegmentAECTagID;						// Fixed Value,refer to OTP Guide
	int		m_AECFlag;
	int		m_AECVersionNumber;
	int		m_MaxBrightnessRatio;
	int		m_MinBrightnessRatio;
	int		m_MainGain;
	int		m_MainExposureTime;
	int		m_SubGain;
	int		m_SubExposureTime;

	// check AF code
	int		m_AFFlag;						// Fixed Value,refer to OTP Guide
	int		m_AFCalDirection;
	int		m_SegmentAFTagID;
	int		m_AFTypeNumber;
	int		m_MaxInfinityCode;
	int		m_MinInfinityCode;
	int		m_MaxMacroCode;
	int		m_MinMacroCode;

	int		m_AFInfinityPeak;
	int		m_AFMacroPeak;

	int		m_MaxPostureDifA;
	int		m_MinPostureDifA;
	int		m_MaxPostureDifB;
	int		m_MinPostureDifB;
	int		m_MaxPostureDifC;
	int		m_MinPostureDifC;

	int		m_MaxAFCode1000cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinAFCode1000cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxAFCode500cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinAFCode500cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxAFCode10cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinAFCode10cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxAFCode12cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MinAFCode12cm;				// Set by ok module CPK,refer to OTP Guide
	int		m_MaxAFCode8cm;		
	int		m_MinAFCode8cm;	
	int		m_MaxAFCode7cm;		
	int		m_MinAFCode7cm;
	int		m_MaxCodeDiff;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinCodeDiff;					// Set by ok module CPK,refer to OTP Guide
	int		m_MaxStartCode;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinStartCode;					// Set by ok module CPK,refer to OTP Guide
	int		m_AFCode8cmEqualCountSpec;    	// 连续多少颗模组不能存在相同的8cm AFCode值
	int		m_AFCode1000cmEqualCountSpec;	// 连续多少颗模组不能存在相同的10m AFCode值
	int		m_MaxAFStartCurrent;
	int		m_MinAFStartCurrent;

	// check SFR code
	int m_Posture_Macro;
	int m_MinCTPosition_Macro;
	int m_MaxCTPosition_Macro;
	int m_MinCenter_Macro;		//Macro 中心视场
	int m_MinField4_Macro;		//Macro 0.4视场
	int m_MinField6_Macro;		//Macro 0.6视场
	int m_MinField8_Macro;		//Macro 0.8视场
	int m_MaxField6CTC_Macro;

	int m_Posture_Mid;
	int m_MinCTPosition_Mid;
	int m_MaxCTPosition_Mid;
	int m_MinCenter_Mid;		//Mid 中心视场
	int m_MinField4_Mid;		//Mid 0.4视场
	int m_MinField6_Mid;		//Mid 0.6视场
	int m_MinField8_Mid;		//Mid 0.8视场
	int m_MaxField6CTC_Mid;

	int m_Posture_Infinity;
	int m_MinCTPosition_Infinity;
	int m_MaxCTPosition_Infinity;
	int m_MinCenter_Infinity;	//Infinity 中心视场
	int m_MinField4_Infinity;	//Infinity 0.4视场
	int m_MinField6_Infinity;	//Infinity 0.6视场
	int m_MinField8_Infinity;	//Infinity 0.8视场
	int m_MaxField6CTC_Infinity;

	// check PDAF code 
	int		m_PDAFFlag;						// Fixed Value,refer to OTP Guide
	int		m_SegmentPDAFTagID;
	int		m_PDAFVersionNum;
	int		m_PDAFTypeNumber;
	int		m_PDAFStep1Flag;	
	int		m_PDAFStep1Size;	  

	int		m_PDAFStep2Flag;	  
	int		m_PDAFStep2Size;	  

	int		m_GainMapFlag;	 				// Fixed Value,refer to OTP Guide
	int		m_GainMapWidth;					// Fixed Value,refer to OTP Guide
	int		m_GainMapHeight;				// Fixed Value,refer to OTP Guide
	int		m_MaxGainMap;					// Set by ok module CPK,refer to OTP Guide
	int		m_MinGainMap;					// Set by ok module CPK,refer to OTP Guide

	int		m_DCCFlag;	 					// Fixed Value,refer to OTP Guide
	int		m_DCCFormat;					// Fixed Value,refer to OTP Guide
	int		m_DCCMapWidth;					// Fixed Value,refer to OTP Guide
	int		m_DCCMapHeight;					// Fixed Value,refer to OTP Guide
	int		m_MaxDCC;						// Set by ok module CPK,refer to OTP Guide
	int		m_MinDCC;						// Set by ok module CPK,refer to OTP Guide

	// check Dual camera calibration
	int		m_SegmentDualCamTagID;
	int		m_DualCamFlag;
	int		m_DualCamVersionNumber;
	int		m_BurnDualCalibration;

	// check CheckSum
	int		m_SegmentCheckSumTagID;

	// check OIS(2017.06.08新增)
	int		m_OISWideTag;					// Fixed Value,refer to OTP Guide
	int		m_OISVersion;					// Fixed Value,refer to OTP Guide

	// check Optical Center/Tilt/Rotation(2017.06.08新增)
	int	m_OCFlag;		  			// Fixed Value,refer to OTP Guide
	int	m_MaxOCX;	  	  			// Set by ok module CPK,refer to OTP Guide
	int	m_MinOCX;	  				// Set by ok module CPK,refer to OTP Guide
	int	m_MaxOCY;	  				// Set by ok module CPK,refer to OTP Guide
	int	m_MinOCY;	  	  			// Set by ok module CPK,refer to OTP Guide
	int	m_MaxShiftX;	 			// Set by ok module CPK,refer to OTP Guide
	int	m_MinShiftX;	  			// Set by ok module CPK,refer to OTP Guide
	int	m_MaxShiftY;	 			// Set by ok module CPK,refer to OTP Guide
	int	m_MinShiftY;	 			// Set by ok module CPK,refer to OTP Guide
	int	m_MaxTiltX;	  				// Set by ok module CPK,refer to OTP Guide
	int	m_MinTiltX;	 				// Set by ok module CPK,refer to OTP Guide
	int	m_MaxTiltY;					// Set by ok module CPK,refer to OTP Guide
	int	m_MinTiltY;					// Set by ok module CPK,refer to OTP Guide
	int	m_MaxRotation;				// Set by ok module CPK,refer to OTP Guide
	int	m_MinRotation;	 			// Set by ok module CPK,refer to OTP Guide

	// check SonySPC(2017.06.10新增)
	int	m_MaxSPC;					// Set by ok module CPK,refer to OTP Guide
	int	m_MinSPC;					// Set by ok module CPK,refer to OTP Guide

	// check AF(up and down posture) Code
	int	m_BurnedAFUpDownPosture;		// If Burned AF(up and down posture) there will set "1",else set "0"

	int	m_AFUpDownPostureFlag;			// Fixed Value,refer to OTP Guide
	int	m_MaxAFCode8cmUp;				// Set by ok module CPK,refer to OTP Guide
	int	m_MinAFCode8cmUp;				// Set by ok module CPK,refer to OTP Guide
	int	m_MaxAFCode1000cmUp;			// Set by ok module CPK,refer to OTP Guide
	int	m_MinAFCode1000cmUp;			// Set by ok module CPK,refer to OTP Guide
	int	m_MaxCodeDiffUp;				// Set by ok module CPK,refer to OTP Guide
	int	m_MinCodeDiffUp;				// Set by ok module CPK,refer to OTP Guide

	int	m_MaxAFCode8cmDown;				// Set by ok module CPK,refer to OTP Guide
	int	m_MinAFCode8cmDown;				// Set by ok module CPK,refer to OTP Guide
	int	m_MaxAFCode1000cmDown;			// Set by ok module CPK,refer to OTP Guide
	int	m_MinAFCode1000cmDown;  		// Set by ok module CPK,refer to OTP Guide
	int	m_MaxCodeDiffDown;				// Set by ok module CPK,refer to OTP Guide
	int m_MinCodeDiffDown;				// Set by ok module CPK,refer to OTP Guide

	// Crosstalk Check
	int m_CrossTalkFlag;				// Set by ok module CPK,refer to OTP Guide

	// Arcsoft Check
	int m_ArcsoftFlag;					// Set by ok module CPK,refer to OTP Guide

	// Current Check
	int m_MaxAVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MinAVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MaxDVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MinDVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MaxIOVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MinIOVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MaxAFVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MinAFVDD;						// Set by ok module CPK,refer to OTP Guide
	int m_MaxDRVAVDD;					// Set by ok module CPK,refer to OTP Guide
	int m_MinDRVAVDD;					// Set by ok module CPK,refer to OTP Guide
	int m_MaxAVDD18;					// Set by ok module CPK,refer to OTP Guide
	int m_MinAVDD18;					// Set by ok module CPK,refer to OTP Guide
	int m_MaxAVDD28;					// Set by ok module CPK,refer to OTP Guide
	int m_MinAVDD28;					// Set by ok module CPK,refer to OTP Guide

    int m_LRCFlag;

}GetTestSpec;




class OtpCheckAPI
{
public:
	OtpCheckAPI()
	{
		m_OTPSize = 0;
	};
	virtual ~OtpCheckAPI(){};
public:
	//请赋值
	CString m_StationName;
	CString m_FuseID;


private:
	int m_OTPSize;
	GetTestSpec m_GetOTPCheckSpec;
	GetTestSpec m_GetOTPCheckSpec1;
	CString TESTSTATION;
	CString STATIONCHECKITEM;
	CString TESTSTATION_1;	//主摄里有副摄数据，所以也要检查副摄数据
	CString STATIONCHECKITEM_1;

public:
	int Testing( wchar_t *pszErrorMsg,BYTE *InputOTPData, int iInputOtpSize, int Flag,CString StationName);
private:
	int OtpCheck(char *pszOutputErrorMsg,int iMaxOutputErrorMsgLen,BYTE *InputOTPData, int iInputOtpSize, int Flag, char *pszFuseID,char *pszStationName);
	int OTPcheckImp(wchar_t* ErrorMsg,BYTE *InputOTPData, int Flag,CString StationName);
	BOOL OnInitialSpec_Project1(const CString &strFilePath,GetTestSpec &m_GetOTPCheckSpec);
	BOOL OnInitialSpec_Project2(const CString &strFilePath, GetTestSpec &m_GetOTPCheckSpec1);
	BOOL SaveLSCDataSum_LG(float * LSCValue, wchar_t * Errormsg, int flag);
	BOOL SaveLSCData_LG(CString channelName , USHORT * channelData, int CenterValueEqualcount, int ErrorCountMax, int TotalErrorCount, int ErrorValueMax, float * FourthCalculateRatio, int * SymColDiff, int * SymRowDiff, wchar_t * Errormsg, int flag);
	BOOL CheckLSCData_LG(USHORT *RchannelData,USHORT *GrchannelData,USHORT *GbchannelData,USHORT *BchannelData,int centerValue, wchar_t * Errormsg,int flag);
	void Catstring(wchar_t *Msg, const wchar_t *format, ...);
	BOOL LensVerificationLib(CString str,USHORT *Data,wchar_t *sErrorMsg,BOOL isProject1,BOOL isWide = TRUE);
	BOOL LSCDataCheck(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch,wchar_t * ErrorMsg);
	BOOL LSCDataCheck_Project1(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch,wchar_t *ErrorMsg);
	BOOL LSCDataCheck_Project2(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch,wchar_t *ErrorMsg);
	int SaveLSCData(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch);
	BOOL CheckModuleEqualValue(wchar_t *moduleKey,wchar_t *TestItemCountName,wchar_t *TestItemRefName,int testValue,int TestItemCountSpec,wchar_t* ErrorMsg);
	BOOL CheckSegmentInfo_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSegmentInfo_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckModuleInfo_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckModuleInfo_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckModuleInfo_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAWB_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAWB_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckHisiLSC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckHisiLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckHisiHDC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckQualcommLSC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckQualcommLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckQualcommLSC_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckMTKLSC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckMTKLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAFCode_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAFCode_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAFCode_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg);

	BOOL CheckSFRMacro_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSFR7M_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSFR7M_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSFRMid_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSFR8CM_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSFR8CM_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSFRInfinity_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);

	BOOL CheckQualcommSPC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckQualcommSPC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSonySPC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckHisiSPC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckLRC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckArcsoft_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckCrossTalk_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckOthers(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAEC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAEC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckOIS_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAFSync_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckAFSync_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckTotalCheckSum_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckDualCamera_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckHisiPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSonyPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckSonyPDAF_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckQualcommPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckQualcommPDAF_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckOC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL CheckMTKPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	BOOL GetDataFromTxt(BYTE *OTPData,int Flag);
	BOOL SaveOTPData(BYTE *OTPReadData,wchar_t *ErrorMsg,int Flag);
	BOOL CheckOVPDAFStep_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg);
	CString GetModulePath();

};