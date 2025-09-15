RAMBASE    = 0x81000000;

MEMORY {
    extram(ARWX) : ORIGIN = RAMBASE, LENGTH = 64M
}

SECTIONS {
    .init_array : {
        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;
        . += 4; /* Padding to ensure this section exists. */
    }
    /* .fini_array : {
        __fini_array_start = .;
        KEEP(*(.fini_array))
        __fini_array_end = .;
        . += 4;
    } */
    /* All of the sections needed will get passed through to output sections automatically. */
    /* Therefore, all that's needed in the sections command is the discards. */

    /DISCARD/ : { *(.got) *(.MIPS.abiflags) *(.reginfo) *(.pdr) *(.comment) }
}
