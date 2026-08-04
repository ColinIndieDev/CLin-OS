#include "io.h"
#include "sleep.h"
#include "idt.h"
#include "rtc.h"

int elapsed = 0;

void kernel_main() {
    idt_init();
    timer_init();
    clear();
    set_color(COLOR_YELLOW, COLOR_BLACK);
    printf("Welcome to the CLin Operating System!\n");
    rtc_time_t now;
    rtc_get_datetime(&now);
    printf("Time: %d:%d:%d\nDate: %d.%d.%d\n", now.hour, now.minute, now.second, now.day, now.month, now.year);
    while (1) {

    }
}
