#include <stdio.h>
#include <stdlib.h>

#include "header.h"

void gotoxy(int x, int y, int color_code) {
  printf("\033[%d;%dH", y, x);
  if (color_code != -1) {
    printf("\033[1;%dm", color_code);
  }
}

void reset_text_color() { printf("\033[0m"); }

void clear_input_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void clear_console() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
