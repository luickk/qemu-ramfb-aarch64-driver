#include  <stdint.h>

#define SERIAL_MMIO 0x09000000

void kprint(uint8_t *print_string);
uint8_t* uitoa(uint8_t *str, uint64_t num, int base);