ROSS  ?= x86_64-elf-
CC     := $(CROSS)gcc
AS     := $(CROSS)gcc
LD     := $(CROSS)ld
OBJCOPY:= $(CROSS)objcopy
CFLAGS := -ffreestanding -O2 -Wall -Wextra -std=gnu99 -m64
LDFLAGS:= -T linker.ld -nostdlib -z max-page-size=0x1000

# C sources
CSRC := $(wildcard src/*.c) $(wildcard drivers/*.c)
ASM  := boot.s src/interrupts.s
OBJS := $(CSRC:.c=.o) $(ASM:.s=.o) rust_modules/target/release/libnovarust.a

all: kernel.iso

rust_modules/target/release/libnovarust.a:
			$(MAKE) -C rust_modules release

%.o: %.s
		$(AS) $(CFLAGS) -c $< -o $@

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS) linker.ld
		$(LD) $(LDFLAGS) $(filter %.o,$^) rust_modules/target/release/libnovarust.a -o kernel.elf
		$(OBJCOPY) -O elf64-x86-64 kernel.elf kernel.bin

iso: kernel.bin grub.cfg
		mkdir -p iso/boot/grub
		cp kernel.bin iso/boot/kernel.bin
		cp grub.cfg iso/boot/grub/grub.cfg
		grub-mkrescue -o kernel.iso iso >/dev/null

kernel.iso: iso

clean:
		rm -rf iso kernel.elf kernel.bin kernel.iso $(CSRC:.c=.o) $(ASM:.s=.o) rust_modules/target

run: kernel.iso
		qemu-system-x86_64 -serial stdio -cdrom kernel.iso

.PHONY: all iso clean run