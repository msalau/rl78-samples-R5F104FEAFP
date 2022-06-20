#include <stdint.h>

__attribute__((section(".opt")))
const uint8_t option_bytes[4] = {
    0x6e,
    0xff,
    0xe9,
    0x04
};
