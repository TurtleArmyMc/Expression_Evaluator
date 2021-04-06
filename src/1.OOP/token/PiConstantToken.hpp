#pragma once

#include "ConstantToken.hpp"
#include <cmath>

class PiConstantToken : public ConstantToken {
  public:
    PiConstantToken() { value = M_PI; };
    virtual std::unique_ptr<IToken> clone() const override { return std::make_unique<PiConstantToken>(*this); }
    virtual bool canBeMultipliedByConstant() const override { return true; }
    virtual std::string toString() const override { return "π"; }
};