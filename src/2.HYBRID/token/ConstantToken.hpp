#pragma once

#include "Token.hpp"
#include <memory>

class ConstantToken : public Token {
  protected:
    long double value;
    static bool validChar(int c);
    ConstantToken();

  public:
    ConstantToken(long double d);
    static std::unique_ptr<ConstantToken> parse(const std::string &, int &);
    virtual long double getValue() const;
    virtual std::unique_ptr<Token> clone() const override;
    virtual std::string toString() const override;
};