/* linker.ld - script de liaison du noyau */
ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

SECTIONS {
    . = 1M;                      /* adresse de chargement */
    .multiboot : {
        *(.multiboot)
    }
    .text : ALIGN(4K) {
        *(.text*)
    }
    .rodata : ALIGN(4K) {
        *(.rodata*)
    }
    .data : ALIGN(4K) {
        *(.data*)
    }
    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss*)
    }
    /DISCARD/ : { *(.eh_frame) *(.note.GNU-stack) }
}

PROVIDE(kernel_end = .);          /* fin du noyau pour l'allocateur */
