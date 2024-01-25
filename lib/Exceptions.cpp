#include "Exceptions.hpp"

UserQuitException::UserQuitException()
    : std::runtime_error("User requested quit") {}
