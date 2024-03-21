#include "createchinook.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

ChinookDB::ChinookDB() : db(nullptr) {
  int rc = sqlite3_open("./docs/chinook.db", &db);
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

void ChinookDB::initialize_database() {
  execute_sql_file("./schema/schema.sql");
  execute_sql_file("./schema/data.sql");
}

void ChinookDB::execute_sql_file(const std::string &file_path) {
  // Attempt to open the provided SQL file.
  std::ifstream file(file_path);
  // Check if the file was successfully opened. If not, log an error and exit
  // the function.
  if (!file.is_open()) {
    std::cerr << "Failed to open SQL file: " << file_path << std::endl;
    return; // Early return to avoid further processing if the file cannot be
            // opened.
  }

  // Read the contents of the file into a string buffer.
  std::stringstream buffer;
  buffer << file.rdbuf(); // Stream the file's content into the buffer.
  std::string sql_commands = buffer.str(); // Convert the buffered stream to a
                                           // string containing SQL commands.

  char *message_error = nullptr;
  // Execute the SQL commands from the file using sqlite3_exec.
  // sqlite3_exec will run all SQL commands as a batch operation on the database
  // pointed to by db.
  if (sqlite3_exec(db, sql_commands.c_str(), nullptr, nullptr,
                   &message_error) != SQLITE_OK) {
    // If execution fails, log the error message provided by SQLite.
    std::cerr << "Error executing SQL file (" << file_path
              << "): " << message_error << std::endl;
    sqlite3_free(message_error); // Free the error message after logging to
                                 // prevent memory leaks.
  }
}

bool ChinookDB::is_valid_state(const std::string &state_code) {
  // Prepare the SQL query to check if a given state code exists in the states
  // table.
  std::string sql = "SELECT NAME FROM states WHERE code = ?;";
  sqlite3_stmt *stmt = nullptr;

  // Prepare the SQL statement for execution.
  // sqlite3_prepare_v2 compiles the SQL query into bytecode for faster
  // execution.
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    // If the statement preparation fails, log the error using sqlite3_errmsg
    // and return false.
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false; // Indicates that state validation could not be performed due
                  // to an error.
  }

  // Bind the provided state_code to the SQL query parameter (? placeholder).
  sqlite3_bind_text(stmt, 1, state_code.c_str(), -1, SQLITE_STATIC);

  // Execute the prepared statement and check if it returns a row.
  // Returning a row indicates that the state code exists in the database.
  bool isValid =
      sqlite3_step(stmt) ==
      SQLITE_ROW; // True if the state code is found, false otherwise.

  // Finalize the statement to free resources. This should be done regardless of
  // the query result.
  sqlite3_finalize(stmt);

  return isValid; // Return the validity of the state code based on query
                  // result.
}
