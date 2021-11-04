#include "PageTableManager.h"
#include "../../logs/logs.h"

PageTableManager globalPageTableManager;

void PageTableManager::PageTableManagerInit(PageTable* PML4Address){
    this->PML4 = PML4Address;
    this->PhysicalMemoryVirtualAddress = 0;
    this->VirtualAddress = 0x10000000000000000; //It must be a multiple of 0x1000
}

void PageTableManager::DefinePhysicalMemoryLocation(void* PhysicalMemoryVirtualAddress){
    if((uint64_t)PhysicalMemoryVirtualAddress % 0x1000){
        PhysicalMemoryVirtualAddress -= (uint64_t)PhysicalMemoryVirtualAddress % 0x1000;
        PhysicalMemoryVirtualAddress += 0x1000;
    }
    this->PhysicalMemoryVirtualAddressSaver = PhysicalMemoryVirtualAddress;
}

void PageTableManager::DefineVirtualTableLocation(){
    this->PhysicalMemoryVirtualAddress = PhysicalMemoryVirtualAddressSaver;
    globalAllocator.PageBitmap.Buffer = (uint8_t*)globalPageTableManager.GetVirtualAddress(globalAllocator.PageBitmap.Buffer);
}

void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory){
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    PageTable* PML4VirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PML4);
    PDE = PML4VirtualAddress->entries[indexer.PDP_i];
    PageTable* PDP;
    PageTable* PDPVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PDP = (PageTable*)globalAllocator.RequestPage();
        PDPVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PDP);  
        memset(PDPVirtualAddress, 0, 0x1000);
        PDE.SetAddress((uint64_t)PDP >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PML4VirtualAddress->entries[indexer.PDP_i] = PDE;
    }
    else
    {
        PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDPVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PDP);    
    }

    
    PDE = PDPVirtualAddress->entries[indexer.PD_i];

    PageTable* PD;
    PageTable* PDVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PD = (PageTable*)globalAllocator.RequestPage();
        PDVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PD); 
        memset(PDVirtualAddress, 0, 0x1000);
        PDE.SetAddress((uint64_t)PD >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDPVirtualAddress->entries[indexer.PD_i] = PDE;
    }
    else
    {
        PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PD);  
    }


    PDE = PDVirtualAddress->entries[indexer.PT_i];
    PageTable* PT;
    PageTable* PTVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PT = (PageTable*)globalAllocator.RequestPage();
        PTVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PT);
        memset(PTVirtualAddress, 0, 0x1000);
        PDE.SetAddress((uint64_t)PT >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDVirtualAddress->entries[indexer.PT_i] = PDE;
    }
    else
    {
        PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PTVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PT);  
    }


    PDE = PTVirtualAddress->entries[indexer.P_i];
    PDE.SetAddress((uint64_t)physicalMemory >> 12);
    PDE.SetFlag(PT_Flag::Present, true);
    PDE.SetFlag(PT_Flag::ReadWrite, true);
    PTVirtualAddress->entries[indexer.P_i] = PDE;
}

void* PageTableManager::MapMemory(void* physicalMemory, size_t pages){
    this->VirtualAddress -= pages * 0x1000;
    void* virtualMemory = (void*)this->VirtualAddress;

    for(int i = 0; i < pages; i++){
        MapMemory((void*)((uint64_t)virtualMemory + i * 0x1000), (void*)((uint64_t)physicalMemory + i * 0x1000));
    }

    return virtualMemory;
}

void PageTableManager::UnmapMemory(void* virtualMemory){
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    PageTable* PML4VirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PML4);
    PDE = PML4VirtualAddress->entries[indexer.PDP_i];
    PageTable* PDP;
    PageTable* PDPVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PDP = (PageTable*)globalAllocator.RequestPage();
        PDPVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PDP);  
        memset(PDPVirtualAddress, 0, 0x1000);
        PDE.SetAddress((uint64_t)PDP >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PML4VirtualAddress->entries[indexer.PDP_i] = PDE;
    }
    else
    {
        PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDPVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PDP);    
    }

    
    PDE = PDPVirtualAddress->entries[indexer.PD_i];

    PageTable* PD;
    PageTable* PDVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PD = (PageTable*)globalAllocator.RequestPage();
        PDVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PD); 
        memset(PDVirtualAddress, 0, 0x1000);
        PDE.SetAddress((uint64_t)PD >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDPVirtualAddress->entries[indexer.PD_i] = PDE;
    }
    else
    {
        PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PD);  
    }

    PDE = PDVirtualAddress->entries[indexer.PT_i];
    PageTable* PT;
    PageTable* PTVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PT = (PageTable*)globalAllocator.RequestPage();
        PTVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PT);
        memset(PTVirtualAddress, 0, 0x1000);
        PDE.SetAddress((uint64_t)PT >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDVirtualAddress->entries[indexer.PT_i] = PDE;
    }
    else
    {
        PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PTVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PT);  
    }


    PDE = PTVirtualAddress->entries[indexer.P_i];
    PDE.SetAddress(0);
    PDE.SetFlag(PT_Flag::Present, false);
    PDE.SetFlag(PT_Flag::ReadWrite, false);
    PTVirtualAddress->entries[indexer.P_i] = PDE;
}

void PageTableManager::MapUserspaceMemory(void* virtualMemory) {
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    PageTable* PML4VirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PML4);
    PDE = PML4VirtualAddress->entries[indexer.PDP_i];
    PageTable* PDP;
    PageTable* PDPVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        return;
    }
    else
    {
        PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDPVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PDP); 
    }
    
    PDE.SetFlag(PT_Flag::User, true);
    PML4VirtualAddress->entries[indexer.PDP_i] = PDE;

    PDE = PDPVirtualAddress->entries[indexer.PD_i];

    PageTable* PD;
    PageTable* PDVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        return;
    }
    else
    {
        PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PD);  
    }

    PDE.SetFlag(PT_Flag::User, true);
    PDPVirtualAddress->entries[indexer.PD_i] = PDE;

    PDE = PDVirtualAddress->entries[indexer.PT_i];

    PageTable* PT;
    PageTable* PTVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        return;
    }
    else
    {
        PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PTVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PT);
    }

    PDE.SetFlag(PT_Flag::User, true);
    PDVirtualAddress->entries[indexer.PT_i] = PDE;

    PDE = PTVirtualAddress->entries[indexer.P_i];
    PDE.SetFlag(PT_Flag::User, true);
    PTVirtualAddress->entries[indexer.P_i] = PDE;
}

void* PageTableManager::GetPhysicalAddress(void* virtualMemory){
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    PageTable* PML4VirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PML4);
    PDE = PML4VirtualAddress->entries[indexer.PDP_i];
    PageTable* PDP;
    PageTable* PDPVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        return 0;
    }
    else
    {
        PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDPVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PDP); 
    }
    
    
    PDE = PDPVirtualAddress->entries[indexer.PD_i];

    PageTable* PD;
    PageTable* PDVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        return 0;
    }
    else
    {
        PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PDVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PD);  
    }

    PDE = PDVirtualAddress->entries[indexer.PT_i];

    PageTable* PT;
    PageTable* PTVirtualAddress;
    if (!PDE.GetFlag(PT_Flag::Present)){
        return 0;
    }
    else
    {
        PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        PTVirtualAddress = (PageTable*)globalPageTableManager.GetVirtualAddress(PT);
    }

    PDE = PTVirtualAddress->entries[indexer.P_i];
    return (void*)((uint64_t)PDE.GetAddress() * 0x1000 + (uint64_t)virtualMemory % 0x1000);
}

void* PageTableManager::GetVirtualAddress(void* physicalAddress){
    return (void*)((uint64_t)PhysicalMemoryVirtualAddress + (uint64_t)physicalAddress);
}

void PageTableManager::CopyHigherHalf(PageTableManager* pageTableManagerToCopy){
    DefinePhysicalMemoryLocation(pageTableManagerToCopy->PhysicalMemoryVirtualAddress);
    this->PhysicalMemoryVirtualAddress = PhysicalMemoryVirtualAddressSaver;
    PageTable* PML4VirtualAddressDestination = (PageTable*)globalPageTableManager.GetVirtualAddress(PML4);
    PageTable* PML4VirtualAddressToCopy = (PageTable*)globalPageTableManager.GetVirtualAddress(pageTableManagerToCopy->PML4);
    for(int i = 255; i < 512; i++){
        PML4VirtualAddressDestination->entries[i] = PML4VirtualAddressToCopy->entries[i];
    }
}