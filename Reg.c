#include "NVP1208.h"

DWORD g_dwRegMask[] = 
{
	/*0x000*/		0x0F00000F,		0x0F0000F0,		0x55000F00,		0x55000000,
	/*0x010*/		0x0F000000,		0x00000000,		0xFF000000,		0xFFFF0F0F,
	/*0x020*/		0xFFFF7717,		0xFFFFFFFF,		0xFFFFFFFF,		0x1777FF00,
	/*0x030*/		0x00FFFF93,		0x00DF0000,		0x00000000,		0x00000000,
	/*0x040*/		0xFF000000,		0x0000C08F,		0x00000000,		0x00000000,
	/*0x050*/		0x00000000,		0x00000000,		0x000F0F00,		0x0000FFFF,
	/*0x060*/		0x00FF3F00,		0xF3000000,		0xFFC34FB0,		0x00CF0000,
	/*0x070*/		0xFFFF01FF,		0xFFFFFFFF,		0xBFFFFFFF,		0xFF3FFFFF,
	/*0x080*/		0xFFFFFFFF,		0x00BFFFFF,		0xFF000000,		0xFFFFFFFF,
	/*0x090*/		0xFFFFFFFF,		0x00000000,		0xF0FFFFFF,		0xFF0F0DFF,
	/*0x0A0*/		0xFF000000,		0xFF1F8FFF,		0x00000000,		0xFFFFFFFF,
	/*0x0B0*/		0x8DFFFFFF,		0xBFFFFFFF,		0xFFFFF3FF,		0xFFFFFFFF,
	/*0x0C0*/		0xFFFFFFFF,		0x7FFFFFFF,		0xFFFFFFFF,		0xFFFF777F,
	/*0x0D0*/		0xFFFFFFF7,		0xFFFFFFF7,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x0E0*/		0xFF73FFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0x000000FF,
	/*0x0F0*/		0x7FFF7F7F,		0x00000000,		0x00000000,		0x1F000000,

	/*0x100*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x110*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x120*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x130*/		0xFFFFFFFF,		0xFFFFFFFF,		0x00000FFF,		0xFFFFFFFF,
	/*0x140*/		0xFFFFFFFF,		0x00000FFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x150*/		0x00000FFF,		0x000017FF,		0x00000000,		0x00000000,
	/*0x160*/		0x1F1F001F,		0x001FFFFF,		0xFD1FFFFF,		0x001FFFFF,
	/*0x170*/		0xFD1FFFFF,		0x001FFFFF,		0xFD1FFFFF,		0x001FFFFF,
	/*0x180*/		0xFD1FFFFF,		0x001FFFFF,		0xFD1FFFFF,		0x00000000,
	/*0x190*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x1A0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x1B0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x1C0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x1D0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x1E0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x1F0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,

	/*0x200*/		0xFFFFFF00,		0xFFFFFFFF,		0x1F1F1F1F,		0xFFFFFFFF,
	/*0x210*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x220*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x230*/		0x1F1F1F1F,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x240*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x250*/		0xFFFFFFFF,		0xFFFFFFFF,		0x0000000F,		0xFFFFFFFF,
	/*0x260*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x270*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x280*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x290*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x2A0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x2B0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x2C0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x2D0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x2E0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x2F0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,

	/*0x300*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x310*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x320*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x330*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x340*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x350*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x360*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x370*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x380*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x390*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x3A0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x3B0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x3C0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x3D0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x3E0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x3F0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,

	/*0x400*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x410*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x420*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x430*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x440*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x450*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x460*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x470*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x480*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x490*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x4A0*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x4B0*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x4C0*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x4D0*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x4E0*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x4F0*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,

	/*0x500*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x510*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x520*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x530*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x540*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x550*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x560*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x570*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x580*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x590*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x5A0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x5B0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x5C0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x5D0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x5E0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x5F0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,

	/*0x600*/		0x0001FFFF,		0x0001FFFF,		0x00000000,		0x00000000,
	/*0x610*/		0x00000000,		0x0001FFFF,		0x0001FFFF,		0x0001FFFF,
	/*0x620*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x630*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x640*/		0x0000FFFF,		0x00000000,		0x00000000,		0x00000000,
	/*0x650*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x660*/		0x00000003,		0x00000000,		0x00000000,		0x00000000,
	/*0x670*/		0x0001FFFF,		0x0001FFFF,		0x000000FF,		0x00000000,
	/*0x680*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x690*/		0x00000000,		0x00000000,		0x000000FF,		0x00000000,
	/*0x6A0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x6B0*/		0x00000000,		0x00000000,		0xFFFFFFFF,		0x00000000,
	/*0x6C0*/		0x0001FFFF,		0x0001FFFF,		0x00000000,		0x00000000,
	/*0x6D0*/		0x00000000,		0x00000000,		0x00000000,		0x0000FFFF,
	/*0x6E0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x6F0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,

	/*0x700*/		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,
	/*0x710*/		0xFFFFFFFF,		0xFFFFFFFF,		0x0000FFFF,		0x00000000,
	/*0x720*/		0x00000001,		0x0000000F,		0x0000000F,		0x00000001,
	/*0x730*/		0x00FFFFFF,		0x00FFFFFF,		0x00FFFFFF,		0x00000001,
	/*0x740*/		0x00000001,		0x00000007,		0x00000000,		0x00000000,
	/*0x750*/		0x0000003F,		0x0000003F,		0x0000003F,		0x00000000,
	/*0x760*/		0x0001FFFF,		0x0001FFFF,		0x00000000,		0x00000000,
	/*0x770*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x780*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x790*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x7A0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x7B0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x7C0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x7D0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x7E0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
	/*0x7F0*/		0x00000000,		0x00000000,		0x00000000,		0x00000000,
};

//////////////////////////////////////////////////////////////////////////
VOID dwWriteRegister(PDEVICE_EXTENSION pdx, DWORD dwOffset, DWORD dwValue)
{
	DWORD dwReg;
	WRITE_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset), dwValue);
	dwValue = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset));
	//DelayMicroSecond(1);
	
}

VOID dwWriteRegister2(PDEVICE_EXTENSION pdx, DWORD dwOffset, DWORD dwValue)
{
	DWORD dwReg;
	WRITE_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset), dwValue);
	dwReg = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset));
	if (dwReg != dwValue)
	{
		WRITE_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset), dwValue);
		dwReg = READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset));
	}
	//KdPrint(("dwWriteRegister2 - %08X, %08X\n", dwReg, dwValue));
	
}

VOID dwWriteRegisterIrq(PDEVICE_EXTENSION pdx, DWORD dwOffset, DWORD dwValue)
{
	WRITE_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset), dwValue);
}


DWORD dwReadRegister(PDEVICE_EXTENSION pdx, DWORD dwOffset)
{
	return READ_REGISTER_ULONG((DWORD *)(pdx->m_pbyMembase+dwOffset));
}

//////////////////////////////////////////////////////////////////////////
VOID Initialize(PDEVICE_EXTENSION pdx)
{
	//////////////////////////////////////////////////////////////////////////
	//Local Variables
	BYTE byDecSection, byDecPath, byDMAChannel;
	BYTE byQ;
	BYTE *pBuffer, *pUserAddress;


	//////////////////////////////////////////////////////////////////////////
	// 1. Video capture
	//	---------------------------------------------------------------------------------------------------------
	//	|   Video Data				| QCnt									| VCAP_STATUS_INFO					|
	//  |							| (first queue only)					|									|
	//	|	Max 0x65400 bytes		| 0x66000-2*sizeof(VCAP_STATUS_INFO)	| 0x66000-sizeof(VCAP_STATUS_INFO)	|
	//	|-------------------------------------------------------------------------------------------------------|
	//	|	Total one queue size = 0x66000 bytes																|
	//	----------------------------------------------------------------------------------------------------------
	for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
	{
		pBuffer = pdx->stDMAVideoBuffer[byDMAChannel].pBuffer;
		for (byQ=0;byQ<MAX_VIDEO_QUEUE;byQ++)
		{
			pBuffer += VIDEO_ONE_QUEUE_SIZE*PAGESIZE;
			pdx->m_pVCAPStatus[byDMAChannel][byQ] = (VCAP_STATUS_INFO *)(pBuffer-sizeof(VCAP_STATUS_INFO));
		
		}
		pdx->m_pVCapCurQCnt[byDMAChannel] = (BYTE *)(pdx->m_pVCAPStatus[byDMAChannel][0]-1);
#if 0
		KdPrint(("VCAPStatus: byDMAChannel=%d, DMA buffer start = %08X-%08X, Status = %08X(%X), QCnt=%08X\n",
			byDMAChannel, pdx->stDMAVideoBuffer[byDMAChannel].pBuffer, pdx->stDMAVideoBuffer[byDMAChannel].pBuffer+VIDEO_ONE_QUEUE_SIZE*PAGESIZE,
			pdx->m_pVCAPStatus[byDMAChannel][0], sizeof(VCAP_STATUS_INFO),
			pdx->m_pVCapCurQCnt[byDMAChannel]));
#endif
		
	}

	// 2. Video Live
	pBuffer = pdx->stDMALiveBuffer.pBuffer;
	for (byQ=0;byQ<MAX_VIDEO_QUEUE;byQ++)
	{
		pBuffer += VIDEO_ONE_QUEUE_SIZE*PAGESIZE;
		pdx->m_pVLiveStatus[byQ] = (VCAP_STATUS_INFO *)(pBuffer-sizeof(VCAP_STATUS_INFO));
	}
	pdx->m_pVLiveCurQCnt = (BYTE *)(pdx->m_pVLiveStatus[0]-1);

	// 3. Audio capture
	//	-------------------------------------------------------------------------------------------------------------
	//	|   Audio Data				| QCnt									| ACAP_STATUS_INFO						|
	//  |							| (first queue only)					|										|
	//	|	Max 8000*16 bytes*16ch	| 8192*16*16-2*sizeof(ACAP_STATUS_INFO)	| 8192*16*16-sizeof(ACAP_STATUS_INFO)	|
	//	|-----------------------------------------------------------------------------------------------------------|
	//	|	Total one queue size = 512 Kbytes																		|
	//	-------------------------------------------------------------------------------------------------------------
	pBuffer = pdx->stDMAAudioBuffer.pBuffer;
	for (byQ=0;byQ<MAX_AUDIO_QUEUE;byQ++)
	{
		pBuffer += AUDIO_ONE_QUEUE_SIZE*PAGESIZE;
		pdx->m_pACAPStatus[byQ] = (ACAP_STATUS_INFO *)(pBuffer-sizeof(ACAP_STATUS_INFO));
	}
	pdx->m_pACapCurQCnt = (BYTE *)(pdx->m_pACAPStatus[0]-1);


}
//////////////////////////////////////////////////////////////////////////
//
VOID SetLocalRegister(PDEVICE_EXTENSION pdx)
{
	//////////////////////////////////////////////////////////////////////////
	// local variables
	BYTE byInternalVDec, byInternalMux, byExternalVDec;
	BYTE byDMAChannel;
	DWORD dwMux, H_DTO, V_DTO;
	DWORD dwOutputFormat;
	DWORD dwBurstSize;
	DWORD dwYOnly, dwRegData;
	DWORD dwBurstValue;
	BYTE reg;

	BYTE byDMAPath;

	//1. set initial value of register
	dwWriteRegister(pdx, REG_MEM_CTRL, 0x00004444);		//default

	//////////////////////////////////////////////////////////////////////////
	//0x650-0x65C must be set to 0
	dwWriteRegister(pdx, 0x650, 0);
	dwWriteRegister(pdx, 0x654, 0);
	dwWriteRegister(pdx, 0x658, 0);
	dwWriteRegister(pdx, 0x65C, 0);
	//////////////////////////////////////////////////////////////////////////
	
	//1.1. set scale factors & DMA size
	//1.1.1. video capture
	for (byDMAPath=0;byDMAPath<(pdx->m_byDecSection*MAX_DECODER);byDMAPath++)
	{
		GetScaleInfo(pdx->m_byVideoFormat, &pdx->m_stVCapSize[byDMAPath], byDMAPath>=EXTERNAL_VDEC_PATH_START);
		SetVCapSizeInfo(pdx, byDMAPath, &pdx->m_stVCapSize[byDMAPath], byDMAPath>=EXTERNAL_VDEC_PATH_START);
	}
	//////////////////////////////////////////////////////////////////////////

	//1.1.2. live, byInternalVDec is 4 now
	GetScaleInfo(pdx->m_byVideoFormat, &pdx->m_stVLiveSize, FALSE);
	SetVLiveSizeInfo(pdx, &pdx->m_stVLiveSize);

	//1.2. output format
	dwYOnly = 0;

	switch (pdx->m_dwBurstSize)
	{
//	case BURST_SIZE_2:			dwBurstValue = 0x02;		break;
	case BURST_SIZE_4:			dwBurstValue = 0x04;		break;
	case BURST_SIZE_8:			dwBurstValue = 0x08;		break;
	case BURST_SIZE_16:			dwBurstValue = 0x10;		break;
	case BURST_SIZE_32:			dwBurstValue = 0x20;		break;
	case BURST_SIZE_64:			dwBurstValue = 0x40;		break;

	default:
	case BURST_SIZE_128:		dwBurstValue = 0x80;		break;
	}

	dwOutputFormat = (DWORD)GetDataFormat(pdx->m_byVideoOutputFormat);
	dwBurstSize = 0x80;
	if (pdx->m_byVideoOutputFormat != DATAFORMAT_YUV422PK)
		dwBurstSize /= 2;
	if (pdx->m_byVideoOutputFormat == DATAFORMAT_Y8)
		dwYOnly = 0x1FFFF;
	
	dwWriteRegister(pdx, REG_PLANAR_MODE, dwOutputFormat);

	dwRegData = dwReadRegister(pdx, REG_SW_CTRL);
	dwRegData = (dwRegData & 0xFFFFE3FF) | (2<<10);		//BURST SEL
	dwWriteRegister(pdx, REG_SW_CTRL, dwRegData);

	dwWriteRegister(pdx, REG_Y_BURST, dwBurstSize);
	dwWriteRegister(pdx, REG_C_BURST, dwBurstSize/2);

	dwWriteRegister(pdx, REG_Y_ONLY, dwYOnly);



	//1.3. set DMA target address
	for (byDMAPath=0;byDMAPath<pdx->m_byDecSection*MAX_DECODER;byDMAPath++)
	{
		pdx->byVCapQueueCount[byDMAPath] = 0;
		SetVideoDMATargetAddress(pdx, byDMAPath, 0);
	}

	pdx->byVLiveQueueCount = 0;
	SetVLiveDMATargetAddress(pdx, 0);

	//2. Audio 
	pdx->byAudioQueueCount = 0;
	SetAudioDMATargetAddress(pdx, AD_PATH_A, 0);
	SetAudioDMATargetAddress(pdx, AD_PATH_B, 1);
	//2.1 Audio Size
	dwWriteRegister(pdx, REG_AUDIO_DMA_SIZE0, CUR_AUDIO_PACKET_LENGTH*MAX_AUDIO_CHANNEL_DEVICE/sizeof(DWORD));
	

	dwRegData = 0x00000000;
	for (byDMAPath=0;byDMAPath<pdx->m_byDecSection*MAX_DECODER;byDMAPath++)
	{
		if (!pdx->m_stVCapSize[byDMAPath].bFrame)
		{
			dwRegData |= 1<<byDMAPath;
		}
	}
	dwWriteRegister(pdx, REG_ODD_FIELD_EN, dwRegData);
	dwWriteRegister(pdx, REG_EVEN_FIELD_EN, 0x00000000);

	//Set live channel
	SetVideoLiveChannel(pdx, pdx->m_byVLiveChannel);

	//3. video loss ON
	//////////////////////////////////////////////////////////////////////////
	//pattern off & activate no-video detection
	dwWriteRegister(pdx, REG_DATA_SEL_07_00, 0x33333333);
	dwWriteRegister(pdx, REG_DATA_SEL_15_08, 0x33333333);
	dwWriteRegister(pdx, REG_DATA_SEL_LIVE, 0x333);

	dwWriteRegister(pdx, REG_PATTERN_TYPE_07_00, 0x99999999);
	dwWriteRegister(pdx, REG_PATTERN_TYPE_15_08, 0x99999999);
	dwWriteRegister(pdx, REG_PATTERN_TYPE_LIVE, 0x00000999);

	//////////////////////////////////////////////////////////////////////////

	
}

//////////////////////////////////////////////////////////////////////////
//
#ifdef _WIN64
VOID SetCaptureStart(PDEVICE_EXTENSION pdx)
{
	//////////////////////////////////////////////////////////////////////////
	//Local variables
	BYTE byDMAChannel;
	BYTE byQ;
	//BYTE *pUserAddress;
	DWORD dwUserAddress;
	DWORD dwData;
	BYTE byPos;

	//////////////////////////////////////////////////////////////////////////
	SetLocalRegister(pdx);

	pdx->m_dwVideoDMAEnable = 0;
	pdx->m_dwVideoIrqEnable = 0;
	pdx->m_dwAudioDMAEnable = 0;
	pdx->m_dwAudioIrqEnable = 0;

	//
	if (pdx->byStartFlag&DMA_VIDEO_RECORD)
	{
		for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		{
			dwUserAddress = (DWORD)(LONGLONG)pdx->stDMAVideoBuffer[byDMAChannel].pUserBuffer;
			for (byQ=0;byQ<MAX_VIDEO_QUEUE;byQ++)
			{
				pdx->m_pVCAPStatus[byDMAChannel][byQ]->byLock = MEM_UNLOCK;
				pdx->m_pVCAPStatus[byDMAChannel][byQ]->dwUserAddress = dwUserAddress;
				dwUserAddress += VIDEO_ONE_QUEUE_SIZE*PAGESIZE;
				pdx->m_pVCAPStatus[byDMAChannel][byQ]->dwFieldCount = 0x100;
			}
			pdx->m_pVCapCurQCnt[byDMAChannel][Q_FRONT] = 0;
			pdx->m_pVCapCurQCnt[byDMAChannel][Q_REAR] = 0;
			pdx->fQueueFullCondition[byDMAChannel] = FALSE;
		}

		for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		{
			if (pdx->m_byVideoDMA[byDMAChannel] == DMA_ON)
			{
				pdx->m_dwVideoDMAEnable |= 1<<byDMAChannel;
				pdx->m_dwVideoIrqEnable |= 1<<byDMAChannel;
				pdx->m_dwVideoDMAMask[byDMAChannel] = 1<<byDMAChannel;
				pdx->m_dwVideoIrqMask[byDMAChannel] = 1<<byDMAChannel;
			}
		}
	}

	if (pdx->byStartFlag&DMA_VIDEO_LIVE)
	{
		//pUserAddress = pdx->stDMALiveBuffer.pUserBuffer;
		dwUserAddress = (DWORD)(LONGLONG)pdx->stDMALiveBuffer.pUserBuffer;
		for (byQ=0;byQ<MAX_VIDEO_QUEUE;byQ++)
		{
			pdx->m_pVLiveStatus[byQ]->byLock = MEM_UNLOCK;
			pdx->m_pVLiveStatus[byQ]->dwUserAddress = dwUserAddress;
			dwUserAddress += VIDEO_ONE_QUEUE_SIZE*PAGESIZE;
		}
		*pdx->m_pVLiveCurQCnt = 0;

		if (pdx->m_byLiveDMA == DMA_ON)
		{
			pdx->m_dwVideoDMAEnable |= 1<<LIVE_VDEC_PATH_START;
			pdx->m_dwVideoIrqEnable |= 1<<LIVE_VDEC_PATH_START;
			pdx->m_dwVideoDMAMask[LIVE_VDEC_PATH_START] = 1<<LIVE_VDEC_PATH_START;
			pdx->m_dwVideoIrqMask[LIVE_VDEC_PATH_START] = 1<<LIVE_VDEC_PATH_START;
		}
	}

	if (pdx->byStartFlag&DMA_AUDIO_RECORD)
	{

		//////////////////////////////////////////////////////////////////////////
		//Set frequency
		SetAudioParametersEx(pdx);
		SetAudioParametersIn(pdx);
		DelayMicroSecond(100);

		//pUserAddress = pdx->stDMAAudioBuffer.pUserBuffer;
		dwUserAddress = (DWORD)(LONGLONG)pdx->stDMAAudioBuffer.pUserBuffer;
		for (byQ=0;byQ<MAX_AUDIO_QUEUE;byQ++)
		{
			pdx->m_pACAPStatus[byQ]->byLock = MEM_UNLOCK;
			//pdx->m_pACAPStatus[byQ]->pUserAddress = pUserAddress;
			pdx->m_pACAPStatus[byQ]->dwUserAddress = dwUserAddress;
			pdx->m_pACAPStatus[byQ]->dwPacketSize = CUR_AUDIO_PACKET_LENGTH*MAX_AUDIO_CHANNEL_DEVICE;
			dwUserAddress += AUDIO_ONE_QUEUE_SIZE*PAGESIZE;
		}
		*pdx->m_pACapCurQCnt = 0;

		if (pdx->m_byAudioDMA == DMA_ON)
		{
			pdx->m_dwAudioDMAEnable = 1;
			pdx->m_dwAudioIrqEnable = AD_IRQ0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		pdx->byCurTablePos[byDMAChannel] = 0;
	pdx->byCurTablePos[LIVE_VDEC_PATH_START] = 0;


	//////////////////////////////////////////////////////////////////////////
	dwData = dwReadRegister(pdx, REG_PATTERN_SET);
	dwData &= ~0x00000001;
	if (pdx->m_byVideoFormat == VIDEO_FORMAT_NTSC)
		dwData |= (1<<12);
	else
		dwData &= ~(1<<12);
	dwWriteRegister(pdx, REG_PATTERN_SET, dwData);
	//////////////////////////////////////////////////////////////////////////

	dwWriteRegister(pdx, REG_NO_DMA_COMMAND, 0x00000000);
	//Set DMA
	dwWriteRegister(pdx, REG_VIDEO_ENABLE, pdx->m_dwVideoDMAEnable);
	dwWriteRegister(pdx, REG_DMA_ENABLE_VIDEO, pdx->m_dwVideoDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_VIDEO, pdx->m_dwVideoIrqEnable);
	dwWriteRegister(pdx, REG_DMA_ENABLE_AUDIO, pdx->m_dwAudioDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_AUDIO, pdx->m_dwAudioIrqEnable);
	dwData = dwReadRegister(pdx, REG_IRQ_ENABLE_ALL);
	dwData |= 1<<0;
	dwWriteRegister(pdx, REG_IRQ_ENABLE_ALL, dwData);

	pdx->byStartFlag = 0;
	pdx->byStopFlag = 0;

}
#else
VOID SetCaptureStart(PDEVICE_EXTENSION pdx)
{
	//////////////////////////////////////////////////////////////////////////
	//Local variables
	BYTE byDMAChannel;
	BYTE byQ;
	BYTE *pUserAddress;
	DWORD dwData;
	BYTE byPos;

	//////////////////////////////////////////////////////////////////////////
	SetLocalRegister(pdx);

	pdx->m_dwVideoDMAEnable = 0;
	pdx->m_dwVideoIrqEnable = 0;
	pdx->m_dwAudioDMAEnable = 0;
	pdx->m_dwAudioIrqEnable = 0;

	//
	if (pdx->byStartFlag&DMA_VIDEO_RECORD)
	{
		for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		{
			pUserAddress = pdx->stDMAVideoBuffer[byDMAChannel].pUserBuffer;
			for (byQ=0;byQ<MAX_VIDEO_QUEUE;byQ++)
			{
				pdx->m_pVCAPStatus[byDMAChannel][byQ]->byLock = MEM_UNLOCK;
				pdx->m_pVCAPStatus[byDMAChannel][byQ]->pUserAddress = pUserAddress;
				pUserAddress += VIDEO_ONE_QUEUE_SIZE*PAGESIZE;
				pdx->m_pVCAPStatus[byDMAChannel][byQ]->dwFieldCount = 0x100;
			}
			pdx->m_pVCapCurQCnt[byDMAChannel][Q_FRONT] = 0;
			pdx->m_pVCapCurQCnt[byDMAChannel][Q_REAR] = 0;
			pdx->fQueueFullCondition[byDMAChannel] = FALSE;
		}

		for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		{
			if (pdx->m_byVideoDMA[byDMAChannel] == DMA_ON)
			{
				pdx->m_dwVideoDMAEnable |= 1<<byDMAChannel;
				pdx->m_dwVideoIrqEnable |= 1<<byDMAChannel;
				pdx->m_dwVideoDMAMask[byDMAChannel] = 1<<byDMAChannel;
				pdx->m_dwVideoIrqMask[byDMAChannel] = 1<<byDMAChannel;
			}
		}
	}

	if (pdx->byStartFlag&DMA_VIDEO_LIVE)
	{
		pUserAddress = pdx->stDMALiveBuffer.pUserBuffer;
		for (byQ=0;byQ<MAX_VIDEO_QUEUE;byQ++)
		{
			pdx->m_pVLiveStatus[byQ]->byLock = MEM_UNLOCK;
			pdx->m_pVLiveStatus[byQ]->pUserAddress = pUserAddress;
			pUserAddress += VIDEO_ONE_QUEUE_SIZE*PAGESIZE;
		}
		*pdx->m_pVLiveCurQCnt = 0;

		if (pdx->m_byLiveDMA == DMA_ON)
		{
			pdx->m_dwVideoDMAEnable |= 1<<LIVE_VDEC_PATH_START;
			pdx->m_dwVideoIrqEnable |= 1<<LIVE_VDEC_PATH_START;
			pdx->m_dwVideoDMAMask[LIVE_VDEC_PATH_START] = 1<<LIVE_VDEC_PATH_START;
			pdx->m_dwVideoIrqMask[LIVE_VDEC_PATH_START] = 1<<LIVE_VDEC_PATH_START;
		}
	}

	if (pdx->byStartFlag&DMA_AUDIO_RECORD)
	{

		//////////////////////////////////////////////////////////////////////////
		//Set frequency
		SetAudioParametersEx(pdx);
		SetAudioParametersIn(pdx);
		DelayMicroSecond(100);

		pUserAddress = pdx->stDMAAudioBuffer.pUserBuffer;
		for (byQ=0;byQ<MAX_AUDIO_QUEUE;byQ++)
		{
			pdx->m_pACAPStatus[byQ]->byLock = MEM_UNLOCK;
			pdx->m_pACAPStatus[byQ]->pUserAddress = pUserAddress;
			pdx->m_pACAPStatus[byQ]->dwPacketSize = CUR_AUDIO_PACKET_LENGTH*MAX_AUDIO_CHANNEL_DEVICE;
			pUserAddress += AUDIO_ONE_QUEUE_SIZE*PAGESIZE;
		}
		*pdx->m_pACapCurQCnt = 0;

		if (pdx->m_byAudioDMA == DMA_ON)
		{
			pdx->m_dwAudioDMAEnable = 1;
			pdx->m_dwAudioIrqEnable = AD_IRQ0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	for (byDMAChannel=0;byDMAChannel<pdx->m_byDecSection*MAX_DECODER;byDMAChannel++)
		pdx->byCurTablePos[byDMAChannel] = 0;
	pdx->byCurTablePos[LIVE_VDEC_PATH_START] = 0;


	//////////////////////////////////////////////////////////////////////////
	dwData = dwReadRegister(pdx, REG_PATTERN_SET);
	dwData &= ~0x00000001;
	if (pdx->m_byVideoFormat == VIDEO_FORMAT_NTSC)
		dwData |= (1<<12);
	else
		dwData &= ~(1<<12);
	dwWriteRegister(pdx, REG_PATTERN_SET, dwData);
	//////////////////////////////////////////////////////////////////////////

	dwWriteRegister(pdx, REG_NO_DMA_COMMAND, 0x00000000);
	//Set DMA
	dwWriteRegister(pdx, REG_VIDEO_ENABLE, pdx->m_dwVideoDMAEnable);
	dwWriteRegister(pdx, REG_DMA_ENABLE_VIDEO, pdx->m_dwVideoDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_VIDEO, pdx->m_dwVideoIrqEnable);
	dwWriteRegister(pdx, REG_DMA_ENABLE_AUDIO, pdx->m_dwAudioDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_AUDIO, pdx->m_dwAudioIrqEnable);
	dwData = dwReadRegister(pdx, REG_IRQ_ENABLE_ALL);
	dwData |= 1<<0;
	dwWriteRegister(pdx, REG_IRQ_ENABLE_ALL, dwData);

	pdx->byStartFlag = 0;
	pdx->byStopFlag = 0;

}
#endif
VOID SetCaptureStop(PDEVICE_EXTENSION pdx)
{
	DWORD dwDMAChannel;
	DWORD dwData;

	if (pdx->byStopFlag&DMA_VIDEO_RECORD)
	{
		dwData = dwReadRegister(pdx, REG_VIDEO_ENABLE);	
		dwData &= 0xFFFF0000;
		dwWriteRegister(pdx, REG_VIDEO_ENABLE, dwData);

		for (dwDMAChannel=0;dwDMAChannel<MAX_DMA_PATH_VCAP;dwDMAChannel++)
		{
			pdx->m_dwVideoDMAMask[dwDMAChannel] = 0;
			pdx->m_dwVideoIrqMask[dwDMAChannel] = 0;
		}
		pdx->m_dwVideoDMAEnable &= 0xFFFF0000;
		pdx->m_dwVideoIrqEnable &= 0xFFFF0000;

	}

	if (pdx->byStopFlag&DMA_VIDEO_LIVE)
	{
		dwData = dwReadRegister(pdx, REG_VIDEO_ENABLE);
		dwData &= 0xFFFEFFFF;
		dwWriteRegister(pdx, REG_VIDEO_ENABLE, dwData);
		pdx->m_dwVideoDMAMask[LIVE_VDEC_PATH_START] = 0;
		pdx->m_dwVideoIrqMask[LIVE_VDEC_PATH_START] = 0;
		pdx->m_dwVideoDMAEnable &= ~(1<<LIVE_VDEC_PATH_START);
		pdx->m_dwVideoIrqEnable &= ~(1<<LIVE_VDEC_PATH_START);
	}

	if (pdx->byStopFlag&DMA_AUDIO_RECORD)
	{
		pdx->m_dwAudioDMAEnable = 0;
		pdx->m_dwAudioIrqEnable = 0;
	}

	dwWriteRegister(pdx, REG_DMA_ENABLE_VIDEO, pdx->m_dwVideoDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_VIDEO, pdx->m_dwVideoIrqEnable);

	dwWriteRegister(pdx, REG_DMA_ENABLE_AUDIO, pdx->m_dwAudioDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_AUDIO, pdx->m_dwAudioIrqEnable);

	if ((pdx->m_dwVideoDMAEnable==0)&&(pdx->m_dwAudioDMAEnable==0))
	{
		dwData = dwReadRegister(pdx, REG_IRQ_ENABLE_ALL);
		dwData &= ~(1<<0);
		dwWriteRegister(pdx, REG_IRQ_ENABLE_ALL, dwData);

	}

}

//////////////////////////////////////////////////////////////////////////
VOID SetVideoDMATargetAddress(PDEVICE_EXTENSION pdx, BYTE byDMAChannel, BYTE byCurQCnt)
{
	DWORD dwDMATargetAddress;
	dwDMATargetAddress = pdx->stDMAVideoBuffer[byDMAChannel].dwBuffer;
	dwDMATargetAddress += VIDEO_ONE_QUEUE_SIZE*PAGESIZE*byCurQCnt;
	if (pdx->m_byVideoOutputFormat == DATAFORMAT_YUV422PK)
	{
		dwWriteRegister2(pdx, REG_BASE_ADDR_Y(byDMAChannel), dwDMATargetAddress);
		dwWriteRegister2(pdx, REG_BASE_ADDR_U(byDMAChannel), 0);
		dwWriteRegister2(pdx, REG_BASE_ADDR_V(byDMAChannel), 0);
	}
	else
	{
		dwWriteRegister2(pdx, REG_BASE_ADDR_Y(byDMAChannel), dwDMATargetAddress+VIDEO_Y_OFFSET);
		dwWriteRegister2(pdx, REG_BASE_ADDR_U(byDMAChannel), dwDMATargetAddress+VIDEO_U_OFFSET);
		dwWriteRegister2(pdx, REG_BASE_ADDR_V(byDMAChannel), dwDMATargetAddress+VIDEO_V_OFFSET);
	}

}

VOID SetVLiveDMATargetAddress(PDEVICE_EXTENSION pdx, BYTE byCurQCnt)
{
	DWORD dwDMATargetAddress;
	dwDMATargetAddress = pdx->stDMALiveBuffer.dwBuffer;
	dwDMATargetAddress += VIDEO_ONE_QUEUE_SIZE*PAGESIZE*byCurQCnt;
	if (pdx->m_byVideoOutputFormat == DATAFORMAT_YUV422PK)
	{
		dwWriteRegister2(pdx, REG_BASE_ADDR_Y_LIVE, dwDMATargetAddress);
		dwWriteRegister2(pdx, REG_BASE_ADDR_U_LIVE, 0);
		dwWriteRegister2(pdx, REG_BASE_ADDR_V_LIVE, 0);
	}
	else
	{
		dwWriteRegister2(pdx, REG_BASE_ADDR_Y_LIVE, dwDMATargetAddress+VIDEO_Y_OFFSET);
		dwWriteRegister2(pdx, REG_BASE_ADDR_U_LIVE, dwDMATargetAddress+VIDEO_U_OFFSET);
		dwWriteRegister2(pdx, REG_BASE_ADDR_V_LIVE, dwDMATargetAddress+VIDEO_V_OFFSET);

	}
}

VOID SetVCapSizeInfo(PDEVICE_EXTENSION pdx, BYTE byDMAChannel, LPST_IMAGE_SIZE lpstImageSize, BOOL fSetExternal)
{
	DWORD DMA_SIZE;			//total number of DMA transferring data with 4bytes unit
	DWORD H_SIZE;			//width of video image
	DWORD H_DTO, V_DTO;


	H_SIZE = dwReadRegister(pdx, REG_HSIZE(byDMAChannel));
	if (lpstImageSize->bFrame)
		DMA_SIZE = lpstImageSize->Width*lpstImageSize->Height/2*PIXEL_BYTE/sizeof(DWORD);
	else
		DMA_SIZE = lpstImageSize->Width*lpstImageSize->Height*PIXEL_BYTE/sizeof(DWORD);

	if (byDMAChannel%2)
	{
		H_SIZE &= 0x0000FFFF;
		H_SIZE |= lpstImageSize->Width<<16;
	}
	else
	{
		H_SIZE &= 0xFFFF0000;
		H_SIZE |= lpstImageSize->Width;
	}
	dwWriteRegister(pdx, REG_HSIZE(byDMAChannel), H_SIZE);
	dwWriteRegister(pdx, REG_DMA_SIZE(byDMAChannel), DMA_SIZE);

	if (byDMAChannel<INTERNAL_VDEC_PATH)
	{
		V_DTO = dwReadRegister(pdx, REG_V_DTO(byDMAChannel))&0xFF000000;
		V_DTO |= lpstImageSize->V_DTO | 0x04000000;		//Enable Scaler
		H_DTO = lpstImageSize->H_DTO;
		dwWriteRegister(pdx, REG_H_DTO(byDMAChannel), H_DTO);
		dwWriteRegister(pdx, REG_V_DTO(byDMAChannel), V_DTO);
	}
	else if (fSetExternal)
	{
		SetExtenalVideoScaler(pdx, byDMAChannel, lpstImageSize);
	}

}

VOID SetVLiveSizeInfo(PDEVICE_EXTENSION pdx, LPST_IMAGE_SIZE lpstImageSize)
{
	DWORD DMA_SIZE;			//total number of DMA transferring data with 4bytes unit
	DWORD H_SIZE;			//width of video image
	DWORD H_DTO, V_DTO;
	
	
	H_SIZE = lpstImageSize->Width;
	if (lpstImageSize->bFrame)
		DMA_SIZE = lpstImageSize->Width*lpstImageSize->Height/2*PIXEL_BYTE/sizeof(DWORD);
	else
		DMA_SIZE = lpstImageSize->Width*lpstImageSize->Height*PIXEL_BYTE/sizeof(DWORD);
	dwWriteRegister(pdx, REG_HSIZE_LIVE, H_SIZE);
	dwWriteRegister(pdx, REG_DMA_SIZE_LIVE, DMA_SIZE);
	
	V_DTO = dwReadRegister(pdx, REG_V_DTO_LIVE)&0xFF000000;
	V_DTO |= lpstImageSize->V_DTO | 0x04000000;
	H_DTO = lpstImageSize->H_DTO;
	dwWriteRegister(pdx, REG_H_DTO_LIVE, H_DTO);
	dwWriteRegister(pdx, REG_V_DTO_LIVE, V_DTO);
}

VOID SetAudioDMATargetAddress(PDEVICE_EXTENSION pdx, BYTE bPath, BYTE byCurQCnt)
{
	DWORD dwDMATargetAddress;
	dwDMATargetAddress = pdx->stDMAAudioBuffer.dwBuffer;
	dwDMATargetAddress += AUDIO_ONE_QUEUE_SIZE*PAGESIZE*byCurQCnt;
	if (bPath == AD_PATH_A)		//BA0_A
	{
		dwWriteRegister2(pdx, REG_AUDIO_BA0_A, dwDMATargetAddress);
	}
	else						//BA0_B
	{
		dwWriteRegister2(pdx, REG_AUDIO_BA0_B, dwDMATargetAddress);
	}

}

//////////////////////////////////////////////////////////////////////////
VOID SetVideoLiveChannel(PDEVICE_EXTENSION pdx, BYTE byVLiveChannel)
{
	DWORD dwRegData;
	DWORD dwVLiveChannel;
	BYTE byDecChannel;
	BYTE bDevAddr[EXT_DECODER_COUNT] = { 0x70, 0x72, 0x74 };
	//byVLiveChannel = 0 to 15
	dwRegData = dwReadRegister(pdx, REG_LIVE_CHANNEL_SET);

	if (byVLiveChannel<EXTERNAL_VDEC_PATH_START)
	{
		dwVLiveChannel = (DWORD)byVLiveChannel;
	}
	else
	{
		//dwVLiveChannel is one of 4, 5 and 6
		dwVLiveChannel = (byVLiveChannel-EXTERNAL_VDEC_PATH_START)/MAX_DECODER+EXTERNAL_VDEC_PATH_START;

		//
		byDecChannel = (byVLiveChannel%MAX_DECODER)|0x10;
		I2C_Write(pdx, 0, bDevAddr[dwVLiveChannel-EXTERNAL_VDEC_PATH_START], 0x3F, byDecChannel);

	}
	dwRegData = (dwRegData&0xFFFFF8FF) | ((dwVLiveChannel<<8)&0x00000700);
	dwWriteRegister(pdx, REG_LIVE_CHANNEL_SET, dwRegData);

}



//////////////////////////////////////////////////////////////////////////
//Base function
VOID DelayPer100nanoseconds(PDEVICE_EXTENSION pdx, DWORD DelayTime)
{
	DWORD	dwFirstHiTime, dwFirstLowTime;
	DWORD	dwLastHiTime,  dwLastLowTime;
	DWORD	dwDiffTime;
	
	GetCurrentSystemTime(&dwFirstHiTime, &dwFirstLowTime);	
	while(TRUE)
	{
		GetCurrentSystemTime(&dwLastHiTime, &dwLastLowTime);
		dwDiffTime = dwLastLowTime - dwFirstLowTime;
		if(dwDiffTime > DelayTime)
			break;
	}
}

VOID GetCurrentSystemTime(DWORD* pdwHiTime, DWORD* pdwLowTime)
{
	ULONGLONG	ticks, rate;
	
	ticks = (ULONGLONG)KeQueryPerformanceCounter((PLARGE_INTEGER)&rate).QuadPart;
	ticks = (ticks & 0xFFFFFFFF00000000) / rate * 10000000 + (ticks & 0x00000000FFFFFFFF) * 10000000 / rate;
	*pdwHiTime = (DWORD)(ticks >> 32) & 0xFFFFFFFF;
	*pdwLowTime = (DWORD)ticks & 0xFFFFFFFF;
}

BOOL LocalTimeInit(PDEVICE_EXTENSION pdx)
{
	LARGE_INTEGER	StartSystemTime, StartLocalTime, StartFrequency;
	LARGE_INTEGER	EndSystemTime, EndLocalTime, EndFrequency, StartCounter;
	TIME_FIELDS		StartTimeFields, EndTimeFields;
	BYTE			i;
	BOOL			bRet = FALSE;

	KeQuerySystemTime(&StartSystemTime);	//Sometimes Error -> why return value void ? Microsoft bug
	ExSystemTimeToLocalTime(&StartSystemTime, &StartLocalTime);
	RtlTimeToTimeFields(&StartLocalTime, &StartTimeFields);
	StartCounter = KeQueryPerformanceCounter(&StartFrequency);

	for(i=0; i<100; i++)
	{
		KeQuerySystemTime(&EndSystemTime);
		StartCounter = KeQueryPerformanceCounter(&EndFrequency);
		ExSystemTimeToLocalTime(&EndSystemTime, &EndLocalTime);
		RtlTimeToTimeFields(&EndLocalTime, &EndTimeFields);
		if((StartTimeFields.Year == EndTimeFields.Year) && (StartTimeFields.Month == EndTimeFields.Month) && 
			(StartTimeFields.Day == EndTimeFields.Day) && (StartTimeFields.Hour == EndTimeFields.Hour) &&
			(StartTimeFields.Minute == EndTimeFields.Minute) && (StartFrequency.QuadPart == EndFrequency.QuadPart))
		{
			pdx->m_liStartCounter.QuadPart = StartCounter.QuadPart;
			pdx->m_liFrequency.QuadPart = EndFrequency.QuadPart;
			pdx->m_liStartLocalTime.QuadPart = EndLocalTime.QuadPart;
			bRet = TRUE;
			break;
		}
		else
		{
			memcpy((BYTE *)&StartTimeFields, (BYTE *)&EndTimeFields, sizeof(TIME_FIELDS));
			StartFrequency.QuadPart = EndFrequency.QuadPart;
		}
	}
	return bRet;
}

#define MICROSECOND		1000000
DWORD GetTick()
//Tick at 1 micro second unit
{
#ifdef MICROSECOND
	ULONGLONG counter;
	ULONGLONG freq;
	DWORD dwTick;
	counter = (ULONGLONG)KeQueryPerformanceCounter((PLARGE_INTEGER)&freq).QuadPart;
	counter = counter * MICROSECOND / freq;
	dwTick = (DWORD)(counter&0xFFFFFFFF);
	return dwTick;
#else
	LARGE_INTEGER time;
	KeQuerySystemTime(&time);
	return (DWORD) ((time.QuadPart/10)&0xFFFFFFFF);
#endif
}

DWORD GetTimeInterval(DWORD dwPrevTime, DWORD dwCurrTime)
{
	if (dwPrevTime>dwCurrTime)
		return (0xFFFFFFFF-dwPrevTime+dwCurrTime);
	return (dwCurrTime-dwPrevTime);
}

VOID DelayMicroSecond(int uSec)
{
	KeStallExecutionProcessor(uSec);
}

VOID Hold(DWORD uSec)
{
	DWORD dwTick;
	dwTick = GetTick();
	while (GetTimeInterval(dwTick, GetTick())<uSec)
	{
		KeStallExecutionProcessor(1);
	}
}

//////////////////////////////////////////////////////////////////////////
VOID AllDMADisable(PDEVICE_EXTENSION pdx)
{

	//////////////////////////////////////////////////////////////////////////
	//video 
	pdx->m_dwVideoDMAEnable = 0x00000000;
	pdx->m_dwVideoIrqEnable = 0x00000000;
	dwWriteRegister(pdx, REG_DMA_ENABLE_VIDEO, pdx->m_dwVideoDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_VIDEO, pdx->m_dwVideoIrqEnable);

	//audio
	pdx->m_dwAudioDMAEnable = 0;
	pdx->m_dwAudioIrqEnable = 0;
	dwWriteRegister(pdx, REG_DMA_ENABLE_AUDIO, pdx->m_dwAudioDMAEnable);
	dwWriteRegister(pdx, REG_IRQ_ENABLE_AUDIO, pdx->m_dwAudioIrqEnable);

	//total
	dwWriteRegister(pdx, REG_IRQ_ENABLE_ALL, 0);

}


//////////////////////////////////////////////////////////////////////////
VOID CheckVideoSync(PDEVICE_EXTENSION pdx)
{
	BYTE byDMAChannel;
	DWORD dwStatus;

	dwStatus = dwReadRegister(pdx, REG_VLOSS_STATUS);
	for (byDMAChannel=0;byDMAChannel<MAX_DMA_PATH_VCAP;byDMAChannel++)
		pdx->bVideoSync[byDMAChannel] = (BYTE)((dwStatus>>byDMAChannel)&1);
}


//////////////////////////////////////////////////////////////////////////
// 
VOID GetScaleInfo(BYTE byVideoFormat, LPST_IMAGE_SIZE lpstImageSize, BOOL fExternal)
{
	//when calling this function, 
	//lpstImageSize->Width and lpstImageSize->Height should be set

	DWORD dwDTOBase = 0x100000;
	if (fExternal)
		dwDTOBase >>= 1;

	lpstImageSize->H_DTO = lpstImageSize->Width*dwDTOBase/IMAGEWIDTH_R704;
	if (lpstImageSize->H_DTO>dwDTOBase)
		lpstImageSize->H_DTO=dwDTOBase;
	if (byVideoFormat == VIDEO_FORMAT_NTSC)
	{
		if (lpstImageSize->Height>IMAGEHEIGHT_R240)
		{
			//both field capture
			lpstImageSize->V_DTO = lpstImageSize->Height*dwDTOBase/IMAGEHEIGHT_R480;
			lpstImageSize->bFrame = TRUE;
		}
		else
		{
			//1 field capture
			lpstImageSize->V_DTO = lpstImageSize->Height*dwDTOBase/IMAGEHEIGHT_R240;
			lpstImageSize->bFrame = FALSE;
		}
	}
	else
	{
		if (lpstImageSize->Height>IMAGEHEIGHT_R288)
		{
			//both field capture
			lpstImageSize->V_DTO = lpstImageSize->Height*dwDTOBase/IMAGEHEIGHT_R576;
			lpstImageSize->bFrame = TRUE;
		}
		else
		{
			//1 field capture
			lpstImageSize->V_DTO = lpstImageSize->Height*dwDTOBase/IMAGEHEIGHT_R288;
			lpstImageSize->bFrame = FALSE;
		}
	}
}

BYTE GetDataFormat(BYTE byDataFormat)
{
	if (byDataFormat == DATAFORMAT_Y8)
		return DATAFORMAT_YUV411PL;
	return byDataFormat;
}

DWORD GetHeightIndication(BYTE byVideoFormat)
{
	return (byVideoFormat == VIDEO_FORMAT_NTSC) ? IMAGEHEIGHT_R240 : IMAGEHEIGHT_R288;
}

//////////////////////////////////////////////////////////////////////////
//
void SetBrightness(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	if (byDMAChannel<INTERNAL_VDEC_PATH)
		SetBrightnessIn(pdx, byDMAChannel);
	else
		SetBrightnessEx(pdx, byDMAChannel);
}

void SetContrast(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	if (byDMAChannel<INTERNAL_VDEC_PATH)
		SetContrastIn(pdx, byDMAChannel);
	else
		SetContrastEx(pdx, byDMAChannel);
}

void SetHue(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	if (byDMAChannel<INTERNAL_VDEC_PATH)
		SetHueIn(pdx, byDMAChannel);
	else
		SetHueEx(pdx, byDMAChannel);
}

void SetSaturation(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	if (byDMAChannel<INTERNAL_VDEC_PATH)
		SetSaturationIn(pdx, byDMAChannel);
	else
		SetSaturationEx(pdx, byDMAChannel);
}
