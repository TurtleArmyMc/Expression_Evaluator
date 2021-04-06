#include "ExpressionToken.hpp"
#include "IEvaluatableToken.hpp"
#include "ParseException.hpp"
#include "PiConstantToken.hpp"
#include "operator/ACosOperatorToken.hpp"
#include "operator/ASinOperatorToken.hpp"
#include "operator/ATanOperatorToken.hpp"
#include "operator/AddOperatorToken.hpp"
#include "operator/CosOperatorToken.hpp"
#include "operator/DivideOperatorToken.hpp"
#include "operator/IFunctionOperatorToken.hpp"
#include "operator/IOperatorToken.hpp"
#include "operator/ISignChangeToken.hpp"
#include "operator/MultiplyOperatorToken.hpp"
#include "operator/PowerOperatorToken.hpp"
#include "operator/SinOperatorToken.hpp"
#include "operator/SubtractOperatorToken.hpp"
#include "operator/TanOperatorToken.hpp"
#include <cstring>

ExpressionToken::ExpressionToken() {}

std::unique_ptr<ExpressionToken> ExpressionToken::parse(const std::string &s) {
    int i = 0;
    return parse(s, false, i);
}

std::unique_ptr<ExpressionToken> ExpressionToken::parse(const std::string &s, bool inParenthesis, int &i) {
    std::unique_ptr<ExpressionToken> ret = std::unique_ptr<ExpressionToken>(new ExpressionToken());

    int start = i;

    // Used to automatically insert *s in between constants/evaluatable tokens
    // and tokens that can be multiplied by constants. For example, 5(6) should
    // become 5*(6).
    bool lastTokenConstant = false;
    bool canMultiplyLastTokenByConstant = false;

    // i is be incremented inside of other parse functions, when whitespace is
    // found, or before entering nested parenthesis. This is to allow nested
    // parse functions to iterate i as far as possible before reaching the end
    // of whatever they were parsing, without needing to then backtrack i.
    while (i < s.length()) {
        switch (s[i]) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            ret->tokens.push_back(std::unique_ptr<IToken>(IOperatorToken::parse(s[i], i)));
            lastTokenConstant = false;
            canMultiplyLastTokenByConstant = false;
            break;
        case '0' ... '9':
        case '.':
            if (canMultiplyLastTokenByConstant) {
                ret->tokens.push_back(std::make_unique<MultiplyOperatorToken>());
            }
            ret->tokens.push_back(std::unique_ptr<IToken>(ConstantToken::parse(s, i)));
            lastTokenConstant = true;
            canMultiplyLastTokenByConstant = false;
            break;
        case '(': {
            std::unique_ptr<ExpressionToken> expressionToken =
                std::unique_ptr<ExpressionToken>(ExpressionToken::parse(s, true, ++i));
            if (lastTokenConstant && expressionToken->canBeMultipliedByConstant()) {
                ret->tokens.push_back(std::make_unique<MultiplyOperatorToken>());
            }
            lastTokenConstant = true;
            canMultiplyLastTokenByConstant = expressionToken->canBeMultipliedByConstant();
            ret->tokens.push_back(std::move(expressionToken));
        } break;
        case ')':
            if (inParenthesis) {
                ++i;
                return std::move(ret);
            } else {
                throw ParseException("Unmatched closing parenthesis.", i);
            }
            break;
        default:
            if (std::isspace((s[i]))) {
                ++i;
            } else {
                // Will throw exception if there is no special token starting
                // at i.
                std::unique_ptr<IToken> token = parseSpecialToken(s, i);

                IValuedToken *valuedToken = dynamic_cast<IValuedToken *>(token.get());
                if (valuedToken) {
                    if (valuedToken->canBeMultipliedByConstant()) {
                        if (lastTokenConstant) {
                            ret->tokens.push_back(std::make_unique<MultiplyOperatorToken>());
                        }
                        canMultiplyLastTokenByConstant = true;
                    } else if (canMultiplyLastTokenByConstant) {
                        ret->tokens.push_back(std::make_unique<MultiplyOperatorToken>());
                        canMultiplyLastTokenByConstant = false;
                    } else {
                        canMultiplyLastTokenByConstant = false;
                    }
                    lastTokenConstant = true;
                } else {
                    canMultiplyLastTokenByConstant = false;
                    lastTokenConstant = false;
                }
                ret->tokens.push_back(std::move(token));
            }
            break;
        }
    }
    if (!inParenthesis) {
        return std::move(ret);
    } else {
        throw ParseException("Could not find closing parenthesis.", start - 1);
    }
}

std::unique_ptr<IToken> ExpressionToken::parseSpecialToken(const std::string &s, int &i) {
    static const char *tokenNames[7] = {
        "PI", "SIN", "COS", "TAN", "ASIN", "ACOS", "ATAN",
    };
    static const std::unique_ptr<IToken> token[7] = {
        std::make_unique<PiConstantToken>(),   std::make_unique<SinOperatorToken>(),
        std::make_unique<CosOperatorToken>(),  std::make_unique<TanOperatorToken>(),
        std::make_unique<ASinOperatorToken>(), std::make_unique<ACosOperatorToken>(),
        std::make_unique<ATanOperatorToken>()};

    for (int tokenName = 0; tokenName < 7; tokenName++) {
        int j;
        for (j = 0;
             tokenNames[tokenName][j] && i + j < s.length() && (tokenNames[tokenName][j] == std::toupper(s[i + j]));
             j++) {
        }
        // If end of token name was reached, a valid token was found
        if (!tokenNames[tokenName][j]) {
            i += j;
            return token[tokenName]->clone();
        }
    }

    throw ParseException(s[i], i);
}

std::unique_ptr<ConstantToken> ExpressionToken::evaluate() const {
    if (tokens.size() == 0) {
        throw EvaluateException("Could not evaluate empty expression.", 0);
    }

    std::vector<std::unique_ptr<IToken>> tokensCopy;

    // Copies tokens to a temp vector. Evaluatable tokens are automatically
    // evaluated and only their value is stored in tokensCopy.
    for (const std::unique_ptr<IToken> &token : tokens) {
        tokensCopy.push_back(token->clone());
    }

    evaluateOperation<IFunctionOperatorToken, IFunctionOperatorToken>(tokensCopy);
    evaluateOperation<PowerOperatorToken, PowerOperatorToken>(tokensCopy);
    evaluateOperation<MultiplyOperatorToken, DivideOperatorToken>(tokensCopy);
    evaluateOperation<AddOperatorToken, SubtractOperatorToken>(tokensCopy);

    if (tokensCopy.size() > 1) {
        // This should never be reached
        throw EvaluateException("Finished evaluating expression without successfully evaluating all tokens.");
    }

    return std::unique_ptr<ConstantToken>(static_cast<ConstantToken *>(tokensCopy[0].release()));
}

template <class Operation1, class Operation2>
void ExpressionToken::evaluateOperation(std::vector<std::unique_ptr<IToken>> &tokensCopy) {
    bool lastTokenConstant = false;

    for (auto i = tokensCopy.begin(); i < tokensCopy.end();) {
        IOperatorToken *operatorToken = dynamic_cast<Operation1 *>(i->get());

        // In the case of the power operator, there is no root operator to be
        // evaluated simultaenously, so Operation1 and Operation2 will both be
        // PowerOperatorToken and there's no reason to cast a second time if
        // the first cast fails
        if (!std::is_same<Operation1, Operation2>::value) {
            // Opposite operations should be evaluated at the same time (ex +
            // and -; * and /) and the program tries to cast the iterator to
            // both operators.
            if (operatorToken == nullptr) {
                operatorToken = dynamic_cast<Operation2 *>(i->get());
            }
        }

        // Checks if the current token is one of the operators being evaluated
        if (operatorToken != nullptr) {
            ConstantToken *a = (i != tokensCopy.begin() ? dynamic_cast<ConstantToken *>((i - 1)->get()) : nullptr);
            ConstantToken *b = getNextConstant(i + 1, tokensCopy);

            *i = std::unique_ptr<ConstantToken>(operatorToken->evaluate(a, b));
            lastTokenConstant = true;
            if (b) {
                tokensCopy.erase(i + 1);
            }
            if (a) {
                tokensCopy.erase(i - 1);
                continue;
            }
        } else {
            // Check if the current token is a constant. If not, it must be an
            // operator not currently being evaluated.
            ConstantToken *constant = dynamic_cast<ConstantToken *>(i->get());

            // If the function has found two constants in a row without an
            // operation in between them, throw an exception.
            if (lastTokenConstant && constant != nullptr) {
                throw EvaluateException("Could not determine operation for constants '" +
                                        static_cast<ConstantToken *>((i - 1)->get())->toString() + "' and '" +
                                        constant->toString() + "'.");
            } else {
                lastTokenConstant = constant != nullptr;
            }
        }
        i++;
    }
}

ConstantToken *ExpressionToken::getNextConstant(std::vector<std::unique_ptr<IToken>>::iterator i,
                                                std::vector<std::unique_ptr<IToken>> &tokensCopy) {
    if (i < tokensCopy.end()) {
        // Check if the current token is a constant and return it if it is.
        ConstantToken *constToken = dynamic_cast<ConstantToken *>(i->get());
        if (constToken != nullptr) {
            return constToken;
        }

        // Check if the current token is a sign change token (+ or -) and keep
        // looking for a constant token or more sign change tokens.
        ISignChangeToken *signChangeToken = dynamic_cast<ISignChangeToken *>(i->get());
        if (signChangeToken != nullptr && i + 1 < tokensCopy.end()) {
            ConstantToken *nextConst = getNextConstant(i + 1, tokensCopy);

            // Return nullptr if no constant was found after the sign change
            // token.
            if (nextConst == nullptr) {
                return nullptr;
            }

            // Combine the two tokens by erasing the next constant and storing
            // the new constant with its sign changed where the sign change
            // token was.
            *i = signChangeToken->changeSign(nextConst);
            tokensCopy.erase(i + 1);

            // Now guaranteed to point to a ConstantToken
            return static_cast<ConstantToken *>(i->get());
        }
    }

    // Return nullptr if there is no next token, or if the next token is not a
    // constant token or sign change token.
    return nullptr;
}

std::string ExpressionToken::toString() const {
    std::string ret("(");
    for (const std::unique_ptr<IToken> &token : tokens) {
        ret += ' ' + token->toString();
    }
    ret += " )";
    return ret;
}

bool ExpressionToken::canBeMultipliedByConstant() const { return true; }