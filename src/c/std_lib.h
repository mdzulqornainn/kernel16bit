#ifndef __STD_LIB_H
#define __STD_LIB_H

#include "std_type.h"

/* bcc tanpa libc tidak bisa pakai / dan % (butuh helper libc), jadi dibuat manual */
int div(int a, int b);
int mod(int a, int b);

void memcpy(byte *dest, byte *src, unsigned int n);
unsigned int strlen(char *str);
bool strcmp(char *str1, char *str2);   /* true jika SAMA */
void strcpy(char *dest, char *src);
void clear(byte *buf, unsigned int n);

#endif
