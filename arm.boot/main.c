#include "main.h"

/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the reset.s file.
 */

char history[HISTORY_LENGTH][MAX_LINE_LENGTH];
char curr_line[MAX_LINE_LENGTH];
int line_index = 0;
int history_index = 0;
State state = NORMAL;
Cursor cursor;

void _start() {
  // Setup
  reset_screen(UART0);
  print_shell_name(UART0);

  // Loop
  while (1) {
    unsigned char c;
    while (uart_receive(UART0, &c) == 0) {}
    
    if (state == NORMAL)
    {
      switch (c)
      {
        case '\r': // Carriage return
        case '\n': // Line feed
          kprintf("NORMAL:: \\r\\n   : char(\\r\\n | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          cc_memcpy(history[0], curr_line, MAX_LINE_LENGTH);
          cursor_to_beginning_of_next_line(UART0);
          execute(UART0, curr_line);
          update_history();
          print_shell_name(UART0);
          break;
 
        case 0x1b: // ESC
          kprintf("NORMAL:: escape  : char(ESC    | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          state = ESC;
          break;

        case 0x7f: // Backspace (DEL)
          kprintf("NORMAL:: delete  : char(DEL    | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          if (line_index == 0)
            break;
          for (int i = line_index - 1; i < MAX_LINE_LENGTH - 1; i++)
            curr_line[i] = curr_line[i + 1];
          cc_printf("\b");
          erase_cursor_to_end_of_line(UART0);
          cc_printf("%s", &curr_line[line_index - 1]);
          cc_memmove(history[0], curr_line, cc_strlen(curr_line));
          history_index = 0;
          cursor.col--;
          line_index--;
          set_cursor(UART0, cursor.row, cursor.col);
          break;

        default: // All normal chars
          kprintf("NORMAL:: default : char(%c      | %3d)   -   cursor(%3d | %3d)\r\n", c, (int)c, cursor.row, cursor.col);
          for (int i = MAX_LINE_LENGTH - 2; i > line_index; i--)
            curr_line[i] = curr_line[i - 1];
          curr_line[line_index] = c;
          erase_cursor_to_end_of_line(UART0);
          cc_printf("%s", &curr_line[line_index]);
          cc_memmove(history[0], curr_line, cc_strlen(curr_line));
          history_index = 0;
          cursor.col++;
          line_index++;
          set_cursor(UART0, cursor.row, cursor.col);
      }
    }
    else if (state == ESC)
    {
      switch (c)
      {
        case '[': // Step to handle special characters (ANSI strings)
          kprintf("ESC::    bracket : char(%c      | %3d)   -   cursor(%3d | %3d)\r\n", c, (int)c, cursor.row, cursor.col);
          state = ARROWS;
          break;

        default: // Handle undefined cases
          kprintf("ESC::    default : char(%c      | %3d)   -   cursor(%3d | %3d)\r\n", c, (int)c, cursor.row, cursor.col);
          state = NORMAL;
      }
    }
    else if (state == ARROWS)
    {
      switch (c)
      {
        case 'A': // Up arrow
          kprintf("ARROWS:: ^^^^^^^ : char(UP     | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          if (!(history_index < HISTORY_LENGTH && history[history_index + 1][0] != '\0'))
          {
            state = NORMAL;
            break;
          }
          history_index++;
          cc_memmove(curr_line, history[history_index], MAX_LINE_LENGTH);
          line_index = cc_strlen(curr_line);
          set_cursor(UART0, cursor.row, cc_strlen(SHELL_PROMPT) + 1);
          erase_cursor_to_end_of_line(UART0);
          cc_printf("%s", curr_line);
          set_cursor(UART0, cursor.row, cc_strlen(SHELL_PROMPT) + line_index + 1);
          state = NORMAL;
          break;

        case 'B': // Down arrow
          kprintf("ARROWS:: vvvvvvv : char(DOWN   | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          if (history_index <= 0)
          {
            state = NORMAL;
            break;
          }
          history_index--;
          cc_memmove(curr_line, history[history_index], MAX_LINE_LENGTH);
          line_index = cc_strlen(curr_line);
          set_cursor(UART0, cursor.row, cc_strlen(SHELL_PROMPT) + 1);
          erase_cursor_to_end_of_line(UART0);
          cc_printf("%s", curr_line);
          set_cursor(UART0, cursor.row, cc_strlen(SHELL_PROMPT) + line_index + 1);
          state = NORMAL;
          break;

        case 'C': // Right arrow
          kprintf("ARROWS:: >>>>>>> : char(RIGHT  | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          if (cursor.col > cc_strlen(SHELL_PROMPT) + cc_strlen(curr_line))
          {
            state = NORMAL;
            break;
          }
          set_cursor(UART0, cursor.row, cursor.col + 1);
          line_index++;
          state = NORMAL;
          break;

        case 'D': // Left arrow
          kprintf("ARROWS:: <<<<<<< : char(LEFT   | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          if (line_index <= 0)
          {
            state = NORMAL;
            break;
          }
          set_cursor(UART0, cursor.row, cursor.col - 1);
          line_index--;
          state = NORMAL;
          break;

        case '3': // Step to handle the Delete key
          state = DEL;
          break;

        default: // Handle undefined cases
          kprintf("ARROWS:: default : char(%c     | %3d)   -   cursor(%3d | %3d)\r\n", c, (int)c, cursor.row, cursor.col);
          state = NORMAL;
      }
    }
    else if (state == DEL)
    {
      switch (c)
      {
        case '~': // Delete
          kprintf("DELETE:: delete  : char(DEL    | %3d)   -   cursor(%3d | %3d)\r\n", (int)c, cursor.row, cursor.col);
          if (curr_line[line_index] == '\0')
            break;
          for (int i = line_index; i < MAX_LINE_LENGTH - 2; i++)
            curr_line[i] = curr_line[i + 1];
          erase_cursor_to_end_of_line(UART0);
          cc_printf("%s", &curr_line[line_index]);
          cc_memmove(history[0], curr_line, cc_strlen(curr_line));
          history_index = 0;
          set_cursor(UART0, cursor.row, cursor.col);
          break;

        default: // Handle undefined cases
          kprintf("DELETE:: default : char(%c     | %3d)   -   cursor(%3d | %3d)\r\n", c, (int)c, cursor.row, cursor.col);
      }
      state = NORMAL;
    }
  }
}
