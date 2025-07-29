CROSS  ?= x86_64-elf-
CC     := $(CROSS)gcc
AS     := $(CROSS)gcc
LD     := $(CROSS)ld
OBJCOPY:= $(CROSS)objcopy
# Avoid SSE usage in interrupt routines
CFLAGS := -ffreestanding -O2 -Wall -Wextra -std=gnu99 -m64 -mgeneral-regs-only
LDFLAGS:= -T linker.ld -nostdlib -z max-page-size=0x1000

# C sources
CSRC := $(wildcard src/*.c) $(wildcard drivers/*.c)
ASM  := boot.s interrupts.s
RUST_LIB := rust_modules/target/x86_64-unknown-linux-gnu/release/libnovarust.a
OBJS := $(CSRC:.c=.o) $(ASM:.s=.o) $(RUST_LIB)

all: kernel.iso

$(RUST_LIB):
	$(MAKE) -C rust_modules release

%.o: %.s
		$(AS) $(CFLAGS) -c $< -o $@

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(filter %.o,$^) $(RUST_LIB) -o kernel.elf
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