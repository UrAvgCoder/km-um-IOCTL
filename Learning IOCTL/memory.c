#include <ntifs.h>
#include "memory.h"
#include "includes.h"


ULONG get_base_addr64(SKernelRecvBase pData) 
{
    ULONG64 base = 0;
    ANSI_STRING AS;
    UNICODE_STRING ModuleName;

    PEPROCESS proc = NULL;
    PsLookupProcessByProcessId((HANDLE)pData.pid, &proc);

    RtlInitAnsiString(&AS, pData.module_name);
    RtlAnsiStringToUnicodeString(&ModuleName, &AS, TRUE); 
    
    DbgPrintEx(0, 0, "Module Name in func %wZ", ModuleName);
    DbgPrintEx(0, 0, "PID: %d", pData.pid);
    //DbgPrintEx(0, 0, "PID in func %wZ\n", pData.pid);

    PPEB32 pPeb = (PPEB32)PsGetProcessWow64Process(proc);
    if (!pPeb) {
        DbgPrintEx(0, 0, "pPeb failed\n");
        return 0;
    }

    KAPC_STATE state;

    KeStackAttachProcess(proc, &state);

    PPEB_LDR_DATA32 pLdr = (PPEB_LDR_DATA32)pPeb->Ldr;

    if (!pLdr) {
        DbgPrintEx(0, 0, "pLdr failed\n");
        KeUnstackDetachProcess(&state);
        return 0;
    }

    for (PLIST_ENTRY32 list = (PLIST_ENTRY32)pLdr->InLoadOrderModuleList.Flink;
        list != &pLdr->InLoadOrderModuleList; list = (PLIST_ENTRY32)list->Flink) {
        PLDR_DATA_TABLE_ENTRY32 pEntry =
            CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
        UNICODE_STRING DLLname;
        DLLname.Length = pEntry->BaseDllName.Length;
        DLLname.MaximumLength = pEntry->BaseDllName.MaximumLength;
        DLLname.Buffer = (PWCH)pEntry->BaseDllName.Buffer;
        
        //RtlInitUnicodeString(&DLLname, &pEntry->BaseDllName);
        if (RtlCompareUnicodeString(&DLLname, &ModuleName, TRUE) == 0) {
            base = pEntry->DllBase;
            KeUnstackDetachProcess(&state);
            return base;
        }
    }
    KeUnstackDetachProcess(&state);
    return 0;
}

