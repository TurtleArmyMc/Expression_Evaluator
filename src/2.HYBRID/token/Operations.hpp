#pragma once

#include "ConstantToken.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

std::unique_ptr<ConstantToken> addTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> subtractTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> multiplyTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> divideTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> powerTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> sinTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> cosTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> tanTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> asinTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> acosTokens(const ConstantToken *const a, const ConstantToken *const b);
std::unique_ptr<ConstantToken> atanTokens(const ConstantToken *const a, const ConstantToken *const b);

const std::unordered_map<Token::TokenType, std::function<std::unique_ptr<ConstantToken>(const ConstantToken *const,
                                                                                        const ConstantToken *const)>>
    tokenToOperation = {
        {Token::Add, addTokens},       {Token::Subtract, subtractTokens}, {Token::Multiply, multiplyTokens},
        {Token::Divide, divideTokens}, {Token::Power, powerTokens},       {Token::Sin, sinTokens},
        {Token::Cos, cosTokens},       {Token::Tan, tanTokens},           {Token::ASin, asinTokens},
        {Token::ACos, acosTokens},     {Token::ATan, atanTokens},
};