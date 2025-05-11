#!/bin/bash
nasm -f elf32 boot/boot.asm -o boot/boot.o
gcc -m32 -c src/fikusos.c -o src/fikusos.o -ffreestanding -nostdlib -O2 -fno-stack-protector
ld -m elf_i386 -T boot/linker.ld -o iso/boot/fikusos.kernel boot/boot.o src/fikusos.o -nostdlib -z noexecstack
grub-mkrescue -o fikusos.iso iso
