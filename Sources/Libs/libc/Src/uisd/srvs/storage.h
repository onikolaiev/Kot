#ifndef _SRV_STORAGE_H
#define _SRV_STORAGE_H 1

#include <kot/sys.h>
#include <kot/uisd.h>
#include <kot/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define SerialNumberSize        0x14
#define DriveModelNumberSize    0x28

typedef KResult (*StorageCallbackHandler)(KResult Status, struct srv_storage_callback_t* Callback, uint64_t GP0, uint64_t GP1, uint64_t GP2, uint64_t GP3);

struct srv_storage_space_info_t{
    thread_t CreateProtectedDeviceSpaceThread;
    thread_t ReadWriteDeviceThread;
    ksmem_t BufferRWKey;
    uint64_t BufferRWAlignement;
    uint64_t BufferRWUsableSize;
    process_t DriverProc;
    uint64_t SpaceSize;
};

struct srv_storage_device_info_t{
    struct srv_storage_space_info_t MainSpace;
    uint64_t DeviceSize;
    uint8_t SerialNumber[SerialNumberSize];
    uint8_t DriveModelNumber[DriveModelNumberSize];
};

struct srv_storage_fs_server_functions_t{
    thread_t Rename;
    thread_t Remove;
    thread_t Fopen;
    thread_t Mkdir;
    thread_t Readdir;
    thread_t Flist;
};

struct srv_storage_callback_t{
    thread_t Self;
    uint64_t Data;
    size64_t Size;
    bool IsAwait;
    KResult Status;
    StorageCallbackHandler Handler;
};

void Srv_Storage_Initialize();

void Srv_Storage_Callback(KResult Status, struct srv_storage_callback_t* Callback, uint64_t GP0, uint64_t GP1, uint64_t GP2, uint64_t GP3);

struct srv_storage_callback_t* Srv_Storage_AddDevice(struct srv_storage_device_info_t* Info, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_RemoveDevice(uint64_t Index, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_NotifyOnNewPartitionByGUIDType(struct GUID_t* PartitionTypeGUID, thread_t ThreadToNotify, process_t ProcessToNotify, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_MountPartition(uint64_t ID, struct srv_storage_fs_server_functions_t* FSServerFunctions, bool IsAwait);
struct srv_storage_callback_t* Srv_Storage_UnmountPartition(uint64_t ID, bool IsAwait);

#if defined(__cplusplus)
}
#endif

#endif