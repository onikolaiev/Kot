#include "main.h"

process_t self;

vector_t* windows = NULL;
vector_t* monitors = NULL;

void renderWindows() {
    // todo: multi threads monitor rendering
    for (uint32_t i = 0; i < monitors->length; i++) {
        Monitor* monitor = (Monitor*) vector_get(monitors, i);
        if (monitor != NULL) {
            monitor->update(windows);
        }
    }
}

thread_t renderThread = NULL;

void threadRender(){
    renderWindows();
    Sys_Execthread(renderThread, NULL, ExecutionTypeQueu, NULL);
    Sys_Close(KSUCCESS);
}

thread_t CreateWindowThread = NULL;
thread_t DestroyWindowThread = NULL;
thread_t GetFramebufferThread = NULL;
thread_t GetWidthThread = NULL;
thread_t GetHeightThread = NULL;
thread_t ShowThread = NULL;
thread_t HideThread = NULL;

/**
 * Return windowId
 **/
void CreateWindow(uint32_t width, uint32_t height, int32_t x, int32_t y) {
    Window* window = new Window(self, width, height, x, y);
    vector_push(windows, window);
    Sys_Close(windows->length-1);
}

/**
 * Return KCODE
 **/
void DestroyWindow(uint32_t windowId) {
    Window* window = (Window*) vector_get(windows, windowId);
    if (window->getOwner() != Sys_GetProcess()) { Sys_Close(KFAIL); }
    window->destroy();
    free(window);
    vector_set(windows, windowId, NULL);
    Sys_Close(KSUCCESS);
}

/**
 * Return Framebuffer
 **/
void GetFramebuffer(uint32_t windowId) {
    Window* window = (Window*) vector_get(windows, windowId);
    if (window->getOwner() != Sys_GetProcess()) { Sys_Close(KFAIL); }
    Sys_Close(window->getFramebufferKey());
}

/**
 * Return KCODE
 **/
void Show(uint32_t windowId) {
    Window* window = (Window*) vector_get(windows, windowId);
    if (window->getOwner() != Sys_GetProcess()) { Sys_Close(KFAIL); }
    window->show(true);
    Sys_Close(KSUCCESS);
}

/**
 * Return KCODE
 **/
void Hide(uint32_t windowId) {
    Window* window = (Window*) vector_get(windows, windowId);
    if (window->getOwner() != Sys_GetProcess()) { Sys_Close(KFAIL); }
    window->show(false);
    Sys_Close(KSUCCESS);
}

/**
 * Return Framebuffer
 **/
void GetHeight(uint32_t windowId) {
    Window* window = (Window*) vector_get(windows, windowId);
    Sys_Close(window->getHeight());
}

/**
 * Return Framebuffer
 **/
void GetWidth(uint32_t windowId) {
    Window* window = (Window*) vector_get(windows, windowId);
    Sys_Close(window->getWidth());
}

void initUISD() {

    Sys_Createthread(self, (uintptr_t) &CreateWindow, PriviledgeApp, &CreateWindowThread);
    Sys_Createthread(self, (uintptr_t) &DestroyWindow, PriviledgeApp, &DestroyWindowThread);
    Sys_Createthread(self, (uintptr_t) &GetFramebuffer, PriviledgeApp, &GetFramebufferThread);
    Sys_Createthread(self, (uintptr_t) &GetWidth, PriviledgeApp, &GetWidthThread);
    Sys_Createthread(self, (uintptr_t) &GetHeight, PriviledgeApp, &GetHeightThread);
    Sys_Createthread(self, (uintptr_t) &Show, PriviledgeApp, &ShowThread);
    Sys_Createthread(self, (uintptr_t) &Hide, PriviledgeApp, &HideThread);

    uintptr_t address = getFreeAlignedSpace(sizeof(uisd_graphics_t));
    ksmem_t key = NULL;
    Sys_CreateMemoryField(self, sizeof(uisd_graphics_t), &address, &key, MemoryFieldTypeShareSpaceRO);
    uisd_graphics_t* OrbSrv = (uisd_graphics_t*) address;
    OrbSrv->ControllerHeader.Process = self;
    OrbSrv->ControllerHeader.IsReadWrite = false;
    OrbSrv->ControllerHeader.Version = Orb_Srv_Version;
    OrbSrv->ControllerHeader.VendorID = Kot_VendorID;
    OrbSrv->ControllerHeader.Type = ControllerTypeEnum_Graphics;

    OrbSrv->CreateWindow = MakeShareableThread(CreateWindowThread, PriviledgeApp);
    OrbSrv->DestroyWindow = MakeShareableThread(DestroyWindowThread, PriviledgeApp);
    OrbSrv->GetFramebuffer = MakeShareableThread(GetFramebufferThread, PriviledgeApp);
    OrbSrv->GetWidth = MakeShareableThread(GetWidthThread, PriviledgeApp);
    OrbSrv->GetHeight = MakeShareableThread(GetHeightThread, PriviledgeApp);
    OrbSrv->Show = MakeShareableThread(ShowThread, PriviledgeApp);
    OrbSrv->Hide = MakeShareableThread(HideThread, PriviledgeApp);

    CreateControllerUISD(ControllerTypeEnum_Graphics, key, true);

}

void initCursor() {
    


}

void initOrb() {

    self = ShareProcessKey(Sys_GetProcess());

    monitors = vector_create();
    windows = vector_create();

    srv_system_callback_t* callback = Srv_System_GetFrameBuffer(true);
    srv_system_framebuffer_t* bootframebuffer = (srv_system_framebuffer_t*)callback->Data;
    free(callback);

    size64_t fb_size = bootframebuffer->Pitch * bootframebuffer->Height;
    
    uint64_t virtualAddress = (uint64_t)MapPhysical((uintptr_t)bootframebuffer->Address, fb_size);

    Monitor* monitor0 = new Monitor(self, (uintptr_t) virtualAddress, bootframebuffer->Width, bootframebuffer->Height, 0, 0);

    free(bootframebuffer);
    
    vector_push(monitors, monitor0);

    loadBootGraphics();

    initCursor();

    Sys_Createthread(self, (uintptr_t) &threadRender, PriviledgeDriver, &renderThread);
    Sys_Execthread(renderThread, NULL, ExecutionTypeQueu, NULL);

}

extern "C" int main() {

    initOrb();
    initUISD();

    Printlog("[ORB] Service started");

    return KSUCCESS;

}