#pragma once
#include <Windows.h>

#define SUM_IO_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_SPECIAL_ACCESS)
#define REQ_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_IO_SUM_DATA {
	INT iNumberFirst;
	INT iNumberSecond;
	INT iResult;
} SKernelSumIoData, * pKernelSumIoData;

typedef struct _KERNEL_IO_RECV_BASE {
	ULONG base_addr;
	DWORD pid;
	const char* module_name;
} SKernelRecvBase, *pKernelRecvBase;


class KInterface {
public:
	HANDLE hDevice = NULL;
	KInterface(LPCSTR RegistryPath) {
		hDevice = CreateFile(RegistryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	}

	INT DoSum(int a, int b) {

		SKernelSumIoData pObject;
		pObject.iNumberFirst = a;
		pObject.iNumberSecond = b;
		pObject.iResult = 0;

		auto bytesReturned = 0UL;
		if (DeviceIoControl(hDevice, SUM_IO_CODE, &pObject, sizeof(pObject), &pObject, sizeof(pObject), &bytesReturned, NULL)) {
			return pObject.iResult;
		}

		return 0;
	}

	ULONG get_base(uint32_t pid, const char* module_name) {
		auto bytesReceived = 0UL;
		SKernelRecvBase pData = { 0 };
		pData.module_name = module_name;
		pData.pid = pid;
		if (DeviceIoControl(hDevice, REQ_BASE, &pData, sizeof(pData), &pData, sizeof(pData), &bytesReceived, NULL)) {
			return pData.base_addr;
		}
		return 0;
	}
	
};