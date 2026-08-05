#include "rtc.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

// God forbid
#define TIMEZONE_OFFSET 2 /* UTC+2 for MESZ (Middle Europa Summer Time) */

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static int rtc_updating(void) {
    outb(CMOS_ADDR, 0x0A);
    return (inportb(CMOS_DATA) & 0x80);
}

static uint8_t get_rtc_reg(int reg) {
    outb(CMOS_ADDR, reg);
    return inportb(CMOS_DATA);
}

void rtc_get_datetime(rtc_time_t *time) {
    while (rtc_updating()) {

    }

    time->second = get_rtc_reg(0x00);
    time->minute = get_rtc_reg(0x02);
    time->hour   = get_rtc_reg(0x04);
    time->day    = get_rtc_reg(0x07);
    time->month  = get_rtc_reg(0x08);
    time->year   = get_rtc_reg(0x09);

    uint8_t registerB = get_rtc_reg(0x0B);

    if (!(registerB & 0x04)) {
        time->second = (time->second & 0x0F) + ((time->second / 16) * 10);
        time->minute = (time->minute & 0x0F) + ((time->minute / 16) * 10);
        time->hour   = ((time->hour & 0x0F) + (((time->hour & 0x70) / 16) * 10)) | (time->hour & 0x80);
        time->day    = (time->day & 0x0F) + ((time->day / 16) * 10);
        time->month  = (time->month & 0x0F) + ((time->month / 16) * 10);
        time->year   = (time->year & 0x0F) + ((time->year / 16) * 10);
    }

    time->year += 2000; 

    time->hour += TIMEZONE_OFFSET;
    if (time->hour >= 24) {
        time->hour -= 24;
        time->day += 1;
    }
}
