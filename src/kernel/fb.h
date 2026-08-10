#pragma once

#include <stdint.h>

typedef struct {
    uint32_t x, y;
} vec2u;

uint32_t *get_framebuffer();
uint32_t get_screen_width();
uint32_t get_screen_height();
void init_graphics(uintptr_t multiboot_addr);
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void draw_rect(vec2u pos, vec2u size, uint32_t color);
