#include "../utils/stdio.h"
#include "../utils/sleep.h"
#include "../utils/rtc.h"
#include "../utils/pmm.h"
#include "../utils/memsize.h"
#include "../utils/string.h"

#include "vfs.h"
#include "idt.h"
#include "fb.h"

extern uint64_t kernel_end;


void init_kernel([[maybe_unused]] uintptr_t multiboot_addr) {
    // init_graphics(multiboot_addr);
    idt_init();
    size_t memory = (size_t)MiB(128);
    pmm_init(memory, (uintptr_t)&kernel_end);
    timer_init();
    vfs_init(10);
}

void start_anim() {
    clear();
    initin();
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    printf("  /$$$$$$  /$$       /$$                  /$$$$$$   /$$$$$$ \n");
    printf(" /$$__  $$| $$      |__/                 /$$__  $$ /$$__  $$\n");
    printf("| $$  \\__/| $$       /$$ /$$$$$$$       | $$  \\ $$| $$  \\__/\n");
    printf("| $$      | $$      | $$| $$__  $$      | $$  | $$|  $$$$$$ \n");
    printf("| $$      | $$      | $$| $$  \\ $$      | $$  | $$ \\____  $$\n");
    printf("| $$    $$| $$      | $$| $$  | $$      | $$  | $$ /$$  \\ $$\n");
    printf("|  $$$$$$/| $$$$$$$$| $$| $$  | $$      |  $$$$$$/|  $$$$$$/\n");
    printf(" \\______/ |________/|__/|__/  |__/       \\______/  \\______/ \n");
    printf("\n");

    const char text[] = "Welcome to the CLin Operating System!\0";
    uint32_t ptr = 0;
    while (text[ptr]) {
        sleepm(10);
        putc(text[ptr++]);
    }
    printf("\n");
    set_color(COLOR_WHITE, COLOR_BLACK);
}

void kernel_main([[maybe_unused]] uintptr_t multiboot_addr) {
    init_kernel(multiboot_addr);
    start_anim();

    vfs_node_t *file = create_file("hello.txt", KiB(1));
    const char txt[] = "Hello from the file system of CLin OS!";
    file_write(file, 0, sizeof(txt), (const uint8_t *)txt);
    printf("Created file named hello.txt\n");

    while (1) {
        __asm__ volatile("hlt");
    }
}
