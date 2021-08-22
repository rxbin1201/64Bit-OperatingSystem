section .multibootHeader

headerStart:
    dd 0xe85250d6
    dd 0
    dd headerEnd - headerStart
    dd 0x100000000 - (0xe85250d6 + 0 + (headerEnd - headerStart))

    dw 0
    dw 0
    dd 8

headerEnd: