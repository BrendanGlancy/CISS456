#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../lib/Exceptions.hpp"
#include "../lib/header.h"

struct PatientRecord {
  std::string ssn, firstName, middleInitial, lastName, address, city, stateCode,
      zip;
  bool valid;
};

class PDR {
public:
  using Callback = std::function<void()>;
  void collect_data();
  void save_data();

  /**
   * Sets a callback function for the menu.
   *
   * @param callback The function to be called for the menu.
   */
  void set_menu_callback(Callback callback) {
    Menu_Callback = std::move(callback);
  }

private:
  PatientRecord patient;
  Callback Menu_Callback;

  // sanitization
  bool valid_ssn(const std::string &ssn);
  bool valid_name(const std::string &name);
  bool valid_state(const std::string &state);
  bool valid_zip(const std::string &zip);
  std::string
  get_valid_input(const std::string &prompt,
                  const std::function<bool(const std::string &)> &validator);

  std::string input_prompt(const std::string &prompt, bool clear);
  std::string set_fname();
  std::string set_minitial();
  std::string set_lname();
  std::string set_address();
  std::string set_city();
  std::string set_state();
  std::string set_zip();
  std::string set_ssn();

  std::string get_input(const std::string &input);
};
