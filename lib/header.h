#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

/**
 * Moves the cursor to a specific position in the console and sets the text
 * color.
 *
 * @param x The x-coordinate (column) of the cursor's new position.
 * @param y The y-coordinate (row) of the cursor's new position.
 * @param color_code The color code for the text. If -1, the color is not
 * changed.
 */
void gotoxy(int x, int y, int color_code);
void reset_text_color();

/**
 * Clears the console screen.
 *
 * This function uses system-specific commands to clear the console screen.
 * It uses 'cls' for Windows and 'clear' for Unix-based systems.
 */
void clear_console();
void clear_input_buffer();

#ifdef __cplusplus
}
#endif
