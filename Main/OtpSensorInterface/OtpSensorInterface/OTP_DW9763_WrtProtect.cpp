#include "StdAfx.h"     
#include "ccmBaseInterface.h"
#include "OTP_DW9763_WrtProtect.h"


OTP_DW9763_WrtProtect::OTP_DW9763_WrtProtect(void)
{
}


OTP_DW9763_WrtProtect::~OTP_DW9763_WrtProtect(void)
{
}


int OTP_DW9763_WrtProtect::OtpInit()
{	
	return 0;
}

int OTP_DW9763_WrtProtect::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;
    USHORT uPageSize = 32;
    BYTE* pByteBuf = new BYTE[endAddr-startAddr+1];
    if (NULL == pByteBuf) return 0;
    
    USHORT uBufLen = 0;
    USHORT uGroup = uPageSize;
    for (USHORT uAddr=startAddr; uAddr<=endAddr; uAddr+=uPageSize)
    {
        uBufLen = endAddr - uAddr + 1;
        if (uBufLen < uPageSize)
        {
            uGroup = uBufLen;
        }

        if (TRUE != I2cReadBlock(uAddr, 2, pByteBuf+uAddr-startAddr, uGroup))
        {
            delete[] pByteBuf;
            return 0;
        }

		if (SendWaitAck() != 1) 
			return 0;
    }

    for(USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {
        buf[uAddr-startAddr] = pByteBuf[uAddr-startAddr];
    }

    delete[] pByteBuf;
    pByteBuf = NULL;

    return 1;
}

int OTP_DW9763_WrtProtect::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
    //擦除之前先读取数据备份，擦除之后，修改指定内容后写入
    USHORT* pReadData = new USHORT[EEPROMENDADDR - EEPROMSTARTADDR + 1];
    if (NULL == pReadData)
    {
        return 0;
    }

    if (TRUE != OtpRead(EEPROMSTARTADDR, EEPROMENDADDR, pReadData))
    {
        delete[] pReadData;
        return 0;
    }

    int i32DriverID = this->pCcmCtr->Config_ReadConfigInt(_T("VCM_Driver"), _T("VCM_SlaveID"), 0x18, CamID);
	if(this->pCcmCtr->Device_WriteI2c((UCHAR)i32DriverID, 0xf9, 0x7f, 0, CamID) != 1) return 0;
    if(this->pCcmCtr->Device_WriteI2c((UCHAR)i32DriverID, 0xa0, 0x04, 0, CamID) != 1) return 0;
    
    if (TRUE != I2cWrite(0x81, 0xee, 0)) return 0;   //擦除所有数据
	if (SendWaitAck() != 1) return 0;

    USHORT uPageSize = 32;
    BYTE* pByteBuf = new BYTE[EEPROMENDADDR - EEPROMSTARTADDR + 1];
    if (NULL == pByteBuf)
    {
        delete[] pReadData;
        return 0;
    }

    for(USHORT uAddr=EEPROMSTARTADDR; uAddr<=EEPROMENDADDR; ++uAddr)
    {
        if ((uAddr>=startAddr) && (uAddr<=endAddr))
        {
            pByteBuf[uAddr] = buf[uAddr-startAddr]  & 0xFF;
        }
        else
        {
            pByteBuf[uAddr] = pReadData[uAddr];
        }
    }

    USHORT uBufLen = 0;
    USHORT uGroup = uPageSize;
    for (USHORT uAddr=EEPROMSTARTADDR; uAddr<=EEPROMENDADDR; uAddr+=uPageSize)
    {
        uBufLen = EEPROMENDADDR - uAddr + 1;
        if (uBufLen < uPageSize)
        {
            uGroup = uBufLen;
        }

        if (TRUE != I2cWriteBlock(uAddr, 2, pByteBuf+uAddr, uGroup))
        {
            delete[] pReadData;
            delete[] pByteBuf;
            return 0;
        }

		if (SendWaitAck() != 1) 
			return 0;
    }

    delete[] pReadData;
    delete[] pByteBuf;
    pByteBuf = NULL; 
    pReadData = NULL;

	if(this->pCcmCtr->Device_WriteI2c((UCHAR)i32DriverID, 0xa0, 0x00, 0, CamID) != 1) return 0;
    if(this->pCcmCtr->Device_WriteI2c((UCHAR)i32DriverID, 0xed, 0x9d, 0, CamID) != 1) return 0;    

    return 1;
}
 