#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system.
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif */

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};
/* ex: 0x2841 -> 0x41 is A, 2 -> Green fg, 8 -> Dark grey bg */
/* ex-2: 0x3A42 -> 0x42 is B, 3 -> Cyan fg, A(10) -> Light green bg */
static inline uint8_t get_vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    /* 00000010 | 10000000 => 10000010 */
    /* 00000011 | 10100000 => 10100011 */
    return fg | bg << 4;
}
/* get vga entry bits */
static inline uint16_t get_vga_entry(unsigned char uc, uint8_t color)
{
    /* 0000000001000001 | 1000001000000000 => 10000001001000001*/
    /* 0000000001000010 | 1010001100000000 => 10100011001000010*/
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* frame_buffer;

void terminal_initialize(void)
{
    terminal_row = 1;
    terminal_column = 0;
    terminal_color = get_vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLUE);
    frame_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            frame_buffer[index] = get_vga_entry(' ', terminal_color);
        }
    }
}

void terminal_set_color(uint8_t color)
{
    terminal_color = color;
}

void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    frame_buffer[index] = get_vga_entry(c, color);
}

void terminal_put_char(char c)
{
    terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        char c = data[i];
        if(c != '\n') {
            terminal_put_char(c);
        }
        else {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void terminal_write_string(const char* data)
{
    terminal_write(data, strlen(data));
}
void main(void)
{
    /* Initialize terminal interface */
    terminal_initialize();

    /* Newline support is left as an exercise. */
    terminal_write_string("  Spartan Penguin OS!\n");
    terminal_write_string("  Totally doing some stuff... please wait!\n");
}

