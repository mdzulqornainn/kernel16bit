#include "std_lib.h"

int div(int a, int b) {
    int q;
    bool neg;
    q = 0;
    neg = false;
    if (b == 0) return 0;
    if (a < 0) { a = -a; neg = !neg; }
    if (b < 0) { b = -b; neg = !neg; }
    while (a >= b) {
        a = a - b;
        q++;
    }
    return neg ? -q : q;
}

int mod(int a, int b) {
    bool neg;
    neg = false;
    if (b == 0) return 0;
    if (a < 0) { a = -a; neg = true; }
    if (b < 0) b = -b;
    while (a >= b) a = a - b;
    return neg ? -a : a;
}

void memcpy(byte *dest, byte *src, unsigned int n) {
    unsigned int i;
    for (i = 0; i < n; i++) dest[i] = src[i];
}

unsigned int strlen(char *str) {
    unsigned int i;
    i = 0;
    while (str[i] != '\0') i++;
    return i;
}

bool strcmp(char *str1, char *str2) {
    unsigned int i;
    i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}

void strcpy(char *dest, char *src) {
    unsigned int i;
    i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void clear(byte *buf, unsigned int n) {
    unsigned int i;
    for (i = 0; i < n; i++) buf[i] = 0;
}
