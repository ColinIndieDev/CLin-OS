#include "fb.h"

uint32_t *framebuffer = 0;
uint32_t fb_width = 0;
uint32_t fb_height = 0;
uint32_t pitch = 0;

struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
};

uint32_t *get_framebuffer() {
    return framebuffer;
}

uint32_t get_screen_width() {
    return fb_width;
}
uint32_t get_screen_height() {
    return fb_height;
}

void init_graphics(uintptr_t multiboot_addr) {
    if (multiboot_addr == 0) {
        return;
    }

    uint8_t* ptr = (uint8_t*)(multiboot_addr + 8);

    while (1) {
        struct multiboot_tag* tag = (struct multiboot_tag*)ptr;

        if (tag->type == 0) {
            break;
        }

        if (tag->type == 8) {
            struct multiboot_tag_framebuffer* fb = (struct multiboot_tag_framebuffer*)tag;

            framebuffer    = (uint32_t*)(uintptr_t)fb->framebuffer_addr;
            fb_width  = fb->framebuffer_width;
            fb_height = fb->framebuffer_height;
            pitch  = fb->framebuffer_pitch;
            break;
        }
        ptr += ((tag->size + 7) & ~7);
    }
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x < 0 || x >= fb_width || y < 0 || y >= fb_height) {
        return;
    }
    uint32_t* line = (uint32_t*)((uint8_t*)framebuffer + (y * pitch) + (x * 4));
    line[x] = color;
}

void draw_rect(vec2u pos, vec2u size, uint32_t color) {
    for (uint32_t y = pos.y; y < pos.y + size.y; y++) {
        for (uint32_t x = pos.x; x < pos.x + size.x; x++) {
            draw_pixel(x, y, color);
        }
    }
}
