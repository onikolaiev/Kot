#include <core/main.h>

framebuffer_t* cloneFramebuffer(framebuffer_t* screen) {
    framebuffer_t* buffer = (framebuffer_t *) malloc(sizeof(framebuffer_t));
    buffer->fb_addr = (uint64_t)((uint8_t*) malloc(screen->fb_size));
    buffer->fb_size = screen->fb_size;
    buffer->width = screen->width;
    buffer->height = screen->height;
    buffer->bpp = screen->bpp;
    buffer->btpp = screen->btpp;
    buffer->pitch = screen->pitch;
    return buffer;
}

extern "C" int main(int argc, char* argv[], bootbuffer_t* Framebuffer){

    Printlog("[FLOWGE] Initialization ...");

    kprocess_t self;
    SYS_GetProcessKey(&self);

    framebuffer_t* screen = (framebuffer_t*) malloc(sizeof(framebuffer_t));
    screen->fb_size = Framebuffer->framebuffer_pitch * Framebuffer->framebuffer_height;

    uint64_t virtualAddress = (uint64_t) KotSpecificData.FreeMemorySpace - screen->fb_size;
    SYS_Map(self, &virtualAddress, true, (uintptr_t *) &Framebuffer->framebuffer_addr, &screen->fb_size, false);

    screen->fb_addr = virtualAddress;
    screen->width = Framebuffer->framebuffer_width;
    screen->height = Framebuffer->framebuffer_height;
    screen->pitch = Framebuffer->framebuffer_pitch;
    screen->bpp = Framebuffer->framebuffer_bpp;
    screen->btpp = screen->bpp/8;

    Context screen_ctx = Context(screen);
    Context backbuffer_ctx = Context(cloneFramebuffer(screen));
   
    backbuffer_ctx.fill(0x000000);
    screen_ctx.swapFrom(&backbuffer_ctx);

    Printlog("[FLOWGE] Service initialized successfully");

    // window logic

    Window w1(&backbuffer_ctx, 400, 400, 100, 100);
    Window w2(&backbuffer_ctx, 210, 210, 90, 90);
    w1.getContext()->clear();
    w2.getContext()->clear();

    w2.getContext()->drawLine(10, 10, 200, 200, 0xffffff);
    w2.getContext()->drawLine(200, 10, 10, 200, 0xffffff);

    w1.getContext()->fillRect(0, 0, 10, 100, 0xff00ff);
    w1.getContext()->fillRect(50, 10, 100, 10, 0x00ff00);

    w1.getContext()->drawLine(200, 200, 250, 250, 0xffffff); // positive octants
    w1.getContext()->drawLine(250, 260, 200, 210, 0xffffff); // negative octants

    w1.getContext()->fillRect(10, 300, 70, 70, 0xffffff);
    w1.getContext()->drawRect(10, 300, 70, 70, 0xff0000);
    
    w1.show();
    w2.show();

    // update logic

    // to optimize: swap back to front on another thread
    backbuffer_ctx.fill(0x000000);
    w2.render(&backbuffer_ctx);
    w1.render(&backbuffer_ctx);
    screen_ctx.swapFrom(&backbuffer_ctx);

    for (int64_t i = 0; i < 100000000; i++) {
        // to be able to see the movement of the window lmao :')
    }

    w1.move(250, 250);
    w2.resize(300, 300);

    backbuffer_ctx.fill(0x000000);
    w1.render(&backbuffer_ctx);
    w2.render(&backbuffer_ctx);
    screen_ctx.swapFrom(&backbuffer_ctx);

    for (int64_t i = 0; i < 100000000; i++) {
        // to be able to see the movement of the window lmao :')
    }

    w2.resize(250, 300);

    backbuffer_ctx.fill(0x000000);
    w1.render(&backbuffer_ctx);
    w2.render(&backbuffer_ctx);
    screen_ctx.swapFrom(&backbuffer_ctx);

    return KSUCCESS;

}