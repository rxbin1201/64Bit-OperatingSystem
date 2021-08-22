#include <System/Cpu/Ports.h>
#include <System/Drivers/Screen.h>

volatile screenChar *TEXT_AREA = (screenChar*) SCREEN_ADDRESS;

uint8_t screenColor(const uint8_t foreground, const uint8_t background) {
    return (background << 4) | (foreground & 0x0F);
}

void clearScreen(uint8_t foreground, uint8_t background) {
    const char space = ' ';
    uint8_t clearColor = screenColor(foreground, background);

    const screenChar clearChar = {
        .character = space,
        .style = clearColor
    };

    for(uint64_t i = 0; i < SCREEN_EXTENT; i++) {
        TEXT_AREA[i] = clearChar;
    }
}

void printChar(const char character, const uint8_t foreground, const uint8_t background) {
    uint16_t position = getCursorPosition();

    if(character == '\n') {
        uint8_t currentRow = (uint8_t) (position / SCREEN_WIDTH);

        if(++currentRow >= SCREEN_HEIGHT) {
            scrollLine();
        } else {
            setCursorPosition(0, currentRow);
        }
    } else if(character == '\b') {
        reverseCursor();
        printChar(' ', foreground, background);
        reverseCursor();
    } else if(character == '\r') {
        uint8_t currentRow = (uint8_t) (position / SCREEN_WIDTH);
        
        setCursorPosition(0, currentRow);
    } else if(character == '\t') {
        for(uint8_t i = 0; i < 4; i++) {
            printChar(' ', foreground, background);
        }

        advanceCursor();
    } else {
        uint8_t style = screenColor(foreground, background);
        screenChar printed = {
            .character = character,
            .style = style
        };

        TEXT_AREA[position] = printed;

        advanceCursor();
    }
}

void print(const char *string, const uint8_t foreground, const uint8_t background) {
    while(*string != '\0') {
        printChar(*string++, foreground, background);
    }
}

uint16_t getCursorPosition() {
    uint16_t position = 0;

    byteOut(CURSOR_PORT_COMMAND, 0x0F);
    position |= byteIn(CURSOR_PORT_DATA);

    byteOut(CURSOR_PORT_COMMAND, 0x0E);
    position |= byteIn(CURSOR_PORT_DATA) << 8;

    return position;
}

void showCursor() {
    uint8_t current;

    byteOut(CURSOR_PORT_COMMAND, 0x0A);
    current = byteIn(CURSOR_PORT_DATA);
    byteOut(CURSOR_PORT_DATA, current & 0xC0);

    byteOut(CURSOR_PORT_COMMAND, 0x0B);
    current = byteIn(CURSOR_PORT_DATA);
    byteOut(CURSOR_PORT_DATA, current & 0xE0);
}

void hideCursor() {
    byteOut(CURSOR_PORT_COMMAND, 0x0A);
    byteOut(CURSOR_PORT_DATA, 0x20);
}

void advanceCursor() {
    uint16_t position = getCursorPosition();
    position++;

    if(position >= SCREEN_EXTENT) {
        scrollLine();
    }

    byteOut(CURSOR_PORT_COMMAND, 0x0F);
    byteOut(CURSOR_PORT_DATA, (uint8_t) (position & 0xFF));

    byteOut(CURSOR_PORT_COMMAND, 0x0E);
    byteOut(CURSOR_PORT_DATA, (uint8_t) ((position >> 8) & 0xFF));
}

void reverseCursor() {
    unsigned short position = getCursorPosition();
    position--;

    byteOut(CURSOR_PORT_COMMAND, 0x0F);
    byteOut(CURSOR_PORT_DATA, (unsigned char) (position & 0xFF));

    byteOut(CURSOR_PORT_COMMAND, 0x0E);
    byteOut(CURSOR_PORT_DATA, (unsigned char) ((position >> 8) & 0xFF));
}

void setCursorPosition(uint8_t x, uint8_t y) {
    uint16_t position = x + ((uint16_t) SCREEN_WIDTH * y);
    
    if(position >= SCREEN_EXTENT) {
        position = SCREEN_EXTENT - 1;
    }

    byteOut(CURSOR_PORT_COMMAND, 0x0F);
    byteOut(CURSOR_PORT_DATA, (uint8_t) (position & 0xFF));

    byteOut(CURSOR_PORT_COMMAND, 0x0E);
    byteOut(CURSOR_PORT_DATA, (uint8_t) ((position >> 8) & 0xFF));
}

void scrollLine() {
    for(uint16_t i = 1; i < SCREEN_HEIGHT; i++) {
        for(uint16_t j = 0; j < SCREEN_WIDTH; j++) {
            uint16_t toPosition = j + ((i - 1) * SCREEN_WIDTH);
            uint16_t fromPosition = j + (i * SCREEN_WIDTH);

            TEXT_AREA[toPosition] = TEXT_AREA[fromPosition];
        }
    }

    uint16_t i = SCREEN_HEIGHT - 1;
    
    for(uint16_t j = 0; j < SCREEN_WIDTH; j++) {
        uint16_t position = j + (i * SCREEN_WIDTH);

        screenChar current = TEXT_AREA[position];
        
        screenChar clear = {
            .character = ' ',
            .style = current.style
        };

        TEXT_AREA[position] = clear;
    }

    setCursorPosition(0, SCREEN_HEIGHT - 1);
}