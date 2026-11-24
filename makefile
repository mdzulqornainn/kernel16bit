all: build

prepare:
	mkdir -p out

diskimage: prepare
	dd if=/dev/zero of=out/system.img bs=512 count=2880

bootloader: prepare
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc

stdlib: prepare
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

kernel: prepare stdlib
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

build: diskimage bootloader kernel

run:
	bochs -f src/config/if2230.config -q

build-run: build run

clean:
	rm -rf out

.PHONY: all prepare diskimage bootloader stdlib kernel build run build-run clean
