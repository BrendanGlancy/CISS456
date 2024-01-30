#include "pdr.hpp"

std::string PDR::input_prompt(const std::string &prompt, bool clear) {
  if (clear)
    clear_input_buffer();

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

    return input;

    std::cout << "Invalid option. Please try again." << std::endl;
  }
}

std::string PDR::set_fname() { return input_prompt("First Name: ", false); }

std::string PDR::set_minitial() {
  return input_prompt("Middle Initial: ", true);
}

std::string PDR::set_lname() { return input_prompt("Last Name: ", false); }

std::string PDR::set_ssn() {
  return input_prompt("Social Security Number: ", false);
}

std::string PDR::set_address() { return input_prompt("Address: ", false); }

std::string PDR::set_city() { return input_prompt("City: ", false); }

std::string PDR::set_state() { return input_prompt("State Code: ", false); }

std::string PDR::set_zip() { return input_prompt("Zip Code: ", false); }

bool PDR::valid_ssn(const std::string &ssn) {
  // SSN must be 9 digits or 11 characters including dashes
  if (ssn.length() != 9 && ssn.length() != 11)
    return false;

  for (size_t i = 0; i < ssn.length(); ++i) {
    if (i == 3 || i == 6) {
      if (ssn.length() == 11 && ssn[i] != '-')
        return false; // Dashes at correct positions
    } else {
      if (!isdigit(ssn[i]))
        return false; // Every other character must be a digit
    }
  }
  return true;
}

bool PDR::valid_name(const std::string &name) {
  if (name.empty())
    return false;

  for (char c : name) {
    if (!isalpha(c) && c != '-' && c != '\'')
      return false;
  }
  return true;
}

bool PDR::valid_state(const std::string &state) {
  const std::set<std::string> valid_states = {
      "AL", // Alabama
      "AK", // Alaska
      "AZ", // Arizona
      "AR", // Arkansas
      "CA", // California
      "CO", // Colorado
      "CT", // Connecticut
      "DE", // Delaware
      "FL", // Florida
      "GA", // Georgia
      "HI", // Hawaii
      "ID", // Idaho
      "IL", // Illinois
      "IN", // Indiana
      "IA", // Iowa
      "KS", // Kansas
      "KY", // Kentucky
      "LA", // Louisiana
      "ME", // Maine
      "MD", // Maryland
      "MA", // Massachusetts
      "MI", // Michigan
      "MN", // Minnesota
      "MS", // Mississippi
      "MO", // Missouri
      "MT", // Montana
      "NE", // Nebraska
      "NV", // Nevada
      "NH", // New Hampshire
      "NJ", // New Jersey
      "NM", // New Mexico
      "NY", // New York
      "NC", // North Carolina
      "ND", // North Dakota
      "OH", // Ohio
      "OK", // Oklahoma
      "OR", // Oregon
      "PA", // Pennsylvania
      "RI", // Rhode Island
      "SC", // South Carolina
      "SD", // South Dakota
      "TN", // Tennessee
      "TX", // Texas
      "UT", // Utah
      "VT", // Vermont
      "VA", // Virginia
      "WA", // Washington
      "WV", // West Virginia
      "WI", // Wisconsin
      "WY"  // Wyoming
  };
  return valid_states.find(state) != valid_states.end();
}

bool PDR::valid_zip(const std::string &zip) {
  if (zip.length() != 5 && zip.length() != 10)
    return false;

  for (size_t i = 0; i < zip.length(); ++i) {
    if (i == 5) {
      if (zip[i] != '-')
        return false; // If length is 10, position 6 must be a hyphen
    } else {
      if (!isdigit(zip[i]))
        return false; // Every other character must be a digit
    }
  }
  return true;
}

std::string PDR::get_valid_input(
    const std::string &prompt,
    const std::function<bool(const std::string &)> &validator) {
  std::string input;
  do {
    input = input_prompt(prompt, false);
    if (!validator(input)) {
      std::cout << "Invalid input. Please try again." << std::endl;
    }
  } while (!validator(input));
  return input;
}

void PDR::collect_data() {
  try {
    patient.firstName =
        get_valid_input("First Name: ", [this](const std::string &input) {
          return valid_name(input);
        });
    patient.middleInitial =
        get_valid_input("Middle Initial: ", [this](const std::string &input) {
          return valid_name(input);
        });
    patient.lastName =
        get_valid_input("Last Name: ", [this](const std::string &input) {
          return valid_name(input);
        });
    patient.ssn = get_valid_input(
        "Social Security Number: ",
        [this](const std::string &input) { return valid_ssn(input); });
    patient.address =
        get_valid_input("Address: ", [this](const std::string &input) {
          return true;
        }); // Assuming address validation always returns true for now
    patient.city = get_valid_input(
        "City: ", [this](const std::string &input) { return true; });
    patient.stateCode =
        get_valid_input("State Code: ", [this](const std::string &input) {
          return valid_state(input);
        });
    patient.zip =
        get_valid_input("Zip Code: ", [this](const std::string &input) {
          return valid_zip(input);
        });

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
