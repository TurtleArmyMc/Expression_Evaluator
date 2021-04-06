#pragma once

#include "../EvaluateException.hpp"
#include "IOperatorToken.hpp"
#include "ISignChangeToken.hpp"

class SubtractOperatorToken : public ISignChangeToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for subtraction operation.");
        }
        if (a) {
            return a->subtract(b);
        } else {
            return std::make_unique<ConstantToken>(-b->getValue());
        }
    }

    std::unique_ptr<ConstantToken> changeSign(const ConstantToken *token) {
        return std::make_unique<ConstantToken>(-token->getValue());
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<SubtractOperatorToken>(); }

    std::string toString() const { return "-"; }
};