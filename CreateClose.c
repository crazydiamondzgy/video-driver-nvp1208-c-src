#include "NVP1208.h"

extern LONG		g_dwDeviceCount;      // Device Number

//=========================================================================================================================//
NTSTATUS DispatchCreate(IN PDEVICE_OBJECT pDO, IN PIRP pIrp)
{
	PDEVICE_EXTENSION	pdx;
	PIO_STACK_LOCATION	pIrpStack;
	NTSTATUS			Status;
	DWORD				i = 0, j, n, dwSystemTime;
	LARGE_INTEGER		liSystemTime, liLocalTime;
	
	pIrp->IoStatus.Information = 0;
	pdx = (PDEVICE_EXTENSION)pDO->DeviceExtension;    // Get local info struct
	KdPrint(("DispatchCreate::total device count = %d, current device number = %d\n", g_dwDeviceCount, pdx->m_byDeviceID));

	Status = IoAcquireRemoveLock(&pdx->m_RemoveLock, pIrp);
	if(!NT_SUCCESS(Status))
		return CompleteRequest(pIrp, Status, 0);
	
	if(!pdx->m_bStarted)
	{
		// We fail all the IRPs that arrive before the device is started.
		pIrp->IoStatus.Status = Status = STATUS_DEVICE_NOT_READY;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		IoReleaseRemoveLock(&pdx->m_RemoveLock, pIrp);
		return Status;
	}
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	AllDMADisable(pdx);

	Status = MDLAllocateCapBuf(pDO);

	I2C_Init(pdx);		//I2C init

	pIrp->IoStatus.Status = Status;
	// Don't boost priority when returning since this took little time.
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	IoReleaseRemoveLock(&pdx->m_RemoveLock, pIrp);

	//KdPrint(("End of DispatchCreate\n"));
	return Status;
}

//////////////////////////////////////////////////////////////////////////
NTSTATUS DispatchClose(IN PDEVICE_OBJECT pDO, IN PIRP pIrp)
{
	PDEVICE_EXTENSION		pdx;
	PIO_STACK_LOCATION		pIrpStack;
	NTSTATUS				Status;
	
	KdPrint(("DispatchClose\n"));
	pIrp->IoStatus.Information = 0;
	pdx = (PDEVICE_EXTENSION)pDO->DeviceExtension;    // Get local info struct
	Status = IoAcquireRemoveLock(&pdx->m_RemoveLock, pIrp);
	if(!NT_SUCCESS (Status)) 
		return CompleteRequest(pIrp, Status, 0);
	
	if(!pdx->m_bStarted) 
	{
		// We fail all the IRPs that arrive before the device is started.
		pIrp->IoStatus.Status = Status = STATUS_DEVICE_NOT_READY;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		IoReleaseRemoveLock(&pdx->m_RemoveLock, pIrp);       
		return Status;
	}
	
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	
	AllDMADisable(pdx);
	MDLFreeCapBuf(pDO);
	
	// We're done with I/O request.  Record the Status of the I/O action.
	pIrp->IoStatus.Status = Status;
	
	// Don't boost priority when returning since this took little time.
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	IoReleaseRemoveLock(&pdx->m_RemoveLock, pIrp);
	return Status;
}

//////////////////////////////////////////////////////////////////////////
NTSTATUS DispatchWrite(PDEVICE_OBJECT fdo, PIRP Irp)
{
	PDEVICE_EXTENSION		pdx = (PDEVICE_EXTENSION)fdo->DeviceExtension;	
	BYTE					*R0Address;
	PIO_STACK_LOCATION		stack = IoGetCurrentIrpStackLocation(Irp);

	R0Address = (BYTE *)MmGetMdlVirtualAddress(Irp->MdlAddress);
	if(R0Address != NULL)
		KKGetPhysicalAddress(R0Address, &pdx->m_PhysDisplayMemBase);

	pdx->m_dwPhysDisplayAddress = (DWORD)pdx->m_PhysDisplayMemBase.LowPart;
	KdPrint(("DispatchWrite, pdx->m_dwPhysDisplayAddress[%X], stack->Parameters.Write.Length = %X\n", pdx->m_dwPhysDisplayAddress, stack->Parameters.Write.Length));
	return CompleteRequest(Irp, STATUS_SUCCESS, 0);
}

