#include "NVP1208.h"

#define SHOW_CHANNEL_BIT(x,y)		((x>>y)&1)

BOOL OnInterrupt(PKINTERRUPT pInterruptObject, PDEVICE_EXTENSION pdx)
{
	static DWORD dwIntStatus, dwI2CInt;
	static BOOL bResult;

	//////////////////////////////////////////////////////////////////////////
	KeQuerySystemTime(&pdx->liSysFileTime);
	ExSystemTimeToLocalTime(&pdx->liSysFileTime, &pdx->liLocalFileTime);
	pdx->dwTick = GetTick();		//for timing check
	//////////////////////////////////////////////////////////////////////////

	bResult = FALSE;

	dwIntStatus = dwReadRegister(pdx, REG_IRQ_STATUS_AUDIO);
	if (dwIntStatus&AD_IRQ0)
		if (OnInterrupt_Audio(pdx, dwIntStatus))
			bResult = TRUE;

	if (dwIntStatus&AD_IRQ1)
	{
		KdPrint(("This audio path is not used\n"));
		dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_AUDIO, AD_IRQ1);
		bResult = TRUE;
	}

	dwIntStatus = dwReadRegister(pdx, REG_IRQ_STATUS_VIDEO);
	if (dwIntStatus&0xFFFF)
		if (OnInterrupt_VCap(pdx, dwIntStatus&0xFFFF))
			bResult = TRUE;

	if (dwIntStatus&(1<<LIVE_VDEC_PATH_START))
		if (OnInterrupt_Live(pdx))
			bResult = TRUE;

	return bResult;
}

BOOL OnInterrupt_Audio(PDEVICE_EXTENSION pdx, DWORD dwInterruptStatus)
{
	BYTE byCurrQueue, byNextQueue;
	BOOL fRet = FALSE;

	byCurrQueue = pdx->byAudioQueueCount;
	if (pdx->m_pACAPStatus[byCurrQueue]->byLock == MEM_UNLOCK)
	{
		byNextQueue = byCurrQueue+2;
		byNextQueue %= MAX_AUDIO_QUEUE;
		fRet = TRUE;

		if (dwInterruptStatus&AD_IRQ0_A)
			SetAudioDMATargetAddress(pdx, AD_PATH_A, byNextQueue);
		else
			SetAudioDMATargetAddress(pdx, AD_PATH_B, byNextQueue);

		dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_AUDIO, dwInterruptStatus);

		if (fRet)
		{
			pdx->m_pACAPStatus[byCurrQueue]->byLock = MEM_LOCK;
			*pdx->m_pACapCurQCnt = byCurrQueue;

			pdx->byAudioQueueCount = (byCurrQueue+1)%MAX_AUDIO_QUEUE;
			KeInsertQueueDpc(&pdx->m_DpcObjectAudio, NULL, pdx);
		}
	}
	else
	{
		dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_AUDIO, AD_IRQ0);
		fRet = TRUE;
	}

	return fRet;
}

BOOL OnInterrupt_VCap(PDEVICE_EXTENSION pdx, DWORD dwInterruptStatus)
{
	BYTE byCurrQCnt, byNextQCnt;
	DWORD dwVLoss, dwField;
	PVCAP_STATUS_INFO pVCapStatus;
	DWORD dwMux = 0;
	BYTE byCurIntChannel;		//current internal channel for NRT
	BYTE byNextIntChannel;		//next internal channel
	DWORD dwStatus;
	DWORD dwDMACommand;
	DWORD dwOddEn, dwEvenEn;
	DWORD dwCurChBit;
	LPST_IMAGE_SIZE lpstImageSize;

	BOOL bRet = FALSE;
	BOOL DoNotNotify, SetNext;
	BYTE byPos;

	BYTE byDMAPath;

	dwStatus =		dwReadRegister(pdx, REG_NO_DMA_STATUS);
	dwVLoss =		dwReadRegister(pdx, REG_VLOSS_STATUS);
	dwField =		dwReadRegister(pdx, REG_VIDEO_FIELD_STAT_EVEN);
	dwDMACommand =	dwReadRegister(pdx, REG_NO_DMA_COMMAND);

	for (byDMAPath=0;byDMAPath<pdx->m_byDecSection*MAX_DECODER;byDMAPath++)
	{
		dwCurChBit = 1<<byDMAPath;
		if (!(dwInterruptStatus&pdx->m_dwVideoIrqMask[byDMAPath]))
		{

			if (dwInterruptStatus&dwCurChBit)
			{
				dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_VIDEO, dwCurChBit);
				bRet = TRUE;
				KdPrint(("an unused interrupt occurs %08X\n"));
			}
			continue;
		}

		bRet = TRUE;


		VideoDMAStop(pdx, byDMAPath);

		byPos = pdx->byCurTablePos[byDMAPath];
		byCurIntChannel = pdx->m_byRecTable[byDMAPath][byPos];
		DoNotNotify = FALSE;

		if (byCurIntChannel == INVALID_CHANNEL_NUMBER)
		{
			//////////////////////////////////////////////////////////////////////////
			if (dwField&dwCurChBit)
				SetNext = TRUE;
			else
				SetNext = FALSE;
			//////////////////////////////////////////////////////////////////////////
			DoNotNotify = TRUE;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// DMA has not been executed at this moment
			// do not have to notify this capture event
			if (!(dwVLoss&dwCurChBit))
			{
				if (dwDMACommand&dwCurChBit)
					DoNotNotify = TRUE;
			}
			//////////////////////////////////////////////////////////////////////////

			lpstImageSize = &pdx->m_stVCapSize[byDMAPath];
			if (dwField&dwCurChBit)
				SetNext = TRUE;
			else
				SetNext = FALSE;

		}

		if (!DoNotNotify)
		{
			byCurrQCnt = pdx->byVCapQueueCount[byDMAPath];
			pVCapStatus = pdx->m_pVCAPStatus[byDMAPath][byCurrQCnt];

			if (pVCapStatus->byLock == MEM_UNLOCK)
			{
				//////////////////////////////////////////////////////////////////////////
				//leave the capture result
				pVCapStatus->byLock = MEM_LOCK;
				pVCapStatus->dwWidth = lpstImageSize->Width;
				pVCapStatus->dwHeight = lpstImageSize->Height;
				pVCapStatus->byField = (BYTE)(dwField>>byDMAPath)&1;
				pVCapStatus->VLoss = (BYTE)(dwVLoss>>byDMAPath)&1;
				pVCapStatus->byChannel = byDMAPath;
				pVCapStatus->bFrame = lpstImageSize->bFrame;
				pVCapStatus->VCAPTime.dwLowDateTime = pdx->liLocalFileTime.LowPart;
				pVCapStatus->VCAPTime.dwHighDateTime = pdx->liLocalFileTime.HighPart;
				pVCapStatus->dwTick = pdx->dwTick;
				*pdx->m_pVCapCurQCnt[byDMAPath] = byCurrQCnt;
				//////////////////////////////////////////////////////////////////////////

				byNextQCnt = (byCurrQCnt+1)%MAX_VIDEO_QUEUE;
				SetVideoDMATargetAddress(pdx, byDMAPath, byNextQCnt);
				pdx->byVCapQueueCount[byDMAPath] = byNextQCnt;

				KeInsertQueueDpc(&pdx->m_DpcObjectVCap, NULL, pdx);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//Set Next Channel
		if (SetNext)
		{
			byPos = (byPos+1)%pdx->m_byRecTableLen[byDMAPath];
			byNextIntChannel = pdx->m_byRecTable[byDMAPath][byPos];
			//////////////////////////////////////////////////////////////////////////
			if (byNextIntChannel==INVALID_CHANNEL_NUMBER)
			{
				dwDMACommand |= dwCurChBit;
			}
			else
			{
				if (dwVLoss&dwCurChBit)
					dwDMACommand |= dwCurChBit;
				else
					dwDMACommand &= ~dwCurChBit;
			}
			dwWriteRegister2(pdx, REG_NO_DMA_COMMAND, dwDMACommand);

			pdx->byCurTablePos[byDMAPath] = byPos;
		}
		//////////////////////////////////////////////////////////////////////////


		dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_VIDEO, dwCurChBit);
		VideoDMAStart(pdx, byDMAPath);
	}

	return bRet;
}

BOOL OnInterrupt_Live(PDEVICE_EXTENSION pdx)
{
	BYTE byDMAChannel;
	BYTE byCurrQCnt, byNextQCnt;
	DWORD dwVLoss, dwField;
	PVCAP_STATUS_INFO pVCapStatus;
	LPST_IMAGE_SIZE lpstImageSize;
	BYTE byLiveChannel = LIVE_VDEC_PATH_START;
	
	BOOL bRet = FALSE;
	
	dwVLoss = dwReadRegister(pdx, REG_VLOSS_STATUS);
	dwField = dwReadRegister(pdx, REG_VIDEO_FIELD_STAT_EVEN);

	if (!pdx->m_dwVideoIrqMask[byLiveChannel])
	{
		dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_VIDEO, (1<<byLiveChannel));
		return TRUE;
	}

	VideoDMAStop(pdx, (BYTE)byLiveChannel);
	byCurrQCnt = pdx->byVLiveQueueCount;
	pVCapStatus = pdx->m_pVLiveStatus[byCurrQCnt];
	lpstImageSize = &pdx->m_stVLiveSize;

	if (pVCapStatus->byLock == MEM_UNLOCK)
	{
		//////////////////////////////////////////////////////////////////////////
		//leave the capture result
		pVCapStatus->byLock = MEM_LOCK;
		pVCapStatus->dwWidth = lpstImageSize->Width;
		pVCapStatus->dwHeight = lpstImageSize->Height;
		pVCapStatus->byField = (BYTE)(dwField>>byLiveChannel)&1;
		pVCapStatus->VLoss = (BYTE)(dwVLoss>>byLiveChannel)&1;
		pVCapStatus->byChannel = pdx->m_byVLiveChannel;	//selected live channel
		*pdx->m_pVLiveCurQCnt = byCurrQCnt;
		//////////////////////////////////////////////////////////////////////////
		
		byNextQCnt = (byCurrQCnt+1)%MAX_VIDEO_QUEUE;
		SetVLiveDMATargetAddress(pdx, byNextQCnt);
		pdx->byVLiveQueueCount = byNextQCnt;
		
		KeInsertQueueDpc(&pdx->m_DpcObjectVLive, NULL, pdx);
	}
		
	dwWriteRegisterIrq(pdx, REG_IRQ_STATUS_VIDEO, 1<<byLiveChannel);
	VideoDMAStart(pdx, byLiveChannel);
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
VOID DpcForIsr_VCap(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx)
{
	if (pdx->m_pVCapEvent)
		KeSetEvent(pdx->m_pVCapEvent, 0, FALSE);

}

//////////////////////////////////////////////////////////////////////////
VOID DpcForIsr_Audio(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx)
{
	if (pdx->m_pAudioEvent)
		KeSetEvent(pdx->m_pAudioEvent, 0, FALSE);
}

//////////////////////////////////////////////////////////////////////////
VOID DpcForIsr_VLive(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx)
{
	if (pdx->m_pVLiveEvent)
		KeSetEvent(pdx->m_pVLiveEvent, 0, FALSE);
}

VOID DpcForIsr_I2C(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx)
{
	KeSetEvent(&pdx->m_kI2CEvent, EVENT_INCREMENT, FALSE);
}

//////////////////////////////////////////////////////////////////////////
VOID VideoDMAStart(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	pdx->m_dwVideoDMAEnable |= pdx->m_dwVideoDMAMask[byDMAChannel];
	pdx->m_dwVideoIrqEnable |= pdx->m_dwVideoIrqMask[byDMAChannel];

	dwWriteRegister2(pdx, REG_DMA_ENABLE_VIDEO, pdx->m_dwVideoDMAEnable);
	dwWriteRegister2(pdx, REG_IRQ_ENABLE_VIDEO, pdx->m_dwVideoIrqEnable);
}

VOID VideoDMAStop(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	pdx->m_dwVideoDMAEnable &= ~pdx->m_dwVideoDMAMask[byDMAChannel];
	pdx->m_dwVideoIrqEnable &= ~pdx->m_dwVideoIrqMask[byDMAChannel];
	
	dwWriteRegister2(pdx, REG_DMA_ENABLE_VIDEO, pdx->m_dwVideoDMAEnable);
	dwWriteRegister2(pdx, REG_IRQ_ENABLE_VIDEO, pdx->m_dwVideoIrqEnable);
}
