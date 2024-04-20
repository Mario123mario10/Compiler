#pragma once
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
const char ETX_CHAR = '\x03'; //End of Text

class Skaner
{
private:
    istream& source;

    size_t currentPosition = 0;
    size_t currentLine = 1;
    size_t currentColumn = 1;

    char charAtPos(size_t position);

public:

    Skaner(istream& source) : source(source) {}
    char currentChar() { return charAtPos(currentPosition); }
    size_t getCurrentPosition() { return currentPosition; }
    size_t getCurrentLine() { return currentLine; }
    size_t getCurrentColumn() { return currentColumn; }

    void advance();
}; 
