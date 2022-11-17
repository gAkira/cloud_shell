#include "main.h"

void echo(int uart, const char *line);

void execute(int uart, const char *line)
{
  if (cc_strncmp(line, "reset", 5) == 0 && (line[5] == '\0' || line[5] == ' '))
    reset_screen(uart);
  else if (cc_strncmp(line, "echo", 4) == 0 && (line[4] == '\0' || line[4] == ' '))
    echo(uart, line);
}


void echo(int uart, const char *line)
{
  int start_of_string = 4;

  while (line && line[start_of_string] != '\0' && line[start_of_string] == ' ')
    start_of_string++;
  uart_send_string(uart, &line[start_of_string]);
  cursor_to_beginning_of_next_line(uart);
}
