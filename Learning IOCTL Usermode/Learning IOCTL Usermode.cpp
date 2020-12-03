#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <TlHelp32.h>
#include "KInterface.h"

struct HandleDisposer {
	using pointer = HANDLE;
	void operator()(HANDLE handle) const {
		if (handle != NULL || handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
	}
};

using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

std::uint32_t get_process_id(std::string_view process_name) {
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	processentry.dwSize = sizeof(MODULEENTRY32);
	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
		if (process_name.compare(processentry.szExeFile) == NULL) {
			return processentry.th32ProcessID;
		}
	}
}

int main()
{
	KInterface hDevice = KInterface("\\\\.\\learningioctl");
	std::cout << "IOCTL communication with Kernel Driver" << std::endl;
	int a = 2000;
	int b = 1001;

	std::cout << hDevice.DoSum(a, b) << std::endl;
	std::cout << "Process ID: " << get_process_id("csgo.exe") << std::endl;


	pKernelSumIoData pData;

	ULONG base_addr = hDevice.get_base(get_process_id("csgo.exe"), "client.dll");
	//std::cout << "csgo base found at: " << base_addr << std::endl;
	printf("csgo base: %llu", base_addr);
	std::cin.get();
	return 0;
}

