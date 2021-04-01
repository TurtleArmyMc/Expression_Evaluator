#pragma once

#include "../ConstantToken.hpp"
#include "../IToken.hpp"

class IOperatorToken : public IToken {
  public:
    static std::unique_ptr<IOperatorToken> parse(char, int &);
    // For now, having up to two arguments works for +, -, *, / and ^.
    // For more complicated operations this interface may be reworked to
    // support more than two arguments for an operation.
    // If a ConstantToken can not be found for a term, nullptr is passed.
    virtual std::unique_ptr<ConstantToken> evaluate(const ConstantToken *, const ConstantToken *) = 0;
};