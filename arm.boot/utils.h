#ifndef UTILS_H
#define UTILS_H

#include "uart.h" 

void erase_entire_screen(int uart);
void cursor_to_home(int uart);
void set_cursor(int uart, int row, int col);
void reset_screen(int uart);
void print_shell_name(int uart);
void erase_cursor_to_end_of_line(int uart);
void cursor_to_beginning_of_next_line(int uart);

#endif
