#include "pch.h"
#include "../Lekser/Lekser.h"

TEST(LexerTests, TestInteger)
{    
    istringstream sourceCode("12345");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::IntConst);
    EXPECT_EQ(get<int>(token.value), 12345);
}

TEST(LexerTests, TestDouble)
{    
    istringstream sourceCode("123.45");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::DoubleConst);
    EXPECT_DOUBLE_EQ(get<double>(token.value), 123.45);
}

TEST(LexerTests, TestTrue)
{    
    istringstream sourceCode("true");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::TrueConst);
}

TEST(LexerTests, TestFalse)
{
    istringstream sourceCode("false");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::FalseConst);
}

TEST(LexerTests, TestString)
{    
    istringstream sourceCode("\"Hello, world!\"");    
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::StringConst);
    EXPECT_EQ(get<string>(token.value), "Hello, world!");
}

TEST(LexerTests, TestEscapedString)
{    
    istringstream sourceCode("\"Hello, \\\"world\\\"!\"");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::StringConst);
    EXPECT_EQ(get<string>(token.value), "Hello, \"world\"!");
}

TEST(LexerTests, TestComment)
{    
    istringstream sourceCode("# This is a comment\n");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Comment);
    EXPECT_EQ(get<string>(token.value), " This is a comment");
}

TEST(LexerTests, TestIdentifier)
{    
    istringstream sourceCode("variableName");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Id);
    EXPECT_EQ(get<string>(token.value), "variableName");
}

TEST(LexerTests, TestETX)
{    
    istringstream sourceCode("");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::ETX);
}

TEST(LexerTests, TestWhitespacesETX)
{    
    istringstream sourceCode("   \n\t   \n\n\n");
    Lexer lexer(sourceCode);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::ETX);
}

//Testy parametryczne dla kewyordow

struct KeywordTestParam
{
    string keyword;
    KW expectedType;
};

class LexerKeywordTest : public ::testing::TestWithParam<KeywordTestParam> {};

TEST_P(LexerKeywordTest, RecognizesKeywordsCorrectly)
{
    auto param = GetParam();
    istringstream sourceString(param.keyword);
    Lexer lexer(sourceString);
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
        KeywordTestParam{ "limit", KW::LIMIT },
        KeywordTestParam{ "null", KW::TNULL }
));


//Testy parametryczne dla symboli

struct SymbolTestParam
{
    string symbol;
    SYM expectedType;
};

class LexerSymbolTest : public ::testing::TestWithParam<SymbolTestParam> {};

TEST_P(LexerSymbolTest, RecognizesSymbolsCorrectly)
{
    auto param = GetParam();
    istringstream sourceString(param.symbol);
    Lexer lexer(sourceString);
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
        SymbolTestParam{ "=", SYM::ASSIGN },
        SymbolTestParam{ ",", SYM::COMMA },
        SymbolTestParam{ ":", SYM::COLON },
        SymbolTestParam{ ".", SYM::DOT }
));

//Testy negatywne (blednych przypadkow)

TEST(LexerTests, TestUnmatchedString)
{
    istringstream sourceString("\"Hello, world!");
    Lexer lexer(sourceString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for unmatched string literal";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::STRING_INFINITE);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for unmatched string literal";
    }
}

TEST(LexerTests, TestInvalidNumberFormat)
{
    istringstream sourceString("123.45.67");
    Lexer lexer(sourceString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for invalid number format (multiple dots)";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::NUMBER_MULTIPLE_DOTS);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for invalid number format (multiple dots)";
    }
}

TEST(LexerTests, TestUnknownToken)
{
    istringstream sourceString("@");
    Lexer lexer(sourceString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for unknown token";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::UNKNOWN_TOKEN);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for unknown token";
    }
}

TEST(LexerTests, TestIntegerOverflow) {
    string largeNumber = "9999999999999999999999999999999999999999999";
    istringstream sourceString(largeNumber);
    Lexer lexer(sourceString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for integer overflow";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::NUMBER_OUT_OF_RANGE_INT);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for integer overflow";
    }
}

TEST(LexerTests, TestNumberTooLong)
{
    size_t maxNumberLength = Lexer::getMaxNumberLength();
    string largeNumber = string(maxNumberLength - 1, '9') + ".9";
    istringstream sourceString(largeNumber);
    Lexer lexer(sourceString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for too long number";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::NUMBER_TOO_LONG);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for too long number";
    }
}

//Dla porownania liczba o maksymalnej mozliwej dlugosci

TEST(LexerTests, TestNumberLong)
{
    size_t maxNumberLength = Lexer::getMaxNumberLength();
    string largeNumber = string(maxNumberLength - 2, '9') + ".9";
    istringstream sourceString(largeNumber);
    Lexer lexer(sourceString);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::DoubleConst);
    EXPECT_DOUBLE_EQ(get<double>(token.value), stod(largeNumber));
}

TEST(LexerTests, TestLeadingZeros)
{
    istringstream sourceString("007.007");
    Lexer lexer(sourceString);
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for invalid number format (multiple leading zeros)";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::NUMBER_MULTIPLE_LEADING_ZEROS);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for invalid number format (multiple leading zeros)";
    }
}

TEST(LexerTests, TestTooLongString)
{
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    string longString = "\"" + string(maxStringLength + 1, 'a') + "\""; // 1025 znaków 'a' wewn¹trz cudzys³owów
    istringstream sourceCode(longString);
    Lexer lexer(sourceCode, maxIdentifierLength, maxStringLength);

    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for string exceeding maximum length";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::STRING_TOO_LONG);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for string exceeding maximum length";
    }
}

//Dla porownania dlugi, ale nie za dlugi string
TEST(LexerTests, TestLongString)
{
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    string longString = "\"" + string(maxStringLength, 'a') + "\""; // 1024 znaków 'a' wewn¹trz cudzys³owów
    istringstream sourceCode(longString);
    Lexer lexer(sourceCode, maxIdentifierLength, maxStringLength);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::StringConst);
    EXPECT_EQ(get<string>(token.value), string(maxStringLength, 'a'));
}

TEST(LexerTests, TestTooLongIdentifier)
{
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    string longIdentifier = string(maxIdentifierLength + 1, 'a'); // 65 znaków 'a', przekraczaj¹cych maksymaln¹ d³ugoœæ identyfikatora
    istringstream sourceCode(longIdentifier);
    Lexer lexer(sourceCode, maxIdentifierLength, maxStringLength);
    
    try {
        auto token = lexer.nextToken();
        FAIL() << "Expected Error for identifier exceeding maximum length";
    }
    catch (Error const& err) {
        EXPECT_EQ(err.getType(), Error::IDENTIFIER_TOO_LONG);
        EXPECT_EQ(err.getLine(), 1);
        EXPECT_EQ(err.getColumn(), 1);
    }
    catch (...) {
        FAIL() << "Expected Error for identifier exceeding maximum length";
    }
}

//Dla porownania dlugi, ale nie za dlugi identyfikator
TEST(LexerTests, TestLongIdentifier)
{
    size_t maxStringLength = 1024, maxIdentifierLength = 64;
    string longIdentifier = string(maxIdentifierLength, 'a'); // 65 znaków 'a', przekraczaj¹cych maksymaln¹ d³ugoœæ identyfikatora
    istringstream sourceCode(longIdentifier);
    Lexer lexer(sourceCode, maxIdentifierLength, maxStringLength);
    auto token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::Id);
    EXPECT_EQ(get<string>(token.value), longIdentifier);
}



//Z³ozone testy

TEST(LexerTests, TestFunctionsExample)
{    
    string source = R"(
def func1(x){
    if (x > 10){
        return true
    }
    return func2(x^2)
}

def func2(x){
    if (x <= 1){
        return false
    }
    return func1(x-3)
}

func1(1) # false
func1(2) # false
func1(3) # true)";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
    TokenType::Keyword,    // def
    TokenType::Id,         // func1
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // if
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // >
    TokenType::IntConst,   // 10
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::TrueConst,  // true
    TokenType::Symbol, // }
    TokenType::Keyword,    // return
    TokenType::Id,         // func2
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // ^
    TokenType::IntConst,   // 2
    TokenType::Symbol,   // )
    TokenType::Symbol, // }
    TokenType::Keyword,    // def
    TokenType::Id,         // func2
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // if
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // <=
    TokenType::IntConst,   // 1
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::FalseConst,  // false
    TokenType::Symbol, // }
    TokenType::Keyword,    // return
    TokenType::Id,         // func1
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // -
    TokenType::IntConst,   // 3
    TokenType::Symbol,   // )
    TokenType::Symbol, // }
    TokenType::Id,         // func1
    TokenType::Symbol,    // (
    TokenType::IntConst,   // 1
    TokenType::Symbol,   // )
    TokenType::Comment,    // # false
    TokenType::Id,         // func1
    TokenType::Symbol,    // (
    TokenType::IntConst,   // 2
    TokenType::Symbol,   // )
    TokenType::Comment,    // # false
    TokenType::Id,         // func1
    TokenType::Symbol,    // (
    TokenType::IntConst,   // 3
    TokenType::Symbol,   // )
    TokenType::Comment,    // # true
    TokenType::ETX,    // 
    };

    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}

TEST(LexerTests, TestWhileExample)
{
    string source = R"(
x = 10
while(x > 0){
    y = 0
    while(y < x){
        print({y})    # 0   # 0   # 0   # 0
        y = y + 8   # 8
    }
    print({x})        # 10  # 7   # 4   # 1
    x = x-3
})";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
    TokenType::Id,         // x
    TokenType::Symbol,     // =
    TokenType::IntConst,   // 10
    TokenType::Keyword,    // while
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // >
    TokenType::IntConst,   // 0
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Id,         // y
    TokenType::Symbol,     // =
    TokenType::IntConst,   // 0
    TokenType::Keyword,    // while
    TokenType::Symbol,    // (
    TokenType::Id,         // y
    TokenType::Symbol,     // <
    TokenType::Id,         // x
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Id,         // print
    TokenType::Symbol,    // (
    TokenType::Symbol,    // {
    TokenType::Id,         // y
    TokenType::Symbol,    // }
    TokenType::Symbol,   // )
    TokenType::Comment,    // # 0 #0 #0 #0
    TokenType::Id,         // y
    TokenType::Symbol,     // =
    TokenType::Id,         // y
    TokenType::Symbol,     // +
    TokenType::IntConst,   // 8
    TokenType::Comment,    // # 8
    TokenType::Symbol, // }
    TokenType::Id,         // print
    TokenType::Symbol,    // (
    TokenType::Symbol,    // {
    TokenType::Id,         // x
    TokenType::Symbol,    // }
    TokenType::Symbol,   // )
    TokenType::Comment,    // # 10 #7 #4 #1
    TokenType::Id,         // x
    TokenType::Symbol,     // =
    TokenType::Id,         // x
    TokenType::Symbol,     // -
    TokenType::IntConst,   // 3
    TokenType::Symbol, // }
    TokenType::ETX,    // 
    };

    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}

TEST(LexerTests, TestDictExample) {
    
    string source = R"(
def compare(key1, value1, key2, value2){
    return key1 + value1 < key2 + value2
}

dict1 = Dict(compare, {1 : 1, 2: 2, 3 : -1}) # kolejnoœæ w s³owniku -  1:1, 3:-1, 2:2)";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
    TokenType::Keyword,    // def
    TokenType::Id,         // compare
    TokenType::Symbol,    // (
    TokenType::Id,         // key1
    TokenType::Symbol,     // ,
    TokenType::Id,         // value1
    TokenType::Symbol,     // ,
    TokenType::Id,         // key2
    TokenType::Symbol,     // ,
    TokenType::Id,         // value2
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // key1
    TokenType::Symbol,     // +
    TokenType::Id,         // value1
    TokenType::Symbol,     // <
    TokenType::Id,         // key2
    TokenType::Symbol,     // +
    TokenType::Id,         // value2
    TokenType::Symbol, // }
    TokenType::Id,         // dict1
    TokenType::Symbol,     // =
    TokenType::Id,         // Dict
    TokenType::Symbol,    // (
    TokenType::Id,         // compare
    TokenType::Symbol,     // ,
    TokenType::Symbol,  // {
    TokenType::IntConst,   // 1
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 1
    TokenType::Symbol,     // ,
    TokenType::IntConst,   // 2
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 2
    TokenType::Symbol,     // ,
    TokenType::IntConst,   // 3
    TokenType::Symbol,     // :
    TokenType::Symbol,     // -
    TokenType::IntConst,   // 1
    TokenType::Symbol, // }
    TokenType::Symbol,    // )
    TokenType::Comment,    // #
    TokenType::ETX
    };


    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}

TEST(LexerTests, TestAccumulateExample) {
    
    string source = R"(
dict1 = Dict({"key1" : 1, "key2" : 2, "key3" : 3})

def sum(start, key, value){
    if (start != ""){
        return start + " " + key
    }
    else{
        return key
    }
}

dict1.accumulate("", sum) # "key1 key2 key3")";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
     TokenType::Id,         // dict1
     TokenType::Symbol,     // =
     TokenType::Id,         // Dict
     TokenType::Symbol,  // (
     TokenType::Symbol,  // {
     TokenType::StringConst,// “key1”
     TokenType::Symbol,     // :
     TokenType::IntConst,   // 1
     TokenType::Symbol,     // ,
     TokenType::StringConst,// “key2”
     TokenType::Symbol,     // :
     TokenType::IntConst,   // 2
     TokenType::Symbol,     // ,
     TokenType::StringConst,// “key3”
     TokenType::Symbol,     // :
     TokenType::IntConst,   // 3
     TokenType::Symbol, // }
     TokenType::Symbol, // )

     TokenType::Keyword,    // def
     TokenType::Id,         // sum
     TokenType::Symbol,    // (
     TokenType::Id,         // start
     TokenType::Symbol,     // ,
     TokenType::Id,         // key
     TokenType::Symbol,     // ,
     TokenType::Id,         // value
     TokenType::Symbol,   // )
     TokenType::Symbol,  // {
     TokenType::Keyword,    // if
     TokenType::Symbol,    // (
     TokenType::Id,         // start
     TokenType::Symbol,     // !=
     TokenType::StringConst,// ""
     TokenType::Symbol,   // )
     TokenType::Symbol,  // {
     TokenType::Keyword,    // return
     TokenType::Id,         // start
     TokenType::Symbol,     // +
     TokenType::StringConst,// " "
     TokenType::Symbol,     // +
     TokenType::Id,         // key
     TokenType::Symbol, // }
     TokenType::Keyword,    // else
     TokenType::Symbol,  // {
     TokenType::Keyword,    // return
     TokenType::Id,         // key
     TokenType::Symbol, // }
     TokenType::Symbol, // }

     TokenType::Id,         // dict1
     TokenType::Symbol,     // .
     TokenType::Id,         // accumulate
     TokenType::Symbol,    // (
     TokenType::StringConst,// ""
     TokenType::Symbol,     // ,
     TokenType::Id,         // sum
     TokenType::Symbol,    // )
     TokenType::Comment,    // #
     TokenType::ETX
    };


    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}

TEST(LexerTests, TestLINQExample) {
    
    string source = R"(
dict1 = Dict({"key1" : 1, "key2" : 2, "key3" : 3, "key4" : 4})

def order_fun(key1, value1, key2, value2){
    return value2 > value1
}

def where_fun(key, value){
    return key > "key1" && value < 4
}

x = 3
y = 2
def limit_fun(x, y){
    return x - y
}

dict2 = select{
from dict1
where where_fun
order by order_fun
desc
limit limit_fun(x, y)
}

dict2.print() # key3 : 3)";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
    TokenType::Id,         // dict1
    TokenType::Symbol,     // =
    TokenType::Id,         // Dict
    TokenType::Symbol,  // (
    TokenType::Symbol,  // {
    TokenType::StringConst,// “key1”
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 1
    TokenType::Symbol,     // ,
    TokenType::StringConst,// “key2”
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 2
    TokenType::Symbol,     // ,
    TokenType::StringConst,// "key3"
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 3
    TokenType::Symbol,     // ,
    TokenType::StringConst,// "key4"
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 4
    TokenType::Symbol, // }
    TokenType::Symbol,  // )

    TokenType::Keyword,    // def
    TokenType::Id,         // order_fun
    TokenType::Symbol,    // (
    TokenType::Id,         // key1
    TokenType::Symbol,     // ,
    TokenType::Id,         // value1
    TokenType::Symbol,     // ,
    TokenType::Id,         // key2
    TokenType::Symbol,     // ,
    TokenType::Id,         // value2
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // value2
    TokenType::Symbol,     // >
    TokenType::Id,         // value1
    TokenType::Symbol, // }

    TokenType::Keyword,    // def
    TokenType::Id,         // where_fun
    TokenType::Symbol,    // (
    TokenType::Id,         // key
    TokenType::Symbol,     // ,
    TokenType::Id,         // value
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // key
    TokenType::Symbol,     // >
    TokenType::StringConst,// "key1"
    TokenType::Symbol,     // &&
    TokenType::Id,         // value
    TokenType::Symbol,     // <
    TokenType::IntConst,   // 4
    TokenType::Symbol, // }

    TokenType::Id,         // x
    TokenType::Symbol,     // =
    TokenType::IntConst,   // 3
    TokenType::Id,         // y
    TokenType::Symbol,     // =
    TokenType::IntConst,   // 2

    TokenType::Keyword,    // def
    TokenType::Id,         // limit_fun
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // ,
    TokenType::Id,         // y
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // x
    TokenType::Symbol,     // -
    TokenType::Id,         // y
    TokenType::Symbol, // }

    TokenType::Id,         // dict2
    TokenType::Symbol,     // =
    TokenType::Keyword,   // select
    TokenType::Symbol,  // {
    TokenType::Keyword,    // from
    TokenType::Id,         // dict1
    TokenType::Keyword,    // where
    TokenType::Id,         // where_fun
    TokenType::Keyword,    // order
    TokenType::Keyword,    // by
    TokenType::Id,         // order_fun
    TokenType::Keyword,    // desc
    TokenType::Keyword,    // limit
    TokenType::Id,         // limit_fun
    TokenType::Symbol,    // (
    TokenType::Id,         // x
    TokenType::Symbol,     // ,
    TokenType::Id,         // y
    TokenType::Symbol,   // )
    TokenType::Symbol, // }
    TokenType::Id,         // dict2
    TokenType::Symbol,     // .
    TokenType::Id,         // print
    TokenType::Symbol,     // (
    TokenType::Symbol,     // )
    TokenType::Comment,     // #
    TokenType::ETX,     // 
    };

    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}

TEST(LexerTests, TestLINQ_2_Example)
{
    
    string source = R"(
def compare(key1, value1, key2, value2){
    return key1 + value1 < key2 + value2
}

dict1 = Dict(compare, {1 : 1, 2: 2, 3 : -1}) # kolejnoœæ w s³owniku -  1:1, 3:-1, 2:2

def sum(start, key, value){
    return start + value
}

dict2 = select{
from dict1
limit dict1.accumulate(0, sum)
}

dict2.print()   # 1 : 1
                # 3 : -1)";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
    TokenType::Keyword,    // def
    TokenType::Id,         // compare
    TokenType::Symbol,    // (
    TokenType::Id,         // key1
    TokenType::Symbol,     // ,
    TokenType::Id,         // value1
    TokenType::Symbol,     // ,
    TokenType::Id,         // key2
    TokenType::Symbol,     // ,
    TokenType::Id,         // value2
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // key1
    TokenType::Symbol,     // +
    TokenType::Id,         // value1
    TokenType::Symbol,     // <
    TokenType::Id,         // key2
    TokenType::Symbol,     // +
    TokenType::Id,         // value2
    TokenType::Symbol, // }

    TokenType::Id,         // dict1
    TokenType::Symbol,     // =
    TokenType::Id,         // Dict
    TokenType::Symbol,    // (
    TokenType::Id,         // compare
    TokenType::Symbol,     // ,
    TokenType::Symbol,  // {
    TokenType::IntConst,   // 1
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 1
    TokenType::Symbol,     // ,
    TokenType::IntConst,   // 2
    TokenType::Symbol,     // :
    TokenType::IntConst,   // 2
    TokenType::Symbol,     // ,
    TokenType::IntConst,   // 3
    TokenType::Symbol,     // :
    TokenType::Symbol,     // -
    TokenType::IntConst,   // 1
    TokenType::Symbol, // }
    TokenType::Symbol,   // )
    TokenType::Comment,   // #

    TokenType::Keyword,    // def
    TokenType::Id,         // sum
    TokenType::Symbol,    // (
    TokenType::Id,         // start
    TokenType::Symbol,     // ,
    TokenType::Id,         // key
    TokenType::Symbol,     // ,
    TokenType::Id,         // value
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // start
    TokenType::Symbol,     // +
    TokenType::Id,         // value
    TokenType::Symbol, // }

    TokenType::Id,         // dict2
    TokenType::Symbol,     // =
    TokenType::Keyword,     // select
    TokenType::Symbol,  // {
    TokenType::Keyword,    // from
    TokenType::Id,         // dict1
    TokenType::Keyword,    // limit
    TokenType::Id,         // dict1
    TokenType::Symbol,     // .
    TokenType::Id,         // accumulate
    TokenType::Symbol,    // (
    TokenType::IntConst,   // 0
    TokenType::Symbol,     // ,
    TokenType::Id,         // sum
    TokenType::Symbol,   // )
    TokenType::Symbol, // }

    TokenType::Id,         // dict2
    TokenType::Symbol,     // .
    TokenType::Id,          // print
    TokenType::Symbol,      // (
    TokenType::Symbol,      // )
    TokenType::Comment,      // #
    TokenType::Comment,      // #
    TokenType::ETX,      // 
    };


    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}

TEST(LexerTests, TestDict_2_Example)
{
    string source = R"(
def compare(key1, value1, key2, value2){
return key1 < key2
}

key1 = "key1"
value1 = 1
key2 = "key2"
value2 = 2
dict1 = <compare>{key1 : value1, key2 : value2})";

    istringstream sourceString(source);
    Lexer lexer(sourceString);

    vector<TokenType> ExpectedTokenTypes = {
    TokenType::Keyword,    // def
    TokenType::Id,         // compare
    TokenType::Symbol,    // (
    TokenType::Id,         // key1
    TokenType::Symbol,     // ,
    TokenType::Id,         // value1
    TokenType::Symbol,     // ,
    TokenType::Id,         // key2
    TokenType::Symbol,     // ,
    TokenType::Id,         // value2
    TokenType::Symbol,   // )
    TokenType::Symbol,  // {
    TokenType::Keyword,    // return
    TokenType::Id,         // key1
    TokenType::Symbol,     // <
    TokenType::Id,         // key2
    TokenType::Symbol, // }

    TokenType::Id,         // key1
    TokenType::Symbol,     // =
    TokenType::StringConst,// "key1"
    TokenType::Id,         // value1
    TokenType::Symbol,     // =
    TokenType::IntConst,   // 1
    TokenType::Id,         // key2
    TokenType::Symbol,     // =
    TokenType::StringConst,// "key2"
    TokenType::Id,         // value2
    TokenType::Symbol,     // =
    TokenType::IntConst,   // 2

    TokenType::Id,         // dict1
    TokenType::Symbol,     // =
    TokenType::Symbol,     // <
    TokenType::Id,         // compare
    TokenType::Symbol,     // >
    TokenType::Symbol,  // {
    TokenType::Id,         // key1
    TokenType::Symbol,     // :
    TokenType::Id,         // value1
    TokenType::Symbol,     // ,
    TokenType::Id,         // key2
    TokenType::Symbol,     // :
    TokenType::Id,         // value2
    TokenType::Symbol, // }
    TokenType::ETX, // }
    };


    for (TokenType expectedTokenType : ExpectedTokenTypes)
    {
        auto token = lexer.nextToken();
        EXPECT_EQ(token.type, expectedTokenType);
    }
}