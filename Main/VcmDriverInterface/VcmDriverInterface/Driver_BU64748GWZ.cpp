#include "StdAfx.h"
#include "ccmBaseInterface.h"
#include "Driver_BU64748GWZ.h"


Driver_BU64748GWZ::Driver_BU64748GWZ(void)
{
    m_bInitFlag = FALSE;
}


Driver_BU64748GWZ::~Driver_BU64748GWZ(void)
{
}

int Driver_BU64748GWZ::Init()
{
    /*if (TRUE == m_bInitFlag)
    {
        return 0;
    } */

    CString strLogInfo = _T("");
    WORD u16_dat = 0;
    u16_dat = I2C_func_MEM_READ(0x30);
    strLogInfo.Format(_T("ret0: %d"), u16_dat);
    Addlog(strLogInfo, COLOR_RED, 200);
    if (0x000D == u16_dat)
    {
        return 0;
    }

    int str = ADJ_OK;

    I2C_func_POFF_____();		// power Off.
    I2C_func_PON______();		// power On.
    func_CHK_VERSION();			// version check.

    str = func_PROGRAM_DOWNLOAD();	//program download.
    strLogInfo.Format(_T("ret1: %d"), str);
    Addlog(strLogInfo, COLOR_RED, 200);
    if(str != ADJ_OK)
        return str;

    str = func_COEF_DOWNLOAD();		//Coefficient download.
    strLogInfo.Format(_T("ret2: %d"), str);
    Addlog(strLogInfo, COLOR_RED, 200);
    if(str != ADJ_OK)
        return str;

    I2C_func_DSP_START();		//DSP Start.
    //WriteCode(100);	
    Set_Close_Mode();	
    /*Sleep(300);
    WriteCode(200);
    Sleep(300);
    WriteCode(400);
    Sleep(300);
    WriteCode(600);
    Sleep(300);
    WriteCode(800);
    Sleep(300);
    WriteCode(1000);     */
    m_bInitFlag = TRUE;  
    return str;  
}

int Driver_BU64748GWZ::ReadCode(USHORT *code)
{
	 
	return 0;
}

int Driver_BU64748GWZ::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	int bFlag = 0;
	USHORT regValue = -1;  
	/*int AFValueH = (code>>8)&0x3;
	int	AFValueL = ( code & 0xff);	
	AFValueH = (0xF2<<2)+AFValueH;
	bFlag = I2cWrite(SlaveID, AFValueH, AFValueL,0);//与直接模式的区别在于S[3:2]，4->1 code  */
    bFlag = I2cWrite(SlaveID, 0x00F2, code, 2);
    CString strLogInfo = _T("");
    //USHORT uRet = func_CHK_VERSION();

    strLogInfo.Format(_T("code: %d"), code);
    Addlog(strLogInfo, COLOR_RED, 200);
	if(bFlag<1)
	{
		return -1;
	}

	if (code == 0)
	{
		Sleep(100);
	}

    //WriteCode(0x200);				//It depends.
    			//Set Close mode.
	return 0;
}

int Driver_BU64748GWZ::AutoFocus()
{	
	return 0;
}

void Driver_BU64748GWZ::Set_Close_Mode()
{
    I2C_func_MEM_WRITE( _M_30_EQCTL , 0x000D);
}

void Driver_BU64748GWZ::Set_Open_Mode()
{
    I2C_func_MEM_WRITE( _M_30_EQCTL , 0x0004);
}


void Driver_BU64748GWZ::I2C_func_PON______()
{
    I2C_func_PER_WRITE( 0xEF, 0x0080 );
}

void Driver_BU64748GWZ::I2C_func_POFF_____()
{
    I2C_func_PER_WRITE( 0xEF, 0x0000 );
}

USHORT Driver_BU64748GWZ::func_CHK_VERSION( )
{
    WORD u16_dat = 0;
    u16_dat = I2C_func_PER_READ( 0x5F );
    CString strLogInfo = _T("");
    strLogInfo.Format(_T("Version: 0x%04x"), u16_dat);
    Addlog(strLogInfo, COLOR_RED, 200);
    return u16_dat;
}

int Driver_BU64748GWZ::func_PROGRAM_DOWNLOAD()
{
    int sts=ADJ_OK;		

    download(0);

    int ver_check = 0;
    ver_check = I2C_func_MEM_READ( _M_F7_FBAF_STS );	// Check Status

    if ( ( ver_check & 0x0004 ) == 0x0004 )
    {
        WORD u16_dat = I2C_func_MEM_READ(_M_FIRMVER);
    }
    else
    {
        return PROG_DL_ERR;
    }
    return sts;
}

int Driver_BU64748GWZ::func_COEF_DOWNLOAD()
{
    int sts = ADJ_OK;

    download( 1 );

    WORD u16_dat = I2C_func_MEM_READ( _M_CD_CEFTYP );
    return sts;
}

//////////////////////////////////////////////////////
//		Download the data from file
//////////////////////////////////////////////////////
void Driver_BU64748GWZ::download(int type)
{
    /* Data Transfer Size per one I2C access */
#define		DWNLD_TRNS_SIZE		(32)

    unsigned char temp[DWNLD_TRNS_SIZE + 1];
    int block_cnt;
    int total_cnt;
    int lp;
    int n;
    int u16_i;

    if (type == 0)
        n = DOWNLOAD_BIN_LEN;
    else
        n = DOWNLOAD_COEF_LEN;	/* RHM_HT 2013/07/10    Modified */

    block_cnt = n / DWNLD_TRNS_SIZE + 1;
    total_cnt = block_cnt;

    //UINT32 u32Cnt = 0;
    while (1) {
        /* Residual Number Check */
        if (block_cnt == 1)
            lp = n % DWNLD_TRNS_SIZE;
        else
            lp = DWNLD_TRNS_SIZE;

        /* Transfer Data set */
        if (lp != 0) {
            if (type == 0) {
                temp[0] = _OP_FIRM_DWNLD;
                for (u16_i = 1; u16_i <= lp; u16_i += 1)
                    temp[u16_i] = DOWNLOAD_BIN[(total_cnt - block_cnt) *
                    DWNLD_TRNS_SIZE + u16_i - 1];
            } else {
                temp[0] = _OP_COEF_DWNLD;
                for (u16_i = 1; u16_i <= lp; u16_i += 1)
                    temp[u16_i] = DOWNLOAD_COEF[(total_cnt - block_cnt) *
                    DWNLD_TRNS_SIZE + u16_i - 1];
            }

            //temp[0] = temp[0] + lp * u32Cnt;   
            //++u32Cnt;
            /* Data Transfer */
            //WR_I2C(_SLV_OIS_, lp + 1, temp);
            //I2cWrite(SlaveID, temp, lp + 1 );
            pCcmCtr->Device_WriteI2cBlock(SlaveID, temp[0], 1, &temp[1], lp);
        }

        /* Block Counter Decrement */
        block_cnt = block_cnt - 1;
        if (block_cnt == 0)
            break;
    }
}


void Driver_BU64748GWZ::I2C_func_DSP_START()
{
    //BYTE out[2] = {0};

    //out[0] = _OP_SpecialCMD;
    //out[1] = _cmd_8C_EI;

    I2cWrite(SlaveID, _OP_SpecialCMD, _cmd_8C_EI, 0);
    //SOutEx( _SLV_FBAF_, out, 2);
}


///////////////////////////////////////////////////
// Peripheral Block Read Write
///////////////////////////////////////////////////
void Driver_BU64748GWZ::I2C_func_PER_WRITE(BYTE u08_adr, WORD u16_dat)
{
    BYTE out[4]={0};

    //out[0] = _OP_Periphe_RW;
    //out[1] = u08_adr;
    //out[2] = u16_dat & 0xFF;
    //out[3] = (u16_dat >> 8) & 0xFF;
    USHORT uReg = (_OP_Periphe_RW<<8) + u08_adr;
    USHORT uDataH = u16_dat >> 8;
    USHORT uDataL = u16_dat & 0xFF;
    u16_dat = (uDataL << 8) + uDataH;
    I2cWrite(SlaveID, uReg, u16_dat, 4);
}

WORD Driver_BU64748GWZ::I2C_func_PER_READ( BYTE u08_adr )
{
    //BYTE in[2] = {0};
    //BYTE read[2] = {0};
    WORD u16_dat;

    //in[0] = _OP_Periphe_RW;
    //in[1] = u08_adr;
    USHORT uReg = (_OP_Periphe_RW<<8) + u08_adr;
    //I2cRead(SlaveID, uReg, &u16_dat, 4);
    Sleep(200);
    //I2cReadBlock(SlaveID, uReg, 2, (BYTE*)&u16_dat, 2);
    pCcmCtr->Device_ReadI2cBlock(SlaveID, uReg, 2, (BYTE*)&u16_dat, 2, TRUE);
    //I2cRead(SlaveID, uReg, &u16_dat, 4);
    u16_dat = ((u16_dat&0xFF)<<8) + ((u16_dat>>8)&0xff);
    //I2cRead(SlaveID, uReg, &u16_dat, 4);
    //SInEx( _SLV_FBAF_, in, 2, read, 2);

    //u16_dat = ( read[0] * 256 ) + read[1];
    return u16_dat;
}

///////////////////////////////////////////////////
// Memory Block Read Write
///////////////////////////////////////////////////
void Driver_BU64748GWZ::I2C_func_MEM_WRITE( BYTE u08_adr, WORD u16_dat )
{
    //BYTE out[4];

    //out[0] = _OP_Memory__RW;
    //out[1] = u08_adr;
    //out[2] = u16_dat & 0xFF;
    //out[3] = (u16_dat >> 8 & 0xFF);

    USHORT uReg = (_OP_Memory__RW<<8) + u08_adr;
    USHORT uDataH = u16_dat >> 8;
    USHORT uDataL = u16_dat & 0xFF;
    u16_dat = (uDataL << 8) + uDataH;
    I2cWrite(SlaveID, uReg, u16_dat, 4);	
}

WORD Driver_BU64748GWZ::I2C_func_MEM_READ( BYTE u08_adr )
{
    //BYTE in[2] = {0};
    //BYTE read[2] = {0};
    WORD u16_dat;

    //in[0] = _OP_Memory__RW;
    //in[1] = u08_adr;

    //SInEx( _SLV_FBAF_, in, 2, read, 2);

    //u16_dat = ( read[0] * 256 ) + read[1];
    USHORT uReg = (_OP_Memory__RW<<8) + u08_adr;
    //I2cReadBlock(SlaveID, uReg, 2, (BYTE*)&u16_dat, 2);
    //I2cRead(SlaveID, uReg, &u16_dat, 4);
    pCcmCtr->Device_ReadI2cBlock(SlaveID, uReg, 2, (BYTE*)&u16_dat, 2, TRUE);
    u16_dat = ((u16_dat&0xFF)<<8) + ((u16_dat>>8)&0xff);
    return u16_dat;
}