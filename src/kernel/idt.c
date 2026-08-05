#include "idt.h"
#include "cmd.h"
#include "../utils/io.h"
#include "../utils/sleep.h"

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];
static idtr_t idtr;

extern void idt_load(idtr_t* idtr_ptr);
extern void isr0_stub();
extern void isr_keyboard();
extern void isr_timer();

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// For the guys who have a german keyboard layout on their PCs
static char kbdde[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '?', '?', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '?', '+', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '?', '?', '#', 0,  '<',
    'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,  0,   0,   ' ', 0,
};
static char kbdde_shift[] = {
    0,  27, '!', '"', '?', '$', '%', '&', '/', '(', ')', '=', '?', '`', '\b',
    '\t','Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '?', '*', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '?', '?', '\'', 0,  '>',
    'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,  0,   0,   ' ', 0,
};
static char kbdde_altgr[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   '{', '[', ']', '}', 0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

#define SHIFT_PRESSED  0x01
#define ALTGR_PRESSED  0x02

char translate_scancode(uint8_t scancode, uint8_t state) {
    if (state & ALTGR_PRESSED) {
        if (kbdde_altgr[scancode] != 0) {
            return kbdde_altgr[scancode];
        }
    }
    if (state & SHIFT_PRESSED) {
        return kbdde_shift[scancode];
    }
    return kbdde[scancode];
}

#define KEYBOARD_PORT 0x60
#define KEY_BACKSPACE 0x0E
#define KEY_RETURN 0x1C

static uint8_t modifier_state = 0;

void keyboard_handler() {
    uint8_t scancode = inportb(KEYBOARD_PORT);

    if (scancode == KEY_BACKSPACE) {
        clearcin();
        clearccmd();
    } else if (scancode == KEY_RETURN) {
        execcmd();
    } else if (scancode & 0x80) {
        scancode &= 0x7F;
        
        if (scancode == 0x2A || scancode == 0x36) {
            modifier_state &= ~SHIFT_PRESSED;
        } else if (scancode == 0x38) { 
            modifier_state &= ~ALTGR_PRESSED;
        }
    } else {
        if (scancode == 0x2A || scancode == 0x36) {
            modifier_state |= SHIFT_PRESSED;
        } else if (scancode == 0x38) {
            modifier_state |= ALTGR_PRESSED;
        } else {
            char ascii = translate_scancode(scancode, modifier_state);
            putcin(ascii);
            putccmd(ascii);
        }
    }
}

void exception_handler(uint64_t interrupt_num) {
    if (interrupt_num == 0) {
        printf("PANIC MODE! (Division by Zero)");
    }
    while(1) { 
        __asm__ volatile("cli; hlt"); 
    }
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    uint64_t addr = (uint64_t)isr;
    descriptor->isr_low    = (uint16_t)(addr & 0xFFFF);
    descriptor->kernel_cs  = 0x08;
    descriptor->ist        = 0;
    descriptor->attribs = flags;
    descriptor->isr_mid    = (uint16_t)((addr >> 16) & 0xFFFF);
    descriptor->isr_high   = (uint32_t)((addr >> 32) & 0xFFFFFFFF);
    descriptor->reserved   = 0;
}

void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void idt_init() {
    idtr.base = (uint64_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * 256) - 1;

    pic_remap();

    idt_set_descriptor(0, isr0_stub, 0x8E);
    idt_set_descriptor(32, isr_timer, 0x8E);
    idt_set_descriptor(33, isr_keyboard, 0x8E);

    idt_load(&idtr);

    // Enable Timer & Keyboard in PIC
    outb(0x21, 0b11111100);
    __asm__ volatile ("sti");
}
