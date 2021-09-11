/* Check if the compiler thinks you are targeting the wrong operating system.
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif */

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

#include "../headers/drivers/terminal.h"
#include "../headers/helpers/helpers.h"

void kernel_main(void)
{
    /* Initialize terminal interface */
    terminal_initialize();
    terminal_write_string("  Spartan Penguin OS!\n");
    terminal_write_string("  Totally doing some stuff... please wait!\n");
}

