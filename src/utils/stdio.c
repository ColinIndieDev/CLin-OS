#include "stdio.h"
#include "../kernel/cmd.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#define VGA_PTR 0xb8000

#define COL_COUNT 80
#define ROW_COUNT 25

typedef struct {
    uint8_t c;
    uint8_t color;
} character_t;

static character_t *buffer = (character_t *)VGA_PTR;
static size_t cur_col = 0;
static size_t cur_col_in = PTR_DEFAULT;
static size_t cur_row = 0;
static uint8_t color = COLOR_WHITE | COLOR_BLACK << 4;
static uint8_t color_in = COLOR_WHITE | COLOR_BLACK << 4;

void initin() {
    buffer[0 + (COL_COUNT * (ROW_COUNT - 1))].c = '>';
    buffer[1 + (COL_COUNT * (ROW_COUNT - 1))].c = ' ';
}

void clearr(size_t row) {
    character_t empty = {
        .c = ' ',
        .color = color
    };

    for (int col = 0; col < COL_COUNT; col++) {
        buffer[col + (COL_COUNT * row)] = empty;
    }
}

void clear() {
    for (int row = 0; row < ROW_COUNT - 1; row++) {
        clearr(row);
    }
}

void reset() {
    clear();
    cur_col = 0;
    cur_row = 0;
}

void putn() {
    cur_col = 0;
    if (cur_row < ROW_COUNT - 2) {
        cur_row++;
        return;
    }

    for (int row = 1; row < ROW_COUNT - 1; row++) {
        for (int col = 0; col < COL_COUNT; col++) {
            if (row > ROW_COUNT - 2) {
                continue;
            }
            buffer[col + (COL_COUNT * (row - 1))] = buffer[col + (COL_COUNT * row)];
        }
    }
}

void putc(char c) {
    if (c == '\n') {
        putn();
        return;
    }
    if (cur_col > COL_COUNT) {
        putn();
    }

    buffer[cur_col + (COL_COUNT * cur_row)] = (character_t) {
        .c = c,
        .color = color
    };

    cur_col++;
}

void putcin(char c) {
    if (c == '\n') {
        return;
    }
    if (cur_col_in < COL_COUNT) {
        buffer[cur_col_in + (COL_COUNT * (ROW_COUNT - 1))] = (character_t) {
            .c = c,
            .color = color_in
        };
    }

    cur_col_in++;
}

void clearcin() {
    if (cur_col_in > PTR_DEFAULT) {
        cur_col_in--;
    }
    buffer[cur_col_in + (COL_COUNT * (ROW_COUNT - 1))].c = ' ';
}

void clearin() {
    character_t empty = {
        .c = ' ',
        .color = color
    };

    for (int col = PTR_DEFAULT; col < COL_COUNT; col++) {
        buffer[col + (COL_COUNT * (ROW_COUNT - 1))] = empty;
    }
    cur_col_in = PTR_DEFAULT;
}

void puts(const char *str) {
    for (int i = 0;;i++) {
        if (str[i] == '\0') {
            return;
        }
        putc(str[i]);
    }
}

void set_color(uint8_t front, uint8_t back) {
    color = front + (back << 4);
}

void set_color_in(uint8_t front, uint8_t back) {
    color_in = front + (back << 4);
}

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

const char hex_chars[] = "0123456789abcdef";

static void print_number(uint64_t number, bool sign, int number_sign, int radix) {
    char buffer[32];
    int pos = 0;

    do {
        uint32_t rem = number % radix;
        buffer[pos++] = hex_chars[rem];
        number /= radix;
    } while (number > 0);

    if (sign && number_sign < 0) {
        buffer[pos++] = '-';
    }

    while (--pos >= 0) {
        putc(buffer[pos]);
    }
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;

    while (*fmt) {
        switch (state) {
        case PRINTF_STATE_NORMAL:
            if (*fmt == '%') {
                state = PRINTF_STATE_LENGTH;
            } else {
                putc(*fmt);
            }
            break;

        case PRINTF_STATE_LENGTH:
            if (*fmt == 'h') {
                length = PRINTF_LENGTH_SHORT;
                state = PRINTF_STATE_LENGTH_SHORT;
            } else if (*fmt == 'l') {
                length = PRINTF_LENGTH_LONG;
                state = PRINTF_STATE_LENGTH_LONG;
            } else {
                goto PRINTF_STATE_SPEC_;
            }
            break;

        case PRINTF_STATE_LENGTH_SHORT:
            if (*fmt == 'h') {
                length = PRINTF_LENGTH_SHORT_SHORT;
                state = PRINTF_STATE_SPEC;
            } else {
                goto PRINTF_STATE_SPEC_;
            }
            break;

        case PRINTF_STATE_LENGTH_LONG:
            if (*fmt == 'l') {
                length = PRINTF_LENGTH_LONG_LONG;
                state = PRINTF_STATE_SPEC;
            } else {
                goto PRINTF_STATE_SPEC_;
            }
            break;

        case PRINTF_STATE_SPEC:
        PRINTF_STATE_SPEC_:
            switch (*fmt) {
            case 'c':
                putc((char)va_arg(args, int));
                break;

            case 's': {
                const char* str = va_arg(args, const char*);
                if (!str) str = "(null)";
                puts(str);
                break;
            }

            case '%':
                putc('%');
                break;

            case 'd':
            case 'i': {
                int64_t n;
                if (length == PRINTF_LENGTH_LONG_LONG || length == PRINTF_LENGTH_LONG) {
                    n = va_arg(args, int64_t);
                }
                else {
                    n = va_arg(args, int);
                }

                int number_sign = 1;
                uint64_t unumber;

                if (n < 0) {
                    number_sign = -1;
                    unumber = (uint64_t)(-n);
                } else {
                    unumber = (uint64_t)n;
                }

                print_number(unumber, true, number_sign, 10);
                break;
            }

            case 'u': {
                uint64_t unumber;
                if (length == PRINTF_LENGTH_LONG_LONG || length == PRINTF_LENGTH_LONG) {
                    unumber = va_arg(args, uint64_t);
                } else {
                    unumber = va_arg(args, unsigned int);
                }

                print_number(unumber, false, 1, 10);
                break;
            }

            case 'X':
            case 'x':
            case 'p': {
                uint64_t unumber;
                if (*fmt == 'p' || length == PRINTF_LENGTH_LONG_LONG || length == PRINTF_LENGTH_LONG) {
                    unumber = va_arg(args, uint64_t);
                } else {
                    unumber = va_arg(args, unsigned int);
                }

                if (*fmt == 'p') {
                    puts("0x");
                }

                print_number(unumber, false, 1, 16);
                break;
            }

            case 'o': {
                uint64_t unumber = va_arg(args, unsigned int);
                print_number(unumber, false, 1, 8);
                break;
            }

            default:
                break;
            }

            state = PRINTF_STATE_NORMAL;
            length = PRINTF_LENGTH_DEFAULT;
            break;
        }
        fmt++;
    }

    va_end(args);
}
