#pragma once

#include "../EvaluateException.hpp"
#include "IFunctionOperatorToken.hpp"
#include <cmath>

class TanOperatorToken : public IFunctionOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for TAN operation.");
        }
        if (a) {
            return std::make_unique<ConstantToken>(a->getValue() * std::tan(b->getValue()));
        } else {
            return std::make_unique<ConstantToken>(std::tan(b->getValue()));
        }
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<TanOperatorToken>(*this); }

    std::string toString() const { return "TAN"; }
};