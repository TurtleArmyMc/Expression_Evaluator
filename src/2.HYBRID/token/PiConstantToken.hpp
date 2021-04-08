#pragma once

#include "ConstantToken.hpp"
#include <cmath>

class PiConstantToken : public ConstantToken {
  public:
    PiConstantToken() {
        type = Pi;
        value = M_PI;
    };
    virtual std::unique_ptr<Token> clone() const override { return std::make_unique<PiConstantToken>(); }
    virtual std::string toString() const override { return "π"; }
};