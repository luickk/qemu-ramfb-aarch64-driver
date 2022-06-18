#include "serial.h"
#include "fb.h"
#include "qemu_dma.h"
#include "image_rgb_map.h"

void kernel_main(void) {
  extern uint64_t _stack_top;
  uint64_t heap_start = (uint64_t)&_stack_top;

  if (check_fw_cfg_dma()) {
    kprint("guest fw_cfg dma-interface enabled \n");
  } else {
    kprint("guest fw_cfg dma-interface NOT enabled - abort \n");
    return;
  }
  
  uint32_t fb_width = 1024;
  uint32_t fb_height = 768;
  uint32_t fb_bpp = 4;
  uint32_t fb_stride = fb_bpp * fb_width;

  fb_info fb = {
    .fb_addr = heap_start,
    .fb_width = fb_width,
    .fb_height = fb_height,
    .fb_bpp = fb_bpp,

    .fb_stride = fb_stride,
    .fb_size = fb_stride * fb_height,
  };

  /* since this is just a "proof of concept", the complete "heap" serves as framebuffer... */
  if (ramfb_setup(&fb) != 0) {
    kprint("error setting up ramfb \n");
  }
  


  kprint("setup ramfb successfull\n");

  // uint8_t pixel[3] = {255, 255, 255};

  // for (int i = 0; i < fb.fb_width; i++) {
  //   for (int j = 0; j < fb.fb_height; j++) {
  //     write_rgb256_pixel(&fb, i, j, pixel);
  //   }
  // }

  draw_rgb256_map(&fb, 500, 500, (uint8_t*)&img[0]);

  while (1);
}