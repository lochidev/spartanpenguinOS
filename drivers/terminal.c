#include "../headers/drivers/terminal.h"
/*-------------------------------------------------------------------------------------*/
/*                                                                                     */
/*  This driver provides an API to interact with the frame buffer                      */
/*                                                                                     */
/*-------------------------------------------------------------------------------------*/

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/*
    The cursor’s position is determined
    with a 16 bits integer: 0 means row zero, column zero;
    1 means row zero, column one;
    80 means row one, column zero and so on.
    Since the position is 16 bits large,
    and the out assembly code instruction argument is
    8 bits, the position must be sent in two turns,
    first 8 bits then the next 8 bits.
*/

void set_cursor(unsigned short pos) {
    /* ex:- position 80 = 0x0050, pos => 0000000001010000 */
    /* 0000000001010000 & 11111111 => low byte=0x50 */
    /* 0101000000000000 & 11111111 => high byte=0x00 */
    port_byte_out(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    port_byte_out(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    port_byte_out(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    port_byte_out(FB_DATA_PORT, (pos & 0x00FF));
}
unsigned short get_cursor_pos() {
    /* ex: high byte=0x50, low byte=0x01 */
    port_byte_out(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    unsigned short result;
    result = port_byte_in(FB_DATA_PORT) << 8; /* 0x5000 */
    port_byte_out(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    result += port_byte_in(FB_DATA_PORT); /* 0x5001 */
    return result;
}
static inline const size_t calculate_index(size_t column, size_t row) {
    return row * VGA_WIDTH + column;
}
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* frame_buffer;
unsigned short cursor_position;

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

void terminal_initialize(void)
{
    terminal_row = 1;
    terminal_column = 0;
    cursor_position = 80;
    set_cursor(cursor_position);
    terminal_color = get_vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLUE);
    frame_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = calculate_index(x, y);
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
    const size_t index = calculate_index(x, y);
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
    cursor_position = calculate_index(terminal_column, terminal_row);
    set_cursor(cursor_position);
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        char c = data[i];
        if(c == '\0')
            return;
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
    size_t size = strlen(data);
    terminal_write(data, size);
}