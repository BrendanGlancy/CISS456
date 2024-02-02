#include "pdr.hpp"

std::string PDR::input_prompt(const std::string &prompt, bool clear) {
  if (clear) clear_input_buffer();

  std::cout << prompt;
  std::string input;
  getline(std::cin, input);
  if (input == "q" || input == "Q") throw UserQuitException();

  return input;
}

std::string PDR::get_input(const std::string &input) {
  while (true) {
    char choice = toupper(input_prompt(input, false)[0]);

    if (choice == 'Q' && input.size() == 1) throw UserQuitException();

    return input;

    std::cout << "Invalid option. Please try again." << std::endl;
  }
}

std::string PDR::set_fname() {
  std::string input;
  do {
    input = input_prompt("First Name: ", true);
    if (!valid_name(input)) {
      std::cout << "Invalid first name. Please try again." << std::endl;
    }
  } while (!valid_name(input));
  return input;
}

std::string PDR::set_minitial() {
  std::string input;
  do {
    input = input_prompt("Middle Initial: ", true);
    if (!valid_name(input)) {
      std::cout << "Invalid middle initial. Please try again." << std::endl;
    }
  } while (!valid_name(input));
  return input;
}

std::string PDR::set_lname() {
  std::string input;
  do {
    input = input_prompt("Last Name: ", false);
    if (!valid_name(input)) {
      std::cout << "Invalid last name. Please try again." << std::endl;
    }
  } while (!valid_name(input));
  return input;
}

std::string PDR::set_ssn() {
  std::string input;
  do {
    input = input_prompt("Social Security Number: ", false);
    if (!valid_ssn(input)) {
      std::cout << "Invalid SSN. Please try again." << std::endl;
    }
  } while (!valid_ssn(input));
  return input;
}

std::string PDR::set_address() {
  std::string input;
  do {
    input = input_prompt("Address: ", false);
    if (input.size() < 5) {
      std::cout << "Invalid Address. Please try again." << std::endl;
    }
  } while (input.size() < 5);
  return input;
}

std::string PDR::set_city() {
  std::string input;
  do {
    input = input_prompt("City: ", false);
    if (input.size() < 3) {
      std::cout << "Invalid City. Please try again." << std::endl;
    }
  } while (input.size() < 3);
  return input;
}

std::string PDR::set_state() {
  std::string input;
  do {
    input = input_prompt("State Code (EX: NY): ", false);
    if (!valid_state(input)) {
      std::cout << "Invalid State Code. Please try again." << std::endl;
    }
  } while (!valid_state(input));
  return input;
}

std::string PDR::set_zip() {
  std::string input;
  do {
    input = input_prompt("Zip Code: ", false);
    if (!valid_zip(input)) {
      std::cout << "Invalid State Code. Please try again." << std::endl;
    }
  } while (!valid_zip(input));
  return input;
}

bool PDR::valid_ssn(const std::string &ssn) {
  // SSN must be 9 digits or 11 characters including dashes
  if (ssn.length() != 9 && ssn.length() != 11) return false;

  for (size_t i = 0; i < ssn.length(); ++i) {
    if (i == 3 || i == 6) {
      if (ssn.length() == 11 && ssn[i] != '-')
        return false;  // Dashes at correct positions
    } else {
      if (!isdigit(ssn[i]))
        return false;  // Every other character must be a digit
    }
  }
  return true;
}

bool PDR::valid_name(const std::string &name) {
  if (name.empty()) return false;

  for (char c : name) {
    if (!isalpha(c) && c != '-' && c != '\'') return false;
  }
  return true;
}

bool PDR::valid_initial(const std::string &name) {
  if (name.empty()) return false;

  for (char c : name) {
    if (!isalpha(c) && c != '-' && c != '\'' && name.size() > 1) return false;
  }
  return true;
}

bool PDR::valid_state(const std::string &state) {
  const std::set<std::string> valid_states = {
      "AL",  // Alabama
      "AK",  // Alaska
      "AZ",  // Arizona
      "AR",  // Arkansas
      "CA",  // California
      "CO",  // Colorado
      "CT",  // Connecticut
      "DE",  // Delaware
      "FL",  // Florida
      "GA",  // Georgia
      "HI",  // Hawaii
      "ID",  // Idaho
      "IL",  // Illinois
      "IN",  // Indiana
      "IA",  // Iowa
      "KS",  // Kansas
      "KY",  // Kentucky
      "LA",  // Louisiana
      "ME",  // Maine
      "MD",  // Maryland
      "MA",  // Massachusetts
      "MI",  // Michigan
      "MN",  // Minnesota
      "MS",  // Mississippi
      "MO",  // Missouri
      "MT",  // Montana
      "NE",  // Nebraska
      "NV",  // Nevada
      "NH",  // New Hampshire
      "NJ",  // New Jersey
      "NM",  // New Mexico
      "NY",  // New York
      "NC",  // North Carolina
      "ND",  // North Dakota
      "OH",  // Ohio
      "OK",  // Oklahoma
      "OR",  // Oregon
      "PA",  // Pennsylvania
      "RI",  // Rhode Island
      "SC",  // South Carolina
      "SD",  // South Dakota
      "TN",  // Tennessee
      "TX",  // Texas
      "UT",  // Utah
      "VT",  // Vermont
      "VA",  // Virginia
      "WA",  // Washington
      "WV",  // West Virginia
      "WI",  // Wisconsin
      "WY"   // Wyoming
  };
  return valid_states.find(state) != valid_states.end();
}

bool PDR::valid_zip(const std::string &zip) {
  if (zip.length() != 5 && zip.length() != 10) return false;

  for (size_t i = 0; i < zip.length(); ++i) {
    if (i == 5) {
      if (zip[i] != '-')
        return false;  // If length is 10, position 6 must be a hyphen
    } else {
      if (!isdigit(zip[i]))
        return false;  // Every other character must be a digit
    }
  }
  return true;
}

void PDR::collect_data() {
  try {
    patient.firstName = set_fname();
    patient.middleInitial = set_minitial();
    patient.lastName = set_lname();
    patient.ssn = set_ssn();
    patient.address = set_address();
    patient.city = set_city();
    patient.stateCode = set_state();
    patient.zip = set_zip();
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
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
  outputFile << patient.firstName << ", " << patient.middleInitial << ", "
             << patient.lastName << ", " << patient.ssn << ", "
             << patient.address << ", " << patient.city << ", "
             << patient.stateCode << ", " << patient.zip;
  outputFile.close();
}
