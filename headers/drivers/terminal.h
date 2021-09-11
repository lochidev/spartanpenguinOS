#ifndef VGA_H
#define VGA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../helpers/helpers.h"
#include "../drivers/lowlevel/io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
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
void set_cursor(unsigned short pos);
unsigned short get_cursor_pos();
static inline uint8_t get_vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t get_vga_entry(unsigned char uc, uint8_t color);
void terminal_initialize(void);
void terminal_set_color(uint8_t color);
void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y);
void terminal_put_char(char c);
void terminal_write(const char* data, size_t size);
void terminal_write_string(const char* data);
#endif


