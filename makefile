# Detect the operating system
UNAME_S := $(shell uname -s)

# Check if clang is installed
CLANG_AVAILABLE := $(shell command -v clang++ >/dev/null 2>&1; echo $$?)

# Specify the default compiler based on the operating system and availability of clang
ifeq ($(UNAME_S),Linux)
  ifeq ($(CLANG_AVAILABLE),0)
    COMPILER ?= CLANG
  else
    COMPILER ?= GCC
  endif
else ifeq ($(UNAME_S),Darwin)
  COMPILER ?= CLANG
else ifeq ($(OS),Windows_NT)
  COMPILER ?= MSVC
else
  COMPILER ?= GCC
endif

# Specify the compilers
ifeq ($(COMPILER),CLANG)
  CXX = clang++
  CC = clang
else ifeq ($(COMPILER),GCC)
  CXX = g++
  CC = gcc
else ifeq ($(COMPILER),MSVC)
  CXX = cl
  CC = cl
endif

# Specify compiler flags
CXXFLAGS = -Wall -std=c++17
CFLAGS = -Wall

# Specify the linker flags
# Add `-lsqlite3` to link the SQLite3 library
LDFLAGS = -lsqlite3 

# Specify the target file
TARGET = pdr_app

# Separate C and C++ source files
CPP_SOURCES = src/pdr.cpp src/db_controller.cpp main.cpp lib/Exceptions.cpp 
C_SOURCES = lib/common.c src/menu.c

# Generate object files from sources
OBJECTS = $(CPP_SOURCES:.cpp=.o) $(C_SOURCES:.c=.o)

all: $(TARGET)

# Test files
TEST_SOURCES = tests/run-test.cpp src/Database.cpp
TEST_C_SOURCES = lib/common.c  
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o) $(TEST_C_SOURCES:.c=.o)
TEST_TARGET = run_tests

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# A pattern rule to create ".o" object files from ".cpp" source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# A pattern rule to create ".o" object files from ".c" source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS) $(LDFLAGS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Add a clean target
clean:
	rm -f $(TARGET) $(OBJECTS) $(TEST_TARGET) $(TEST_OBJECTS)
