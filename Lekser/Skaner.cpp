#include "Skaner.h"

char Skaner::charAtPos(size_t position)
{
    source.seekg(position);
    return source.good() ? source.get() : ETX_CHAR;
}

void Skaner::advance()
{
    if (currentChar() == '\n')
    {
        ++currentLine;
        currentColumn = 0;
    }
    ++currentPosition;
    ++currentColumn;
}