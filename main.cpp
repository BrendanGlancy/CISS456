#include <iostream>
#include <memory> // For std::unique_ptr
#include <sstream>
#include <vector>

#include "lib/header.h"          // Ensure this header is necessary or exists.
#include "src/db_controller.hpp" // Note the updated file name to match conventions.
#include "src/menu.h"
#include "src/pdr.hpp"

using std::istringstream;
using std::string;

void prompt_init(char *message) {
  clear_console();
  info_header(message, 0);
}

void collect_data(PDR &patient_info, char *message) {
  prompt_init(message);
  patient_info.update_encounter();
}

void patients(PDR &patient_info) {
  char message[256] = " Welcome Patient Admin ";
  prompt_init(message);
  patient_info.edit_patients();
}

void ICD10S(PDR &patient_info) {
  char message[256] = "  Welcome ICD10 Admin  ";
  prompt_init(message);
  patient_info.edit_ICD10S();
}

int get_choice() {
  string input;
  int choice;

  while (true) {
    getline(std::cin, input);
    istringstream stream(input);

    if (stream >> choice && stream.eof())
      return choice;

    printf("Invalid Input, Try again: ");
  }
}

int main() {
  char message[256] = "  Saved PDR to DB  ";

  bool running = true;

  DB_Manager db_controller; // Initializes the database upon construction.
  PDR patient_info(db_controller);

  welcome();

  while (running) {
    info_header(message, 4);
    display_menu();

    int choice = get_choice();

    switch (choice) {
    case 1:
      // New Encounter
      collect_data(patient_info, message);
      break;
    case 2:
      patients(patient_info);
      clear_console();
      // table_viewer();
      break;
    case 3:
      ICD10S(patient_info);
      clear_console();
      // table_viewer();
      break;
    case 4:
      // View all Tables
      clear_console();
      table_viewer();
      db_controller.view_tables();
      break;
    case 5:
      // Exit
      clear_console();
      reset_text_color();
      running = false;
      break;
    default:
      std::cout << "Invalid choice" << std::endl;
      break;
    }
  }

  return 0;
}
