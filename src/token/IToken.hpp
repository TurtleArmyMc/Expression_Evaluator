#pragma once

#include <memory>
#include <string>

class IToken {
  public:
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<IToken> clone() const = 0;
};