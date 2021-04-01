#pragma once

#include "../EvaluateException.hpp"
#include "IOperatorToken.hpp"

class PowerOperatorToken : public IOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!a || !b) {
            throw EvaluateException("Could not find term for power operation.");
        }
        return a->pow(b);
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<PowerOperatorToken>(*this); }

    std::string toString() const { return "^"; }
};