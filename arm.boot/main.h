/*
 * test.h
 *
 *  Created on: Jan 12, 2021
 *      Author: ogruber
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stddef.h>
#include <stdint.h>

// My includes
#include "cc_libc.h"
#include "utils.h"

#define MAX_LINE_LENGTH 300
#define HISTORY_LENGTH 21
#define SHELL_PROMPT "cloud_shell> "

// Struct to deal with cursor position
typedef struct _Cursor {
  unsigned int row;
  unsigned int col;
} Cursor;

// Enum to handle ANSI strings. Used to implement a FSM
typedef enum _State {
  NORMAL,
  ESC,
  ARROWS,
  DEL
} State;

// Global variable to keep track of history and ther current working line
extern char history[][MAX_LINE_LENGTH];
extern char curr_line[MAX_LINE_LENGTH];
extern int line_index;
extern int history_index;

// Global cursor
extern Cursor cursor;

// Global state of the FSM
extern State state;

// To stop the warning about implicit declaration of kprintf and cc_printf
void kprintf(const char *fmt, ...);
void cc_printf(const char *fmt, ...);
void execute(int uart, const char *line);
void update_history(void);

/*
 * Board constantes such as BARs
 */
#include "board.h"

#endif /* MAIN_H_ */
