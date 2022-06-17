#include "fb.h"
#include "qemu_dma.h"

void memcpy_(void *dest, void *src, uint64_t n)
{
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}

int ramfb_setup_c(uint64_t fb) {
    uint32_t select = qemu_cfg_find_file();

    if (select == 0) {
        return 1;
    }

    struct QemuRAMFBCfg cfg = {
        .addr   = __builtin_bswap64(fb),
        .fourcc = __builtin_bswap32(DRM_FORMAT_XRGB8888),
        .flags  = __builtin_bswap32(0),
        .width  = __builtin_bswap32(FRAMEBUFFER_WIDTH),
        .height = __builtin_bswap32(FRAMEBUFFER_HEIGHT),
        .stride = __builtin_bswap32(FRAMEBUFFER_STRIDE),
    };
    qemu_cfg_write_entry(&cfg, select, sizeof(cfg));
}

void write_pixel_packed(uint64_t fb_addr, uint16_t x, uint16_t y, uint32_t stride, uint8_t pixel[4]) {
    void *dest_far = (void*)(fb_addr + ((y * stride) + x));

    memcpy_(dest_far, pixel, 4);
}
