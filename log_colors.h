//------------------------------------------------------------------------------
// log_colors.h
//
// Defines ansi text color attributes
//------------------------------------------------------------------------------
#pragma once

#define ANSI_ESCAPE_CODE		"\x1B"
#define ANSI_GRAPHICS_MODE		"m"
#define ANSI_VALUE_SEPERATOR	";"

#define ANSI_BLACK		"0"
#define ANSI_RED		"1"
#define ANSI_GREEN		"2"
#define ANSI_YELLOW		"3"
#define ANSI_BLUE		"4"
#define ANSI_MAGENTA	"5"
#define ANSI_CYAN		"6"
#define ANSI_WHITE		"7"
#define ANSI_FOREGROUND "3"
#define ANSI_BACKGROUND "4"
#define ANSI_BOLD		ANSI_VALUE_SEPERATOR "1"
#define ANSI_NONE		""
#define ANSI_RESET		"0"

// The graphics attrib layout
//
// ANSI_ESCAPE_CODE 
//  [ANSI_FOREGROUND FG_COLOR]
//  [ANSI_VALUE_SEPERATOR ANSI_BOLD]
//  [ANSI_VALUE_SEPERATOR]
//  [ANSI_BACKGROUND BG_COLOR]
//  [ANSI_VALUE_SEPERATOR ANSI_BOLD]
// ANSI_GRAPHICS_MODE

// Set the foreground and background colors and styles
#define LOG_GRAPHICS_ATTRIB(FG_COLOR, FG_STYLE, BG_COLOR, BG_STYLE) ANSI_ESCAPE_CODE ANSI_FOREGROUND FG_COLOR FG_STYLE ANSI_VALUE_SEPERATOR ANSI_BACKGROUND BG_COLOR BG_STYLE ANSI_GRAPHICS_MODE
// Set just the foreground color and style
#define LOG_GRAPHICS_ATTRIB_FG(FG_COLOR, FG_STYLE) ANSI_ESCAPE_CODE ANSI_FOREGROUND FG_COLOR FG_STYLE ANSI_GRAPHICS_MODE
// Set just the background color and style
#define LOG_GRAPHICS_ATTRIB_BG(BG_COLOR, BG_STYLE) ANSI_ESCAPE_CODE ANSI_BACKGROUND BG_COLOR BG_STYLE ANSI_GRAPHICS_MODE

// Helpers for making a foreground color
#define LOG_COLOR(COLOR)		LOG_GRAPHICS_ATTRIB_FG(COLOR, ANSI_NONE)
#define LOG_COLOR_BOLD(COLOR)	LOG_GRAPHICS_ATTRIB_FG(COLOR, ANSI_BOLD)

// Foreground text color, on a black background
#define LOG_RED				LOG_COLOR(ANSI_RED)
#define LOG_GREEN			LOG_COLOR(ANSI_GREEN)
#define LOG_YELLOW			LOG_COLOR(ANSI_YELLOW)
#define LOG_BLUE			LOG_COLOR(ANSI_BLUE)
#define LOG_MAGENTA			LOG_COLOR(ANSI_MAGENTA)
#define LOG_CYAN			LOG_COLOR(ANSI_CYAN)
#define LOG_WHITE			LOG_COLOR(ANSI_WHITE)
// Foreground text color (bright), on a black background
#define LOG_BOLD_RED		LOG_COLOR_BOLD(ANSI_RED,		ANSI_BOLD)
#define LOG_BOLD_GREEN		LOG_COLOR_BOLD(ANSI_GREEN,		ANSI_BOLD)
#define LOG_BOLD_YELLOW		LOG_COLOR_BOLD(ANSI_YELLOW,		ANSI_BOLD)
#define LOG_BOLD_BLUE		LOG_COLOR_BOLD(ANSI_BLUE,		ANSI_BOLD)
#define LOG_BOLD_MAGENTA	LOG_COLOR_BOLD(ANSI_MAGENTA,	ANSI_BOLD)
#define LOG_BOLD_CYAN		LOG_COLOR_BOLD(ANSI_CYAN,		ANSI_BOLD)
#define LOG_BOLD_WHITE		LOG_COLOR_BOLD(ANSI_WHITE,		ANSI_BOLD)
// Resets the graphics attributes to default, i.e. "\x1B[0m"
#define LOG_RESET			ANSI_ESCAPE_CODE ANSI_RESET ANSI_GRAPHICS_MODE