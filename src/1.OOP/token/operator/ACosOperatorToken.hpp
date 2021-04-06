#pragma once

#include "../EvaluateException.hpp"
#include "IFunctionOperatorToken.hpp"
#include <cmath>

class ACosOperatorToken : public IFunctionOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for ACOS operation.");
        }
        if (a) {
            return std::make_unique<ConstantToken>(a->getValue() * std::acos(b->getValue()));
        } else {
            return std::make_unique<ConstantToken>(std::acos(b->getValue()));
        }
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<ACosOperatorToken>(*this); }

    std::string toString() const { return "ACOS"; }
};