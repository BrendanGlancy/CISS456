#pragma once

#include <sqlite3.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

using std::optional;
using std::string;

/**
 * SSN INTEGER UNIQUE,
 * LastName VARCHAR(255),
 * Position VARCHAR(5),
 * LastServiceDate DATE DEFAULT CURRENT_DATE,
 * StateCode CHAR(2) NOT NULL
 */
struct PatientRecord {
  string ssn, last_name, position, service_date, state_code;
  bool valid;

  PatientRecord()
      : ssn(""), last_name(""), position(""), service_date(""), state_code(""),
        valid(false) {}
};

/**
 *  InjuryID INTEGER PRIMARY KEY AUTOINCREMENT,
 *  PatientSSN INTEGER NOT NULL,
 *  ICD10Code VARCHAR(7) NOT NULL,
 *  InjuryDate DATE NOT NULL,
 *  Description TEXT,
 *  FOREIGN KEY (PatientSSN) REFERENCES PATIENTS(SSN),
 *  FOREIGN KEY (ICD10Code) REFERENCES ICD10S(Code)
 */
struct Injury {
  int injury_id;
  string patient_ssn;
  string icd10_code;
  string injury_date;
  string description;
  bool valid;

  Injury() : injury_id(0), valid(false) {} // Default constructor
};

class DB_Manager {
public:
  DB_Manager();
  ~DB_Manager();

  /**
   * Checks to see if a state code is valid
   * @param state_code
   * @returns bool
   */
  bool is_valid_state(const string &state_code);

  /**
   * Checks to see if a state code is valid
   * @param state_code
   * @returns bool
   */
  bool is_valid_icd_code(const string &icd_code);

  /**
   * Checks to see if a user exsists
   * @param user_info
   * @returns bool
   */
  optional<PatientRecord> match_user(const string &user_info);

  /**
   * Prints all tables to the console
   */
  void view_tables();

  /**
   * Prints patients from the db
   */
  void view_patients();

  /**
   * Prints patients from the db
   */
  void view_icd();

  /**
   * Allows for the user to add an injury to the injury table
   */
  void injury_controller(Injury injury);

private:
  sqlite3 *db;
  void execute_sql_file(const string &file_path);
  void initialize_database();
  bool is_db_initialized();
};
