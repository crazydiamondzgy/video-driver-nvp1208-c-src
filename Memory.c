#include "NVP1208.h"
//////////////////////////////////////////////////////////////////////////
BOOL AllocDMABuffer(PDEVICE_EXTENSION pdx, LPST_DMA_MEMORY lpDMABuf, DWORD dwSize)
{
	PALLOCATE_COMMON_BUFFER pAllocateCommonBuffer;
	pAllocateCommonBuffer = pdx->m_pAdapterObject->DmaOperations->AllocateCommonBuffer;
	if (!lpDMABuf->pBuffer)
	{
		lpDMABuf->dwBufferSize = dwSize;
		lpDMABuf->pBuffer = (*pAllocateCommonBuffer)(pdx->m_pAdapterObject,
			lpDMABuf->dwBufferSize, &lpDMABuf->phyBuffer, FALSE);
		
		if (!lpDMABuf->pBuffer)
		{
			lpDMABuf->dwBufferSize = 0;
			return FALSE;
		}
		lpDMABuf->dwBuffer = lpDMABuf->phyBuffer.LowPart;		//32bit system
	}
	else 
	{
		KdPrint(("Already allocated\n"));
	}
	return TRUE;
}

VOID FreeDMABuffer(PDEVICE_EXTENSION pdx, LPST_DMA_MEMORY lpDMABuf)
{
	PFREE_COMMON_BUFFER pFreeCommonBuffer;
	pFreeCommonBuffer = pdx->m_pAdapterObject->DmaOperations->FreeCommonBuffer;
	
	if (lpDMABuf->pBuffer)
	{
		(*pFreeCommonBuffer)(pdx->m_pAdapterObject, 
			lpDMABuf->dwBufferSize, lpDMABuf->phyBuffer, lpDMABuf->pBuffer, FALSE);
		lpDMABuf->pBuffer = NULL;
		lpDMABuf->dwBufferSize = 0;
	}
}
//////////////////////////////////////////////////////////////////////////
NTSTATUS DmaMemAllocPool(PDEVICE_OBJECT pDeviceObject)
{
	NTSTATUS			status	= STATUS_SUCCESS;
	PDEVICE_EXTENSION	pdx		= (PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;
	BYTE byDecSection, byDecPath, byDMAChannel;

	if(pdx->m_bBufferAllocate == TRUE)
	{
		DmaMemFreePool(pDeviceObject);
	}

	//Allocate audio buffer
	if (!AllocDMABuffer(pdx, &pdx->stDMAAudioBuffer, MAX_AUDIO_DMA_BUFFER*PAGESIZE))
	{
		return STATUS_INVALID_DEVICE_REQUEST;
	}

	//Allocate video buffer for recording
	byDMAChannel = 0;
	for (byDecSection=0;byDecSection<pdx->m_byDecSection;byDecSection++)
	{
		for (byDecPath=0;byDecPath<MAX_DECODER;byDecPath++)
		{
			if (!AllocDMABuffer(pdx, &pdx->stDMAVideoBuffer[byDMAChannel], MAX_VIDEO_DMA_BUFFER*PAGESIZE))
				goto _FAIL_TO_ALLOC_VIDEO_BUF;
			byDMAChannel++;
		}
	}

	//Allocate video buffer for live
	if (!AllocDMABuffer(pdx, &pdx->stDMALiveBuffer, MAX_VIDEO_DMA_BUFFER*PAGESIZE))
		goto _FAIL_TO_ALLOC_VIDEO_BUF;

	return STATUS_SUCCESS;

_FAIL_TO_ALLOC_VIDEO_BUF:
	byDMAChannel = 0;
	for (byDecSection=0;byDecSection<pdx->m_byDecSection;byDecSection++)
	{
		for (byDecPath=0;byDecPath<MAX_DECODER;byDecPath++)
		{
			FreeDMABuffer(pdx, &pdx->stDMAVideoBuffer[byDMAChannel]);
			byDMAChannel++;
		}
	}

	FreeDMABuffer(pdx, &pdx->stDMAAudioBuffer);

	return STATUS_INVALID_DEVICE_REQUEST;

}

VOID DmaMemFreePool(PDEVICE_OBJECT pDeviceObject)
{
	PDEVICE_EXTENSION	pdx	= (PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;
	BYTE byDecSection, byDecPath, byDMAChannel;

	if(pdx->m_bBufferAllocate == TRUE)
	{
		byDMAChannel = 0;
		for (byDecSection=0;byDecSection<pdx->m_byDecSection;byDecSection++)
		{
			for (byDecPath=0;byDecPath<MAX_DECODER;byDecPath++)
			{
				FreeDMABuffer(pdx, &pdx->stDMAVideoBuffer[byDMAChannel]);
				byDMAChannel++;
			}
		}

		FreeDMABuffer(pdx, &pdx->stDMAAudioBuffer);
		FreeDMABuffer(pdx, &pdx->stDMALiveBuffer);
		pdx->m_bBufferAllocate = FALSE;

	}
}
//////////////////////////////////////////////////////////////////////////
BOOL MDLAllocDMABuf(PDEVICE_EXTENSION pdx, LPST_DMA_MEMORY lpDMABuf)
{
	if (lpDMABuf->pMDLBuffer)
	{
		KdPrint(("MDLAllocDMABuf -- already allocated\n"));
		return FALSE;
	}
	lpDMABuf->pMDLBuffer = IoAllocateMdl(lpDMABuf->pBuffer, lpDMABuf->dwBufferSize,	FALSE, FALSE, NULL);
	if (!lpDMABuf->pMDLBuffer)
		return FALSE;
	MmBuildMdlForNonPagedPool(lpDMABuf->pMDLBuffer);
#ifdef _WIN64
	lpDMABuf->pUserBuffer = (BYTE *)MmMapLockedPagesSpecifyCache(lpDMABuf->pMDLBuffer,
																 UserMode,
																 MmNonCached,
																 NULL,
																 FALSE, 
																 NormalPagePriority);
#else
	lpDMABuf->pUserBuffer = (BYTE *)MmMapLockedPages(lpDMABuf->pMDLBuffer, UserMode);
#endif
	if (!lpDMABuf->pUserBuffer)
	{
		IoFreeMdl(lpDMABuf->pMDLBuffer);
		lpDMABuf->pMDLBuffer = NULL;
		return FALSE;
	}
	return TRUE;
}

VOID MDLFreeDMABuf(PDEVICE_EXTENSION pdx, LPST_DMA_MEMORY lpDMABuf)
{
	if (lpDMABuf->pUserBuffer && lpDMABuf->pMDLBuffer)
	{
		MmUnmapLockedPages(lpDMABuf->pUserBuffer, lpDMABuf->pMDLBuffer);
		lpDMABuf->pUserBuffer = NULL;
	}
	if (lpDMABuf->pMDLBuffer)
	{
		IoFreeMdl(lpDMABuf->pMDLBuffer);
		lpDMABuf->pMDLBuffer = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
NTSTATUS MDLAllocateCapBuf(PDEVICE_OBJECT pDeviceObject)
{
	PDEVICE_EXTENSION	pdx		= (PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;
	BYTE				byDecSection, byDecPath, byDMAChannel;
	
	if(pdx->m_bMDLAllocate == TRUE)
		MDLFreeCapBuf(pDeviceObject);
	
	
	if (!MDLAllocDMABuf(pdx, &pdx->stDMAAudioBuffer))
		return STATUS_INVALID_DEVICE_REQUEST;
	
	byDMAChannel = 0;
	for (byDecSection=0;byDecSection<pdx->m_byDecSection;byDecSection++)
	{
		for (byDecPath=0;byDecPath<MAX_DECODER;byDecPath++)
		{
			if (!MDLAllocDMABuf(pdx, &pdx->stDMAVideoBuffer[byDMAChannel]))
				goto FAIL_TO_ALLOC_MDL;

			KdPrint(("pUserBuffer[%d] = %08X\n", byDMAChannel, pdx->stDMAVideoBuffer[byDMAChannel].pUserBuffer));
			byDMAChannel++;
		}
	}

	if (!MDLAllocDMABuf(pdx, &pdx->stDMALiveBuffer))
		goto FAIL_TO_ALLOC_MDL;
	pdx->m_bMDLAllocate = TRUE;
	return STATUS_SUCCESS;
		
FAIL_TO_ALLOC_MDL:
	MDLFreeDMABuf(pdx, &pdx->stDMAAudioBuffer);
	byDMAChannel = 0;
	for (byDecSection=0;byDecSection<pdx->m_byDecSection;byDecSection++)
	{
		for (byDecPath=0;byDecPath<MAX_DECODER;byDecPath++)
		{
			MDLFreeDMABuf(pdx, &pdx->stDMAVideoBuffer[byDMAChannel]);
			byDMAChannel++;
		}
	}
	return STATUS_INVALID_DEVICE_REQUEST;
		
}

VOID MDLFreeCapBuf(PDEVICE_OBJECT pDeviceObject)
{
	PDEVICE_EXTENSION	pdx = (PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;
	BYTE				byDecSection, byDecPath, byDMAChannel;
	
	if (pdx->m_bMDLAllocate)
	{
		MDLFreeDMABuf(pdx, &pdx->stDMAAudioBuffer);
		byDMAChannel = 0;
		for (byDecSection=0;byDecSection<pdx->m_byDecSection;byDecSection++)
		{
			for (byDecPath=0;byDecPath<MAX_DECODER;byDecPath++)
			{
				MDLFreeDMABuf(pdx, &pdx->stDMAVideoBuffer[byDMAChannel]);
				byDMAChannel++;
			}
		}
		MDLFreeDMABuf(pdx, &pdx->stDMALiveBuffer);
		pdx->m_bMDLAllocate = FALSE;
	}
	
}

//////////////////////////////////////////////////////////////////////////
NTSTATUS KKGetPhysicalAddress(IN unsigned char *pR0Address, OUT PHYSICAL_ADDRESS *PhysAddress)
{
#ifdef _WIN64
	return STATUS_NOT_SUPPORTED;
#else
	NTSTATUS	status = STATUS_SUCCESS;

	KdPrint(("KKGetPhysicalAddress \n"));
	*PhysAddress = MmGetPhysicalAddress(pR0Address);
	return status;
#endif
}


