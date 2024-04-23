#include "db_controller.hpp"
#include <cmath>

DB_Manager::DB_Manager() : db(nullptr) {
  int rc = sqlite3_open("./docs/pdr.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    return; // Early return to avoid further operations if open fails
  }
  initialize_database();
}

DB_Manager::~DB_Manager() {
  if (db) {
    sqlite3_close(db);
    db = nullptr; // Good practice to nullify pointer after deletion
  }
}

bool DB_Manager::is_db_initialized() {
  const char *check_query =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='PATIENTS';";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, check_query, -1, &stmt, nullptr) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      sqlite3_finalize(stmt);
      return true;
    }
    sqlite3_finalize(stmt);
  }
  return false;
}

void DB_Manager::initialize_database() {
  if (!is_db_initialized()) {
    execute_sql_file("./schema/schema.sql");
    execute_sql_file("./schema/data.sql");
  }
}

void DB_Manager::execute_sql_file(const string &file_path) {
  // Attempt to open the provided SQL file.
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open SQL file: " << file_path << std::endl;
    return;
  }

  // converts the sql file to a giant string
  std::stringstream buffer;
  buffer << file.rdbuf();
  string sql_commands = buffer.str();

  // throws it in sqlite3_exec
  char *message_error = nullptr;
  if (sqlite3_exec(db, sql_commands.c_str(), nullptr, nullptr,
                   &message_error) != SQLITE_OK) {
    std::cerr << "Error executing SQL file (" << file_path
              << "): " << message_error << std::endl;
    sqlite3_free(message_error); // Free the error message after logging to
  }
}

bool DB_Manager::is_valid_state(const string &state_code) {
  string sql = "SELECT NAME FROM states WHERE code = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false; // Indicates that state validation could not be performed due
  }

  sqlite3_bind_text(stmt, 1, state_code.c_str(), -1, SQLITE_STATIC);

  // True if the state code is found, false otherwise.
  bool isValid = sqlite3_step(stmt) == SQLITE_ROW;

  sqlite3_finalize(stmt);

  return isValid; // Return the validity of the state code based on query
}

bool DB_Manager::is_valid_icd_code(const string &icd_code) {
  string sql =
      "SELECT 1 FROM ICD10S WHERE Code = ? LIMIT 1;"; // More efficient query
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }

  // Using SQLITE_TRANSIENT to ensure the string memory is managed by SQLite
  if (sqlite3_bind_text(stmt, 1, icd_code.c_str(), -1, SQLITE_TRANSIENT) !=
      SQLITE_OK) {
    std::cerr << "Failed to bind ICD code: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt); // Finalize statement to avoid resource leaks
    return false;
  }

  bool isValid = sqlite3_step(stmt) == SQLITE_ROW;

  sqlite3_finalize(stmt);
  return isValid;
}

// helper function not included in header
string get_column_text(sqlite3_stmt *stmt, int column_index) {
  const char *text =
      reinterpret_cast<const char *>(sqlite3_column_text(stmt, column_index));
  return text ? string(text) : "";
}

std::optional<PatientRecord> DB_Manager::match_user(const string &user_info) {
  string sql = "SELECT SSN, LastName, Position, LastServiceDate, "
               "StateCode FROM PATIENTS WHERE LastName = ? OR SSN = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return std::nullopt; // Return an empty optional if the statement
                         // preparation fails
  }

  sqlite3_bind_text(stmt, 1, user_info.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, user_info.c_str(), -1, SQLITE_STATIC);

  PatientRecord record;

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    record.ssn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    record.last_name = get_column_text(stmt, 1);
    record.position = get_column_text(stmt, 2);
    record.service_date = get_column_text(stmt, 3);
    record.state_code = get_column_text(stmt, 4);
    record.valid = true;
  } else {
    record.valid = false; // Set valid to false if no record is found
  }

  sqlite3_finalize(stmt);
  return record.valid ? std::optional<PatientRecord>(record) : std::nullopt;
}

// should be two functions
void DB_Manager::view_tables() {
  const char *sql =
      "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW)
    std::cout << "- " << sqlite3_column_text(stmt, 0) << std::endl;

  sqlite3_finalize(stmt);

  std::cout << "Enter a table name to see its Contents: " << std::endl;
  string table;
  std::getline(std::cin, table); // Read the user input for table name

  // Construct SQL query to view contents of the specified table
  string sql_table = "SELECT * FROM " + table + ";";

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
    input = getchar();
  } while (input != '\n');
}

optional<Injury> DB_Manager::match_description(const string &description) {
  string sql = "SELECT PatientSSN, ICD10Code, InjuryDate, Description FROM "
               "ENCOUNTERS WHERE DESCRIPTION = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return std::nullopt; // Return an empty optional if the statement
                         // preparation fails
  }

  sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC);

  Injury injury;

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    injury.patient_ssn = get_column_text(stmt, 0);
    injury.icd10_code =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    injury.description = get_column_text(stmt, 2);
    injury.valid = true;
  } else {
    injury.valid = false; // Set valid to false if no injury is found
  }

  sqlite3_finalize(stmt);
  return injury.valid ? std::optional<Injury>(injury) : std::nullopt;
}

optional<ICD10S> DB_Manager::match_icd(const string &icd_code) {
  string sql = "SELECT ICD10Code, Description FROM ICD10S WHERE CODE = ?;";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return std::nullopt; // Return an empty optional if the statement
                         // preparation fails
  }

  sqlite3_bind_text(stmt, 1, icd_code.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, icd_code.c_str(), -1, SQLITE_STATIC);

  ICD10S icd;

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    icd.icd10_code =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    icd.description = get_column_text(stmt, 1);
    icd.valid = true;
  } else {
    icd.valid = false; // Set valid to false if no injury is found
  }

  sqlite3_finalize(stmt);
  return icd.valid ? std::optional<ICD10S>(icd) : std::nullopt;
}

void DB_Manager::view_patients() {
  string sql_table = "SELECT * FROM PATIENTS;";
  sqlite3_stmt *stmt;

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
}

void DB_Manager::view_icd() {
  string sql_table = "SELECT * FROM ICD10S;";
  sqlite3_stmt *stmt;

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
      std::cout << "\033[1;37m";
      std::cout << std::left << std::setw(5) << text;
    }
    std::cout << std::endl;
  }

  sqlite3_finalize(stmt);
}

void DB_Manager::view_encounters() {
  string sql_table = "SELECT * FROM ENCOUNTERS;";
  sqlite3_stmt *stmt;

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
      std::cout << "\033[1;37m";
      std::cout << std::left << std::setw(5) << text;
    }
    std::cout << std::endl;
  }

  sqlite3_finalize(stmt);
}

void DB_Manager::injury_controller(Injury new_injury) {
  string sql = "INSERT INTO ENCOUNTERS (PatientSSN, ICD10Code, InjuryDate, "
               "Description) VALUES (?,?,?,?)";
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
  }

  sqlite3_bind_text(stmt, 1, new_injury.patient_ssn.c_str(), -1,
                    SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, new_injury.icd10_code.c_str(), -1,
                    SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, new_injury.injury_date.c_str(), -1,
                    SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, new_injury.description.c_str(), -1,
                    SQLITE_TRANSIENT);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
  } else {
    std::cout << "Injury Recording Success" << std::endl;
  }

  sqlite3_finalize(stmt);
}
