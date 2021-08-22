#include <System/Drivers/Screen.h>

void main() {
    setCursorPosition(0, 0);
    clearScreen(CYAN, BLACK);

    print("Simple 64-Bit OS loaded!", CYAN, BLACK);

    for(uint16_t i = 0; i < 25; i++) {
        print("\nNew Line!", CYAN, BLACK);
    }

    print("\nScroll done!", LIGHT_GREEN, BLACK);
}