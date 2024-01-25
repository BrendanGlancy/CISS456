#include "pdr.hpp"

bool valid_ssn(const std::string &ssn) {
  return (ssn.length() == 9 && std::all_of(ssn.begin(), ssn.end(), isdigit));
}

bool valid_name(const std::string &name) { return !name.empty(); }

bool valid_state(const std::string &state) { return !state.empty(); }

bool valid_zip(const std::string &zip) {
  return std::all_of(zip.begin(), zip.end(), isdigit);
}

std::string PDR::input_prompt(const std::string &prompt) {
  bool clear = true;
  if (clear) clear_input_buffer();

  std::cout << prompt;
  std::string input;
  getline(std::cin, input);
  if (input == "q" || input == "Q") throw UserQuitException();

  return input;
}

std::string PDR::get_input(const std::string &input) {
  while (true) {
    char choice = toupper(input_prompt(input)[0]);

    if (choice == 'Q' && input.size() == 1) throw UserQuitException();

    return input;

    std::cout << "Invalid option. Please try again." << std::endl;
  }
}

bool PDR::validate_input() {
  bool valid;
  std::string input;

  // Call all validation functions and set 'valid' to false if any fails
  if (!valid_ssn(input) || !valid_name(input) || !valid_state(input) ||
      !valid_zip(input)) {
    valid = false;
    std::cout << "Invalid input, please try again." << std::endl;
  } else {
    // All validations passed, set 'valid' to true
    valid = true;
  }

  return valid;
}

std::string PDR::set_fname() {
  return get_input(input_prompt("Color (W)hite, (G)ray, (B)lack: "));
}

std::string PDR::set_minitial() {
  return get_input(input_prompt("Middle Initial: "));
}

std::string PDR::set_lname() { return get_input(input_prompt("Last Name: ")); }

std::string PDR::set_ssn() {
  return get_input(input_prompt("Social Security Number: "));
}

std::string PDR::set_address() { return get_input(input_prompt("Address: ")); }

std::string PDR::set_city() { return get_input(input_prompt("City: ")); }

std::string PDR::set_state() { return get_input(input_prompt("State Code: ")); }

std::string PDR::set_zip() { return get_input(input_prompt("Zip Code: ")); }

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
    patient.valid = PDR::validate_input();

  } catch (const std::exception &e) {
    if (Menu_Callback) {
      Menu_Callback();
    }
  }
}

void PDR::save_data() {
  if (patient.valid == false) throw std::runtime_error("Information Not Valid");

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
