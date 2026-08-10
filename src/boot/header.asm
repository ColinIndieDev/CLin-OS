section .multiboot_header
align 8
header_start:
dd 0xe85250d6 ; multiboot2
dd 0 ; protected mode i386
dd header_end - header_start
dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

; Remove / Comment this part for VGA-Buffer (Text only)
; align 8
; framebuffer_tag_start:
; dw 5
; dw 1
; dd framebuffer_tag_end - framebuffer_tag_start
; dd 1024 ; width
; dd 768 ; height
; dd 32 ; bits per pixel
; framebuffer_tag_end:

align 8
dw 0
dw 0
dd 8
header_end:
