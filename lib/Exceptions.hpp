#pragma once

#include <stdexcept>

class UserQuitException : public std::runtime_error {
 public:
  UserQuitException();
};

class InvalidChoiceException : public std::runtime_error {
 public:
  InvalidChoiceException();
};
