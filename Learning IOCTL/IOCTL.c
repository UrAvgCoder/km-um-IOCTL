#include <ntifs.h>
#include "memory.h"
#include "includes.h"

#define SUM_IO_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_SPECIAL_ACCESS)
#define REQ_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_SPECIAL_ACCESS)

UNICODE_STRING device_name, symlink_name, driver_name;

#define IO_INPUT(Type) ((Type)(pStack->Parameters.DeviceIoControl.Type3InputBuffer))
#define IO_OUTPUT(Type) ((Type)(pIrp->UserBuffer))


NTSTATUS OnIoControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	UNREFERENCED_PARAMETER(pDeviceObject);

	DbgPrintEx(0, 0, "IoControl handled\n");

	NTSTATUS ntStatus = STATUS_SUCCESS;
	__try {
		PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
		switch (pStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case SUM_IO_CODE: {
			DbgPrintEx(0, 0, "Sum IOCTL received\n");

			IO_OUTPUT(pKernelSumIoData)->iResult = IO_INPUT(pKernelSumIoData)->iNumberFirst + IO_INPUT(pKernelSumIoData)->iNumberSecond;
			pIrp->IoStatus.Information = sizeof(SKernelSumIoData);
		}
		break;

		case REQ_BASE: {
			DbgPrintEx(0, 0, "Request Base IOCTL received\n");
			SKernelRecvBase obj = { 0 };
			obj.pid = IO_INPUT(pKernelRecvBase)->pid;
			obj.module_name = IO_INPUT(pKernelRecvBase)->module_name;
			IO_OUTPUT(pKernelRecvBase)->base_addr = get_base_addr64(obj);
			pIrp->IoStatus.Information = sizeof(SKernelRecvBase);
		}
		break;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		ntStatus = STATUS_UNSUCCESSFUL;
		DbgPrintEx(0, 0, "OnIoControl Exception cought\n");
	}

	pIrp->IoStatus.Status = ntStatus;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return ntStatus;
}

NTSTATUS OnMajorFunctionCall(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);

	switch (pStack->MajorFunction) {
	case IRP_MJ_DEVICE_CONTROL:
		OnIoControl(pDeviceObject, pIrp);
		break;
	default:
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}
	return STATUS_SUCCESS;
}

NTSTATUS OnDriverUnload(IN PDRIVER_OBJECT pDriverObject) {
	UNREFERENCED_PARAMETER(pDriverObject);

	DbgPrintEx(0, 0, "unload routine called\n");

	IoDeleteDevice(pDriverObject->DeviceObject);
	IoDeleteSymbolicLink(&symlink_name);

	return STATUS_SUCCESS;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	//UNREFERENCED_PARAMETER(pRegistryPath);
	//UNREFERENCED_PARAMETER(pDriverObject);

	//RtlInitUnicodeString(&driver_name, L"\\Device\\learningioctldriver");
	//IoCreateDriver(&driver_name, &DriverEntryTest);
	//return STATUS_SUCCESS;

	DbgPrintEx(0, 0, "Driver loaded, system range start in %p, our entry at: %p\n", MmSystemRangeStart, DriverEntry);

	pDriverObject->DriverUnload = &OnDriverUnload;

	NTSTATUS ntStatus = 0;

	RtlInitUnicodeString(&device_name, L"\\Device\\learningioctl");
	RtlInitUnicodeString(&symlink_name, L"\\??\\learningioctl");

	PDEVICE_OBJECT pDeviceObject = NULL;
	ntStatus = IoCreateDevice(pDriverObject, 0, &device_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (!NT_SUCCESS(ntStatus)) {
		DbgPrintEx(0, 0, "IoCreateDevice failed, status: %p\n", ntStatus);
		return ntStatus;
	}

	ntStatus = IoCreateSymbolicLink(&symlink_name, &device_name);
	if (!NT_SUCCESS(ntStatus)) {
		DbgPrintEx(0, 0, "IoCreateSymbolicLink failed, status: %p\n", ntStatus);
		return ntStatus;
	}

	for (ULONG t = 0; t < IRP_MJ_MAXIMUM_FUNCTION; t++) {
		pDriverObject->MajorFunction[t] = &OnMajorFunctionCall;
	}

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	DbgPrintEx(0, 0, "Driver Entry Completed\n");

	return STATUS_SUCCESS;
}

//NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
//	UNREFERENCED_PARAMETER(pRegistryPath);
//	UNREFERENCED_PARAMETER(pDriverObject);
//
//	RtlInitUnicodeString(&driver_name, L"\\Device\\learningioctldriver");
//	IoCreateDriver(&driver_name, &DriverEntryTest);
//	return STATUS_SUCCESS;
//}
