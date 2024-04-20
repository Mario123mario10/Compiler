#pragma once
#include <iostream>
#include <string>

using namespace std;

class Error
{
public:
	enum Type
	{
		NUMBER_TOO_LONG, NUMBER_MULTIPLE_LEADING_ZEROS, NUMBER_MULTIPLE_DOTS, NUMBER_OUT_OF_RANGE_DOUBLE, NUMBER_OUT_OF_RANGE_INT,
		IDENTIFIER_TOO_LONG, STRING_TOO_LONG, STRING_INFINITE, UNKNOWN_TOKEN
	};
private:
	Type type;
	size_t line;
	size_t column;

	string getMessage(Type type) const;
public:
	Error(Type type, size_t line, size_t column) : type(type), line(line), column(column) {}
	string what() const { return getMessage(type) + " at line " + to_string(line) + ' ' + to_string(column); }
	Type getType() const { return type; }
	size_t getLine() const { return line; }
	size_t getColumn() const { return column; }
};