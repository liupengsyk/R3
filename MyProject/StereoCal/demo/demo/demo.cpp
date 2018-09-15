#pragma once

#include "stdafx.h"
#include "hwdepthcalibapi.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <afx.h>
using namespace std;
 
#pragma comment(lib,"HWCalibDll.lib")

#define HDC_Calib_Malloc(type,size) (type *)malloc(sizeof(type)*(size))

#define HDC_Calib_Free(ptr)\
{\
	if (NULL != (ptr))\
{\
	free(ptr);\
	ptr = NULL;\
}	\
}

#define HDC_CALIB_DISTANCE_NUM_DAFAULT   (2)         /**< �궨�������� */
#define HDC_CALIB_IMAGE_NUM_DAFAULT      (3)         /**< �궨ÿ������ͼ������ */
#define HDC_CALIB_OTP_BUF_SIZE_DAFAULT   (600)//(8804)//(480)       /**< OTP buffer Ĭ�ϴ�С byte */


int g_iDistance[3] = 
{
    //15,30,60
	//18,35,60
	20,50
};

int g_iAngle[3] = 
{
	//15,30,60
	//18,35,60
	0,25,-25
};


static HDC_Calib_BOOL ReadStringList( const string& filename, vector<string>& l )
{
	l.resize(0);
	ifstream fin(filename);

	if (!fin.is_open())
	{ 
		printf("Error opening %s file\n",filename.c_str()); 
	}

	char buffer[256];
	while (!fin.eof() ) 
	{
		fin.getline(buffer,255);
		string str(buffer);
		if (string::npos != str.find("jpg") || string::npos != str.find("bmp")|| string::npos != str.find("jpeg"))
		{
			l.push_back(str);
		}
	}
	
	return (l.size()>0)?HDC_CALIB_TRUE:HDC_CALIB_FALSE;
}

#if 0
//#if _DEBUG
//HDC_CALIB_EXPORT_API HDC_Calib_INT32 HDC_Calib_ParseAndPrintCameraParams(const char *otpFile,const char* outFile);
//#endif
int main(int argc, char* argv[])
{
	//if (argc < 3)
	//{
	//	printf("Error USAGE: 'exe otp.bin out.txt' !\n");
	//	return HDC_CALIB_STATUS_FAILE;
	//}
	
	
	const char* inFile  = "otp.bin";
	const char* outFile = "out.txt";

	HDC_Calib_INT32 iRet = HDC_Calib_ParseAndPrintCameraParams(inFile,outFile);
	if (iRet < 0)
	{
		printf("Parse Camera Params Error!\n");
	}
	return 0;
}


#else//����

#define HDC_EN_TEST (0)

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Error USAGE: 'exe setting.xml' !\n");
        return HDC_CALIB_STATUS_FAILE;
    }

    int iLength = strlen(argv[1]);
    if (iLength > HDC_CALIB_MAX_PATH || iLength <= 0)
    {
        printf("Error USAGE: 'exe setting.xml' !\n");
        return HDC_CALIB_STATUS_FAILE;
    }

#if 0
	HDC_Calib_INT32 vcm_code_7cm = 723;
	HDC_Calib_INT32 vcm_code_15cm = 416;
	HDC_Calib_INT32 vcm_code_2000cm = 214;
	HDC_Calib_INT32 distance_cm = 17;
	HDC_Calib_INT32 vcm_code = 0;
	HDC_Calib_eRetFlag eRetFlag = HDC_Calib_GetVCMCodeByDepth(vcm_code_7cm, vcm_code_15cm, vcm_code_2000cm, distance_cm, &vcm_code);
	if (HDC_CALIB_RET_OK != eRetFlag )
	{
		printf("vcm code calc error!\n");
	}
	else
	{
		if (vcm_code != /*385*/392)
		{
			printf("Error:vcm code[%d] != 385!\n",vcm_code);
			assert(0);
			//return 0;
		}
		printf("vcm code = %d!\n",vcm_code);
	}
#endif

    HDC_Calib_BOOL  bRet        = HDC_CALIB_FALSE;
    HDC_Calib_INT32 iFileCount  = 0;
    HDC_Calib_INT32 iCount      = 0;
    HDC_Calib_InitParams        stInitParams;
    HDC_Calib_Output   		    stOutput;
    HDC_Calib_InputParams	    stInputParams;

    HDC_Calib_INT8  chOutputLog[HDC_CALIB_MAX_PATH];
    HDC_Calib_eStatus   eStatus = HDC_CALIB_STATUS_FAILE;
    HDC_Calib_eRetFlag  eRet    = HDC_CALIB_RET_FAIL;		
    HDC_CALIB_HANDLE    pHandle = HDC_CALIB_NULL;
    HDC_Calib_INT32 iTest = 0;
    FILE *pOTPFile = HDC_CALIB_NULL;

    vector<string> vecFiles;
    int iFileNum = 0;
    bRet = ReadStringList("all_images.xml",vecFiles);
    iFileNum = (int)vecFiles.size();
    if (HDC_CALIB_FALSE == bRet || 0 == iFileNum )
    {
        printf("read 'all_images.xml' file fail !\n");
        return HDC_CALIB_STATUS_FAILE;
    }
	//˫��Ե�²��� ѹ��ͼƬ1600*1200   MMI��׼�����룬  �Ƕ�+/-25�㡣
    stInitParams.uiDistanceNum  = HDC_CALIB_DISTANCE_NUM_DAFAULT;
    stInitParams.eImageType     = HDC_CALIB_TYPE_BMP;
    memcpy(stInitParams.chCfgName ,argv[1],iLength);
	 printf(stInitParams.chCfgName);
    stInitParams.chCfgName[iLength] = '\0';
    stInitParams.pDrawFun   = HDC_CALIB_NULL;
    stInitParams.pLogFunc   = HDC_CALIB_NULL;
    stInitParams.pOutLog    = HDC_CALIB_NULL;
    stInitParams.eDbgLevel  = HDC_CALIB_LOG_INFO;

    stOutput.uiBufSize      = 600;//HDC_CALIB_OTP_BUF_SIZE_DAFAULT;
    stOutput.pCalibBuffer   = HDC_Calib_Malloc(HDC_Calib_UINT8,stOutput.uiBufSize*sizeof(HDC_Calib_UINT8));
    if (HDC_CALIB_NULL == stOutput.pCalibBuffer)
    {
        printf("Error in Create HDC Output buffer!\n");
        return HDC_CALIB_STATUS_FAILE;
    }
	 printf("sucess in Create HDC Output buffer!\n");
    eRet =  HDC_Calib_Create(&stInitParams,&pHandle); 
    if (HDC_CALIB_RET_OK != eRet)
    {
        printf("Error in Create HDC Calibration!\n");
        return HDC_CALIB_STATUS_FAILE;
    }
	 printf("sucess in Create HDC Calibration!\n");
            
	//ѭ���ȴ�����ͼ��
#if HDC_EN_TEST
    while (iFileCount < 1)
#else
    while (iFileCount < iFileNum-(stInitParams.uiDistanceNum*2))
#endif
    {
		CStdioFile logfile;
		logfile.Open(_T("log.txt"),CFile::modeCreate|CFile::modeWrite);
        bRet   = HDC_CALIB_TRUE;
        iCount = 0;

        char tmp[256];
        sprintf(tmp,"otp_%d.bin",iTest);
        pOTPFile = fopen(tmp,"wb");
        if (HDC_CALIB_NULL == pOTPFile)
        {
            printf("open otp.bin file fail !\n");
            return HDC_CALIB_STATUS_FAILE;
        }
        iTest++;

        do 
		{                
			stInputParams.uiImagesNum   = HDC_CALIB_IMAGE_NUM_DAFAULT;
			stInputParams.uiDistance    = g_iDistance[iCount];
            if (iFileNum <= (int)(iFileCount+stInputParams.uiImagesNum))
            {
                printf("HDC_Calib_Process:Input File Not Enought,HDC Stop!\n");
                bRet = HDC_CALIB_FALSE;
                break;
            }
            for (HDC_Calib_INT32 i = 0; i < (HDC_Calib_INT32)stInputParams.uiImagesNum;++i)
            {
				stInputParams.stImageList[i].iChartAngle  = g_iAngle[i];
                memcpy_s(stInputParams.stImageList[i].chLeftImageName,HDC_CALIB_MAX_PATH,vecFiles[iFileCount++].c_str(),HDC_CALIB_MAX_PATH);
                memcpy_s(stInputParams.stImageList[i].chRightImageName,HDC_CALIB_MAX_PATH,vecFiles[iFileCount++].c_str(),HDC_CALIB_MAX_PATH);
            }

            eRet = HDC_Calib_Process (pHandle,&stInputParams);
            if (HDC_CALIB_RET_OK != eRet)
            {
                //���У׼ʧ��,�㷨������̷��ش���
                printf("HDC_Calib_Process:Error in HDC Calibration Processing!\n");
                bRet = HDC_CALIB_FALSE;
                break;
            }

            iCount++;
#if HDC_EN_TEST
        } while (iCount < 1);
#else
        } while (iCount < stInitParams.uiDistanceNum);
#endif

        if (HDC_CALIB_TRUE == bRet)
        {
            eRet = HDC_Calib_CheckParams(pHandle,&eStatus,chOutputLog);
            if (eRet == HDC_CALIB_RET_OK)
            {
                if (eStatus != HDC_CALIB_STATUS_OK)
                {
                    //todo: ��У׼�������ʧ�ܽ��д���
                    //�Բ��׶Σ����ȱ������У׼�Ĳ������ں���������ֵ���ݲ���NG�Ĳ���
                    printf("HDC_Calib_CheckParams:HDC Params Check Error!\n");
					logfile.WriteString(_T("HDC_FAIL\n"));
					logfile.WriteString(_T("HDC_Calib_CheckParams:HDC Params Check Error!\n"));
                }
               // else 
                {                    
                    //���У׼�ɹ������ñ������У׼���
                    eRet = HDC_Calib_GenerateOTPBuffer(pHandle,&stOutput);
                    if (eRet != HDC_CALIB_RET_OK)
                    {
                        printf("HDC_Calib_GenerateOTPBuffer:Error in HDC Calibration Processing!\n");
                        bRet = HDC_CALIB_FALSE;
						logfile.WriteString(_T("HDC_FAIL\n"));
						logfile.WriteString(_T("HDC_Calib_GenerateOTPBuffer:Error in HDC Calibration Processing!\n"));
                        //todo: ��У׼ʧ�ܽ��д���
                    }
                    else
                    {
                        //todo:�������У׼���,�����������д�Ĵ�������
                        printf("HDC_Calib_GenerateOTPBuffer:Generate OTP Buffer Success!\n");
						logfile.WriteString(_T("HDC_PASS\n"));
						logfile.WriteString(_T("HDC_Calib_GenerateOTPBuffer:Generate OTP Buffer Success!\n"));
                    }

                    //save for test
                    fwrite(stOutput.pCalibBuffer,1,stOutput.uiBufSize,pOTPFile);

                }
            }
            else
            {
                //todo: ��У׼����������ʧ�ܽ��д���
                printf("HDC_Calib_CheckParams:Error in HDC Params Check Processing Input Image!\n");
				logfile.WriteString(_T("HDC_FAIL\n"));
				logfile.WriteString(_T("HDC_Calib_CheckParams:Error in HDC Params Check Processing Input Image!\n"));
            }
        }
        else
        {
            //todo:��ǰģ�����У׼ʧ��,����ԭ��
            printf("HDC_Calib_Process:Error in HDC Calibration Processing,please Check!\n");
			logfile.WriteString(_T("HDC_FAIL\n"));
			logfile.WriteString(_T("HDC_Calib_Process:Error in HDC Calibration Processing,please Check!\n"));
        }    
		 
		logfile.Close();
        fclose(pOTPFile);

	//�õ�otp���ݵ�ͬʱ����һ�����ݳ������������ݵ���Ч��

		HDC_Calib_ParseAndPrintCameraParams(tmp,"out.txt");
    }

    eRet = HDC_Calib_Destory(pHandle);  
    HDC_Calib_Free(stOutput.pCalibBuffer);
    return 0;
}

#endif

