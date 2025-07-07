# Makefile – compilation du noyau minimal x86_64
# Les outils utilisent le préfixe $(CROSS)
CROSS  ?= x86_64-elf-
CC     := $(CROSS)gcc      # compilateur C
AS     := $(CROSS)gcc      # assembleur
LD     := $(CROSS)ld       # éditeur de liens
OBJCOPY:= $(CROSS)objcopy  # conversion de format
CFLAGS := -ffreestanding -O2 -Wall -Wextra -std=gnu99 -m64
LDFLAGS:= -T linker.ld -nostdlib -z max-page-size=0x1000

OBJS = boot.o kernel.o interrupts.o  # fichiers objets

all: kernel.iso

boot.o: boot.s                        # amorce assembleur
	$(AS) $(CFLAGS) -c $< -o $@

kernel.o: kernel.c                    # noyau en C
	$(CC) $(CFLAGS) -c $< -o $@

interrupts.o: interrupts.s            # stubs d'interruptions
	$(AS) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS) linker.ld         # édition des liens
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf
	$(OBJCOPY) -O elf64-x86-64 kernel.elf kernel.bin

iso: kernel.bin grub.cfg              # création de l'image ISO
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso

kernel.iso: iso                       # dépend de la cible précédente

clean:                                # nettoyage
	rm -rf iso kernel.elf kernel.bin kernel.iso $(OBJS)

run: kernel.iso                       # exécution sous QEMU
	qemu-system-x86_64 -cdrom kernel.iso

.PHONY: all iso clean run
