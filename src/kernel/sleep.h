#pragma once

#include <stdint.h>

void timer_init();
uint64_t get_ticks();
void sleepm(uint64_t ms);
