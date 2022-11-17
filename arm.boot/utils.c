// Some auxiliary functions for the shell
#include "main.h"

void erase_entire_screen(int uart)
{
  uart_send_string(uart, "\e[2J");
}


void cursor_to_home(int uart)
{
  set_cursor(uart, 1, 1);
}


void set_cursor(int uart, int row, int col)
{
  if (uart = UART0)
  {
    cc_printf("\e[%d;%dH", row, col);
    cursor.row = row;
    cursor.col = col;
  }
  else
    kprintf("\e[%d;%dH", row, col);
}


void reset_screen(int uart)
{
  erase_entire_screen(uart);
  cursor_to_home(uart);
}


void print_shell_name(int uart)
{
  if (uart == UART0)
  {
    cc_printf("\e[1m%s\e[22m", SHELL_PROMPT);
    cursor.col += cc_strlen(SHELL_PROMPT);
  }
  else
    kprintf("\e[1m%s\e[22m", SHELL_PROMPT);
  set_cursor(uart, cursor.row, cursor.col);
}

void erase_cursor_to_end_of_line(int uart)
{
  uart_send_string(uart, "\e[K");
}


void cursor_to_beginning_of_next_line(int uart)
{
  cursor.row++;
  cursor.col = 1;
  set_cursor(uart, cursor.row, cursor.col);
}
