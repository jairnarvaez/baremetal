#include "utils.h"
#include <stdint.h>

void* memcpy(void* dest, const void* src, size_t n)
{
    uint32_t* dest_pointer32 = dest;
    const uint32_t* src_pointer32 = src;
    size_t temp = n >> 2;

    while (temp--) {
        *dest_pointer32++ = *src_pointer32++;
    }

    size_t leftover = n % 4;
    uint8_t* dest_ponter8 = (uint8_t*)dest_pointer32;
    const uint8_t* src_pointer8 = (const uint8_t*)src_pointer32;
    while (leftover--) {
        *dest_ponter8++ = *src_pointer8++;
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

int string_length(const char* str)
{
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
