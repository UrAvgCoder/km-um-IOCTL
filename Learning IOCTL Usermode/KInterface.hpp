#pragma once
#include "definitions.hpp"

namespace KernelInterface {
	class KInterface {
	public:
		HANDLE hDevice = NULL;
		KInterface(LPCSTR RegistryPath) {
			hDevice = CreateFile(RegistryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
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

		template<typename t>
		t read_memory(ULONG process_id, ULONG address) {
			SKernelReadData info = { 0 };
			info.pid = process_id;
			info.address = address;
			info.size = sizeof(t);
			auto bytesReceived = 0;
			if (DeviceIoControl(hDevice, READ_DATA, &info, sizeof(info), &info, sizeof(info), &bytesReceived, NULL)) {
				return (t)info.buffer;			
			}
			return 0;
		}

		template<typename t>
		t write_memory(ULONG process_id, ULONG address, t buffer) {
			SKernelWriteData info = { 0 };
			info.pid = process_id;
			info.address = address;
			info.buffer = buffer;
			info.size = sizeof(t);
			auto bytesReceived = 0;
			DeviceIoControl(hDevice, WRITE_DATA, &info, sizeof(info), &info, sizeof(info), &bytesReceived, NULL)
		}
	};
}
