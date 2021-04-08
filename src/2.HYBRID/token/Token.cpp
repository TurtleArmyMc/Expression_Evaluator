#include "Token.hpp"
#include "ConstantToken.hpp"
#include "PiConstantToken.hpp"

const std::unordered_map<Token::TokenType, bool> Token::canBeMultipliedByConstant = {
    {Expression, true}, {Constant, false}, {Pi, true},     {Add, false},  {Subtract, false},
    {Multiply, false},  {Divide, false},   {Power, false}, {Sin, false},  {Cos, false},
    {Tan, false},       {ASin, false},     {ACos, false},  {ATan, false},
};

Token::Token() {}

Token::Token(TokenType tokenType) {
    if (tokenType == Expression || tokenType == Constant || tokenType == Pi) {
        // Should never be reached.
        throw std::invalid_argument("Invalid token type.");
    }
    type = tokenType;
}

std::unique_ptr<Token> Token::getToken(const Token::TokenType &tokenType) {
    if (tokenType == Expression) {
        // Should never be reached.
        throw std::invalid_argument("Invalid token type.");
    } else if (tokenType == Constant) {
        return std::make_unique<ConstantToken>(0);
    } else if (tokenType == Pi) {
        return std::make_unique<PiConstantToken>();
    } else {
        return std::make_unique<Token>(tokenType);
    }
}

Token::TokenType Token::getType() const { return type; }

bool Token::isConstant(const std::unique_ptr<Token> &token) {
    return token->getType() == Constant || token->getType() == Pi;
}

std::string Token::toString() const {
    switch (type) {
    case Add:
        return "+";
    case Subtract:
        return "-";
    case Multiply:
        return "*";
    case Divide:
        return "/";
    case Power:
        return "^";
    case Sin:
        return "SIN";
    case Cos:
        return "COS";
    case Tan:
        return "TAN";
    case ASin:
        return "ASIN";
    case ACos:
        return "ACOS";
    case ATan:
        return "ATAN";
    default:
        throw std::logic_error("Invalid token type");
    }
}

std::unique_ptr<Token> Token::clone() const { return std::make_unique<Token>(*this); };
