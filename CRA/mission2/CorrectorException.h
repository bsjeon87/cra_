#pragma once

#include <stdexcept>
#include <string>

using std::exception;
using std::string;

class CorrectorException : public std::exception {
public:
    explicit CorrectorException(const std::string& msg);

private:
    string message;
};