#include "Database.hpp"
#include "pdr.hpp"

#include <functional>
#include <iostream>

/**
 * For this we need to:
 *   1. Remove everything have to do with the car struct
 *   2. We are going to start with a valid state table
 *   3. Then we are going to have a PDR table
 */

Database::Database() {
  int rc = sqlite3_open("./docs/database.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
  }
}

// this is where we need to create the tables
void Database::seed_db() {
  const std::string sql = "CREATE TABLE IF NOT EXISTS Patient_Data_Record ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "pdr_firstname TEXT NOT NULL,"
                          "pdr_middleinitial TEXT NOT NULL,"
                          "pdr_lastname TEXT NOT NULL,"
                          "pdr_ssn TEXT NOT NULL,"
                          "pdr_address TEXT NOT NULL,"
                          "pdr_city TEXT NOT NULL,"
                          "pdr_state TEXT NOT NULL,"
                          "pdr_zipcode TEXT NOT NULL";

  execute_sql(sql, "Error creating Patient_Data_Record table: ");
}

bool Database::prepare_stmt(const char *sql, sqlite3_stmt **stmt) {
  if (sqlite3_prepare_v2(db, sql, -1, stmt, NULL) != SQLITE_OK) {
    std::cerr << "Error preparing statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }
  return true;
}

// Notice a pattern, we just take everything in the car struct and bind it, so
// to update this for this class we just need to bind everything in the new pdr
// struct
void Database::bind_stmt(sqlite3_stmt *stmt, const PatientRecord &data) {
  sqlite3_bind_text(stmt, 1, data.firstName.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, data.middleInitial.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, data.lastName.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, data.ssn.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, data.address.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 6, data.city.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 7, data.stateCode.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 8, data.zip.c_str(), -1, SQLITE_TRANSIENT);
}

void Database::execute_sql(const std::string &sql, const std::string &msg) {
  char *errMsg = nullptr;
  int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);

  if (rc != SQLITE_OK) {
    std::cerr << msg << errMsg << std::endl;
    sqlite3_free(errMsg);
  } else {
    std::cout << "Operation success" << std::endl;
  }
}

// this one is specifically for deliverable 2
void Database::insert_state(const PatientRecord &data) {
  const char *sql = "INSERT INTO states ("
                    "valid_state,"
                    "VALUES (?);";
  sqlite3_stmt *stmt;

  if (prepare_stmt(sql, &stmt)) {
    bind_stmt(stmt, data);
    sql_error(stmt, "Error inserting state codes data: ");
  }
}

// this one is look forward a little bit, this is going to all user data
void Database::insert_pdr(const PatientRecord &data) {
  const char *sql = "INSERT INTO states ("
                    "valid_state,"
                    "VALUES (?);";
  sqlite3_stmt *stmt;

  if (prepare_stmt(sql, &stmt)) {
    bind_stmt(stmt, data);
    sql_error(stmt, "Error inserting state codes data: ");
  }
}

void Database::query_all() {
  const char *sql = "SELECT * FROM states;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    display_db(stmt);
    sqlite3_finalize(stmt);

    clear_input_buffer();

    std::cout << "Press [enter] to continue...";
    getchar();
  } else
    std::cerr << "Error preparing statement: " << sqlite3_errmsg(db)
              << std::endl;
}

// update this to reflect the healthcare version of this project
void Database::display_db(sqlite3_stmt *stmt) {
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    printf("============================================\n");
    std::cout << "id: " << sqlite3_column_int(stmt, 0) << std::endl;
    std::cout << "First Name: " << sqlite3_column_int(stmt, 1) << std::endl;
    std::cout << "Middle Initial: " << sqlite3_column_text(stmt, 2)
              << std::endl;
    std::cout << "Last Name: " << sqlite3_column_text(stmt, 3) << std::endl;
    std::cout << "Social Security Number: " << sqlite3_column_text(stmt, 4)
              << std::endl;
    std::cout << "Address: " << sqlite3_column_text(stmt, 5) << std::endl;
    std::cout << "City: " << sqlite3_column_text(stmt, 6) << std::endl;
    std::cout << "State: " << sqlite3_column_text(stmt, 7) << std::endl;
    std::cout << "Zip: " << sqlite3_column_text(stmt, 8) << std::endl;
    std::cout << std::endl;
    printf("============================================\n");
  }
}

// the below can stay
void Database::sql_error(sqlite3_stmt *stmt, std::string msg) {
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << msg << sqlite3_errmsg(db) << std::endl;
  }
}

int Database::get_id() {
  int id;
  std::cout << "Enter ID of the table you want to Modify: ";
  std::cin >> id;
  return id;
}

void Database::update_db() {
  int update_id = get_id();

  std::string sql = col_choice();
  clear_input_buffer();
  std::string update = get_update();
  sqlite3_stmt *stmt;

  // sql.c_str() converts the string to a const char *
  if (prepare_stmt(sql.c_str(), &stmt)) {
    sqlite3_bind_text(stmt, 1, update.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, update_id);
    sql_error(stmt, "Error inserting vehicle_configuration data: ");
  }
}

// this just need to be modified
std::string Database::col_choice() {
  // TODO: improve the keys names
  std::unordered_map<char, std::string> columns = {
      {'i', "id"},
      {'f', "patient_FirstName"},
      {'m', "patient_MiddleInitial"},
      {'l', "patient_LastName"},
      {'s', "patient_SSN"},
      {'a', "patient_Address"},
      {'c', "patient_City"},
      {'s', "patient_State"},
      {'z', "patient_ZipCode"},
  };

  std::cout
      << "Which column would you like to update? (i) ID, (f) First Name, (m) "
         "Middle Initial, (l) "
         "Last Name, (s) Social Security Number, (a) Address, (c) City, (s) "
         "State, (z) Zip Code";
  char column;
  std::cin >> column;

  std::string sql = "UPDATE Patient_Data_Record SET " + columns[column] +
                    " = ? WHERE id = ?;";
  return sql;
}

std::string Database::get_update() {
  std::string update;
  std::cout << "What would you like to update it to? ";
  std::getline(std::cin, update);
  return update;
}

void Database::delete_db() {
  int delete_id = get_id();
  const char *sql = "DELETE FROM Patient_Data_Record WHERE id = ?;";
  sqlite3_stmt *stmt;

  if (prepare_stmt(sql, &stmt)) {
    sqlite3_bind_int(stmt, 1, delete_id);
    sql_error(stmt, "Error inserting Patient_Data_Record data: ");
  }
}

// testing
bool Database::table_exists(const std::string &table_name) {
  const std::string sql =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='" +
      table_name + "';";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool Database::column_exists(const std::string &table_name,
                             const std::string &column_name) {
  const std::string sql = "SELECT " + column_name + " FROM " + table_name + ";";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

// destructor
Database::~Database() { sqlite3_close(db); }
