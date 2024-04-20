#include "Lekser.h"

bool Lexer::tryBuildNumber()
{
    if (!isdigit(skaner.currentChar())) return false;

    size_t startLine = skaner.getCurrentLine();
    size_t startColumn = skaner.getCurrentColumn();

    bool isFloatingPoint = false;
    double value = 0.0;
    double valueAfterDot = 0.0;

    size_t readCharacters = 0;
    size_t afterDot = 0;

    while (isdigit(skaner.currentChar()) || (!isFloatingPoint && skaner.currentChar() == '.'))
    {
        //Jesli dotad byla kropka, a mamy juz 310 znakow, to wiecej nie zmiescimy, a jeszcze chcemy cos czytac
        //Jesli dotad nie bylo kropki, a mamy juz 309 znakow, to nie zmiescimy double'a (bo potrzebne by byly 2 dodatkowe znaki: kropka i cyfra)
        size_t characterLimit = isFloatingPoint ? maxNumberLength : maxNumberLength - 1;
        if (readCharacters >= characterLimit) throw Error(Error::NUMBER_TOO_LONG, startLine, startColumn);

        if (skaner.currentChar() == '.') isFloatingPoint = true; //Pierwsza kropka
        else if (readCharacters == 1 && value == 0.0 && skaner.currentChar() == '0') //Wykrywanie liczby typu 007
        {
            throw Error(Error::NUMBER_MULTIPLE_LEADING_ZEROS, startLine, startColumn);
        }
        else if (!isFloatingPoint) value = value * 10 + (skaner.currentChar() - '0');
        else
        {
            valueAfterDot = valueAfterDot * 10 + (skaner.currentChar() - '0');
            ++afterDot;
        }
        skaner.advance();
        ++readCharacters;
    }

    // Check for multiple consecutive dots or invalid number format
    if (skaner.currentChar() == '.') throw Error(Error::NUMBER_MULTIPLE_DOTS, startLine, startColumn);
    value += valueAfterDot / pow(10.0, afterDot); //Dodajemy czesc ulamkowa
    if (isFloatingPoint)
    {
        if (!isfinite(value)) throw Error(Error::NUMBER_OUT_OF_RANGE_DOUBLE, startLine, startColumn);
        lastToken = Token{ TokenType::DoubleConst, value, startLine, startColumn };
    }
    else
    {
        if (value > static_cast<double>(numeric_limits<int>::max())) throw Error(Error::NUMBER_OUT_OF_RANGE_INT, startLine, startColumn);
        lastToken = Token{ TokenType::IntConst, static_cast<int>(value), startLine, startColumn };
    }
    return true;
}

bool Lexer::tryBuildIdentifierOrKeyword()
{
    if (!isalpha(skaner.currentChar()) && skaner.currentChar() != '_') return false;

    size_t startLine = skaner.getCurrentLine();
    size_t startColumn = skaner.getCurrentColumn();
    string identifier;

    while (isalnum(skaner.currentChar()) || skaner.currentChar() == '_')
    {
        if (identifier.length() >= maxIdentifierLength) throw Error(Error::IDENTIFIER_TOO_LONG, startLine, startColumn);
        identifier += skaner.currentChar();
        skaner.advance();
    }

    // Sprawdzenie, czy utworzony identyfikator jest s³owem kluczowym (lub true, false)
    if (identifier == "true") lastToken = Token{ TokenType::TrueConst, {}, startLine, startColumn };
    else if (identifier == "false") lastToken = Token{ TokenType::FalseConst, {}, startLine, startColumn };
    else if (auto it = keywordMap.find(identifier); it != keywordMap.end())
    {
        // Jeœli tak, zwróæ id tokenu reprezentuj¹cego s³owo kluczowe
        lastToken = Token{ TokenType::Keyword, static_cast<int>(it->second), startLine, startColumn };
    }
    else
    {
        // W przeciwnym razie zwróæ token reprezentuj¹cy identyfikator
        lastToken = Token{ TokenType::Id, identifier, startLine, startColumn };
    }
    return true;
}

bool Lexer::tryBuildString()
{
    if (skaner.currentChar() != '"') return false;

    string value;
    size_t startLine = skaner.getCurrentLine();
    size_t startColumn = skaner.getCurrentColumn();

    skaner.advance(); // Pomijamy pocz¹tkowy cudzys³ów

    while (skaner.currentChar() != '"' && skaner.currentChar() != ETX_CHAR)
    {
        if (value.length() >= maxStringLength) throw Error(Error::STRING_TOO_LONG, startLine, startColumn);

        if (skaner.currentChar() == '\\')
        { // Obs³uga sekwencji ucieczki
            skaner.advance(); // Przesuwamy siê, aby uzyskaæ nastêpny znak po '\'
            switch (skaner.currentChar())
            {
            case 'n': value += '\n'; break;
            case 't': value += '\t'; break;
            case '\\': value += '\\'; break;
            case '"': value += '"'; break;
            default: value += skaner.currentChar(); // Traktujemy doslownie nastpeny znak
            }
        }
        else value += skaner.currentChar();
        skaner.advance();
    }

    if (skaner.currentChar() == ETX_CHAR) throw Error(Error::STRING_INFINITE, startLine, startColumn);

    skaner.advance(); // Pomijamy koñcowy cudzys³ów
    lastToken = Token{ TokenType::StringConst, value, startLine, startColumn };
    return true;
}

bool Lexer::tryBuildComment()
{
    if (skaner.currentChar() != '#') return false;

    size_t startLine = skaner.getCurrentLine();
    size_t startColumn = skaner.getCurrentColumn();

    // Pomijamy znak rozpoczynaj¹cy komentarz, tutaj '#'
    skaner.advance();

    string commentContent;
    while (skaner.currentChar() != '\n' && skaner.currentChar() != ETX_CHAR)
    {
        commentContent += skaner.currentChar();
        skaner.advance();
    }

    lastToken = Token{ TokenType::Comment, commentContent, startLine, startColumn };
    return true;
}

bool Lexer::tryBuildSymbol()
{
    size_t startLine = skaner.getCurrentLine();
    size_t startColumn = skaner.getCurrentColumn();

    string longestMatch;
    string shortSymbol(1, skaner.currentChar());

    if (symbols.count(shortSymbol)) longestMatch = shortSymbol;
    else if (!symbolFirsts.count(shortSymbol)) return false; //Pierwszy znak nie jest poczatkiem symbolu
    //Pierwszy znak jest poczatkiem symbolu, wiec przynajmniej 1 znak pasuje

    skaner.advance(); //Jestemy ustawieni za 1 znakiem symbolu (na drugim znaku symbolu lub na czyms kolejnym)
    string longSymbol = shortSymbol + skaner.currentChar();
    if (symbols.count(longSymbol))
    {
        longestMatch = longSymbol;
        skaner.advance(); //Symbol jest 2-znakowy, wiec przesuwamy sie o dodatkowa pozycje
    }

    if (longestMatch.empty()) return false; // Symbol mia³ potencja³ byæ 2-znakowy ale okaza³ siê nim nie byæ np. &*

    int symbolId = static_cast<int>(symbols[longestMatch]); // Pobranie ID symbolu
    lastToken = Token{ TokenType::Symbol, symbolId, startLine, startColumn };
    return true;
}

void Lexer::readWhitespaces() { while (isspace(skaner.currentChar())) skaner.advance(); } // Dopoki biezacy znak jest bialy, przesuwamy sie do kolejnego

Token Lexer::nextToken()
{
    readWhitespaces();
    if (skaner.currentChar() == ETX_CHAR) return Token{ TokenType::ETX }; // Zakoñczenie tekstu

    size_t startLine = skaner.getCurrentLine();
    size_t startColumn = skaner.getCurrentColumn();
    try
    {
        if (tryBuildNumber() || tryBuildIdentifierOrKeyword() || tryBuildString() || tryBuildComment() || tryBuildSymbol()) return lastToken;
    }
    catch (const Error& E) { throw E; }

    // Jesli zaden token nie pasujem, rzucamy wyjatek
    throw Error(Error::UNKNOWN_TOKEN, startLine, startColumn); // np. &* albo %
}