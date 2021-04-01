#pragma once

#include "../EvaluateException.hpp"
#include "IOperatorToken.hpp"

class DivideOperatorToken : public IOperatorToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!a || !b) {
            throw EvaluateException("Could not find term for division operation.");
        }
        return a->divide(b);
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<DivideOperatorToken>(*this); }

    std::string toString() const { return "/"; }
};