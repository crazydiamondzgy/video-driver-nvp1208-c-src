#include "NVP1208.h"

void InitializeNVP1208Dec(PDEVICE_EXTENSION pdx)
{
	//////////////////////////////////////////////////////////////////////////
	//NVP1208 decoder (0x000~0x0FC)
	//NTSC
// 	DWORD NVP1208_DECODER_NTSC[] = 
// 	{
// 		0x00000000, 0x00000000, 0x00000000,	0x00000000,		//0x000, read only register
// 		0x00000000, 0x00000000, 0x00000000, 0x00000000,		//0x010, read only register
// 		0x00000004, 0x00000000, 0x00000000, 0x00000000,		//0x020, reserved
// 		0x00888802, 0x00020000, 0x32100003, 0xBA987654,		//0x030
// 		
// 		0x48E4FEDC, 0x88880000, 0x88888888, 0x88888888,		//0x040
// 		0x19888888, 0x00001618, 0xC0FF0000, 0xAAAAAAAA,		//0x050
// 		0x020002AA, 0x00000200, 0x3C550500, 0x00073C3C,		//0x060
// 		0x6C4000D0, 0x4020009F, 0x0F385080, 0x00104304,		//0x070
// 		
// 		0x00B83000, 0xB9110606, 0x800005B2, 0x37498037,		//0x080
// 		0xFFDFFFEF, 0x00000000, 0x800A1560, 0x88010C80,		//0x090
// 		0x00000000, 0x80110180, 0x00000000, 0x03008002,		//0x0A0
// 		0x00230000, 0x2FF0DC2A, 0x0FD38257, 0x009D501F,		//0x0B0
// 		
// 		0x30000000, 0x0007001A, 0x09800068, 0xA243E010,		//0x0C0
// 		0x25150000, 0x00030300, 0x00321001,	0x003FED00,		//0x0D0
// 		0xB5510000, 0xFF220313, 0x00000000, 0x0000008D,		//0x0E0
// 		0x10200000, 0x00000000, 0x00000000, 0x10000000		//0x0F0
// 
// 	};

	//2010.12.14
	//comment: should be careful about 0x34 setting
	DWORD NVP1208_DECODER_NTSC[] = 
	{
		0x00000000, 0x00000000, 0x00000000,	0x00000000,		//0x000, read only register
		0x00000000, 0x00000000, 0x00000000, 0x00000000,		//0x010, read only register
		0x00000004, 0x00000000, 0x00000000, 0x00000000,		//0x020, reserved
		0x00888802, 0x00020000, 0x00000000, 0x00000000,		//0x030
		
		0x48E4FEDC, 0x00000000, 0x00000000, 0x00000000,		//0x040
		0x00000000, 0x00000000, 0x00FF0000, 0x0000AAAA,		//0x050
		0x020002AA, 0x00000200, 0x3C550500, 0x00070000,		//0x060
		0x6C4000D0, 0x4020009F, 0x0F385080, 0x00104304,		//0x070
		
		0x00B83000, 0xB9110606, 0x800005B2, 0x37498037,		//0x080
		0xFFDFFFEF, 0x00000000, 0x800A1560, 0x88010C80,		//0x090
		0x00000000, 0x80110180, 0x00000000, 0x03008002,		//0x0A0
		0x00230000, 0x2FF0D12A, 0x0FD38257, 0x009D501F,		//0x0B0
		
		0x30000000, 0x0007001A, 0x09800068, 0xA243E010,		//0x0C0
		0x25150000, 0x00030300, 0x00321001,	0x003FED00,		//0x0D0
		0xB5510000, 0xFF220313, 0x00000000, 0x0000008D,		//0x0E0
		0x10200000, 0x00000000, 0x00000000, 0x10000000		//0x0F0
			
	};
	//////////////////////////////////////////////////////////////////////////
	//NTSC, 0x200-0x25C
	DWORD NVP1208_DECODER_NTSC_2[] = 
	{
		0x80800000, 0x55F08080, 0x00000000, 0x00000000,		//0x200
		0x80808080, 0x80808080, 0x00000000, 0x00000000,		//0x210
		0x03030303, 0x00000000, 0x00000000, 0x00006666,		//0x220
		0x11111111, 0x30303030, 0x3B3B3B3B, 0x00000000,		//0x230
		0x28282828, 0x00000000, 0x00000000, 0x00000000,		//0x240
		0x00000000, 0x88883210, 0x00000000, 0x88888888		//0x250
	};

	//////////////////////////////////////////////////////////////////////////
	//PAL
// 	DWORD NVP1208_DECODER_PAL[] = 
// 	{
// 		0x00000000, 0x00000000, 0x00000000,	0x00000000,		//0x000, read only register
// 		0x00000000, 0x00000000, 0x00000000, 0x00000000,		//0x010, read only register
// 		0x00000004, 0x00000000, 0x00000000, 0x00000000,		//0x020, reserved
// 		0x00888802, 0x00020000, 0x32100003, 0xBA987654,		//0x030
// 		
// 		0x48E4FEDC, 0x88880000, 0x88888888, 0x88888888,		//0x040
// 		0x19888888, 0x00001618, 0xC0FF0000, 0xAAAAAAAA,		//0x050
// 		0x020002AA, 0x00000200, 0x3C550500, 0x00073C3C,		//0x060
// 		0x6C4000D0, 0x4020009F, 0x0F385080, 0x00104304,		//0x070
// 		
// 		0x00D83000, 0xB9110606, 0x800005B2, 0x37498037,		//0x080
// 		0xFFDFFFEF, 0x00000000, 0x800A15BD, 0x88010C80,		//0x090
// 		0x00000000, 0x80110180, 0x00000000, 0x03008000,		//0x0A0
// 		0x00230404, 0x2FF0CC2A, 0x0FD30157, 0x0080501F,		//0x0B0
// 		
// 		0x40000000, 0x000D001E, 0x09800068, 0xA243E000,		//0x0C0
// 		0x25170000, 0x00020200, 0x00321011,	0x003FED00,		//0x0D0
// 		0xA8510000, 0xFF220313, 0x00000000, 0x0000008D,		//0x0E0
// 		0x10200000, 0x00000000, 0x00000000, 0x10000000		//0x0F0
// 	};

	DWORD NVP1208_DECODER_PAL[] = 
	{
		0x00000000, 0x00000000, 0x00000000,	0x00000000,		//0x000, read only register
		0x00000000, 0x00000000, 0x00000000, 0x00000000,		//0x010, read only register
		0x00000004, 0x00000000, 0x00000000, 0x00000000,		//0x020, reserved
		0x00888802, 0x00020000, 0x00000000, 0x00000000,		//0x030
		
		0x48E4FEDC, 0x00000000, 0x00000000, 0x00000000,		//0x040
		0x00000000, 0x00000000, 0x00FF0000, 0x0000AAAA,		//0x050
		0x020002AA, 0x00000200, 0x3C550500, 0x00070000,		//0x060
		0x6C4000D0, 0x4020009F, 0x0F385080, 0x00104304,		//0x070
		
		0x00D83000, 0xB9110606, 0x800005B2, 0x37498037,		//0x080
		0xFFDFFFEF, 0x00000000, 0x800A15BD, 0x88010C80,		//0x090
		0x00000000, 0x80110180, 0x00000000, 0x03008000,		//0x0A0
		0x00230404, 0x2FF0CC2A, 0x0FD30157, 0x0080501F,		//0x0B0
		
		0x40000000, 0x000D001E, 0x09800068, 0xA243E000,		//0x0C0
		0x25170000, 0x00020200, 0x00321011,	0x003FED00,		//0x0D0
		0xA8510000, 0xFF220313, 0x00000000, 0x0000008D,		//0x0E0
		0x10200000, 0x00000000, 0x00000000, 0x10000000		//0x0F0
	};
	
	//////////////////////////////////////////////////////////////////////////
	//0x200-0x25C, PAL
	DWORD NVP1208_DECODER_PAL_2[] = 
	{
		0x80800000, 0xAAF08080, 0x1D1D1D1D, 0x00000000,		//0x200
		0x80808080, 0x80808080, 0x00000000, 0x00000000,		//0x210
		0x03030303, 0x04040404, 0x04040404, 0x00007777,		//0x220
		0x11111111, 0x54545454, 0x1E1E1E1E, 0x00000000,		//0x230
		0x0D0D0D0D, 0x00000000, 0x00000000, 0x00000000,		//0x240
		0x00000000, 0x88883210, 0x00000000, 0x88888888,		//0x250
	};


	DWORD *pTable;
	DWORD dwAddr;
	DWORD dwTableNo;
	DWORD dwData;

	if (pdx->m_byVideoFormat == VIDEO_FORMAT_NTSC)
		pTable = NVP1208_DECODER_NTSC;
	else
		pTable = NVP1208_DECODER_PAL;

	//////////////////////////////////////////////////////////////////////////
	//Set 0x020-0x0EC
	dwWriteRegister(pdx, REG_BANK_SEL, 0x00000000);

	dwAddr = 0x020;
	dwTableNo = dwAddr/sizeof(DWORD);
	for (dwAddr=0x020;dwAddr<=0x0EC;dwAddr+=sizeof(DWORD), dwTableNo++)
	{
		dwWriteRegister(pdx, dwAddr, pTable[dwTableNo]);
	}

	dwWriteRegister(pdx, REG_BANK_SEL, 0x10000000);

	if (pdx->m_byVideoFormat == VIDEO_FORMAT_NTSC)
		pTable = NVP1208_DECODER_NTSC_2;
	else
		pTable = NVP1208_DECODER_PAL_2;

	dwAddr = 0x200;
	dwTableNo = (dwAddr-0x200)/sizeof(DWORD);
	for (dwAddr=0x200; dwAddr<=0x258;dwAddr+=sizeof(DWORD), dwTableNo++)
	{
		dwWriteRegister(pdx, dwAddr, pTable[dwTableNo]);
	}

	KdPrint(("InitializeNVP1208Dec\n"));

}

void SetBrightnessIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	DWORD dwRegData;
	DWORD dwMask;

	dwMask = 0xFF<<(8*byDMAChannel);
	dwRegData = dwReadRegister(pdx, REG_BRIGHTNESS);

	dwRegData &= ~dwMask;
	dwRegData |= (pdx->Brightness[byDMAChannel]<<(8*byDMAChannel));
	dwWriteRegister(pdx, REG_BRIGHTNESS, dwRegData);

}

void SetContrastIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	DWORD dwRegData;
	DWORD dwMask;
	
	dwMask = 0xFF<<(8*byDMAChannel);
	dwRegData = dwReadRegister(pdx, REG_CONTRAST);
	
	dwRegData &= ~dwMask;
	dwRegData |= (pdx->Contrast[byDMAChannel]<<(8*byDMAChannel));
	dwWriteRegister(pdx, REG_CONTRAST, dwRegData);
}

void SetHueIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	DWORD dwRegData;
	DWORD dwMask;
	
	dwMask = 0xFF<<(8*byDMAChannel);
	dwRegData = dwReadRegister(pdx, REG_HUE);
	
	dwRegData &= ~dwMask;
	dwRegData |= (pdx->Hue[byDMAChannel]<<(8*byDMAChannel));
	dwWriteRegister(pdx, REG_HUE, dwRegData);
}

void SetSaturationIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel)
{
	DWORD dwRegData;
	DWORD dwMask;
	
	dwMask = 0xFF<<(8*byDMAChannel);
	dwRegData = dwReadRegister(pdx, REG_SATURATION);
	
	dwRegData &= ~dwMask;
	dwRegData |= (pdx->Saturation[byDMAChannel]<<(8*byDMAChannel));
	dwWriteRegister(pdx, REG_SATURATION, dwRegData);
}

void SetSharpnessIn(PDEVICE_EXTENSION pdx, BYTE byHSharpness, BYTE byVSharpness)
{
	DWORD dwRegData = 0;
	DWORD dwData;

	dwData = (byHSharpness<<4)&0xF0 | (byVSharpness&0x0F);
	dwRegData = dwData | (dwData<<8) | (dwData<<16) | (dwData<<24);
	dwWriteRegister(pdx, REG_SHARPNESS, dwRegData);
}

void SetAudioParametersIn(PDEVICE_EXTENSION pdx)
{
	DWORD dwRegData;
	dwRegData = dwReadRegister(pdx, REG_AUDIO_CTRL_PB);
	if (pdx->m_bySampleFrequency == AUDIO_FRQ_8)
	{
		dwRegData &= ~(1<<27);
	}
	else
	{
		dwRegData |= (1<<27);
	}
	dwWriteRegister(pdx, REG_AUDIO_CTRL_PB, dwRegData);

}

void SetAudioGainIn(PDEVICE_EXTENSION pdx, BYTE byAudioChannel, BYTE byGain, BYTE byMute)
{
	DWORD dwRegData;
	DWORD dwGain;
	DWORD shift;
	dwRegData = dwReadRegister(pdx, REG_AUDIO_GAIN);

	//////////////////////////////////////////////////////////////////////////
	//byAudioChannel is one of 0~3
	
	shift = byAudioChannel*4;
	if (byMute)
		dwGain = 0;
	else
		dwGain = byGain&0x0F;
	dwGain <<= 8;
	dwRegData &= ~(0xF00<<shift);
	dwRegData |= dwGain<<shift;

	dwWriteRegister(pdx, REG_AUDIO_GAIN, dwRegData);

}
