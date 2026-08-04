#include "sleep.h"

static volatile uint64_t ticks = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void timer_handler() {
    ticks++;
}

uint64_t get_ticks() {
    return ticks;
}

void timer_init() {
    uint32_t frequency = 1000;
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

void sleepm(uint64_t ms) {
    uint64_t target = ticks + ms;
    while (ticks < target) {
        __asm__ volatile ("hlt");
    }
}

/*
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void sleepm(uint32_t ms) {
    outb(0x43, 0xB0); 

    while (ms > 0) {
        uint32_t current_ms = (ms > 50) ? 50 : ms;
        uint16_t count = current_ms * 1193;

        outb(0x42, (uint8_t)(count & 0xFF));
        outb(0x42, (uint8_t)((count >> 8) & 0xFF));

        uint8_t control = inb(0x61);
        outb(0x61, (control & 0xDC) | 0x01);

        while (!(inb(0x61) & 0x20)) {
            __asm__ volatile ("pause");
        }

        ms -= current_ms;
    }
}
*/
