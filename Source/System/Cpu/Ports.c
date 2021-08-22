#include <System/Cpu/Ports.h>

uint8_t byteIn(uint16_t port) {
    uint8_t result;
    
    __asm__ volatile("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void byteOut(uint16_t port, uint8_t data) {
    __asm__ volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}