#pragma once

#include <stdint.h>

enum {
    COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GRAY = 7,
	COLOR_DARK_GRAY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_PINK = 13,
	COLOR_YELLOW = 14,
	COLOR_WHITE = 15,
};

void initin();
void clear();
void reset();
void putc(char c);
void puts(const char *str);
void putcin(char c);
void clearcin();
void clearin();
void printf(const char *fmt, ...);
void set_color(uint8_t front, uint8_t back);
void set_color_in(uint8_t front, uint8_t back);
