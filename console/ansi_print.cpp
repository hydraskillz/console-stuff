//------------------------------------------------------------------------------
// ansi_print.cpp
//
// This implementation is only needed on windows
//------------------------------------------------------------------------------
#include "ansi_print.h"
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <io.h>

#ifndef FOREGROUND_MASK
#	define FOREGROUND_MASK (FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#endif
#ifndef BACKGROUND_MASK
#	define BACKGROUND_MASK (BACKGROUND_RED|BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY)
#endif

class AnsiPrinter
{
public:
	static const char ESCAPE = '\x1B';
	static const char BRACE = '[';
	static const char VALUE_SEPERATOR = ';';
	static const char CMD_SET_MODE = 'h';
	static const char CMD_RESET_MODE = 'l';
	static const char CMD_SET_GRAPHICS_MODE = 'm';
	static const int MAX_VALUES = 6;
	static const int INVALID_VALUE = -1;
	static const WORD INVALID_ATTRIBUTES = (WORD) -1;

	enum TextAttribute
	{
		TA_ALL_OFF		= 0,
		TA_BOLD			= 1,
		TA_UNDERSCORE	= 4,
		TA_BLINK		= 5,
		TA_INVERTED		= 7,
		TA_CONCEALED	= 8,
	};

	static const int TOTAL_NUM_COLORS = 7;
	static const int FOREGROUND_BEGIN = 30;
	static const int BACKGROUND_BEGIN = 40;
	static const int FOREGROUND_END = FOREGROUND_BEGIN + TOTAL_NUM_COLORS;
	static const int BACKGROUND_END = BACKGROUND_BEGIN + TOTAL_NUM_COLORS;

	AnsiPrinter();

	bool IsAnsiEscapeCode(const char c) const { return c == ESCAPE; }
	void ResetValueTable();
	char ReadAnsiValues(const char*& ptr);
	void SetGraphicsMode(WORD& attributes, WORD originalAttributes);

	int Print(const char* buffer);

private:
	HANDLE m_handle;
	WORD m_attributes;
	int m_valueCount;
	int m_valueTable[MAX_VALUES];
};


int ansi_printf(const char* format, ...)
{
	static AnsiPrinter printer;

	// Make a buffer to print into
	static const size_t BUFFER_SIZE = 8192;
	static char buffer[BUFFER_SIZE];

	// Apply the format
	va_list args;
	va_start(args, format);
	int count = vsnprintf(buffer, BUFFER_SIZE, format, args);
	if (count != -1)
	{
		// Print to stdout and apply color/style
		printer.Print(buffer);
	}
	va_end(args);

	return count;
}

AnsiPrinter::AnsiPrinter()
{
	// Get a handle to stdout
	m_handle = (HANDLE) _get_osfhandle(_fileno(stdout));
	m_valueCount = 0;
	m_attributes = INVALID_ATTRIBUTES;
}

int AnsiPrinter::Print(const char* buffer)
{
	// We couldn't get a handle on the stdout - just print the buffer
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		puts(buffer);
		return 0;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_handle, &csbi);
	WORD attributes = csbi.wAttributes;
	WORD originalAttributes = attributes;

	if (m_attributes == INVALID_ATTRIBUTES)
	{
		m_attributes = attributes;
	}

	const char* ptr = buffer;

	while (*ptr != 0)
	{
		const char currentCharacter = *ptr;

		// We found an ansi escape character
		if (IsAnsiEscapeCode(currentCharacter))
		{
			++ptr;
			char ansiCode = ReadAnsiValues(ptr);

			switch (ansiCode)
			{
				case CMD_SET_MODE:
					break;

				case CMD_RESET_MODE:
					break;

				case CMD_SET_GRAPHICS_MODE:
					SetGraphicsMode(attributes, originalAttributes);
					break;

				default:
					break;
			}
		}
		else
		{
			putchar(*ptr);
			++ptr;
		}
	}
	return ptr - buffer;
}

void AnsiPrinter::ResetValueTable()
{
	for (int i = 0; i < MAX_VALUES; ++i)
	{
		m_valueTable[i] = INVALID_VALUE;
	}
}

char AnsiPrinter::ReadAnsiValues(const char*& ptr)
{
	ResetValueTable();

	int nextCodeIndex = 0;
	char valueCode = *ptr++;
	while (valueCode != 0)
	{
		// Check for a numeric value
		if (isdigit(valueCode))
		{
			if (m_valueTable[nextCodeIndex] == INVALID_VALUE)
			{
				m_valueTable[nextCodeIndex] = valueCode - '0';
			}
			else
			{
				m_valueTable[nextCodeIndex] = m_valueTable[nextCodeIndex] * 10 + valueCode - '0';
			}
		}
		// Skip the brace
		else if (valueCode == BRACE)
		{
		}
		// Found a value seperator
		else if (valueCode == VALUE_SEPERATOR)
		{
			if (nextCodeIndex + 1 == MAX_VALUES)
			{
				break;
			}
			++nextCodeIndex;
		}
		// End of ansi code
		else
		{
			break;
		}
		valueCode = *ptr++;
	}
	m_valueCount = nextCodeIndex;
	return valueCode;
}

void AnsiPrinter::SetGraphicsMode(WORD& attributes, WORD originalAttributes)
{
	attributes = originalAttributes;
	for (int i = 0; i <= m_valueCount; ++i)
	{
		int value = m_valueTable[i];

		// Reset
		if (value == INVALID_VALUE || value == TA_ALL_OFF)
		{
			attributes = m_attributes;
		}
		// Bold
		else if (value == TA_BOLD)
		{
			attributes |= FOREGROUND_INTENSITY;
		}
		// Unsupported
		//else if (value == TA_UNDERSCORE) {}
		// Unsupported
		//else if (value == TA_BLINK) {}
		else if (value == TA_INVERTED)
		{
			attributes =
				((attributes & FOREGROUND_MASK) << 4) |
				((attributes & BACKGROUND_MASK) >> 4);
		}
		else if (value >= FOREGROUND_BEGIN && value <= FOREGROUND_END)
		{
			attributes = (attributes & BACKGROUND_MASK);
			int color = value - FOREGROUND_BEGIN;
			// Mix the colors
			if (color & 1)
				attributes |= FOREGROUND_RED;
			if (color & 2)
				attributes |= FOREGROUND_GREEN;
			if (color & 4)
				attributes |= FOREGROUND_BLUE;
		}
		else if (value >= BACKGROUND_BEGIN && value <= BACKGROUND_END)
		{
			attributes = (attributes & FOREGROUND_MASK);
			int color = value - BACKGROUND_BEGIN;
			// Mix the colors
			if (color & 1)
				attributes |= BACKGROUND_RED;
			if (color & 2)
				attributes |= BACKGROUND_GREEN;
			if (color & 4)
				attributes |= BACKGROUND_BLUE;
		}
	}
	// Apply the attributes
	SetConsoleTextAttribute(m_handle, attributes);
}