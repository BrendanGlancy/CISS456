#pragma once

#include <sqlite3.h>

#include <string>

class ChinookDB {
 public:
  ChinookDB();
  ~ChinookDB();

  /**
   * Checks to see if a state code is valid
   * @param state_code
   * @returns bool
   */
  bool is_valid_state(const std::string& state_code);

  /**
   * Checks to see if the user is an admin, accorrding to the admin table
   * @param user_id
   * @returns bool
   */
  bool is_user_admin(const std::string& user_id);

  /**
   *
   */
  void view_tables();

 private:
  sqlite3* db;
  void execute_sql_file(const std::string& file_path);
  void initialize_database();
  bool is_db_initialized();
};
