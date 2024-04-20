#pragma once
#include <string>
#include <set>
#include <map>
#include <variant>
#include <limits>
#include <cmath>
#include <algorithm>

#include "Token.h"
#include "Skaner.h"
#include "Error.h"

using namespace std;

class Lexer
{
private:
    Skaner skaner;
    const size_t maxIdentifierLength = 64;
    const size_t maxStringLength = 1024;
    static const size_t maxNumberLength = 310; //1.7e308 : 309 cyfr i kropka

    Token lastToken; //ostatnio wygenerowany token

    map<string, KW> keywordMap =
    {
                {"def", KW::DEF},
                {"if", KW::IF},
                {"else", KW::ELSE},
                {"while", KW::WHILE},
                {"break", KW::BREAK},
                {"continue", KW::CONTINUE},
                {"return", KW::RETURN},
                {"select", KW::SELECT},
                {"from", KW::FROM},
                {"where", KW::WHERE},
                {"order", KW::ORDER},
                {"by", KW::BY},
                {"desc", KW::DESC},
                {"limit", KW::LIMIT},
                {"null", KW::TNULL}
    };

    // Mapa wszystkich symboli z ich unikalnymi numerami identyfikacyjnymi
    map<string, SYM> symbols =
    {
        {"+", SYM::PLUS}, {"-", SYM::MINUS},
        {"*", SYM::MULTIPLY}, {"/", SYM::DIVIDE},
        {"^", SYM::POWER},
        {"==", SYM::EQUALS}, {"!=", SYM::NOT_EQUAL}, {">", SYM::GREATER_THAN}, {">=", SYM::GREATER_EQUAL}, {"<", SYM::LESS_THAN}, {"<=", SYM::LESS_EQUAL},
        {"&&", SYM::AND}, {"||", SYM::OR}, {"!", SYM::NOT},
        {"(", SYM::LEFT_PAR}, {")", SYM::RIGH_PAR},
        {"{", SYM::LEFT_BRACE}, {"}", SYM::RIGHT_BRACE},
        {"=",SYM::ASSIGN},
        {",",SYM::COMMA},{":",SYM::COLON},{".",SYM::DOT}
    };
    set<string> symbolFirsts = { "&","|" }; //Poczatki symboli, ktore same nie sa symbolami

    bool tryBuildNumber();
    bool tryBuildIdentifierOrKeyword();
    bool tryBuildString();
    bool tryBuildComment();
    bool tryBuildSymbol();
    void readWhitespaces();

public:

    Lexer(istream& source, size_t maxIdentifierLength = 64, size_t maxStringLength = 1024) :
        skaner(source), maxIdentifierLength(maxIdentifierLength), maxStringLength(maxStringLength) {}
    size_t getMaxIdentifierLength() { return maxIdentifierLength; }
    size_t getMaxStringLength() { return maxStringLength; }
    static size_t getMaxNumberLength() { return maxNumberLength; }

    Token nextToken();
};
