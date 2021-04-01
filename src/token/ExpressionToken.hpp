#pragma once

#include "IEvaluatableToken.hpp"
#include <vector>

class ExpressionToken : public IEvaluatableToken {
  protected:
    ExpressionToken();
    std::vector<std::unique_ptr<IToken>> tokens;
    static std::unique_ptr<ExpressionToken> parse(const std::string &s, bool inParenthesis, int &i);

  public:
    template <class Operation1, class Operation2>
    static void evaluateOperation(std::vector<std::unique_ptr<IToken>> &tokensCopy);
    static ConstantToken *getNextConstant(std::vector<std::unique_ptr<IToken>>::iterator,
                                          std::vector<std::unique_ptr<IToken>> &);
    static std::unique_ptr<ExpressionToken> parse(const std::string &s);
    std::unique_ptr<ConstantToken> evaluate() const;
    bool canBeMultipliedByConstant() const;
    std::string toString() const;
};