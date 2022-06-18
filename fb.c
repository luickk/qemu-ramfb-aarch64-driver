#include "fb.h"
#include "qemu_dma.h"
#include "serial.h"

void memcpy_(void *dest, void *src, uint64_t n)
{
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}

int ramfb_setup(fb_info *fb) {
    uint32_t select = qemu_cfg_find_file();

    if (select == 0) {
        return 1;
    }

    struct QemuRAMFBCfg cfg = {
        .addr   = __builtin_bswap64(fb->fb_addr),
        .fourcc = __builtin_bswap32(DRM_FORMAT_XRGB8888),
        .flags  = __builtin_bswap32(0),
        .width  = __builtin_bswap32(fb->fb_width),
        .height = __builtin_bswap32(fb->fb_height),
        .stride = __builtin_bswap32(fb->fb_stride),
    };
    qemu_cfg_write_entry(&cfg, select, sizeof(cfg));
}

void write_xrgb256_pixel(fb_info *fb, uint16_t x, uint16_t y, uint8_t pixel[4]){
    memcpy_((void*)fb->fb_addr + ((y * fb->fb_stride) + (x * fb->fb_bpp)), pixel, 4);
}


void write_rgb256_pixel(fb_info *fb, uint16_t x, uint16_t y, uint8_t pixel[3]) {
    // offset one byte (xrgb)
    memcpy_((void*)fb->fb_addr + ((y * fb->fb_stride) + (x * fb->fb_bpp)), pixel, 4);
}

void draw_rgb256_map(fb_info *fb, uint32_t x_res, uint32_t y_res, uint8_t *rgb_map) {

    uint8_t map_bpp = 4;
    uint64_t map_stride = x_res * map_bpp;
    uint64_t map_size = map_stride*y_res;

    uint64_t i = 0;
    for (int map_i = 0; map_i < map_size; map_i += 4) {
        if (map_i%map_stride == 0 && map_i != 0) {
            i += fb->fb_stride-map_stride;
        }
        // 1 compensates for alignement (xRGB)
        memcpy_((void*)fb->fb_addr + i, &rgb_map[map_i], 4);

        i += 4;
    }
}
