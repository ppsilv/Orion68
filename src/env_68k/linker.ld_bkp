MEMORY {
    RAM (rwx) : ORIGIN = 0x00082000, LENGTH = 1M - 0x2000  /* 0x82000 para 1MB */
}

SECTIONS {
    .text : {
        *(.text*)
        *(.init)
    } > RAM

    .rodata : {
        *(.rodata*)
    } > RAM

    .data : {
        *(.data*)
    } > RAM

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    .stack (NOLOAD) : {
        . = ALIGN(4);
        _stack_start = .;
        . += 0x1000; /* 4KB stack */
        _stack_end = .;
    } > RAM

    /DISCARD/ : {
        *(.comment)
        *(.note*)
    }
}
