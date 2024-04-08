#include <iostream>
#include <iomanip> 
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <limits>
#include <algorithm>

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
    Whitespaces
};

using TokenValue = std::variant<int, double, std::string>;

struct Token {
    TokenType type;
    TokenValue value;
    int lineNumber;
    int columnNumber;
    int absoluteIndex;
};

class Lexer {
public:
    Lexer(const std::string& source) : source_(source) 
    {
        const std::string combinedPattern = numConstPattern + "|" + boolConstPattern + "|" + nullConstPattern + "|" +
            stringConstPattern + "|" + idPattern + "|" + symbolPattern + "|" + commentPattern + "|" + whitespacePattern;
        tokenRegex = std::regex(combinedPattern, std::regex_constants::ECMAScript);
    }

    std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;
        int currentLine = 1;
        int currentColumn = 1;
        size_t lastLineStart = 0;
        size_t nextTokenStart = 0;
        size_t processedLength = 0;


        auto words_begin = std::sregex_iterator(source_.begin(), source_.end(), tokenRegex);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
        {
            std::smatch match = *i;
            std::string matchStr = match.str();
            size_t matchStart = match.position();
            size_t matchLength = match.length();

            //Wykrywanie nieznancyh tokenow - na przyklad: "ala ma kota
            //To nie jest string_const, bo nie konczy sie cudzyslowem. Nieznany token zaczyna sie (i w tym przypadku konczy) na pozycji znaku '"'
            //Bo potem mamy juz ala, ktore mozemy rozpoznac jako id

            if (matchStart > nextTokenStart)
            {
                size_t gapStart = nextTokenStart;
                size_t gapEnd = matchStart;
                // Znalezienie linii i kolumny dla pocz¹tku luki
                std::string gapText = source_.substr(gapStart, gapEnd - gapStart);
                auto gapLine = currentLine + std::count(source_.begin() + processedLength, source_.begin() + gapStart, '\n');
                auto lineStartPos = source_.rfind('\n', gapStart - 1);
                auto gapColumn = gapStart - (lineStartPos == std::string::npos ? 0 : lineStartPos);

                throw std::runtime_error("Nieprawidlowy token na pozycji " + std::to_string(gapLine) + ":" + std::to_string(gapColumn));
            }

            processedLength += matchLength;
            nextTokenStart = matchStart + matchLength;
            
            try
            {
                auto [type, value] = getTokenTypeAndValue(matchStr); // Rozpakowanie wyniku

                // Oblicz numer linii dla aktualnego tokenu
                if (type == TokenType::Whitespaces)
                {
                    size_t newLines = std::count(source_.begin() + matchStart, source_.begin() + nextTokenStart, '\n');
                    currentLine += newLines;
                }

                // ZnajdŸ pocz¹tek ostatniej linii przed tokenem
                size_t lineStart = source_.rfind('\n', matchStart);
                if (lineStart == std::string::npos) lineStart = 0; // Jeœli nie znaleziono, oznacza to, ¿e jesteœmy w pierwszej linii
                else lineStart += 1; // Przesuwamy siê za znak nowej 
                lastLineStart = lineStart;

                // Oblicz numer kolumny dla aktualnego tokenu
                currentColumn = matchStart - lineStart + 1; // +1, bo kolumny numerujemy od 1

                if (type != TokenType::Whitespaces) tokens.push_back(Token{ type, value, currentLine, currentColumn, static_cast<int>(matchStart) });
            }
            catch (const std::runtime_error& e) {
                throw std::runtime_error(e.what() + std::string(" na pozycji ") + std::to_string(currentLine) + ":" + std::to_string(currentColumn));
            }
            
        }

        // Sprawdzenie, czy na koñcu jest nieprzetworzony fragment tekstu
        if (nextTokenStart < source_.length())
        {
            size_t gapStart = nextTokenStart;
            size_t gapEnd = source_.length();
            // Znalezienie linii i kolumny dla pocz¹tku luki
            std::string gapText = source_.substr(gapStart, gapEnd - gapStart);
            auto gapLine = currentLine + std::count(source_.begin() + processedLength, source_.begin() + gapStart, '\n');
            auto lineStartPos = source_.rfind('\n', gapStart - 1);
            auto gapColumn = gapStart - (lineStartPos == std::string::npos ? 0 : lineStartPos);

            throw std::runtime_error("Nieprawidlowy token na pozycji " + std::to_string(gapLine) + ":" + std::to_string(gapColumn));
        }

        return tokens;
    }



private:
    std::string source_;
    std::regex tokenRegex;
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
        {",",22},{":",23},{";",24}
    };

    // Definicje wzorcow do pattern matchingu
    const std::string numConstPattern = R"(0|[1-9][0-9]*(\.[0-9]+)?)";
    const std::string boolConstPattern = R"(true|false)";
    const std::string nullConstPattern = R"(null)";
    const std::string stringConstPattern = R"("[^"\\]*(\\.[^"\\]*)*")";
    const std::string idPattern = R"([A-Za-z_][A-Za-z_0-9]*)";
    const std::string symbolPattern = R"(=|==|!=|>=|<=|<|>|\|\||&&|!|\+|\-|\*|\/|\^|\(|\)|\{|\}|\[|\]|,|\:|;)";
    const std::string commentPattern = R"(#[^\n]*)";
    const std::string whitespacePattern = R"(\s+)";

    std::pair<TokenType, TokenValue> getTokenTypeAndValue(const std::string& token) {
        // S³owa kluczowe
        if (auto kw = keywordMap.find(token); kw != keywordMap.end()) return { TokenType::Keyword, kw->second };
        // Symbole
        if (auto sm = symbols.find(token); sm != symbols.end()) return { TokenType::Symbol, sm->second }; // Zwracamy typ Symbol i numer 
        // Numeryczne sta³e
        if (std::regex_match(token, std::regex(numConstPattern))) {
            try {
                if (token.find('.') != std::string::npos) {
                    // Próba konwersji na double
                    return { TokenType::DoubleConst, std::stod(token) };
                }
                else {
                    // Próba konwersji na int
                    long long tempValue = std::stoll(token); // U¿ywam std::stoll aby obs³u¿yæ wiêkszy zakres
                    if (tempValue < std::numeric_limits<int>::min() || tempValue > std::numeric_limits<int>::max()) {
                        throw std::out_of_range("Wartoœæ int poza zakresem");
                    }
                    return { TokenType::IntConst, static_cast<int>(tempValue) };
                }
            }
            catch (const std::out_of_range& e) {
                // Obs³uga sytuacji, gdy liczba jest poza zakresem
                throw std::runtime_error("Liczba poza zakresem w tokenie: " + token);
            }
            catch (const std::invalid_argument& e) {
                // Obs³uga sytuacji, gdy argument nie jest poprawn¹ liczb¹
                throw std::runtime_error("Nieprawid³owy argument dla liczby w tokenie: " + token);
            }
        }

        // Stringi
        if (token[0] == '"') return { TokenType::StringConst, token.substr(1, token.size() - 2) };
        // Komentarze
        if (token[0] == '#') return { TokenType::Comment, {} };
        //BoolConst
        if (std::regex_match(token, std::regex(boolConstPattern))) return { TokenType::BoolConst, token=="true"}; //0 = false, 1 = true
        //NullConst
        if (token == "null") return { TokenType::NullConst, {} }; //0 = false, 1 = true
        // Identyfikatory
        if (std::regex_match(token, std::regex(idPattern))) return { TokenType::Id, token };
        // W przeciwnym razie, skoro sprawdzamy wykryty token, to sa to biale znaki
        return { TokenType::Whitespaces, token };
    }

};

int main() {
    std::string sourceCode = R"(
        def factorial(n) {
            if (n == 0) {
                return 1;
            } else {
                return n * factorial(n - 1);
            }
        }
        print("Factorial of 5 is ", factorial(5));
        "ala ma kota"
        x = 88888888888888888888888
    )";

    Lexer lexer(sourceCode);
    std::vector<Token> tokens;
    try
    {
        tokens = lexer.tokenize();
        std::cout << "Tokenizacja zakonczona sukcesem.\n";
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
        default: tokenType = "Unknown";
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

        std::cout << std::setw(20) << tokenType << std::setw(10) << value << token.lineNumber << ":" << token.columnNumber << '\n';
    }

    return 0;
}