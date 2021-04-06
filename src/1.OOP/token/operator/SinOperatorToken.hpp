#pragma once

#include "../EvaluateException.hpp"
#include "IFunctionOperatorToken.hpp"
#include <cmath>

class SinOperatorToken : public IFunctionOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for SIN operation.");
        }
        if (a) {
            return std::make_unique<ConstantToken>(a->getValue() * std::sin(b->getValue()));
        } else {
            return std::make_unique<ConstantToken>(std::sin(b->getValue()));
        }
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<SinOperatorToken>(*this); }

    std::string toString() const { return "SIN"; }
};