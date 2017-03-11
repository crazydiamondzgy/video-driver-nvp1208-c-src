#ifdef _WIN64
#include	<wdm.h>
#ifndef DEVICE_DRIVER
#define DEVICE_DRIVER
#endif
#else
#include	<ntddk.h>
#endif
#include	<stdio.h>
#include	"..\\common\\ioctl.h"        // Get IOCTL interface definitions
#include	"..\\common\\Constant.h"
#include	"..\\Common\\DefineBase.h"

typedef unsigned long		DWORD;
typedef unsigned short		WORD;
typedef unsigned char		BYTE;
typedef unsigned int		UINT;
typedef BOOLEAN				BOOL;

typedef volatile unsigned long ULONG_REG, *PULONG_REG;

#include "..\\Common\\DefineStruct.h"

#if     !defined(__NVP1208__)
#define __NVP1208_H__

#define NT_DEVICE_NAME	L"\\Device\\NVP1208%d"
#define DOS_DEVICE_NAME L"\\DosDevices\\NVP1208%d"
			
// driver local data structure specific to each device object
typedef struct _DEVICE_EXTENSION 
{
	PDEVICE_OBJECT		m_pLowerDeviceObject;		// The top of the stack
	PDEVICE_OBJECT		m_pPdo;						// the PDO

	IO_REMOVE_LOCK		m_RemoveLock;				// removal control locking structure	
	UNICODE_STRING		m_win32DeviceName;
	
	PDMA_ADAPTER		m_pAdapterObject;			// DMA adapter object
	PKINTERRUPT			m_pInterruptObject;			// interrupt object
	KDPC				m_DpcObjectVCap;
	KDPC				m_DpcObjectAudio;
	KDPC				m_DpcObjectVLive;

	PKEVENT				m_pVCapEvent;
	PKEVENT				m_pAudioEvent;
	PKEVENT				m_pVLiveEvent;
	
	LARGE_INTEGER		m_liFrequency;
	LARGE_INTEGER		m_liStartCounter;
	LARGE_INTEGER		m_liStartLocalTime;
	LARGE_INTEGER		m_liIntCounter;
	LARGE_INTEGER		m_liIntLocalTime;
	LARGE_INTEGER		m_liDiffCounter;
	
	PHYSICAL_ADDRESS	m_PhysDisplayMemBase;

	//////////////////////////////////////////////////////////////////////////
	//DMA memory
	ST_DMA_MEMORY	stDMAVideoBuffer[MAX_DMA_PATH_VCAP];
	ST_DMA_MEMORY	stDMAAudioBuffer;
	ST_DMA_MEMORY	stDMALiveBuffer;

    //////////////////////////////////////////////////////////////////////////
	// device
	BYTE		m_byDeviceID;				// DeviceID.
	BOOL		m_bIrqshare;
	BOOL		m_bStarted;
	BOOL		m_bRemoved;
	BOOL		m_bPortWasMapped;
	BOOL		m_bBufferAllocate;
	BOOL		m_bMDLAllocate;

	BYTE		*m_pbyMembase;				// mapped memory base address
	DWORD		m_dwMemsize;				// size of mapped memory area

	DWORD		m_dwPhysDisplayAddress;
	BYTE		m_byDecSection;
	BYTE		byDevAddr[EXT_DECODER_COUNT];

	//////////////////////////////////////////////////////////////////////////
	// result
	VCAP_STATUS_INFO	*m_pVCAPStatus[MAX_DMA_PATH_VCAP][MAX_VIDEO_QUEUE];
	VCAP_STATUS_INFO	*m_pVLiveStatus[MAX_VIDEO_QUEUE];
	ACAP_STATUS_INFO	*m_pACAPStatus[MAX_AUDIO_QUEUE];

	BYTE		*m_pVCapCurQCnt[MAX_DMA_PATH_VCAP];
	BYTE		*m_pVLiveCurQCnt;
	BYTE		*m_pACapCurQCnt;
	//////////////////////////////////////////////////////////////////////////
	//DMA and Interrupt
	//Video
	BYTE		m_byVideoDMA[MAX_DMA_PATH_VCAP];
	BYTE		m_byLiveDMA;
	BYTE		m_byAudioDMA;


	DWORD		m_dwVideoDMAEnable;		//this is for register setting of enabling DMA
	DWORD		m_dwVideoIrqEnable;		//this is for register setting of enabling interrupt
	DWORD		m_dwVideoDMAMask[MAX_DMA_PATH_VCAP+LIVE_VDEC_PATH];		//this is for keeping initial status of enabling DMA when capture starts
	DWORD		m_dwVideoIrqMask[MAX_DMA_PATH_VCAP+LIVE_VDEC_PATH];		//this is for keeping initial status of enabling interrupt when capture starts
	//Audio
	DWORD		m_dwAudioDMAEnable;
	DWORD		m_dwAudioIrqEnable;

	//////////////////////////////////////////////////////////////////////////
	//Video capture property
	BYTE		m_byVideoFormat;			//NTSC or PAL
	DWORD		m_dwHeightIndication;
	BYTE		m_byVideoOutputFormat;

	ST_IMAGE_SIZE		m_stVCapSize[MAX_DMA_PATH_VCAP];
	ST_IMAGE_SIZE		m_stVLiveSize;

	BYTE		m_byVLiveChannel;

	//Audio property
	BYTE		m_bySampleFrequency;

	//////////////////////////////////////////////////////////////////////////
	//capture start/stop
	BYTE byStartFlag;
	BYTE byStopFlag;

	BYTE byVCapQueueCount[MAX_DMA_PATH_VCAP];
	BYTE byAudioQueueCount;
	BYTE byVLiveQueueCount;

	//////////////////////////////////////////////////////////////////////////
	//Color
	BYTE Brightness[MAX_DMA_PATH_VCAP];
	BYTE Contrast[MAX_DMA_PATH_VCAP];
	BYTE Hue[MAX_DMA_PATH_VCAP];
	BYTE Saturation[MAX_DMA_PATH_VCAP];
	//////////////////////////////////////////////////////////////////////////

	BYTE bVideoSync[MAX_DMA_PATH_VCAP];

	BYTE m_byRecTable[MAX_DMA_PATH_VCAP+LIVE_VDEC_PATH][MAX_CHANNEL_CHANGEABLE];
	BYTE m_byRecTableLen[MAX_DMA_PATH_VCAP+LIVE_VDEC_PATH];

	BYTE byCurTablePos[MAX_DMA_PATH_VCAP+LIVE_VDEC_PATH];

	BOOL fQueueFullCondition[MAX_DMA_PATH_VCAP];

	DWORD m_dwBurstSize;

	LARGE_INTEGER liSysFileTime, liLocalFileTime;
	LARGE_INTEGER liPreLocalTime;

	DWORD dwTick;
	DWORD dwPreTick;

	//////////////////////////////////////////////////////////////////////////
	//I2C
	FAST_MUTEX			m_fmutexI2C;
	KDPC				m_DpcObjectI2C;
	KEVENT				m_kI2CEvent;

	//////////////////////////////////////////////////////////////////////////
	//Test
	DWORD dwIntCnt_even;
	DWORD dwIntCnt_odd;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define arraysize(p) (sizeof(p)/sizeof((p)[0]))

#if DBG
#define DebugPrint(_x_) DbgPrint ("NVP1208:"); DbgPrint _x_;
#define TRAP() DbgBreakPoint()
#else
#define DebugPrint(_x_)
#define TRAP()
#endif

//////////////////////////////////////////////////////////////////////////
//associated with WDM
//DriverEntry
NTSTATUS	DriverEntry(IN  PDRIVER_OBJECT DriverObject, IN  PUNICODE_STRING RegistryPath);
//AddDevice
NTSTATUS	AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject);
//Dispatch Routines
NTSTATUS	DispatchCreate(IN PDEVICE_OBJECT pDO, IN PIRP pIrp);
NTSTATUS	DispatchClose(IN PDEVICE_OBJECT pDO, IN PIRP pIrp);
NTSTATUS	DispatchWrite(IN PDEVICE_OBJECT pDO, IN PIRP pIrp);
NTSTATUS	DispatchControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS	DispatchPnp(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	DispatchPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
//PNP
NTSTATUS	StartDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	StopDevice(IN PDEVICE_OBJECT pDeviceObject, IN PIRP Irp);
//Unload
VOID		Unload(IN PDRIVER_OBJECT DriverObject);
//Completion
NTSTATUS	CompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);
NTSTATUS	CompleteRequest(IN PIRP pIrp, IN NTSTATUS status, IN ULONG_PTR info);

//Debugging PNP
PCHAR		PnPMinorFunctionString(BYTE MinorFunction);

//////////////////////////////////////////////////////////////////////////
//synchronize event
NTSTATUS SynchronizeEvent(PIRP pIrp, HANDLE hRing3Event, PKEVENT *ppkRing0Event);
VOID DereferenceEvent(PKEVENT pKEvent);

//////////////////////////////////////////////////////////////////////////
//associated with DMA buffer
NTSTATUS	DmaMemAllocPool(PDEVICE_OBJECT pDeviceObject);
VOID		DmaMemFreePool(PDEVICE_OBJECT pDeviceObject);
NTSTATUS	MDLAllocateCapBuf(PDEVICE_OBJECT pDeviceObject);
VOID		MDLFreeCapBuf(PDEVICE_OBJECT pDeviceObject);
NTSTATUS	KKGetPhysicalAddress(IN unsigned char *pR0Address, OUT PHYSICAL_ADDRESS *PhysAddress);

//////////////////////////////////////////////////////////////////////////
//ISR
VOID		DpcForIsr_VCap (PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx);
VOID		DpcForIsr_Audio(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx);
VOID		DpcForIsr_VLive(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx);
VOID		DpcForIsr_I2C(PKDPC Dpc, PDEVICE_OBJECT pDeviceObject, PIRP junk, PDEVICE_EXTENSION pdx);

BOOL		OnInterrupt(PKINTERRUPT pInterruptObject, PDEVICE_EXTENSION pdx);
BOOL		OnInterrupt_Audio(PDEVICE_EXTENSION pdx, DWORD dwInterruptStatus);
BOOL		OnInterrupt_VCap(PDEVICE_EXTENSION pdx, DWORD dwInterruptStatus);
BOOL		OnInterrupt_Live(PDEVICE_EXTENSION pdx);

VOID		VideoDMAStart(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
VOID		VideoDMAStop(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//register access
VOID		dwWriteRegister(PDEVICE_EXTENSION pdx, DWORD dwOffset, DWORD dwValue);
VOID		dwWriteRegister2(PDEVICE_EXTENSION pdx, DWORD dwOffset, DWORD dwValue);
VOID		dwWriteRegisterIrq(PDEVICE_EXTENSION pdx, DWORD dwOffset, DWORD dwValue);
DWORD		dwReadRegister(PDEVICE_EXTENSION pdx, DWORD dwOffset);

//////////////////////////////////////////////////////////////////////////
// Initialize
VOID		MemberVariableInit(PDEVICE_EXTENSION pdx);
VOID		Initialize(PDEVICE_EXTENSION pdx);
BOOL 		LocalTimeInit(PDEVICE_EXTENSION pdx);
VOID		AllDMADisable(PDEVICE_EXTENSION pdx);
BYTE		GetDecoderSection(PDEVICE_EXTENSION pdx);


//////////////////////////////////////////////////////////////////////////
//I2C
BOOL		I2C_Init(PDEVICE_EXTENSION pdx);
BOOL		I2C_Read(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData);
BOOL		I2C_Write(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE bData);
BOOL		I2C_ReadBurst(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength);
BOOL		I2C_WriteBurst(PDEVICE_EXTENSION pdx, BYTE bPort, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength);

BOOL		I2C_P0_ReadBurst(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength);
BOOL		I2C_P0_WriteBurst(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData, DWORD dwLength);

BOOL		I2C_P0_Read(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData);
BOOL		I2C_P0_Write(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE bData);

void		I2C_P0_Start(PDEVICE_EXTENSION pdx);
void		I2C_P0_Stop(PDEVICE_EXTENSION pdx);
void		I2C_P0_SetSDAMode(PDEVICE_EXTENSION pdx, DWORD dwMode);
void		I2C_P0_SetSDA(PDEVICE_EXTENSION pdx, DWORD dwSig);
BYTE		I2C_P0_GetSDA(PDEVICE_EXTENSION pdx);
void		I2C_P0_SetSCL(PDEVICE_EXTENSION pdx, DWORD dwSig);
void		I2C_P0_TransmitByte(PDEVICE_EXTENSION pdx, BYTE bData);
BOOL		I2C_P0_ReadAck(PDEVICE_EXTENSION pdx);
BYTE		I2C_P0_ReceiveByte(PDEVICE_EXTENSION pdx);
void		I2C_P0_SendOutAck(PDEVICE_EXTENSION pdx, DWORD dwSig);

BOOL		I2C_P1_Read(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE *bData);
BOOL		I2C_P1_Write(PDEVICE_EXTENSION pdx, BYTE bDevAddr, BYTE bSubAddr, BYTE bData);

void		I2C_P1_Start(PDEVICE_EXTENSION pdx);
void		I2C_P1_Stop(PDEVICE_EXTENSION pdx);
void		I2C_P1_SetSDAMode(PDEVICE_EXTENSION pdx, DWORD dwMode);
void		I2C_P1_SetSDA(PDEVICE_EXTENSION pdx, DWORD dwSig);
BYTE		I2C_P1_GetSDA(PDEVICE_EXTENSION pdx);
void		I2C_P1_SetSCL(PDEVICE_EXTENSION pdx, DWORD dwSig);
void		I2C_P1_TransmitByte(PDEVICE_EXTENSION pdx, BYTE bData);
BOOL		I2C_P1_ReadAck(PDEVICE_EXTENSION pdx);
BYTE		I2C_P1_ReceiveByte(PDEVICE_EXTENSION pdx);
void		I2C_P1_SendOutAck(PDEVICE_EXTENSION pdx, DWORD dwSig);

//////////////////////////////////////////////////////////////////////////
//basic function
DWORD		GetTick();
DWORD		GetTimeInterval(DWORD dwPrevTime, DWORD dwCurrTime);
VOID		GetCurrentSystemTime(DWORD* pdwHiTime, DWORD* pdwLowTime);
VOID		DelayPer100nanoseconds(PDEVICE_EXTENSION pdx, DWORD DelayTime);
VOID		DelayMicroSecond(int mSec);
VOID		Hold(DWORD uSec);
//basic function for NVP1208
VOID		GetScaleInfo(BYTE byVideoFormat, LPST_IMAGE_SIZE lpstImageSize, BOOL fExternal);
BYTE		GetDataFormat(BYTE byDataFormat);
DWORD		GetHeightIndication(BYTE byVideoFormat);
//////////////////////////////////////////////////////////////////////////
// Decoder setting
VOID		Decoder_Init(PDEVICE_EXTENSION pdx);
VOID		External_Decoder_Init(PDEVICE_EXTENSION pdx);
VOID		Chip_Reset(PDEVICE_EXTENSION pdx, DWORD dwReset);
VOID		SetExtenalVideoScaler(PDEVICE_EXTENSION pdx, BYTE byDMAChannel, LPST_IMAGE_SIZE lpstImageSize);

VOID		SetDecoderAFEMode(PDEVICE_EXTENSION pdx);

//////////////////////////////////////////////////////////////////////////
// capture start/stop
VOID		SetLocalRegister(PDEVICE_EXTENSION pdx);
VOID		SetCaptureStart(PDEVICE_EXTENSION pdx);
VOID		SetCaptureStop(PDEVICE_EXTENSION pdx);

VOID		SetVideoLiveChannel(PDEVICE_EXTENSION pdx, BYTE byVLiveChannel);

VOID		CheckVideoSync(PDEVICE_EXTENSION pdx);

//////////////////////////////////////////////////////////////////////////
//
VOID SetVideoDMATargetAddress(PDEVICE_EXTENSION pdx, BYTE byDMAChannel, BYTE byCurQCnt);
VOID SetVLiveDMATargetAddress(PDEVICE_EXTENSION pdx, BYTE byCurQCnt);
VOID SetVCapSizeInfo(PDEVICE_EXTENSION pdx, BYTE byDMAChannel, LPST_IMAGE_SIZE lpstImageSize, BOOL fSetExternal);
VOID SetVLiveSizeInfo(PDEVICE_EXTENSION pdx, LPST_IMAGE_SIZE lpstImageSize);
VOID SetAudioDMATargetAddress(PDEVICE_EXTENSION pdx, BYTE bPath, BYTE byCurQCnt);

//////////////////////////////////////////////////////////////////////////
//NVP1104A.c
void InitializeNVP1104A(PDEVICE_EXTENSION pdx, BYTE bI2CPort, BYTE bDeviceAddr, BYTE bAudioStage);
void SetScaler(PDEVICE_EXTENSION pdx, BYTE bI2CPort, BYTE bDevAddr, BYTE bDecoder, DWORD H_DTO, DWORD V_DTO);
void SetBrightnessEx(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);		//byExternalChannel 0 to 11
void SetContrastEx(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetHueEx(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetSaturationEx(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetBlacknWhiteEx(PDEVICE_EXTENSION pdx, BYTE byExternalChannel);
void SetSharpnessEx(PDEVICE_EXTENSION pdx, BYTE byHSharpness, BYTE byVSharpness);
void SetAudioParametersEx(PDEVICE_EXTENSION pdx);
void SetAudioGainEx(PDEVICE_EXTENSION pdx, BYTE byAudioChannel, BYTE byGain, BYTE byMute);

void InitializeNVP1104A_AFE(PDEVICE_EXTENSION pdx, BYTE bI2CPort, BYTE bDeviceAddr);
//////////////////////////////////////////////////////////////////////////
//NVP1208Decoder.c
//NVP1208 decoder part
void InitializeNVP1208Dec(PDEVICE_EXTENSION pdx);

void SetBrightnessIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetContrastIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetHueIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetSaturationIn(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);

void SetSharpnessIn(PDEVICE_EXTENSION pdx, BYTE byHSharpness, BYTE byVSharpness);
void SetAudioParametersIn(PDEVICE_EXTENSION pdx);
void SetAudioGainIn(PDEVICE_EXTENSION pdx, BYTE byAudioChannel, BYTE byGain, BYTE byMute);

//////////////////////////////////////////////////////////////////////////
void SetBrightness(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetContrast(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetHue(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);
void SetSaturation(PDEVICE_EXTENSION pdx, BYTE byDMAChannel);


#endif


