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
        n = -(unsigned int)(num);
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

void float2string(float num, int decimals, char* str)
{
    int i = 0;

    if (num < 0) {
        str[i++] = '-';
        num = -num;
    }

    int int_part = (int)num;
    float dec_part = num - int_part;

    char buf_int[12];
    int2string(int_part, buf_int);
    int j = 0;
    while (buf_int[j] != '\0') {
        str[i++] = buf_int[j++];
    }

    str[i++] = '.';

    for (int d = 0; d < decimals; d++) {
        dec_part *= 10;
        int digit = (int)dec_part;
        str[i++] = digit + '0';
        dec_part -= digit;
    }

    str[i] = '\0';
}

int string_length(const char* str)
{
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int string_compare(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int concat_string(char* dest, size_t dest_size, const char* src)
{
    if (!dest || !src || dest_size == 0)
        return -1;

    size_t dest_len = string_length(dest);

    if (dest_len >= dest_size - 1)
        return -1;

    char* write_ptr = dest + dest_len;
    char* dest_end = dest + dest_size - 1;

    while (*src != '\0' && write_ptr < dest_end) {
        *write_ptr++ = *src++;
    }

    *write_ptr = '\0';

    if (*src != '\0')
        return -1;

    return (int)(write_ptr - dest);
}

int vsconcat_strings(char* dest, size_t dest_size, const char* first, va_list args)
{
    char* write_ptr = dest;
    char* dest_end = dest + dest_size;

    size_t len = string_length(first);

    if (write_ptr + len >= dest_end)
        return -1;

    memcpy(write_ptr, first, len);
    write_ptr += len;

    const char* next;
    while ((next = va_arg(args, const char*))) {
        size_t next_len = string_length(next);
        if (write_ptr + next_len >= dest_end)
            break;
        memcpy(write_ptr, next, next_len);
        write_ptr += next_len;
    }

    return write_ptr - dest;
}

int atoi(const char* s)
{
    int result = 0;
    int sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        s++;
    }
    return result * sign;
}
