#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#include "../lib/header.h"

/**
 * Displays a welcome message to the user.
 * This function is typically called at the beginning of the program.
 */
void welcome();

/**
 * Displays the main menu of the application.
 * This function lists the available options for the user to choose from.
 */
void display_menu();

/**
 * Prompts the user to configure a car.
 * This function guides the user through the process of entering car details.
 */
void pdr_prompt();
void new_pdr();

/**
 * Prompts the user to configure a car.
 * This function guides the user through the process of entering car details.
 */
void injury_prompt();

/**
 * Displays a header for informational messages.
 *
 * @param message The message to be displayed in the header.
 */
void info_header(char message[], int offseT);

/**
 * Displays a prompt for an admin db manager to login.
 */
void pdr_admin();
void table_viewer();

#ifdef __cplusplus
}
#endif
