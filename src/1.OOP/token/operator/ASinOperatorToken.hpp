#pragma once

#include "../EvaluateException.hpp"
#include "IFunctionOperatorToken.hpp"
#include <cmath>

class ASinOperatorToken : public IFunctionOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for ASIN operation.");
        }
        if (a) {
            return std::make_unique<ConstantToken>(a->getValue() * std::asin(b->getValue()));
        } else {
            return std::make_unique<ConstantToken>(std::asin(b->getValue()));
        }
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<ASinOperatorToken>(*this); }

    std::string toString() const { return "ASIN"; }
};