#include <iostream>
#include <vector>

#include "lib/header.h"
#include "src/menu.h"
#include "src/pdr.hpp"

void handleCollectData(std::vector<PDR *> &patient_info, char *message,
                       int &patient_objs) {
  patient_info.push_back(new PDR());
  // you dont actually need to keep track of these anymore
  patient_objs = patient_info.size();
  clear_console();
  snprintf(message, sizeof("   Data save success   "),
           "%d patient object(s) created", patient_objs);
  pdr_prompt();
  patient_info.back()->collect_data();
}

void handleStoreData(std::vector<PDR *> &patients, char *message) {
  if (!patients.empty()) {
    for (auto patient : patients) {
      patient->save_data();
    }
    patients.clear();
    strcpy(message, "   Data save success   ");
  } else {
    strcpy(message, "   No data to save     ");
  }
}

int main() {
  int count = 0;
  int patient_objs = 0;
  char *message = (char *)malloc(sizeof("  Delete data success  ") + 2);

  bool running = true;

  std::vector<PDR *> patient;

  welcome();

  while (running) {
    if (count > 0) infoHeader(message);
    displayMenu();

    int choice = getChoice();
    count++;

    switch (choice) {
      case 1:
        handleCollectData(patient, message, patient_objs);
        break;
      case 2:
        handleStoreData(patient, message);
        break;
      case 3:
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
