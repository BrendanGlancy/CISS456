#pragma once
#include "../lib/header.h"
#include "VehicleConfiguration.hpp"
#include <filesystem>
#include <sqlite3.h>

/**
 * Class representing a database for managing car data.
 */
class Database {
public:
  /**
   * Constructor for the Database class.
   * Initializes a new instance of the database.
   */
  Database();

  /**
   * Seeds the database with initial data.
   * This method is typically used to populate the database with test data.
   */
  void seed_db();

  /**
   * Inserts a new car record into the database.
   *
   * @param data The car data to be inserted into the database.
   */
  void insert_db(const Car &data);

  /**
   * Queries and displays all car records in the database.
   * This method is used to read and display all existing records.
   */
  void query_all();

  /**
   * Updates existing car records in the database.
   * This method should include logic to modify specific records.
   */
  void update_db();

  /**
   * Deletes car records from the database.
   * This method should include logic to remove specific records.
   */
  void delete_db();

  // Testing methods

  /**
   * Checks if a specific table exists in the database.
   *
   * @param table_name The name of the table to check.
   * @return True if the table exists, false otherwise.
   */
  bool table_exists(const std::string &table_name);

  /**
   * Checks if a specific column exists in a given table in the database.
   *
   * @param table_name The name of the table.
   * @param column_name The name of the column to check.
   * @return True if the column exists in the specified table, false otherwise.
   */
  bool column_exists(const std::string &table_name,
                     const std::string &column_name);

  // destructor
  ~Database();

private:
  sqlite3 *db;
  void connect();
  void display_db(sqlite3_stmt *stmt);
  void sql_error(sqlite3_stmt *stmt, std::string msg);
  void execute_sql(const std::string &sql, const std::string &msg);
  bool prepare_stmt(const char *sql, sqlite3_stmt **stmt);
  std::string col_choice();
  std::string get_update();
  void bind_stmt(sqlite3_stmt *stmt, const Car &data);
  int get_id();
};
