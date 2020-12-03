#pragma once

#include "ntapi.hpp"

#define SUM_IO_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_SPECIAL_ACCESS)
#define REQ_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_SPECIAL_ACCESS)
#define READ_DATA CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_NEITHER, FILE_SPECIAL_ACCESS)
#define WRITE_DATA CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_NEITHER, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_IO_SUM_DATA {
	INT iNumberFirst;
	INT iNumberSecond;
	INT iResult;
} SKernelSumIoData, * pKernelSumIoData;


typedef struct _KERNEL_IO_RECV_BASE {
	ULONG base_addr;
	DWORD pid;
	const char* module_name;
} SKernelRecvBase, * pKernelRecvBase;

typedef struct _KERNEL_READ_DATA {
	ULONG pid;
	ULONG address;
	ULONG buffer;
	ULONG size;
} SKernelReadData, * pKernelReadData;

typedef struct _KERNEL_WRITE_DATA {
	ULONG pid;
	ULONG address;
	ULONG buffer;
	ULONG size;
} SKernelWriteData, * pKernelWriteData;