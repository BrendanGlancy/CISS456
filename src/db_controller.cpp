#include "db_controller.hpp"

DB_Manager::DB_Manager() : db(nullptr) {
  int rc = sqlite3_open("./docs/pdr.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    return;  // Early return to avoid further operations if open fails
  }
  initialize_database();
}

DB_Manager::~DB_Manager() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;  // Good practice to nullify pointer after deletion
  }
}

bool DB_Manager::is_db_initialized() {
  const char *check_query =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='PATIENTS';";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, check_query, -1, &stmt, nullptr) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      sqlite3_finalize(stmt);
      return true;  // The STATES table exists, indicating initialization has
    }
    sqlite3_finalize(stmt);
  }
  return false;  // Initialization needed
}

void DB_Manager::initialize_database() {
  if (!is_db_initialized()) {
    execute_sql_file("./schema/schema.sql");
    execute_sql_file("./schema/data.sql");
  }
}

void DB_Manager::execute_sql_file(const std::string &file_path) {
  // Attempt to open the provided SQL file.
  std::ifstream file(file_path);
  // Check if the file was successfully opened. If not, log an error and exit
  if (!file.is_open()) {
    std::cerr << "Failed to open SQL file: " << file_path << std::endl;
    return;  // Early return to avoid further processing if the file cannot be
  }

  // Read the contents of the file into a string buffer.
  std::stringstream buffer;
  buffer << file.rdbuf();  // Stream the file's content into the buffer.
  std::string sql_commands = buffer.str();  // Convert the buffered stream to a

  char *message_error = nullptr;
  if (sqlite3_exec(db, sql_commands.c_str(), nullptr, nullptr,
                   &message_error) != SQLITE_OK) {
    std::cerr << "Error executing SQL file (" << file_path
              << "): " << message_error << std::endl;
    sqlite3_free(message_error);  // Free the error message after logging to
                                  // prevent memory leaks.
  }
}

bool DB_Manager::is_valid_state(const std::string &state_code) {
  std::string sql = "SELECT NAME FROM states WHERE code = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;  // Indicates that state validation could not be performed due
                   // to an error.
  }

  sqlite3_bind_text(stmt, 1, state_code.c_str(), -1, SQLITE_STATIC);

  bool isValid =
      sqlite3_step(stmt) ==
      SQLITE_ROW;  // True if the state code is found, false otherwise.

  sqlite3_finalize(stmt);

  return isValid;  // Return the validity of the state code based on query
                   // result
}

bool DB_Manager::is_valid_icd_code(const std::string &icd_code) {
  std::string sql = "SELECT * FROM ICD10S WHERE Code = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;  // Indicates that state validation could not be performed due
                   // to an error.
  }

  sqlite3_bind_text(stmt, 1, icd_code.c_str(), -1, SQLITE_STATIC);

  bool isValid =
      sqlite3_step(stmt) ==
      SQLITE_ROW;  // True if the state code is found, false otherwise.

  sqlite3_finalize(stmt);

  return isValid;  // Return the validity of the state code based on query
                   // result
}

bool DB_Manager::match_user(const std::string &user_info) {
  std::string sql = "SELECT * FROM states WHERE lname = ? or ssn = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;  // Indicates that state validation could not be performed due
                   // to an error.
  }

  sqlite3_bind_text(stmt, 1, user_info.c_str(), -1, SQLITE_STATIC);

  bool isValid =
      sqlite3_step(stmt) ==
      SQLITE_ROW;  // True if the state code is found, false otherwise.

  sqlite3_finalize(stmt);

  return isValid;  // Return the validity of the state code based on query
                   // result
}

void DB_Manager::view_tables() {
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
  std::getline(std::cin, table);  // Read the user input for table name

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
      const char *text =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
      std::cout << std::left << std::setw(11) << text;
    }
    std::cout << std::endl;
  }

  sqlite3_finalize(stmt);

  std::cout << "Press <ENTER> To return to Main Menu" << std::endl;
  char input;

  do {
    input = getchar();  // Read the user input
  } while (input != '\n');  // Wait until 'Enter' is pressed
}
