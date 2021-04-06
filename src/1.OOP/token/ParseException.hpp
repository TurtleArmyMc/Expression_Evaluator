#pragma once

#include <exception>
#include <string>

class ParseException : public std::exception {
  protected:
    int charNumber;
    std::string message;

  public:
    ParseException(const char &c, int n) {
        charNumber = n;
        message = std::string("Unexpected character '");
        message += c;
        message += "' at position " + std::to_string(n + 1) + " when parsing expression.";
    }

    ParseException(const std::string &m, int n) {
        charNumber = n;
        message = std::string(m);
    }

    const char *what() const noexcept { return message.c_str(); };

    int getCharNumber() const { return charNumber; };
};