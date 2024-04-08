#include <iostream>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <limits>
#include <algorithm>

const char ETX_CHAR = '\x03'; //End of Text

enum class TokenType {
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
};

using TokenValue = std::variant<int, double, std::string, std::monostate>; // Dodane std::monostate dla tokenów bez wartoœci

struct Token {
    TokenType type;
    TokenValue value;
    size_t lineNumber;
    size_t columnNumber;
    size_t absoluteIndex;
};

class Lexer
{
private:
    std::string source_;

    size_t currentPosition = 0;
    size_t currentLine = 1;
    size_t currentColumn = 1;

    const size_t maxIdentifierLength = 64;
    const size_t maxStringLength = 1024;

    std::map<std::string, int> keywordMap = {
                {"def", 0},
                {"if", 1},
                {"else", 2},
                {"while", 3},
                {"break", 4},
                {"continue", 5},
                {"return", 6},
                {"select", 7},
                {"from", 8},
                {"where", 9},
                {"order", 10},
                {"by", 11},
                {"desc", 12},
                {"limit", 13},
    };

    // Mapa wszystkich symboli z ich unikalnymi numerami identyfikacyjnymi
    std::map<std::string, int> symbols = {
        {"+", 1}, {"-", 2},
        {"*", 3}, {"/", 4},
        {"^", 5},
        {"==", 6}, {"!=", 7}, {">", 8}, {">=", 9}, {"<", 10}, {"<=", 11},
        {"&&", 12}, {"||", 13}, {"!", 14},
        {"(", 15}, {")", 16},
        {"{", 17}, {"}", 18},
        {"[", 19}, {"]", 20},
        {"=",21},
        {",",22},{":",23},{".",24}
    };

    void setSource(const std::string& source)
    {
        source_ = source + ETX_CHAR; //Straznik na koncu, dzieki ktoremu wykryjemy nieprawidlowy token na samym koñcu
    }

    char currentChar() {
        return currentPosition < source_.length() ? source_[currentPosition] : ETX_CHAR;
    }

    void advance() {
        if (currentChar() == '\n') {
            currentLine++;
            currentColumn = 0;
        }
        currentPosition++;
        currentColumn++;
    }

    Token readNumber() {
        size_t start = currentPosition;
        while (isdigit(currentChar()) || currentChar() == '.') {
            advance();
        }

        // Ekstrakcja tekstu reprezentuj¹cego liczbê
        std::string numberStr = source_.substr(start, currentPosition - start);

        // Sprawdzenie, czy liczba zawiera kropkê, co wskazywa³oby na liczbê zmiennoprzecinkow¹
        if (numberStr.find('.') != std::string::npos) {
            try {
                double value = std::stod(numberStr);
                return Token{ TokenType::DoubleConst, value, currentLine, currentColumn - (currentPosition - start) };
            }
            catch (const std::out_of_range&) {
                throw std::runtime_error("Double value out of range at line: " + std::to_string(currentLine) + " column: " + std::to_string(currentColumn));
            }
        }
        else {
            try {
                int value = std::stoi(numberStr);
                return Token{ TokenType::IntConst, value, currentLine, currentColumn - (currentPosition - start) };
            }
            catch (const std::out_of_range&) {
                throw std::runtime_error("Integer value out of range at line: " + std::to_string(currentLine) + " column: " + std::to_string(currentColumn));
            }
        }
    }

    Token readIdentifierOrKeyword() {
        size_t start = currentPosition;
        while (isalnum(currentChar()) || currentChar() == '_') {
            advance();
        }

        std::string identifier = source_.substr(start, currentPosition - start);

        // Sprawdzenie, czy utworzony identyfikator jest s³owem kluczowym
        if (keywordMap.find(identifier) != keywordMap.end()) {
            // Jeœli tak, zwróæ token reprezentuj¹cy s³owo kluczowe
            return Token{ TokenType::Keyword, identifier, currentLine, currentColumn - (currentPosition - start) };
        }
        else {
            // W przeciwnym razie zwróæ token reprezentuj¹cy identyfikator
            return Token{ TokenType::Id, identifier, currentLine, currentColumn - (currentPosition - start) };
        }
    }


    Token readString() {
        advance(); // Pomijamy pocz¹tkowy cudzys³ów
        std::string value;
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;

        while (currentChar() != '"' && currentChar() != ETX_CHAR) {
            if (currentChar() == '\\') { // Obs³uga sekwencji ucieczki
                advance(); // Przesuwamy siê, aby uzyskaæ nastêpny znak po '\'
                switch (currentChar()) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default: value += '\\'; value += currentChar(); // Nieznana sekwencja ucieczki
                }
            }
            else {
                value += currentChar();
            }
            advance();
        }

        if (currentChar() == ETX_CHAR) {
            throw std::runtime_error("String not closed before end of file at line " + std::to_string(startLine) + " column " + std::to_string(startColumn));
        }

        advance(); // Pomijamy koñcowy cudzys³ów

        return Token{ TokenType::StringConst, value, startLine, startColumn };
    }


    Token readComment() {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;
        // Pomijamy znak rozpoczynaj¹cy komentarz, tutaj '#'
        advance();

        std::string commentContent;
        while (currentChar() != '\n' && currentChar() != ETX_CHAR) {
            commentContent += currentChar();
            advance();
        }

        // Nie pomijamy znaku nowej linii ('\n') na koñcu komentarza, 
        // poniewa¿ jego obecnoœæ mo¿e byæ istotna dla innych elementów leksera,
        // takich jak zliczanie linii.

        return Token{ TokenType::Comment, commentContent, startLine, startColumn };
    }

    Token readSymbol() {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;

        std::string potentialSymbol(1, currentChar());
        std::string longestMatch = potentialSymbol;

        // Sprawdzanie d³u¿szych symboli z³o¿onych
        advance(); // Przesuwamy siê do przodu, aby sprawdziæ potencjalne z³o¿one symbole
        if (!symbols.count(potentialSymbol)) {
            throw std::runtime_error("Unknown symbol at line " + std::to_string(currentLine) + " column " + std::to_string(currentColumn));
        }

        int symbolId = symbols[potentialSymbol]; // Zak³adamy, ¿e ka¿dy symbol ma unikalne ID.

        // Jeœli obecny znak mo¿e tworzyæ z³o¿ony symbol z poprzednim, sprawdŸ to
        potentialSymbol += currentChar();
        if (symbols.count(potentialSymbol)) {
            longestMatch = potentialSymbol; // Znaleziono d³u¿szy, pasuj¹cy symbol
            symbolId = symbols[longestMatch]; // Aktualizujemy ID na nowo znaleziony, d³u¿szy symbol
            advance(); // Konsumujemy dodatkowy znak, który jest czêœci¹ z³o¿onego symbolu
        }

        return Token{ TokenType::Symbol, longestMatch, startLine, startColumn };
    }


    Token readWhitespace() {
        size_t startLine = currentLine;
        size_t startColumn = currentColumn;

        while (isspace(currentChar())) {
            // Przesuwamy siê do kolejnego znaku
            advance();
        }
        return Token{ TokenType::Whitespaces, " ", startLine, startColumn};
    }


public:
    
    Lexer(size_t maxIdentifierLength = 64, size_t maxStringLength = 1024) : maxIdentifierLength(maxIdentifierLength), maxStringLength(maxStringLength) {}
    void loadFromString(const std::string& source) { setSource(source); } //Obsluga zrodla: ciag znakow
    void loadFromFile(const std::string& filePath) //Obsluga zrodla: plik
    {
        std::ifstream file(filePath);
        if (!file) throw std::runtime_error("Nieudany odczyt z pliku: " + filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        setSource(buffer.str());
    }

    std::optional<Token> nextToken()
    {
        while (currentChar() != ETX_CHAR) {
            if (isspace(currentChar())) {
                return readWhitespace();
            }
            else if (isdigit(currentChar())) {
                return readNumber();
            }
            else if (isalpha(currentChar()) || currentChar() == '_') {
                return readIdentifierOrKeyword();
            }
            else if (currentChar() == '"') {
                return readString();
            }
            else if (currentChar() == '#') {
                return readComment();
            }
            else {
                return readSymbol();
            }
        }
        return std::nullopt; // Zakoñczenie pliku
    }
};

int main() {
    std::string sourceCode = R"(
        def factorial(n) {
            if (n == 0) {
                return 1
            } else {
                return n * factorial(n - 1)
            }
        }

        print("Factorial of 5 is ", factorial(5))
        "ala ma \"kota\"\naaaaaa" "basia ma helikopter"
        #x = 888888888333333333333333333333333333333333333333333333333333333333
x=3y=7
x=3y=7.222 .2
        
    )";

    Lexer lexer(10,20);
    lexer.loadFromString(sourceCode);

    try
    {
        //lexer.loadFromFile("kod.txt");
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
        
    std::vector<Token> tokens;
    try
    {
        while (auto tokenOpt = lexer.nextToken())
        {
            auto token = tokenOpt.value();
            tokens.push_back(token);
        }
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "Blad podczas tokenizacji: " << error.what() << std::endl;
        return 1;
    }
    
 
    std::cout << "Tokens:\n";
    std::cout << std::left << std::setw(20) << "Type" << std::setw(10) << "Value" << "Position (Line:Column)\n";
    std::cout << std::string(60, '-') << '\n';

    for (const auto& token : tokens) {
        std::string tokenType;
        switch (token.type) {
        case TokenType::IntConst: tokenType = "IntConst"; break;
        case TokenType::DoubleConst: tokenType = "DoubleConst"; break;
        case TokenType::BoolConst: tokenType = "BoolConst"; break;
        case TokenType::NullConst: tokenType = "NullConst"; break;
        case TokenType::StringConst: tokenType = "StringConst"; break;
        case TokenType::Id: tokenType = "Id"; break;
        case TokenType::Keyword: tokenType = "Keyword"; break;
        case TokenType::Symbol: tokenType = "Symbol"; break;
        case TokenType::Comment: tokenType = "Comment"; break;
        default: tokenType = "Whitespaces";
        }

        std::string value = std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            }
            else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                return std::to_string(arg);
            }
            else {
                return "n/a";
            }
            }, token.value);

        std::cout << std::setw(20) << tokenType << std::setw(20) << value << token.lineNumber << ":" << token.columnNumber << '\n';
    }

    return 0;
}