#include "qemu_dma.h"
#include "serial.h"

void kernel_main(void) {
  extern const volatile unsigned int _stack_top;
  uint64_t heap_start = _stack_top;

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

  uint64_t *fb_write = (uint64_t*)heap_start;

  for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
    *(fb_write+i) = 155;
  }

  while (1);
}