#include "NVP1208.h"

//////////////////////////////////////////////////////////////////////////
VOID Chip_Reset(PDEVICE_EXTENSION pdx, DWORD dwReset)
{
	DWORD dwGPIODir;
	DWORD dwGPIO;

	dwWriteRegister(pdx, REG_GPIO_DIR, 0xFFFFFFFF);		//direction to read all

	dwGPIODir = dwReadRegister(pdx, REG_GPIO_DIR);
	dwGPIODir &= ~dwReset;		//direction to output
	dwWriteRegister(pdx, REG_GPIO_DIR, dwGPIODir);

	dwGPIO = dwReadRegister(pdx, REG_GPIO_IN);
	dwGPIO &= ~dwReset;
	dwWriteRegister(pdx, REG_GPIO_OUT, dwGPIO);

	DelayMicroSecond(4);

	dwGPIO |= dwReset;
	dwWriteRegister(pdx, REG_GPIO_OUT, dwGPIO);
	DelayMicroSecond(10);
}
//////////////////////////////////////////////////////////////////////////
//
VOID Decoder_Init(PDEVICE_EXTENSION pdx)
{
	InitializeNVP1208Dec(pdx);
}

//////////////////////////////////////////////////////////////////////////
//Initialize decoder

VOID External_Decoder_Init(PDEVICE_EXTENSION pdx)
{
	BYTE *pTable;

	int dev;
	BYTE bDevAddr[EXT_DECODER_COUNT] = { 0x70, 0x72, 0x74 };
	BYTE bAudioStage[EXT_DECODER_COUNT];

	switch (pdx->m_byDecSection)
	{
	case 2:
		bAudioStage[0] = STAGE_SINGLE;
		break;
	case 3:
		bAudioStage[0] = STAGE_FIRST;
		bAudioStage[1] = STAGE_LAST;
		break;
	case 4:
		bAudioStage[0] = STAGE_FIRST;
		bAudioStage[1] = STAGE_MIDDLE;
		bAudioStage[2] = STAGE_LAST;
		break;

	}

	for (dev=0;dev<(pdx->m_byDecSection-1);dev++)
	{
		InitializeNVP1104A(pdx, 0, bDevAddr[dev], bAudioStage[dev]);
	}
}

VOID SetExtenalVideoScaler(PDEVICE_EXTENSION pdx, BYTE byDMAChannel, LPST_IMAGE_SIZE lpstImageSize)
{
	//bExternalChannel range = [0~11]
	BYTE bDevAddr[EXT_DECODER_COUNT] = { 0x70, 0x72, 0x74 };
	BYTE bExternalChannel = byDMAChannel - EXTERNAL_VDEC_PATH_START;

	BYTE i, bData;

	BYTE bCurrentDev;
	BYTE bCurrentChannel;
	BYTE *pHDTOData, *pVDTOData;

	bCurrentDev = bExternalChannel/MAX_DECODER;
	bCurrentChannel = bExternalChannel%MAX_DECODER;

	SetScaler(pdx, 0, bDevAddr[bCurrentDev], bCurrentChannel, lpstImageSize->H_DTO, lpstImageSize->V_DTO);

}

//////////////////////////////////////////////////////////////////////////
VOID SetDecoderAFEMode(PDEVICE_EXTENSION pdx)
{
	BYTE bDevAddr = 0x76;
	BYTE bSubAddr = 0x1B;
	BYTE bData, bDevID = 0x73;

	I2C_Read(pdx, 0, bDevAddr, bSubAddr, &bData);
	if (bData == bDevID)
	{
		InitializeNVP1104A_AFE(pdx, 0, bDevAddr);
	}
}

