#include "createchinook.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

ChinookDB::ChinookDB() : db(nullptr) {
  int rc = sqlite3_open("./docs/chinook.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    return;  // Early return to avoid further operations if open fails
  }
  initialize_database();
}

ChinookDB::~ChinookDB() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;  // Good practice to nullify pointer after deletion
  }
}

void ChinookDB::initialize_database() {
  execute_sql_file("./schema/schema.sql");
  execute_sql_file("./schema/data.sql");
}

void ChinookDB::execute_sql_file(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open SQL file: " << file_path << std::endl;
    return;  // Early return if file can't be opened
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string sql_commands = buffer.str();
  char* message_error = nullptr;
  if (sqlite3_exec(db, sql_commands.c_str(), nullptr, nullptr,
                   &message_error) != SQLITE_OK) {
    std::cerr << "Error executing SQL file (" << file_path
              << "): " << message_error << std::endl;
    sqlite3_free(message_error);
  }
}

bool ChinookDB::is_valid_state(const std::string& state_code) {
  std::string sql = "SELECT NAME FROM states WHERE code = ?;";
  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;  // Early return if statement preparation fails
  }
  sqlite3_bind_text(stmt, 1, state_code.c_str(), -1, SQLITE_STATIC);
  bool isValid = sqlite3_step(stmt) == SQLITE_ROW;  // True if a row is returned
  sqlite3_finalize(stmt);
  return isValid;
}
