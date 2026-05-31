MEMORY {
    RAM (rwx) : ORIGIN = 0x00082000, LENGTH = 1M - 0x2000
}

SECTIONS {
    /* Garante que o início da seção .text é alinhado */
    . = ALIGN(2);

    .text : {
        /* Força alinhamento do início da seção */
        . = ALIGN(2);
        *(.text*)
        *(.init)
        /* Força alinhamento do final da seção */
        . = ALIGN(2);
    } > RAM

    .rodata : {
        . = ALIGN(2);
        *(.rodata*)
        . = ALIGN(2);
    } > RAM

    .data : {
        . = ALIGN(2);
        *(.data*)
        . = ALIGN(2);
    } > RAM

    .bss (NOLOAD) : {
        . = ALIGN(2);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(2);
        __bss_end = .;
    } > RAM

    .stack (NOLOAD) : {
        . = ALIGN(2);  /* Mudei de 4 para 2 para consistência */
        _stack_start = .;
        . += 0x1000;
        . = ALIGN(2);
        _stack_end = .;
    } > RAM

    /DISCARD/ : {
        *(.comment)
        *(.note*)
        /* Descarta também seções de alinhamento excessivo */
        *(.align*)
    }
}
