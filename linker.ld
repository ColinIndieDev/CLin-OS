ENTRY(_start)

SECTIONS
{
    . = 2M;

    .boot : {
        KEEP(*(.multiboot_header))
    }

    .text : {
        *(.text*)
    }
    .rodata : {
        *(.rodata*)
    }
    .data : {
        *(.data*)
    }
    .bss : {
        *(COMMON)
        *(.bss*)
    }
    kernel_end = .;
}
