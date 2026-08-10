#include "cmd.h"
#include "vfs.h"
#include "../utils/stdio.h"
#include "../utils/rtc.h"
#include "../utils/string.h"
#include "../utils/memsize.h"

#include <stdint.h>

static char cmd_buffer[256] = {0};
static uint32_t ptr = 0;

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
    if (strncmp(cmd_buffer, "cat ", 4) == 0) {
        uint32_t nameptr = 4;
        char filename[256];
        char buffer[KiB(1)];
        uint32_t i = 0;

        while (cmd_buffer[nameptr] && i < 255) {
            filename[i++] = cmd_buffer[nameptr++];
        }
        filename[i] = '\0';

        if (i == 0) {
            set_color(COLOR_RED, COLOR_BLACK);
            printf("Usage: cat <filename>\n");
            set_color(COLOR_WHITE, COLOR_BLACK);

            uint32_t cptr = 0;
            while (cmd_buffer[cptr]) {
                cmd_buffer[cptr++] = '\0';
            }
            ptr = 0;
            clearin();
            return;
        }

        uint32_t idx = 0;
        for (idx = 0; idx < get_file_array_size(); idx++) {
            vfs_node_t *file = &(get_file_array()[idx]);
            if (strcmp(filename, file->name) == 0) {
                uint32_t bytes_read = file_read(file, 0, sizeof(buffer) - 1, (uint8_t *)buffer);
                buffer[bytes_read] = '\0';
                printf("%s\n", buffer);

                uint32_t cptr = 0;
                while (cmd_buffer[cptr]) {
                    cmd_buffer[cptr++] = '\0';
                }
                ptr = 0;
                clearin();
                return;
            }
        }
        set_color(COLOR_RED, COLOR_BLACK);
        printf("Cannot find %s\n", filename);
        set_color(COLOR_WHITE, COLOR_BLACK);
        
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
