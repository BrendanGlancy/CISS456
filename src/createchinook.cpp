#include "createchinook.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

ChinookDB::ChinookDB() : db(nullptr) {
  int rc = sqlite3_open("./docs/pdr.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    return; // Early return to avoid further operations if open fails
  }
  initialize_database();
}

ChinookDB::~ChinookDB() {
  if (db) {
    sqlite3_close(db);
    db = nullptr; // Good practice to nullify pointer after deletion
  }
}

bool ChinookDB::is_db_initialized() {
  const char *check_query =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='PATIENTS';";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, check_query, -1, &stmt, nullptr) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      sqlite3_finalize(stmt);
      return true; // The STATES table exists, indicating initialization has
    }
    sqlite3_finalize(stmt);
  }
  return false; // Initialization needed
}

void ChinookDB::initialize_database() {
  if (!is_db_initialized()) {
    execute_sql_file("./schema/schema.sql");
    execute_sql_file("./schema/data.sql");
    execute_sql_file("./schema/A3_data.sql");
  }
}

void ChinookDB::execute_sql_file(const std::string &file_path) {
  // Attempt to open the provided SQL file.
  std::ifstream file(file_path);
  // Check if the file was successfully opened. If not, log an error and exit
  if (!file.is_open()) {
    std::cerr << "Failed to open SQL file: " << file_path << std::endl;
    return; // Early return to avoid further processing if the file cannot be
  }

  // Read the contents of the file into a string buffer.
  std::stringstream buffer;
  buffer << file.rdbuf(); // Stream the file's content into the buffer.
  std::string sql_commands = buffer.str(); // Convert the buffered stream to a

  char *message_error = nullptr;
  if (sqlite3_exec(db, sql_commands.c_str(), nullptr, nullptr,
                   &message_error) != SQLITE_OK) {
    std::cerr << "Error executing SQL file (" << file_path
              << "): " << message_error << std::endl;
    sqlite3_free(message_error); // Free the error message after logging to
                                 // prevent memory leaks.
  }
}

bool ChinookDB::is_valid_state(const std::string &state_code) {
  std::string sql = "SELECT NAME FROM states WHERE code = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false; // Indicates that state validation could not be performed due
                  // to an error.
  }

  sqlite3_bind_text(stmt, 1, state_code.c_str(), -1, SQLITE_STATIC);

  bool isValid =
      sqlite3_step(stmt) ==
      SQLITE_ROW; // True if the state code is found, false otherwise.

  sqlite3_finalize(stmt);

  return isValid; // Return the validity of the state code based on query result
}

bool ChinookDB::is_user_admin(const std::string &user_id) {
  std::string sql = "SELECT is_admin FROM user_permissions WHERE user_id = ?;";
  sqlite3_stmt *stmt = nullptr;
  bool is_admin = false;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false; // Statement preparation failed
  }

  sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    is_admin = sqlite3_column_int(stmt, 0) !=
               0; // Assuming is_admin is stored as an integer (0 or 1)
  }

  sqlite3_finalize(stmt);
  return is_admin;
}

void ChinookDB::view_tables() {
  const char *sql =
      "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::cout << "- " << sqlite3_column_text(stmt, 0) << std::endl;
  }

  sqlite3_finalize(stmt);

  std::cout << "Enter a table name to see its Contents: " << std::endl;
  std::string table;
  std::getline(std::cin, table); // Read the user input for table name

  // Construct SQL query to view contents of the specified table
  std::string sql_table = "SELECT * FROM " + table + ";";

  if (sqlite3_prepare_v2(db, sql_table.c_str(), -1, &stmt, nullptr) !=
      SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return;
  }

  // Execute the query and print the results
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    // Assuming the table has multiple columns, print each column's value
    for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
      std::cout << sqlite3_column_text(stmt, i) << "\t";
    }
    std::cout << std::endl;
  }

  sqlite3_finalize(stmt);

  std::cout << "Press <ENTER> To return to Main Menu" << std::endl;
  char input;

  do {
    input = getchar();     // Read the user input
  } while (input != '\n'); // Wait until 'Enter' is pressed
}
