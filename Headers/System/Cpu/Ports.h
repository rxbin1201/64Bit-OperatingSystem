#ifndef __CPU_PORTS
#define __CPU_PORTS

#include <System/Common/Types.h>

uint8_t byteIn(uint16_t port);

void byteOut(uint16_t port, uint8_t data);

#endif