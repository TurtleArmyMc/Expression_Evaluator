#include "IOperatorToken.hpp"
#include "../ParseException.hpp"
#include "AddOperatorToken.hpp"
#include "DivideOperatorToken.hpp"
#include "MultiplyOperatorToken.hpp"
#include "PowerOperatorToken.hpp"
#include "SubtractOperatorToken.hpp"

std::unique_ptr<IOperatorToken> IOperatorToken::parse(char c, int &i) {
    switch (c) {
    case '+':
        ++i;
        return std::make_unique<AddOperatorToken>();
    case '-':
        ++i;
        return std::make_unique<SubtractOperatorToken>();
    case '*':
        ++i;
        return std::make_unique<MultiplyOperatorToken>();
    case '/':
        ++i;
        return std::make_unique<DivideOperatorToken>();
    case '^':
        ++i;
        return std::make_unique<PowerOperatorToken>();
    default:
        std::string errorMessage = std::string("Failed to parse character '");
        errorMessage += c;
        errorMessage += "' at position " + std::to_string(i + 1) + " as operator.";
        throw ParseException(errorMessage, i);
    }
}