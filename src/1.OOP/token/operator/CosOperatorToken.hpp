#pragma once

#include "../EvaluateException.hpp"
#include "IFunctionOperatorToken.hpp"
#include <cmath>

class CosOperatorToken : public IFunctionOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for COS operation.");
        }
        if (a) {
            return std::make_unique<ConstantToken>(a->getValue() * std::cos(b->getValue()));
        } else {
            return std::make_unique<ConstantToken>(std::cos(b->getValue()));
        }
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<CosOperatorToken>(*this); }

    std::string toString() const { return "COS"; }
};