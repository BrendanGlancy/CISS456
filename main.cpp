#include <iostream>
#include <memory> // For std::unique_ptr
#include <sstream>
#include <vector>

#include "lib/header.h"          // Ensure this header is necessary or exists.
#include "src/createchinook.hpp" // Note the updated file name to match conventions.
#include "src/menu.h"
#include "src/pdr.hpp"

void handleCollectData(std::vector<std::unique_ptr<PDR>> &patient_info,
                       ChinookDB &db, char *message) {
  patient_info.push_back(std::make_unique<PDR>(db));
  snprintf(message, sizeof("   Data save success   "),
           "%lu pdr object(s) created", patient_info.size());
  clear_console();
  // Assuming pdr_prompt is a global function that's available
  pdr_prompt();
  patient_info.back()->collect_data();
}

void handleStoreData(const std::vector<std::unique_ptr<PDR>> &patients,
                     char *message) {
  if (!patients.empty()) {
    for (const auto &patient : patients) {
      patient->save_data();
    }
    strcpy(message, "   Data save success   ");
  } else {
    strcpy(message, "   No data to save     ");
  }
}

int get_choice() {
  std::string input;
  int choice;

  while (true) {
    getline(std::cin, input);
    std::istringstream stream(input);

    if (stream >> choice && stream.eof()) {
      return choice;
    }

    printf("Invalid Input, Try again: ");
  }
}

int main() {
  char message[256] = " Welcome to PDR System ";

  bool running = true;

  std::vector<std::unique_ptr<PDR>> patient_info;
  ChinookDB db_controller; // Initializes the database upon construction.

  welcome();

  while (running) {
    infoHeader(message);
    displayMenu();

    int choice = get_choice();

    switch (choice) {
    case 1:
      handleCollectData(patient_info, db_controller, message);
      break;
    case 2:
      handleStoreData(patient_info, message);
      break;
    case 3:
      // Implement any additional options if necessary.
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
