# CISS456

# Patient Demographic Record (PDR) Application

The Patient Demographic Record (PDR) Application is a C++ application designed to manage and record patient demographic information in a healthcare setting. This system is a continuation of the CISS256 project, shifting focus from vehicle configuration to healthcare management.

<p align="center">
  <img src="https://github.com/BrendanGlancy/CISS456/assets/your-image-name.png" alt="image"/>
</p>

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building and Running](#building-and-running)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Features

- **Patient Data Management**: Efficiently record and manage patient demographic information.
- **Data Validation**: Ensure the accuracy and integrity of the data entered by users.
- **File Operations**: Capability to save and retrieve patient data from external files.
- **User-Friendly Interface**: Easy-to-navigate menus and prompts for seamless user interaction.

## Getting Started

### Prerequisites

Ensure you have the following installed:
- A C++ compiler (e.g., GCC, G++)
- Make (optional for easier building)

### Installing Make

- **Windows**: Install "GNU Make for Windows" or use Chocolatey package manager:

  ```powershell
  choco install make
  ```

- **Mac**: Make is included with Xcode Command Line Tools:

  ```bash
  xcode-select --install
  ```

- **Linux**: Install Make using the system's package manager, e.g., for Ubuntu:

  ```bash
  sudo apt-get install build-essential
  ```

### Building and Running

1. Clone the repository:

   ```bash
   git clone https://github.com/BrendanGlancy/CISS456.git
   cd CISS456
   ```

2. Build the application:

   ```bash
   make
   ```

   *If you don't have Make, compile using your C++ compiler, e.g., `g++ -o pdr_app src/*.cpp`

3. Run the application:

   ```bash
   ./pdr_app
   ```

## Project Structure

*not yet determined*

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Official Requirements for the CISS456 Patient Demographic Record (PDR) Application

### Overview

The Patient Demographic Record (PDR) Application is designed as a comprehensive project for the CISS456 course. This application, developed in C++, aims to facilitate the management and recording of patient demographic information within a healthcare setting. Building upon the foundational knowledge acquired in CISS256, this project shifts focus towards healthcare management, integrating aspects of databases, and, time permitting, AI/ML and C# .NET capabilities.

### Requirements

#### Segment 01: Warmup Exercise

- **Objective:** Develop a C++ application capable of entering patient demographic information into a single table (struct), including a user interface (UI) for data entry.
- **Fields:** Include fields for SSN, Name (first, last, middle initial), Address (street, city, state code, ZIP). Implement reasonable validation for each field.
- **Output:** Data entry and ASCII file output in CSV format, with one record per row per patient.

#### Segment 02: Database Integration

- **Objective:** Integrate local database management system (DBMS) capabilities into the PDR Application.
- **DBMS Options:** SQLite or MySQL, with a preference for SQLite due to its simplicity for local applications.
- **State Table Lookup:** Implement a feature for validating state entries against a predefined state table in the database.

#### Segment 03: Advanced Database Operations

- **Objective:** Expand the application to support more complex database operations, focusing on patient encounters.
- **Operations:** Manage key fields, invoke parent and child tables for transactions, and ensure accurate record creation and modification based on patient encounters.
- **SQL Skills:** Use SQL for database interactions, emphasizing the use of index files for efficient querying.

### Development Tools and Environment

- **IDE and Toolchains:** Neutral stance on IDE and toolchains, but support is primarily provided for Visual Studio Code (VSC).
- **Development System:** Students are expected to have access to a development system, whether a personal laptop, library resources, or labs.uakron.edu.
- **Additional Tools:** Engagement with external SQL tutorials (e.g., w3schools.com, sqlitetutorial.net) is encouraged to supplement learning and application development.

### Collaborative Work

- Projects can be undertaken individually or in teams of two, provided both students are from the same course section. Collaboration is encouraged to foster a community of learning and shared expertise.

### Submission Guidelines

- **Warmup Exercise Deadline:** February 1. Early submissions are welcome.
- **Progression:** Subsequent segments (A2, A3) will build upon the capabilities developed in the initial exercise, with specific focus areas detailed in each segment's requirements.

### Additional Notes

- Textbooks for this course are optional and supplemental, with recommended readings from Forouzan for networking and Deitel for comprehensive C++ development.
- Students are encouraged to seek further clarification and guidance as needed, with specific references to Deitel for additional reading material.

### Acknowledgments

- The development and refinement of this project are supported by resources and inspiration from the OpenEMR project and its comprehensive approach to healthcare management software.

<footer>Don't forget to give this repository a star! ⭐️</footer>
