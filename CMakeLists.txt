cmake_minimum_required(VERSION 3.31)

# Name of executable
project(OS C ASM_NASM)

# ===== CMAKE SETTINGS =====

enable_language(ASM_NASM)
enable_language(C)

set(CMAKE_ASM_NASM_FLAGS "-f elf64")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -nostdlib -fno-stack-protector -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mcmodel=kernel -Wall -Wextra")

add_executable(kernel.elf
    src/boot/header.asm
    src/boot/main.asm
    src/boot/main64.asm
    
    src/utils/stdio.c
    src/utils/sleep.c
    src/utils/string.c
    src/utils/rtc.c
    src/utils/stdlib.c
    src/utils/pmm.c

    src/kernel/idt.c
    src/kernel/idt.asm
    src/kernel/cmd.c
    src/kernel/fb.c
    src/kernel/vfs.c
    src/kernel/main.c
)

target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
    -nostdlib
    -static
)
