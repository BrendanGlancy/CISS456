#pragma once

#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>  // Include for regex functionality
#include <sstream>
#include <string>

#include "../lib/Exceptions.hpp"
#include "../lib/header.h"
#include "db_controller.hpp"  // Path to the database manager class

using std::function;
using std::string;

struct PatientRecord {
  string ssn, first_name, middle_initial, last_name, address, city, state_code,
      zip;
  bool valid;
};

class PDR {
 public:
  explicit PDR(DB_Manager& db)
      : db(db) {}  // Constructor with a DB_Manager reference

  using Callback = function<void()>;

  /**
   * Collects data for updating or adding a patient's injury record.
   */
  void collect_data();

  /**
   * Sets a callback function to return to the main menu.
   *
   * @param callback The function to be called for returning to the menu.
   */
  void set_menu_callback(Callback callback);

  /**
   * Initiates the process to update or add an injury record for a patient.
   */
  void update_or_add_injury();

 private:
  PatientRecord patient;   // Holds the current patient's data
  Callback Menu_Callback;  // Callback for returning to the menu
  DB_Manager& db;          // Reference to the database manager

  // Input and validation methods
  bool valid_ssn(const string& ssn);
  bool valid_state(const string& state);  // Validates state using the database
  bool match_lname(const string& last_name);
  bool match_ssn(const string& ssn);
  bool lname_ssn();
  bool valid_date(const string& date);  // Validates the format of a date

  // Methods for updating and adding injury records
  void update_injury_record();
  void add_injury_record(const string& identifier);
  string set_icd_code();
  string set_injury_date();
  string set_description();

  // Utility methods for getting and validating user input
  string get_valid_input(const string& prompt,
                         const function<bool(const string&)>& validator);
  string input_prompt(const string& prompt);

  // Methods to set individual attributes of a patient
  string set_lname();
  string set_ssn();

  // Helper method to process user input
  string get_input(const string& input);
};
