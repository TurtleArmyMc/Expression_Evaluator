#pragma once

#include "../EvaluateException.hpp"
#include "IOperatorToken.hpp"
#include "ISignChangeToken.hpp"

class AddOperatorToken : public ISignChangeToken {
  public:
    std::unique_ptr<ConstantToken> evaluate(const ConstantToken *a, const ConstantToken *b) {
        if (!b) {
            throw EvaluateException("Could not find term for addition operation.");
        }
        if (a) {
            return a->add(b);
        } else {
            return std::make_unique<ConstantToken>(b->getValue());
        }
    }

    std::unique_ptr<ConstantToken> changeSign(const ConstantToken *token) {
        return std::make_unique<ConstantToken>(token->getValue());
    }

    std::unique_ptr<IToken> clone() const { return std::make_unique<AddOperatorToken>(*this); }

    std::string toString() const { return "+"; }
};