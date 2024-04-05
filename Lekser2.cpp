#include <iostream>
#include <regex>
#include <string>
#include <vector>

enum class TokenType {
	NumConst,
	BoolConst,
	NullConst,
	StringConst,
	Id,
	Keyword,
	Symbol,
	Comment,
	Unknown
};

struct Token {
	TokenType type;
	std::string value;
};

class Lexer {
public:
	Lexer(const std::string& source) : source_(source) {}

	std::vector<Token> tokenize() {
		std::vector<Token> tokens;
		std::regex tokenRegex(
			//R"((0|[1-9][0-9]*\.[0-9]+|[1-9][0-9]*)|"true"|"false"|"null"|"[^"\\]*(\\.[^"\\]*)*"|[A-Za-z_][A-Za-z_0-9]*|==|!=|>=|<=|<|>|\|\||&&|!|\+|\-|\*|\/|\^|\(|\)|\{|\}|\[|\]|,|;|#[^\n]*|\.)",
			R"((0|[1-9][0-9]*[\.[0-9]+]?)|"true"|"false"|"null"|"[^"\\]*(\\.[^"\\]*)*"|[A-Za-z_][A-Za-z_0-9]*|==|!=|>=|<=|<|>|\|\||&&|!|\+|\-|\*|\/|\^|\(|\)|\{|\}|\[|\]|,|;|#[^\n]*|\.)",
			std::regex_constants::ECMAScript
			);
		auto words_begin = std::sregex_iterator(source_.begin(), source_.end(), tokenRegex);
		auto words_end = std::sregex_iterator();

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			std::string matchStr = match.str();
			TokenType type = determineType(matchStr);
			tokens.push_back(Token{ type, matchStr });
		}

		return tokens;
	}

private:
	std::string source_;

	TokenType determineType(const std::string& token) {
		if (std::regex_match(token, std::regex(R"((0|[1-9][0-9]*(\.[0-9]+)?))"))) return TokenType::NumConst;
		if (std::regex_match(token, std::regex(R"("true"|"false")"))) return TokenType::BoolConst;
		if (token == "null") return TokenType::NullConst;
		if (std::regex_match(token, std::regex(R"("[^"\\]*(\\.[^"\\]*)*")"))) return TokenType::StringConst;
		if (std::regex_match(token, std::regex(R"([A-Za-z_][A-Za-z_0-9]*)"))) return TokenType::Id;
		if (token[0] == '#') return TokenType::Comment;
		if (std::regex_match(token, std::regex(R"(==|!=|>=|<=|<|>|\|\||&&|!|\+|\-|\*|\/|\^|\(|\)|\{|\}|\[|\]|,|;)"))) return TokenType::Symbol;
		return TokenType::Unknown;
	}
};

int main() {
	std::string sourceCode = R"(
        def myFunction(x, y) {
            if (x > 10) {
                print "x is greater than 10"
            }
        }
    )";

	Lexer lexer(sourceCode);
	auto tokens = lexer.tokenize();

	for (const auto& token : tokens) {
		std::cout << "Token Type: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
	}

	return 0;
}
