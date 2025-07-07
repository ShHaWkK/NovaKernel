/* interrupts.s - stubs d'interruptions et chargeurs de tables */
.global isr0
.global irq1
.global load_idt
.global load_gdt

/* Charge l'IDT depuis l'adresse fournie */
load_idt:
    lidt (%rdi)
    ret

/* Charge la GDT depuis l'adresse fournie */
load_gdt:
    lgdt (%rdi)
    ret

/* Routine commune appelée par les stubs */
.isr_common:
    pushq %rax                    # sauvegarde des registres
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    mov %rsp, %rdi                # adresse de la stack pour le C
    call *%rax                    # appel du handler en C
    popq %r11                     # restauration
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
    add $16, %rsp                 # retire errcode et rip
    iretq

isr0:                                   # division par zéro
    mov $isr0_handler, %rax
    sub $16, %rsp       /* espace pour errcode et rip */
    jmp .isr_common

irq1:                                   # interruption clavier
    mov $irq1_handler, %rax
    sub $16, %rsp
    jmp .isr_common
