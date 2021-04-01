#pragma once

#include "ConstantToken.hpp"
#include <memory>

class IEvaluatableToken : public IValuedToken {
  public:
    virtual std::unique_ptr<ConstantToken> evaluate() const = 0;
    virtual std::unique_ptr<IToken> clone() const override {
        // Evaluatable tokens are evaluated to constant tokens when cloned.
        return evaluate();
    }
};