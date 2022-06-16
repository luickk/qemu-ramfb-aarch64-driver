#include "serial.h"

void put_char(uint8_t ch) {
    *((uint64_t*)SERIAL_MMIO) = ch;
}


void kprint(uint8_t *print_string) {
    uint64_t i = 0;
    while(1) {
        if (print_string[i] == 0) {
            break;
        }
        put_char(print_string[i]);
        i++;
    }
}
