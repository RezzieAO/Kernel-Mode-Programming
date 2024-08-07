#pragma warning (disable : 4100)

#include <ntifs.h>
#include <ntstrsafe.h>
#include <ntddk.h>
#include <ntdef.h>

#define  BUFFER_SIZE 30


NTSTATUS UnloadDriver(_In_ PDRIVER_OBJECT pDriverObject)
{
    
    DbgPrint("Unloading");

    return STATUS_SUCCESS;
}



extern "C"
{
    DRIVER_INITIALIZE DriverEntry;
    _Use_decl_annotations_ NTSTATUS DriverEntry( struct _DRIVER_OBJECT  *DriverObject, PUNICODE_STRING  RegistryPath)
    {
        DriverObject->DriverUnload = (PDRIVER_UNLOAD) UnloadDriver;
        
        DbgPrint("DriverEntry");
        
		
        NTSTATUS status = NULL;
        NTSTATUS ExitStatus = NULL;
        OBJECT_ATTRIBUTES objAttribs;
        HANDLE processHandle = NULL;
        int PID = 12612;
        CLIENT_ID clientId;

        InitializeObjectAttributes(&objAttribs,
            NULL,
            OBJ_KERNEL_HANDLE,
            NULL,
            NULL);


        clientId.UniqueProcess = (HANDLE)PID;
        clientId.UniqueThread = NULL;

        status = ZwOpenProcess(&processHandle, PROCESS_ALL_ACCESS, &objAttribs, &clientId);

        

        ExitStatus = ZwTerminateProcess(processHandle, ExitStatus);

        ZwClose(processHandle);
        
        
        return STATUS_SUCCESS;
    }
}



