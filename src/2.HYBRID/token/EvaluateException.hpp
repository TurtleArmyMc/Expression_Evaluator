#pragma once

#include <exception>
#include <string>

class EvaluateException : public std::exception {
  protected:
    int tokenNumber;
    std::string message;

  public:
    EvaluateException(const std::string &m) {
        message = std::string(m);
        tokenNumber = 0;
    }

    EvaluateException(const std::string &m, int n) {
        message = std::string(m);
        tokenNumber = n;
    }

    const char *what() const noexcept { return message.c_str(); }

    int getTokenNumber() const { return tokenNumber; }
};