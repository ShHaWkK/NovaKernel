# NovaKernel

A minimal 64-bit kernel for x86_64 written in C and assembly. It boots with GRUB using the Multiboot2 specification and simply prints `Hello from kernel` to the screen. The project also sets up a basic GDT, IDT, paging and includes a tiny bump allocator.

Ce projet contient des commentaires en français expliquant chaque fichier et fonction principale.

## Prerequisites (Linux)

Install the following packages on a Debian based system:

```sh
sudo apt-get install build-essential grub-pc-bin xorriso qemu-system-x86 rustc cargo
```

`x86_64-elf-gcc` is optional. If it is not installed, you can build using the host toolchain by passing `CROSS=` to `make`.

## Building

Build everything and produce a bootable ISO with:

```sh
make
```

## Running with QEMU

Run the kernel in QEMU with:

```sh
make run
```

This will start QEMU and boot the generated `kernel.iso` image.

## Using Docker

If you are on Windows or prefer an isolated environment, you can build and run
the kernel inside a Docker container. First build the image:

```sh
docker build -t novakernel .
```

Then run QEMU from the container:

```sh
docker run --rm -it -v $(pwd):/src -w /src novakernel make run
```

On Windows use `%cd%` instead of `$(pwd)`.

## Structure des fichiers

- `Makefile` : script de compilation
- `boot.s` : amorce et passage en mode long
- `kernel.c` : code principal du noyau
- `interrupts.s` : stubs d'interruptions
- `linker.ld` : script de liaison
- `grub.cfg` : configuration de GRUB

## Next milestones

1. Add a simple task scheduler for multitasking.
2. Implement a filesystem driver (FAT16 or EXT2) and basic VFS layer.
3. Provide a userland shell running in ring3.
4. Start a networking stack with minimal TCP/IP support.
