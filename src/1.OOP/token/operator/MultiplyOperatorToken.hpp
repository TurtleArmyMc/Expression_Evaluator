#pragma once

#include "../EvaluateException.hpp"
#include "IOperatorToken.hpp"

class MultiplyOperatorToken : public IOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!a || !b) {
            throw EvaluateException("Could not find term for multiplication operation.");
        }
        return a->multiply(b);
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<MultiplyOperatorToken>(*this); }

    std::string toString() const { return "*"; }
};