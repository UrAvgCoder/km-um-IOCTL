#pragma once
#include "definitions.hpp"

namespace memory {
	ULONG get_module_base(SKernelRecvBase);

	NTSTATUS read_memory(PEPROCESS target_process, void* source, void* target, size_t size);

	NTSTATUS write_memory(PEPROCESS target_process, void* source, void* target, size_t size);
}
