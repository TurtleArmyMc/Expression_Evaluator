#include "token/EvaluateException.hpp"
#include "token/ExpressionToken.hpp"
#include "token/ParseException.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        while (true) {
            std::string userString;
            std::cout << "Enter an expression or 'done':\n";
            std::getline(std::cin, userString);

            if (userString == "done") {
                break;
            }

            try {
                std::unique_ptr<ExpressionToken> userExpression = ExpressionToken::parse(userString);
                std::cout << userExpression->toString() << '\n';
                std::cout << "Answer: " << userExpression->evaluate()->toString() << "\n\n";
            } catch (const ParseException &e) {
                for (int i = 0; i < e.getCharNumber(); i++) {
                    std::cout << ' ';
                }
                std::cout << "^ Error: " << e.what() << "\n\n";
            } catch (const EvaluateException &e) {
                std::cout << "Error: " << e.what() << "\n\n";
            } catch (const std::exception &e) {
                std::cout << "Error: " << e.what() << "\n\n";
            }
        }
    } else {
        std::string userString;
        for (int i = 1; i < argc; i++) {
            userString += argv[i];
        }
        std::cout << userString << '\n';

        try {
            std::unique_ptr<ExpressionToken> userExpression = ExpressionToken::parse(userString);
            std::cout << userExpression->toString() << '\n';
            std::cout << "Answer: " << userExpression->evaluate()->toString() << '\n';
        } catch (const ParseException &e) {
            for (int i = 0; i < e.getCharNumber(); i++) {
                std::cout << ' ';
            }
            std::cout << "^ Error: " << e.what() << '\n';
        } catch (const EvaluateException &e) {
            std::cout << "Error: " << e.what() << '\n';
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}