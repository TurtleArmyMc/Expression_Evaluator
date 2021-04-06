#pragma once

#include "../EvaluateException.hpp"
#include "IFunctionOperatorToken.hpp"
#include <cmath>

class ATanOperatorToken : public IFunctionOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for ATAN operation.");
        }
        if (a) {
            return std::make_unique<ConstantToken>(a->getValue() * std::atan(b->getValue()));
        } else {
            return std::make_unique<ConstantToken>(std::atan(b->getValue()));
        }
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<ATanOperatorToken>(*this); }

    std::string toString() const { return "ATAN"; }
};