; kernel.asm - fungsi assembly yang dipanggil dari kernel.c (dikompilasi: nasm -f as86)
global _putInMemory
global _interrupt
global _makeInterrupt21
extern _handleInterrupt21

; void putInMemory(int segment, int address, byte b)
_putInMemory:
    push bp
    mov bp, sp
    push ds
    mov ax, [bp+4]      ; segment
    mov si, [bp+6]      ; address
    mov cl, [bp+8]      ; byte
    mov ds, ax
    mov [si], cl
    pop ds
    pop bp
    ret

; int interrupt(int number, int AX, int BX, int CX, int DX)
_interrupt:
    push bp
    mov bp, sp
    mov ax, [bp+4]      ; nomor interrupt
    push ds
    mov bx, cs
    mov ds, bx
    mov si, intr
    mov [si+1], al      ; tulis nomor interrupt ke instruksi "int"
    pop ds
    mov ax, [bp+6]
    mov bx, [bp+8]
    mov cx, [bp+10]
    mov dx, [bp+12]
intr:
    int 0x00            ; diganti saat runtime
    mov ah, 0           ; return AL saja
    pop bp
    ret

; void makeInterrupt21() - pasang handler INT 0x21 di interrupt vector table
_makeInterrupt21:
    mov dx, _interrupt21ServiceRoutine
    push ds
    mov ax, 0
    mov ds, ax
    mov si, 0x84        ; 0x21 * 4
    mov ax, cs
    mov [si+2], ax      ; segment
    mov [si], dx        ; offset
    pop ds
    ret

_interrupt21ServiceRoutine:
    push dx
    push cx
    push bx
    push ax
    call _handleInterrupt21
    pop ax
    pop bx
    pop cx
    pop dx
    iret
