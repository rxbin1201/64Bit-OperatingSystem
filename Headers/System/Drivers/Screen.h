#ifndef __SCREEN_DRIVER
#define __SCREEN_DRIVER

#include <System/Common/Types.h>

#define SCREEN_ADDRESS 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_EXTENT 80 * 25

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define BROWN 6
#define GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define YELLOW 14
#define WHITE 15

#define CURSOR_PORT_COMMAND (uint16_t) 0x3D4
#define CURSOR_PORT_DATA (uint16_t) 0x3D5

typedef struct __attribute__((packed)) {
    char character;
    char style;
} screenChar;

uint8_t screenColor(const uint8_t foreground, const uint8_t background);

void clearScreen(const uint8_t foreground, const uint8_t background);

void printChar(const char character, const uint8_t foreground, const uint8_t background);

void print(const char *string, const uint8_t foreground, const uint8_t background);

uint16_t getCursorPosition();

void showCursor();
void hideCursor();

void advanceCursor();
void reverseCursor();
void setCursorPosition(uint8_t x, uint8_t y);

void scrollLine();

#endif