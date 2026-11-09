# CLin Operating System
## About

One day I decided making an OD couldnt be that hard so I tried making one. I mean it cannot do anything productive but it is working which is a miracle
by itself


## Features / What I made

- Stack & Heap (+ memory management)
- Interrput Description Table (IDT)
- a basic c std library (putc, printf etc.)
- a tty / terminal
- some commands
- real time clock
- sleep
- vga buffer customization with helper
- long mode (64 bit)
- possible and ready to go setup for framebuffer mode
- Virtual File System (VFS)

## Restriction(s)
I currently only used QEMU to run my operating system so no guarantee if it would work booting it from usb since maybße some stuff is not initialized
or ther problems which QEMU forgives unlike the real hardware though.

Additionally there is no actual filesystem since it will not be saved anywhere or on the hard drive which I have not figured out at all
