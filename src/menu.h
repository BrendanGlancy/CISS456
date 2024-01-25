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
void displayMenu();

/**
 * Prompts the user to configure a car.
 * This function guides the user through the process of entering car details.
 */
void pdr_prompt();

/**
 * Retrieves the user's choice from the menu.
 *
 * @return The integer value representing the user's choice.
 */
int getChoice();

/**
 * Displays a header for informational messages.
 *
 * @param message The message to be displayed in the header.
 */
void infoHeader(char message[]);

#ifdef __cplusplus
}
#endif
