#include "pdr.hpp"
#include <cctype>
#include <ctime>
#include <iostream>

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
 * Below functions simply pass a prompt such as "First Name: "
 * @returns the field we are looking for as type string
 * if we need input validation, we call a seperate input validation function the
 * check
 */
string PDR::set_icd_code() {
  string input;
  do {
    input = input_prompt("\033[1;36mICD-10 Code: ");
    if (!db.is_valid_icd_code(input))
      std::cout << "Invalid ICD-10 Code. Please try again." << std::endl;
  } while (!db.is_valid_icd_code(input));
  return input;
}

string PDR::set_injury_date() {
  string input;
  time_t now = time(0);
  tm *timeinfo = localtime(&now);
  char buffer[80];

  do {
    input = input_prompt("Injury Date (YYYY-MM-DD) [T] for Today: ");
    if (toupper(input[0]) == 'T') {
      std::strftime(buffer, sizeof("YYYY-MM-DD"), "%Y-%m-%d", timeinfo);
      input = buffer;
      break;
    } else if (!valid_date(input) && input != "T") {
      std::cout << "Invalid ICD-10 Code. Please try again." << std::endl;
    }
  } while (!valid_date(input));
  return input;
}

string PDR::set_description() {
  return input_prompt("Description of Injury: ");
}

string PDR::set_lname() {
  string input;
  do {
    input = input_prompt("Last Name: ");
    if (!valid_name(input))
      std::cout << "Invalid last name. Please try again." << std::endl;
  } while (!valid_name(input));
  return input;
}

string PDR::set_ssn() {
  string input;
  do {
    input = input_prompt("Social Security Number: ");
    if (!valid_ssn(input))
      std::cout << "Invalid SSN. Please try again." << std::endl;
  } while (!valid_ssn(input));
  return input;
}

string PDR::set_state() {
  string input;
  do {
    input = input_prompt("State Code (EX: NY): ");
    std::transform(input.begin(), input.end(), input.begin(),
                   ::toupper);     // Convert to upper case
    if (!db.is_valid_state(input)) // Use the db instance for state validation
      std::cout << "Invalid State Code. Please try again." << std::endl;
  } while (!db.is_valid_state(input));
  return input;
}

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
  if (ssn.length() != 9 && ssn.length() != 11)
    return false;

  for (size_t i = 0; i < ssn.length(); ++i) {
    if (i == 3 || i == 6) {
      if (ssn.length() == 11 && ssn[i] != '-')
        return false;
    } else {
      if (!isdigit(ssn[i]))
        return false;
    }
  }
  return true;
}

bool PDR::valid_name(const string &name) {
  if (name.empty())
    return false;

  for (char c : name)
    if (!isalpha(c) && c != '-' && c != '\'')
      return false;
  return true;
}

/**
 * @Depricated, from A2
 */
bool PDR::valid_state(const string &state) { return db.is_valid_state(state); }

optional<PatientRecord> PDR::match_lname(const string &last_name) {
  // very complex way to convert to upper, probs faster on cpp side
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

void PDR::add_injury_record(const string &ssn) {
  clear_console();
  injury_prompt();
  db.view_icd();
  // color code at beginning, very ugly
  std::cout << "\033[0;31m================================================="
            << std::endl;

  Injury curr_injury;
  curr_injury.icd10_code = set_icd_code();
  curr_injury.injury_date = set_injury_date();
  curr_injury.description = set_description();
  curr_injury.patient_ssn = ssn;
  db.injury_controller(curr_injury);
}

/**
 * @info
 * Initiates the process to update or add an injury record for a patient.
 */
void PDR::update_encounter() {
  try {
    std::optional<PatientRecord> curr_patient = lname_ssn();
    if (curr_patient) {
      std::cout << "Patient found: " << std::endl;
      std::cout << "SSN: " << curr_patient->ssn << std::endl;
      std::cout << "Last Name: " << curr_patient->last_name << std::endl;
      std::cout << "Position: " << curr_patient->position << std::endl;
      std::cout << "Last Service Date: " << curr_patient->service_date
                << std::endl;
      std::cout << "State Code: " << curr_patient->state_code << std::endl;

      add_injury_record(curr_patient->ssn);

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
    if (Menu_Callback)
      Menu_Callback();
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    if (Menu_Callback)
      Menu_Callback();
  }
}

void PDR::edit_patients() {
  try {
    std::optional<PatientRecord> curr_patient = lname_ssn();
    if (curr_patient) {
      std::cout << "Patient found: " << std::endl;
      std::cout << "SSN: " << curr_patient->ssn << std::endl;
      std::cout << "Last Name: " << curr_patient->last_name << std::endl;
      std::cout << "Position: " << curr_patient->position << std::endl;
      std::cout << "Last Service Date: " << curr_patient->service_date
                << std::endl;
      std::cout << "State Code: " << curr_patient->state_code << std::endl;

      add_injury_record(curr_patient->ssn);

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
    if (Menu_Callback)
      Menu_Callback();
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    if (Menu_Callback)
      Menu_Callback();
  }
}

void PDR::edit_ICD10S() {
  try {
    std::optional<PatientRecord> curr_patient = lname_ssn();
    if (curr_patient) {
      std::cout << "Patient found: " << std::endl;
      std::cout << "SSN: " << curr_patient->ssn << std::endl;
      std::cout << "Last Name: " << curr_patient->last_name << std::endl;
      std::cout << "Position: " << curr_patient->position << std::endl;
      std::cout << "Last Service Date: " << curr_patient->service_date
                << std::endl;
      std::cout << "State Code: " << curr_patient->state_code << std::endl;

      add_injury_record(curr_patient->ssn);

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
    if (Menu_Callback)
      Menu_Callback();
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    if (Menu_Callback)
      Menu_Callback();
  }
}

PDR::~PDR() {}
