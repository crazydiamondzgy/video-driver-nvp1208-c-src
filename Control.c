#include "NVP1208.h"

//////////////////////////////////////////////////////////////////////////
NTSTATUS DispatchControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	NTSTATUS			Status = STATUS_SUCCESS;
	PIO_STACK_LOCATION	stack;
	PDEVICE_EXTENSION	pdx;
	DWORD				dwRetSize = 0, dwInSize, dwOutSize, dwCode;

	HANDLE	hRing3Event = NULL;
	DWORD	*pdwSysBuf;
	BYTE	*pbySysBuf;
	BYTE	byDMAChannel, byInternalCH;
	BYTE	byData;
	BYTE	byDMAPath;

	pdx		= (PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;
	Status	= IoAcquireRemoveLock(&pdx->m_RemoveLock, pIrp);
	
	if (!NT_SUCCESS(Status))
		return CompleteRequest(pIrp, Status, 0);	
	
	stack		= IoGetCurrentIrpStackLocation(pIrp);
	dwInSize	= stack->Parameters.DeviceIoControl.InputBufferLength;
	dwOutSize	= stack->Parameters.DeviceIoControl.OutputBufferLength;
	dwCode		= stack->Parameters.DeviceIoControl.IoControlCode;
	
	switch(dwCode)
	{
	case IOCTL_GET_INIT_INFO:
		((BYTE *)pIrp->AssociatedIrp.SystemBuffer)[0] = pdx->m_byDecSection;
		dwRetSize = sizeof(BYTE);
		break;

	case IOCTL_GET_ALL_VIDEO_SYNC:
		if (dwOutSize<sizeof(BYTE)*MAX_DMA_PATH_VCAP)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		CheckVideoSync(pdx);
		RtlCopyMemory(pIrp->AssociatedIrp.SystemBuffer, pdx->bVideoSync, sizeof(BYTE)*MAX_DMA_PATH_VCAP);
		dwRetSize = sizeof(BYTE)*MAX_DMA_PATH_VCAP;
		break;

	case IOCTL_SET_VIDEO_EVENT:
		if (dwInSize<sizeof(DWORD))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		hRing3Event = (HANDLE) pdwSysBuf[0];
		if (!hRing3Event)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		Status = SynchronizeEvent(pIrp, hRing3Event, &pdx->m_pVCapEvent);
		break;

	case IOCTL_SET_AUDIO_EVENT:
		if (dwInSize<sizeof(DWORD))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		hRing3Event = (HANDLE) pdwSysBuf[0];
		if (!hRing3Event)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		Status = SynchronizeEvent(pIrp, hRing3Event, &pdx->m_pAudioEvent);
		break;
	case IOCTL_SET_VLIVE_EVENT:
		if (dwInSize<sizeof(DWORD))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		hRing3Event = (HANDLE) pdwSysBuf[0];
		if (!hRing3Event)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		Status = SynchronizeEvent(pIrp, hRing3Event, &pdx->m_pVLiveEvent);
		break;

	case IOCTL_DEREFERENCE_EVENT:
		if (!pdx->m_pVCapEvent)			DereferenceEvent(pdx->m_pVCapEvent);
		if (!pdx->m_pVLiveEvent)		DereferenceEvent(pdx->m_pVLiveEvent);
		if (!pdx->m_pAudioEvent)		DereferenceEvent(pdx->m_pAudioEvent);
		pdx->m_pVCapEvent = NULL;
		pdx->m_pVLiveEvent = NULL;
		pdx->m_pAudioEvent = NULL;
		break;

	case IOCTL_VIDEO_BUFFER_MAPPING:
		if (dwOutSize<(sizeof(DWORD)*pdx->m_byDecSection*MAX_DECODER))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		{
			pdwSysBuf[byDMAChannel] = (DWORD)pdx->stDMAVideoBuffer[byDMAChannel].pUserBuffer;
		}
		dwRetSize = sizeof(DWORD)*byDMAChannel;
		break;

	case IOCTL_AUDIO_BUFFER_MAPPING:
		if (dwOutSize<sizeof(DWORD))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		pdwSysBuf[0] = (DWORD)pdx->stDMAAudioBuffer.pUserBuffer;
		dwRetSize = sizeof(DWORD);
		break;
	case IOCTL_VLIVE_BUFFER_MAPPING:
		if (dwOutSize<sizeof(DWORD))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		pdwSysBuf[0] = (DWORD)pdx->stDMALiveBuffer.pUserBuffer;
		dwRetSize = sizeof(DWORD);
		break;

	case IOCTL_DEVICE_ALL_RESET:
		Chip_Reset(pdx, 0x00000001);
		Decoder_Init(pdx);
		pdx->m_byDecSection = GetDecoderSection(pdx);
		
		External_Decoder_Init(pdx);

		Initialize(pdx);
		break;
	case IOCTL_VIDEO_INIT_PARAMETER:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
		if (pdx->m_byVideoFormat != pbySysBuf[0])
		{
			pdx->m_byVideoFormat = pbySysBuf[0];
			Decoder_Init(pdx);
			External_Decoder_Init(pdx);
		}

		pdx->m_dwHeightIndication = GetHeightIndication(pdx->m_byVideoFormat);
		//////////////////////////////////////////////////////////////////////////
		//set data format
		pdx->m_byVideoOutputFormat = pbySysBuf[1];
		//////////////////////////////////////////////////////////////////////////
		break;

	case IOCTL_SET_VCAP_PROPERTY:
		if (dwInSize<sizeof(DWORD)*3)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;

		byDMAPath = (BYTE)pdwSysBuf[0];
		pdx->m_stVCapSize[byDMAPath].Width = pdwSysBuf[1];
		pdx->m_stVCapSize[byDMAPath].Height = pdwSysBuf[2];


		break;
	case IOCTL_SET_VLIVE_PROPERTY:
		if (dwInSize<sizeof(DWORD)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
		pdx->m_stVLiveSize.Width = pdwSysBuf[0];
		pdx->m_stVLiveSize.Height = pdwSysBuf[1];
		break;

	case IOCTL_SET_VLIVE_CHANNEL:
		if (dwInSize<sizeof(BYTE))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;

		if (pdx->m_byVLiveChannel != pbySysBuf[0])
		{
			pdx->m_byVLiveChannel = pbySysBuf[0];
			SetVideoLiveChannel(pdx, pbySysBuf[0]);
		}
		break;

	case IOCTL_AUDIO_INIT_PARAMETER:
		//get frequency & sampling bit rate
		//but this application capture only 16bit data
		if (dwInSize<sizeof(BYTE))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
		if (pdx->m_bySampleFrequency != pbySysBuf[0])
		{
			//set frequency
			pdx->m_bySampleFrequency = pbySysBuf[0];
		}

		break;

	case IOCTL_SET_VIDEO_DMA:
		if (dwInSize == sizeof(BYTE)*2)
		{
			//pbySysBuf[0]: VCap DMA channel
			//pbySysBuf[1]: Enable(DMA_ON=1) or disable(DMA_OFF=0) status
			pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			byDMAChannel = pbySysBuf[0];
			if (byDMAChannel<(pdx->m_byDecSection*MAX_DECODER))
			{
				pdx->m_byVideoDMA[byDMAChannel] = pbySysBuf[1];
			}
			else if (byDMAChannel<MAX_DMA_PATH_VCAP)
			{
				pdx->m_byVideoDMA[byDMAChannel] = DMA_OFF;
			}
			else
			{
				KdPrint(("IOCTL_SET_VIDEO_DMA, invalide DMA channel is tried to be set\n"));
			}
		}
		else if (dwInSize == sizeof(DWORD))
		{
			//each bit represents video DMA is enable or disable
			pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
			for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
			{
				pdx->m_byVideoDMA[byDMAChannel] = (BYTE)((pdwSysBuf[0]>>byDMAChannel)&DMA_ON);
			}
			for (;byDMAChannel<MAX_DMA_PATH_VCAP;byDMAChannel++)
			{
				pdx->m_byVideoDMA[byDMAChannel] = DMA_OFF;
			}
		}
		else if (dwInSize == sizeof(BYTE)*MAX_DMA_PATH_VCAP)
		{
			//each BYTE represents video DMA is enable or disable
			pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
			{
				pdx->m_byVideoDMA[byDMAChannel] = pbySysBuf[byDMAChannel];
			}
			for (;byDMAChannel<MAX_DMA_PATH_VCAP;byDMAChannel++)
			{
				pdx->m_byVideoDMA[byDMAChannel] = DMA_OFF;
			}
			
		}
		else
		{
			Status = STATUS_INVALID_PARAMETER;
		}
		break;
	case IOCTL_SET_AUDIO_DMA:
		if (dwInSize<sizeof(BYTE))
			Status = STATUS_INVALID_PARAMETER;
		else
		{
			pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			pdx->m_byAudioDMA = pbySysBuf[0];
		}
		break;
	case IOCTL_SET_VLIVE_DMA:
		if (dwInSize<sizeof(BYTE))
			Status = STATUS_INVALID_PARAMETER;
		else
		{
			pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			pdx->m_byLiveDMA = pbySysBuf[0];
		}
		break;

	case IOCTL_BURST_SIZE:
		if (dwInSize<sizeof(DWORD))
			Status = STATUS_INVALID_PARAMETER;
		else
		{
			pdwSysBuf = (DWORD *)pIrp->AssociatedIrp.SystemBuffer;
			pdx->m_dwBurstSize = pdwSysBuf[0];
		}
		break;

	case IOCTL_CAPTURE_START:
		if (dwInSize<sizeof(BYTE))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		pdx->byStartFlag = ((BYTE *)pIrp->AssociatedIrp.SystemBuffer)[0];
		if (pdx->byStartFlag)
			SetCaptureStart(pdx);
		break;

	case IOCTL_CAPTURE_STOP:
		if (dwInSize<sizeof(BYTE))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		
		pdx->byStopFlag = ((BYTE *)pIrp->AssociatedIrp.SystemBuffer)[0];
		if (pdx->byStopFlag)
			SetCaptureStop(pdx);
		break;

	case IOCTL_SET_CHANNEL_TABLE:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE byDMAChannel, byTableLen;
			pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			byDMAChannel = pbySysBuf[0];
			byTableLen = pbySysBuf[1];

			if (dwInSize<sizeof(BYTE)*(2+byTableLen))
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}
			else if (byTableLen>MAX_CHANNEL_CHANGEABLE)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}

			pdx->m_byRecTableLen[byDMAChannel] = byTableLen;
			memcpy(pdx->m_byRecTable[byDMAChannel], &pbySysBuf[2], byTableLen);

		}
		break;

		//////////////////////////////////////////////////////////////////////////

	case IOCTL_SET_BRIGHTNESS:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE *pbyBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			byDMAChannel = pbyBuf[0];
			pdx->Brightness[byDMAChannel] = pbyBuf[1];
			SetBrightness(pdx, byDMAChannel);

		}
		break;
	
	case IOCTL_SET_CONTRAST:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE *pbyBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			byDMAChannel = pbyBuf[0];
			pdx->Contrast[byDMAChannel] = pbyBuf[1];
			SetContrast(pdx, byDMAChannel);

		}
		break;
	
	case IOCTL_SET_SATURATION:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE *pbyBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			byDMAChannel = pbyBuf[0];
			pdx->Saturation[byDMAChannel] = pbyBuf[1];
			SetSaturation(pdx, byDMAChannel);
		}
		break;
	
	case IOCTL_SET_HUE:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE *pbyBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			byDMAChannel = pbyBuf[0];
			pdx->Hue[byDMAChannel] = pbyBuf[1];
			SetHue(pdx, byDMAChannel);
		}
		break;
		
	case IOCTL_SET_VGAIN:
		//not used
		break;

	case IOCTL_SET_BLACK_N_WHITE:
		break;

	case IOCTL_SET_SHARPNESS:
		if (dwInSize<sizeof(BYTE)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
		SetSharpnessIn(pdx, pbySysBuf[0], pbySysBuf[1]);
		SetSharpnessEx(pdx, pbySysBuf[0], pbySysBuf[1]);
		break;
		//////////////////////////////////////////////////////////////////////////

	case IOCTL_SET_AUDIO_GAIN:
		if (dwInSize<sizeof(BYTE)*3)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		pbySysBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;

		if (pbySysBuf[0]<INTERNAL_AUDIO_COUNT)
			SetAudioGainIn(pdx, pbySysBuf[0], pbySysBuf[1], pbySysBuf[2]);
		else
			SetAudioGainEx(pdx, pbySysBuf[0], pbySysBuf[1], pbySysBuf[2]);

		break;
		

	case IOCTL_I2C_WRITE_SINGLE:
		if (dwInSize<sizeof(BYTE)*4)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE *pbyBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			I2C_Write(pdx, pbyBuf[0], pbyBuf[1], pbyBuf[2], pbyBuf[3]);
		}
		break;

	case IOCTL_I2C_READ_SINGLE:
		if ((dwInSize<sizeof(BYTE)*3)||(dwOutSize<sizeof(BYTE)))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			BYTE *pbyBuf = (BYTE *)pIrp->AssociatedIrp.SystemBuffer;
			if (!I2C_Read(pdx, pbyBuf[0], pbyBuf[1], pbyBuf[2], &byData))
			{
				Status = STATUS_INVALID_DEVICE_REQUEST;
				break;
			}
			pbyBuf[0] = byData;
			dwRetSize = sizeof(BYTE);
		}
		break;

	case IOCTL_I2C_WRITE_BURST:
		if (dwInSize<sizeof(ST_I2C_BURST))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			LPST_I2C_BURST lpI2C = (LPST_I2C_BURST)pIrp->AssociatedIrp.SystemBuffer;
			I2C_WriteBurst(pdx, lpI2C->byPort, lpI2C->byDevAddr, lpI2C->bySubAddr, 
				lpI2C->byData, lpI2C->dwLength);
		}
		break;

	case IOCTL_I2C_READ_BURST:
		if ((dwInSize<sizeof(ST_I2C_BURST))&&(dwOutSize<sizeof(ST_I2C_BURST)))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			LPST_I2C_BURST lpI2C = (LPST_I2C_BURST)pIrp->AssociatedIrp.SystemBuffer;
			I2C_ReadBurst(pdx, lpI2C->byPort, lpI2C->byDevAddr, lpI2C->bySubAddr,
				lpI2C->byData, lpI2C->dwLength);
			dwRetSize = sizeof(ST_I2C_BURST);
		}

		break;
		//////////////////////////////////////////////////////////////////////////
		//Read and Write local register
	case IOCTL_SET_LOCAL_REGISTER:
		if (dwInSize<sizeof(DWORD)*2)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			DWORD dwLocalOffset, dwLocalData;
			dwLocalOffset	= ((DWORD *)pIrp->AssociatedIrp.SystemBuffer)[0];		
			dwLocalData		= ((DWORD *)pIrp->AssociatedIrp.SystemBuffer)[1];
			
			dwWriteRegister(pdx, dwLocalOffset, dwLocalData);
		}
		break;
		
	case IOCTL_GET_LOCAL_REGISTER:
		if ((dwInSize<sizeof(DWORD))||(dwOutSize<sizeof(DWORD)))
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		else
		{
			DWORD dwLocalOffset, dwLocalData;
			dwLocalOffset	= ((DWORD *)pIrp->AssociatedIrp.SystemBuffer)[0];
			dwLocalData		= dwReadRegister(pdx, dwLocalOffset);
			((DWORD*)pIrp->AssociatedIrp.SystemBuffer)[0] = dwLocalData;
			dwRetSize = sizeof(DWORD);
		}
		break;

	default:
		Status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	IoReleaseRemoveLock(&pdx->m_RemoveLock, pIrp);
	return CompleteRequest(pIrp, Status, dwRetSize);
}

//////////////////////////////////////////////////////////////////////////
NTSTATUS SynchronizeEvent(PIRP pIrp, HANDLE hRing3Event, PKEVENT *ppkRing0Event)
{
	NTSTATUS Status;
	Status = ObReferenceObjectByHandle(hRing3Event, SYNCHRONIZE, *ExEventObjectType, 
									   pIrp->RequestorMode, (PVOID *)(ppkRing0Event), NULL);
	if (!NT_SUCCESS(Status))
	{
		KdPrint(("ObreferenceObjectByHandle fail - %08X\n", hRing3Event));
	}
	return Status;

}
VOID DereferenceEvent(PKEVENT pKEvent)
{
	ObDereferenceObject((PVOID) pKEvent);
}
