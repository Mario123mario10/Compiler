#include "Error.h"

string Error::getMessage(Type type) const
{
	switch (type)
	{
	case NUMBER_TOO_LONG:
		return "Too long number";
	case NUMBER_MULTIPLE_LEADING_ZEROS:
		return "Invalid number format with multiple leading zeros";
	case NUMBER_MULTIPLE_DOTS:
		return "Invalid number format with multiple dots";
	case NUMBER_OUT_OF_RANGE_DOUBLE:
		return "Double value out of range";
	case NUMBER_OUT_OF_RANGE_INT:
		return "Integer value out of range";
	case IDENTIFIER_TOO_LONG:
		return "Identifier too long";
	case STRING_TOO_LONG:
		return "String too long";
	case STRING_INFINITE:
		return "String not closed before end of file";
	case UNKNOWN_TOKEN:
		return "Unknown token";
	default:
		return "Unrecognized error";
	}
}