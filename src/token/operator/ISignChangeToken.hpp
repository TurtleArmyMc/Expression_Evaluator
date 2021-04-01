#pragma once

#include "../ConstantToken.hpp"
#include "IOperatorToken.hpp"

class ISignChangeToken : public IOperatorToken {
  public:
    virtual std::unique_ptr<ConstantToken> changeSign(const ConstantToken *token) = 0;
};