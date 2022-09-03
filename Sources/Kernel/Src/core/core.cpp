#include <core/core.h>

extern "C" void main(uintptr_t boot){   
    /* load arch */
    ArchInfo_t* ArchInfo = arch_initialize(boot);
    ArchInfo->revision = 1;

    /* load init file */
    initrd::File* InitFile = initrd::FindInitFile();
    
    if(InitFile != NULL){
        uintptr_t BufferInitFile = malloc(InitFile->size);
        initrd::Read(InitFile, BufferInitFile);
        kthread_t* mainthread;
        ELF::loadElf(BufferInitFile, PriviledgeDriver, &mainthread);
        arguments_t* InitParameters = (arguments_t*)malloc(sizeof(arguments_t));
        ThreadShareData_t DataInfo;
        GetDataToStartService(ArchInfo, mainthread, InitParameters, &DataInfo.Data, &DataInfo.Size);
        DataInfo.ParameterPosition = 0x0;
        globalTaskManager->Execthread(mainthread, mainthread, ExecutionTypeQueu, InitParameters, &DataInfo, NULL);
        free(InitParameters);
    }else{
        Error("Can't load initialization file");
    }
    free(ArchInfo);

    
    globalTaskManager->EnabledScheduler(CPU::GetAPICID());

    LaunchUserSpace();

    while(true){
        asm("hlt");
    };
}


