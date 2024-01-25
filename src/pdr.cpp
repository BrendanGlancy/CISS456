#include "pdr.hpp"
#include <iostream>

std::string PDR::input_prompt(const std::string &prompt, bool clear) {
  std::cout << prompt;
  std::string input;
  getline(std::cin, input);
  if (input == "q" || input == "Q")
    throw UserQuitException();

  return input;
}

std::string PDR::get_input(const std::string &input) {
  while (true) {
    char choice = toupper(input_prompt(input, false)[0]);

    if (choice == 'Q' && input.size() == 1)
      throw UserQuitException();

    std::cout << "Invalid option. Please try again." << std::endl;
  }
  return input;
}

std::string PDR::set_fname() {
  return get_input(input_prompt("First Name: ", true));
}

std::string PDR::set_minitial() {
  return get_input(input_prompt("Middle Initial: ", false));
}

std::string PDR::set_lname() {
  return get_input(input_prompt("Last Name: ", false));
}

std::string PDR::set_ssn() {
  return get_input(input_prompt("Social Security Number: ", false));
}

std::string PDR::set_address() {
  return get_input(input_prompt("Address: ", false));
}

std::string PDR::set_city() { return get_input(input_prompt("City: ", false)); }

std::string PDR::set_state() {
  return get_input(input_prompt("State Code: ", false));
}

std::string PDR::set_zip() {
  return get_input(input_prompt("Zip Code: ", false));
}

void PDR::collect_data() {
  try {
    patient.firstName = PDR::set_fname();
    patient.middleInitial = PDR::set_minitial();
    patient.lastName = PDR::set_lname();
    patient.ssn = PDR::set_ssn();
    patient.address = PDR::set_address();
    patient.city = PDR::set_city();
    patient.stateCode = PDR::set_state();
    patient.zip = PDR::set_zip();

  } catch (const std::exception &e) {
    if (Menu_Callback) {
      Menu_Callback();
    }
  }
}

void PDR::save_data() {
  std::ofstream outputFile("output.txt");
  if (!outputFile.is_open()) {
    throw std::runtime_error("Error opening file");
  }
  outputFile << "First Name: " << patient.firstName << "\n"
             << "Middle Initial: " << patient.middleInitial << "\n"
             << "Last Name: " << patient.lastName << "\n"
             << "Social Security: " << patient.ssn << "\n"
             << "Address: " << patient.address << "\n"
             << "City: " << patient.city << "\n"
             << "State Code: " << patient.stateCode << "\n"
             << "Zip: " << patient.zip << "\n";
  outputFile.close();
}
