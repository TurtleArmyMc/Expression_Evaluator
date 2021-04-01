#pragma once

#include "IValuedToken.hpp"
#include <memory>

class ConstantToken : public IValuedToken {
  private:
    long double value;
    static bool validChar(int c);

  public:
    static std::unique_ptr<ConstantToken> parse(const std::string &, int &);
    ConstantToken(long double d);
    long double getValue() const;
    bool canBeMultipliedByConstant() const;
    virtual std::unique_ptr<IToken> clone() const override;
    std::string toString() const;

    std::unique_ptr<ConstantToken> add(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> subtract(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> multiply(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> pow(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> divide(const ConstantToken *) const;
};