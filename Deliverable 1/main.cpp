#include <iostream>
#include <string>
#include <cctype>


struct PatientRecord {
    std::string ssn;
    std::string firstName;
    char middleInitial;
    std::string lastName;
    std::string address;
    std::string city;
    std::string stateCode;
    std::string zip;
};


bool isValidSSN(const std::string& ssn) {
    
    return (ssn.length() == 9 && std::all_of(ssn.begin(), ssn.end(), isdigit));
}


bool isValidName(const std::string& name) {
    
    
    return !name.empty();
}

bool isValidStateCode(const std::string& stateCode) {
    
    return !stateCode.empty();
}


bool isValidZIP(const std::string& zip) {
   
    return std::all_of(zip.begin(), zip.end(), isdigit);
}


PatientRecord enterPatientData() {
    PatientRecord record;

 
    do {
        std::cout << "Enter SSN (9 digits): ";
        std::cin >> record.ssn;
    } while (!isValidSSN(record.ssn));


    std::cout << "Enter First Name: ";
    std::cin >> record.firstName;

    std::cout << "Enter Middle Initial: ";
    std::cin >> record.middleInitial;

    std::cout << "Enter Last Name: ";
    std::cin >> record.lastName;


    std::cout << "Enter Address: ";
    std::cin >> record.address;

   
    std::cout << "Enter City: ";
    std::cin >> record.city;

  
    do {
        std::cout << "Enter State Code: ";
        std::cin >> record.stateCode;
    } while (!isValidStateCode(record.stateCode));

   
    do {
        std::cout << "Enter ZIP Code: ";
        std::cin >> record.zip;
    } while (!isValidZIP(record.zip));

    return record;
}

int main() {

    PatientRecord patient = enterPatientData();


    std::cout << "\nPatient Demographic Record:\n";
    std::cout << "SSN: " << patient.ssn << "\n";
    std::cout << "Name: " << patient.firstName << " " << patient.middleInitial << ". " << patient.lastName << "\n";
    std::cout << "Address: " << patient.address << ", " << patient.city << ", " << patient.stateCode << " " << patient.zip << "\n";

    return 0;
}
