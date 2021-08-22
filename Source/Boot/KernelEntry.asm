global callKernel
extern main

section .text

BITS 64

callKernel:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	call main
    
    hlt