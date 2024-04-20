#pragma once
#include <variant>
#include <string>

using namespace std;

enum class TokenType
{
    IntConst,
    DoubleConst,
    TrueConst,
    FalseConst,
    StringConst,
    Id,
    Keyword,
    Symbol,
    Comment,
    ETX
};

using TokenValue = variant<int, double, string>;

struct Token
{
    TokenType type;
    TokenValue value;
    size_t lineNumber;
    size_t columnNumber;
};

enum class KW { DEF, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN, SELECT, FROM, WHERE, ORDER, BY, DESC, LIMIT, TNULL };
enum class SYM {
    PLUS, MINUS, MULTIPLY, DIVIDE, POWER, EQUALS, NOT_EQUAL, GREATER_THAN, GREATER_EQUAL, LESS_THAN, LESS_EQUAL, AND, OR, NOT,
    LEFT_PAR, RIGH_PAR, LEFT_BRACE, RIGHT_BRACE, ASSIGN, COMMA, COLON, DOT
};