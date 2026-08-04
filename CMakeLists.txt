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
    src/impl/x86-64/boot/header.asm
    src/impl/x86-64/boot/main.asm
    src/impl/x86-64/boot/main64.asm
    
    src/kernel/io.c
    src/kernel/sleep.c
    src/kernel/rtc.c
    src/kernel/idt.c
    src/kernel/idt.asm

    src/kernel/main.c
)

target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
    -nostdlib
    -static
)
