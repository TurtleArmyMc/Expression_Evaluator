#include "ConstantToken.hpp"
#include "ParseException.hpp"
#include <cmath>

bool ConstantToken::validChar(int c) { return c == '.' || ('0' <= c && c <= '9'); }

std::unique_ptr<ConstantToken> ConstantToken::parse(const std::string &s, int &i) {
    bool hasDecimal = false;
    std::string sNum;
    for (; i < s.length() && validChar(s[i]); i++) {
        if (s[i] == '.') {
            if (hasDecimal) {
                // Throw exception if two decimal points are found
                throw ParseException(s[i], i);
            } else {
                hasDecimal = true;
            }
        }
        sNum.push_back(s[i]);
    }

    return std::make_unique<ConstantToken>(std::stold(sNum));
}

bool ConstantToken::canBeMultipliedByConstant() const { return false; }

ConstantToken::ConstantToken(long double d) { value = d; }

long double ConstantToken::getValue() const { return value; }

std::unique_ptr<ConstantToken> ConstantToken::add(const ConstantToken *token) const {
    return std::make_unique<ConstantToken>(this->value + token->getValue());
}

std::unique_ptr<ConstantToken> ConstantToken::subtract(const ConstantToken *token) const {
    return std::make_unique<ConstantToken>(this->value - token->getValue());
}

std::unique_ptr<ConstantToken> ConstantToken::multiply(const ConstantToken *token) const {
    return std::make_unique<ConstantToken>(this->value * token->getValue());
}

std::unique_ptr<ConstantToken> ConstantToken::divide(const ConstantToken *token) const {
    return std::make_unique<ConstantToken>(this->value / token->getValue());
}

std::unique_ptr<ConstantToken> ConstantToken::pow(const ConstantToken *token) const {
    return std::make_unique<ConstantToken>(std::pow(this->value, token->getValue()));
}

std::unique_ptr<IToken> ConstantToken::clone() const { return std::make_unique<ConstantToken>(*this); }

std::string ConstantToken::toString() const {
    std::string ret = std::to_string(value);
    // Remove the decimal point and everything after it if the token is an
    // integer, or remove trailing 0s after the decimal point if there are any
    int dotIndex = ret.find('.');
    if (dotIndex == -1) {
        return ret;
    }
    int notZeroIndex = ret.find_last_not_of('0');
    return ret.substr(0, notZeroIndex > dotIndex ? notZeroIndex + 1 : dotIndex);
}