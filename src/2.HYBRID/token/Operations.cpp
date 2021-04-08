#include "Operations.hpp"
#include "EvaluateException.hpp"
#include <cmath>

std::unique_ptr<ConstantToken> addTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for addition operation.");
    }
    if (!a) {
        return std::make_unique<ConstantToken>(b->getValue());
    } else {
        return std::make_unique<ConstantToken>(a->getValue() + b->getValue());
    }
}

std::unique_ptr<ConstantToken> subtractTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for subtraction operation.");
    }
    if (!a) {
        return std::make_unique<ConstantToken>(-b->getValue());
    } else {
        return std::make_unique<ConstantToken>(a->getValue() - b->getValue());
    }
}

std::unique_ptr<ConstantToken> multiplyTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!a || !b) {
        throw EvaluateException("Could not find term for multiplication operation.");
    }
    return std::make_unique<ConstantToken>(a->getValue() * b->getValue());
}

std::unique_ptr<ConstantToken> divideTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!a || !b) {
        throw EvaluateException("Could not find term for division operation.");
    }
    return std::make_unique<ConstantToken>(a->getValue() / b->getValue());
}

std::unique_ptr<ConstantToken> powerTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!a || !b) {
        throw EvaluateException("Could not find term for power operation.");
    }
    return std::make_unique<ConstantToken>(std::pow(a->getValue(), b->getValue()));
}

std::unique_ptr<ConstantToken> sinTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for SIN operation.");
    }
    if (a) {
        return std::make_unique<ConstantToken>(a->getValue() * std::sin(b->getValue()));
    } else {
        return std::make_unique<ConstantToken>(std::sin(b->getValue()));
    }
}

std::unique_ptr<ConstantToken> cosTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for COS operation.");
    }
    if (a) {
        return std::make_unique<ConstantToken>(a->getValue() * std::cos(b->getValue()));
    } else {
        return std::make_unique<ConstantToken>(std::cos(b->getValue()));
    }
}

std::unique_ptr<ConstantToken> tanTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for TAN operation.");
    }
    if (a) {
        return std::make_unique<ConstantToken>(a->getValue() * std::tan(b->getValue()));
    } else {
        return std::make_unique<ConstantToken>(std::tan(b->getValue()));
    }
}

std::unique_ptr<ConstantToken> asinTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for ASIN operation.");
    }
    if (a) {
        return std::make_unique<ConstantToken>(a->getValue() * std::asin(b->getValue()));
    } else {
        return std::make_unique<ConstantToken>(std::asin(b->getValue()));
    }
}

std::unique_ptr<ConstantToken> acosTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for ACOS operation.");
    }
    if (a) {
        return std::make_unique<ConstantToken>(a->getValue() * std::acos(b->getValue()));
    } else {
        return std::make_unique<ConstantToken>(std::acos(b->getValue()));
    }
}

std::unique_ptr<ConstantToken> atanTokens(const ConstantToken *const a, const ConstantToken *const b) {
    if (!b) {
        throw EvaluateException("Could not find term for ATAN operation.");
    }
    if (a) {
        return std::make_unique<ConstantToken>(a->getValue() * std::atan(b->getValue()));
    } else {
        return std::make_unique<ConstantToken>(std::atan(b->getValue()));
    }
}
