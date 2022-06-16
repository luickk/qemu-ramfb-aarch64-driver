#include "qemu_dma.h"
#include "serial.h"

void kernel_main(void) {
  extern const volatile unsigned int _stack_top;
  uint64_t heap_start = _stack_top;

  check_fw_cfg_avail();
  kprint("test\n");
  
  /* since this is just a "proof of concept", the complete "heap" serves as framebuffer... */

  ramfb_setup_c(heap_start);
  
  kprint("done\n");

  uint8_t *fb_write = (uint8_t*)heap_start;

  for (int i = heap_start; i < heap_start+400000; i++) {
    fb_write[i] = 255;
  }

  kprint("set pixel to black\n");

  while (1);
}