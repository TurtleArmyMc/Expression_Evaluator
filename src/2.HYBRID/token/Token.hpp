#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Token {
  public:
    enum TokenType {
        Expression,
        Constant,
        Pi,
        Add,
        Subtract,
        Multiply,
        Divide,
        Power,
        Sin,
        Cos,
        Tan,
        ASin,
        ACos,
        ATan,
    };
    static const std::unordered_map<TokenType, bool> canBeMultipliedByConstant;

  protected:
    Token();
    TokenType type;

  public:
    Token(TokenType tokenType);
    TokenType getType() const;
    static std::unique_ptr<Token> getToken(const TokenType &);
    static bool isConstant(const std::unique_ptr<Token> &token);
    virtual std::string toString() const;
    virtual std::unique_ptr<Token> clone() const;
};