#include "pdr.hpp"
#include "db_controller.hpp"
#include <cctype>

/**
 * Seperate function to display the prompt for an input
 * This is because we are checking for q or Q every time incase
 * the user wants to quit
 */
string PDR::input_prompt(const string &prompt) {
  std::cout << prompt;
  string input;
  getline(std::cin, input);
  if (input == "q" || input == "Q")
    throw UserQuitException();

  return input;
}

/**
 * Returns the input unless its a q, then we throw
 * @UserQuitException which is s a callback to the main menu
 */
string PDR::get_input(const string &input) {
  while (true) {
    char choice = toupper(input_prompt(input)[0]);

    if (choice == 'Q' && input.size() == 1)
      throw UserQuitException();

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

string PDR::set_lname() {
  string input;
  do {
    input = input_prompt("Last Name: ");
    if (!valid_name(input)) {
      std::cout << "Invalid last name. Please try again." << std::endl;
    }
  } while (!valid_name(input));
  return input;
}

string PDR::set_ssn() {
  string input;
  do {
    input = input_prompt("Social Security Number: ");
    if (!valid_ssn(input)) {
      std::cout << "Invalid SSN. Please try again." << std::endl;
    }
  } while (!valid_ssn(input));
  return input;
}

string PDR::set_state() {
  string input;
  do {
    input = input_prompt("State Code (EX: NY): ");
    std::transform(input.begin(), input.end(), input.begin(),
                   ::toupper);       // Convert to upper case
    if (!db.is_valid_state(input)) { // Use the db instance for state validation
      std::cout << "Invalid State Code. Please try again." << std::endl;
    }
  } while (!db.is_valid_state(input));
  return input;
}

// this could return the patient info
optional<PatientRecord> PDR::lname_ssn() {
  db.view_patients();
  while (true) {
    string input = input_prompt("Find Patient by Last Name or SSN [L/s]: ");
    if (!input.empty()) {
      char choice = toupper(input[0]);
      if (choice == 'L') {
        string lname = set_lname();
        return match_lname(lname);
      } else if (choice == 'S') {
        string ssn = set_ssn();
        return match_ssn(ssn);
      }
    }
  }
}

bool PDR::valid_ssn(const string &ssn) {
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

bool PDR::valid_name(const string &name) {
  if (name.empty())
    return false;

  for (char c : name) {
    if (!isalpha(c) && c != '-' && c != '\'')
      return false;
  }
  return true;
}

bool PDR::valid_state(const string &state) {
  return db.is_valid_state(state); // Delegate to ChinookDB instance
}

optional<PatientRecord> PDR::match_lname(const string &last_name) {
  string modified_name = last_name;
  std::transform(modified_name.begin(), modified_name.end(),
                 modified_name.begin(),
                 [](unsigned char c) { return std::toupper(c); });

  return db.match_user(modified_name);
}

optional<PatientRecord> PDR::match_ssn(const string &ssn) {
  return db.match_user(ssn);
}

bool PDR::valid_date(const string &date) {
  return regex_match(date, std::regex("\\d{4}-\\d{2}-\\d{2}"));
}

void PDR::update_or_add_injury() {
  try {
    // Attempt to find a patient by last name or SSN.
    std::optional<PatientRecord> curr_patient = lname_ssn();
    if (curr_patient) {
      // If a patient record is found, display the patient's details.
      std::cout << "Patient found: " << std::endl;
      std::cout << "SSN: " << curr_patient->ssn << std::endl;
      std::cout << "Last Name: " << curr_patient->last_name << std::endl;
      std::cout << "Position: " << curr_patient->position << std::endl;
      std::cout << "Last Service Date: " << curr_patient->service_date
                << std::endl;
      std::cout << "State Code: " << curr_patient->state_code << std::endl;

      // Optionally, update or add an injury entry.
      std::string icd_code = set_icd_code();
      std::string injury_date = set_injury_date();
      std::string description = set_description();

    } else {
      std::cout << "No patient found. Please try again or add a new patient."
                << std::endl;

      std::cout << "Press <ENTER> To return to Main Menu" << std::endl;
      char input;

      do {
        input = getchar();
      } while (input != '\n');
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

PDR::~PDR() {}
