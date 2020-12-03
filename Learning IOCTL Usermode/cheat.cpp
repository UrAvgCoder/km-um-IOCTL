#include "cheat.hpp"

void cheat::run()
{
	KernelInterface::KInterface hDevice = KernelInterface::KInterface("\\\\.\\learningioctl");
	printf("[kernel-csgo] Looking for csgo.exe process..\n");
	ULONG process_id = memory::get_process_id_by_name("csgo.exe");
	if (process_id) {
		printf("[kernel-csgo] Process identifier found: %d\n", process_id);
		printf("[kernel-csgo] Looking for client.dll...\n");
		ULONG client_base = hDevice.get_base(process_id, "client.dll");
		if (client_base)
		{
			printf("[kernel-csgo] client.dll found: 0x%x\n", client_base);
			printf("[kernel-csgo] Running...\n");
			for (;;) {

			}
		}
		else {
			printf("[kernel-csgo] Failed to obtain client.dll\n");
		}
	}
	else {
		printf("[kernel-csgo] Failed to find csgo.exe process\n");
	}
}
