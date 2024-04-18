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

void collect_data(PDR &patient_info, char *message) {
  clear_console();
  // Assuming pdr_prompt is a global function that's available
  pdr_prompt();
  patient_info.update_or_add_injury();
}

void store_data(PDR &patient_info, char *message) {
  std::cout << "Not Implemented" << std::endl;
}

int get_choice() {
  string input;
  int choice;

  while (true) {
    getline(std::cin, input);
    istringstream stream(input);

    if (stream >> choice && stream.eof()) {
      return choice;
    }

    printf("Invalid Input, Try again: ");
  }
}

int main() {
  char message[256] = " Welcome to PDR System ";

  bool running = true;

  DB_Manager db_controller; // Initializes the database upon construction.
  PDR patient_info(db_controller);

  welcome();

  while (running) {
    info_header(message);
    display_menu();

    int choice = get_choice();

    switch (choice) {
    case 1:
      collect_data(patient_info, message);
      break;
    case 2:
      store_data(patient_info, message);
      break;
    case 3:
      clear_console();
      table_viewer();
      db_controller.view_tables();
      break;
    case 4:
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
