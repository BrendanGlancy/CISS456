#include "menu.h"

#include <stdio.h>

#include "../lib/header.h"

void welcome() {
  clear_console();
  gotoxy(0, 0, 34);
  printf("=========================================================");
  gotoxy(2, 2, 36);
  printf(" __          __    _                               _ ");
  gotoxy(2, 3, 36);
  printf(" \\ \\        / /   | |                             | |");
  gotoxy(2, 4, 36);
  printf("  \\ \\  /\\  / /___ | |  ___  ___   _ __ ___    ___ | |");
  gotoxy(2, 5, 36);
  printf("   \\ \\/  \\/ // _ \\| | / __|/ _ \\ | '_ ` _ \\  / _ \\| |");
  gotoxy(2, 6, 36);
  printf("    \\  /\\  /|  __/| || (__| (_) || | | | | ||  __/|_|");
  gotoxy(2, 7, 36);
  printf("     \\/  \\/  \\___||_| \\___|\\___/ |_| |_| |_| \\___|(_)");
  gotoxy(0, 9, 34);
  printf("========================================================="
         "\n");
}

void display_menu() {
  gotoxy(16, 11, 33);
  printf(">>>> MAIN MENU <<<<");
  gotoxy(16, 13, 32);
  printf("1. Create Encounter");
  gotoxy(16, 14, 32);
  printf("2. Edit Patient Table");
  gotoxy(16, 15, 32);
  printf("3. Edit ICD10S Table");
  gotoxy(16, 16, 32);
  printf("4. View All Tables");
  gotoxy(16, 18, 31);
  printf("5. Exit");
  gotoxy(16, 20, 34);
  printf("Enter your choice: ");
}

void pdr_prompt() {
  clear_console();
  gotoxy(0, 2, 36);
  printf("=================================================");
  gotoxy(0, 4, 36);
  printf("---=              Update Injury              =---");
  gotoxy(0, 6, 36);
  printf("---=      Please Select the Patient Info     =---");
  gotoxy(0, 8, 36);
  printf("---=           Enter [Q/q] to quit           =---");
  gotoxy(0, 10, 36);
  printf("=================================================\n");
  gotoxy(0, 11, 36);
  printf("\n");
}

// make more dynamic
void info_header(char *message, int offset) {
  clear_console();
  gotoxy(offset, 1, 36);
  printf("=================================================");
  gotoxy(offset, 3, 36);
  printf("---=                 Hello                   =---");
  gotoxy(offset, 5, 36);
  printf("---=         %s         =---", message);
  gotoxy(offset, 7, 36);
  printf("---=            Enter [2] to save            =---");
  gotoxy(offset, 9, 36);
  printf("=================================================\n");
}

void table_viewer() {
  clear_console();
  gotoxy(0, 2, 36);
  printf("=================================================");
  gotoxy(0, 4, 36);
  printf("---=               DB VIEWER                 =---");
  gotoxy(0, 6, 36);
  printf("---=       Examine the DB Configuration      =---");
  gotoxy(0, 8, 36);
  printf("---=          Press [ENTER] to quit          =---");
  gotoxy(0, 10, 36);
  printf("=================================================\n");
}

void injury_prompt() {
  clear_console();
  gotoxy(0, 2, 31);
  printf("=================================================");
  gotoxy(0, 4, 36);
  printf("---=               ENCOUNTER                 =---");
  gotoxy(0, 6, 36);
  printf("---=          Enter the Injury Info          =---");
  gotoxy(0, 8, 36);
  printf("---=          Press [ENTER] to quit          =---");
  gotoxy(0, 10, 31);
  printf("=================================================\n");
}
