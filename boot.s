/* boot.s -  Multiboot2 et passage en mode long */

.set MAGIC,        0xE85250D6
.set ARCH,         0
.set HEADER_LEN,   header_end - header_start
.set CHECKSUM,     -(MAGIC + ARCH + HEADER_LEN)

.section .multiboot               # définition de l'en-tête Multiboot2
header_start:
    .long MAGIC
    .long ARCH
    .long HEADER_LEN
    .long CHECKSUM
    /* End tag */
    .word 0
    .word 0
    .long 8
header_end:

/* Pile temporaire pour le démarrage */
.section .bss
    .align 16
stack_bottom:
    .skip 0x4000
stack_top:

.section .text
.global _start
.global gdt_descriptor             # exporté pour le C
.code32
_start:
    mov $stack_top, %esp
    call enter_long_mode          # active le mode long

/* Switch to long mode and jump to 64-bit C kernel */
enter_long_mode:
    cli                           # pas d'interruptions durant le setup
    /* Load GDT */
    lgdt gdt_descriptor           # charge la GDT 64 bits

    mov $0x10, %ax                # sélecteur du segment de données
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss

    /* Enable PAE */
    mov %cr4, %eax
    or $0x20, %eax                # active PAE
    mov %eax, %cr4

    /* Load PML4 */
    mov $pml4_table, %eax         # adresse de la table PML4
    mov %eax, %cr3

    /* Enable long mode in EFER */
    mov $0xC0000080, %ecx
    rdmsr
    or $0x00000100, %eax          # bit LME (Long Mode Enable)
    wrmsr

    /* Enable paging */
    mov %cr0, %eax
    or $0x80000001, %eax          # active le paging et protégé
    mov %eax, %cr0

    /* Far jump to flush pipeline */
    jmp $0x08, $long_mode_entry   # saut loin pour passer en 64 bits

/* 64-bit GDT */
.align 16
gdt64:
    .quad 0x0000000000000000  /* entrée nulle */
    .quad 0x00af9a000000ffff  /* segment code */
    .quad 0x00af92000000ffff  /* segment données */

gdt_descriptor:
    .word gdt64_end - gdt64 - 1
    .long gdt64
    .long 0

gdt64_end:

/* Tables de pages pour mapper identiquement les premiers 2 MiB */
.align 4096
pml4_table:
    .quad pdpt_table + 0x3
    .fill 511,8,0

.align 4096
pdpt_table:
    .quad pd_table + 0x3
    .fill 511,8,0

.align 4096
pd_table:
    .quad 0x00000000 + 0x83  /* page de 2 MiB */
    .fill 511,8,0

.code64
long_mode_entry:
    mov $stack_top, %rsp         # met en place la pile 64 bits
    mov %rbx, %rdi               # pointeur Multiboot passé au C
    .extern kernel_main
    call kernel_main             # transfert au noyau C

.hang:                              # boucle infinie en cas de retour
    hlt
    jmp .hang
