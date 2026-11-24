#include "kernel.h"
#include "std_lib.h"

#define MAX_INPUT 128

/* main() WAJIB jadi fungsi pertama karena link pakai ld86 -d */
int main() {
    char buf[MAX_INPUT];

    clearScreen();
    makeInterrupt21();

    printString("Selamat datang di OS Milestone 1!\r\n");

    while (true) {
        printString("> ");
        interrupt(0x21, 0x1, (int) buf, 0, 0);   /* readString lewat syscall */
        printString("Kamu mengetik: ");
        interrupt(0x21, 0x0, (int) buf, 0, 0);   /* printString lewat syscall */
        printString("\r\n");
    }
}

/* Dipanggil dari _interrupt21ServiceRoutine di kernel.asm */
void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString((char *) BX);
            break;
        case 0x1:
            readString((char *) BX);
            break;
        default:
            printString("Invalid interrupt\r\n");
    }
}

/* INT 0x10, AH=0x0E: teletype output, cetak 1 karakter lalu cursor maju */
void printString(char *str) {
    int i;
    i = 0;
    while (str[i] != '\0') {
        interrupt(0x10, 0x0E00 + str[i], 0x0000, 0, 0);
        i++;
    }
}

/* INT 0x16, AH=0x00: tunggu 1 tombol, hasil ASCII di AL. Berhenti di Enter. */
void readString(char *buf) {
    int i;
    char c;
    i = 0;

    while (true) {
        c = interrupt(0x16, 0x0000, 0, 0, 0);

        if (c == '\r') {                      /* Enter */
            buf[i] = '\0';
            interrupt(0x10, 0x0E00 + '\r', 0, 0, 0);
            interrupt(0x10, 0x0E00 + '\n', 0, 0, 0);
            return;
        } else if (c == '\b') {               /* Backspace */
            if (i > 0) {
                i--;
                interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
                interrupt(0x10, 0x0E00 + ' ', 0, 0, 0);
                interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
            }
        } else if (i < MAX_INPUT - 1) {       /* karakter biasa, cegah overflow */
            buf[i] = c;
            i++;
            interrupt(0x10, 0x0E00 + c, 0, 0, 0);  /* echo ke layar */
        }
    }
}

/* Bersihkan layar, warna putih (0x07), cursor ke kiri atas */
void clearScreen() {
    /* INT 0x10, AH=0x06 AL=0x00: scroll up seluruh layar = clear
       BH=0x07: atribut putih di atas hitam
       CX=0x0000: pojok kiri atas, DX=0x184F: baris 24 kolom 79 */
    interrupt(0x10, 0x0600, 0x0700, 0x0000, 0x184F);

    /* INT 0x10, AH=0x02: set posisi cursor, BH=page 0, DX=0x0000 (baris 0, kolom 0) */
    interrupt(0x10, 0x0200, 0x0000, 0, 0x0000);
}
