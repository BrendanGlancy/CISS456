#pragma once

#include <sqlite3.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

class DB_Manager {
 public:
  DB_Manager();
  ~DB_Manager();

  /**
   * Checks to see if a state code is valid
   * @param state_code
   * @returns bool
   */
  bool is_valid_state(const std::string& state_code);

  /**
   * Checks to see if a state code is valid
   * @param state_code
   * @returns bool
   */
  bool is_valid_icd_code(const std::string& icd_code);

  /**
   * Checks to see if a user exsists
   * @param user_info
   * @returns bool
   */
  bool match_user(const std::string& user_info);

  /**
   * Prints all tables to the console
   */
  void view_tables();

  /**
   * Allows for the user to add an injury to the injury table
   */
  void injury_controller();

 private:
  sqlite3* db;
  void execute_sql_file(const std::string& file_path);
  void initialize_database();
  bool is_db_initialized();
};
