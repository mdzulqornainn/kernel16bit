#ifndef __KERNEL_H
#define __KERNEL_H

#include "std_type.h"

/* Implementasi di kernel.asm */
extern void putInMemory(int segment, int address, byte b);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
extern void makeInterrupt21();

/* Implementasi di kernel.c */
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *str);
void readString(char *buf);
void clearScreen();

#endif
