#include "pdr.hpp"

#include <regex>

/**
 * Seperate function to display the prompt for an input
 * This is because we are checking for q or Q every time incase
 * the user wants to quit
 */
string PDR::input_prompt(const string &prompt) {
  std::cout << prompt;
  string input;
  getline(std::cin, input);
  if (input == "q" || input == "Q") throw UserQuitException();

  return input;
}

/**
 * Returns the input unless its a q, then we throw
 * @UserQuitException which is s a callback to the main menu
 */
string PDR::get_input(const string &input) {
  while (true) {
    char choice = toupper(input_prompt(input)[0]);

    if (choice == 'Q' && input.size() == 1) throw UserQuitException();

    return input;

    std::cout << "Invalid option. Please try again." << std::endl;
  }
}

/**
 * All the below function simply pass a prompt such as "First Name: "
 * @returns the field we are looking for as type string
 * if we need input validation, we call a seperate input validation function the
 * check
 */
string PDR::set_icd_code() {
  string input;
  do {
    input = input_prompt("ICD-10 Code: ");
    if (!db.is_valid_icd_code(input)) {
      std::cout << "Invalid ICD-10 Code. Please try again." << std::endl;
    }
  } while (!valid_ssn(input));
  return input;
}

string PDR::set_injury_date() {
  // TODO, add an option to select the current date or enter manually
  string input;
  do {
    input = input_prompt("Injury Date (YYYY-MM-DD): ");
    if (!valid_date(input)) {
      std::cout << "Invalid ICD-10 Code. Please try again." << std::endl;
    }
  } while (!valid_ssn(input));
  return input;
}

string PDR::set_description() {
  return input_prompt("Description of Injury: ");
}

bool PDR::lname_ssn() {
  string input;
  do {
    input = input_prompt("Find User By Last Name or SSN [L/s]");
    if (!input.empty()) {
      input[0] =
          std::toupper(input[0]);  // Convert the first character to upper case
    }
    if (input == "L") {
      return true;
    }
  } while (input != "S" || input != "L");
  return false;
}

bool PDR::valid_ssn(const string &ssn) {
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

bool PDR::valid_state(const string &state) {
  return db.is_valid_state(state);  // Delegate to ChinookDB instance
}

bool PDR::match_lname(const string &last_name) {
  return db.match_user(last_name);
}

bool PDR::match_ssn(const string &ssn) { return db.match_user(ssn); }

bool PDR::valid_date(const string &date) {
  return regex_match(date, std::regex("\\d{4}-\\d{2}-\\d{2}"));
}

void PDR::update_or_add_injury() {
  // needs refactored to many nest
  try {
    // Check if updating by last name or SSN
    if (lname_ssn()) {
      if (!match_lname(patient.last_name)) {
        std::cout << "No matching records found. Would you like to add a new "
                     "record? [Y/N]"
                  << std::endl;
        if (toupper(input_prompt("Choice: ")[0]) == 'Y') {
          // pass
        }
      } else {
        // pass
      }
    }
  } catch (const UserQuitException &e) {
    std::cerr << "User chose to quit: " << e.what() << std::endl;
    if (Menu_Callback) {
      Menu_Callback();
    }
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    if (Menu_Callback) {
      Menu_Callback();
    }
  }
}
