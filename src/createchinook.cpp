#include <iostream>
#include <sqlite3.h>
#include <string>

// Enum for states
enum States {
  AL,
  AK,
  AZ,
  AR,
  CA,
  CO,
  CT,
  DE,
  FL,
  GA,
  HI,
  ID,
  IL,
  IN,
  IA,
  KS,
  KY,
  LA,
  ME,
  MD,
  MA,
  MI,
  MN,
  MS,
  MO,
  MT,
  NE,
  NV,
  NH,
  NJ,
  NM,
  NY,
  NC,
  ND,
  OH,
  OK,
  OR,
  PA,
  RI,
  SC,
  SD,
  TN,
  TX,
  UT,
  VT,
  VA,
  WA,
  WV,
  WI,
  WY,
  NUM_STATES // To keep track of the number of states
};

// Function to check if a state is valid
bool isValidState(const char *state) {
  for (int i = 0; i < NUM_STATES; ++i) {
    if (state[0] == static_cast<char>('A' + i) &&
        state[1] == static_cast<char>('L' + i)) {
      return true;
    }
  }
  return false;
}

// Function to create SQLite database
int createDatabase(const char *state) {
  sqlite3 *DB;
  char *messageError;

  int exit = sqlite3_open("chinook.db", &DB);

  if (exit) {
    std::cerr << "Error opening SQLite DB: " << sqlite3_errmsg(DB) << std::endl;
    return -1;
  } else {
    std::cout << "Opened Database Successfully!" << std::endl;
  }

  std::string sql = "CREATE TABLE IF NOT EXISTS STATES("
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "STATE           CHAR(2)    NOT NULL);";

  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

  if (exit != SQLITE_OK) {
    std::cerr << "Error Create Table: " << messageError << std::endl;
    sqlite3_free(messageError);
    return -1;
  } else {
    std::cout << "Table created Successfully!" << std::endl;
  }

  // Insert data
  sql = "INSERT INTO STATES (STATE) VALUES ('" + std::string(state) + "');";

  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

  if (exit != SQLITE_OK) {
    std::cerr << "Error Insert: " << messageError << std::endl;
    sqlite3_free(messageError);
    return -1;
  } else {
    std::cout << "Record inserted Successfully!" << std::endl;
  }

  sqlite3_close(DB);

  return 0;
}

// Function to fetch state abbreviation from database
std::string fetchStateFromDatabase() {
  sqlite3 *DB;
  sqlite3_stmt *stmt;
  const char *tail;
  std::string state;

  int exit = sqlite3_open("chinook.db", &DB);

  if (exit) {
    std::cerr << "Error opening SQLite DB: " << sqlite3_errmsg(DB) << std::endl;
    return "";
  }

  std::string sql = "SELECT STATE FROM STATES;";

  exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, &tail);

  if (exit != SQLITE_OK) {
    std::cerr << "Error SELECT: " << sqlite3_errmsg(DB) << std::endl;
    return "";
  }

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    state = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
  }

  sqlite3_finalize(stmt);
  sqlite3_close(DB);

  return state;
}

int main() {
  // Fetch state abbreviation from database
  std::string stateFromDB = fetchStateFromDatabase();

  if (stateFromDB.empty()) {
    std::cerr << "Error fetching state from database!" << std::endl;
    return -1;
  }

  char state[3];
  std::cout << "Enter state abbreviation (2 characters): ";
  std::cin >> state;

  // Check if state is valid
  if (!isValidState(state)) {
    std::cerr << "Invalid state abbreviation!" << std::endl;
    return -1;
  }

  // Create database with state
  if (createDatabase(state) != 0) {
    std::cerr << "Error creating database!" << std::endl;
    return -1;
  }

  return 0;
}
