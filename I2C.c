#include "NVP1208.h"

//////////////////////////////////////////////////////////////////////////
//Applied HW mode I2C but it is not working properly so it could not be used any more
//////////////////////////////////////////////////////////////////////////
BOOL I2C_Init(PDEVICE_EXTENSION pdx)
{
	DWORD dwRegData=0;

	dwRegData = 1<<8;
	dwWriteRegister(pdx, REG_I2C_CONFIG0, dwRegData);
	dwWriteRegister(pdx, REG_I2C_CONFIG1, dwRegData);
	dwWriteRegister(pdx, REG_I2C_P0, 0x03);
	dwWriteRegister(pdx, REG_I2C_P1, 0x03);
	DelayMicroSecond(4);
	
	return TRUE;
}
//=================================================================================//
BOOL I2C_Read(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData)
{
	if (bPort == 0)
		return I2C_P0_Read(pdx, bDevAddr, bSubAddr, bData);
	else if (bPort == 1)
		return I2C_P1_Read(pdx, bDevAddr, bSubAddr, bData);

	return FALSE;
}
//=================================================================================//
BOOL I2C_Write(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE bData)
{
	if (bPort == 0)
		return I2C_P0_Write(pdx, bDevAddr, bSubAddr, bData);
	else
		return I2C_P1_Write(pdx, bDevAddr, bSubAddr, bData);

	return TRUE;
}

BOOL I2C_ReadBurst(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength)
{
	if (bPort == 0)
		return I2C_P0_ReadBurst(pdx, bDevAddr, bSubAddr, bData, dwLength);
	return FALSE;
}

BOOL I2C_WriteBurst(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength)
{
	if (bPort == 0)
		return I2C_P0_WriteBurst(pdx, bDevAddr, bSubAddr, bData, dwLength);
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//Software I2C control
BOOL I2C_P0_ReadBurst(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength)
{
	DWORD i;

	
	I2C_P0_Start(pdx);
	I2C_P0_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write device address - 0\n"));
		return FALSE;
	}
	I2C_P0_TransmitByte(pdx, bSubAddr);
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write sub address\n"));
		return FALSE;
	}
	
	I2C_P0_Start(pdx);
	I2C_P0_TransmitByte(pdx, (BYTE)I2CR(bDevAddr));

	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write device address -2 "));
		return FALSE;
	}

	for (i=0;i<dwLength;i++)
	{
		bData[i] = I2C_P0_ReceiveByte(pdx);
		if (i==(dwLength-1))
			I2C_P0_SendOutAck(pdx, 1);
		else
			I2C_P0_SendOutAck(pdx, 0);
		DelayMicroSecond(2);
	}
	I2C_P0_Stop(pdx);
	return TRUE;
}

BOOL I2C_P0_WriteBurst(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength)
{
	DWORD i;
	I2C_P0_Start(pdx);
	I2C_P0_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Write error at port 0: write device address\n"));
		return FALSE;
	}
	I2C_P0_TransmitByte(pdx, bSubAddr);
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Write error at port 0: write sub address\n"));
		return FALSE;
	}
	for (i=0;i<dwLength;i++)
	{
		I2C_P0_TransmitByte(pdx, bData[i]);
		I2C_P0_ReadAck(pdx);
	}
	I2C_P0_Stop(pdx);
	return TRUE;
}

//=================================================================================//
BOOL I2C_P0_Read(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData)
{
	BYTE bRead;
	KdPrint(("I2C_P0_Read %02X, %02X, %02X\n", bDevAddr, bSubAddr, *bData));

	I2C_P0_Start(pdx);
	I2C_P0_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write device address - 0\n"));
		return FALSE;
	}
	I2C_P0_TransmitByte(pdx, bSubAddr);
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write sub address\n"));
		return FALSE;
	}

	I2C_P0_Start(pdx);
	I2C_P0_TransmitByte(pdx, (BYTE)I2CR(bDevAddr));
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write device address - 1\n"));
		return FALSE;
	}

	*bData = I2C_P0_ReceiveByte(pdx);
	I2C_P0_SendOutAck(pdx, 1);
	I2C_P0_Stop(pdx);
	return TRUE;
}
//=================================================================================//
BOOL I2C_P0_Write(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE bData)
{
	KdPrint(("I2C_P0_Write %02X, %02X, %02X\n", bDevAddr, bSubAddr, bData));
	I2C_P0_Start(pdx);
	I2C_P0_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Write error at port 0: write device address\n"));
		return FALSE;
	}
	I2C_P0_TransmitByte(pdx, bSubAddr);
	if (!I2C_P0_ReadAck(pdx))
	{
		I2C_P0_Stop(pdx);
		KdPrint(("I2C Write error at port 0: write sub address\n"));
		return FALSE;
	}
	I2C_P0_TransmitByte(pdx, bData);
	I2C_P0_ReadAck(pdx);
	I2C_P0_Stop(pdx);
	return TRUE;
}
//=================================================================================//
void I2C_P0_Start(PDEVICE_EXTENSION pdx)
{
	I2C_P0_SetSDAMode(pdx, I2C_MODE_WRITE);
	I2C_P0_SetSDA(pdx, 1);
	I2C_P0_SetSCL(pdx, 1);

	I2C_P0_SetSDA(pdx, 0);
	DelayMicroSecond(1);		//un4given	//2010.11.16
}
//=================================================================================//
void I2C_P0_Stop(PDEVICE_EXTENSION pdx)
{
	I2C_P0_SetSDAMode(pdx, I2C_MODE_WRITE);

	I2C_P0_SetSCL(pdx, 0);
	I2C_P0_SetSDA(pdx, 0);

	I2C_P0_SetSCL(pdx, 1);
	DelayMicroSecond(1);		//un4given	//2010.11.16
	I2C_P0_SetSDA(pdx, 1);
	DelayMicroSecond(1);		//un4given	//2010.11.16
	I2C_P0_SetSCL(pdx, 1);
}
//=================================================================================//
void I2C_P0_SetSDAMode(PDEVICE_EXTENSION pdx, DWORD dwMode)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P0));

	//checked
	if (dwMode == I2C_MODE_READ)
		dwData = dwData | (1<<5);
	else
		dwData = dwData & ~(1<<5);

	WRITE_REGISTER_ULONG((DWORD*)(pdx->m_pbyMembase + REG_I2C_P0), dwData);
}
//=================================================================================//
void I2C_P0_SetSDA(PDEVICE_EXTENSION pdx, DWORD dwSig)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P0));
	
	//checked
	if (dwSig)
		dwData = dwData | (1<<1);
	else 
		dwData = dwData & ~(1<<1);

	WRITE_REGISTER_ULONG((DWORD*)(pdx->m_pbyMembase + REG_I2C_P0), dwData);				// 0x240
}
//=================================================================================//
BYTE I2C_P0_GetSDA(PDEVICE_EXTENSION pdx)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P0));

	return (BYTE) ((dwData>>3) & 0x1);
}
//=================================================================================//
void I2C_P0_SetSCL(PDEVICE_EXTENSION pdx, DWORD dwSig)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P0));

	if (dwSig)
		dwData = dwData | (1<<0);
	else
		dwData = dwData & ~(1<<0);

	WRITE_REGISTER_ULONG((DWORD*)(pdx->m_pbyMembase + REG_I2C_P0), dwData);
	DelayMicroSecond(1);		//un4given	//2010.11.16
}
//=================================================================================//
void I2C_P0_TransmitByte(PDEVICE_EXTENSION pdx, BYTE bData)
{
	int		i, nCount;

	I2C_P0_SetSDAMode(pdx, I2C_MODE_WRITE);

	for (i=0, nCount = 7; i<8; i++, nCount--)
	{
		I2C_P0_SetSCL(pdx, 0);
		DelayMicroSecond(1);		//un4given	//2010.11.16
		I2C_P0_SetSDA(pdx, (bData>>nCount)&1);
		DelayMicroSecond(1);		//un4given	//2010.11.16
		I2C_P0_SetSCL(pdx, 1);
	}
}
//=================================================================================//
BOOL I2C_P0_ReadAck(PDEVICE_EXTENSION pdx)
{
	DWORD	dwPreTick, dwCurTick;
	BYTE	bAck = 1;

	I2C_P0_SetSCL(pdx, 0);
	I2C_P0_SetSDAMode(pdx, I2C_MODE_READ);

	dwPreTick = GetTick();
	do
	{
		bAck = I2C_P0_GetSDA(pdx);
		dwCurTick = GetTick();
	} while ((bAck != 0) && GetTimeInterval(dwPreTick, dwCurTick)<500);
	
	I2C_P0_SetSCL(pdx, 1);
	I2C_P0_SetSCL(pdx, 0);

	if (bAck)
	{
		I2C_P0_Stop(pdx);
		return FALSE;
	}
	return TRUE;
}
//=================================================================================//
BYTE I2C_P0_ReceiveByte(PDEVICE_EXTENSION pdx)
{
	BYTE	bRecv = 0;
	int		i;

	I2C_P0_SetSDAMode(pdx, I2C_MODE_READ);

	for (i=0; i<8; i++)		//8 means 8bit
	{
		bRecv <<= 1;
		I2C_P0_SetSCL(pdx, 0);
		DelayMicroSecond(1);		//un4given		//2010.11.16
		I2C_P0_SetSCL(pdx, 1);
		DelayMicroSecond(1);		//un4given		//2010.11.16
		bRecv |= I2C_P0_GetSDA(pdx);
	}
	KdPrint(("I2C_P0_ReceiveByte ... byRecv = %02X\n", bRecv));
	return bRecv;
}
//=================================================================================//
void I2C_P0_SendOutAck(PDEVICE_EXTENSION pdx, DWORD dwSig)
{
	I2C_P0_SetSCL(pdx, 0);

	I2C_P0_SetSDAMode(pdx, I2C_MODE_WRITE);
	I2C_P0_SetSDA(pdx, dwSig);

	I2C_P0_SetSCL(pdx, 1);
	I2C_P0_SetSCL(pdx, 0);
}

//////////////////////////////////////////////////////////////////////////
BOOL I2C_P1_ReadBurst(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength)
{
	DWORD i;
	
	
	I2C_P1_Start(pdx);
	I2C_P1_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Read error at port 1: write device address - 0\n"));
		return FALSE;
	}
	I2C_P1_TransmitByte(pdx, bSubAddr);
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Read error at port 1: write sub address\n"));
		return FALSE;
	}
	
	I2C_P1_Start(pdx);
	I2C_P1_TransmitByte(pdx, (BYTE)I2CR(bDevAddr));
	
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Read error at port 0: write device address -2 "));
		return FALSE;
	}
	
	for (i=0;i<dwLength;i++)
	{
		bData[i] = I2C_P1_ReceiveByte(pdx);
		if (i==(dwLength-1))
			I2C_P1_SendOutAck(pdx, 1);
		else
			I2C_P1_SendOutAck(pdx, 0);
		DelayMicroSecond(2);
	}
	I2C_P1_Stop(pdx);
	return TRUE;
}

BOOL I2C_P1_WriteBurst(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength)
{
	DWORD i;
	I2C_P1_Start(pdx);
	I2C_P1_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Write error at port 1: write device address\n"));
		return FALSE;
	}
	I2C_P1_TransmitByte(pdx, bSubAddr);
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Write error at port 1: write sub address\n"));
		return FALSE;
	}
	for (i=0;i<dwLength;i++)
	{
		I2C_P1_TransmitByte(pdx, bData[i]);
		I2C_P1_ReadAck(pdx);
	}
	I2C_P1_Stop(pdx);
	return TRUE;
}

//=================================================================================//
BOOL I2C_P1_Read(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData)
{
	BYTE	bRead;

	I2C_P1_Start(pdx);
	I2C_P1_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Read error at port 1: write device address - 0\n"));
		return FALSE;
	}
	I2C_P1_TransmitByte(pdx, bSubAddr);
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Read error at port 1: write sub address\n"));
		return FALSE;
	}

	I2C_P1_Start(pdx);
	I2C_P1_TransmitByte(pdx, (BYTE)I2CR(bDevAddr));
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Read error at port 1: write device address - 1\n"));
		return FALSE;
	}

	*bData = I2C_P1_ReceiveByte(pdx);
	I2C_P1_SendOutAck(pdx, 1);
	I2C_P1_Stop(pdx);
	return TRUE;
}
//=================================================================================//
BOOL I2C_P1_Write(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE bData)
{
	I2C_P1_Start(pdx);
	I2C_P1_TransmitByte(pdx, (BYTE)I2CW(bDevAddr));
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Write error at port 1: write device address\n"));
		return FALSE;
	}
	I2C_P1_TransmitByte(pdx, bSubAddr);
	if (!I2C_P1_ReadAck(pdx))
	{
		I2C_P1_Stop(pdx);
		KdPrint(("I2C Write error at port 1: write sub address\n"));
		return FALSE;
	}
	I2C_P1_TransmitByte(pdx, bData);
	I2C_P1_ReadAck(pdx);
	I2C_P1_Stop(pdx);
	return TRUE;
}
//=================================================================================//
void I2C_P1_Start(PDEVICE_EXTENSION pdx)
{
	I2C_P1_SetSDAMode(pdx, I2C_MODE_WRITE);
	I2C_P1_SetSDA(pdx, 1);
	I2C_P1_SetSCL(pdx, 1);
	I2C_P1_SetSDA(pdx, 0);
}
//=================================================================================//
void I2C_P1_Stop(PDEVICE_EXTENSION pdx)
{
	I2C_P1_SetSDAMode(pdx, I2C_MODE_WRITE);

	I2C_P1_SetSCL(pdx, 0);
	I2C_P1_SetSDA(pdx, 0);
	I2C_P1_SetSCL(pdx, 1);

	I2C_P1_SetSDA(pdx, 1);
	I2C_P1_SetSCL(pdx, 1);
}
//=================================================================================//
void I2C_P1_SetSDAMode(PDEVICE_EXTENSION pdx, DWORD dwMode)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P1));

	if (dwMode == I2C_MODE_READ)
		dwData = dwData & 0x1FFF;
	else
		dwData = dwData | 0x2000;

	WRITE_REGISTER_ULONG((DWORD*)(pdx->m_pbyMembase + REG_I2C_P1), dwData);
}
//=================================================================================//
void I2C_P1_SetSDA(PDEVICE_EXTENSION pdx, DWORD dwSig)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P1));

	if (dwSig)
		dwData = dwData | 0x200;
	else
		dwData = dwData & 0x3D3F;

	WRITE_REGISTER_ULONG((DWORD*)(pdx->m_pbyMembase + REG_I2C_P1), dwData);
}
//=================================================================================//
BYTE I2C_P1_GetSDA(PDEVICE_EXTENSION pdx)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P1));

	return (BYTE) ((dwData>>11) &0x1);
}
//=================================================================================//
void I2C_P1_SetSCL(PDEVICE_EXTENSION pdx, DWORD dwSig)
{
	DWORD		dwData;

	dwData = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase + REG_I2C_P1));

	if(dwSig)
		dwData = dwData |0x100;
	else
		dwData = dwData & 0x3E3F;

	WRITE_REGISTER_ULONG((DWORD*)(pdx->m_pbyMembase + REG_I2C_P1), dwData);
}
//=================================================================================//
void I2C_P1_TransmitByte(PDEVICE_EXTENSION pdx, BYTE bData)
{
	int		i, nCount;

	I2C_P1_SetSDAMode(pdx, I2C_MODE_WRITE);

	for (i=0, nCount = 7; i<8; i++, nCount--)
	{
		I2C_P1_SetSCL(pdx, 0);
		I2C_P1_SetSDA(pdx, (bData>>nCount)&1);
		I2C_P1_SetSCL(pdx, 1);
	}
}
//=================================================================================//
BOOL I2C_P1_ReadAck(PDEVICE_EXTENSION pdx)
{
	DWORD	dwPreTick, dwCurTick;
	BYTE	bAck = 1;

	I2C_P1_SetSCL(pdx, 0);
	I2C_P1_SetSDAMode(pdx, I2C_MODE_READ);

	dwPreTick = GetTick();
	do
	{
		bAck = I2C_P1_GetSDA(pdx);
		dwCurTick = GetTick();
	} while ((bAck != 0) && GetTimeInterval(dwPreTick, dwCurTick)<500);
	
	I2C_P1_SetSCL(pdx, 1);
	I2C_P1_SetSCL(pdx, 0);

	if (bAck)
	{
		I2C_P1_Stop(pdx);
		return FALSE;
	}
	return TRUE;
}
//=================================================================================//
BYTE I2C_P1_ReceiveByte(PDEVICE_EXTENSION pdx)
{
	BYTE	bRecv = 0;
	int		i;

	I2C_P1_SetSDAMode(pdx, I2C_MODE_READ);

	for(i=0; i<8; i++)		//8 means 8bit
	{
		bRecv <<= 1;
		I2C_P1_SetSCL(pdx, 0);
		I2C_P1_SetSCL(pdx, 1);
		bRecv |= I2C_P1_GetSDA(pdx);
	}
	return bRecv;
}
//=================================================================================//
void I2C_P1_SendOutAck(PDEVICE_EXTENSION pdx, DWORD dwSig)
{
	I2C_P1_SetSCL(pdx, 0);

	I2C_P1_SetSDAMode(pdx, I2C_MODE_WRITE);
	I2C_P1_SetSDA(pdx, dwSig);

	I2C_P1_SetSCL(pdx, 1);
	I2C_P1_SetSCL(pdx, 0);
}

