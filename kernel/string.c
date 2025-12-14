#include "string.h"

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, unsigned int n) {
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    if (n == (unsigned int)-1) return 0;
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strlen(const char *s) {
    int len = 0;
    while (*s++) len++;
    return len;
}