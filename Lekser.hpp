#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <variant>
#include <limits>
#include <algorithm>

using namespace std;
const char ETX_CHAR = '\x03'; //End of Text

enum class TokenType
{
    IntConst,
    DoubleConst,
    BoolConst,
    NullConst,
    StringConst,
    Id,
    Keyword,
    Symbol,
    Comment,
    Whitespaces,
    ETX
};

using TokenValue = variant<int, double, string>; 

struct Token
{
    TokenType type;
    TokenValue value;
    size_t lineNumber;
    size_t columnNumber;
    size_t absoluteIndex;
};

enum class KW { DEF, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN, SELECT, FROM, WHERE, ORDER, BY, DESC, LIMIT };
enum class SYM {
    PLUS, MINUS, MULTIPLY, DIVIDE, POWER, EQUALS, NOT_EQUAL, GREATER_THAN, GREATER_EQUAL, LESS_THAN, LESS_EQUAL, AND, OR, NOT,
    LEFT_PAR, RIGH_PAR, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET, ASSIGN, COMMA, COLON, DOT
};

class Lexer
{
private:
    string source;

    size_t currentPosition = 0;
    size_t currentLine = 1;
    size_t currentColumn = 1;

    const size_t maxIdentifierLength = 64;
    const unsigned long long maxStringLength = 9*1e18;

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
        {"[", SYM::LEFT_BRACKET}, {"]", SYM::RIGHT_BRACKET},
        {"=",SYM::ASSIGN},
        {",",SYM::COMMA},{":",SYM::COLON},{".",SYM::DOT}
    };

    void setSource(const string& source) { this->source = source; }
    char currentChar() { return currentPosition < source.length() ? source[currentPosition] : ETX_CHAR; }

    void advance()
    {
        if (currentChar() == '\n')
        {
            currentLine++;
            currentColumn = 0;
        }
        currentPosition++;
        currentColumn++;
    }

    Token readNumber()
    {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        size_t start = currentPosition;

        while (isdigit(currentChar()) || currentChar() == '.') advance();
        string numberStr = source.substr(start, currentPosition - start);

        size_t dots = count(numberStr.begin(), numberStr.end(), '.');

        // Check for multiple dots or invalid number format
        if (dots > 1)
        {
            throw runtime_error("Invalid number format with multiple dots at line " + to_string(startLine) + " column " + to_string(startColumn));
        }

        // Attempt to convert string to number
        if (dots > 0)
        {
            try
            {
                double value = stod(numberStr);
                return Token{ TokenType::DoubleConst, value, startLine, startColumn, start };
            }
            catch (const out_of_range&)
            {
                throw runtime_error("Double value out of range at line " + to_string(startLine) + " column " + to_string(startColumn));
            }
        }
        else
        {
            try
            {
                int value = stoi(numberStr);
                return Token{ TokenType::IntConst, value, startLine, startColumn, start };
            }
            catch (const out_of_range&)
            {
                throw runtime_error("Integer value out of range at line " + to_string(startLine) + " column " + to_string(startColumn));
            }
        }
    }
  

    Token readIdentifierOrKeyword()
    {
        size_t start = currentPosition;
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        string identifier;

        while (isalnum(currentChar()) || currentChar() == '_')
        {
            identifier += currentChar();
            if (identifier.length() > maxIdentifierLength)
            {
                throw runtime_error("Identifier too long (more than " + to_string(maxIdentifierLength) + " characters) at line " + to_string(startLine) + " column " + to_string(startColumn));
            }
            advance();
        }

        // Sprawdzenie, czy utworzony identyfikator jest s³owem kluczowym (lub null, true, false)
        if (identifier == "null") return Token{ TokenType::NullConst, {}, startLine, startColumn, start };
        if (identifier == "true" || identifier == "false") return Token{ TokenType::BoolConst, identifier == "true", startLine, startColumn, start };
        if (auto it = keywordMap.find(identifier); it != keywordMap.end())
        {
            // Jeœli tak, zwróæ id tokenu reprezentuj¹cego s³owo kluczowe
            return Token{ TokenType::Keyword, static_cast<int>(it->second), startLine, startColumn, start };
        }
        else
        {
            // W przeciwnym razie zwróæ token reprezentuj¹cy identyfikator
            return Token{ TokenType::Id, identifier, startLine, startColumn, start };
        }
    }


    Token readString()
    {
        string value;
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        size_t start = currentPosition;
        advance(); // Pomijamy pocz¹tkowy cudzys³ów

        while (currentChar() != '"' && currentChar() != ETX_CHAR)
        {
            if (currentChar() == '\\')
            { // Obs³uga sekwencji ucieczki
                advance(); // Przesuwamy siê, aby uzyskaæ nastêpny znak po '\'
                switch (currentChar())
                {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default: value += '\\'; value += currentChar();// Nieznana sekwencja ucieczki
                }
            }
            else value += currentChar();
            advance();

            if (value.length() > maxStringLength)
            {
                throw runtime_error("String too long (more than " + to_string(maxStringLength) + " characters) at line " + to_string(startLine) + " column " + to_string(startColumn));
            }
        }

        if (currentChar() == ETX_CHAR)
        {
            throw runtime_error("String not closed before end of file at line " + to_string(startLine) + " column " + to_string(startColumn));
        }

        advance(); // Pomijamy koñcowy cudzys³ów

        return Token{ TokenType::StringConst, value, startLine, startColumn, start };
    }


    Token readComment()
    {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        size_t start = currentPosition;
        // Pomijamy znak rozpoczynaj¹cy komentarz, tutaj '#'
        advance();

        string commentContent;
        while (currentChar() != '\n' && currentChar() != ETX_CHAR)
        {
            commentContent += currentChar();
            advance();
        }

        // Nie pomijamy znaku nowej linii ('\n') na koñcu komentarza, 
        // poniewa¿ jego obecnoœæ mo¿e byæ istotna dla innych elementów leksera,
        // takich jak zliczanie linii.

        return Token{ TokenType::Comment, commentContent, startLine, startColumn, start };
    }

    Token readSymbol()
    {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        size_t start = currentPosition;
        string potentialSymbol;
        string longestMatch;

        for (int lookAhead = 0; lookAhead < 2; ++lookAhead)
        { // Symbol ma maksymalnie 2 znaki
            if (currentPosition + lookAhead < source.size())
            {
                potentialSymbol += source[currentPosition + lookAhead];
                // Sprawdzanie, czy bie¿¹cy potencjalny symbol pasuje
                if (symbols.find(potentialSymbol) != symbols.end()) longestMatch = potentialSymbol; // Znaleziono nowy najd³u¿szy pasuj¹cy 
            }
        }

        if (!longestMatch.empty())
        {
            currentPosition += longestMatch.length(); // Aktualizacja pozycji o d³ugoœæ znalezionego symbolu
            currentColumn += longestMatch.length(); // Aktualizacja kolumny o d³ugoœæ znalezionego symbolu
            int symbolId = static_cast<int>(symbols[longestMatch]); // Pobranie ID symbolu
            return Token{ TokenType::Symbol, symbolId, startLine, startColumn, start };
        }

        // Gdy nie znajdziemy pasuj¹cego symbolu, rzuæ wyj¹tek
        throw runtime_error("Unknown symbol at line " + to_string(startLine) + " column " + to_string(startColumn));
    }

    Token readWhitespace()
    {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        size_t start = currentPosition;

        while (isspace(currentChar())) advance(); // Przesuwamy siê do kolejnego znaku
        return Token{ TokenType::Whitespaces, " ", startLine, startColumn, start };
    }


public:

    Lexer(size_t maxIdentifierLength = 64, unsigned long long maxStringLength = 1024) : maxIdentifierLength(maxIdentifierLength), maxStringLength(maxStringLength) {}
    size_t getMaxIdentifierLength() { return maxIdentifierLength;  }
    size_t getMaxStringLength() { return maxStringLength; }
    void loadFromString(const string& source) { setSource(source); } //Obsluga zrodla: ciag znakow
    void loadFromFile(const string& filePath) //Obsluga zrodla: plik
    {
        ifstream file(filePath);
        if (!file) throw runtime_error("Nieudany odczyt z pliku: " + filePath);
        stringstream buffer;
        buffer << file.rdbuf();
        setSource(buffer.str());
    }

    Token nextToken()
    {
        while (currentChar() != ETX_CHAR)
        {
            if (isspace(currentChar())) return readWhitespace();
            if (isdigit(currentChar())) return readNumber();
            if (isalpha(currentChar()) || currentChar() == '_') return readIdentifierOrKeyword();
            if (currentChar() == '"') return readString();
            if (currentChar() == '#') return readComment();
            return readSymbol();
        }
        return Token{ TokenType::ETX, " "}; // Zakoñczenie pliku
    }
};

