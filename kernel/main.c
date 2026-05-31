#include <stdint.h>
#include <stddef.h>

__attribute__((section(".multiboot")))
const uint32_t multiboot_header[] = {
    0x1BADB002,
    0x00000003,
    -(0x1BADB002 + 0x00000003)
};

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static size_t row = 0;
static size_t column = 0;
static uint8_t color = 0x0F;

// Creates a VGA cell that essentially,
// is a space that looks empty. Like a space
// character.
static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Delcare a function called `clear_screen`
// `void` means this function returns nothing.
// `static` means this function is only visable
// inside this `.c` file.

static void clear_screen(void) {
    // We loop over every row of the screen.
    // `y` goes from 0 - 24.
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        // Iner loops; loops over every COLUMN of the
        // current row.
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            // This just writes an empty blank space
            // into the current screen cell.
            VGA_MEMORY[y * VGA_WIDTH + x] = vga_entry(' ', color);
        }
    }

    row = 0;
    column = 0;
}

static void putchar(char c) {
    if (c == '\n') {
        column = 0;
        row++;
        return;
    }

    VGA_MEMORY[row * VGA_WIDTH + column] = vga_entry(c, color);
    column++;

    if (column >= VGA_WIDTH) {
        column = 0;
        row++;
    }

    if (row >= VGA_HEIGHT) {
        row = 0;
    }
}

static void print(const char* text) {
    for (size_t i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
    }
}

// Kernel starts here
void kernel_main(void) {
    clear_screen();

    print("Welcome to TawilOS Mobile\n");
    print("Fully custom phone OS boot test\n");
    print("--------------------------------\n");
    print("Kernel loaded successfully.\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
