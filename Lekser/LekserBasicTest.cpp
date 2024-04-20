#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include "Lekser.h"

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
<=
        }
        &&
        print("Factorial of 5 is ", factorial(5))
        "ala ma \"kota\"\naaaaaa" "basia ma helikopter"
        #x = 888888888333333333333333333333333333333333333333333333333333333333
x=3y=7
x=3y=7.222 .2
#x=8888888888
        
    )";

    string filePath = "kod.txt";
    ifstream file(filePath, ios::in | ios::binary); //ios::binary zeby nie liczylo podwojnie nowych linii w Windows
    if (!file.is_open()) throw runtime_error("Nieudany odczyt z pliku: " + filePath); 

    istringstream sourceString(sourceCode);
    Lexer lexer(sourceString, 10, 20);
    //Lexer lexer(file);
        
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
    catch (const Error& error)
    {
        cerr << "Blad podczas tokenizacji: " << error.what() << endl;
        return 1;
    }

    cout << "Tokens:\n";
    cout << left << setw(20) << "Type" << setw(10) << "Value" << "Position (Line:Column)\n";
    cout << string(60, '-') << '\n';

    for (const auto& token : tokens)
    {
        string tokenType, tokenValue;
        switch (token.type)
        {
        case TokenType::IntConst: tokenType = "IntConst"; tokenValue = to_string(get<int>(token.value)); break;
        case TokenType::DoubleConst: tokenType = "DoubleConst"; tokenValue = to_string(get<double>(token.value)); break;
        case TokenType::TrueConst: tokenType = "TrueConst"; break;
        case TokenType::FalseConst: tokenType = "FalseConst"; break;
        case TokenType::StringConst: tokenType = "StringConst"; tokenValue = get<string>(token.value); break;
        case TokenType::Id: tokenType = "Id"; tokenValue = get<string>(token.value); break;
        case TokenType::Keyword: tokenType = "Keyword"; tokenValue = to_string(get<int>(token.value)); break;
        case TokenType::Symbol: tokenType = "Symbol"; tokenValue = to_string(get<int>(token.value)); break;
        case TokenType::Comment: tokenType = "Comment"; tokenValue = get<string>(token.value); break;
        default: tokenType = "End of Text";
        }

        cout << setw(20) << tokenType << setw(20) << tokenValue << token.lineNumber << ":" << token.columnNumber << '\n';
    }

    return 0;
}