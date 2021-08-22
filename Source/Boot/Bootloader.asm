global start
extern callKernel

section .text

BITS 32

start:
    mov esp, stackTop

    call setupPageTables
    call enablePaging

    lgdt [GDT64.Pointer]
    jmp GDT64.codeSegment:callKernel

    hlt

setupPageTables:
    mov eax, pageTableL3
    or eax, 0b11
    mov [pageTableL4], eax

    mov eax, pageTableL2
    or eax, 0b11
    mov [pageTableL3], eax

    mov ecx, 0

.loop:
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [pageTableL2 + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .loop

    ret

enablePaging:
    mov eax, pageTableL4
    mov cr3, eax

    mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

    mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

    mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

section .bss
align 4096
    
pageTableL4:
    resb 4096

pageTableL3:
    resb 4096

pageTableL2:
    resb 4096

stackBottom:
    resb 4096 * 4

stackTop:

section .rodata

GDT64:
    dq 0

.codeSegment: equ $ - GDT64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)

.Pointer:
    dw $ - GDT64 - 1
    dq GDT64
