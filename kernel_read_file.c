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
        
        
        UNICODE_STRING     uniName; // buffer containing file path
        OBJECT_ATTRIBUTES  objAttr; // struct of attributes related to the file

        RtlInitUnicodeString(&uniName, L"\\DosDevices\\C:\\example.txt"); // initializing unicode string - uniName = L"\\DosDevices\\C:\\example.txt"
       
       // initializing kernel object - handle accessed in kernel mode
       InitializeObjectAttributes(&objAttr, &uniName, 
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL, NULL);


        HANDLE   handle; // the handle to the file
        NTSTATUS ntstatus;
        IO_STATUS_BLOCK ioStatusBlock;

        // Do not try to perform any file operations at higher IRQL levels.
        // Instead, you may use a work item or a system worker thread to perform file operations.

        if (KeGetCurrentIrql() != PASSIVE_LEVEL)
            return STATUS_INVALID_DEVICE_STATE;

         LARGE_INTEGER byteOffset;

		ntstatus = ZwCreateFile(&handle,
                           GENERIC_READ,
                            &objAttr, &ioStatusBlock,
                            NULL,
                            FILE_ATTRIBUTE_NORMAL,
                            0,
                            FILE_OPEN, 
                            FILE_SYNCHRONOUS_IO_NONALERT,
                            NULL, 0);
    if(NT_SUCCESS(ntstatus))
	{
        byteOffset.LowPart = byteOffset.HighPart = 0;
        ntstatus = ZwReadFile(handle, NULL, NULL, NULL, &ioStatusBlock,
                              buffer, BUFFER_SIZE, &byteOffset, NULL);
        if(NT_SUCCESS(ntstatus))
		{
            buffer[BUFFER_SIZE-1] = '\0';
            DbgPrint("%s\n", buffer);
        }
		
        ZwClose(handle);
    }
        
        
        return STATUS_SUCCESS;
    }
}



