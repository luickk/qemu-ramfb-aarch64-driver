#include "serial.h"
#include "fb.h"
#include "qemu_dma.h"

void kernel_main(void) {
  extern uint64_t _stack_top;
  uint64_t heap_start = (uint64_t)&_stack_top;

  if (check_fw_cfg_dma()) {
    kprint("guest fw_cfg dma-interface enabled \n");
  } else {
    kprint("guest fw_cfg dma-interface NOT enabled - abort \n");
    return;
  }
  
  /* since this is just a "proof of concept", the complete "heap" serves as framebuffer... */
  if (ramfb_setup_c(heap_start) != 0) {
    kprint("error setting up ramfb \n");
  }
  
  kprint("setup ramfb successfull\n");

  uint8_t str[20] = {};
  kprint(uitoa(str, heap_start, 10));

  // bright red
  uint8_t pixel[4] = {0, 255, 64, 0};
  for (int i = 0; i < FRAMEBUFFER_WIDTH; i++) {
    for (int j = 0; j < FRAMEBUFFER_HEIGHT; j++) {
      write_pixel_packed(heap_start, i, j, FRAMEBUFFER_STRIDE, pixel);
    }
  }

  while (1);
}