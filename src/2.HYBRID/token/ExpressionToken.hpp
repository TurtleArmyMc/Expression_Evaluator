#pragma once

#include "ConstantToken.hpp"
#include "Token.hpp"
#include <vector>

class ExpressionToken : public Token {
  protected:
    ExpressionToken();
    std::vector<std::unique_ptr<Token>> tokens;
    static std::unique_ptr<ExpressionToken> parse(const std::string &s, bool inParenthesis, int &i);

  public:
    static void evaluateOperations(const std::vector<Token::TokenType> operations,
                                                    std::vector<std::unique_ptr<Token>> &evaluateTokens);
    static ConstantToken *getNextConstant(std::vector<std::unique_ptr<Token>>::iterator,
                                          std::vector<std::unique_ptr<Token>> &);
    static std::unique_ptr<ExpressionToken> parse(const std::string &s);
    static std::unique_ptr<Token> parseSpecialToken(const std::string &s, int &i);
    virtual std::unique_ptr<Token> clone() const override;

    std::unique_ptr<ConstantToken> evaluate() const;
    bool canBeMultipliedByConstant() const;
    std::string toString() const;
};