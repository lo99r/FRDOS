ENTRY(Main)

SECTIONS {
    . = 0x0200000;

    /DISCARD/ : {
        *(.discard)
        *(.discard.*)
        *(.interp .dynamic)
        *(.dynsym .dynstr .hash .gnu.hash)
        *(.eh_frame)
    }
    
    .text ALIGN(0x1000) :   /* align at 4 KB */
    {
        *(.text)             /* all text sections from all files */
    }
    .rodata ALIGN(0x1000) :   /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN(0x1000) :   /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN(0x1000) :   /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
}
_end = .;
