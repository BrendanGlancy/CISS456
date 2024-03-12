#include <iostream>
#include <string>
#include <sqlite3.h>

// Function to fetch state abbreviation from database
std::string fetchStateFromDatabase() {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char* tail;
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
        state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
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
    if (state[0] != stateFromDB[0] || state[1] != stateFromDB[1]) {
        std::cerr << "Invalid state abbreviation!" << std::endl;
        return -1;
    }

    std::cout << "State abbreviation matches database." << std::endl;
    return 0;
}

