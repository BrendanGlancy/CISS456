#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "../lib/Exceptions.hpp"
#include "../lib/header.h"
#include "createchinook.hpp" // Include the path to the ChinookDB header

struct PatientRecord {
  std::string ssn, firstName, middleInitial, lastName, address, city, stateCode,
      zip;
  bool valid;
};

class PDR {
public:
  PDR(ChinookDB &db)
      : db(db) {} // Add a constructor to accept a ChinookDB reference
  using Callback = std::function<void()>;
  void collect_data();
  void save_data();

  /**
   * Sets a callback function for the menu.
   *
   * @param callback The function to be called for the menu.
   */
  void set_menu_callback(Callback callback);

private:
  PatientRecord patient;               // Holds the current patient's data
  std::function<void()> Menu_Callback; // Callback for returning to the menu
  ChinookDB &db; // Reference to the database for state validation

  // Input and validation methods
  bool valid_ssn(const std::string &ssn);
  bool valid_name(const std::string &name);
  bool valid_initial(const std::string &name);
  bool valid_state(
      const std::string &state); // This will now interact with the database
  bool valid_zip(const std::string &zip);

  // Utility methods for getting and validating user input
  std::string
  get_valid_input(const std::string &prompt,
                  const std::function<bool(const std::string &)> &validator);
  std::string input_prompt(const std::string &prompt);

  std::string set_fname();
  std::string set_minitial();
  std::string set_lname();
  std::string set_address();
  std::string set_city();
  std::string set_state(); // State input that will validate using the database
  std::string set_zip();
  std::string set_ssn();

  // Helper method to process user input
  std::string get_input(const std::string &input);
};
