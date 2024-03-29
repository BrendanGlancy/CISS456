#pragma once

#include <sqlite3.h>

#include <string>

class ChinookDB {
 public:
  ChinookDB();
  ~ChinookDB();

  bool is_valid_state(const std::string& state_code);
  bool is_user_admin(const std::string& user_id);
  void view_tables();

 private:
  sqlite3* db;
  void execute_sql_file(const std::string& file_path);
  void initialize_database();
  bool is_db_initialized();
};
