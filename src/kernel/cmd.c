#include "cmd.h"
#include "../utils/io.h"
#include "../utils/rtc.h"

#include <stdint.h>

static char cmd_buffer[256] = {0};
static uint32_t ptr = 0;

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void putccmd(char c) {
    cmd_buffer[ptr++] = c;
}

void clearccmd() {
    if (ptr > 0) {
        ptr--;
    }
    cmd_buffer[ptr] = '\0';
}

void execcmd() {
    if (strcmp(cmd_buffer, "time") == 0) {
        rtc_time_t now;
        rtc_get_datetime(&now);
        printf("Time: %d:%d:%d\nDate: %d.%d.%d\n", now.hour, now.minute, now.second, now.day, now.month, now.year);

        uint32_t cptr = 0;
        while (cmd_buffer[cptr]) {
            cmd_buffer[cptr++] = '\0';
        }
        ptr = 0;
        clearin();
        return;
    }
    if (strcmp(cmd_buffer, "clear") == 0) {
        reset();

        uint32_t cptr = 0;
        while (cmd_buffer[cptr]) {
            cmd_buffer[cptr++] = '\0';
        }
        ptr = 0;
        clearin();
        return;
    }

    set_color(COLOR_RED, COLOR_BLACK);
    printf("Invalid cmd: \'%s\'\n", cmd_buffer);
    set_color(COLOR_WHITE, COLOR_BLACK);

    uint32_t cptr = 0;
    while (cmd_buffer[cptr]) {
        cmd_buffer[cptr++] = '\0';
    }
    ptr = 0;
    clearin();
}
