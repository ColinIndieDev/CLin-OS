#include "../utils/io.h"
#include "../utils/sleep.h"
#include "../utils/rtc.h"
#include "../utils/alloc.h"
#include "idt.h"

extern uint64_t kernel_end;

#define MEMORY_MB 128

void kernel_main() {
    idt_init();
    size_t memory = MEMORY_MB * 1024 * 1024;
    pmm_init(memory, (uintptr_t)&kernel_end);
    timer_init();
    clear();
    initin();
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    printf("Welcome to the CLin Operating System!\n");
    set_color(COLOR_WHITE, COLOR_BLACK);

    while (1) {
        // Waiting
    }
}
