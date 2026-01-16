#include "utils.h"

void* memcpy(void* dest, const void* src, size_t n)
{
    unsigned char* d = dest;
    const unsigned char* s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void* memset(void* s, int c, size_t n)
{
    unsigned char* p = s;
    while (n--)
        *p++ = (unsigned char)c;
    return s;
}

void reverse_string(char* str, int len)
{
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void int2string(int num, char* str)
{
    int i = 0;
    unsigned int n;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        str[i++] = '-';
        n = (unsigned int)(-num);
    } else {
        n = (unsigned int)num;
    }

    int start_digits = i;

    while (n != 0) {
        str[i++] = (n % 10) + '0';
        n /= 10;
    }

    str[i] = '\0';

    reverse_string(&str[start_digits], i - start_digits);
}
