#pragma once

#include "IToken.hpp"

class IValuedToken : public IToken {
  public:
    // Whether or not * should be inserted in between this token and a constant
    // if there is no other operation in between them.
    virtual bool canBeMultipliedByConstant() const = 0;
};