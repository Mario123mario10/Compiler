#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include "Lekser.hpp"

int main()
{
    string sourceCode = R"(
        def factorial(n) {
            if (n == 0) {
                return 1
            } else {
                return n * factorial(n - 1)
            }
            if (true) return null
        }
        &&
        print("Factorial of 5 is " + {factorial(5)})
        "ala ma \"kota\"\naaaaaa" "basia ma helikopter"
        #x = 888888888333333333333333333333333333333333333333333333333333333333
x=3y=7
x=3y=7.222 .2
#x=8888888888

print({Factorial of 5 is }, factorial(5))
        
    )";

    Lexer lexer(10,20);
    lexer.loadFromString(sourceCode);

    try
    {
        //lexer.loadFromFile("kod.txt");
    }
    catch (const runtime_error& error)
    {
        cerr << error.what() << endl;
        return 1;
    }
        
    vector<Token> tokens;
    try
    {
        Token token;
        do
        {
            token = lexer.nextToken();
            tokens.push_back(token);
        } while (token.type != TokenType::ETX);
    }
    catch (const runtime_error& error)
    {
        cerr << "Blad podczas tokenizacji: " << error.what() << endl;
        return 1;
    }
    
    //double value = 7.222;
    //string = to_string(value);

    cout << "Tokens:\n";
    cout << left << setw(20) << "Type" << setw(10) << "Value" << "Position (Line:Column)\n";
    cout << string(60, '-') << '\n';

    for (const auto& token : tokens)
    {
        string tokenType;
        switch (token.type)
        {
        case TokenType::IntConst: tokenType = "IntConst"; break;
        case TokenType::DoubleConst: tokenType = "DoubleConst"; break;
        case TokenType::BoolConst: tokenType = "BoolConst"; break;
        case TokenType::NullConst: tokenType = "NullConst"; break;
        case TokenType::StringConst: tokenType = "StringConst"; break;
        case TokenType::Id: tokenType = "Id"; break;
        case TokenType::Keyword: tokenType = "Keyword"; break;
        case TokenType::Symbol: tokenType = "Symbol"; break;
        case TokenType::Comment: tokenType = "Comment"; break;
        case TokenType::Whitespaces: tokenType = "Whitespaces"; break;
        default: tokenType = "End of Text";
        }

        string value = visit([](auto&& arg) -> string {
            using T = decay_t<decltype(arg)>;
            if constexpr (is_same_v<T, string>) return arg;
            else if constexpr (is_same_v<T, int> || is_same_v<T, double>) return to_string(arg);
            else return "n/a";
            }, token.value);

        cout << setw(20) << tokenType << setw(20) << value << token.lineNumber << ":" << token.columnNumber << '\n';
    }

    return 0;
}