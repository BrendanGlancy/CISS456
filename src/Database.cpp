#include "Database.hpp"

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
  const std::string sql = "CREATE TABLE IF NOT EXISTS vehicle_configuration ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "vehicle_dealer TEXT NOT NULL,"
                          "vehicle_memo TEXT NOT NULL,"
                          "vehicle_color TEXT NOT NULL,"
                          "vehicle_engine TEXT NOT NULL,"
                          "vehicle_cargoOrPassenger TEXT NOT NULL,"
                          "vehicle_cargoRoofline TEXT NOT NULL,"
                          "vehicle_wheelbase TEXT NOT NULL,"
                          "vehicle_make TEXT NOT NULL,"
                          "vehicle_model TEXT NOT NULL,"
                          "vehicle_year INTEGER NOT NULL,"
                          "vehicle_price INTEGER NOT NULL);";

  execute_sql(sql, "Error creating vehicle_configuration table: ");
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
void Database::bind_stmt(sqlite3_stmt *stmt, const Car &data) {
  sqlite3_bind_text(stmt, 1, data.dealer_name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, data.memo.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, data.color.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, data.ev_ic.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, data.cargo_passenger.c_str(), -1,
                    SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 6, data.cargo_roofline.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 7, data.wheelbase.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 8, data.make.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 9, data.model.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 10, data.year);
  sqlite3_bind_int(stmt, 11, data.price);
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
void Database::insert_state(const Car &data) {
  const char *sql = "INSERT INTO states ("
                    "valid_state,"
                    "VALUES (?);";
  sqlite3_stmt *stmt;

  if (prepare_stmt(sql, &stmt)) {
    bind_stmt(stmt, data);
    sql_error(stmt, "Error inserting state codes data: ");
  }
}

// this one is look forward a liitle bit, this is going to all user data
void Database::insert_pdr(const PDR &data) {
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
    std::cout << "ID: " << sqlite3_column_int(stmt, 0) << std::endl;
    std::cout << "Dealer: " << sqlite3_column_text(stmt, 1) << std::endl;
    std::cout << "Memo: " << sqlite3_column_text(stmt, 2) << std::endl;
    std::cout << "Color: " << sqlite3_column_text(stmt, 3) << std::endl;
    std::cout << "Engine: " << sqlite3_column_text(stmt, 4) << std::endl;
    std::cout << "Cargo or Passenger: " << sqlite3_column_text(stmt, 5)
              << std::endl;
    std::cout << "Cargo Roofline: " << sqlite3_column_text(stmt, 6)
              << std::endl;
    std::cout << "Wheelbase: " << sqlite3_column_text(stmt, 7) << std::endl;
    std::cout << "Make: " << sqlite3_column_text(stmt, 8) << std::endl;
    std::cout << "Model: " << sqlite3_column_text(stmt, 9) << std::endl;
    std::cout << "Year: " << sqlite3_column_int(stmt, 10) << std::endl;
    std::cout << "Price: " << sqlite3_column_int(stmt, 11) << std::endl;
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
      {'d', "vehicle_dealer"},
      {'m', "vehicle_memo"},
      {'c', "vehicle_color"},
      {'e', "vehicle_engine"},
      {'c', "vehicle_cargoOrPassenger"},
      {'r', "vehicle_cargoRoofline"},
      {'w', "vehicle_wheelbase"},
      {'!', "vehicle_make"},
      {'#', "vehicle_model"},
      {'y', "vehicle_year"},
      {'p', "vehicle_price"}};

  std::cout
      << "Which column would you like to update? (d) Dealer, (m) Memo, (c) "
         "Color, (e) Engine, (c) Cargo or Passenger, (r) Cargo Roofline, (w) "
         "Wheelbase, (!) Make, (#) Model, (y) Year, (p) Price: ";
  char column;
  std::cin >> column;

  std::string sql = "UPDATE vehicle_configuration SET " + columns[column] +
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
  const char *sql = "DELETE FROM vehicle_configuration WHERE id = ?;";
  sqlite3_stmt *stmt;

  if (prepare_stmt(sql, &stmt)) {
    sqlite3_bind_int(stmt, 1, delete_id);
    sql_error(stmt, "Error inserting vehicle_configuration data: ");
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
