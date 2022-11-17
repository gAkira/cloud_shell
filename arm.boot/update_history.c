#include "main.h"

static int is_empty(const char *line);

void update_history(void)
{
  if (!is_empty(history[0]))
    for (int i = HISTORY_LENGTH - 1; i > 0; i--)
      cc_memcpy(history[i], history[i - 1], MAX_LINE_LENGTH);
  cc_bzero(history[0], MAX_LINE_LENGTH);
  cc_bzero(curr_line, MAX_LINE_LENGTH);
  line_index = 0;
  history_index = 0;
}

static int is_empty(const char *line)
{
  for (int i = 0; line[i] != '\0'; i++)
    if (line[i] != ' ' && line[i] != '\t')
      return (0);
  return (1);
}
