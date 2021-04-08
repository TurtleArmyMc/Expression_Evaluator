#include "ExpressionToken.hpp"
#include "EvaluateException.hpp"
#include "Operations.hpp"
#include "ParseException.hpp"
#include "PiConstantToken.hpp"
#include <cstring>
#include <map>

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

    while (i < s.length()) {
        switch (s[i]) {
        case '+':
            ret->tokens.push_back(std::make_unique<Token>(Add));
            lastTokenConstant = false;
            canMultiplyLastTokenByConstant = false;
            ++i;
            break;

        case '-':
            ret->tokens.push_back(std::make_unique<Token>(Subtract));
            lastTokenConstant = false;
            canMultiplyLastTokenByConstant = false;
            ++i;
            break;

        case '*':
            ret->tokens.push_back(std::make_unique<Token>(Multiply));
            lastTokenConstant = false;
            canMultiplyLastTokenByConstant = false;
            ++i;
            break;

        case '/':
            ret->tokens.push_back(std::make_unique<Token>(Divide));
            lastTokenConstant = false;
            canMultiplyLastTokenByConstant = false;
            ++i;
            break;

        case '^':
            ret->tokens.push_back(std::make_unique<Token>(Power));
            lastTokenConstant = false;
            canMultiplyLastTokenByConstant = false;
            ++i;
            break;
        case '0' ... '9':
        case '.':
            if (canMultiplyLastTokenByConstant) {
                ret->tokens.push_back(std::make_unique<Token>(Multiply));
            }
            ret->tokens.push_back(std::unique_ptr<Token>(ConstantToken::parse(s, i)));
            lastTokenConstant = true;
            canMultiplyLastTokenByConstant = false;
            break;
        case '(': {
            if (canMultiplyLastTokenByConstant || lastTokenConstant) {
                ret->tokens.push_back(std::make_unique<Token>(Multiply));
            }
            ret->tokens.push_back(ExpressionToken::parse(s, true, ++i));
            lastTokenConstant = true;
            canMultiplyLastTokenByConstant = true;
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
                std::unique_ptr<Token> token = parseSpecialToken(s, i);
                bool canMultiplyTokenByConstant = Token::canBeMultipliedByConstant.at(token->getType());
                bool tokenConstant = Token::isConstant(token);
                if ((canMultiplyLastTokenByConstant && tokenConstant) ||
                    (lastTokenConstant && canMultiplyTokenByConstant)) {
                    ret->tokens.push_back(std::make_unique<Token>(Multiply));
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

std::unique_ptr<Token> ExpressionToken::parseSpecialToken(const std::string &s, int &i) {
    static const std::map<const char *, TokenType> specialTokens = {
        {"PI", Pi}, {"SIN", Sin}, {"COS", Cos}, {"TAN", Tan}, {"ASIN", ASin}, {"ACOS", ACos}, {"ATAN", ATan}};

    for (const auto &token : specialTokens) {
        int j;
        for (j = 0; token.first[j] && i + j < s.length() && (token.first[j] == std::toupper(s[i + j])); j++) {
        }
        // If end of the token's name was reached, a valid token was found
        if (!token.first[j]) {
            i += j;
            return Token::getToken(token.second);
        }
    }

    throw ParseException(s[i], i);
}

std::unique_ptr<ConstantToken> ExpressionToken::evaluate() const {
    if (tokens.size() == 0) {
        throw EvaluateException("Could not evaluate empty expression.", 0);
    }

    std::vector<std::unique_ptr<Token>> tokensCopy;

    // Copies tokens to a temp vector. Evaluatable tokens are automatically
    // evaluated and only their value is stored in tokensCopy.
    for (const std::unique_ptr<Token> &token : tokens) {
        tokensCopy.push_back(token->clone());
    }

    evaluateOperations({Sin, Cos, Tan, ASin, ACos, ATan}, tokensCopy);
    evaluateOperations({Power}, tokensCopy);
    evaluateOperations({Multiply, Divide}, tokensCopy);
    evaluateOperations({Add, Subtract}, tokensCopy);

    if (tokensCopy.size() > 1) {
        // This should never be reached
        throw EvaluateException("Finished evaluating expression without successfully evaluating all tokens.");
    }

    return std::unique_ptr<ConstantToken>(static_cast<ConstantToken *>(tokensCopy[0].release()));
}

void ExpressionToken::evaluateOperations(const std::vector<TokenType> operations,
                                         std::vector<std::unique_ptr<Token>> &evaluateTokens) {
    bool lastTokenConstant = false;

    for (auto i = evaluateTokens.begin(); i < evaluateTokens.end();) {
        bool evaluatedOperator = false;
        bool increment = true;

        for (const TokenType &operation : operations) {
            if ((*i)->getType() == operation) {
                const ConstantToken *const a =
                    (lastTokenConstant ? static_cast<ConstantToken *>((i - 1)->get()) : nullptr);
                const ConstantToken *const b = getNextConstant(i + 1, evaluateTokens);
                *i = tokenToOperation.at(operation)(a, b);
                if (b) {
                    evaluateTokens.erase(i + 1);
                }
                if (a) {
                    evaluateTokens.erase(i - 1);
                    increment = false;
                }
                evaluatedOperator = true;
                lastTokenConstant = true;
                break;
            }
        }

        if (!evaluatedOperator) {
            bool tokenIsConstant = Token::isConstant(*i);
            if (tokenIsConstant && lastTokenConstant) {
                throw EvaluateException("Could not determine operation for constants '" + (*(i - 1))->toString() +
                                        "' and '" + (*i)->toString() + "'.");
            }
            lastTokenConstant = tokenIsConstant;
        }
        if (increment) {
            i++;
        }
    }
}

ConstantToken *ExpressionToken::getNextConstant(std::vector<std::unique_ptr<Token>>::iterator i,
                                                std::vector<std::unique_ptr<Token>> &evaluateTokens) {
    if (i < evaluateTokens.end()) {
        // Check if the current token is a constant and return it if it is.
        if (Token::isConstant(*i)) {
            return static_cast<ConstantToken *>(i->get());
        }

        // Loop over 'combinable tokens', such as negate and functions, and
        // try to combine them with the next constant, and return the new
        // modified constant.
        for (const TokenType &combinableOperation : {Add, Subtract, Sin, Cos, Tan, ASin, ACos, ATan}) {
            if ((*i)->getType() == combinableOperation) {
                ConstantToken *nextConst = getNextConstant(i + 1, evaluateTokens);
                if (nextConst != nullptr) {
                    // Combine the two tokens by replacing the combinable
                    // operation with the result of said operation on the next
                    // constant and erasing the next constant.
                    *i = tokenToOperation.at(combinableOperation)(nullptr, nextConst);
                    evaluateTokens.erase(i + 1);

                    // Now guaranteed to point to a ConstantToken
                    return static_cast<ConstantToken *>(i->get());
                } else {
                    return nullptr;
                }
            }
        }
    }

    // Return nullptr if there is no next token, or if the next token is not a
    // constant token or sign change token.
    return nullptr;
}

std::unique_ptr<Token> ExpressionToken::clone() const { return evaluate(); }

std::string ExpressionToken::toString() const {
    std::string ret("(");
    for (const std::unique_ptr<Token> &token : tokens) {
        ret += ' ' + token->toString();
    }
    ret += " )";
    return ret;
}
