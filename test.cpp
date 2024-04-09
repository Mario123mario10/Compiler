#include "pch.h"
#include "../Lekser/Lekser.hpp"

TEST(LexerTests, TestInteger) {
    Lexer lexer;
    lexer.loadFromString("12345");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::IntConst);
    EXPECT_EQ(get<int>(token.value), 12345);
}


TEST(LexerTests, TestDouble) {
    Lexer lexer;
    lexer.loadFromString("123.45");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::DoubleConst);
    EXPECT_DOUBLE_EQ(get<double>(token.value), 123.45);
}

TEST(LexerTests, TestBool) {
    Lexer lexer;
    lexer.loadFromString("true");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::BoolConst);
    EXPECT_EQ(get<int>(token.value), 1); // pobieramy ze struktury tokenu warto�� tokenu jako typ int
}

TEST(LexerTests, TestNull) {
    Lexer lexer;
    lexer.loadFromString("null");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::NullConst);
}

TEST(LexerTests, TestString) {
    Lexer lexer;
    lexer.loadFromString("\"Hello, world!\"");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::StringConst);
    EXPECT_EQ(get<string>(token.value), "Hello, world!");
}

TEST(LexerTests, TestEscapedString) {
    Lexer lexer;
    lexer.loadFromString("\"Hello, \\\"world\\\"!\"");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::StringConst);
    EXPECT_EQ(get<string>(token.value), "Hello, \"world\"!");
}

TEST(LexerTests, TestComment) {
    Lexer lexer;
    lexer.loadFromString("# This is a comment\n");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Comment);
    EXPECT_EQ(get<string>(token.value), " This is a comment");
}

TEST(LexerTests, TestIdentifier) {
    Lexer lexer;
    lexer.loadFromString("variableName");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Id);
    EXPECT_EQ(get<string>(token.value), "variableName");
}

TEST(LexerTests, TestWhitespaces) {
    Lexer lexer;
    lexer.loadFromString("   \n\t   \n\n\n");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Whitespaces);
}

TEST(LexerTests, TestETX) {
    Lexer lexer;
    lexer.loadFromString("");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::ETX);
}

TEST(LexerTests, TestWhitespacesETX) {
    Lexer lexer;
    lexer.loadFromString("   \n\t   \n\n\n");
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Whitespaces);
    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::ETX);
}

//Testy parametryczne dla kewyordow

struct KeywordTestParam
{
    string keyword;
    KW expectedType;
};

class LexerKeywordTest : public ::testing::TestWithParam<KeywordTestParam>
{
protected:
    Lexer lexer;
};

TEST_P(LexerKeywordTest, RecognizesKeywordsCorrectly) {
    auto param = GetParam();
    lexer.loadFromString(param.keyword);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Keyword);
    EXPECT_EQ(get<int>(token.value), static_cast<int>(param.expectedType));
}

INSTANTIATE_TEST_CASE_P(Default, LexerKeywordTest,
    ::testing::Values(
        KeywordTestParam{ "def", KW::DEF },
        KeywordTestParam{ "if", KW::IF },
        KeywordTestParam{ "else", KW::ELSE },
        KeywordTestParam{ "while", KW::WHILE },
        KeywordTestParam{ "break", KW::BREAK },
        KeywordTestParam{ "continue", KW::CONTINUE },
        KeywordTestParam{ "return", KW::RETURN },
        KeywordTestParam{ "select", KW::SELECT },
        KeywordTestParam{ "from", KW::FROM },
        KeywordTestParam{ "where", KW::WHERE },
        KeywordTestParam{ "order", KW::ORDER },
        KeywordTestParam{ "by", KW::BY },
        KeywordTestParam{ "desc", KW::DESC },
        KeywordTestParam{ "limit", KW::LIMIT }
));

//Testy parametryczne dla symboli

struct SymbolTestParam
{
    string symbol;
    SYM expectedType;
};

class LexerSymbolTest : public ::testing::TestWithParam<SymbolTestParam>
{
protected:
    Lexer lexer;
};

TEST_P(LexerSymbolTest, RecognizesSymbolsCorrectly)
{
    auto param = GetParam();
    lexer.loadFromString(param.symbol);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Symbol);
    EXPECT_EQ(get<int>(token.value), static_cast<int>(param.expectedType));
}

INSTANTIATE_TEST_CASE_P(Default, LexerSymbolTest,
    ::testing::Values(
        SymbolTestParam{ "+", SYM::PLUS },
        SymbolTestParam{ "-", SYM::MINUS },
        SymbolTestParam{ "*", SYM::MULTIPLY },
        SymbolTestParam{ "/", SYM::DIVIDE },
        SymbolTestParam{ "^", SYM::POWER },
        SymbolTestParam{ "==", SYM::EQUALS },
        SymbolTestParam{ "!=", SYM::NOT_EQUAL },
        SymbolTestParam{ ">", SYM::GREATER_THAN },
        SymbolTestParam{ ">=", SYM::GREATER_EQUAL },
        SymbolTestParam{ "<", SYM::LESS_THAN },
        SymbolTestParam{ "<=", SYM::LESS_EQUAL },
        SymbolTestParam{ "&&", SYM::AND },
        SymbolTestParam{ "||", SYM::OR },
        SymbolTestParam{ "!", SYM::NOT },
        SymbolTestParam{ "(", SYM::LEFT_PAR },
        SymbolTestParam{ ")", SYM::RIGH_PAR },
        SymbolTestParam{ "{", SYM::LEFT_BRACE },
        SymbolTestParam{ "}", SYM::RIGHT_BRACE },
        SymbolTestParam{ "[", SYM::LEFT_BRACKET },
        SymbolTestParam{ "]", SYM::RIGHT_BRACKET },
        SymbolTestParam{ "=", SYM::ASSIGN },
        SymbolTestParam{ ",", SYM::COMMA },
        SymbolTestParam{ ":", SYM::COLON },
        SymbolTestParam{ ".", SYM::DOT }
));

//Testy negatywne (blednych przypadkow)

TEST(LexerTests, TestUnmatchedString) {
    Lexer lexer;
    lexer.loadFromString("\"Hello, world!");
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error for unmatched string literal";
    }
    catch (runtime_error const& err) {
        EXPECT_EQ(err.what(), string("String not closed before end of file at line 1 column 1"));
    }
    catch (...) {
        FAIL() << "Expected runtime_error for unmatched string literal";
    }
}

TEST(LexerTests, TestInvalidNumberFormat) {
    Lexer lexer;
    lexer.loadFromString("123.45.67");
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error";
    }
    catch (runtime_error const& err) {
        EXPECT_EQ(err.what(), string("Invalid number format with multiple dots at line 1 column 1"));
    }
    catch (...) {
        FAIL() << "Expected runtime_error";
    }
}

TEST(LexerTests, TestUnknownSymbol) {
    Lexer lexer;
    lexer.loadFromString("@");
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error for unknown symbol";
    }
    catch (runtime_error const& err) {
        EXPECT_EQ(err.what(), string("Unknown symbol at line 1 column 1"));
    }
    catch (...) {
        FAIL() << "Expected runtime_error for unknown symbol";
    }
}

TEST(LexerTests, TestIntOverflow) {
    Lexer lexer;
    string largeNumber = "9999999999999999999999999999999999999999999"; 
    lexer.loadFromString(largeNumber);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error for number overflow";
    }
    catch (runtime_error const& err) {
        EXPECT_TRUE(string(err.what()).find("value out of range") != string::npos);
    }
    catch (...) {
        FAIL() << "Expected runtime_error for number overflow";
    }
}

TEST(LexerTests, TestDoubleOverflow) {
    Lexer lexer;
    string largeNumber = string(400, '9') + ".5";
    lexer.loadFromString(largeNumber);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error for number overflow";
    }
    catch (runtime_error const& err) {
        EXPECT_TRUE(string(err.what()).find("value out of range") != string::npos);
    }
    catch (...) {
        FAIL() << "Expected runtime_error for number overflow";
    }
}

TEST(LexerTests, TestTooLongString) {
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    Lexer lexer(maxIdentifierLength,maxStringLength);
    string longString = "\"" + string(maxStringLength + 1, 'a') + "\""; // 1025 znak�w 'a' wewn�trz cudzys�ow�w
    lexer.loadFromString(longString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error for string exceeding maximum length";
    }
    catch (runtime_error const& err) {
        EXPECT_TRUE(string(err.what()).find("String too long") != string::npos);
    }
    catch (...) {
        FAIL() << "Expected runtime_error for string exceeding maximum length";
    }
}

//Dla porownania dlugi, ale nie za dlugi string
TEST(LexerTests, TestLongString) {
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    Lexer lexer(maxIdentifierLength, maxStringLength);
    string longString = "\"" + string(maxStringLength, 'a') + "\""; // 1024 znak�w 'a' wewn�trz cudzys�ow�w
    lexer.loadFromString(longString);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::StringConst);
    EXPECT_EQ(get<string>(token.value), string(maxStringLength, 'a'));
}

TEST(LexerTests, TestTooLongIdentifier) {
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    Lexer lexer(maxIdentifierLength, maxStringLength);
    string longIdentifier = string(maxIdentifierLength + 1, 'a'); // 65 znak�w 'a', przekraczaj�cych maksymaln� d�ugo�� identyfikatora
    lexer.loadFromString(longIdentifier);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected runtime_error for identifier exceeding maximum length";
    }
    catch (runtime_error const& err) {
        EXPECT_TRUE(string(err.what()).find("Identifier too long") != string::npos);
    }
    catch (...) {
        FAIL() << "Expected runtime_error for identifier exceeding maximum length";
    }
}

//Dla porownania dlugi, ale nie za dlugi identyfikator
TEST(LexerTests, TestLongIdentifier) {
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    Lexer lexer(maxIdentifierLength, maxStringLength);
    string longIdentifier = string(maxIdentifierLength, 'a'); // 65 znak�w 'a', przekraczaj�cych maksymaln� d�ugo�� identyfikatora
    lexer.loadFromString(longIdentifier);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Id);
    EXPECT_EQ(get<string>(token.value), longIdentifier);
}

