#pragma once

#include "IValuedToken.hpp"
#include <memory>

class ConstantToken : public IValuedToken {
  protected:
    long double value;
    static bool validChar(int c);
    ConstantToken();

  public:
    ConstantToken(long double d);
    static std::unique_ptr<ConstantToken> parse(const std::string &, int &);
    virtual long double getValue() const;
    virtual bool canBeMultipliedByConstant() const override;
    virtual std::unique_ptr<IToken> clone() const override;
    virtual std::string toString() const override;

    std::unique_ptr<ConstantToken> add(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> subtract(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> multiply(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> pow(const ConstantToken *) const;
    std::unique_ptr<ConstantToken> divide(const ConstantToken *) const;
};