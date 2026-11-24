; bootloader.asm - load kernel dari disk ke memori 0x1000:0000 lalu lompat ke kernel
bits 16

KERNEL_SEGMENT equ 0x1000   ; kernel diletakkan di alamat fisik 0x10000
KERNEL_SECTORS equ 15       ; jumlah sektor kernel yang dibaca
KERNEL_START   equ 1        ; kernel mulai di sektor 1 (setelah bootloader)

bootloader:
    ; baca kernel dari floppy pakai INT 0x13
    mov ax, KERNEL_SEGMENT
    mov es, ax
    mov bx, 0x0000          ; ES:BX = tujuan
    mov ah, 0x02            ; fungsi read sector
    mov al, KERNEL_SECTORS  ; jumlah sektor
    mov ch, 0               ; cylinder 0
    mov cl, KERNEL_START+1  ; sektor (mulai dari 1, jadi +1)
    mov dh, 0               ; head 0
    mov dl, 0x00            ; drive A (floppy)
    int 0x13

    ; set segment register untuk kernel
    mov ax, KERNEL_SEGMENT
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ax, 0xFFF0
    mov sp, ax
    mov bp, ax

    jmp KERNEL_SEGMENT:0x0000

    times 510-($-$$) db 0
    dw 0xAA55               ; boot signature
