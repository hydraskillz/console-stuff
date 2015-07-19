//------------------------------------------------------------------------------
// ansi_print.h
//
// Abstraction point for ansi printf
//------------------------------------------------------------------------------
#pragma once

#ifdef _WIN32
	int ansi_printf(const char* format, ...);
#else
#	define ansi_printf(format, ...) printf(format, __VA_ARGA__)
#endif