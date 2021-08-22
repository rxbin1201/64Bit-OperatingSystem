ENTRY(start)

SECTIONS
{
    . = 1M;

    .boot :
    {
        KEEP(*(.multibootHeader))
    }

    .text :
    {
        *(.text)
    }
}