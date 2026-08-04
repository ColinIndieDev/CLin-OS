build:
	cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang
	cmake --build build
	cp build/kernel.elf iso-root/boot/kernel.elf
	grub-mkrescue -o clin.iso iso-root
